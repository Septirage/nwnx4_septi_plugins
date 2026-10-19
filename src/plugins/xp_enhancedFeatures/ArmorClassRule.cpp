
#include <misc/log.h>
#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include <misc/Patch.h>
#include "../../septutil/mini.h"

#include <string>
#include <vector>
#include <sstream>
#include <cstring>
#include <stdexcept>

#include "RuleParser.h"
#include "BonusParser.h"

extern std::unique_ptr<LogNWNX> logger;

//The engine computes the AC in 3 places:
// - character sheet: CalculateAndReturnAC (0x5e01d0), from the AC fields of the stats
// - DM stats display (ProcessShowStatsForCreature 0x6b9a80), same sum written in the function itself
// - combat: CalculateACStuff (0x5b52b0, thiscall stats, attacker, touch ; ret 8). From the AC fields of the stats, then
//   the "versus" effects matching the attacker can replace them (best bonus, worst penalty). Touch: no natural/armor/shield.
//Each AC type (natural, armor, shield, deflection) keeps the best bonus and the worst penalty, the AC gets bonus - penalty.
//The dodge bonuses and penalties stack.
//The 3 use the same functions for the dodge feats, the innate feat AC and Canny Defense: the stacking types are added
//at the start of these functions. The non stacking types are added to each of the 3 sums.

//CalculateDogeBonusFeat (fastcall ECX = stats): dodge feats. The callers add the dodge effects and cap the total at 20.
//Called by the sheet, the DM display and combat (CalculateSomethingRelatedToDodge, only when the Dex is not denied)
#define OFFS_CalculateDodgeBonusFeat		0x007541a0		//push esi / push edi / xor edi,edi / push edi (5 bytes)
unsigned long ReturnDodgeBonusFeatBody		= 0x007541a5;

//CalculateFeatAC (thiscall stats, touch ; ret 4): hardcoded feat AC ("Innate AC"), added to the total. Its first block
//(Pale Master, Dragon Disciple...) is skipped on a touch attack. Called by CalculateACInate (sheet: touch = 0, combat:
//touch given) and by the DM display
#define OFFS_CalculateFeatAC				0x00754810		//push esi / push edi / xor edi,edi / cmp [esp+0xC],edi (8 bytes)
unsigned long ReturnFeatACBody				= 0x00754818;

//CalculateCanny_Unfettered_Defense (thiscall stats, Dex denied ; ret 4): 0 when the Dex is denied. Added to the total
//by the sheet (last value), the DM display and combat
#define OFFS_CalculateCannyDefense			0x005b5180		//push ebx / xor ebx,ebx / cmp [esp+8],ebx (7 bytes)
unsigned long ReturnCannyDefenseBody		= 0x005b5187;

//CalculateAndReturnAC (fastcall ECX = creature): AC of the character sheet (also used by aspectManager)
#define OFFS_CalculateAndReturnAC			0x005e01d0		//push ecx / push esi / mov esi,ecx / push edi (5 bytes)
unsigned long ReturnCalculateAndReturnACBody	= 0x005e01d5;

//ProcessShowStatsForCreature (DM display) writes its own sum: its call to CalculateCanny_Unfettered_Defense
//(added to the "Class&Race" column) also adds the non stacking types
#define OFFS_DMStatsCannyCall				0x006b9fbb

//End of CalculateACStuff: cmp [esp+0x78],0 (touch, 5 bytes), then + the natural/armor/shield part when not touch.
//Reached after the "versus" effects, also by the debug path (0x5b62c0)
#define OFFS_CombatACTail					0x005b62ce
unsigned long ReturnCombatACTail			= 0x005b62d3;

//Stack of CalculateACStuff at its end (the shield bonus is in EDI)
#define FRAME_ShieldMalus					0x1C
#define FRAME_ArmorMalus					0x20
#define FRAME_NaturalMalus					0x24
#define FRAME_ArmorBonus					0x28
#define FRAME_NaturalBonus					0x2C
#define FRAME_DeflectionBonus				0x30
#define FRAME_DeflectionMalus				0x34
#define FRAME_EffectList					0x4C		//Creature + 0x1A0
#define FRAME_Touch							0x78

//AC fields of the stats (int8): best effect bonus / worst effect penalty of each type
#define STATS_ArmorBonus					0x59F
#define STATS_ArmorMalus					0x5A0
#define STATS_DeflectionBonus				0x5A1
#define STATS_DeflectionMalus				0x5A2
#define STATS_ShieldBonus					0x5A3
#define STATS_ShieldMalus					0x5A4
#define STATS_NaturalBonus					0x5A5
#define STATS_NaturalMalus					0x5A6


enum ACRuleType {
	AC_RULE_DODGE = 0,
	AC_RULE_NATURAL,
	AC_RULE_ARMOR,
	AC_RULE_SHIELD,
	AC_RULE_DEFLECTION,
	AC_RULE_UNTYPED,
	AC_RULE_INNATE,
	AC_RULE_TYPE_COUNT
};

#define AC_MASK(type)						(1 << (type))
//Types lost on a touch attack
#define AC_MASK_NOT_TOUCH					(AC_MASK(AC_RULE_NATURAL) | AC_MASK(AC_RULE_ARMOR) | AC_MASK(AC_RULE_SHIELD))


struct ACRuleSet {
	RuleParser::Rule* featRule;
	RuleParser::Rule* areaTypeRule;
	RuleParser::Rule* extraRule;
	MathExpressionParser::Expr* bonusCalculation;
	int index;
	int acType;		// ACRuleType


	ACRuleSet(MathExpressionParser::Expr* bonus, int idx)
	    : bonusCalculation(bonus)
	    , index(idx)
	    , acType(AC_RULE_UNTYPED)
	{
		featRule = nullptr;
		areaTypeRule = nullptr;
		extraRule    = nullptr;
	}

	~ACRuleSet()
	{
		delete bonusCalculation;
		delete areaTypeRule;
		delete extraRule;
		delete featRule;
	}
};

//Result of the rules, by type
struct ACRuleValues {
	int iSum[AC_RULE_TYPE_COUNT];			//Stacking types (dodge, untyped, innate)
	int iBestBonus[AC_RULE_TYPE_COUNT];		//Non stacking types: best positive value
	int iWorstMalus[AC_RULE_TYPE_COUNT];	//Non stacking types: worst negative value, as a positive number
};


std::vector<ACRuleSet*> m_armorClassRuleSetList;


static bool ParseACRuleCondition(std::string sRuleX, std::string sValue, RuleParser::RuleType tFeatRule, std::vector<ACRuleSet*>& myRuleList)
{
	try {
		std::vector<RuleParser::Token> tokens1 = RuleParser::tokenize(sValue, tFeatRule);
		int pos1                   = 0;
		RuleParser::Rule* root1                = RuleParser::parseExpression(tokens1, pos1);
		if (tFeatRule == RuleParser::RuleType::FEAT)
			myRuleList.back()->featRule = root1;
		else if (tFeatRule == RuleParser::RuleType::AREA)
			myRuleList.back()->areaTypeRule = root1;
		else
			myRuleList.back()->extraRule = root1;

	} catch (const std::exception& e) {
		std::string sRuleType;

		if (tFeatRule == RuleParser::RuleType::FEAT)
			sRuleType = "Feat";
		else if (tFeatRule == RuleParser::RuleType::AREA)
			sRuleType = "Area";
		else
			sRuleType = "Extra";
		logger->Err("Error during parse of [%s] \"%s\" = %s : %s", sRuleX.c_str(),
		            sRuleType.c_str(), sValue.c_str(), e.what());
		logger->Info("Due to the Error, the complete rule will not be applied");
		return false;
	}

	return true;
}


static bool ParseACModifier(int iRuleNumber, std::string sValue, std::vector<ACRuleSet*>& myRuleList, std::string sField)
{
	try {
		auto bonusCalc = MathExpressionParser::prepareSimplified(sValue, MathExpressionParser::Mode::INTEGER);
		ACRuleSet* rs1 = new ACRuleSet(bonusCalc, iRuleNumber);

		myRuleList.push_back(rs1);
	} catch (const std::exception& e) {

		logger->Err("Error during parse of \"%s\" = %s : %s", sField.c_str(), sValue.c_str(), e.what());
		logger->Info("Due to the Error, the complete rule will not be applied");
		return false;
	}

	return true;
}


//"DODGE", "NATURAL", "ARMOR", "SHIELD", "DEFLECTION", "UNTYPED", "INNATE". -1 if unknown
static int ParseACType(std::string sValue)
{
	static const char* acTypeNames[AC_RULE_TYPE_COUNT] = {
		"DODGE", "NATURAL", "ARMOR", "SHIELD", "DEFLECTION", "UNTYPED", "INNATE"
	};

	std::string sWord;
	std::istringstream iss(sValue);
	if (!(iss >> sWord))
		return -1;

	for (char& c : sWord)
		c = (char)toupper((unsigned char)c);

	for (int i = 0; i < AC_RULE_TYPE_COUNT; i++)
	{
		if (sWord == acTypeNames[i])
			return i;
	}

	return -1;
}


//Rules of the types in uTypeMask. As in the other rule files, a rule reference (Rule2, !Rule2) only sees the rules
//of the type being computed: each type is evaluated on its own.
static void armorClassRuleApplication(int pcBlock, uint32_t uTypeMask, ACRuleValues& values)
{
	int areaType = 0;

	memset(&values, 0, sizeof(values));

	if (m_armorClassRuleSetList.empty())
		return;

	int pcPtr = *(int*)(pcBlock + 0xA4);

	//GetAreaType once for all
	{
		GameObject* Object;
		GameObjectManager m_ObjectManager;
		NWN::OBJECTID areaID = *(NWN::OBJECTID*)(pcPtr + 0x144);

		if ((areaID & NWN::INVALIDOBJID) != 0)
			areaID &= ~(NWN::LISTTYPE_MASK);

		if ((Object = m_ObjectManager.GetGameObject(areaID)) != NULL) {
			if (Object->GetObjectType() == NWN::OBJECT_TYPE_AREA) {
				char* areaPtr = (char*)Object->AsArea();

				areaType = *(uint32_t*)(areaPtr + 0x8);
			}
		}
	}

	for (int iType = 0; iType < AC_RULE_TYPE_COUNT; iType++)
	{
		if ((uTypeMask & AC_MASK(iType)) == 0)
			continue;

		RuleParser::m_activatedRuleSet.clear();

		for (ACRuleSet* myRule : m_armorClassRuleSetList)
		{
			if (myRule->acType != iType)
				continue;

			bool bOk = true;
			if (myRule->featRule != nullptr)
			{
				bOk = RuleParser::evaluateRule(myRule->featRule, RuleParser::RuleType::FEAT, pcBlock);
			}

			if (bOk && myRule->areaTypeRule != nullptr)
			{
				bOk = RuleParser::evaluateRule(myRule->areaTypeRule, RuleParser::RuleType::AREA, areaType);
			}

			if (bOk && myRule->extraRule != nullptr)
			{
				bOk = RuleParser::evaluateRule(myRule->extraRule, RuleParser::RuleType::EXTRA, pcPtr);
			}

			if (bOk)
			{
				RuleParser::m_activatedRuleSet.insert(myRule->index);

				int iValue = myRule->bonusCalculation->evaluateInt(pcBlock);

				values.iSum[iType] += iValue;
				if (iValue > values.iBestBonus[iType])
					values.iBestBonus[iType] = iValue;
				if (-iValue > values.iWorstMalus[iType])
					values.iWorstMalus[iType] = -iValue;
			}
		}
	}

	RuleParser::m_activatedRuleSet.clear();
}


//Non stacking type: a rule bonus only counts when better than the engine bonus, a rule penalty only when worse than
//the engine penalty. Returns what the rules add to the AC.
static int NonStackingDelta(const ACRuleValues& values, int iType, int iEngineBonus, int iEngineMalus)
{
	int iDelta = 0;

	if (values.iBestBonus[iType] > iEngineBonus)
		iDelta += values.iBestBonus[iType] - iEngineBonus;
	if (values.iWorstMalus[iType] > iEngineMalus)
		iDelta -= values.iWorstMalus[iType] - iEngineMalus;

	return iDelta;
}

//Sheet and DM display: what the natural/armor/shield/deflection rules add, against the AC fields of the stats
static int StatsNonStackingDelta(int pcBlock)
{
	ACRuleValues values;
	armorClassRuleApplication(pcBlock, AC_MASK(AC_RULE_DEFLECTION) | AC_MASK_NOT_TOUCH, values);

	int iDelta = NonStackingDelta(values, AC_RULE_NATURAL, *(int8_t*)(pcBlock + STATS_NaturalBonus), *(int8_t*)(pcBlock + STATS_NaturalMalus));
	iDelta += NonStackingDelta(values, AC_RULE_ARMOR, *(int8_t*)(pcBlock + STATS_ArmorBonus), *(int8_t*)(pcBlock + STATS_ArmorMalus));
	iDelta += NonStackingDelta(values, AC_RULE_SHIELD, *(int8_t*)(pcBlock + STATS_ShieldBonus), *(int8_t*)(pcBlock + STATS_ShieldMalus));
	iDelta += NonStackingDelta(values, AC_RULE_DEFLECTION, *(int8_t*)(pcBlock + STATS_DeflectionBonus), *(int8_t*)(pcBlock + STATS_DeflectionMalus));

	return iDelta;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Original functions: the overwritten first instructions, then the rest of the engine function.
// Called as the engine does, they return to the caller as the engine function does.
//////////////////////////////////////////////////////////////////////////////////////////////////////////

__declspec(naked) int __fastcall CalculateDodgeBonusFeatOriginal(void* pStats)
{
	__asm
	{
		push	esi
		push	edi
		xor		edi, edi
		push	edi
		jmp		dword ptr [ReturnDodgeBonusFeatBody]
	}
}

__declspec(naked) int __fastcall CalculateFeatACOriginal(void* pStats, void* Unused, int bTouch)
{
	__asm
	{
		push	esi
		push	edi
		xor		edi, edi
		cmp		dword ptr [esp + 0xC], edi
		jmp		dword ptr [ReturnFeatACBody]
	}
}

__declspec(naked) int __fastcall CalculateCannyDefenseOriginal(void* pStats, void* Unused, int bDexDenied)
{
	__asm
	{
		push	ebx
		xor		ebx, ebx
		cmp		dword ptr [esp + 8], ebx
		jmp		dword ptr [ReturnCannyDefenseBody]
	}
}

__declspec(naked) int __fastcall CalculateAndReturnACOriginal(void* pCreature)
{
	__asm
	{
		push	ecx
		push	esi
		mov		esi, ecx
		push	edi
		jmp		dword ptr [ReturnCalculateAndReturnACBody]
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Hooks at the start of the engine functions. A __fastcall (with an unused EDX when the engine function has stack
// parameters) has the same convention as the engine function.
//////////////////////////////////////////////////////////////////////////////////////////////////////////

//Dodge: added to the dodge feats, so the engine adds the dodge effects, caps at 20 and drops it all when the Dex is denied
int __fastcall DodgeBonusFeatHook(void* pStats)
{
	ACRuleValues values;
	armorClassRuleApplication((int)pStats, AC_MASK(AC_RULE_DODGE), values);

	return CalculateDodgeBonusFeatOriginal(pStats) + values.iSum[AC_RULE_DODGE];
}

//Innate: added to the feat AC, only when not a touch attack (as its first block)
int __fastcall FeatACHook(void* pStats, void* Unused, int bTouch)
{
	int iAC = CalculateFeatACOriginal(pStats, NULL, bTouch);
	if (bTouch != 0)
		return iAC;

	ACRuleValues values;
	armorClassRuleApplication((int)pStats, AC_MASK(AC_RULE_INNATE), values);

	return iAC + values.iSum[AC_RULE_INNATE];
}

//Untyped: added to Canny Defense, whatever the denied Dex
int __fastcall CannyDefenseHook(void* pStats, void* Unused, int bDexDenied)
{
	ACRuleValues values;
	armorClassRuleApplication((int)pStats, AC_MASK(AC_RULE_UNTYPED), values);

	return CalculateCannyDefenseOriginal(pStats, NULL, bDexDenied) + values.iSum[AC_RULE_UNTYPED];
}

//Character sheet: + the natural/armor/shield/deflection rules
int __fastcall CalculateAndReturnACHook(void* pCreature)
{
	int iAC = CalculateAndReturnACOriginal(pCreature);

	int pcBlock = *(int*)((int)pCreature + 0x1FC4);
	if (pcBlock == 0)
		return iAC;

	return iAC + StatsNonStackingDelta(pcBlock);
}

//DM display: replaces its call to CalculateCanny_Unfettered_Defense (thiscall: ECX = stats, 1 stack parameter, ret 4).
//Canny Defense with the untyped rules, + the natural/armor/shield/deflection rules
int __fastcall DMStatsCannyDefenseHook(void* pStats, void* Unused, int bDexDenied)
{
	return CannyDefenseHook(pStats, NULL, bDexDenied) + StatsNonStackingDelta((int)pStats);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Combat: natural/armor/shield/deflection rules at the end of CalculateACStuff, against the values of the
// engine after the "versus" effects. Natural/armor/shield skipped on a touch attack, as the engine does.
//////////////////////////////////////////////////////////////////////////////////////////////////////////

int __stdcall CombatACTypedRules(uint8_t* pFrame, int iShieldBonus)
{
	int pCreature = *(int*)(pFrame + FRAME_EffectList) - 0x1A0;
	int pcBlock = *(int*)(pCreature + 0x1FC4);
	if (pcBlock == 0)
		return 0;

	bool bTouch = (*(int*)(pFrame + FRAME_Touch) != 0);

	uint32_t uTypeMask = AC_MASK(AC_RULE_DEFLECTION);
	if (!bTouch)
		uTypeMask |= AC_MASK_NOT_TOUCH;

	ACRuleValues values;
	armorClassRuleApplication(pcBlock, uTypeMask, values);

	int iDelta = NonStackingDelta(values, AC_RULE_DEFLECTION, *(int*)(pFrame + FRAME_DeflectionBonus), *(int*)(pFrame + FRAME_DeflectionMalus));
	if (!bTouch)
	{
		iDelta += NonStackingDelta(values, AC_RULE_NATURAL, *(int*)(pFrame + FRAME_NaturalBonus), *(int*)(pFrame + FRAME_NaturalMalus));
		iDelta += NonStackingDelta(values, AC_RULE_ARMOR, *(int*)(pFrame + FRAME_ArmorBonus), *(int*)(pFrame + FRAME_ArmorMalus));
		iDelta += NonStackingDelta(values, AC_RULE_SHIELD, iShieldBonus, *(int*)(pFrame + FRAME_ShieldMalus));
	}

	return iDelta;
}

//Jumped to from the end of CalculateACStuff. EAX = AC without the natural/armor/shield part, EDI = shield bonus,
//ESP = stack of CalculateACStuff. ECX and EDX are free.
__declspec(naked) void CombatACTailHook()
{
	__asm
	{
		push	eax						//AC so far
		push	edi						//Shield bonus, used by the end of the engine function
		lea		ecx, [esp + 8]			//Stack of CalculateACStuff
		push	edi
		push	ecx
		call	CombatACTypedRules		//stdcall
		pop		edi
		add		eax, [esp]
		add		esp, 4

		//Overwritten instruction, then the end of the engine function
		cmp		dword ptr [esp + 0x78], 0
		jmp		dword ptr [ReturnCombatACTail]
	}
}


Patch _ArmorClassRulePatches[] = {
	Patch((DWORD)OFFS_CalculateDodgeBonusFeat, (char*)"\xe9\x00\x00\x00\x00", (int)5),
	Patch(OFFS_CalculateDodgeBonusFeat + 1, (relativefunc)DodgeBonusFeatHook),

	Patch((DWORD)OFFS_CalculateFeatAC, (char*)"\xe9\x00\x00\x00\x00\x90\x90\x90", (int)8),
	Patch(OFFS_CalculateFeatAC + 1, (relativefunc)FeatACHook),

	Patch((DWORD)OFFS_CalculateCannyDefense, (char*)"\xe9\x00\x00\x00\x00\x90\x90", (int)7),
	Patch(OFFS_CalculateCannyDefense + 1, (relativefunc)CannyDefenseHook),

	Patch((DWORD)OFFS_CalculateAndReturnAC, (char*)"\xe9\x00\x00\x00\x00", (int)5),
	Patch(OFFS_CalculateAndReturnAC + 1, (relativefunc)CalculateAndReturnACHook),

	Patch(OFFS_DMStatsCannyCall + 1, (relativefunc)DMStatsCannyDefenseHook),

	Patch((DWORD)OFFS_CombatACTail, (char*)"\xe9\x00\x00\x00\x00", (int)5),
	Patch(OFFS_CombatACTail + 1, (relativefunc)CombatACTailHook),

	Patch()
};

Patch *ArmorClassRulePatches = _ArmorClassRulePatches;

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool isArmorClassAlreadyPatched = false;

void desinitArmorClassRule()
{
	for (auto ruleSet : m_armorClassRuleSetList) {
		delete ruleSet;
	}
	m_armorClassRuleSetList.clear();
}

void initArmorClassRule(std::string nxhome, std::string sFileName)
{
	std::string inifile(nxhome);
	inifile.append("\\");
	inifile.append(sFileName);

	mINI::INIFile file(inifile);
	mINI::INIStructure ini;

	bool bOkRead = file.read(ini);

	if (!bOkRead)
		logger->Err("Cant find ArmorClassFile : %s", sFileName.c_str());

	if (bOkRead && ini.has("General"))
	{
		if (ini.get("General").has("DisableHook"))
		{
			std::string sDisableHook = ini.get("General").get("DisableHook");

			bOkRead = !(sDisableHook == "1");
		}
	}

	if (bOkRead)
	{
		int iRuleNumber = 1;
		std::string sACRuleBase = "ACRule";

		std::string sACRuleX = sACRuleBase + std::to_string(iRuleNumber);

		while (ini.has(sACRuleX))
		{
			auto section = ini.get(sACRuleX);

			if (!section.has("Bonus"))
			{
				logger->Err("[ArmorClassFile]: No Bonus field for [%s].", sACRuleX.c_str());
			}
			else if (!section.has("Type"))
			{
				logger->Err("[ArmorClassFile]: No Type field for [%s].", sACRuleX.c_str());
			}
			else
			{
				int iACType = ParseACType(section.get("Type"));

				if (iACType < 0)
				{
					logger->Err("[ArmorClassFile]: Bad Type field for [%s] : %s (DODGE, NATURAL, ARMOR, SHIELD, DEFLECTION, UNTYPED or INNATE).", sACRuleX.c_str(), section.get("Type").c_str());
				}
				else if (ParseACModifier(iRuleNumber, section.get("Bonus"), m_armorClassRuleSetList, "Bonus"))
				{
					bool bValidRule = true;
					m_armorClassRuleSetList.back()->acType = iACType;

					if (section.has("Area"))
					{
						bValidRule = ParseACRuleCondition(sACRuleX, section.get("Area"), RuleParser::RuleType::AREA, m_armorClassRuleSetList);
					}

					if (bValidRule && section.has("Feat"))
					{
						bValidRule = ParseACRuleCondition(sACRuleX, section.get("Feat"), RuleParser::RuleType::FEAT, m_armorClassRuleSetList);
					}

					if (bValidRule && section.has("Extra"))
					{
						bValidRule = ParseACRuleCondition(sACRuleX, section.get("Extra"), RuleParser::RuleType::EXTRA, m_armorClassRuleSetList);
					}

					if (!bValidRule)
					{
						ACRuleSet* rsToDel = m_armorClassRuleSetList.back();
						m_armorClassRuleSetList.pop_back();
						delete rsToDel;
					}
				}
			}

			iRuleNumber++;
			sACRuleX = sACRuleBase + std::to_string(iRuleNumber);
		}
	}


	if (bOkRead && !isArmorClassAlreadyPatched)
	{
		isArmorClassAlreadyPatched = true;
		int i = 0;
		while (ArmorClassRulePatches[i].Apply()) {
			i++;
		}
	}
	else if (!bOkRead && isArmorClassAlreadyPatched)
	{
		isArmorClassAlreadyPatched = false;

		for (int i = (int)(sizeof(_ArmorClassRulePatches) / sizeof(Patch)) - 2; i >= 0; i--)
			ArmorClassRulePatches[i].Remove();
	}
}

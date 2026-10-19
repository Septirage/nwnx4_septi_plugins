
#include <misc/log.h>
#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include <misc/Patch.h>
#include "../../septutil/mini.h"

#include <string>
#include <vector>
#include <sstream>
#include <stdexcept>

#include "RuleParser.h"
#include "BonusParser.h"

extern std::unique_ptr<LogNWNX> logger;

//CalculateGenericSaveBonuses (thiscall stats, save kind ; ret 4): hardcoded feat bonuses of the saves, + stats+0x0E.
//Called by CalculateFortitude / CalculateWill / CalculateReflex (character sheet and every save), the DM stats display
//and the feat prerequisite test (MinFortSave column, automatic level up with a package)
#define OFFS_CalculateGenericSaveBonuses	0x0075a200		//push ecx / push ebx / push esi / mov esi,ecx (5 bytes)
unsigned long ReturnGenericSaveBonusesBody	= 0x0075a205;

//Save roll (0x638cc0): the effect bonuses against the save type. With / without a "versus" object, and the
//bonuses against spells (type 15) when a save of another type comes from a spell
#define FUNC_ComputeEffectBonuses			0x005f52f0
#define OFFS_SaveRollEffectsVersusCall		0x00638d88
#define OFFS_SaveRollEffectsCall			0x00638e68
#define OFFS_SaveRollEffectsVsSpellCall		0x00638ebd

#define SAVETYPE_NONE						-1

//Save kind given by the engine
#define SAVE_FORTITUDE						1
#define SAVE_REFLEX							2
#define SAVE_WILL							3


struct SaveRuleSet {
	RuleParser::Rule* featRule;
	RuleParser::Rule* areaTypeRule;
	RuleParser::Rule* extraRule;
	MathExpressionParser::Expr* bonusCalculation;
	int index;
	uint8_t saveMask;	// bit (1 << save kind)
	int saveType;		// SAVETYPE_NONE: every save of this kind, else only the saves against this type (SAVING_THROW_TYPE_*)


	SaveRuleSet(MathExpressionParser::Expr* bonus, int idx)
	    : bonusCalculation(bonus)
	    , index(idx)
	    , saveMask(0)
	    , saveType(SAVETYPE_NONE)
	{
		featRule = nullptr;
		areaTypeRule = nullptr;
		extraRule    = nullptr;
	}

	~SaveRuleSet()
	{
		delete bonusCalculation;
		delete areaTypeRule;
		delete extraRule;
		delete featRule;
	}
};


std::vector<SaveRuleSet*> m_savingThrowRuleSetList;

//RemoveBaseSaveFeatRules: the hardcoded feat bonuses of the engine are not applied, the rule file replaces them
bool bRemoveBaseSaveFeatRules = false;


static bool ParseSaveRuleCondition(std::string sSaveRuleX, std::string sValue, RuleParser::RuleType tFeatRule, std::vector<SaveRuleSet*>& myRuleList)
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
		logger->Err("Error during parse of [%s] \"%s\" = %s : %s", sSaveRuleX.c_str(),
		            sRuleType.c_str(), sValue.c_str(), e.what());
		logger->Info("Due to the Error, the complete rule will not be applied");
		return false;
	}

	return true;
}


static bool ParseSaveModifier(int iRuleNumber, std::string sValue, std::vector<SaveRuleSet*>& myRuleList, std::string sField)
{
	try {
		auto bonusCalc = MathExpressionParser::prepareSimplified(sValue, MathExpressionParser::Mode::INTEGER);
		SaveRuleSet* rs1 = new SaveRuleSet(bonusCalc, iRuleNumber);

		myRuleList.push_back(rs1);
	} catch (const std::exception& e) {

		logger->Err("Error during parse of \"%s\" = %s : %s", sField.c_str(), sValue.c_str(), e.what());
		logger->Info("Due to the Error, the complete rule will not be applied");
		return false;
	}

	return true;
}


//"FORT", "REFLEX", "WILL", "ALL", several separated by spaces. 0 if a word is unknown
static uint8_t ParseSaveMask(std::string sValue)
{
	uint8_t uMask = 0;
	std::istringstream iss(sValue);
	std::string sWord;

	while (iss >> sWord)
	{
		for (char& c : sWord)
			c = (char)toupper((unsigned char)c);

		if (sWord == "FORT" || sWord == "FORTITUDE")
			uMask |= (1 << SAVE_FORTITUDE);
		else if (sWord == "REFLEX")
			uMask |= (1 << SAVE_REFLEX);
		else if (sWord == "WILL")
			uMask |= (1 << SAVE_WILL);
		else if (sWord == "ALL")
			uMask |= (1 << SAVE_FORTITUDE) | (1 << SAVE_REFLEX) | (1 << SAVE_WILL);
		else
			return 0;
	}

	return uMask;
}


//Values of the SAVING_THROW_TYPE_* constants (nwscript), or a number. -2 if unknown
static int ParseSaveType(std::string sValue)
{
	static const char* saveTypeNames[] = {
		"ALL", "MIND_SPELLS", "POISON", "DISEASE", "FEAR", "SONIC", "ACID", "FIRE", "ELECTRICITY", "POSITIVE",
		"NEGATIVE", "DEATH", "COLD", "DIVINE", "TRAP", "SPELL", "GOOD", "EVIL", "LAW", "CHAOS"
	};

	std::string sWord;
	std::istringstream iss(sValue);
	if (!(iss >> sWord))
		return -2;

	for (char& c : sWord)
		c = (char)toupper((unsigned char)c);
	if (sWord.rfind("SAVING_THROW_TYPE_", 0) == 0)
		sWord = sWord.substr(18);

	for (int i = 1; i < (int)(sizeof(saveTypeNames) / sizeof(saveTypeNames[0])); i++)
	{
		if (sWord == saveTypeNames[i])
			return i;
	}

	try {
		size_t pos = 0;
		int iType = std::stoi(sWord, &pos);
		if (pos == sWord.size() && iType > 0 && iType < 256)
			return iType;
	} catch (const std::exception&) {}

	return -2;
}


//Sum of the rules for this save kind.
//iSaveType == SAVETYPE_NONE: rules without SaveType (CalculateFortitude/Reflex/Will).
//Else: rules against this type (save roll). The rules without SaveType are evaluated too (already counted by
//CalculateFortitude/Reflex/Will, so no bonus) so that a typed rule can reference them.
static int32_t savingThrowRuleApplication(int pcBlock, int iSaveKind, int iSaveType)
{
	int32_t iSaveMod = 0;
	int areaType = 0;

	if (iSaveKind < SAVE_FORTITUDE || iSaveKind > SAVE_WILL)
		return 0;

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

	RuleParser::m_activatedRuleSet.clear();

	for (SaveRuleSet* myRule : m_savingThrowRuleSetList)
	{
		if ((myRule->saveMask & (1 << iSaveKind)) == 0)
			continue;
		if (myRule->saveType != SAVETYPE_NONE && myRule->saveType != iSaveType)
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
			if (myRule->saveType == iSaveType)
				iSaveMod += myRule->bonusCalculation->evaluateInt(pcBlock);
		}
	}

	RuleParser::m_activatedRuleSet.clear();

	return iSaveMod;
}


//Original function: the overwritten first instructions, then the rest of the engine function (ret 4)
__declspec(naked) int __fastcall CalculateGenericSaveBonusesOriginal(void* pStats, void* Unused, int iSaveKind)
{
	__asm
	{
		push	ecx
		push	ebx
		push	esi
		mov		esi, ecx
		jmp		dword ptr [ReturnGenericSaveBonusesBody]
	}
}


//Jumped to from the start of CalculateGenericSaveBonuses (thiscall: ECX = stats, save kind on the stack, ret 4).
//A __fastcall with an unused EDX and one stack parameter has the same convention.
int __fastcall GenericSaveBonusesHook(int pcBlock, int Unused, int iSaveKind)
{
	int iBonus;

	if (bRemoveBaseSaveFeatRules)
		iBonus = *(int8_t*)(pcBlock + 0x0E);	//Not a feat bonus: kept
	else
		iBonus = CalculateGenericSaveBonusesOriginal((void*)pcBlock, NULL, iSaveKind);

	return iBonus + savingThrowRuleApplication(pcBlock, iSaveKind, SAVETYPE_NONE);
}


__declspec(naked) int __fastcall ComputeEffectBonuses(void* pCreature, void* Unused, int iBonusKind, int pVersus, int i3, int i4,
	int iSaveKind, int iSaveType, int i7, int i8, int i9)
{
	__asm
	{
		mov		edx, FUNC_ComputeEffectBonuses;
		jmp		edx;
	}
}


//Replaces the calls to ComputeEffectBonuses (thiscall: ECX = creature, 9 parameters, ret 0x24) of the save roll,
//where the save kind and the save type are known: + the rules against this save type
int __fastcall SaveRollEffectBonusesHook(void* pCreature, void* Unused, int iBonusKind, int pVersus, int i3, int i4,
	int iSaveKind, int iSaveType, int i7, int i8, int i9)
{
	int iBonus = ComputeEffectBonuses(pCreature, NULL, iBonusKind, pVersus, i3, i4, iSaveKind, iSaveType, i7, i8, i9);

	//Bytes given as dwords: only the low byte is meaningful
	uint8_t uSaveKind = (uint8_t)iSaveKind;
	uint8_t uSaveType = (uint8_t)iSaveType;
	if (uSaveType == 0)
		return iBonus;

	int pcBlock = *(int*)((char*)pCreature + 0x1FC4);
	if (pcBlock == 0)
		return iBonus;

	return iBonus + savingThrowRuleApplication(pcBlock, uSaveKind, uSaveType);
}


Patch _SavingThrowRulePatches[] = {
	Patch((DWORD)OFFS_CalculateGenericSaveBonuses, (char*)"\xe9", (int)1),
	Patch(OFFS_CalculateGenericSaveBonuses + 1, (relativefunc)GenericSaveBonusesHook),

	Patch(OFFS_SaveRollEffectsVersusCall + 1, (relativefunc)SaveRollEffectBonusesHook),
	Patch(OFFS_SaveRollEffectsCall + 1, (relativefunc)SaveRollEffectBonusesHook),
	Patch(OFFS_SaveRollEffectsVsSpellCall + 1, (relativefunc)SaveRollEffectBonusesHook),

	Patch()
};

Patch *SavingThrowRulePatches = _SavingThrowRulePatches;

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool isSavingThrowAlreadyPatched = false;

//Save roll (0x638cc0, thiscall creature, save kind, DC, save type, versus object, 3 other parameters ; ret 0x20).
//Before the d20 it only computes: base, effects, hardcoded feats, spell bonuses (from the versus object), cap at +20.
//The DC and the 3 last parameters are only used after the d20.
#define FUNC_SaveRoll						0x00638cc0
//Just before the d20: mov ecx,[0x86443c] (6 bytes). Base at [esp+0x1C], bonus (capped at +20) at [esp+0x14], EAX = 20
#define OFFS_SaveRollBeforeD20				0x00639781
unsigned long ReturnSaveRollD20				= 0x00639787;
//Exit of the save roll returning "failed" (pop edi/esi/ebp, xor al,al, pop ebx, add esp,0x24, ret 0x20)
unsigned long ReturnSaveRollFailed			= 0x00639b8f;

typedef uint8_t (__thiscall *SaveRollFn)(void* pCreature, uint32_t uSaveKind, uint32_t uDC, uint32_t uSaveType,
	uint32_t uVersus, int iParam6, uint32_t uParam7, int iParam8);

//Probe: the save roll stops before the d20 and gives its base and bonus
bool bSaveRollProbe = false;
int iSaveRollProbeBase = 0;
int iSaveRollProbeBonus = 0;

__declspec(naked) void SaveRollBeforeD20Hook()
{
	__asm
	{
		cmp		byte ptr [bSaveRollProbe], 0
		je		NormalRoll

		mov		eax, dword ptr [esp + 0x1C]
		mov		dword ptr [iSaveRollProbeBase], eax
		mov		eax, dword ptr [esp + 0x14]
		mov		dword ptr [iSaveRollProbeBonus], eax
		jmp		dword ptr [ReturnSaveRollFailed]

	NormalRoll:
		//Overwritten instruction, EAX (20) untouched
		mov		ecx, dword ptr ds:[0x0086443c]
		jmp		dword ptr [ReturnSaveRollD20]
	}
}

//Used for the GetSavingThrowVsType
Patch SaveRollProbePatches[] = {
	Patch((DWORD)OFFS_SaveRollBeforeD20, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6),
	Patch(OFFS_SaveRollBeforeD20 + 1, (relativefunc)SaveRollBeforeD20Hook),

	Patch()
};

//Script: the save of the creature against this type, computed by the save roll itself (without the d20).
//iSaveKind: 1 Fortitude, 2 Reflex, 3 Will. iSaveType: SAVING_THROW_TYPE_*. versusID: the object the save is against
//(creature: effects against its race/alignment; spell object: the bonuses against the spells), or invalid.
int GetSavingThrowVsType(int objectID, int iSaveKind, int iSaveType, int versusID)
{
	GameObject* Object;
	GameObjectManager m_ObjectManager;

	NWN::OBJECTID ObjectId = (NWN::OBJECTID)objectID;
	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject(ObjectId)) == NULL || Object->GetObjectType() != NWN::OBJECT_TYPE_CREATURE)
		return 0;

	if (*(int*)((uint8_t*)Object + 0x1FC4) == 0)
		return 0;
	if (iSaveKind < SAVE_FORTITUDE || iSaveKind > SAVE_WILL || iSaveType < 0 || iSaveType > 255)
		return 0;

	iSaveRollProbeBase = 0;
	iSaveRollProbeBonus = 0;

	bSaveRollProbe = true;
	((SaveRollFn)FUNC_SaveRoll)(Object, (uint32_t)iSaveKind, 0, (uint32_t)iSaveType, (uint32_t)versusID, 0, 0, 0);
	bSaveRollProbe = false;

	return iSaveRollProbeBase + iSaveRollProbeBonus;
}


void desinitSavingThrowRule()
{
	for (auto ruleSet : m_savingThrowRuleSetList) {
		delete ruleSet;
	}
	m_savingThrowRuleSetList.clear();
	bRemoveBaseSaveFeatRules = false;
}

void initSavingThrowPatch()
{
	int i = 0;
	while (SaveRollProbePatches[i].Apply()) {
		i++;
	}
}

void initSavingThrowRule(std::string nxhome, std::string sFileName)
{
	std::string inifile(nxhome);
	inifile.append("\\");
	inifile.append(sFileName);

	mINI::INIFile file(inifile);
	mINI::INIStructure ini;

	bool bOkRead = file.read(ini);

	if (!bOkRead)
		logger->Err("Cant find SavingThrowFile : %s", sFileName.c_str());

	if (bOkRead && ini.has("General"))
	{
		if (ini.get("General").has("DisableHook"))
		{
			std::string sDisableHook = ini.get("General").get("DisableHook");

			bOkRead = !(sDisableHook == "1");
		}

		if (ini.get("General").has("RemoveBaseSaveFeatRules"))
		{
			bRemoveBaseSaveFeatRules = (ini.get("General").get("RemoveBaseSaveFeatRules") == "1");
		}
	}

	if (bOkRead)
	{
		int iRuleNumber = 1;
		std::string sSaveRuleBase = "SaveRule";

		std::string sSaveRuleX = sSaveRuleBase + std::to_string(iRuleNumber);

		while (ini.has(sSaveRuleX))
		{
			auto section = ini.get(sSaveRuleX);

			if (!section.has("Bonus"))
			{
				logger->Err("[SavingThrowFile]: No Bonus field for [%s].", sSaveRuleX.c_str());
			}
			else if (!section.has("Save"))
			{
				logger->Err("[SavingThrowFile]: No Save field for [%s].", sSaveRuleX.c_str());
			}
			else
			{
				uint8_t uSaveMask = ParseSaveMask(section.get("Save"));
				int iSaveType = SAVETYPE_NONE;
				if (section.has("SaveType"))
					iSaveType = ParseSaveType(section.get("SaveType"));

				if (uSaveMask == 0)
				{
					logger->Err("[SavingThrowFile]: Bad Save field for [%s] : %s (FORT, REFLEX, WILL or ALL).", sSaveRuleX.c_str(), section.get("Save").c_str());
				}
				else if (iSaveType == -2)
				{
					logger->Err("[SavingThrowFile]: Bad SaveType field for [%s] : %s.", sSaveRuleX.c_str(), section.get("SaveType").c_str());
				}
				else if (ParseSaveModifier(iRuleNumber, section.get("Bonus"), m_savingThrowRuleSetList, "Bonus"))
				{
					bool bValidRule = true;
					m_savingThrowRuleSetList.back()->saveMask = uSaveMask;
					m_savingThrowRuleSetList.back()->saveType = iSaveType;

					if (section.has("Area"))
					{
						bValidRule = ParseSaveRuleCondition(sSaveRuleX, section.get("Area"), RuleParser::RuleType::AREA, m_savingThrowRuleSetList);
					}

					if (bValidRule && section.has("Feat"))
					{
						bValidRule = ParseSaveRuleCondition(sSaveRuleX, section.get("Feat"), RuleParser::RuleType::FEAT, m_savingThrowRuleSetList);
					}

					if (bValidRule && section.has("Extra"))
					{
						bValidRule = ParseSaveRuleCondition(sSaveRuleX, section.get("Extra"), RuleParser::RuleType::EXTRA, m_savingThrowRuleSetList);
					}

					if (!bValidRule)
					{
						SaveRuleSet* rsToDel = m_savingThrowRuleSetList.back();
						m_savingThrowRuleSetList.pop_back();
						delete rsToDel;
					}
				}
			}

			iRuleNumber++;
			sSaveRuleX = sSaveRuleBase + std::to_string(iRuleNumber);
		}
	}


	if (bOkRead && !isSavingThrowAlreadyPatched)
	{
		isSavingThrowAlreadyPatched = true;
		int i = 0;
		while (SavingThrowRulePatches[i].Apply()) {
			i++;
		}
	}
	else if (!bOkRead && isSavingThrowAlreadyPatched)
	{
		isSavingThrowAlreadyPatched = false;

		//Remove in reverse order (last element is the empty Patch())
		for (int i = (int)(sizeof(_SavingThrowRulePatches) / sizeof(Patch)) - 2; i >= 0; i--)
			SavingThrowRulePatches[i].Remove();
	}
}

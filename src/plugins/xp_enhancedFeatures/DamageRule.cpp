
#include <misc/log.h>
#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include <misc/Patch.h>
#include "../../septutil/mini.h"

#include <string>
#include <vector>
#include <stdexcept>

#include "RuleParser.h"
#include "BonusParser.h"

extern std::unique_ptr<LogNWNX> logger;

//CalculatePhysicalDamage (thiscall stats, target creature or 0, 1 = off hand ; ret 8): the fixed bonuses of the
//physical damage (Strength, feats...), not the weapon dice. The attack (FUN_005a7690, target known) multiplies it on
//a critical; UpdateCombatInformation (character sheet, no target) calls it for each hand.
#define OFFS_CalculatePhysicalDamage		0x005a5f90		//push ebp / mov ebp,esp / and esp,-8 (6 bytes)
unsigned long ReturnPhysicalDamageBody		= 0x005a5f96;


struct DamageRuleSet {
	RuleParser::Rule* featRule;
	RuleParser::Rule* areaTypeRule;
	RuleParser::Rule* extraRule;
	MathExpressionParser::Expr* bonusCalculation;
	int index;


	DamageRuleSet(MathExpressionParser::Expr* bonus, int idx)
	    : bonusCalculation(bonus)
	    , index(idx)
	{
		featRule = nullptr;
		areaTypeRule = nullptr;
		extraRule    = nullptr;
	}

	~DamageRuleSet()
	{
		delete bonusCalculation;
		delete areaTypeRule;
		delete extraRule;
		delete featRule;
	}
};


std::vector<DamageRuleSet*> m_damageRuleSetList;


static bool ParseDamageRuleCondition(std::string sRuleX, std::string sValue, RuleParser::RuleType tFeatRule, std::vector<DamageRuleSet*>& myRuleList)
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


static bool ParseDamageModifier(int iRuleNumber, std::string sValue, std::vector<DamageRuleSet*>& myRuleList, std::string sField)
{
	try {
		auto bonusCalc = MathExpressionParser::prepareSimplified(sValue, MathExpressionParser::Mode::INTEGER);
		DamageRuleSet* rs1 = new DamageRuleSet(bonusCalc, iRuleNumber);

		myRuleList.push_back(rs1);
	} catch (const std::exception& e) {

		logger->Err("Error during parse of \"%s\" = %s : %s", sField.c_str(), sValue.c_str(), e.what());
		logger->Info("Due to the Error, the complete rule will not be applied");
		return false;
	}

	return true;
}


//Sum of the rules
static int32_t damageRuleApplication(int pcBlock)
{
	int32_t iMod = 0;
	int areaType = 0;

	if (m_damageRuleSetList.empty())
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

	for (DamageRuleSet* myRule : m_damageRuleSetList)
	{
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
			iMod += myRule->bonusCalculation->evaluateInt(pcBlock);
		}
	}

	RuleParser::m_activatedRuleSet.clear();

	return iMod;
}


//Original function: the overwritten first instructions, then the rest of the engine function (ret 8)
__declspec(naked) int __fastcall CalculatePhysicalDamageOriginal(void* pStats, void* Unused, void* pTarget, int iOffHand)
{
	__asm
	{
		push	ebp
		mov		ebp, esp
		and		esp, 0xFFFFFFF8
		jmp		dword ptr [ReturnPhysicalDamageBody]
	}
}

//Jumped to from the start of CalculatePhysicalDamage (thiscall: ECX = stats, 2 stack parameters, ret 8).
//A __fastcall with an unused EDX and two stack parameters has the same convention.
//The engine floors the total physical damage at 1, so a negative bonus is safe.
int __fastcall PhysicalDamageHook(void* pStats, void* Unused, void* pTarget, int iOffHand)
{
	return CalculatePhysicalDamageOriginal(pStats, NULL, pTarget, iOffHand) + damageRuleApplication((int)pStats);
}


Patch _DamageRulePatches[] = {
	Patch((DWORD)OFFS_CalculatePhysicalDamage, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6),
	Patch(OFFS_CalculatePhysicalDamage + 1, (relativefunc)PhysicalDamageHook),

	Patch()
};

Patch *DamageRulePatches = _DamageRulePatches;

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool isDamageAlreadyPatched = false;

void desinitDamageRule()
{
	for (auto ruleSet : m_damageRuleSetList) {
		delete ruleSet;
	}
	m_damageRuleSetList.clear();
}

void initDamageRule(std::string nxhome, std::string sFileName)
{
	std::string inifile(nxhome);
	inifile.append("\\");
	inifile.append(sFileName);

	mINI::INIFile file(inifile);
	mINI::INIStructure ini;

	bool bOkRead = file.read(ini);

	if (!bOkRead)
		logger->Err("Cant find DamageFile : %s", sFileName.c_str());

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
		std::string sDamageRuleBase = "DamageRule";

		std::string sDamageRuleX = sDamageRuleBase + std::to_string(iRuleNumber);

		while (ini.has(sDamageRuleX))
		{
			auto section = ini.get(sDamageRuleX);

			if (!section.has("Bonus"))
			{
				logger->Err("[DamageFile]: No Bonus field for [%s].", sDamageRuleX.c_str());
			}
			else if (ParseDamageModifier(iRuleNumber, section.get("Bonus"), m_damageRuleSetList, "Bonus"))
			{
				bool bValidRule = true;

				if (section.has("Area"))
				{
					bValidRule = ParseDamageRuleCondition(sDamageRuleX, section.get("Area"), RuleParser::RuleType::AREA, m_damageRuleSetList);
				}

				if (bValidRule && section.has("Feat"))
				{
					bValidRule = ParseDamageRuleCondition(sDamageRuleX, section.get("Feat"), RuleParser::RuleType::FEAT, m_damageRuleSetList);
				}

				if (bValidRule && section.has("Extra"))
				{
					bValidRule = ParseDamageRuleCondition(sDamageRuleX, section.get("Extra"), RuleParser::RuleType::EXTRA, m_damageRuleSetList);
				}

				if (!bValidRule)
				{
					DamageRuleSet* rsToDel = m_damageRuleSetList.back();
					m_damageRuleSetList.pop_back();
					delete rsToDel;
				}
			}

			iRuleNumber++;
			sDamageRuleX = sDamageRuleBase + std::to_string(iRuleNumber);
		}
	}


	if (bOkRead && !isDamageAlreadyPatched)
	{
		isDamageAlreadyPatched = true;
		int i = 0;
		while (DamageRulePatches[i].Apply()) {
			i++;
		}
	}
	else if (!bOkRead && isDamageAlreadyPatched)
	{
		isDamageAlreadyPatched = false;

		//Remove in reverse order (last element is the empty Patch())
		for (int i = (int)(sizeof(_DamageRulePatches) / sizeof(Patch)) - 2; i >= 0; i--)
			DamageRulePatches[i].Remove();
	}
}

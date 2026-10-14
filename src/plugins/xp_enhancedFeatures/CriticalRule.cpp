
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

//The 3 engine functions (thiscall: ECX = stats, 1 parameter: 1 = off hand ; ret 4)
//Threat: first roll of the threat range (19 for 19-20). Attack roll + character sheet (both hands)
#define OFFS_CriticalThreat					0x0059eaa0		//sub esp,8 / cmp [esp+0xC],1 (8 bytes)
unsigned long ReturnCriticalThreatBody	= 0x0059eaa8;
//Confirm: bonus to the confirmation roll. Attack roll + TouchAttack
#define OFFS_CriticalConfirm				0x0059ec10		//cmp [esp+4],0 (5 bytes)
unsigned long ReturnCriticalConfirmBody	= 0x0059ec15;
//Multiplier (GetCreatureCritMultilplier). Damages + character sheet (off hand)
#define OFFS_CriticalMultiplier				0x0059eca0		//cmp [esp+4],1 (5 bytes)
unsigned long ReturnCriticalMultiplierBody = 0x0059eca5;

//UpdateCombatInformation has an inline copy of GetCreatureCritMultilplier for the main hand (0x5ac907-0x5ac963,
//result in EBX). Replaced by a call to the function, so that the sheet uses the hooked function too
#define OFFS_InlineMainHandMultiplier		0x005ac907

//Safety bounds: below, the engine gives no critical at all (threat) or no weapon damage on a critical (multiplier)
#define CRITICAL_MIN_THREAT					2
#define CRITICAL_MIN_MULTIPLIER				1


struct CriticalRuleSet {
	RuleParser::Rule* featRule;
	RuleParser::Rule* areaTypeRule;
	RuleParser::Rule* extraRule;
	MathExpressionParser::Expr* bonusCalculation;
	int index;


	CriticalRuleSet(MathExpressionParser::Expr* bonus, int idx)
	    : bonusCalculation(bonus)
	    , index(idx)
	{
		featRule = nullptr;
		areaTypeRule = nullptr;
		extraRule    = nullptr;
	}

	~CriticalRuleSet()
	{
		delete bonusCalculation;
		delete areaTypeRule;
		delete extraRule;
		delete featRule;
	}
};


std::vector<CriticalRuleSet*> m_criticalThreatRuleSetList;
std::vector<CriticalRuleSet*> m_criticalConfirmRuleSetList;
std::vector<CriticalRuleSet*> m_criticalMultiplierRuleSetList;


static bool ParseCriticalRuleCondition(std::string sRuleX, std::string sValue, RuleParser::RuleType tFeatRule, std::vector<CriticalRuleSet*>& myRuleList)
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


static bool ParseCriticalModifier(int iRuleNumber, std::string sValue, std::vector<CriticalRuleSet*>& myRuleList, std::string sField)
{
	try {
		auto bonusCalc = MathExpressionParser::prepareSimplified(sValue, MathExpressionParser::Mode::INTEGER);
		CriticalRuleSet* rs1 = new CriticalRuleSet(bonusCalc, iRuleNumber);

		myRuleList.push_back(rs1);
	} catch (const std::exception& e) {

		logger->Err("Error during parse of \"%s\" = %s : %s", sField.c_str(), sValue.c_str(), e.what());
		logger->Info("Due to the Error, the complete rule will not be applied");
		return false;
	}

	return true;
}


//Sum of the rules of one list
static int32_t criticalRuleApplication(int pcBlock, std::vector<CriticalRuleSet*>& myRuleList)
{
	int32_t iMod = 0;
	int areaType = 0;

	if (myRuleList.empty())
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

	for (CriticalRuleSet* myRule : myRuleList)
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


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Original functions: the overwritten first instructions, then the rest of the engine function.
// Called as the engine does (ECX = stats, the parameter on the stack), they return to the caller with ret 4.
//////////////////////////////////////////////////////////////////////////////////////////////////////////

__declspec(naked) int __fastcall CriticalThreatOriginal(void* pStats, void* Unused, int iOffHand)
{
	__asm
	{
		sub		esp, 8
		cmp		dword ptr [esp + 0xC], 1
		jmp		dword ptr [ReturnCriticalThreatBody]
	}
}

__declspec(naked) int __fastcall CriticalConfirmOriginal(void* pStats, void* Unused, int iOffHand)
{
	__asm
	{
		cmp		dword ptr [esp + 4], 0
		jmp		dword ptr [ReturnCriticalConfirmBody]
	}
}

__declspec(naked) int __fastcall CriticalMultiplierOriginal(void* pStats, void* Unused, int iOffHand)
{
	__asm
	{
		cmp		dword ptr [esp + 4], 1
		jmp		dword ptr [ReturnCriticalMultiplierBody]
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Hooks: jumped to from the start of the engine functions. A __fastcall with an unused EDX and one stack
// parameter has the same convention as the engine function (thiscall, ret 4).
//////////////////////////////////////////////////////////////////////////////////////////////////////////

int __fastcall CriticalThreatHook(void* pStats, void* Unused, int iOffHand)
{
	int iThreat = CriticalThreatOriginal(pStats, NULL, iOffHand);

	//The range grows: its first roll goes down
	iThreat -= criticalRuleApplication((int)pStats, m_criticalThreatRuleSetList);
	if (iThreat < CRITICAL_MIN_THREAT)
		iThreat = CRITICAL_MIN_THREAT;

	return iThreat;
}

int __fastcall CriticalConfirmHook(void* pStats, void* Unused, int iOffHand)
{
	return CriticalConfirmOriginal(pStats, NULL, iOffHand) + criticalRuleApplication((int)pStats, m_criticalConfirmRuleSetList);
}

static int ApplyMultiplierRules(int pcBlock, int iMultiplier)
{
	iMultiplier += criticalRuleApplication(pcBlock, m_criticalMultiplierRuleSetList);
	if (iMultiplier < CRITICAL_MIN_MULTIPLIER)
		iMultiplier = CRITICAL_MIN_MULTIPLIER;

	return iMultiplier;
}

int __fastcall CriticalMultiplierHook(void* pStats, void* Unused, int iOffHand)
{
	return ApplyMultiplierRules((int)pStats, CriticalMultiplierOriginal(pStats, NULL, iOffHand));
}

Patch _CriticalRulePatches[] = {
	Patch((DWORD)OFFS_CriticalThreat, (char*)"\xe9\x00\x00\x00\x00\x90\x90\x90", (int)8),
	Patch(OFFS_CriticalThreat + 1, (relativefunc)CriticalThreatHook),

	Patch((DWORD)OFFS_CriticalConfirm, (char*)"\xe9\x00\x00\x00\x00", (int)5),
	Patch(OFFS_CriticalConfirm + 1, (relativefunc)CriticalConfirmHook),

	Patch((DWORD)OFFS_CriticalMultiplier, (char*)"\xe9\x00\x00\x00\x00", (int)5),
	Patch(OFFS_CriticalMultiplier + 1, (relativefunc)CriticalMultiplierHook),

	//PUSH 0 / MOV ECX,EBP / CALL GetCreatureCritMultilplier (0x59eca0) / MOV EBX,EAX / JMP 0x5ac964
	Patch((DWORD)OFFS_InlineMainHandMultiplier, (char*)"\x6a\x00\x8b\xcd\xe8\x90\x23\xff\xff\x8b\xd8\xeb\x50", (int)13),

	Patch()
};

Patch *CriticalRulePatches = _CriticalRulePatches;

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool isCriticalAlreadyPatched = false;

static void ClearCriticalRuleList(std::vector<CriticalRuleSet*>& myRuleList)
{
	for (auto ruleSet : myRuleList) {
		delete ruleSet;
	}
	myRuleList.clear();
}

void desinitCriticalRule()
{
	ClearCriticalRuleList(m_criticalThreatRuleSetList);
	ClearCriticalRuleList(m_criticalConfirmRuleSetList);
	ClearCriticalRuleList(m_criticalMultiplierRuleSetList);
}

//Read the sections sRuleBase1, sRuleBase2... into myRuleList
static void ParseCriticalRuleSections(mINI::INIStructure& ini, std::string sRuleBase, std::vector<CriticalRuleSet*>& myRuleList)
{
	int iRuleNumber = 1;
	std::string sRuleX = sRuleBase + std::to_string(iRuleNumber);

	while (ini.has(sRuleX))
	{
		auto section = ini.get(sRuleX);

		if (!section.has("Bonus"))
		{
			logger->Err("[CriticalFile]: No Bonus field for [%s].", sRuleX.c_str());
		}
		else if (ParseCriticalModifier(iRuleNumber, section.get("Bonus"), myRuleList, "Bonus"))
		{
			bool bValidRule = true;

			if (section.has("Area"))
			{
				bValidRule = ParseCriticalRuleCondition(sRuleX, section.get("Area"), RuleParser::RuleType::AREA, myRuleList);
			}

			if (bValidRule && section.has("Feat"))
			{
				bValidRule = ParseCriticalRuleCondition(sRuleX, section.get("Feat"), RuleParser::RuleType::FEAT, myRuleList);
			}

			if (bValidRule && section.has("Extra"))
			{
				bValidRule = ParseCriticalRuleCondition(sRuleX, section.get("Extra"), RuleParser::RuleType::EXTRA, myRuleList);
			}

			if (!bValidRule)
			{
				CriticalRuleSet* rsToDel = myRuleList.back();
				myRuleList.pop_back();
				delete rsToDel;
			}
		}

		iRuleNumber++;
		sRuleX = sRuleBase + std::to_string(iRuleNumber);
	}
}

void initCriticalRule(std::string nxhome, std::string sFileName)
{
	std::string inifile(nxhome);
	inifile.append("\\");
	inifile.append(sFileName);

	mINI::INIFile file(inifile);
	mINI::INIStructure ini;

	bool bOkRead = file.read(ini);

	if (!bOkRead)
		logger->Err("Cant find CriticalFile : %s", sFileName.c_str());

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
		ParseCriticalRuleSections(ini, "CriticalThreatRule", m_criticalThreatRuleSetList);
		ParseCriticalRuleSections(ini, "CriticalConfirmRule", m_criticalConfirmRuleSetList);
		ParseCriticalRuleSections(ini, "CriticalMultiplierRule", m_criticalMultiplierRuleSetList);
	}


	if (bOkRead && !isCriticalAlreadyPatched)
	{
		isCriticalAlreadyPatched = true;
		int i = 0;
		while (CriticalRulePatches[i].Apply()) {
			i++;
		}
	}
	else if (!bOkRead && isCriticalAlreadyPatched)
	{
		isCriticalAlreadyPatched = false;

		for (int i = (int)(sizeof(_CriticalRulePatches) / sizeof(Patch)) - 2; i >= 0; i--)
			CriticalRulePatches[i].Remove();
	}
}

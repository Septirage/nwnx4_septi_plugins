
#include <misc/log.h>
#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include <misc/Patch.h>
#include "../../septutil/mini.h"

#include <unordered_map>
#include <string>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <sstream>
#include <stdexcept>

#include "RuleParser.h"
#include "BonusParser.h"

extern std::unique_ptr<LogNWNX> logger;

unsigned long ReturnBAFeatBeforeLastFeatEnd = 0x005b13a8;
unsigned long ReturnBAMeleeEnd = 0x0059da07;		//POP EBX / ADD ESP,8 / RET 0xC
unsigned long ReturnBARangedEnd = 0x0059db02;		//RET 8

#define OFFS_BonusAttackFeatStart		0x005b13a3
#define OFFS_BonusAttackFeatKeepEsi1	0x005b135a
#define OFFS_BonusAttackFeatKeepEsi2	0x005b1378

#define OFFS_BonusAttackMeleeEnd		0x0059da02		//End of GetMeleeAttackBonus
#define OFFS_BonusAttackRangedEnd		0x0059dafd		//End of GetRangedAttackBonus
#define OFFS_InlineRangedAttackBonus	0x005ad838		//UpdateCombatInformation inline copy of GetRangedAttackBonus


struct BARuleSet {
//	std::string name;
	RuleParser::Rule* featRule;
	RuleParser::Rule* areaTypeRule;
	RuleParser::Rule* extraRule;
	MathExpressionParser::Expr* bonusCalculation;
	int index;


	BARuleSet(MathExpressionParser::Expr* bonus, int idx)
	    : bonusCalculation(bonus)
	    , index(idx)
	{
		featRule = nullptr;
		areaTypeRule = nullptr;
		extraRule    = nullptr;
	}

	~BARuleSet()
	{ 
		delete bonusCalculation;
		delete areaTypeRule;
		delete extraRule;
		delete featRule;
	}
};


std::vector<BARuleSet*> m_bonusAttackFeatRuleSetList;


bool ParseRuleFeatOrAreaType(std::string sBonusAttackFeatRuleX, int iRuleNumber, std::string sValue, RuleParser::RuleType tFeatRule, std::vector<BARuleSet*>& myRuleList)
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
		logger->Err("Error during parse of [%s] \"%s\" = %s : %s", sBonusAttackFeatRuleX.c_str(),
		            sRuleType.c_str(), sValue.c_str(), e.what());
		logger->Info("Due to the Error, the complete rule will not be applied");
		return false;
	}

	return true;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ParseBonusAttackModifier(int iRuleNumber, std::string sValue, std::vector<BARuleSet*>& myRuleList, std::string sField)
{
	try {
		auto bonusCalc = MathExpressionParser::prepareSimplified(sValue, MathExpressionParser::Mode::INTEGER);
		BARuleSet* rs1 = new BARuleSet(bonusCalc, iRuleNumber);

		myRuleList.push_back(rs1);
	} catch (const std::exception& e) {

		logger->Err("Error during parse of \"%s\" = %s : %s", sField.c_str(), sValue.c_str(), e.what());
		logger->Info("Due to the Error, the complete rule will not be applied");
		return false;
	}

	return true;
}


//Insert more BA Calculation 
int32_t __fastcall bonusAttackFeatApplication(int pcBlock, int targetBlock)
{
	int32_t iBAMod = 0;
	int areaType = 0;

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


	for (BARuleSet* myRule : m_bonusAttackFeatRuleSetList) 
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
			iBAMod += myRule->bonusCalculation->evaluateInt(pcBlock);
		}
	}

	RuleParser::m_activatedRuleSet.clear();

	return iBAMod;
}




__declspec(naked) void CallBonusAttackFeats()
{
	__asm
	{
		MOV		EDX, dword ptr [EBP + 8]
		MOV		ECX, ESI

		CALL bonusAttackFeatApplication

		ADD		dword ptr [ESP + 0xC], EAX

		MOV		EAX, dword ptr [ESP + 0xC]
		POP		EDI

		JMP		dword ptr[ReturnBAFeatBeforeLastFeatEnd]
	}
}


//End of GetMeleeAttackBonus (character sheet, TouchAttackMelee, DM stats). ESI = stats, EBX = result
__declspec(naked) void CallBonusAttackFeatsMelee()
{
	__asm
	{
		MOV		ECX, ESI
		XOR		EDX, EDX

		CALL bonusAttackFeatApplication

		ADD		EBX, EAX

		POP		EDI
		POP		ESI
		POP		EBP
		MOV		EAX, EBX

		JMP		dword ptr[ReturnBAMeleeEnd]
	}
}


//End of GetRangedAttackBonus (character sheet, TouchAttackRanged, DM stats). ESI = stats, EBX = result
__declspec(naked) void CallBonusAttackFeatsRanged()
{
	__asm
	{
		MOV		ECX, ESI
		XOR		EDX, EDX

		CALL bonusAttackFeatApplication

		ADD		EBX, EAX

		POP		EDI
		POP		ESI
		MOV		EAX, EBX
		POP		EBX

		JMP		dword ptr[ReturnBARangedEnd]
	}
}


Patch _BonusAttackFeatPatches[] = {
	Patch(OFFS_BonusAttackFeatKeepEsi1, (char*)"\x9a", (int)1),		//mov esi, ... => mov ebx
	Patch(OFFS_BonusAttackFeatKeepEsi2, (char*)"\xcb", (int)1),		//mov ecx,esi => mov ecx,ebx
	Patch((DWORD)OFFS_BonusAttackFeatStart, (char*)"\xe9\x00\x00\x00\x00", (int)5),
    Patch(OFFS_BonusAttackFeatStart + 1, (relativefunc)CallBonusAttackFeats),

	Patch((DWORD)OFFS_BonusAttackMeleeEnd, (char*)"\xe9\x00\x00\x00\x00", (int)5),
	Patch(OFFS_BonusAttackMeleeEnd + 1, (relativefunc)CallBonusAttackFeatsMelee),

	Patch((DWORD)OFFS_BonusAttackRangedEnd, (char*)"\xe9\x00\x00\x00\x00", (int)5),
	Patch(OFFS_BonusAttackRangedEnd + 1, (relativefunc)CallBonusAttackFeatsRanged),

	//PUSH 0 / PUSH 0 / MOV ECX,EBP / CALL GetRangedAttackBonus (0x59da10) / JMP 0x5ad8ad
	Patch(OFFS_InlineRangedAttackBonus, (char*)"\x6a\x00\x6a\x00\x8b\xcd\xe8\xcd\x01\xff\xff\xeb\x68", (int)13),

	Patch()
};

Patch *BonusAttackFeatPatches = _BonusAttackFeatPatches;

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool isBAAlreadyPatched = false;

void desinitBAFeat()
{
	//Clear Rule Feat
	for (auto ruleSet : m_bonusAttackFeatRuleSetList) {
		delete ruleSet;
	}
	m_bonusAttackFeatRuleSetList.clear();
}

void initBAFeat(std::string nxhome, std::string sFileName)
{
	std::string inifile(nxhome);
	inifile.append("\\");
	inifile.append(sFileName);

	mINI::INIFile file(inifile);
	mINI::INIStructure ini;
	bool bOkRead = file.read(ini);

	if (!bOkRead)
		logger->Err("Cant find BAFile : %s", sFileName.c_str());

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
		std::string sBonusAttackFeatRuleBase = "BonusAttackRule";

		std::string sBonusAttackFeatRuleX = sBonusAttackFeatRuleBase + std::to_string(iRuleNumber);

		while (ini.has(sBonusAttackFeatRuleX))
		{
		
			//We need a bonus. Else, error and no need to parse other stuff
			if (ini.get(sBonusAttackFeatRuleX).has("Bonus"))
			{
				std::string sRuleBonusAttackMod = ini.get(sBonusAttackFeatRuleX).get("Bonus");

				if (ParseBonusAttackModifier(iRuleNumber, sRuleBonusAttackMod, m_bonusAttackFeatRuleSetList, "Bonus"))
				{
					//Ok, we can test and parse other fields
					bool bValidRule = true;

					if (ini.get(sBonusAttackFeatRuleX).has("Area"))
					{
						std::string sValue = ini.get(sBonusAttackFeatRuleX).get("Area");

						bValidRule = ParseRuleFeatOrAreaType(sBonusAttackFeatRuleX, iRuleNumber, sValue,
							RuleParser::RuleType::AREA, m_bonusAttackFeatRuleSetList);
					}

					if (bValidRule && ini.get(sBonusAttackFeatRuleX).has("Feat"))
					{
						std::string sValue = ini.get(sBonusAttackFeatRuleX).get("Feat");

						bValidRule = ParseRuleFeatOrAreaType(sBonusAttackFeatRuleX, iRuleNumber, sValue,
							RuleParser::RuleType::FEAT, m_bonusAttackFeatRuleSetList);
					}

					if (bValidRule && ini.get(sBonusAttackFeatRuleX).has("Extra"))
					{
						std::string sValue = ini.get(sBonusAttackFeatRuleX).get("Extra");

						bValidRule = ParseRuleFeatOrAreaType(sBonusAttackFeatRuleX, iRuleNumber, sValue,
							RuleParser::RuleType::EXTRA, m_bonusAttackFeatRuleSetList);
					}

					if (!bValidRule)
					{
						//Remove this rule.
						BARuleSet* rsToDel = m_bonusAttackFeatRuleSetList.back();
						m_bonusAttackFeatRuleSetList.pop_back();
						delete rsToDel;
					}
				}
			}
			else
			{
				logger->Err("[BonusAttacksFeatsIniFile]: No Bonus field for [%s].", sBonusAttackFeatRuleX.c_str());
			}

			iRuleNumber++;
			sBonusAttackFeatRuleX = sBonusAttackFeatRuleBase + std::to_string(iRuleNumber);
		}

	}


	if (bOkRead && !isBAAlreadyPatched)
	{
		isBAAlreadyPatched = true;
		int i = 0;
		while(BonusAttackFeatPatches[i].Apply()) {
			i++;
		}
	}
	else if (!bOkRead && isBAAlreadyPatched)
	{
		isBAAlreadyPatched = false;

		for (int i = (int)(sizeof(_BonusAttackFeatPatches) / sizeof(Patch)) - 2; i >= 0; i--)
			BonusAttackFeatPatches[i].Remove();
	}
}


//Ask the engine to recompute the combat information (character sheet) of oCreature.
//Done at the next player update, and only sent to the client if something changed.
void RefreshCombatInformation(int objectID)
{
	NWN::OBJECTID ObjectId = (NWN::OBJECTID)objectID;
	GameObject* Object;
	GameObjectManager m_ObjectManager;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject(ObjectId)) == NULL)
		return;

	if (Object->GetObjectType() != NWN::OBJECT_TYPE_CREATURE)
		return;

	uint8_t* pStats = *(uint8_t**)((uint8_t*)Object + 0x1FC4);
	if (pStats == NULL)
		return;

	//"Combat information must be updated" flag (also set by AddFeat, effects, SetCombatMode...)
	*(uint32_t*)(pStats + 0xE10) = 1;
}


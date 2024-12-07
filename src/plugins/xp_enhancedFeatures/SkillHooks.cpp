
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

const int EndOfSkillFeat = 0x00758669;
const int GoBackSkillAbility = 0x00756c9c;
const int EndOfSkillAbility = 0x00756cbf;

int iRemoveBaseSkillFeats = 0;


struct SkillRuleSet {
	//	std::string name;
	RuleParser::Rule* featRule;
	RuleParser::Rule* areaTypeRule;
	RuleParser::Rule* extraRule;
	MathExpressionParser::Expr* bonusCalculation;
	int index;
	int iAbility;


	SkillRuleSet(MathExpressionParser::Expr* bonus, int idx)
		: bonusCalculation(bonus)
		, index(idx)
	{
		featRule = nullptr;
		areaTypeRule = nullptr;
		extraRule    = nullptr;
		iAbility = 6;
	}

	SkillRuleSet(int idx, int iAb) : iAbility(iAb), index(idx)
	{
		featRule = nullptr;
		areaTypeRule = nullptr;
		extraRule    = nullptr;
		bonusCalculation = nullptr;	
	}

	~SkillRuleSet()
	{ 
		delete bonusCalculation;
		delete areaTypeRule;
		delete extraRule;
		delete featRule;
	}
};

std::unordered_map<uint32_t, std::vector<SkillRuleSet*>> m_skillFeatRuleSets;
std::unordered_map<uint32_t, std::vector<SkillRuleSet*>> m_skillAbilityRuleSets;


bool ParseRuleFeatOrAreaType(std::string sSkillFeatRuleX, int iRuleNumber, std::string sValue, int iSkillId, RuleParser::RuleType tFeatRule, std::unordered_map<uint32_t, std::vector<SkillRuleSet*>>& myRuleMap, bool bAllowRule=true);


uint32_t __fastcall ApplySkillFeats(char* CreaPtr, uint32_t skillId, uint32_t areaType)
{
	uint32_t uBonusTotal = 0;
	skillId += 1;
	//We have rules for this skill. Test and apply them
	if (m_skillFeatRuleSets.find(skillId) != m_skillFeatRuleSets.end())
	{
		for (SkillRuleSet* myRule : m_skillFeatRuleSets[skillId])
		{
			bool bOk = true;
			if (myRule->featRule != nullptr)
			{
				bOk = RuleParser::evaluateRule(myRule->featRule, RuleParser::RuleType::FEAT, (int)CreaPtr);
			}

			if (bOk && myRule->areaTypeRule != nullptr)
			{
				bOk = RuleParser::evaluateRule(myRule->areaTypeRule, RuleParser::RuleType::AREA, areaType);
			}

			if (bOk && myRule->extraRule != nullptr) {
				bOk = RuleParser::evaluateRule(myRule->extraRule, RuleParser::RuleType::EXTRA, *(int*)(CreaPtr+0xA4));
			}

			if (bOk)
			{
				RuleParser::m_activatedRuleSet.insert(myRule->index);
				uBonusTotal += myRule->bonusCalculation->evaluateInt((int)CreaPtr);
			}
		}
	}
	RuleParser::m_activatedRuleSet.clear();
	return uBonusTotal;
}

int __fastcall MySkillAbility(char* CreaPtr, uint32_t skillId)
{
	int iAbility = -1;
	//We have rules for this skill. Test and apply them
	if (m_skillAbilityRuleSets.find(skillId) != m_skillAbilityRuleSets.end())
	{
		int pcPtr = *(int*)(CreaPtr + 0xA4);
		int areaType = 0;

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



		for (SkillRuleSet* myRule : m_skillAbilityRuleSets[skillId])
		{
			bool bOk = true;
			if (myRule->featRule != nullptr)
			{
				bOk = RuleParser::evaluateRule(myRule->featRule, RuleParser::RuleType::FEAT, (int)CreaPtr);
			}

			if (bOk && myRule->areaTypeRule != nullptr)
			{
				bOk = RuleParser::evaluateRule(myRule->areaTypeRule, RuleParser::RuleType::AREA, areaType);
			}

			if (bOk && myRule->extraRule != nullptr) {
				bOk = RuleParser::evaluateRule(myRule->extraRule, RuleParser::RuleType::EXTRA, pcPtr);
			}

			if (bOk)
			{
				return myRule->iAbility;
			}
		}
	}
	return -1;
}


unsigned long ReturnSkillFeats = 0x00756db9;

//0x00756db3
__declspec(naked) void CallSkillFeats()
{
	__asm
	{
		//Save registers
		PUSH EAX
		PUSH ECX
		PUSH EDX

		//Prepare parameters (ecx = creaptr, skillId, areaType)
		MOV EDX, EAX
		MOV EAX, [ESP + 0x18]
		PUSH EAX
		CALL ApplySkillFeats

		//Apply the feats Gains
		ADD EBP, EAX
		
		//Restore register
		POP EDX
		POP ECX
		POP EAX


		//Do we remove the original game skillFeats ?
		CMP iRemoveBaseSkillFeats, 0x1
		JE	LeaveSkillFeats

		//Go back to normal behavior
		CMP	EAX, 0x1C
		JGE	LeaveSkillFeats

		JMP dword ptr[ReturnSkillFeats]
		//JMP	dword ptr[EAX*0x4 + 0x00758670]


		LeaveSkillFeats:
		JMP	dword ptr[EndOfSkillFeat]
	}
}



//0x00756c96
__declspec(naked) void CallSkillAbility()
{
	__asm
	{
		PUSH ECX
		PUSH EDX

		MOVZX EDX, DX //Second parameter
		CALL MySkillAbility

		POP EDX
		POP ECX

		CMP EAX, -1
		JE AbilitySKillNotFound

		PUSH EDI
		MOV ESI, EAX
		JMP dword ptr[EndOfSkillAbility]

	AbilitySkillNotFound:
		MOV ESI, 0x0086443C
		MOV ESI, [ESI]
		JMP dword ptr[GoBackSkillAbility]
	}
}






bool ParseSkillModifier(int iRuleNumber, std::string sValue, int iSkillId, std::unordered_map<uint32_t, std::vector<SkillRuleSet*>>& myRuleMap)
{
	try {
		auto bonusCalc = MathExpressionParser::prepareSimplified(sValue, MathExpressionParser::Mode::INTEGER);
		SkillRuleSet* rs1 = new SkillRuleSet(bonusCalc, iRuleNumber);

		myRuleMap[iSkillId].push_back(rs1);
	} catch (const std::exception& e) {
		logger->Err("Error during parse of \"Bonus\" = %s : %s", sValue.c_str(), e.what());
		logger->Info("Due to the Error, the complete rule will not be applied");
		return false;
	}

	return true;
}

void CreateSkillAbility(int iRuleNumber, int iSkillId, int iAbility, std::unordered_map<uint32_t, std::vector<SkillRuleSet*>>& myRuleMap)
{
	SkillRuleSet* rs1 = new SkillRuleSet(iRuleNumber, iAbility);
	myRuleMap[iSkillId].push_back(rs1);
}


bool ParseRuleFeatOrAreaType(std::string sSkillFeatRuleX, int iRuleNumber, std::string sValue, int iSkillId, RuleParser::RuleType tFeatRule, std::unordered_map<uint32_t, std::vector<SkillRuleSet*>>& myRuleMap, bool bAllowRule)
{
	try {
		std::vector<RuleParser::Token> tokens1 = RuleParser::tokenize(sValue, tFeatRule, bAllowRule);
		int pos1                   = 0;
		RuleParser::Rule* root1                = RuleParser::parseExpression(tokens1, pos1);
		if (tFeatRule == RuleParser::RuleType::FEAT)
			myRuleMap[iSkillId].back()->featRule = root1;
		else if (tFeatRule == RuleParser::RuleType::AREA)
			myRuleMap[iSkillId].back()->areaTypeRule = root1;
		else
			myRuleMap[iSkillId].back()->extraRule = root1;

	} catch (const std::exception& e) {
		std::string sRuleType;

		if (tFeatRule == RuleParser::RuleType::FEAT)
			sRuleType = "Feat";
		else if (tFeatRule == RuleParser::RuleType::AREA)
			sRuleType = "Area";
		else
			sRuleType = "Extra";
		logger->Err("Error during parse of [%s] \"feat\" = %s : %s", sSkillFeatRuleX.c_str(),
			sRuleType.c_str(), sValue.c_str(), e.what());
		logger->Info("Due to the Error, the complete rule will not be applied");
		return false;
	}

	return true;
}


#define OFFS_SkillFeatStart		0x00756db3
#define OFFS_SkillAbilityStart	0x00756c96

//0x00756db3
Patch _SkillFeatPatches[] =
{
	Patch((DWORD)OFFS_SkillFeatStart, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6),
	Patch(OFFS_SkillFeatStart + 1, (relativefunc)CallSkillFeats),


	Patch()
};

Patch *SkillFeatPatches = _SkillFeatPatches;

//0x00756c96
Patch _SkillAbilityPatches[] =
{
	Patch((DWORD)OFFS_SkillAbilityStart, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6),
	Patch(OFFS_SkillAbilityStart + 1, (relativefunc)CallSkillAbility),


	Patch()
};

Patch *SkillAbilityPatches = _SkillAbilityPatches;

bool isAlreadyPatchedFeat = false;
bool isAlreadyPatchedAb = false;

void desinitSkillHooks()
{
	//Clear Rule Feat
	for (const auto& pair : m_skillFeatRuleSets) {
		for (auto ruleSet : pair.second) {
			delete ruleSet;
		}
	}
	m_skillFeatRuleSets.clear();


	//Clear Rule ability
	for (const auto& pair : m_skillAbilityRuleSets) {
		for (auto ruleSet : pair.second) {
			delete ruleSet;
		}
	}
	m_skillAbilityRuleSets.clear();
}

void initSkillHooks(std::string nxhome, std::string sFileName)
{
	std::string inifile(nxhome);
	inifile.append("\\");
	inifile.append(sFileName);

	// first, create a file instance
	mINI::INIFile file(inifile);

	// next, create a structure that will hold data
	mINI::INIStructure ini;
	bool bHasFeatRule = false;
	bool bHasAbilityRule = false;

	// now we can read the file
	bool bOkRead = file.read(ini);

	if (!bOkRead)
		logger->Err("Cant find SkillFile : %s", sFileName.c_str());

	iRemoveBaseSkillFeats = 0;
	if (bOkRead && ini.has("General"))
	{
		if (ini.get("General").has("RemoveBaseSkillFeatRules"))
		{
			if (ini.get("General").get("RemoveBaseSkillFeatRules") == "1")
			{
				iRemoveBaseSkillFeats = 1;
			}
		}

		if (ini.get("General").has("DisableHook"))
		{
			std::string sDisableHook = ini.get("General").get("DisableHook");

			bOkRead = !(sDisableHook == "1");
		}
	}

	//Skill Feats
	if(bOkRead)
	{
		//Now parse the SkillFeatRules
		int iRuleNumber = 1;
		std::string sSkillFeatRuleBase = "SkillRule";

		std::string sSkillFeatRuleX = sSkillFeatRuleBase + std::to_string(iRuleNumber);

		while (ini.has(sSkillFeatRuleX))
		{

			//We need a bonus. Else, error and no need to parse other stuff
			if (ini.get(sSkillFeatRuleX).has("Bonus") && ini.get(sSkillFeatRuleX).has("Skill"))
			{
				std::string sSkillBonus = ini.get(sSkillFeatRuleX).get("Bonus");
				std::string sSkillId = ini.get(sSkillFeatRuleX).get("Skill");
				int iSkillId = -1;
				
				try {
					iSkillId = std::stoi(sSkillId);
				}
				catch (const std::exception& ex) {
					iSkillId = -1;
				}
				

				if (iSkillId != -1 && ParseSkillModifier(iRuleNumber, sSkillBonus, iSkillId, m_skillFeatRuleSets))
				{
					//Ok, we can test and parse other fields
					bool bValidRule = true;

					//if (ini.get(sSkillAbilityRuleX).has("name"))

					if (ini.get(sSkillFeatRuleX).has("Area"))
					{
						std::string sValue = ini.get(sSkillFeatRuleX).get("Area");

						bValidRule = ParseRuleFeatOrAreaType(sSkillFeatRuleX, iRuleNumber, sValue, iSkillId,
							RuleParser::RuleType::AREA, m_skillFeatRuleSets);
					}

					if (bValidRule && ini.get(sSkillFeatRuleX).has("Feat"))
					{
						std::string sValue = ini.get(sSkillFeatRuleX).get("Feat");

						bValidRule = ParseRuleFeatOrAreaType(sSkillFeatRuleX, iRuleNumber, sValue, iSkillId,
							RuleParser::RuleType::FEAT, m_skillFeatRuleSets);
					}

					if (bValidRule && ini.get(sSkillFeatRuleX).has("Extra"))
					{
						std::string sValue = ini.get(sSkillFeatRuleX).get("Extra");

						bValidRule = ParseRuleFeatOrAreaType(sSkillFeatRuleX, iRuleNumber, sValue, iSkillId,
							RuleParser::RuleType::EXTRA, m_skillFeatRuleSets);
					}

					if (!bValidRule)
					{
						//Todo Remove this rule.
						SkillRuleSet* rsToDel = m_skillFeatRuleSets[iSkillId].back();
						m_skillFeatRuleSets[iSkillId].pop_back();
						delete rsToDel;
					}
					else {
						bHasFeatRule = true;
					}
				}
			}
			else
			{
				logger->Err("[SkillFeatsIniFile]: No Skill or bonus field for [%s].", sSkillFeatRuleX.c_str());
			}

			iRuleNumber++;
			sSkillFeatRuleX = sSkillFeatRuleBase + std::to_string(iRuleNumber);
		}
	}


	//Skill Ability
	if(bOkRead)
	{
		//Now parse the SkillAbilityRules
		int iRuleNumber = 1;
		std::string sSkillAbilityRuleBase = "SkillAbility";

		std::string sSkillAbilityRuleX = sSkillAbilityRuleBase + std::to_string(iRuleNumber);

		while (ini.has(sSkillAbilityRuleX))
		{
			//We need a bonus. Else, error and no need to parse other stuff
			if (ini.get(sSkillAbilityRuleX).has("Ability") && ini.get(sSkillAbilityRuleX).has("Skill"))
			{
				std::string sSkillAbility = ini.get(sSkillAbilityRuleX).get("Ability");
				std::string sSkillId = ini.get(sSkillAbilityRuleX).get("Skill");
				int iSkillId = -1;
				int iSkillAbility = -1;

				try {
					iSkillId = std::stoi(sSkillId);
				}
				catch (const std::exception& ex) {
					iSkillId = -1;
				}


				if (sSkillAbility == "STR")
					iSkillAbility = 0;
				else if (sSkillAbility == "DEX")
					iSkillAbility = 1;
				else if (sSkillAbility == "CON")
					iSkillAbility = 2;
				else if (sSkillAbility == "INT")
					iSkillAbility = 3;
				else if (sSkillAbility == "WIS")
					iSkillAbility = 4;
				else if (sSkillAbility == "CHA")
					iSkillAbility = 5;
				else if (sSkillAbility == "NOP")
					iSkillAbility = 6;




				if (iSkillId != -1 && iSkillAbility != -1)
				{
					CreateSkillAbility(iRuleNumber, iSkillId, iSkillAbility, m_skillAbilityRuleSets);
					//Ok, we can test and parse other fields
					bool bValidRule = true;

					if (ini.get(sSkillAbilityRuleX).has("Area"))
					{
						std::string sValue = ini.get(sSkillAbilityRuleX).get("Area");

						bValidRule = ParseRuleFeatOrAreaType(sSkillAbilityRuleX, iRuleNumber, sValue, iSkillId,
							RuleParser::RuleType::AREA, m_skillAbilityRuleSets);
					}

					if (bValidRule && ini.get(sSkillAbilityRuleX).has("Feat"))
					{
						std::string sValue = ini.get(sSkillAbilityRuleX).get("Feat");

						bValidRule = ParseRuleFeatOrAreaType(sSkillAbilityRuleX, iRuleNumber, sValue, iSkillId,
							RuleParser::RuleType::FEAT, m_skillAbilityRuleSets);
					}

					if (bValidRule && ini.get(sSkillAbilityRuleX).has("Extra"))
					{
						std::string sValue = ini.get(sSkillAbilityRuleX).get("Extra");

						bValidRule = ParseRuleFeatOrAreaType(sSkillAbilityRuleX, iRuleNumber, sValue, iSkillId,
							RuleParser::RuleType::EXTRA, m_skillAbilityRuleSets, false);
					}

					if (!bValidRule)
					{
						//Todo Remove this rule.
						SkillRuleSet* rsToDel = m_skillAbilityRuleSets[iSkillId].back();
						m_skillAbilityRuleSets[iSkillId].pop_back();
						delete rsToDel;
					}
					else {
						bHasAbilityRule = true;
					}
				}
			}
			else
			{
				logger->Err("[SkillFeatsIniFile]: No Skill or Ability fied for [%s].", sSkillAbilityRuleX.c_str());
			}

			iRuleNumber++;
			sSkillAbilityRuleX = sSkillAbilityRuleBase + std::to_string(iRuleNumber);
		}
	}

	//Change to bOkRead in order to allow to remove all the skill Feat. 
	// Base one with the RemoveBaseSkillFeatRules, and no more with a file without any SkillRules
	if (bOkRead && !isAlreadyPatchedFeat)
	{
		isAlreadyPatchedFeat = true;
		int i = 0;
		while(SkillFeatPatches[i].Apply()) {
			i++;
		}
	}
	else if (!bOkRead && isAlreadyPatchedFeat)
	{
		isAlreadyPatchedFeat = false;
		int i = 0;
		while(SkillFeatPatches[i].Remove()) {
			i++;
		}
	}

	//No need to keep the hook if you don't have any rule.
	if (bHasAbilityRule && !isAlreadyPatchedAb)
	{
		isAlreadyPatchedAb = true;
		int i = 0;
		while(SkillAbilityPatches[i].Apply()) {
			i++;
		}
	}
	else if (!bHasAbilityRule && isAlreadyPatchedAb)
	{
		isAlreadyPatchedAb = false;
		int i = 0;
		while (SkillAbilityPatches[i].Remove()) {
			i++;
		}
	}
}
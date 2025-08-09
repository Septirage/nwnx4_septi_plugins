
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

unsigned long ReturnHitPointFeatBeforeLastFeatEnd = 0x00753c87;//0x00753c82;//= 0x00753c7e;

#define OFFS_HitPointFeatStart		0x00753c82	
//0x00753c79


struct HPRuleSet {
//	std::string name;
	RuleParser::Rule* featRule;
	RuleParser::Rule* areaTypeRule;
	RuleParser::Rule* extraRule;
	MathExpressionParser::Expr* bonusCalculation;
	int index;


	HPRuleSet(MathExpressionParser::Expr* bonus, int idx)
	    : bonusCalculation(bonus)
	    , index(idx)
	{
		featRule = nullptr;
		areaTypeRule = nullptr;
		extraRule    = nullptr;
	}

	~HPRuleSet()
	{ 
		delete bonusCalculation;
		delete areaTypeRule;
		delete extraRule;
		delete featRule;
	}
};


std::vector<HPRuleSet*> m_hitPointFeatRuleSetList;


bool ParseRuleFeatOrAreaType(std::string sHitPointFeatRuleX, int iRuleNumber, std::string sValue, RuleParser::RuleType tFeatRule, std::vector<HPRuleSet*>& myRuleList)
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
		logger->Err("Error during parse of [%s] \"%s\" = %s : %s", sHitPointFeatRuleX.c_str(),
		            sRuleType.c_str(), sValue.c_str(), e.what());
		logger->Info("Due to the Error, the complete rule will not be applied");
		return false;
	}

	return true;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ParseHitPointModifier(int iRuleNumber, std::string sValue, std::vector<HPRuleSet*>& myRuleList, std::string sField)
{
	try {
		auto bonusCalc = MathExpressionParser::prepareSimplified(sValue, MathExpressionParser::Mode::INTEGER);
		HPRuleSet* rs1 = new HPRuleSet(bonusCalc, iRuleNumber);

		myRuleList.push_back(rs1);
	} catch (const std::exception& e) {

		logger->Err("Error during parse of \"%s\" = %s : %s", sField.c_str(), sValue.c_str(), e.what());
		logger->Info("Due to the Error, the complete rule will not be applied");
		return false;
	}

	return true;
}


//Insert more HP Calculation 
int32_t __fastcall hitPointFeatApplication(int pcBlock)
{
	int32_t iHPMod = 0;
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


	for (HPRuleSet* myRule : m_hitPointFeatRuleSetList) 
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
			iHPMod += myRule->bonusCalculation->evaluate(pcBlock);
		}
	}

	RuleParser::m_activatedRuleSet.clear();

	return iHPMod;
}




__declspec(naked) void CallHitPointFeats()
{
	__asm
	{
		MOV ECX, ESI

		CALL hitPointFeatApplication

		ADD EDI, EAX
		

		//prepare return
		MOV EDX, dword ptr [ESI]
		MOV EAX, dword ptr [EDX + 0x14]


		PUSH 0x704

		JMP		dword ptr[ReturnHitPointFeatBeforeLastFeatEnd]
	}
}


Patch _HitPointFeatPatches[] = {
	Patch((DWORD)OFFS_HitPointFeatStart, (char*)"\xe9", (int)1),
    Patch(OFFS_HitPointFeatStart + 1, (relativefunc)CallHitPointFeats),

	Patch()
};

Patch *HitPointFeatPatches = _HitPointFeatPatches;

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool isHPAlreadyPatched = false;

void desinitHPFeat()
{
	//Clear Rule Feat
	for (auto ruleSet : m_hitPointFeatRuleSetList) {
		delete ruleSet;
	}
	m_hitPointFeatRuleSetList.clear();
}

void initHPFeat(std::string nxhome, std::string sFileName)
{
	std::string inifile(nxhome);
	inifile.append("\\");
	inifile.append(sFileName);

	// first, create a file instance
	mINI::INIFile file(inifile);

	// next, create a structure that will hold data
	mINI::INIStructure ini;

	// now we can read the file
	bool bOkRead = file.read(ini);

	if (!bOkRead)
		logger->Err("Cant find HitPointFile : %s", sFileName.c_str());

	if (bOkRead && ini.has("General"))
	{
		if (ini.get("General").has("DisableHook"))
		{
			std::string sDisableHook = ini.get("General").get("DisableHook");

			bOkRead = !(sDisableHook == "1");
		}
	}

	//HitPointFeat
	if (bOkRead)
	{
		//Now parse the HPFeatRules
		int iRuleNumber = 1;
		std::string sHitPointFeatRuleBase = "HitPointRule";

		std::string sHitPointFeatRuleX = sHitPointFeatRuleBase + std::to_string(iRuleNumber);

		while (ini.has(sHitPointFeatRuleX))
		{
		
			//We need a bonus. Else, error and no need to parse other stuff
			if (ini.get(sHitPointFeatRuleX).has("Bonus"))
			{
				std::string sRuleHitPointMod = ini.get(sHitPointFeatRuleX).get("Bonus");

				if (ParseHitPointModifier(iRuleNumber, sRuleHitPointMod, m_hitPointFeatRuleSetList, "Bonus"))
				{
					//Ok, we can test and parse other fields
					bool bValidRule = true;

					//if (ini.get(sHitPointFeatRuleX).has("name"))

					if (ini.get(sHitPointFeatRuleX).has("Area"))
					{
						std::string sValue = ini.get(sHitPointFeatRuleX).get("Area");

						bValidRule = ParseRuleFeatOrAreaType(sHitPointFeatRuleX, iRuleNumber, sValue,
							RuleParser::RuleType::AREA, m_hitPointFeatRuleSetList);
					}

					if (bValidRule && ini.get(sHitPointFeatRuleX).has("Feat"))
					{
						std::string sValue = ini.get(sHitPointFeatRuleX).get("Feat");

						bValidRule = ParseRuleFeatOrAreaType(sHitPointFeatRuleX, iRuleNumber, sValue,
							RuleParser::RuleType::FEAT, m_hitPointFeatRuleSetList);
					}

					if (bValidRule && ini.get(sHitPointFeatRuleX).has("Extra"))
					{
						std::string sValue = ini.get(sHitPointFeatRuleX).get("Extra");

						bValidRule = ParseRuleFeatOrAreaType(sHitPointFeatRuleX, iRuleNumber, sValue,
							RuleParser::RuleType::EXTRA, m_hitPointFeatRuleSetList);
					}

					if (!bValidRule)
					{
						//Remove this rule.
						HPRuleSet* rsToDel = m_hitPointFeatRuleSetList.back();
						m_hitPointFeatRuleSetList.pop_back();
						delete rsToDel;
					}
				}
			}
			else
			{
				logger->Err("[HitPointFeatsIniFile]: No Bonus field for [%s].", sHitPointFeatRuleX.c_str());
			}

			iRuleNumber++;
			sHitPointFeatRuleX = sHitPointFeatRuleBase + std::to_string(iRuleNumber);
		}

	}


	//HitPointFeats
	if (bOkRead && !isHPAlreadyPatched)
	{
		isHPAlreadyPatched = true;
		int i = 0;
		while(HitPointFeatPatches[i].Apply()) {
			i++;
		}
	}
	else if (!bOkRead && isHPAlreadyPatched)
	{
		isHPAlreadyPatched = false;

		HitPointFeatPatches[1].Remove();
		HitPointFeatPatches[0].Remove();
		/*
		int i = 0;
		while(HitPointFeatPatches[i].Remove()) {
			i++;
		}
		*/
	}
}


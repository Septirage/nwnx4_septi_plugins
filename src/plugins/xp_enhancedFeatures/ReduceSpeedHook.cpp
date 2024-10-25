
#include "../../misc/log.h"
#include "../../septutil/mini.h"
#include "../../NWN2Lib/NWN2.h"
#include "../../NWN2Lib/NWN2Common.h"
#include "..\..\misc\Patch.h"

#include "../../septutil/NwN2DataPos.h"


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



#define OFFS_ReduceSpeedStart		0x005ed2af
const int EndOfReduceSpeedNumber = 0x005ED387;

struct ReduceSpeedRuleSet {
	//	std::string name;
	RuleParser::Rule* featRule;
	RuleParser::Rule* areaTypeRule;
	RuleParser::Rule* extraRule;
	MathExpressionParser::Expr* bonusCalculation;
	int index;


	ReduceSpeedRuleSet(MathExpressionParser::Expr* bonus, int idx)
		: bonusCalculation(bonus)
		, index(idx)
	{
		featRule = nullptr;
		areaTypeRule = nullptr;
		extraRule    = nullptr;
	}

	~ReduceSpeedRuleSet()
	{ 
		delete bonusCalculation;
		delete areaTypeRule;
		delete extraRule;
		delete featRule;
	}
};

std::vector<ReduceSpeedRuleSet*> m_reduceSpeedRuleList;


bool ParseRuleFeatOrAreaType(std::string sReduceSpeedRuleX, int iRuleNumber, std::string sValue, RuleParser::RuleType tFeatRule, std::vector<ReduceSpeedRuleSet*>& myRuleList, bool bAllowRule=true);


uint32_t __fastcall ApplyReduceSpeed(char* CreaPtr)
{
	int iBonusTotal = 0;
	//for every rules

	int CreaPartPtr = *(int*)(CreaPtr + AmCrtPtrAppBlock);


	int areaType = 0;

	//GetAreaType once for all
	{
		GameObject* Object;
		GameObjectManager m_ObjectManager;
		NWN::OBJECTID areaID = *(NWN::OBJECTID*)(CreaPtr + AmCommonArea);

		if ((areaID & NWN::INVALIDOBJID) != 0)
			areaID &= ~(NWN::LISTTYPE_MASK);

		if ((Object = m_ObjectManager.GetGameObject(areaID)) != NULL) 
		{
			if (Object->GetObjectType() == NWN::OBJECT_TYPE_AREA)
			{
				char* areaPtr = (char*)Object->AsArea();

				areaType = *(uint32_t*)(areaPtr + AmAreaFlag);
			}
		}
	}



	for (ReduceSpeedRuleSet* myRule : m_reduceSpeedRuleList)
	{
		bool bOk = true;
		if (myRule->featRule != nullptr)
		{
			bOk = RuleParser::evaluateRule(myRule->featRule, RuleParser::RuleType::FEAT, CreaPartPtr);
		}

		if (bOk && myRule->areaTypeRule != nullptr)
		{
			bOk = RuleParser::evaluateRule(myRule->areaTypeRule, RuleParser::RuleType::AREA, areaType);
		}

		if (bOk && myRule->extraRule != nullptr) {
			bOk = RuleParser::evaluateRule(myRule->extraRule, RuleParser::RuleType::EXTRA, (int)CreaPtr);
		}

		if (bOk)
		{
			RuleParser::m_activatedRuleSet.insert(myRule->index);
			iBonusTotal += myRule->bonusCalculation->evaluateInt((int)CreaPartPtr);
		}
	}

	if (iBonusTotal < 0)
	{
		iBonusTotal = 0;
	}

	RuleParser::m_activatedRuleSet.clear();
	return iBonusTotal;
}

//0x00756db3
__declspec(naked) void CallReduceSpeed()
{
	__asm
	{
		CALL ApplyReduceSpeed

		MOV dword ptr[ESP+0x10], EAX

		JMP dword ptr[EndOfReduceSpeedNumber]
	}
}

#define OFFS_DontShareDetect	0x00565A23
const int EndOfDontShareDetect = 0x00565A2A;

__declspec(naked) void DontShareDetect()
{
	__asm
	{
		MOVZX   ECX, word ptr [EDI + 0x0F28]
		PUSHF 
		AND		ECX, 0xFFFFFFFD
		POPF 
		JMP	dword ptr[EndOfDontShareDetect]

	}
}


bool ParseReduceSpeedModifier(int iRuleNumber, std::string sValue, std::vector<ReduceSpeedRuleSet*>& myRuleList)
{
	try {
		auto bonusCalc = MathExpressionParser::prepareSimplified(sValue, MathExpressionParser::Mode::INTEGER);
		ReduceSpeedRuleSet* rs1 = new ReduceSpeedRuleSet(bonusCalc, iRuleNumber);

		myRuleList.push_back(rs1);
	} catch (const std::runtime_error& e) {
		logger->Err("Error during parse of \"Impact\" = %s : %s", sValue.c_str(), e.what());
		logger->Info("Due to the Error, the complete rule will not be applied");
		return false;
	}

	return true;
}

bool ParseRuleFeatOrAreaType(std::string sReduceSpeedRuleX, int iRuleNumber, std::string sValue, RuleParser::RuleType tFeatRule, std::vector<ReduceSpeedRuleSet*>& myRuleList, bool bAllowRule)
{
	try {
		std::vector<RuleParser::Token> tokens1 = RuleParser::tokenize(sValue, tFeatRule, bAllowRule);
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
		logger->Err("Error during parse of [%s] \"feat\" = %s : %s", sReduceSpeedRuleX.c_str(),
			sRuleType.c_str(), sValue.c_str(), e.what());
		logger->Info("Due to the Error, the complete rule will not be applied");
		return false;
	}

	return true;
}

//0x00756db3
Patch _ReduceSpeedPatches[] =
{
	Patch((DWORD)OFFS_ReduceSpeedStart, (char*)"\xe9\x00\x00\x00\x00", (int)5),
	Patch(OFFS_ReduceSpeedStart + 1, (relativefunc)CallReduceSpeed),

	Patch((DWORD)OFFS_DontShareDetect, (char*)"\xe9\x00\x00\x00\x00\x90\x90", (int)7),
	Patch(OFFS_DontShareDetect + 1, (relativefunc)DontShareDetect),


	Patch()
};

Patch *ReduceSpeedPatches = _ReduceSpeedPatches;

bool isAlreadyPatchedReduceSpeed = false;

void desinitReduceSpeedHooks()
{
	//Clear Rule
	for (auto ruleSet : m_reduceSpeedRuleList) {
		delete ruleSet;
	}
	m_reduceSpeedRuleList.clear();
}

void initReduceSpeedHooks(std::string nxhome, std::string sFileName)
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


	if(!bOkRead)
	{
		logger->Err("Cant find ReduceSpeedFile : %s", sFileName.c_str());
	}
	
	if (bOkRead && ini.has("General"))
	{
		if (ini.get("General").has("DisableHook"))
		{
			std::string sDisableHook = ini.get("General").get("DisableHook");

			bOkRead = !(sDisableHook == "1");
		}
	}
	

	//ReduceSpeed Rule
	if(bOkRead)
	{
		//Now parse the Rules
		int iRuleNumber = 1;
		std::string sReduceSpeedRuleBase = "ReduceSpeedRule";

		std::string sReduceSpeedRuleX = sReduceSpeedRuleBase + std::to_string(iRuleNumber);

		while (ini.has(sReduceSpeedRuleX))
		{

			//We need a Impact. Else, error and no need to parse other stuff
			if (ini.get(sReduceSpeedRuleX).has("Impact"))
			{
				std::string sReduceSpeedBonus = ini.get(sReduceSpeedRuleX).get("Impact");				

				if (ParseReduceSpeedModifier(iRuleNumber, sReduceSpeedBonus, m_reduceSpeedRuleList))
				{
					//Ok, we can test and parse other fields
					bool bValidRule = true;

					if (ini.get(sReduceSpeedRuleX).has("Area"))
					{
						std::string sValue = ini.get(sReduceSpeedRuleX).get("Area");

						bValidRule = ParseRuleFeatOrAreaType(sReduceSpeedRuleX, iRuleNumber, sValue, 
							RuleParser::RuleType::AREA, m_reduceSpeedRuleList);
					}

					if (bValidRule && ini.get(sReduceSpeedRuleX).has("Feat"))
					{
						std::string sValue = ini.get(sReduceSpeedRuleX).get("Feat");

						bValidRule = ParseRuleFeatOrAreaType(sReduceSpeedRuleX, iRuleNumber, sValue,
							RuleParser::RuleType::FEAT, m_reduceSpeedRuleList);
					}

					if (bValidRule && ini.get(sReduceSpeedRuleX).has("Extra"))
					{
						std::string sValue = ini.get(sReduceSpeedRuleX).get("Extra");

						bValidRule = ParseRuleFeatOrAreaType(sReduceSpeedRuleX, iRuleNumber, sValue,
							RuleParser::RuleType::EXTRA, m_reduceSpeedRuleList);
					}

					if (!bValidRule)
					{
						//Todo Remove this rule.
						ReduceSpeedRuleSet* rsToDel = m_reduceSpeedRuleList.back();
						m_reduceSpeedRuleList.pop_back();
						delete rsToDel;
					}
				}
			}
			else
			{
				logger->Err("[ReduceSpeedIniFile]: No Impact field for [%s].", sReduceSpeedRuleX.c_str());
			}

			iRuleNumber++;
			sReduceSpeedRuleX = sReduceSpeedRuleBase + std::to_string(iRuleNumber);
		}
	}

	if (bOkRead && !isAlreadyPatchedReduceSpeed)
	{
		isAlreadyPatchedReduceSpeed = true;
		int i = 0;
		while(ReduceSpeedPatches[i].Apply()) {
			i++;
		}
	}
	else if (!bOkRead && isAlreadyPatchedReduceSpeed)
	{
		isAlreadyPatchedReduceSpeed = false;
		int i = 0;
		while(ReduceSpeedPatches[i].Remove()) {
			i++;
		}
	}
}
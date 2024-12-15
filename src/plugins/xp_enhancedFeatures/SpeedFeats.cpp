
#include <misc/log.h>
#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include <misc\Patch.h>
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

///////////////////BENCHMARKSTUFF : define BENCHMARKSPEED TO Test the PluginVersion, define BENCHARKSPEEDOLDVERSION TO TEST THE base game version

#if defined(BENCHMARKSPEED) || defined(BENCHMARKSPEEDOLDVERSION)

#include <algorithm>
#include <chrono>


std::chrono::time_point<std::chrono::steady_clock> ttime_start;


    size_t ttime_times = 12000;
	size_t ttime_j = 0;
    double ttime_total = 0.0;
    double ttime_max   = 0.0;



	void startStepPerfCount()
	{ 
		ttime_start = std::chrono::steady_clock::now();
	}

	void endStepPerfCount()
	{
		ttime_j++;
	    auto countEnd = std::chrono::steady_clock::now();


        std::chrono::duration<double, std::milli> countDur = countEnd - ttime_start;
	    double value                                       = countDur.count();
	    if (ttime_max < value)
		    ttime_max = value;
	    ttime_total += value;

		if (ttime_j == ttime_times)
		{
		    double div = ttime_times;
		    ttime_j    = 0;
			

			double mean = ttime_total / div;

			ttime_total = 0;


			logger->Err("Mean time on 12000 execution : %f / Max : %f", mean, ttime_max);

		}


	}

#endif 
///////////// End of IFDEF BENCHMARKSPEED


unsigned long ReturnSpeedFeatBeforeSpecialFeatEnd		= 0x005E804D;

//TODO, put inside the object/header file
bool m_CalculationTypeSum = false;
bool m_useSpeedFeats      = false;


struct RuleSet {
//	std::string name;
	RuleParser::Rule* featRule;
	RuleParser::Rule* areaTypeRule;
	RuleParser::Rule* extraRule;
	MathExpressionParser::Expr* bonusCalculation;
	int index;


	RuleSet(MathExpressionParser::Expr* bonus, int idx)
	    : bonusCalculation(bonus)
	    , index(idx)
	{
		featRule = nullptr;
		areaTypeRule = nullptr;
		extraRule    = nullptr;
	}

	~RuleSet()
	{ 
		delete bonusCalculation;
		delete areaTypeRule;
		delete extraRule;
		delete featRule;
	}
};


std::vector<RuleSet*> m_speedFeatRuleSetList;
std::vector<RuleSet*> m_minSpeedRuleSetList;
std::vector<RuleSet*> m_maxSpeedRuleSetList;


bool ParseRuleFeatOrAreaType(std::string sSpeedFeatRuleX, int iRuleNumber, std::string sValue, RuleParser::RuleType tFeatRule, std::vector<RuleSet*>& myRuleList)
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
		logger->Err("Error during parse of [%s] \"%s\" = %s : %s", sSpeedFeatRuleX.c_str(),
		            sRuleType.c_str(), sValue.c_str(), e.what());
		logger->Info("Due to the Error, the complete rule will not be applied");
		return false;
	}

	return true;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

bool ParseSpeedModifier(int iRuleNumber, std::string sValue, std::vector<RuleSet*>& myRuleList, std::string sField)
{
	try {
		auto bonusCalc = MathExpressionParser::prepareSimplified(sValue, MathExpressionParser::Mode::FLOAT);
		RuleSet* rs1 = new RuleSet(bonusCalc, iRuleNumber);

		myRuleList.push_back(rs1);
	} catch (const std::exception& e) {

		logger->Err("Error during parse of \"%s\" = %s : %s", sField.c_str(), sValue.c_str(), e.what());
		logger->Info("Due to the Error, the complete rule will not be applied");
		return false;
	}

	return true;
}

#define OFFS_SpeedFeatStart			0x0005e7e16



//Patch Speed Calculation (stop just before the special feat for now)
void __fastcall speedFeatApplication(int pcPtr, float* pfSpeedBase)
{
#ifdef BENCHMARKSPEED
	startStepPerfCount();
#endif

	float fSpeedBase = *pfSpeedBase;
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



	int pcBlock = *(int*)(pcPtr + 0x1fc4);
	RuleParser::m_activatedRuleSet.clear();


	for (RuleSet* myRule : m_speedFeatRuleSetList) 
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
			float fMod = myRule->bonusCalculation->evaluate(pcBlock);
			if (m_CalculationTypeSum)
				fSpeedBase += fMod;
			else
				fSpeedBase *= fMod;
		}
	}

	//Test MinValue, stop at first ok !
	for (RuleSet* myRule : m_minSpeedRuleSetList)
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
			float fMin = myRule->bonusCalculation->evaluate(pcBlock);
			if (fSpeedBase < fMin)
			{
				fSpeedBase = fMin;
				//No need to test Max, just return.
				*pfSpeedBase = fSpeedBase;
				return;
			}

			//Only first matter
			break;
		}
	}

	// Test MaxValue, stop at first ok !
	for (RuleSet* myRule : m_maxSpeedRuleSetList) {
		bool bOk = true;
		if (myRule->featRule != nullptr) {
			bOk = RuleParser::evaluateRule(myRule->featRule, RuleParser::RuleType::FEAT, pcBlock);
		}

		if (bOk && myRule->areaTypeRule != nullptr) {
			bOk = RuleParser::evaluateRule(myRule->areaTypeRule, RuleParser::RuleType::AREA, areaType);
		}

		if (bOk && myRule->extraRule != nullptr) {
			bOk = RuleParser::evaluateRule(myRule->extraRule, RuleParser::RuleType::EXTRA, pcPtr);
		}

		if (bOk) {
			float fMax = myRule->bonusCalculation->evaluate(pcBlock);
			if (fSpeedBase > fMax) {
				fSpeedBase = fMax;
			}

			// Only first matter
			break;
		}
	}
	RuleParser::m_activatedRuleSet.clear();

	*pfSpeedBase = fSpeedBase;

#ifdef BENCHMARKSPEED
	endStepPerfCount();
#endif
}




__declspec(naked) void CallSpeedFeats()
{
	__asm
	{
		MOV ECX, ESI
		LEA EDX, [ESP+0x8]

		CALL speedFeatApplication

		//fstp dword ptr [esp + 0x8]

		JMP		dword ptr[ReturnSpeedFeatBeforeSpecialFeatEnd]
	}
}


#ifdef BENCHMARKSPEEDOLDVERSION

void __fastcall CalcStartStepClock() { startStepPerfCount(); }
void __fastcall CalcEndStepClock() { endStepPerfCount(); }

unsigned long ReturnStartStep = 0x5e7e1f;
#define FORTEST_JUSTBEFORESTUFF  0x005E804D
unsigned long ReturnEndStep = 0x005e8054;
unsigned long FUN_005c0540  = 0x005c0540;

__declspec(naked) void CallTestSpeedFeats()
{
	__asm
	{
		PUSH EAX
		PUSH ECX
		PUSH EDX
		CALL CalcStartStepClock

		POP EDX
		POP ECX
		POP EAX

		PUSH 0X0
		PUSH 0X1
		PUSH 0x8C8
		JMP dword ptr[ReturnStartStep]
	}
}

__declspec(naked) void CallEndTestSpeedFeats()
{
	__asm
	{
		PUSH EAX
		PUSH EDX

		CALL CalcEndStepClock

		POP EDX
		POP EAX


		MOV ECX, ESI
		CALL dword ptr[FUN_005c0540]


		JMP dword ptr[ReturnEndStep]
	}
}
#endif 


Patch _SpeedFeatPatches[] = {
#ifndef BENCHMARKSPEEDOLDVERSION
    Patch((DWORD)OFFS_SpeedFeatStart, (char*)"\xe9\x00\x00\x00\x00\x90\x90\x90\x90", (int)9),
    Patch(OFFS_SpeedFeatStart + 1, (relativefunc)CallSpeedFeats),

#else
	// For benchmark without the SpeedFeatSolution
    Patch((DWORD)OFFS_SpeedFeatStart, (char*)"\xe9\x00\x00\x00\x00\x90\x90\x90\x90", (int)9),
    Patch(OFFS_SpeedFeatStart + 1, (relativefunc)CallTestSpeedFeats),

    Patch((DWORD)FORTEST_JUSTBEFORESTUFF, (char*)"\xe9\x00\x00\x00\x00\x90", (int)7),
    Patch(FORTEST_JUSTBEFORESTUFF + 1, (relativefunc)CallEndTestSpeedFeats),
#endif

	Patch()
};

Patch *SpeedFeatPatches = _SpeedFeatPatches;

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool isSpeedAlreadyPatched = false;

void desinitSpeedFeat()
{
	//Clear Rule Feat
	for (auto ruleSet : m_speedFeatRuleSetList) {
		delete ruleSet;
	}
	m_speedFeatRuleSetList.clear();


	//Clear Rule MinSpeed
	for (auto ruleSet : m_minSpeedRuleSetList) {
		delete ruleSet;
	}
	m_minSpeedRuleSetList.clear();


	//Clear Rule MaxSpeed
	for (auto ruleSet : m_maxSpeedRuleSetList) {
		delete ruleSet;
	}
	m_maxSpeedRuleSetList.clear();
}

void initSpeedFeat(std::string nxhome, std::string sFileName)
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
		logger->Err("Cant find SpeedFile : %s", sFileName.c_str());

	m_useSpeedFeats = true;

	if (bOkRead && ini.has("General"))
	{

		if (ini.get("General").has("CalculationType"))
		{
			std::string sValue     = ini.get("General").get("CalculationType");
			std::string lowerValue = sValue;
			std::transform(lowerValue.begin(), lowerValue.end(), lowerValue.begin(), ::tolower);
			if (lowerValue == "sum")
			{
				m_CalculationTypeSum = true;
			} else if (lowerValue == "factor") {
				m_CalculationTypeSum = false;
			}
		}

		if (ini.get("General").has("DisableHook"))
		{
			std::string sDisableHook = ini.get("General").get("DisableHook");

			bOkRead = !(sDisableHook == "1");
		}
	}

	//SpeedFeat
	if (bOkRead && m_useSpeedFeats)
	{
		//Now parse the SpeedFeatRules
		int iRuleNumber = 1;
		std::string sSpeedFeatRuleBase = "SpeedRule";

		std::string sSpeedFeatRuleX = sSpeedFeatRuleBase + std::to_string(iRuleNumber);

		while (ini.has(sSpeedFeatRuleX))
		{
		
			//We need a bonus. Else, error and no need to parse other stuff
			if (ini.get(sSpeedFeatRuleX).has("Bonus"))
			{
				std::string sRuleSpeedMod = ini.get(sSpeedFeatRuleX).get("Bonus");

				//If we do a factor (default stuff), we need to add 1.0 to the bonus. Else it will reduce everything.
				if (!m_CalculationTypeSum) {
					sRuleSpeedMod += " + 1.0";
				}

				if (ParseSpeedModifier(iRuleNumber, sRuleSpeedMod, m_speedFeatRuleSetList, "Bonus"))
				{
					//Ok, we can test and parse other fields
					bool bValidRule = true;

					//if (ini.get(sSpeedFeatRuleX).has("name"))

					if (ini.get(sSpeedFeatRuleX).has("Area"))
					{
						std::string sValue = ini.get(sSpeedFeatRuleX).get("Area");

						bValidRule = ParseRuleFeatOrAreaType(sSpeedFeatRuleX, iRuleNumber, sValue,
							RuleParser::RuleType::AREA, m_speedFeatRuleSetList);
					}

					if (bValidRule && ini.get(sSpeedFeatRuleX).has("Feat"))
					{
						std::string sValue = ini.get(sSpeedFeatRuleX).get("Feat");

						bValidRule = ParseRuleFeatOrAreaType(sSpeedFeatRuleX, iRuleNumber, sValue,
							RuleParser::RuleType::FEAT, m_speedFeatRuleSetList);
					}

					if (bValidRule && ini.get(sSpeedFeatRuleX).has("Extra"))
					{
						std::string sValue = ini.get(sSpeedFeatRuleX).get("Extra");

						bValidRule = ParseRuleFeatOrAreaType(sSpeedFeatRuleX, iRuleNumber, sValue,
							RuleParser::RuleType::EXTRA, m_speedFeatRuleSetList);
					}

					if (!bValidRule)
					{
						//Todo Remove this rule.
						RuleSet* rsToDel = m_speedFeatRuleSetList.back();
						m_speedFeatRuleSetList.pop_back();
						delete rsToDel;
					}
				}
			}
			else
			{
				logger->Err("[SpeedFeatsIniFile]: No Bonus field for [%s].", sSpeedFeatRuleX.c_str());
			}

			iRuleNumber++;
			sSpeedFeatRuleX = sSpeedFeatRuleBase + std::to_string(iRuleNumber);
		}

	}

	//Get List for Min then for Max (maxMonkSpeed will only be a specific max)

	//Min speed
	if(bOkRead)
	{

		int iRuleNumber                = 1;
		std::string sMinSpeedRuleBase = "MinSpeedRule";

		std::string sMinSpeedRuleX = sMinSpeedRuleBase + std::to_string(iRuleNumber);

		while (ini.has(sMinSpeedRuleX)) 
		{
			// We need a bonus. Else, error and no need to parse other stuff
			if (ini.get(sMinSpeedRuleX).has("Min")) {
				std::string sRuleSpeedMod = ini.get(sMinSpeedRuleX).get("Min");

				if (ParseSpeedModifier(iRuleNumber, sRuleSpeedMod, m_minSpeedRuleSetList, "Min")) {
					// Ok, we can test and parse other fields
					bool bValidRule = true;

					// if (ini.get(sSpeedFeatRuleX).has("name"))

					if (ini.get(sMinSpeedRuleX).has("Area")) {
						std::string sValue = ini.get(sMinSpeedRuleX).get("Area");

						bValidRule = ParseRuleFeatOrAreaType(sMinSpeedRuleX, iRuleNumber, sValue,
							RuleParser::RuleType::AREA, m_minSpeedRuleSetList);
					}

					if (bValidRule && ini.get(sMinSpeedRuleX).has("Feat")) {
						std::string sValue = ini.get(sMinSpeedRuleX).get("Feat");

						bValidRule = ParseRuleFeatOrAreaType(sMinSpeedRuleX, iRuleNumber, sValue,
							RuleParser::RuleType::FEAT, m_minSpeedRuleSetList);
					}

					if (bValidRule && ini.get(sMinSpeedRuleX).has("Extra"))
					{
						std::string sValue = ini.get(sMinSpeedRuleX).get("Extra");

						bValidRule = ParseRuleFeatOrAreaType(sMinSpeedRuleX, iRuleNumber, sValue,
							RuleParser::RuleType::EXTRA, m_minSpeedRuleSetList);
					}

					if (!bValidRule) {
						// Remove this rule.
						RuleSet* rsToDel = m_minSpeedRuleSetList.back();
						m_minSpeedRuleSetList.pop_back();
						delete rsToDel;
					}
				}
			} else {
				logger->Err("[SpeedFeatsIniFile]: No Min field for [%s].",
				            sMinSpeedRuleX.c_str());
			}

			iRuleNumber++;
			sMinSpeedRuleX = sMinSpeedRuleBase + std::to_string(iRuleNumber);
		}
	}

	//MaxSpeed
	if(bOkRead)
	{
		int iRuleNumber               = 1;
		std::string sMaxSpeedRuleBase = "MaxSpeedRule";

		std::string sMaxSpeedRuleX = sMaxSpeedRuleBase + std::to_string(iRuleNumber);

		while (ini.has(sMaxSpeedRuleX)) {
			// We need a bonus. Else, error and no need to parse other stuff
			if (ini.get(sMaxSpeedRuleX).has("Max")) {
				std::string sRuleSpeedMod = ini.get(sMaxSpeedRuleX).get("Max");

				if (ParseSpeedModifier(iRuleNumber, sRuleSpeedMod, m_maxSpeedRuleSetList, "Max")) {
					// Ok, we can test and parse other fields
					bool bValidRule = true;

					// if (ini.get(sSpeedFeatRuleX).has("name"))

					if (ini.get(sMaxSpeedRuleX).has("Area")) {
						std::string sValue = ini.get(sMaxSpeedRuleX).get("Area");

						bValidRule = ParseRuleFeatOrAreaType(sMaxSpeedRuleX, iRuleNumber, sValue,
							RuleParser::RuleType::AREA, m_maxSpeedRuleSetList);
					}

					if (bValidRule && ini.get(sMaxSpeedRuleX).has("Feat")) {
						std::string sValue = ini.get(sMaxSpeedRuleX).get("Feat");

						bValidRule = ParseRuleFeatOrAreaType(sMaxSpeedRuleX, iRuleNumber, sValue,
							RuleParser::RuleType::FEAT, m_maxSpeedRuleSetList);
					}

					if (bValidRule && ini.get(sMaxSpeedRuleX).has("Extra"))
					{
						std::string sValue = ini.get(sMaxSpeedRuleX).get("Extra");

						bValidRule = ParseRuleFeatOrAreaType(sMaxSpeedRuleX, iRuleNumber, sValue,
							RuleParser::RuleType::EXTRA, m_maxSpeedRuleSetList);
					}

					if (!bValidRule) {
						// Remove this rule.
						RuleSet* rsToDel = m_maxSpeedRuleSetList.back();
						m_maxSpeedRuleSetList.pop_back();
						delete rsToDel;
					}
				}
			} else {
				logger->Err("[SpeedFeatsIniFile]: No Max field for [%s].",  sMaxSpeedRuleX.c_str());
			}

			iRuleNumber++;
			sMaxSpeedRuleX = sMaxSpeedRuleBase + std::to_string(iRuleNumber);
		}
	}

	//CallSpeedFeats
	if (bOkRead && !isSpeedAlreadyPatched)
	{
		isSpeedAlreadyPatched = true;
		int i = 0;
		while(SpeedFeatPatches[i].Apply()) {
			i++;
		}
	}
	else if (!bOkRead && isSpeedAlreadyPatched)
	{
		isSpeedAlreadyPatched = false;
		int i = 0;
		while(SpeedFeatPatches[i].Remove()) {
			i++;
		}
	}
}


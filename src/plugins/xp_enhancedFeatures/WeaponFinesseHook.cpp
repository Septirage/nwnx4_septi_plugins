

#include <misc/log.h>
#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include <misc/Patch.h>

#include "../../septutil/mini.h"
#include "../../septutil/NwN2DataPos.h"

#include <unordered_set>
#include <set>
#include "RuleParser.h"
#include "BonusParser.h"
#include "WeaponFinesseHook.h"

extern std::unique_ptr<LogNWNX> logger;

#define OFFS_WeaponFinesseAll			0x007563a0

#define FCT_WeaponCreatureSizeDiff 0x007547a0

int iRemoveBaseWeaponFinesse = 0;

struct WeaponFinesseRuleSet {

	RuleParser::Rule* featRule;
	RuleParser::Rule* areaTypeRule;
	RuleParser::Rule* extraRule;

	bool bTestSize;

	WeaponFinesseRuleSet(bool bTSize) : bTestSize(bTSize)
	{
		featRule = nullptr;
		areaTypeRule = nullptr;
		extraRule = nullptr;
	}

	~WeaponFinesseRuleSet()
	{
		delete featRule;
		delete areaTypeRule;
		delete extraRule;
	}
};



std::unordered_map<uint32_t, std::vector<WeaponFinesseRuleSet*>> m_weaponFinesseRuleSets;
std::vector<WeaponFinesseRuleSet*> m_ListOfRuleSet;



__declspec(naked) int __fastcall WeaponSizeMinusCreatureSize_Intern(__in void* pCreature, __in void* Unused, __in void* pWeapon, __in int creatureSize)
{
	__asm
	{
		mov             edx, FCT_WeaponCreatureSizeDiff;
		jmp             edx;
	}
}

int WeaponSizeMinusCreatureSize(void* pCreature, void* pWeapon, int creatureSize)
{
	return WeaponSizeMinusCreatureSize_Intern(pCreature, NULL, pWeapon, creatureSize);
}

uint32_t __fastcall CheckWeaponFinesseAll(char* pCreature, char* pWeapon, int creatureSize)
{
	uint32_t iWeaponType = *(uint32_t*)(pWeapon + AmItmType);
	int iSizeDiff = WeaponSizeMinusCreatureSize(pCreature, pWeapon, creatureSize);

	int iValid = 0;
	bool bAreaType = false;
	int areaType = 0;

	//Do we have rule for this weapon type ?
	if (m_weaponFinesseRuleSets.find(iWeaponType) != m_weaponFinesseRuleSets.end())
	{
		for (WeaponFinesseRuleSet* myRule : m_weaponFinesseRuleSets[iWeaponType])
		{
			bool bOk = true;

			//We test size and weapon size is too big
			if (myRule->bTestSize && iSizeDiff > 0)
			{
				bOk = false;
			}

			if (bOk && myRule->featRule != nullptr)
			{
				bOk = RuleParser::evaluateRule(myRule->featRule, RuleParser::RuleType::FEAT, (int)pCreature);
			}

			//Not really used for this type of rules, so get the areaType only if needed
			if (bOk && myRule->areaTypeRule != nullptr)
			{
				if (!bAreaType)
				{
					bAreaType = true;
					{
						GameObject* Object;
						GameObjectManager m_ObjectManager;
						*(int*)(pCreature + 0xA4);
						
						NWN::OBJECTID areaID = *(NWN::OBJECTID*)((*(int*)(pCreature+0xA4)) + 0x144);

						if ((areaID & NWN::INVALIDOBJID) != 0)
							areaID &= ~(NWN::LISTTYPE_MASK);

						if ((Object = m_ObjectManager.GetGameObject(areaID)) != NULL) {
							if (Object->GetObjectType() == NWN::OBJECT_TYPE_AREA) {
								char* areaPtr = (char*)Object->AsArea();

								areaType = *(uint32_t*)(areaPtr + 0x8);
							}
						}
					}
				}
				bOk = RuleParser::evaluateRule(myRule->areaTypeRule, RuleParser::RuleType::AREA, areaType);
			}

			if (bOk && myRule->extraRule != nullptr) {
				bOk = RuleParser::evaluateRule(myRule->extraRule, RuleParser::RuleType::EXTRA, *(int*)(pCreature+0xA4));
			}

			//Validated, we can stop now
			if (bOk)
			{
				iValid = 1;
				break;
			}
		}
	}

	return iValid;
}

// LAB_00756431  <= OK
// 

const int WeaponFinesseIsOk = 0x00756431;
const int WeaponFinesseIsKo = 0x007563e1;
const int WeaponFinContinue = 0x007563a5;

__declspec(naked) void TestWeaponFinesseAll()
{
	//ECX == pCreature (will be in ESI)
	//dword ptr [ESP + 0x14] == creatureSize (will use ebx)
	//dword ptr [ESP + 0x10] == pWeapon ( will use edi)
	__asm
	{
		PUSH EBX
		PUSH ESI
		PUSH EDI

		MOV	ESI, ECX

		MOV	EBX, dword ptr[ESP + 0x14]
		MOV EDI, dword ptr[ESP + 0x10]

		TEST EDI, EDI
		JZ	GoToWeaponFinIsOK

		PUSH EBX
		MOV	EDX, EDI
		CALL CheckWeaponFinesseAll

		CMP EAX, 0X1
		JE	GoToWeaponFinIsOK

		CMP iRemoveBaseWeaponFinesse, 0x1
		JE	GoToWeaponFinIsKo

		//Go Back to "normal behavior"
		MOV	ECX, ESI
		PUSH 0x0
		jmp dword ptr[WeaponFinContinue]

GoToWeaponFinIsOK:
		JMP	dword ptr[WeaponFinesseIsOk]

GoToWeaponFinIsKo:
		JMP dword ptr[WeaponFinesseIsKo]
	}
}


Patch _PatchWeaponFinesse[] =
{
	Patch(OFFS_WeaponFinesseAll, (char*)"\xe9\x00\x00\x00\x00", (int)5),
	Patch(OFFS_WeaponFinesseAll + 1, (relativefunc)TestWeaponFinesseAll),

	Patch()
};
Patch *PatchWeaponFinesse = _PatchWeaponFinesse;


bool isWFAlreadyPatched = false;

bool ParseRuleFeatOrAreaType(std::string sWpnFinesseRuleX, int iRuleNumber, std::string sValue, RuleParser::RuleType tFeatRule, std::set<uint32_t>& itemTypeList)
{
	try {
		std::vector<RuleParser::Token> tokens1 = RuleParser::tokenize(sValue, tFeatRule, false);
		int pos1                   = 0;
		RuleParser::Rule* root1                = RuleParser::parseExpression(tokens1, pos1);

		for (const auto& value : itemTypeList)
		{
			if (tFeatRule == RuleParser::RuleType::FEAT)
				m_weaponFinesseRuleSets[value].back()->featRule = root1;
			else if (tFeatRule == RuleParser::RuleType::AREA)
				m_weaponFinesseRuleSets[value].back()->areaTypeRule = root1;
			else
				m_weaponFinesseRuleSets[value].back()->extraRule = root1;
		}

	} catch (const std::exception& e) {
		std::string sRuleType;

		if (tFeatRule == RuleParser::RuleType::FEAT)
			sRuleType = "Feat";
		else if (tFeatRule == RuleParser::RuleType::AREA)
			sRuleType = "Area";
		else
			sRuleType = "Extra";
		logger->Err("Error during parse of [%s] \"%s\" = %s : %s", sWpnFinesseRuleX.c_str(),
			sRuleType.c_str(), sValue.c_str(), e.what());
		logger->Info("Due to the Error, the complete rule will not be applied");
		return false;
	}

	return true;
}


void CreateWeaponFinesseRule(bool bCheckSize, std::set<uint32_t>& itemTypeList)
{
	WeaponFinesseRuleSet* rs1 = new WeaponFinesseRuleSet(bCheckSize);
	m_ListOfRuleSet.push_back(rs1);

	for (const auto& value : itemTypeList)
	{
		m_weaponFinesseRuleSets[value].push_back(rs1);
	}
}

void RemoveLastRule(std::set<uint32_t>& itemTypeList)
{
	for (const auto& value : itemTypeList)
	{
		m_weaponFinesseRuleSets[value].pop_back();
	}
	//Remove this rule.
	WeaponFinesseRuleSet* rsToDel = m_ListOfRuleSet.back();
	m_ListOfRuleSet.pop_back();
	delete rsToDel;
}

void desinitWeaponFinesse()
{
	//Clear WeaponFinesse Rules
	for(const auto& ruleS: m_ListOfRuleSet) {
		delete ruleS;
	}
	m_ListOfRuleSet.clear();
	m_weaponFinesseRuleSets.clear();
}

void initWeaponFinesse(std::string nxhome, std::string sFileName)
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
		logger->Err("Cant find WeaponFinesseFile : %s", sFileName.c_str());

	iRemoveBaseWeaponFinesse = 0;

	if (bOkRead && ini.has("General"))
	{
		if (ini.get("General").has("RemoveBaseWpnFinesseRules"))
		{
			if (ini.get("General").get("RemoveBaseWpnFinesseRules") == "1")
			{
				iRemoveBaseWeaponFinesse = 1;
			}
		}

		if (ini.get("General").has("DisableHook"))
		{
			std::string sDisableHook = ini.get("General").get("DisableHook");

			bOkRead = !(sDisableHook == "1");
		}
	}

	//WeaponFinesse rules
	if(bOkRead)
	{
		//Now parse the SpeedFeatRules
		int iRuleNumber = 1;
		std::string sWpnFinesseRuleBase = "WpnFinesseRule";

		std::string sWpnFinesseRuleX = sWpnFinesseRuleBase + std::to_string(iRuleNumber);

		while (ini.has(sWpnFinesseRuleX))
		{
			std::string sWpnItemType = ini.get(sWpnFinesseRuleX).get("ItemType");

			std::set<uint32_t> itemTypeList;
			std::istringstream iss(sWpnItemType);
			uint32_t number;

			while (iss >> number) {
				itemTypeList.insert(number);
			}

			if (!itemTypeList.empty())
			{
				bool bCheckSize = false;

				if (ini.get(sWpnFinesseRuleX).has("CheckSize"))
				{
					if (ini.get(sWpnFinesseRuleX).get("CheckSize") == "1")
						bCheckSize = true;
				}


				//Create rule.
				CreateWeaponFinesseRule(bCheckSize, itemTypeList);

				//Ok, we can test and parse other fields
				bool bValidRule = true;

				if (ini.get(sWpnFinesseRuleX).has("Area"))
				{
					std::string sValue = ini.get(sWpnFinesseRuleX).get("Area");

					bValidRule = ParseRuleFeatOrAreaType(sWpnFinesseRuleX, iRuleNumber, sValue,
						RuleParser::RuleType::AREA, itemTypeList);
				}

				if (bValidRule && ini.get(sWpnFinesseRuleX).has("Feat"))
				{
					std::string sValue = ini.get(sWpnFinesseRuleX).get("Feat");

					bValidRule = ParseRuleFeatOrAreaType(sWpnFinesseRuleX, iRuleNumber, sValue,
						RuleParser::RuleType::FEAT, itemTypeList);
				}

				if (bValidRule && ini.get(sWpnFinesseRuleX).has("Extra"))
				{
					std::string sValue = ini.get(sWpnFinesseRuleX).get("Extra");

					bValidRule = ParseRuleFeatOrAreaType(sWpnFinesseRuleX, iRuleNumber, sValue,
						RuleParser::RuleType::EXTRA, itemTypeList);
				}

				if (!bValidRule)
				{
					//Todo Remove this rule.
					RemoveLastRule(itemTypeList);
				}


			}
			else
			{
				logger->Err("[WeaponFinesseIniFile]: No valid ItemType field for [%s].", sWpnFinesseRuleX.c_str());
			}

			iRuleNumber++;
			sWpnFinesseRuleX = sWpnFinesseRuleBase + std::to_string(iRuleNumber);
		}
	}

	//CallWpnFinesseHook
	if (bOkRead && !isWFAlreadyPatched)
	{
		isWFAlreadyPatched = true;
		int i = 0;
		while(PatchWeaponFinesse[i].Apply()) {
			i++;
		}
	}
	else if (!bOkRead && isWFAlreadyPatched)
	{
		isWFAlreadyPatched = false;
		int i = 0;
		while(PatchWeaponFinesse[i].Remove()) {
			i++;
		}
	}
}

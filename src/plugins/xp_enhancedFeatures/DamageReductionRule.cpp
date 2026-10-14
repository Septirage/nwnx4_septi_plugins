
#include <misc/log.h>
#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include <misc/Patch.h>
#include "../../septutil/mini.h"

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <stdexcept>

#include "RuleParser.h"
#include "BonusParser.h"

extern std::unique_ptr<LogNWNX> logger;

//Calculate_DamageReduction (0x586240, thiscall damaged object, damage packet ; virtual): builds the list of the damage
//reductions (effects, item properties, creature's own DR, then hardcoded feats), sorted from the biggest, then applies
//the first one that is not bypassed.
//The hardcoded feat block (creatures only) is replaced by the rules: jump from its start to the selection.
#define OFFS_DRFeatBlock					0x005863f6		//mov edx,[esi] / mov eax,[edx+0x14] (5 bytes). ESI = stats, EBX = packet
unsigned long ReturnDRSelection				= 0x00586796;	//mov ecx,ebx / call 0x57b870

//Inserts a DR entry in the packet list, sorted (thiscall packet, entry ; ret 4). Copies the bypass array.
#define FUNC_InsertDamageReduction			0x00584670

//DR types (DR_TYPE_*) and the bypass test of the engine (0x57b650)
#define DR_TYPE_NONE						0
#define DR_TYPE_DMGTYPE						1	//subtype = bit position of the damage type
#define DR_TYPE_MAGICBONUS					2	//subtype = damage power (DAMAGE_POWER_*)
#define DR_TYPE_EPIC						3
#define DR_TYPE_GMATERIAL					4	//subtype = GMATERIAL_* (item "GMaterial" field)
#define DR_TYPE_ALIGNMENT					5	//subtype = ALIGNMENT_* (2 lawful, 3 chaotic, 4 good, 5 evil)
#define DR_TYPE_NON_RANGED					6	//applies only against a creature holding a ranged weapon

#define DR_FLAG_OR							1	//Bypassed by any of the bypasses. Else: by all of them


//Engine structures of a DR entry (0x1C bytes) and of one of its bypasses (8 bytes)
struct DRBypass {
	uint32_t type;
	uint8_t subtype;
	uint8_t pad[3];
};

struct DREntry {
	int amount;
	DRBypass* bypasses;
	int count;
	int capacity;
	int flags;
	int limit;			//0: infinite
	int effectIndex;	//-1: not from an effect
};

//Bypasses of a rule. The rules with the same bypasses are summed in one entry
struct DRBypassGroup {
	std::vector<DRBypass> bypasses;
	int flags;
};


struct DamageReductionRuleSet {
	RuleParser::Rule* featRule;
	RuleParser::Rule* areaTypeRule;
	RuleParser::Rule* extraRule;
	MathExpressionParser::Expr* bonusCalculation;
	int index;
	int groupIndex;		// in m_drBypassGroupList


	DamageReductionRuleSet(MathExpressionParser::Expr* bonus, int idx)
	    : bonusCalculation(bonus)
	    , index(idx)
	    , groupIndex(0)
	{
		featRule = nullptr;
		areaTypeRule = nullptr;
		extraRule    = nullptr;
	}

	~DamageReductionRuleSet()
	{
		delete bonusCalculation;
		delete areaTypeRule;
		delete extraRule;
		delete featRule;
	}
};


std::vector<DamageReductionRuleSet*> m_damageReductionRuleSetList;
std::vector<DRBypassGroup> m_drBypassGroupList;


static bool ParseDRRuleCondition(std::string sRuleX, std::string sValue, RuleParser::RuleType tFeatRule, std::vector<DamageReductionRuleSet*>& myRuleList)
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


static bool ParseDRModifier(int iRuleNumber, std::string sValue, std::vector<DamageReductionRuleSet*>& myRuleList, std::string sField)
{
	try {
		auto bonusCalc = MathExpressionParser::prepareSimplified(sValue, MathExpressionParser::Mode::INTEGER);
		DamageReductionRuleSet* rs1 = new DamageReductionRuleSet(bonusCalc, iRuleNumber);

		myRuleList.push_back(rs1);
	} catch (const std::exception& e) {

		logger->Err("Error during parse of \"%s\" = %s : %s", sField.c_str(), sValue.c_str(), e.what());
		logger->Info("Due to the Error, the complete rule will not be applied");
		return false;
	}

	return true;
}


static std::string ToUpperWithoutPrefix(std::string sWord, const char* sPrefix)
{
	for (char& c : sWord)
		c = (char)toupper((unsigned char)c);

	size_t uLen = strlen(sPrefix);
	if (sWord.compare(0, uLen, sPrefix) == 0)
		sWord = sWord.substr(uLen);

	return sWord;
}

//Number in [iMin, iMax], else -1
static int ParseBoundedNumber(const std::string& sWord, int iMin, int iMax)
{
	try {
		size_t pos = 0;
		int iValue = std::stoi(sWord, &pos);
		if (pos == sWord.size() && iValue >= iMin && iValue <= iMax)
			return iValue;
	} catch (const std::exception&) {}

	return -1;
}

//One bypass: "NONE", "MAGICBONUS n", "EPIC", "GMATERIAL n", "DMGTYPE x", "ALIGNMENT x", "NON_RANGED"
//(DR_TYPE_ prefix accepted). Error message in sError if false.
static bool ParseOneBypass(std::string sValue, DRBypass& bypass, std::string& sError)
{
	std::istringstream iss(sValue);
	std::string sType, sSub, sExtra;

	memset(&bypass, 0, sizeof(bypass));

	if (!(iss >> sType))
	{
		sError = "empty bypass";
		return false;
	}
	iss >> sSub;
	if (iss >> sExtra)
	{
		sError = "too many words in \"" + sValue + "\"";
		return false;
	}

	sType = ToUpperWithoutPrefix(sType, "DR_TYPE_");

	bool bNoSub = (sType == "NONE" || sType == "EPIC" || sType == "NON_RANGED");
	if (bNoSub != sSub.empty())
	{
		sError = bNoSub ? (sType + " takes no value") : (sType + " needs a value");
		return false;
	}

	if (sType == "NONE")
		bypass.type = DR_TYPE_NONE;
	else if (sType == "EPIC")
		bypass.type = DR_TYPE_EPIC;
	else if (sType == "NON_RANGED")
		bypass.type = DR_TYPE_NON_RANGED;
	else if (sType == "MAGICBONUS")
	{
		//Enhancement +1 to +20. DAMAGE_POWER_ENERGY (6) is between +5 and +6: +6 is 7...
		int iBonus = ParseBoundedNumber(sSub, 1, 20);
		if (iBonus < 0)
		{
			sError = "MAGICBONUS needs a number from 1 to 20";
			return false;
		}
		bypass.type = DR_TYPE_MAGICBONUS;
		bypass.subtype = (uint8_t)(iBonus <= 5 ? iBonus : iBonus + 1);
	}
	else if (sType == "GMATERIAL")
	{
		int iMaterial = ParseBoundedNumber(sSub, 0, 255);
		if (iMaterial < 0)
		{
			sError = "GMATERIAL needs a number from 0 to 255";
			return false;
		}
		bypass.type = DR_TYPE_GMATERIAL;
		bypass.subtype = (uint8_t)iMaterial;
	}
	else if (sType == "DMGTYPE")
	{
		//DAMAGE_TYPE_* name or value (a power of 2). The engine wants the bit position.
		static const char* damageTypeNames[] = {
			"BLUDGEONING", "PIERCING", "SLASHING", "MAGICAL", "ACID", "COLD", "DIVINE", "ELECTRICAL",
			"FIRE", "NEGATIVE", "POSITIVE", "SONIC"
		};
		const int iNbDamageTypes = (int)(sizeof(damageTypeNames) / sizeof(damageTypeNames[0]));

		std::string sName = ToUpperWithoutPrefix(sSub, "DAMAGE_TYPE_");
		int iBit = -1;
		for (int i = 0; i < iNbDamageTypes; i++)
		{
			if (sName == damageTypeNames[i])
				iBit = i;
		}

		if (iBit < 0)
		{
			int iFlag = ParseBoundedNumber(sSub, 1, 1 << (iNbDamageTypes - 1));
			for (int i = 0; i < iNbDamageTypes && iFlag > 0; i++)
			{
				if (iFlag == (1 << i))
					iBit = i;
			}
		}

		if (iBit < 0)
		{
			sError = "DMGTYPE needs one DAMAGE_TYPE_* name or value";
			return false;
		}
		bypass.type = DR_TYPE_DMGTYPE;
		bypass.subtype = (uint8_t)iBit;
	}
	else if (sType == "ALIGNMENT")
	{
		static const char* alignmentNames[] = { "LAWFUL", "CHAOTIC", "GOOD", "EVIL" };	//ALIGNMENT_* 2 to 5

		std::string sName = ToUpperWithoutPrefix(sSub, "ALIGNMENT_");
		int iAlignment = ParseBoundedNumber(sName, 2, 5);
		for (int i = 0; i < 4; i++)
		{
			if (sName == alignmentNames[i])
				iAlignment = i + 2;
		}

		if (iAlignment < 0)
		{
			sError = "ALIGNMENT needs LAWFUL, CHAOTIC, GOOD or EVIL";
			return false;
		}
		bypass.type = DR_TYPE_ALIGNMENT;
		bypass.subtype = (uint8_t)iAlignment;
	}
	else
	{
		sError = "unknown bypass type " + sType;
		return false;
	}

	return true;
}

//Bypass field: one bypass, or several all joined by '|' (or) or all by '&' (and). -1 if error, else the group index
static int ParseBypassField(std::string sValue, std::string& sError)
{
	bool bOr = (sValue.find('|') != std::string::npos);
	bool bAnd = (sValue.find('&') != std::string::npos);
	if (bOr && bAnd)
	{
		sError = "'|' and '&' can not be mixed";
		return -1;
	}

	DRBypassGroup group;
	group.flags = bOr ? DR_FLAG_OR : 0;

	char cSeparator = bOr ? '|' : '&';
	std::istringstream iss(sValue);
	std::string sPart;
	while (std::getline(iss, sPart, cSeparator))
	{
		DRBypass bypass;
		if (!ParseOneBypass(sPart, bypass, sError))
			return -1;
		group.bypasses.push_back(bypass);
	}

	if (group.bypasses.empty())
	{
		sError = "empty bypass";
		return -1;
	}

	//One bypass: "or" and "and" are the same
	if (group.bypasses.size() == 1)
		group.flags = 0;

	//Same bypasses in another order: same group
	std::sort(group.bypasses.begin(), group.bypasses.end(), [](const DRBypass& a, const DRBypass& b) {
		return (a.type != b.type) ? (a.type < b.type) : (a.subtype < b.subtype);
	});

	for (size_t i = 0; i < m_drBypassGroupList.size(); i++)
	{
		const DRBypassGroup& other = m_drBypassGroupList[i];
		if (other.flags != group.flags || other.bypasses.size() != group.bypasses.size())
			continue;

		bool bSame = true;
		for (size_t j = 0; j < group.bypasses.size() && bSame; j++)
			bSame = (other.bypasses[j].type == group.bypasses[j].type && other.bypasses[j].subtype == group.bypasses[j].subtype);

		if (bSame)
			return (int)i;
	}

	m_drBypassGroupList.push_back(group);
	return (int)m_drBypassGroupList.size() - 1;
}


__declspec(naked) void __fastcall InsertDamageReduction(void* pPacket, void* Unused, DREntry* pEntry)
{
	__asm
	{
		mov		edx, FUNC_InsertDamageReduction;
		jmp		edx;
	}
}


//Sum of the rules by bypass group, then one DR entry for each group with a positive sum
int __stdcall DamageReductionRuleApplication(int pcBlock, void* pPacket)
{
	int areaType = 0;

	if (m_damageReductionRuleSetList.empty())
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

	std::vector<int> groupSums(m_drBypassGroupList.size(), 0);

	RuleParser::m_activatedRuleSet.clear();

	for (DamageReductionRuleSet* myRule : m_damageReductionRuleSetList)
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
			groupSums[myRule->groupIndex] += myRule->bonusCalculation->evaluateInt(pcBlock);
		}
	}

	RuleParser::m_activatedRuleSet.clear();

	for (size_t i = 0; i < groupSums.size(); i++)
	{
		//The engine does not check the sign: a negative entry would add damage
		if (groupSums[i] <= 0)
			continue;

		std::vector<DRBypass>& bypasses = m_drBypassGroupList[i].bypasses;
		DREntry entry = { groupSums[i], bypasses.data(), (int)bypasses.size(), (int)bypasses.size(), m_drBypassGroupList[i].flags, 0, -1 };
		InsertDamageReduction(pPacket, NULL, &entry);
	}

	return 0;
}

//Jumped to from the start of the hardcoded feat block of Calculate_DamageReduction (creatures only).
//ESI = stats, EBX = packet. Back to the selection of the entry, after the feat block.
__declspec(naked) void DamageReductionFeatBlockHook()
{
	__asm
	{
		push	ebx						//Packet
		push	esi						//Stats
		call	DamageReductionRuleApplication		//stdcall, keeps EBX/ESI/EDI/EBP
		jmp		dword ptr [ReturnDRSelection]
	}
}


Patch _DamageReductionRulePatches[] = {
	Patch((DWORD)OFFS_DRFeatBlock, (char*)"\xe9", (int)1),
	Patch(OFFS_DRFeatBlock + 1, (relativefunc)DamageReductionFeatBlockHook),

	Patch()
};

Patch *DamageReductionRulePatches = _DamageReductionRulePatches;

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool isDamageReductionAlreadyPatched = false;

void desinitDamageReductionRule()
{
	for (auto ruleSet : m_damageReductionRuleSetList) {
		delete ruleSet;
	}
	m_damageReductionRuleSetList.clear();
	m_drBypassGroupList.clear();
}

void initDamageReductionRule(std::string nxhome, std::string sFileName)
{
	std::string inifile(nxhome);
	inifile.append("\\");
	inifile.append(sFileName);

	mINI::INIFile file(inifile);
	mINI::INIStructure ini;

	bool bOkRead = file.read(ini);

	if (!bOkRead)
		logger->Err("Cant find DamageReductionFile : %s", sFileName.c_str());

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
		std::string sDRRuleBase = "DamageReductionRule";

		std::string sDRRuleX = sDRRuleBase + std::to_string(iRuleNumber);

		while (ini.has(sDRRuleX))
		{
			auto section = ini.get(sDRRuleX);

			if (!section.has("Bonus"))
			{
				logger->Err("[DamageReductionFile]: No Bonus field for [%s].", sDRRuleX.c_str());
			}
			else if (!section.has("Bypass"))
			{
				logger->Err("[DamageReductionFile]: No Bypass field for [%s].", sDRRuleX.c_str());
			}
			else
			{
				std::string sError;
				int iGroupIndex = ParseBypassField(section.get("Bypass"), sError);

				if (iGroupIndex < 0)
				{
					logger->Err("[DamageReductionFile]: Bad Bypass field for [%s] : %s (%s).", sDRRuleX.c_str(), section.get("Bypass").c_str(), sError.c_str());
				}
				else if (ParseDRModifier(iRuleNumber, section.get("Bonus"), m_damageReductionRuleSetList, "Bonus"))
				{
					bool bValidRule = true;
					m_damageReductionRuleSetList.back()->groupIndex = iGroupIndex;

					if (section.has("Area"))
					{
						bValidRule = ParseDRRuleCondition(sDRRuleX, section.get("Area"), RuleParser::RuleType::AREA, m_damageReductionRuleSetList);
					}

					if (bValidRule && section.has("Feat"))
					{
						bValidRule = ParseDRRuleCondition(sDRRuleX, section.get("Feat"), RuleParser::RuleType::FEAT, m_damageReductionRuleSetList);
					}

					if (bValidRule && section.has("Extra"))
					{
						bValidRule = ParseDRRuleCondition(sDRRuleX, section.get("Extra"), RuleParser::RuleType::EXTRA, m_damageReductionRuleSetList);
					}

					if (!bValidRule)
					{
						DamageReductionRuleSet* rsToDel = m_damageReductionRuleSetList.back();
						m_damageReductionRuleSetList.pop_back();
						delete rsToDel;
					}
				}
			}

			iRuleNumber++;
			sDRRuleX = sDRRuleBase + std::to_string(iRuleNumber);
		}
	}


	if (bOkRead && !isDamageReductionAlreadyPatched)
	{
		isDamageReductionAlreadyPatched = true;
		int i = 0;
		while (DamageReductionRulePatches[i].Apply()) {
			i++;
		}
	}
	else if (!bOkRead && isDamageReductionAlreadyPatched)
	{
		isDamageReductionAlreadyPatched = false;

		//Remove in reverse order (last element is the empty Patch())
		for (int i = (int)(sizeof(_DamageReductionRulePatches) / sizeof(Patch)) - 2; i >= 0; i--)
			DamageReductionRulePatches[i].Remove();
	}
}

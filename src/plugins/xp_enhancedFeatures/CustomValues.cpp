#include <misc/log.h>
#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include <misc/Patch.h>
#include "../../septutil/mini.h"

#include "../../septutil/NwN2DataPos.h"
#include "../../septutil/nwn2gff.h"


#include <unordered_map>
#include <string>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <sstream>
#include <stdexcept>
#include "nwn2heap.h"
#include "CustomValues.h"
#include "RuleParser.h"
#include "BonusParser.h"


extern std::unique_ptr<LogNWNX> logger;

typedef void (__cdecl * NWN2Heap_Deallocate_Proc)(void *p);
extern NWN2Heap_Deallocate_Proc NWN2Heap_Deallocate;




bool bCustomValuesActivated = false;
int uCustomValuesNumbers = 0;
uint32_t uPLCstValue[5];
uint32_t uPrepCstValue = 0;

std::vector<CustomValueMode> g_customValueModes;

const char* pStrCustomValue = "CustomValues";
const char* pStrValueCst = "Value";

struct CustomValueRuleSet {
	RuleParser::Rule* featRule;
	RuleParser::Rule* areaTypeRule;
	RuleParser::Rule* extraRule;
	MathExpressionParser::Expr* bonusCalculation;
	int index;

	CustomValueRuleSet(MathExpressionParser::Expr* bonus, int idx)
		: bonusCalculation(bonus)
		, index(idx)
	{
		featRule = nullptr;
		areaTypeRule = nullptr;
		extraRule = nullptr;
	}

	~CustomValueRuleSet()
	{
		delete bonusCalculation;
		delete areaTypeRule;
		delete extraRule;
		delete featRule;
	}
};

// Two independent rule sets, sharing the same rule shape (CustomValueRuleSet) but with
// their own [CustomValueRuleN] / [CustomValueEvolutionRuleN] sections and rule numbering:
// Computed rules compute a value live on read, Evolution rules apply a delta on tick.
std::unordered_map<uint32_t, std::vector<CustomValueRuleSet*>> m_customValueRuleSets;
std::unordered_map<uint32_t, std::vector<CustomValueRuleSet*>> m_customValueEvolutionRuleSets;

struct EvolutionEntry {
	int index;
	int period; // seconds
};

std::vector<EvolutionEntry> g_evolutionEntries;

// Second half of the per-creature CustomValue block: last tick time (seconds) per
// index, used only for Evolution. Not GFF-persisted.
static int GetCustomValueLastTick(GameObject* pCreature, int iIdx)
{
	if (((int*)pCreature)[1] == NULL)
		return 0;

	return ((int**)pCreature)[1][uCustomValuesNumbers + iIdx];
}

static void SetCustomValueLastTick(GameObject* pCreature, int iIdx, int iValue)
{
	if (((int*)pCreature)[1] == NULL)
		return;

	((int**)pCreature)[1][uCustomValuesNumbers + iIdx] = iValue;
}

// Sums the Bonus of every CustomValueRule matching iIdx for pCreature (0 if none match).
// ruleMap is either m_customValueRuleSets (Computed) or m_customValueEvolutionRuleSets (Evolution).
int EvaluateCustomValueRules(GameObject* pCreature, int iIdx, std::unordered_map<uint32_t, std::vector<CustomValueRuleSet*>>& ruleMap)
{
	auto it = ruleMap.find(iIdx);
	if (it == ruleMap.end())
		return 0;

	char* pCreaBytes = (char*)pCreature;
	int CreaPartPtr = *(int*)(pCreaBytes + AmCrtPtrAppBlock);

	int areaType = 0;
	{
		GameObject* Object;
		GameObjectManager m_ObjectManager;
		NWN::OBJECTID areaID = *(NWN::OBJECTID*)(pCreaBytes + AmCommonArea);

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

	int iBonusTotal = 0;
	for (CustomValueRuleSet* myRule : it->second)
	{
		bool bOk = true;
		if (myRule->featRule != nullptr)
			bOk = RuleParser::evaluateRule(myRule->featRule, RuleParser::RuleType::FEAT, CreaPartPtr);

		if (bOk && myRule->areaTypeRule != nullptr)
			bOk = RuleParser::evaluateRule(myRule->areaTypeRule, RuleParser::RuleType::AREA, areaType);

		if (bOk && myRule->extraRule != nullptr)
			bOk = RuleParser::evaluateRule(myRule->extraRule, RuleParser::RuleType::EXTRA, (int)pCreature);

		if (bOk)
		{
			RuleParser::m_activatedRuleSet.insert(myRule->index);
			iBonusTotal += myRule->bonusCalculation->evaluateInt(CreaPartPtr);
		}
	}

	RuleParser::m_activatedRuleSet.clear();
	return iBonusTotal;
}





#define OFF_SaveCreatureDataBeforeEqItmList		0x005fad20
const int ReturnAfterCreatureSaveCustomValues = 0x005fad27;


#define FCT_ADDR_PREPARE_LIST		  0x00795B60
#define FCT_ADDR_PREPARE_LIST_ENTRY   0x007954D0
#define FCT_ADDR_ADD_LIST_VALUE		  0x00795EA0

__declspec(naked) void SaveCustomValues()
{
	__asm
	{
		PUSH	pStrCustomValue
		PUSH	EBP
		LEA		EAX, [uPLCstValue]
		PUSH	EAX
		MOV		ECX, ESI
		MOV		EAX, FCT_ADDR_PREPARE_LIST
		CALL	EAX

		PUSH	EBP

		MOV		EBP, 0


SaveCustomValueStartOfLoop:

		PUSH	2
		LEA		EAX, [uPLCstValue]
		PUSH	EAX
		LEA		EAX, [uPrepCstValue]
		PUSH	EAX
		MOV		ECX, ESI
		MOV		EAX, FCT_ADDR_PREPARE_LIST_ENTRY
		CALL	EAX


		PUSH	pStrValueCst
		MOV		EAX, [EDI + 0x4]
		MOV		EAX, [EAX + EBP*4]
		PUSH	EAX
		LEA		EAX, [uPrepCstValue]
		PUSH	EAX
		MOV		ECX, ESI
		MOV		EAX, FCT_ADDR_ADD_LIST_VALUE
		CALL	EAX

		ADD		EBP, 1
		CMP		EBP, uCustomValuesNumbers
		JL		SaveCustomValueStartOfLoop

		POP		EBP

		CMP		dword ptr [EDI + 0x100c],0x0
		JMP		dword ptr[ReturnAfterCreatureSaveCustomValues]
	}
}

#define OFF_RetrieveCreaStatStart 0x005b1704
const int ReturnAfterCreatureLoadCustomValues = 0x005b170a;

void __fastcall RetrieveCustomValues(void* puVar, void* gffPtr, int* aiStack110, void* pCreature)
{

	void* newPtr[5];
	void* newPtr2[5];

	if (((int*)pCreature)[1] == NULL)
		return;

	int* pCustomValueTab = ((int**)pCreature)[1];

	int iFound = NWNGetGFFList(puVar, newPtr, gffPtr, pStrCustomValue);
	if (iFound != 0)
	{
		int iNb = NWNGetGFFListLength(puVar, newPtr);
		

		for (int i = 0; i < iNb && i < uCustomValuesNumbers; i++)
		{
			NWNGetGFFStruct(puVar, newPtr2, newPtr, i);

			pCustomValueTab[i] = NWNGetGFFInt(puVar, newPtr2, pStrValueCst, aiStack110, 0);
		}
	}
}

__declspec(naked) void LoadCustomValues()
{
	__asm
	{
		MOV		EAX, dword ptr [EBX + 0xa4]
		//MOV		EAX, dword ptr [EAX]
		PUSH	EAX

		LEA		EAX, [ESP + 0x14]
		PUSH	EAX

		MOV		EDX, EDI
		MOV		ECX, ESI

		CALL	RetrieveCustomValues

		MOV		EAX,dword ptr [EBX + 0x90]

		JMP		dword ptr[ReturnAfterCreatureLoadCustomValues]
	}
}


void __fastcall MallocCustomValues(void* pCreature)
{
	//Allocate new one
	NWN2_HeapMgr *pHeapMgr = NWN2_HeapMgr::Instance();
	NWN2_Heap *pHeap = pHeapMgr->GetDefaultHeap();
	// Second half is per-index Evolution bookkeeping (last tick time), not GFF-persisted.
	size_t size = 2 * uCustomValuesNumbers * sizeof(int);

	int* newCustomValue = (int*)pHeap->Allocate(size);
	memset(newCustomValue, 0, size);

	((int**)pCreature)[1] = newCustomValue;
}


#define OFF_StartOfCreatureInitialization 0x005fd7ce
const int ReturnAfterCreatureAllocateCustomValues = 0x005fd7d4;

__declspec(naked) void AllocateCustomValues()
{
	__asm
	{
		MOV		ECX, ESI
		CALL	MallocCustomValues

		LEA		ECX, [ESI + 0x310]

		JMP		dword ptr[ReturnAfterCreatureAllocateCustomValues]
	}
}


#define OFF_FreeCreatureAfterStat	0x005f9967
const int ReturnAfterFreeCreatureCustomValue = 0x005f996d;

void __fastcall FreeCreatureCustomValues(void* pCreature)
{
	int* pCstValBlck = ((int**)pCreature)[1];

	if (NWN2Heap_Deallocate)
		NWN2Heap_Deallocate( pCstValBlck );

	((int**)pCreature)[1] = NULL;
}

__declspec(naked) void FreeCustomValues()
{
	__asm
	{
		MOV		ECX, ESI

		CALL	FreeCreatureCustomValues

		MOV		EDI, dword ptr [ESI + 0xF70]

		JMP		dword ptr[ReturnAfterFreeCreatureCustomValue]
	}
}

Patch _LoadSaveCustomValuesPatches[] =
{
	Patch((DWORD)OFF_SaveCreatureDataBeforeEqItmList, (char*)"\xe9\x00\x00\x00\x00\x90\x90", (int)7),
	Patch(OFF_SaveCreatureDataBeforeEqItmList + 1, (relativefunc)SaveCustomValues),

	Patch((DWORD)OFF_RetrieveCreaStatStart, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6),
	Patch(OFF_RetrieveCreaStatStart + 1, (relativefunc)LoadCustomValues),

	Patch((DWORD)OFF_FreeCreatureAfterStat, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6),
	Patch(OFF_FreeCreatureAfterStat + 1, (relativefunc)FreeCustomValues),

	Patch((DWORD)OFF_StartOfCreatureInitialization, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6),
	Patch(OFF_StartOfCreatureInitialization + 1, (relativefunc)AllocateCustomValues),


	Patch()
};
Patch *LoadSaveCustomValuesPatches = _LoadSaveCustomValuesPatches;


void __fastcall TickCustomValueEvolution(GameObject* pCreature, uint32_t* ptrCurrentTime);

#define OFFS_CreatureAIUpdateBeforeEffect		0x005fe62d
const int ReturnToCreatureAIUpdateEffect = 0x005fe635;

__declspec(naked) void HookForEvolveCustomValues()
{
	__asm
	{
		MOV		EDX, EBX
		MOV		ECX, ESI
		CALL	TickCustomValueEvolution

		XOR		EBX, EBX
		CMP		dword ptr [ESI + 0x1A4], EBX

		JMP		dword ptr[ReturnToCreatureAIUpdateEffect]
	}
}

Patch _PatchHookEvolveCustomValues[] =
{
	Patch((DWORD)OFFS_CreatureAIUpdateBeforeEffect, (char*)"\xe9\x00\x00\x00\x00\x90\x90\x90", (int)8),
	Patch(OFFS_CreatureAIUpdateBeforeEffect + 1, (relativefunc)HookForEvolveCustomValues),

	Patch()
};
Patch *PatchHookEvolveCustomValues = _PatchHookEvolveCustomValues;

void prepareSaveLoadCustomValues()
{
	if (bCustomValuesActivated)
	{
		int i = 0;
		while(LoadSaveCustomValuesPatches[i].Apply()) {
			i++;
		}
	}
}

void initCustomValuesNumber(int uNumber)
{
	bCustomValuesActivated = (uNumber > 0);
	uCustomValuesNumbers = bCustomValuesActivated ? uNumber : 0;

	g_customValueModes.assign(uCustomValuesNumbers, CustomValueMode::Raw);

	prepareSaveLoadCustomValues();
}

int GetCustomValuesNumber()
{
	return uCustomValuesNumbers;
}

CustomValueMode GetCustomValueMode(int iIdx)
{
	if (iIdx < 0 || iIdx >= (int)g_customValueModes.size())
		return CustomValueMode::Raw;

	return g_customValueModes[iIdx];
}

// Internal only: used while parsing CustomValueEFF.ini to assign Computed/Evolution.
// Fails if iIdx is out of bounds or already assigned a non-Raw mode (mutual exclusivity).
static bool SetCustomValueMode(int iIdx, CustomValueMode mode)
{
	if (iIdx < 0 || iIdx >= (int)g_customValueModes.size())
		return false;

	if (g_customValueModes[iIdx] != CustomValueMode::Raw)
		return false;

	g_customValueModes[iIdx] = mode;
	return true;
}

void _SetCustomValue(GameObject* pCreature, int iIdx, int iValue)
{
	if (((int*)pCreature)[1] == NULL)
		return;

	((int**)pCreature)[1][iIdx] = iValue;
}

int _GetCustomValue(GameObject* pCreature, int iIdx)
{
	if (((int*)pCreature)[1] == NULL)
		return 0;

	return ((int**)pCreature)[1][iIdx];
}



GameObject* CreatureFromIdForCustomValue(int iObjectID)
{
	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;

	ObjectId = (NWN::OBJECTID) iObjectID;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return NULL;

	if(Object->GetObjectType() != NWN::OBJECT_TYPE_CREATURE)
		return NULL;

	if (((int*)Object)[1] == NULL)
		return NULL;

	return Object;
}

void SetCustomValue(int objectID, int iIdx, int iNumber)
{
	//Bad index
	if (!bCustomValuesActivated || iIdx < 0 || iIdx >= uCustomValuesNumbers)
		return;

	GameObject* myCreature = CreatureFromIdForCustomValue(objectID);
	if (myCreature == NULL)
		return;

	_SetCustomValue(myCreature, iIdx, iNumber);
}

int GetCustomValue(int objectID, int iIdx)
{
	GameObject* myCreature = CreatureFromIdForCustomValue(objectID);
	if (myCreature == NULL)
		return 0;

	return GetCreatureCustomValue(myCreature, iIdx);
}


int GetCreatureCustomValue(GameObject* pCreature, int iIdx)
{
	if (!bCustomValuesActivated || iIdx < 0 || iIdx >= uCustomValuesNumbers)
		return 0;

	if (GetCustomValueMode(iIdx) == CustomValueMode::Computed)
		return EvaluateCustomValueRules(pCreature, iIdx, m_customValueRuleSets);

	return _GetCustomValue(pCreature, iIdx);
}

void __fastcall TickCustomValueEvolution(GameObject* pCreature, uint32_t* ptrCurrentTime)
{
	if (!bCustomValuesActivated || g_evolutionEntries.empty())
		return;

	static uint32_t uBaseCalendar = ptrCurrentTime[14];
	static uint32_t uFactorForDay = (uint32_t)(*((uint8_t*)ptrCurrentTime + 0x2C)) * 24 * 60;

	uint32_t dayElapsed = ptrCurrentTime[14] - uBaseCalendar;
	uint32_t secondInDay = ptrCurrentTime[15] / 1000;
	uint32_t currentTime = dayElapsed * uFactorForDay + secondInDay;


	for (const EvolutionEntry& entry : g_evolutionEntries)
	{
		int lastTick = GetCustomValueLastTick(pCreature, entry.index);

		if (lastTick == 0)
		{
			// First observation for this creature/index: start counting from now
			// instead of firing immediately just because currentTime is already large.
			SetCustomValueLastTick(pCreature, entry.index, currentTime);
			continue;
		}

		int elapsed = currentTime - lastTick;
		if (elapsed < entry.period)
			continue;

		int delta = EvaluateCustomValueRules(pCreature, entry.index, m_customValueEvolutionRuleSets);
		if (delta != 0)
		{
			int newValue = _GetCustomValue(pCreature, entry.index) + delta;
			_SetCustomValue(pCreature, entry.index, newValue);
		}

		SetCustomValueLastTick(pCreature, entry.index, currentTime);
	}
}


// Parses a whitespace-separated list of indices (same convention as MonkWeaponList)
// and assigns them the given mode. Logs and skips indices that are out of range or
// already assigned to another mode.
static void ParseCustomValueIndexList(const std::string& sList, CustomValueMode mode, const char* sFieldName)
{
	std::istringstream iss(sList);
	int iIdx;

	while (iss >> iIdx)
	{
		if (!SetCustomValueMode(iIdx, mode))
		{
			logger->Err("[CustomValueRuleFile]: Cannot set index %d as %s: out of range or already assigned to another mode.", iIdx, sFieldName);
		}
	}
}

// Parses a whitespace-separated list of "index:period" pairs (period in seconds) and
// registers each index as Evolution.
static void ParseEvolutionIndexList(const std::string& sList)
{
	std::istringstream iss(sList);
	std::string token;

	while (iss >> token)
	{
		size_t colonPos = token.find(':');
		int iIdx = -1;
		int iPeriod = -1;

		if (colonPos != std::string::npos)
		{
			try {
				iIdx = std::stoi(token.substr(0, colonPos));
				iPeriod = std::stoi(token.substr(colonPos + 1));
			} catch (const std::exception&) {
				iIdx = -1;
			}
		}

		if (colonPos == std::string::npos || iPeriod <= 0)
		{
			logger->Err("[CustomValueRuleFile]: Invalid EvolutionValues entry '%s' (expected index:periodInSeconds).", token.c_str());
			continue;
		}

		if (!SetCustomValueMode(iIdx, CustomValueMode::Evolution))
		{
			logger->Err("[CustomValueRuleFile]: Cannot set index %d as Evolution: out of range or already assigned to another mode.", iIdx);
			continue;
		}

		g_evolutionEntries.push_back({ iIdx, iPeriod });
	}
}

// Walks a RuleParser rule tree looking for a CustomValue(n) function referencing an
// index that is itself Computed, which would make evaluation circular.
static bool RuleReferencesComputedCustomValue(RuleParser::Rule* node)
{
	if (node == nullptr)
		return false;

	if (node->type == RuleParser::TokenType::FUNCTION && node->value == RuleParser::FCT_CUSTOMVALUE
		&& GetCustomValueMode(node->params.back()) == CustomValueMode::Computed)
	{
		return true;
	}

	return RuleReferencesComputedCustomValue(node->left) || RuleReferencesComputedCustomValue(node->right);
}

// Same check for a BonusParser expression tree (Bonus= formulas).
static bool ExprReferencesComputedCustomValue(MathExpressionParser::Expr* expr)
{
	if (expr == nullptr)
		return false;

	if (auto* fct = dynamic_cast<MathExpressionParser::FunctionT*>(expr))
	{
		return fct->functionToUse == FunctionType::CUSTOMVALUE
			&& GetCustomValueMode(fct->params.back()) == CustomValueMode::Computed;
	}

	if (auto* bin = dynamic_cast<MathExpressionParser::BinaryOp*>(expr))
		return ExprReferencesComputedCustomValue(bin->left) || ExprReferencesComputedCustomValue(bin->right);

	return false;
}

static bool ParseCustomValueBonus(int iRuleNumber, std::string sValue, int iCustomValueIdx,
	std::unordered_map<uint32_t, std::vector<CustomValueRuleSet*>>& ruleMap)
{
	try {
		auto bonusCalc = MathExpressionParser::prepareSimplified(sValue, MathExpressionParser::Mode::INTEGER);

		if (GetCustomValueMode(iCustomValueIdx) == CustomValueMode::Computed && ExprReferencesComputedCustomValue(bonusCalc))
		{
			logger->Err("[CustomValueRule%d]: \"Bonus\" cannot reference another Computed CustomValue.", iRuleNumber);
			delete bonusCalc;
			return false;
		}

		CustomValueRuleSet* rs1 = new CustomValueRuleSet(bonusCalc, iRuleNumber);
		ruleMap[iCustomValueIdx].push_back(rs1);
	} catch (const std::exception& e) {
		logger->Err("Error during parse of \"Bonus\" = %s : %s", sValue.c_str(), e.what());
		logger->Info("Due to the Error, the complete rule will not be applied");
		return false;
	}

	return true;
}

static bool ParseCustomValueRuleFeatOrAreaType(std::string sCustomValueRuleX, int iRuleNumber, std::string sValue, int iCustomValueIdx, RuleParser::RuleType tFeatRule,
	std::unordered_map<uint32_t, std::vector<CustomValueRuleSet*>>& ruleMap, bool bAllowRule = true)
{
	try {
		std::vector<RuleParser::Token> tokens1 = RuleParser::tokenize(sValue, tFeatRule, bAllowRule);
		int pos1 = 0;
		RuleParser::Rule* root1 = RuleParser::parseExpression(tokens1, pos1);

		if (tFeatRule == RuleParser::RuleType::EXTRA && GetCustomValueMode(iCustomValueIdx) == CustomValueMode::Computed
			&& RuleReferencesComputedCustomValue(root1))
		{
			delete root1;
			logger->Err("[%s]: \"Extra\" cannot reference another Computed CustomValue.", sCustomValueRuleX.c_str());
			return false;
		}

		if (tFeatRule == RuleParser::RuleType::FEAT)
			ruleMap[iCustomValueIdx].back()->featRule = root1;
		else if (tFeatRule == RuleParser::RuleType::AREA)
			ruleMap[iCustomValueIdx].back()->areaTypeRule = root1;
		else
			ruleMap[iCustomValueIdx].back()->extraRule = root1;

	} catch (const std::exception& e) {
		std::string sRuleType;

		if (tFeatRule == RuleParser::RuleType::FEAT)
			sRuleType = "Feat";
		else if (tFeatRule == RuleParser::RuleType::AREA)
			sRuleType = "Area";
		else
			sRuleType = "Extra";

		logger->Err("Error during parse of [%s] \"%s\" = %s : %s", sCustomValueRuleX.c_str(),
			sRuleType.c_str(), sValue.c_str(), e.what());
		logger->Info("Due to the Error, the complete rule will not be applied");
		return false;
	}

	return true;
}

void initCustomValuesHooks(std::string nxhome, std::string sFileName)
{
	g_customValueModes.assign(uCustomValuesNumbers, CustomValueMode::Raw);
	g_evolutionEntries.clear();

	std::string inifile(nxhome);
	inifile.append("\\");
	inifile.append(sFileName);

	mINI::INIFile file(inifile);
	mINI::INIStructure ini;

	if (!file.read(ini))
	{
		logger->Err("Cant find CustomValueRuleFile : %s", sFileName.c_str());
		return;
	}

	if (ini.has("General"))
	{
		if (ini.get("General").has("ComputedValues"))
			ParseCustomValueIndexList(ini.get("General").get("ComputedValues"), CustomValueMode::Computed, "Computed");

		if (ini.get("General").has("EvolutionValues"))
			ParseEvolutionIndexList(ini.get("General").get("EvolutionValues"));
	}

	//improvement, make the hook only if valid evolutionvalues rules ?
	{
		int i = 0;
		while (PatchHookEvolveCustomValues[i].Apply()) {
			i++;
		}
	}

	//Computed CustomValue rules
	{
		int iRuleNumber = 1;
		std::string sCustomValueRuleBase = "CustomValueRule";

		std::string sCustomValueRuleX = sCustomValueRuleBase + std::to_string(iRuleNumber);

		while (ini.has(sCustomValueRuleX))
		{
			if (ini.get(sCustomValueRuleX).has("Bonus") && ini.get(sCustomValueRuleX).has("CustomValue"))
			{
				std::string sBonus = ini.get(sCustomValueRuleX).get("Bonus");
				std::string sCustomValueIdx = ini.get(sCustomValueRuleX).get("CustomValue");
				int iCustomValueIdx = -1;

				try {
					iCustomValueIdx = std::stoi(sCustomValueIdx);
				}
				catch (const std::exception& ex) {
					iCustomValueIdx = -1;
				}

				if (iCustomValueIdx != -1 && GetCustomValueMode(iCustomValueIdx) != CustomValueMode::Computed)
				{
					logger->Err("[%s]: index %d is not declared Computed (see ComputedValues in [General]).", sCustomValueRuleX.c_str(), iCustomValueIdx);
					iCustomValueIdx = -1;
				}

				if (iCustomValueIdx != -1 && ParseCustomValueBonus(iRuleNumber, sBonus, iCustomValueIdx, m_customValueRuleSets))
				{
					bool bValidRule = true;

					if (ini.get(sCustomValueRuleX).has("Area"))
					{
						std::string sValue = ini.get(sCustomValueRuleX).get("Area");

						bValidRule = ParseCustomValueRuleFeatOrAreaType(sCustomValueRuleX, iRuleNumber, sValue, iCustomValueIdx,
							RuleParser::RuleType::AREA, m_customValueRuleSets);
					}

					if (bValidRule && ini.get(sCustomValueRuleX).has("Feat"))
					{
						std::string sValue = ini.get(sCustomValueRuleX).get("Feat");

						bValidRule = ParseCustomValueRuleFeatOrAreaType(sCustomValueRuleX, iRuleNumber, sValue, iCustomValueIdx,
							RuleParser::RuleType::FEAT, m_customValueRuleSets);
					}

					if (bValidRule && ini.get(sCustomValueRuleX).has("Extra"))
					{
						std::string sValue = ini.get(sCustomValueRuleX).get("Extra");

						bValidRule = ParseCustomValueRuleFeatOrAreaType(sCustomValueRuleX, iRuleNumber, sValue, iCustomValueIdx,
							RuleParser::RuleType::EXTRA, m_customValueRuleSets);
					}

					if (!bValidRule)
					{
						CustomValueRuleSet* rsToDel = m_customValueRuleSets[iCustomValueIdx].back();
						m_customValueRuleSets[iCustomValueIdx].pop_back();
						delete rsToDel;
					}
				}
			}
			else
			{
				logger->Err("[CustomValueRuleFile]: No CustomValue or Bonus field for [%s].", sCustomValueRuleX.c_str());
			}

			iRuleNumber++;
			sCustomValueRuleX = sCustomValueRuleBase + std::to_string(iRuleNumber);
		}
	}

	//Evolution CustomValue rules
	{
		int iRuleNumber = 1;
		std::string sCustomValueEvoRuleBase = "CustomValueEvolutionRule";

		std::string sCustomValueEvoRuleX = sCustomValueEvoRuleBase + std::to_string(iRuleNumber);

		while (ini.has(sCustomValueEvoRuleX))
		{
			if (ini.get(sCustomValueEvoRuleX).has("Bonus") && ini.get(sCustomValueEvoRuleX).has("CustomValue"))
			{
				std::string sBonus = ini.get(sCustomValueEvoRuleX).get("Bonus");
				std::string sCustomValueIdx = ini.get(sCustomValueEvoRuleX).get("CustomValue");
				int iCustomValueIdx = -1;

				try {
					iCustomValueIdx = std::stoi(sCustomValueIdx);
				}
				catch (const std::exception& ex) {
					iCustomValueIdx = -1;
				}

				if (iCustomValueIdx != -1 && GetCustomValueMode(iCustomValueIdx) != CustomValueMode::Evolution)
				{
					logger->Err("[%s]: index %d is not declared Evolution (see EvolutionValues in [General]).", sCustomValueEvoRuleX.c_str(), iCustomValueIdx);
					iCustomValueIdx = -1;
				}

				if (iCustomValueIdx != -1 && ParseCustomValueBonus(iRuleNumber, sBonus, iCustomValueIdx, m_customValueEvolutionRuleSets))
				{
					bool bValidRule = true;

					if (ini.get(sCustomValueEvoRuleX).has("Area"))
					{
						std::string sValue = ini.get(sCustomValueEvoRuleX).get("Area");

						bValidRule = ParseCustomValueRuleFeatOrAreaType(sCustomValueEvoRuleX, iRuleNumber, sValue, iCustomValueIdx,
							RuleParser::RuleType::AREA, m_customValueEvolutionRuleSets);
					}

					if (bValidRule && ini.get(sCustomValueEvoRuleX).has("Feat"))
					{
						std::string sValue = ini.get(sCustomValueEvoRuleX).get("Feat");

						bValidRule = ParseCustomValueRuleFeatOrAreaType(sCustomValueEvoRuleX, iRuleNumber, sValue, iCustomValueIdx,
							RuleParser::RuleType::FEAT, m_customValueEvolutionRuleSets);
					}

					if (bValidRule && ini.get(sCustomValueEvoRuleX).has("Extra"))
					{
						std::string sValue = ini.get(sCustomValueEvoRuleX).get("Extra");

						bValidRule = ParseCustomValueRuleFeatOrAreaType(sCustomValueEvoRuleX, iRuleNumber, sValue, iCustomValueIdx,
							RuleParser::RuleType::EXTRA, m_customValueEvolutionRuleSets);
					}

					if (!bValidRule)
					{
						CustomValueRuleSet* rsToDel = m_customValueEvolutionRuleSets[iCustomValueIdx].back();
						m_customValueEvolutionRuleSets[iCustomValueIdx].pop_back();
						delete rsToDel;
					}
				}
			}
			else
			{
				logger->Err("[CustomValueRuleFile]: No CustomValue or Bonus field for [%s].", sCustomValueEvoRuleX.c_str());
			}

			iRuleNumber++;
			sCustomValueEvoRuleX = sCustomValueEvoRuleBase + std::to_string(iRuleNumber);
		}
	}
}



//finalment...
//0x005fe62d   XOR EBX, EBX   /  CMP dword ptr [ESI + 0x1a4], EBX  => 0x005fe635
//ESI === ptrCreature
//ebx === ptr servertimer


//ebx == timermodule ?
// 0x005fe647   CMP dword ptr [ESI + 0xeec],EBX   (6 octets)  retours => 0x005fe64d    
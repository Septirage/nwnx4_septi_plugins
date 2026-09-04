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
#include "RuleParser.h"
#include "BonusParser.h"


extern std::unique_ptr<LogNWNX> logger;

typedef void (__cdecl * NWN2Heap_Deallocate_Proc)(void *p);
extern NWN2Heap_Deallocate_Proc NWN2Heap_Deallocate;




bool bCustomValuesActivated = false;
int uCustomValuesNumbers = 0;
uint32_t uPLCstValue[5];
uint32_t uPrepCstValue = 0;

const char* pStrCustomValue = "CustomValues";
const char* pStrValueCst = "Value";







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
	size_t size = uCustomValuesNumbers * sizeof(int);

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

	prepareSaveLoadCustomValues();
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
	//Bad index
	if (!bCustomValuesActivated || iIdx < 0 || iIdx >= uCustomValuesNumbers)
		return 0;

	GameObject* myCreature = CreatureFromIdForCustomValue(objectID);
	if (myCreature == NULL)
		return 0;

	return _GetCustomValue(myCreature, iIdx);
}


int GetCreatureCustomValue(GameObject* pCreature, int iIdx)
{
	if (!bCustomValuesActivated || iIdx < 0 || iIdx >= uCustomValuesNumbers)
		return 0;

	return _GetCustomValue(pCreature, iIdx);
}


enum class CustomValueMode { Raw, Computed, Evolution };
static std::vector<CustomValueMode> g_customValueModes; // rempli au parsing de CustomValueEFF.ini
/*
int GetCustomValueForRule(int index, void* pCreatureBlock)
{
	if (index < 0 || index >= CustomValuesState::uCount)
		return 0;

	switch (g_customValueModes[index])
	{
	case CustomValueMode::Computed:
		logger->Warn("CustomValue: mode Computed pas encore implémenté, retourne 0");
		return 0; // stub temporaire
	case CustomValueMode::Raw:
	case CustomValueMode::Evolution:
	default:
		return ((int*)pCreatureBlock)[index]; // lecture directe existante
	}
}
*/



#ifdef PASFAITPASFAIT
void desinitCustomValuesHooks()
{
	//Clear Rule
	for (auto ruleSet : m_reduceSpeedRuleList) {
		delete ruleSet;
	}
	m_reduceSpeedRuleList.clear();
}

void initCustomValuesHooks(std::string nxhome, std::string sFileName)
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
		logger->Err("Cant find CustomValuesFile : %s", sFileName.c_str());
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

		ReduceSpeedPatches[3].Remove();
		ReduceSpeedPatches[2].Remove();
		ReduceSpeedPatches[1].Remove();
		ReduceSpeedPatches[0].Remove();
		/*
		int i = 0;
		while(ReduceSpeedPatches[i].Remove()) {
		i++;
		}
		*/
	}
}


#endif

#include <misc/log.h>
#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include <misc/Patch.h>
#include <misc/ini.h>
#include <hook/scriptManagement.h>

#include "../../septutil/NwN2DataPos.h"
#include "../../septutil/NwN2Utilities.h"
//#include "../../septutil/intern2daMgt.h"

#include <nwn2heap.h>

#include <cstdint>
#include <detours/detours.h>

std::string sWeightHookScript = "";
std::string sScriptOnLoaded = "";
std::string sChangeGoldScript = "";
std::string sTransitionScript = "";
std::string sItemBaseCostScript = "";
std::string g_sCanIStackScript = "";
std::string g_sOnSplitScript = "";

extern std::unique_ptr<LogNWNX> logger;

#define OFFS_CalculateInventoryWeightHook 0x005D66BD
#define OFFS_WPTransition		0x005D177B
#define OFFS_DoorTransition		0x005D19CC

#define OFFS_EndOfCalculateBaseCost	0x005d49ab
#define OFFS_EndCanIStack			0x005d3ab1

#define OFFS_EndOfSplit				0x005e6353
//0x005c8420

unsigned long ReturnEndICanStack = 0x005d3ab9;
unsigned long ReturnICantStack = 0x005d39ad;

unsigned long ReturnCalculateBaseCost = 0x005d49b1;

// 1.23
// g_pVirtualMachine
constexpr uintptr_t NWN2_OFFSET_CVIRTUALMACHINE = 0x00864424;
// CVirtualMachine::ExecuteScript
constexpr uintptr_t NWN2_OFFSET_EXECUTESCRIPT = 0x0072B380;

struct CVirtualMachine
{

};

using CVirtualMachine_ExecuteScript_t = BOOL(__thiscall*)(CVirtualMachine* thisVM,
	const NWN::CExoString& scriptName, NWN::OBJECTID objectId, uint32_t unknown1, uint32_t unknown2);
CVirtualMachine_ExecuteScript_t CVirtualMachine_ExecuteScript =
std::bit_cast<CVirtualMachine_ExecuteScript_t>(NWN2_OFFSET_EXECUTESCRIPT);

CVirtualMachine*
GetNwn2VirtualMachine()
{
	const auto vm = std::bit_cast<CVirtualMachine**>(NWN2_OFFSET_CVIRTUALMACHINE);
	return *vm;
}












/*
/////////////////////////// Weight
int32_t __fastcall CalculateWeightScriptCaller(uint32_t uId, int32_t currentWeight)
{
	GameObjectManager m_ObjectManager;
	CreatureObject* creatureWithInventory;
	NWN::OBJECTID oID = (NWN::OBJECTID)uId;

	if ((creatureWithInventory = m_ObjectManager.GetCreature(oID)) == NULL)
		return currentWeight;


	int32_t inventoryWeight = currentWeight;
	int32_t equipementWeight = *(int32_t*)((char*)creatureWithInventory + AmCrtWeightEq);
	int32_t totalWeight = currentWeight + equipementWeight;

	int32_t newTotalWeight  = 0;

	bool isExecScriptOk = true;

	NWScript::ClearScriptParams();
	NWScript::AddScriptParameterObject(oID);
	NWScript::AddScriptParameterInt((int32_t)totalWeight);
	//NWScript::AddScriptParameterInt((int32_t)inventoryWeight);
	//NWScript::AddScriptParameterInt((int32_t)equipementWeight);
	newTotalWeight = NWScript::ExecuteScriptEnhanced(sWeightHookScript.c_str(), oID, false, &isExecScriptOk);

	if (!isExecScriptOk)
	{
		return currentWeight;
	}

	return (newTotalWeight - equipementWeight);
}

// Calculate inventory Weight : 5d65c0
unsigned long ReturnToCalculateInventoryWeight = 0x005D66C2;

//Hook at 5d66bd
__declspec(naked) void ReturnOfInventoryWeight()
{
	__asm
	{
		MOV		EAX, dword ptr[ESP + 0x10]

		PUSH	EDX
		PUSH	ECX

		//Set current weight as second param
		MOV		EDX, EAX

		//Get Creature/ObjectID as first param
		MOV		ECX, dword ptr[ESP + 0x1C]
		MOV		ECX, dword ptr[ECX]

		CALL	CalculateWeightScriptCaller

		POP		ECX
		POP		EDX

		POP		EDI
		JMP		dword ptr[ReturnToCalculateInventoryWeight]
	}
}
*/

/*
Patch _CalculateWeightPatch[] =
{
Patch(OFFS_CalculateInventoryWeightHook, (char*)"\xe9\x00\x00\x00\x00", (int)5),
Patch(OFFS_CalculateInventoryWeightHook + 1, (relativefunc)ReturnOfInventoryWeight),

Patch()
};
Patch *CalculateWeightPatch = _CalculateWeightPatch;
*/
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////// Global OnClientLoaded
/////////////////////////////////////////////////////////////////////////////////

#define OFFS_AREALOAD_ONCLIENTENTER_TESTSCRIPTEMPTY 0x00595C43
#define OFFS_AREALOAD_ONCLIENTENTER_SCRIPTCALL		0x00595cf1

unsigned long ReturnAreaLoadOnClientEnter		   = 0x00595C4B;
unsigned long ReturnAreaLoadOnClientEnterAfterCall = 0x00595cf6;

unsigned long CallOnClientEnterScript = 0x0072b380;



uint8_t gTestOnClientEnterScript = 0;
uint32_t gAreaIDOnClientEnter = 0;



void __fastcall CallGlobalOnClientEnter(NWN::OBJECTID iAreaID)
{
	const NWN::CExoString script = {.m_sString       = sScriptOnLoaded.data(),
		.m_nBufferLength = std::size((sScriptOnLoaded)) + 1};
	const auto vm                = GetNwn2VirtualMachine();

	CVirtualMachine_ExecuteScript(vm, script, iAreaID, 1, 0);
}


__declspec(naked) void AreaOnClientEnter_TestScript()
{
	__asm
	{
		//Save the value, so we don't have to test it again.
		MOV		gTestOnClientEnterScript, AL

		MOV		AL, 0x0

		//Continue like if we have a script
		JMP		dword ptr[ReturnAreaLoadOnClientEnter]
	}
}

__declspec(naked) void AreaOnClientEnter_CallScript()
{
	__asm
	{
		MOV		AL, gTestOnClientEnterScript
		MOV		gAreaIDOnClientEnter, EDX

		TEST	AL, AL
		JNZ		NoAreaOnClientEnterScript

		CALL	[CallOnClientEnterScript]

		PUSH	EAX

		JMP		AreaOnClientEnterGeneralScript

		//Unstack all parameters
NoAreaOnClientEnterScript:
		POP		ESI
		POP		EDX
		POP		EDI
		POP		EDI

		PUSH	0x0

		//Call The GenericOnClientEnter
AreaOnClientEnterGeneralScript:

		MOV		ECX, gAreaIDOnClientEnter
		CALL	CallGlobalOnClientEnter

		POP		EAX

		JMP		dword ptr[ReturnAreaLoadOnClientEnterAfterCall]
	}
}

Patch _GlobalOnClientEnterPatch[] =
{
	Patch((DWORD)OFFS_AREALOAD_ONCLIENTENTER_TESTSCRIPTEMPTY, (char*)"\xe9\x00\x00\x00\x00\x90\x90\x90", (int)8), //JMP
	Patch(OFFS_AREALOAD_ONCLIENTENTER_TESTSCRIPTEMPTY + 1, (relativefunc)AreaOnClientEnter_TestScript),

	Patch((DWORD)OFFS_AREALOAD_ONCLIENTENTER_SCRIPTCALL, (char*)"\xe9\x00\x00\x00\x00", (int)5), //JMP
	Patch(OFFS_AREALOAD_ONCLIENTENTER_SCRIPTCALL + 1, (relativefunc)AreaOnClientEnter_CallScript),

	Patch()
};
Patch *GlobalOnClientEnterPatch = _GlobalOnClientEnterPatch;


/////////////////////////////////////////////////////////////////////////////////
/////////////////////////// On GoldChange (GainGold/TakeGold)
/////////////////////////////////////////////////////////////////////////////////
int(__fastcall* OriginalGG)(void* pThis, void* _, int amount, int fb, NWN::OBJECTID obj);

int(__fastcall* OriginalTG)(void* pThis, void* _, int amount, int fb, NWN::OBJECTID obj);

DWORD
FindHookGG()
{
	// 83 7C 24 0C 00 53 56 57 8B F1
	// 8B 44 24 04 56 57 6A 01 50 8B F9 E8 ** ** ** ** 8B F0 85 F6 75 07 5F 33 C0 5E C2 18 00 8B 54
	// 24 14
	char* ptr = (char*)0x400000;
	while (ptr < (char*)0x800000) {
		if ((ptr[0] == (char)0x83) && (ptr[1] == (char)0x7C) && (ptr[2] == (char)0x24) &&
			(ptr[3] == (char)0x0C) && (ptr[4] == (char)0x00) && (ptr[5] == (char)0x53) &&
			(ptr[6] == (char)0x56) && (ptr[7] == (char)0x57) && (ptr[8] == (char)0x8B) &&
			(ptr[9] == (char)0xF1))
			return (DWORD)ptr;
		else
			ptr++;
	}
	return NULL;
}

DWORD 
FindHookTG()
{
	// 83 7C 24 0C 00 53 56 57 8B F9
	char* ptr = (char*)0x400000;
	while (ptr < (char*)0x800000) {
		if ((ptr[0] == (char)0x83) && (ptr[1] == (char)0x7C) && (ptr[2] == (char)0x24) &&
			(ptr[3] == (char)0x0C) && (ptr[4] == (char)0x00) && (ptr[5] == (char)0x53) &&
			(ptr[6] == (char)0x56) && (ptr[7] == (char)0x57) && (ptr[8] == (char)0x8B) &&
			(ptr[9] == (char)0xF9))
			return (DWORD)ptr;
		else
			ptr++;
	}
	return NULL;
}



int __fastcall GGHookProc(void* pThis, void* _, int amount, int fb, NWN::OBJECTID obj)
{
	int32_t newAmount = amount;
	NWN::CNWSCreature* ptrC = (NWN::CNWSCreature*)pThis;
	bool isExecScriptOk = true;

	NWScript::ClearScriptParams();
	NWScript::AddScriptParameterObject(ptrC->m_idSelf);
	NWScript::AddScriptParameterInt((int32_t)newAmount);
	newAmount = NWScript::ExecuteScriptEnhanced(sChangeGoldScript.c_str(), ptrC->m_idSelf, false, &isExecScriptOk, true);

	if (!isExecScriptOk)
	{
		newAmount = amount;
	}

	return OriginalGG(pThis, _, newAmount, fb, obj);
}

int __fastcall TGHookProc(void* pThis, void* _, int amount, int fb, NWN::OBJECTID obj)
{
	int32_t newAmount = (amount * -1);
	NWN::CNWSCreature* ptrC = (NWN::CNWSCreature*)pThis;
	bool isExecScriptOk = true;

	NWScript::ClearScriptParams();
	NWScript::AddScriptParameterObject(ptrC->m_idSelf);
	NWScript::AddScriptParameterInt((int32_t)newAmount);
	newAmount = NWScript::ExecuteScriptEnhanced(sChangeGoldScript.c_str(), ptrC->m_idSelf, false, &isExecScriptOk, true);

	if (!isExecScriptOk)
	{
		newAmount = amount;
	}
	else
	{
		newAmount = (newAmount * -1);
	}

	return OriginalTG(pThis, _, newAmount, fb, obj);
}

int
HookChgGold()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	int tg_success, gg_success, detour_success;
	DWORD tg = FindHookTG();
	if (tg) {
		logger->Info("o TakeGold located at %x.", tg);
		*(DWORD*)&OriginalTG = tg;
		tg_success           = DetourAttach(&(PVOID&)OriginalTG, TGHookProc) == 0;
	} else {
		logger->Info("! TakeGold locate failed.");
		return 0;
	}

	DWORD gg = FindHookGG();
	if (gg) {
		logger->Info("o GiveGold located at %x.", gg);
		*(DWORD*)&OriginalGG = gg;
		gg_success           = DetourAttach(&(PVOID&)OriginalGG, GGHookProc) == 0;
	} else {
		logger->Info("! GiveGold locate failed.");
		return 0;
	}
	detour_success = DetourTransactionCommit() == 0;
	return gg_success && tg_success && detour_success;
}


/////////////////////////////////////////////////////////////////////////////////
/////////////////////////// Transition Hook
/////////////////////////////////////////////////////////////////////////////////
unsigned long CalledByTransition	= 0x005EC920;
unsigned long ReturnWPTransition	= 0x005D1780;
unsigned long ReturnDoorTransition	= 0x005D19D1;

int __fastcall StartTransitionCallCondScript(void* nop, void* _, void* ptrCreature, NWN::OBJECTID objectTransID)
{
	bool isExecScriptOk = true;
	int iTransiOk = 1;

	GameObject* ptrC = (GameObject*)ptrCreature;

	NWScript::ClearScriptParams();
	NWScript::AddScriptParameterObject(ptrC->GetObjectId());
	NWScript::AddScriptParameterObject(objectTransID);
	iTransiOk = NWScript::ExecuteScriptEnhanced(sTransitionScript.c_str(), ptrC->GetObjectId(), false, &isExecScriptOk);

	if (!isExecScriptOk || iTransiOk != 1)
		return 0;

	return 1;
}

__declspec(naked) void WayPointTransition()
{
	__asm
	{
		call dword ptr[CalledByTransition]
		test    eax, eax
		je      GoBack1
		push    ebx
		push    esi
		call    StartTransitionCallCondScript
		GoBack1 :
		jmp     dword ptr[ReturnWPTransition]
	}
}

__declspec(naked) void DoorTransition()
{
	__asm
	{
		call dword ptr[CalledByTransition]
		test     eax, eax
		je      GoBack2
		push    ebx
		push    esi
		call    StartTransitionCallCondScript
		GoBack2 :
		jmp     dword ptr[ReturnDoorTransition]
	}
}


Patch _StartTransitionPatch[] =
{
	Patch((DWORD)OFFS_WPTransition, (char*)"\xe9\x00\x00\x00\x00", (int)5), //JMP
	Patch(OFFS_WPTransition + 1, (relativefunc)WayPointTransition),

	Patch((DWORD)OFFS_DoorTransition, (char*)"\xe9\x00\x00\x00\x00", (int)5), //JMP
	Patch(OFFS_DoorTransition + 1, (relativefunc)DoorTransition),

	Patch()
};
Patch *StartTransitionPatch = _StartTransitionPatch;


/////////////////////////////////////////////////////////////////////////////////
/////////////////////////// OnCostChange
/////////////////////////////////////////////////////////////////////////////////

int __fastcall CallCalculateBaseCost(int iCurrentBaseCost, NWN::CNWSItem* pItem)
{
	if (sItemBaseCostScript != "")
	{
		GameObject* pObj = pItem->AsGameObject();

		bool isExecScriptOk = true;

		NWScript::ClearScriptParams();
		NWScript::AddScriptParameterObject(pObj->GetObjectId());
		NWScript::AddScriptParameterInt(iCurrentBaseCost);
		int iRet = NWScript::ExecuteScriptEnhanced(sItemBaseCostScript.c_str(), GetModuleID(), false, &isExecScriptOk, true);
	}

	return iCurrentBaseCost;
}

__declspec(naked) void EndOfCalculateBaseCost()
{
	__asm
	{
		MOV dword ptr[ESI + 0xA94], ECX
		PUSH EDX
		PUSH EAX
		PUSH ECX

		MOV		EDX, ESI
		CALL	CallCalculateBaseCost

		MOV dword ptr[ESI + 0xA94], EAX

		POP ECX
		MOV ECX, EAX
		POP EAX
		POP EDX

		JMP dword ptr[ReturnCalculateBaseCost]
	}
}

Patch _ItemBaseCostCalcPatch[] =
{
	Patch((DWORD)OFFS_EndOfCalculateBaseCost, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6), //JMP
	Patch(OFFS_EndOfCalculateBaseCost + 1, (relativefunc)EndOfCalculateBaseCost),

	Patch()
};
Patch* ItemBaseCostCalcPatch = _ItemBaseCostCalcPatch;




/////////////////////////////////////////////////////////////////////////////////
/////////////////////////// CanIStack
/////////////////////////////////////////////////////////////////////////////////

uint32_t g_iStackFixChoice = 0;

uint32_t __fastcall CanIStackUpgraded(char* pItem1, char* pItem2)
{
	//Basic fix, really test item properties
	if (g_iStackFixChoice != 0)
	{
		//Number are already tested so only need one of them
		uint32_t iNumberP1 = *(uint32_t*)(pItem1 + AmItmPropertyNb);
		AmItmProperty* pProperties1 = *(AmItmProperty**)(pItem1 + AmItmPropertyPtr);
		AmItmProperty* pProperties2 = *(AmItmProperty**)(pItem2 + AmItmPropertyPtr);

		for (int i = 0; i < iNumberP1; i++)
		{
			//If any difference on property => not stackable
			if (pProperties1[i].uPropertyName != pProperties2[i].uPropertyName ||
				pProperties1[i].uSubType != pProperties2[i].uSubType ||
				pProperties1[i].uCostTable != pProperties2[i].uCostTable ||
				pProperties1[i].uCostValue != pProperties2[i].uCostValue ||
				pProperties1[i].uParam1 != pProperties2[i].uParam1 ||
				pProperties1[i].uParam1Value != pProperties2[i].uParam1Value ||
				pProperties1[i].uChanceAppear != pProperties2[i].uChanceAppear
				)
			{
				return 0;
			}
		}
	}

	if (g_sCanIStackScript != "")
	{
		NWN::CNWSItem* Item1 = (NWN::CNWSItem*)pItem1;
		NWN::CNWSItem* Item2 = (NWN::CNWSItem*)pItem2;
		GameObject* ob1 = Item1->AsGameObject();
		GameObject* ob2 = Item2->AsGameObject();

		bool isExecScriptOk = true;

		NWScript::ClearScriptParams();
		NWScript::AddScriptParameterObject(ob1->GetObjectId());
		NWScript::AddScriptParameterObject(ob2->GetObjectId());
		int iRet = NWScript::ExecuteScriptEnhanced(g_sCanIStackScript.c_str(), GetModuleID(), false, &isExecScriptOk, true);

		iRet = (iRet != 0) ? 1 : 0;


		if (isExecScriptOk)
		{
			return iRet;
		}
	}

	return 1;
}

__declspec(naked) void UpgradeCanIStack()
{
	__asm
	{
		MOV		ECX, ESI
		MOV		EDX, EBP
		CALL	CanIStackUpgraded
		TEST	EAX, EAX
		JZ		JmpToDontStack

		POP		EDI
		POP		EBX
		POP		ESI
		
		JMP	dword ptr[ReturnEndICanStack]

JmpToDontStack:
		JMP dword ptr[ReturnICantStack]
	}
}

Patch _FixStackingPatch[] =
{
	Patch(OFFS_EndCanIStack, (char*)"\xe9\x00\x00\x00\x00\x90\x90\x90", (int)8),
	Patch(OFFS_EndCanIStack + 1, (relativefunc)UpgradeCanIStack),

	Patch()
};

Patch *FixStackingPatch = _FixStackingPatch;


/////////////////////////////////////////////////////////////////////////////////
/////////////////////////// OnSplit
/////////////////////////////////////////////////////////////////////////////////
unsigned long CalledBySplitStack = 0x005c8420;
unsigned long ReturnSplitStack  = 0x005e635c;


void __fastcall CallOnSplitStack(char* pOrigin, char* pNew)
{
	if (g_sOnSplitScript != "")
	{
		NWN::CNWSItem* Item1 = (NWN::CNWSItem*)pOrigin;
		NWN::CNWSItem* Item2 = (NWN::CNWSItem*)pNew;
		GameObject* ob1 = Item1->AsGameObject();
		GameObject* ob2 = Item2->AsGameObject();

		bool isExecScriptOk = true;

		NWScript::ClearScriptParams();
		NWScript::AddScriptParameterObject(ob1->GetObjectId());
		NWScript::AddScriptParameterObject(ob2->GetObjectId());
		int iRet = NWScript::ExecuteScriptEnhanced(g_sOnSplitScript.c_str(), GetModuleID(), false, &isExecScriptOk, true);
	}
}

__declspec(naked) void PrepareOnSplitCall()
{
	__asm
	{
		PUSH	EDX
		MOV		EDX, dword ptr [ESP + 0x14]
		MOV		ECX, EBP
		CALL	CallOnSplitStack

		POP		EDX


		//Redo the old code
		PUSH	0x1
		MOV		ECX, ESI
		CALL	dword ptr[CalledBySplitStack]
		JMP		dword ptr[ReturnSplitStack]
	}
}

Patch _OnSplitStackPatch[] =
{
	Patch(OFFS_EndOfSplit, (char*)"\xe9\x00\x00\x00\x00\x90\x90\x90\x90", (int)9),
	Patch(OFFS_EndOfSplit + 1, (relativefunc)PrepareOnSplitCall),

	Patch()
};

Patch *OnSplitStackPatch = _OnSplitStackPatch;



/////////////////////////////////////////////////////////////////////////////////
/////////////////////////// Global OnDestroy
/////////////////////////////////////////////////////////////////////////////////
// Every object class (creature: 0x5f1720 -> 0x5850e0) uses this Destroy (vtable +8). The destruction is certain:
// the event 0xB handlers already checked their conditions (destroyable...) and removed the object from its area.
#define OFFS_DestroyObject				0x005850e0
// AI master: call of the object event handler (vtable +0x128). ESI = event, EDI = object
#define OFFS_ObjectEventHandlerCall		0x0066f1cd
// Return address of the temporary trigger the DM cheat 0x0B creates (never in an area) and destroys
#define RETADDR_DMCheatTempTrigger		0x00629755

unsigned long CalledByDestroyObject		= 0x00413eb0;
unsigned long ReturnDestroyObject		= 0x005850e8;
unsigned long ReturnObjectEventHandler	= 0x0066f28e;

std::string g_sOnDestroyScript = "";
uint32_t g_uOnDestroyObjectTypes = 0x41;	//OBJECT_TYPE_CREATURE | OBJECT_TYPE_PLACEABLE

struct DestroyedLocation
{
	NWN::OBJECTID oObject;
	NWN::OBJECTID oArea;
	float fPosition[3];
	float fOrientation[2];
};

//Location of the object handled by the current event 0xB, kept before its handler removes it from its area
static DestroyedLocation g_destroyEventLocation = { NWN::INVALIDOBJID, NWN::INVALIDOBJID, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };
//Location given to the GlobalOnDestroy script running
static DestroyedLocation g_destroyedLocation = { NWN::INVALIDOBJID, NWN::INVALIDOBJID, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };

//Object type byte (+0xA4) -> OBJECT_TYPE_* (as the NWScript GetObjectType 0x68ef20)
static uint32_t GetNWScriptObjectType(uint8_t uType)
{
	switch (uType)
	{
		case 5:		return 0x1;		//Creature
		case 6:		return 0x2;		//Item
		case 7:		return 0x4;		//Trigger
		case 9:		return 0x40;	//Placeable
		case 10:	return 0x8;		//Door
		case 11:	return 0x10;	//Area of effect
		case 12:	return 0x20;	//Waypoint
		case 13:	return 0x100;	//Encounter
		case 14:	return 0x80;	//Store
		case 0x15:	return 0x200;	//Light
		case 0x16:	return 0x400;	//Placed effect
		default:	return 0;
	}
}

void __fastcall KeepDestroyEventLocation(char* pObject)
{
	g_destroyEventLocation.oObject = *(NWN::OBJECTID*)(pObject + 0xa0);
	g_destroyEventLocation.oArea = *(NWN::OBJECTID*)(pObject + 0x144);
	for (int i = 0; i < 3; i++)
		g_destroyEventLocation.fPosition[i] = *(float*)(pObject + 0x70 + i * 4);
	g_destroyEventLocation.fOrientation[0] = *(float*)(pObject + 0x1c);
	g_destroyEventLocation.fOrientation[1] = *(float*)(pObject + 0x20);
}

void __fastcall CallGlobalOnDestroy(char* pObject, uint32_t uReturnAddress)
{
	if (uReturnAddress == RETADDR_DMCheatTempTrigger)
		return;

	if ((GetNWScriptObjectType(*(uint8_t*)(pObject + 0xa4)) & g_uOnDestroyObjectTypes) == 0)
		return;

	NWN::OBJECTID oObject = *(NWN::OBJECTID*)(pObject + 0xa0);

	//Kept for a destruction done inside a GlobalOnDestroy script (DespawnRosterMember...)
	DestroyedLocation previousLocation = g_destroyedLocation;

	//Roster despawn, timed placed effect, DM kill: destroyed without event 0xB, no location
	if (g_destroyEventLocation.oObject == oObject)
		g_destroyedLocation = g_destroyEventLocation;
	else
		g_destroyedLocation = { oObject, NWN::INVALIDOBJID, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f} };

	const NWN::CExoString script = {.m_sString       = g_sOnDestroyScript.data(),
		.m_nBufferLength = std::size(g_sOnDestroyScript) + 1};
	CVirtualMachine_ExecuteScript(GetNwn2VirtualMachine(), script, oObject, 1, 0);

	g_destroyedLocation = previousLocation;
}

//Area of OBJECT_SELF before its destruction, only during the GlobalOnDestroy script
int GetDestroyedArea()
{
	return (int)g_destroyedLocation.oArea;
}

//"X", "Y", "Z" (position), "OrientX", "OrientY" (facing vector) of OBJECT_SELF before its destruction
float GetDestroyedLocationFloat(std::string_view sValue)
{
	if (g_destroyedLocation.oArea == NWN::INVALIDOBJID)
		return 0.0f;

	if (sValue == "X")
		return g_destroyedLocation.fPosition[0];
	else if (sValue == "Y")
		return g_destroyedLocation.fPosition[1];
	else if (sValue == "Z")
		return g_destroyedLocation.fPosition[2];
	else if (sValue == "OrientX")
		return g_destroyedLocation.fOrientation[0];
	else if (sValue == "OrientY")
		return g_destroyedLocation.fOrientation[1];

	return 0.0f;
}

__declspec(naked) void DestroyObjectHook()
{
	__asm
	{
		PUSH	ECX
		MOV		EDX, dword ptr [ESP + 0x4]	//Return address of Destroy
		CALL	CallGlobalOnDestroy
		POP		ECX

		//Redo the old code
		PUSH	ESI
		MOV		ESI, ECX
		CALL	dword ptr [CalledByDestroyObject]
		JMP		dword ptr [ReturnDestroyObject]
	}
}

__declspec(naked) void ObjectEventHandlerCallHook()
{
	__asm
	{
		CMP		dword ptr [ESI + 0x10], 0xB	//Event destroy
		JNE		CallEventHandler

		MOV		ECX, EDI
		CALL	KeepDestroyEventLocation

CallEventHandler:
		//Redo the old code (call of the event handler)
		MOV		EAX, dword ptr [ESI + 0x4]
		MOV		ECX, dword ptr [ESI]
		MOV		EDX, dword ptr [EBP]
		MOV		EDX, dword ptr [EDX + 0x128]
		PUSH	EAX
		MOV		EAX, dword ptr [ESI + 0x14]
		PUSH	ECX
		MOV		ECX, dword ptr [ESI + 0x8]
		PUSH	EAX
		MOV		EAX, dword ptr [ESI + 0x10]
		PUSH	ECX
		PUSH	EAX
		MOV		ECX, EBP
		CALL	EDX

		//The event is done: the location must not be used for another destruction
		MOV		dword ptr [g_destroyEventLocation], 0x7F000000

		JMP		dword ptr [ReturnObjectEventHandler]
	}
}

Patch _GlobalOnDestroyPatch[] =
{
	Patch(OFFS_DestroyObject, (char*)"\xe9\x00\x00\x00\x00\x90\x90\x90", (int)8),
	Patch(OFFS_DestroyObject + 1, (relativefunc)DestroyObjectHook),

	Patch(OFFS_ObjectEventHandlerCall, (char*)"\xe9\x00\x00\x00\x00", (int)5),
	Patch(OFFS_ObjectEventHandlerCall + 1, (relativefunc)ObjectEventHandlerCallHook),

	Patch()
};

Patch *GlobalOnDestroyPatch = _GlobalOnDestroyPatch;



/////////////////////////////////////////////////////////////////////////////////
/////////////////////////// DM Possess/Unpossess
/////////////////////////////////////////////////////////////////////////////////
#define OFFS_DMPossessCreature	0x0067a450	// DM message possess/unpossess (this = player)
#define OFFS_GetModuleLimbo		0x00549e90	// __fastcall (ServerExoApp) -> module
#define OFFS_RemoveFromLimbo	0x00613bc0	// __thiscall (module, object id)

typedef void* (__fastcall *GetModuleLimbo_t)(void* pServerExoApp);
typedef void (__thiscall *RemoveFromLimbo_t)(void* pModule, NWN::OBJECTID oObject);

static GetModuleLimbo_t const GetModuleLimbo = (GetModuleLimbo_t)OFFS_GetModuleLimbo;
static RemoveFromLimbo_t const RemoveFromLimbo = (RemoveFromLimbo_t)OFFS_RemoveFromLimbo;

void(__fastcall* OriginalDMPossessCreature)(void* pPlayer, void* _, NWN::OBJECTID oTarget, uint32_t uMode);

// On possess, the DM avatar is sent to the module limbo list. On unpossess, the engine puts it back
// in the area but never removes it from that list (RecallCreatureFromLimboToLocation does it).
// GetLimboCreatureCount/GetCreatureInLimbo then still see the avatar, and a recall would teleport the DM.
void __fastcall DMPossessCreatureHook(void* pPlayer, void* _, NWN::OBJECTID oTarget, uint32_t uMode)
{
	NWN::OBJECTID oAvatar = *(NWN::OBJECTID*)((char*)pPlayer + 0x118);	// Avatar while possessing

	OriginalDMPossessCreature(pPlayer, _, oTarget, uMode);

	//Not an unpossess
	if (oAvatar == NWN::INVALIDOBJID || *(NWN::OBJECTID*)((char*)pPlayer + 0x118) != NWN::INVALIDOBJID)
		return;

	GameObjectManager m_ObjectManager;
	GameObject* pAvatar = m_ObjectManager.GetGameObject(oAvatar);
	if (pAvatar == NULL)
		return;

	//Only once back in an area (else the jump action removes it itself)
	if (*(NWN::OBJECTID*)((char*)pAvatar + 0x144) == NWN::INVALIDOBJID)
		return;

	void* pServerExoApp = *(void**)(*(char**)0x0086442c + 0x4);
	void* pModule = GetModuleLimbo(pServerExoApp);
	if (pModule != NULL)
		RemoveFromLimbo(pModule, oAvatar);
}

int HookDMPossessCreature()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	*(DWORD*)&OriginalDMPossessCreature = OFFS_DMPossessCreature;
	bool bAttachOk = DetourAttach(&(PVOID&)OriginalDMPossessCreature, DMPossessCreatureHook) == 0;
	bool bCommitOk = DetourTransactionCommit() == 0;
	return bAttachOk && bCommitOk;
}



void SmallHookFunctions(SimpleIniConfig* config)
{
	int iTest = 0;
	int i = 0;
	std::string sScript = "";

	/*
	//Weight Hook
	config->Read("CalculateWeightScript", &sScript, std::string(""));
	if (sScript != "")
	{
		sWeightHookScript = sScript;
		logger->Info("* HookScript for CalculateWeight: %s", sScript.c_str());
		while (CalculateWeightPatch[i].Apply()) {
			i++;
		}
	}
	*/

	//GoldChangeHook
	config->Read("OnGoldChangeScript", &sScript, std::string(""));
	if (sScript != "")
	{
		sChangeGoldScript = sScript;
		logger->Info("* HookScript for OnGoldChange: %s", sScript.c_str());
		HookChgGold();
	}


	//Global OnClientLoaded
	config->Read("GlobalOnClientEnterScript", &sScript, std::string(""));
	if (sScript != "")
	{
		sScriptOnLoaded = sScript;
		logger->Info("* HookScript for GlobalOnClientEnter: %s", sScript.c_str());

		i = 0;
		while (GlobalOnClientEnterPatch[i].Apply()) {
			i++;
		}
	}


	//StartTransitionHook
	config->Read("StartTransitionScript", &sScript, std::string(""));
	if (sScript != "")
	{
		sTransitionScript = sScript;
		logger->Info("* HookScript for StartTransition: %s", sScript.c_str());
		i = 0;
		while (StartTransitionPatch[i].Apply()) {
			i++;
		}
	}

	
	config->Read("OnItemBaseCostCalculationScript", &sScript, std::string(""));
	if (sScript != "")
	{
		sItemBaseCostScript = sScript;
		logger->Info("* Script for ItemBaseCostCalculation: %s", sScript.c_str());
		i = 0;
		while (ItemBaseCostCalcPatch[i].Apply()) {
			i++;
		}
	}

	
	config->Read("FixItemStacking", &iTest, 0);
	config->Read("OnItemStackScript", &sScript, std::string(""));
	if (iTest > 0 || sScript != "")
	{
		if(iTest != 0)
			logger->Info("* StackingVerification bugfix Enabled");
		if (sScript != "")
			logger->Info("* Script for IsStackingPossible: %s", sScript.c_str());

		g_iStackFixChoice = iTest;
		g_sCanIStackScript = sScript;
		i = 0;
		while (FixStackingPatch[i].Apply())
		{
			i++;
		}
	}


	//OnSplitStack hook
	config->Read("OnSplitStackScript", &sScript, std::string(""));
	if (sScript != "")
	{
		g_sOnSplitScript = sScript;
		logger->Info("* HookScript for OnSplitStack: %s", sScript.c_str());
		i = 0;
		while (OnSplitStackPatch[i].Apply()) {
			i++;
		}
	}


	//Global OnDestroy
	config->Read("GlobalOnDestroyScript", &sScript, std::string(""));
	if (sScript != "")
	{
		g_sOnDestroyScript = sScript;
		config->Read("GlobalOnDestroyObjectTypes", &iTest, 0x41);
		g_uOnDestroyObjectTypes = (uint32_t)iTest;
		logger->Info("* HookScript for GlobalOnDestroy: %s (object types %d)", sScript.c_str(), iTest);
		i = 0;
		while (GlobalOnDestroyPatch[i].Apply()) {
			i++;
		}
	}


	//DM avatar kept in the limbo list after unpossess
	config->Read("FixUnpossessLimbo", &iTest, 0);
	if (iTest == 1)
	{
		if (HookDMPossessCreature())
			logger->Info("* FixUnpossessLimbo");
		else
			logger->Info("! FixUnpossessLimbo: hook failed");
	}
}

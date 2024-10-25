
#include "../../misc/log.h"
#include "../../NWN2Lib/NWN2.h"
#include "../../NWN2Lib/NWN2Common.h"
#include "..\..\misc\Patch.h"
#include "../../misc/ini.h"
#include "../../hook/scriptManagement.h"

#include "../../septutil/NwN2DataPos.h"
//#include "../../septutil/intern2daMgt.h"
#include "nwn2heap.h"

#include <cstdint>
#include <detours/detours.h>

std::string sWeightHookScript = "";
std::string sScriptOnLoaded = "";
std::string sChangeGoldScript = "";
std::string sTransitionScript = "";

extern std::unique_ptr<LogNWNX> logger;

#define OFFS_CalculateInventoryWeightHook 0x005D66BD
#define OFFS_WPTransition		0x005D177B
#define OFFS_DoorTransition		0x005D19CC



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
	newAmount = NWScript::ExecuteScriptEnhanced(sChangeGoldScript.c_str(), ptrC->m_idSelf, false, &isExecScriptOk);

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
	newAmount = NWScript::ExecuteScriptEnhanced(sChangeGoldScript.c_str(), ptrC->m_idSelf, false, &isExecScriptOk);

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

/*
Patch _CalculateWeightPatch[] =
{
	Patch(OFFS_CalculateInventoryWeightHook, (char*)"\xe9\x00\x00\x00\x00", (int)5),
	Patch(OFFS_CalculateInventoryWeightHook + 1, (relativefunc)ReturnOfInventoryWeight),

	Patch()
};
Patch *CalculateWeightPatch = _CalculateWeightPatch;
*/

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
}
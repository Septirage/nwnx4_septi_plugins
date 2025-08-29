#include <atomic>
#include <semaphore>
#include <thread>
#include <mutex>

//#include <synchapi.h>


#include "ThreadSynchronisation.h"


#include <misc/log.h>
#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include <misc/Patch.h>
#include <misc/ini.h>

NWN::OBJECTID GetModuleID_()
{
	int ptr = *(int*)OFFS_g_pAppManager;
	ptr = *(int*)(ptr + 4);
	ptr = *(int*)(ptr + 4);
	NWN::OBJECTID result = *(NWN::OBJECTID*)(ptr + 0x10088);
	return result;
}


extern std::unique_ptr<LogNWNX> logger;
/*

*/

//Thread Identification mechanism
static thread_local bool isThreadLocked = false;
static thread_local bool isMainThread   = false;

// Grab the pointerTable only the first time
static bool bSaved = false;
static uint32_t* pPointerTable;

//Contexts handling
static uint32_t currentHandlerId = 0;



/*
Quick structure of copied type
*/

// VMStack
struct ActionVMStack {
	uint32_t paramCount; // +0x00 - Number of parameters
	uint32_t field_04; // +0x04
	uint32_t paramSize; // +0x08 - Size for arrays allocation
	uint8_t* typeList; // +0x0C - Parameter types array
	void** paramList; // +0x10 - Parameter values array
	uint32_t ptrToVMScriptObj; // +0x14
};

// VMScript
struct ActionVMScript {
	ActionVMStack* pParamObj; // +0x00
	uint32_t field_04; // +0x04
	uint32_t field_08; // +0x08
	uint32_t field_0C; // +0x0C
	uint8_t* unknowArray; // +0x10		// ??????
	uint32_t uSizeUknArray; // +0x14
	NWN::CExoString scriptName; // +0x18
	uint32_t field_20; // +0x20
	uint32_t field_24; // +0x24
};


struct ContextData {
	ActionVMScript* data; // ObjActionPtr
};

static std::unordered_map<uint32_t, ContextData> contexts;


// Synchronisation mechanism
static std::mutex g_sharedMutex;
static std::unique_ptr<std::counting_semaphore<10>> actionSemaphore;
static std::atomic<bool> mainThreadWaiting {false};

//Shutting down mechanism
static std::atomic<bool> g_is_shutting_down {false};
static std::atomic<bool> g_UseThreadingSystem {false};
static std::atomic<int> waitingThreadsCount{0};


// Exports for External usages
extern "C" __declspec(dllexport) int __cdecl BeginContextInsertion()
{
	//Avoid possibility of deadlock by faultly calling it from main thread
	if (isMainThread)
	{
		isThreadLocked = true;
		return 1;
	}

	if (g_is_shutting_down.load() || !g_UseThreadingSystem.load())
		return 0;

	actionSemaphore->acquire();

	if (g_is_shutting_down.load())
	{
		actionSemaphore->release();
		return 0;
	}

	waitingThreadsCount++;
	g_sharedMutex.lock();
	waitingThreadsCount--;

	if (g_is_shutting_down.load())
	{
		g_sharedMutex.unlock();
		actionSemaphore->release();
		return 0;
	}

	isThreadLocked = true;
	return 1;
}

__declspec(naked) int __fastcall PutContextInQueue(uint32_t* pThis, void* Unused, uint32_t dateDelay, uint32_t timeDelay, uint32_t uiObject, uint32_t uiObject2, int iPAram, ActionVMScript* pActionObj, int iParam2, int iParm3)
{
	__asm
	{
		mov edx, 0x0066ee60
		jmp edx
	}
}

extern "C" __declspec(dllexport) int __cdecl InsertContext(uint32_t iHandler)
{
	// will garanty that we use threading system and are right context
	if (!isThreadLocked)
		return -1;

	if (contexts.count(iHandler) != 0)
	{
		ActionVMScript* pContext = contexts[iHandler].data;
		//Grab the module objectID instead of presume it is 0 TODO
		PutContextInQueue(pPointerTable, NULL, 0, 0, GetModuleID_(), GetModuleID_(), 1, pContext, 10, 1);

		contexts.erase(iHandler);

		return 1;
	}
	return -2;
}

extern "C" __declspec(dllexport) int __cdecl EndContextInsertion()
{
	//will garanty that we use threading system and are right context
	if (!isThreadLocked)
		return -1;

	if (isMainThread)
	{
		isThreadLocked = false;
		return g_is_shutting_down ? 0 : 1;
	}

	g_sharedMutex.unlock();

	// Wait that the main loop start again
	while (mainThreadWaiting.load() && !g_is_shutting_down.load()) {
		std::this_thread::yield();
	}

	actionSemaphore->release();
	isThreadLocked = false;
	return g_is_shutting_down ? 0 : 1;
}

void __fastcall PutMutexLock()
{
	mainThreadWaiting.store(true);
	g_sharedMutex.unlock();
	while (waitingThreadsCount.load() > 0) {
		std::this_thread::yield();
	}
	g_sharedMutex.lock();
	mainThreadWaiting.store(false);
}

void __fastcall PrepareCloseThreads()
{
	g_is_shutting_down = true;
	try {
		g_sharedMutex.unlock();		//Should always be ok
	} catch (...) {
		// Ignore if already unlock
	}

	//Let time for the cascade stuff to happens
	std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

#define OFFS_MainLoopAction 0x0059a50d
#define OFFS_ClosePrgAction 0x0040b226

unsigned long CallSrvFct       = 0x004fcf00;
unsigned long ReturnToMainLoop = 0x0059a512;

unsigned long CallSrvFctClose = 0x0042d8b0;
unsigned long ReturnToClose   = 0x0040b22b;

// call 04fcf00
__declspec(naked) void PrepareMutexLock()
{
	__asm
	{
		CALL dword ptr[CallSrvFct]
		CALL PutMutexLock;
		jmp dword ptr[ReturnToMainLoop]
	}
}

__declspec(naked) void CloseAllThreads()
{
	__asm
	{
		CALL dword ptr[CallSrvFctClose]
		CALL PrepareCloseThreads
		jmp dword ptr[ReturnToClose]
	}
}

Patch _AddMutexStuff[] = 
{
	Patch(OFFS_MainLoopAction, (char*)"\xe9", (int)1),
	Patch(OFFS_MainLoopAction + 1, (relativefunc)PrepareMutexLock),

	Patch(OFFS_ClosePrgAction, (char*)"\xe9", (int)1),
	Patch(OFFS_ClosePrgAction + 1, (relativefunc)CloseAllThreads),

	Patch()
};

Patch* AddMutexStuff = _AddMutexStuff;


#define OFFS_AssignCommandValidPreparation 0x006b5311
#define OFFS_AssignCommandError            0x006b536a

void __fastcall RemoveAssignCommandHooks();




//hooked function
__declspec(naked) void CallRemoveAssignCommandHooks()
{
	__asm
	{
		CALL RemoveAssignCommandHooks
		mov		edi, OFFS_AssignCommandError
		jmp		edi
	}
}

//hooked function
int __fastcall SaveAssignCommandContext(uint32_t* pThis, void* Unused, uint32_t dateDelay, uint32_t timeDelay, uint32_t uiObject, uint32_t uiObject2, int iPAram, ActionVMScript* pActionObj, int iParam2, int iParm3)
{
	if (!bSaved) {
		pPointerTable = pThis;
		bSaved        = true;
	}
	currentHandlerId++;

	ContextData ctx;

	ctx.data = pActionObj;

	contexts[currentHandlerId]       = ctx;


	RemoveAssignCommandHooks();

	return 1;
}


Patch _CaptureCommandCtx[] = {
	Patch(OFFS_AssignCommandValidPreparation, (char*)"\xe8", (int)1), // CALL
	Patch(OFFS_AssignCommandValidPreparation + 1, (relativefunc)SaveAssignCommandContext),

	Patch(OFFS_AssignCommandError, (char*)"\xe9", (int)1), // JMP
	Patch(OFFS_AssignCommandError + 1, (relativefunc)CallRemoveAssignCommandHooks),

	Patch()
};

Patch* CaptureCommandCtx = _CaptureCommandCtx;


void __fastcall RemoveAssignCommandHooks() 
{
	int i = 0;
	while (CaptureCommandCtx[i].Remove()) {
		i++;
	}
}

extern "C" __declspec(dllexport) int __cdecl PrepareSaveContext() {
	//Can be done only from main thread
	if (!isMainThread)
		return 0;

	//Patch everything
	int i = 0;
	while (CaptureCommandCtx[i].Apply()) {
		i++;
	}

	return 1;
}


// Exports for External usages
extern "C" __declspec(dllexport) uint32_t __cdecl GetContextHandler() 
{
	// Can be done only from main thread
	if (!isMainThread)
		return 0;

	return currentHandlerId;
}


bool ThreadPatchFunction(SimpleIniConfig* config)
{
	int i = 0;
	int iTest = 0;

	config->Read("AllowThreading", &iTest, 0);
	if (iTest > 0) 
	{
		if (iTest > 10)
		{
			logger->Info("Threading System Max value: 10");
			iTest = 10;
		}
		logger->Info("Threading system activated with: %d", iTest);
		actionSemaphore = std::make_unique<std::counting_semaphore<10>>(iTest);
		g_sharedMutex.lock();
		isMainThread = true;
		g_UseThreadingSystem.store(true);
		i = 0;
		while (AddMutexStuff[i].Apply()) {
			i++;
		}
	}

	return true;
}

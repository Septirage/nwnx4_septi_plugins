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


#include <nwn2heap.h>

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
static bool willBePersistant     = true;



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
	uint8_t* unknowArray; // +0x10		// ?????? verifier ce qu'il peut contenir !
	uint32_t uSizeUknArray; // +0x14
	NWN::CExoString scriptName; // +0x18
	uint32_t field_20; // +0x20
	uint32_t field_24; // +0x24
};


struct ContextData {
	ActionVMScript* data; // ObjActionPtr
	bool isPersistent; // true = persistant, false = oneshotn
};

static std::unordered_map<uint32_t, ContextData> contexts;

/*
NWN2_MemoryPoolMgr stuff :
*/

__declspec(naked) uint8_t* __cdecl MemPoolMgrAllocateVMScript(uint32_t uSize)
{
	__asm
	{
		mov edx, 0x00730570
		jmp edx
	}
}

__declspec(naked) uint8_t* __cdecl MemPoolMgrAllocateVMStack(uint32_t uSize)
{
	__asm
	{
		mov edx, 0x00730500
		jmp edx
	}
}

void __declspec(naked) __cdecl MemPoolMgrFreeVMStack(ActionVMStack* pToFree)
{
	__asm
	{
		mov edx, 0x00730550
		jmp edx
	}
}

void __declspec(naked) __cdecl MemPoolMgrFreeVMScript(ActionVMScript* pToFree)
{
	__asm
	{
		mov edx, 0x007305c0
		jmp edx
	}
}



// Synchronisation mechanism
static std::mutex g_sharedMutex;
static std::unique_ptr<std::counting_semaphore<10>> actionSemaphore;
static std::atomic<bool> mainThreadWaiting {false};

//Shutting down mechanism
static std::atomic<bool> g_is_shutting_down {false};
static std::atomic<bool> g_UseThreadingSystem {false};


// Exports for External usages
extern "C" __declspec(dllexport) int BeginContextInsertion()
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

	g_sharedMutex.lock();
	if (g_is_shutting_down.load())
	{
		g_sharedMutex.unlock();
		actionSemaphore->release();
		return 0;
	}

	isThreadLocked = true;
	return 1;
}

//Will allocate and fill ListTypeParam and ListParam and all others data
__declspec(naked) int __fastcall CopyActionParamList(ActionVMStack* pDest, void* Unused, ActionVMStack* pOrigin, uint32_t iParam, uint32_t iParam2)
{
	__asm
	{
		mov edx, 0x00727a50
		jmp edx
	}
}


ActionVMScript* CopyAssignCommandContext(ActionVMScript* pOriginal)
{
	//Start will be close to ForPersist, we will just use MemPoolMgr Allocation function instead of new.

	ActionVMScript* pCopy = (ActionVMScript*)MemPoolMgrAllocateVMScript(0x28);
	memcpy(pCopy, pOriginal, 0x28);

	//Copy the CExoString with NWN2Heap Allocate memory
	NWN2_HeapMgr* pHeapMgr = NWN2_HeapMgr::Instance();
	NWN2_Heap* pHeap       = pHeapMgr->GetDefaultHeap();
	pCopy->scriptName.m_sString     = (char*)pHeap->Allocate(pCopy->scriptName.m_nBufferLength);

	memcpy(pCopy->scriptName.m_sString, pOriginal->scriptName.m_sString, pCopy->scriptName.m_nBufferLength);


	//Copy UnknowArray if needed
	if (pOriginal->uSizeUknArray > 0)
	{
		pCopy->unknowArray = (uint8_t*)pHeap->Allocate(pOriginal->uSizeUknArray);
		memcpy(pCopy->unknowArray, pOriginal->unknowArray, pOriginal->uSizeUknArray);
	}

	if (pOriginal->pParamObj) 
	{
		// Create a new ParamObj
		pCopy->pParamObj = (ActionVMStack*)MemPoolMgrAllocateVMStack(0x18);

		// Prepare quick access pointers
		ActionVMStack* pOrigParam = pOriginal->pParamObj;
		ActionVMStack* pCopyParam = pCopy->pParamObj;

		//Clean it because we will use the NWN2 CopyParam function
		std::memset(pCopyParam, 0, 0x18);

		//This is constant, keep it
		pCopyParam->ptrToVMScriptObj = pOrigParam->ptrToVMScriptObj;

		//Use basic params
		CopyActionParamList(pCopyParam, NULL, pOrigParam, 0, 0);
	}

	return pCopy;
}

ActionVMScript* CopyAndCleanAssignCommandCtxForPersist(ActionVMScript* pOriginal)
{
	ActionVMScript* pCopy = new ActionVMScript();
	memcpy(pCopy, pOriginal, 0x28);

	// CopyExoStringForPersistant(&pCopy->scriptName, &pOriginal->scriptName);

	if (pOriginal->pParamObj) {
		// Create a new ParamObj
		pCopy->pParamObj = new ActionVMStack();

		// Prepare quick access pointers
		ActionVMStack* pOrigParam = pOriginal->pParamObj;
		ActionVMStack* pCopyParam = pCopy->pParamObj;

		memcpy(pCopyParam, pOrigParam, 0x18);

		/* Need to reverse the complexe type before doing that
		if(pOrigParam->typeList)
		{
		pCopyParam->typeList = new uint8_t[pCopyParam->paramSize];
		memcpy(pCopyParam->typeList, pOrigParam->typeList, pOrigParam->paramSize);
		}

		if(pOrigParam->paramList)
		{
		pCopyParam->typeList = new uint32_t[pCopyParam->paramSize];

		for(uint32_t i = 0; i < pCopyParam->paramCount; i++)
		{

		}
		}
		*/

		//Free the MemoryPool for pOrigParam to give back some memory to NWN2MemoryPool
		MemPoolMgrFreeVMStack(pOrigParam);
	}

	// Free the MemoryPool for pOriginal to give back some memory to NWN2MemoryPool
	MemPoolMgrFreeVMScript(pOriginal);


	return pCopy;
}

__declspec(naked) int __fastcall PutContextInQueue(uint32_t* pThis, void* Unused, uint32_t dateDelay, uint32_t timeDelay, uint32_t uiObject, uint32_t uiObject2, int iPAram, ActionVMScript* pActionObj, int iParam2, int iParm3)
{
	__asm
	{
		mov edx, 0x0066ee60
		jmp edx
	}
}

extern "C" __declspec(dllexport) int InsertContext(uint32_t iHandler)
{
	// will garanty that we use threading system and are right context
	if (!isThreadLocked)
		return -1;

	if (contexts.count(iHandler) != 0)
	{
		ActionVMScript* pContext = contexts[iHandler].data;
		if (contexts[iHandler].isPersistent)
		{
			pContext = CopyAssignCommandContext(pContext);
		}
		//Grab the module objectID instead of presume it is 0 TODO
		PutContextInQueue(pPointerTable, NULL, 0, 0, GetModuleID_(), GetModuleID_(), 1, pContext, 10, 1);

		if (!contexts[iHandler].isPersistent)
			contexts.erase(iHandler);

		return 1;
	}
	return -2;
}

extern "C" __declspec(dllexport) int EndContextInsertion()
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

	if (willBePersistant)
		pActionObj       = CopyAndCleanAssignCommandCtxForPersist(pActionObj);
	ctx.data = pActionObj;
	ctx.isPersistent = willBePersistant;


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

// Exports for External usages
int PrepareSaveContext(int bPersistant)
{
	//Can be done only from main thread
	if (!isMainThread)
		return 0;

	willBePersistant = (bPersistant != 0);

	//Patch everything
	int i = 0;
	while (CaptureCommandCtx[i].Apply()) {
		i++;
	}

	return 1;
}

extern "C" __declspec(dllexport) int PrepareSaveContext() {
	return PrepareSaveContext(0);
}


// Exports for External usages
extern "C" __declspec(dllexport) uint32_t GetContextHandler() 
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

#include "EnhancedFeatures.h"
#include "EnhancedCommon.h"
#include "nwn2heap.h"
#include "../../NWN2Lib/NWN2.h"
#include "../../NWN2Lib/NWN2Common.h"
#include "..\..\misc\Patch.h"
#include <bit>
#include <cassert>
#include <charconv>
#include <detours/detours.h>
#include <optional>
#include <string_view>
#include <iostream>
#include <fstream>
#include "../../septutil/srvadmin.h"
#include "RuleParser.h"
#include "SpeedFeats.h"
#include "SkillHooks.h"
#include "WeaponFinesseHook.h"
#include "ReduceSpeedHook.h"
#include "SmallPatchFunctions.h"
#include "SmallHookFunctions.h"
#include "StoreRetrievePatch.h"

#include "MagicFunctions.h"

std::unique_ptr<LogNWNX> logger;


typedef void (__cdecl * NWN2Heap_Deallocate_Proc)(void *p);

NWN2Heap_Deallocate_Proc NWN2Heap_Deallocate;

namespace {


	// 1.23
	// g_pVirtualMachine
	constexpr uintptr_t NWN2_OFFSET_CVIRTUALMACHINE = 0x00864424;
	// CVirtualMachine::ExecuteScript
	constexpr uintptr_t NWN2_OFFSET_EXECUTESCRIPT = 0x0072B380;
	constexpr uintptr_t NWN2_OFFSET_EXECUTESCRIPT_ENH = 0x0072B050;


	constexpr uintptr_t NWN2_OFFSET_InitParam = 0x0055EA40;
	constexpr uintptr_t NWN2_OFFSET_CleanParam = 0x006b5cd0;

	struct CVirtualMachine
	{

	};

	using CVirtualMachine_ExecuteScript_t = BOOL(__thiscall*)(CVirtualMachine* thisVM,
		const NWN::CExoString& scriptName, NWN::OBJECTID objectId, uint32_t unknown1, uint32_t unknown2);
	CVirtualMachine_ExecuteScript_t CVirtualMachine_ExecuteScript =
		std::bit_cast<CVirtualMachine_ExecuteScript_t>(NWN2_OFFSET_EXECUTESCRIPT);

	using CVirtualMachine_ExecuteScriptEnhanced_t = BOOL(__thiscall*)(CVirtualMachine* thisVM,
		const NWN::CExoString& scriptName, NWN::OBJECTID objectID, void* ParamList, uint32_t unknow1, uint32_t unknow2);
	CVirtualMachine_ExecuteScriptEnhanced_t CVirtualMachine_ExecuteScriptEnhanced =
		std::bit_cast<CVirtualMachine_ExecuteScriptEnhanced_t>(NWN2_OFFSET_EXECUTESCRIPT_ENH);

	using CVirtualMachine_InitParam_t = void(__thiscall*)(void* paramLst, uint32_t iNb);
	using CVirtualMachine_CleanParam_t = void(__thiscall*)(void* paramLst);

	CVirtualMachine_InitParam_t CVirtualMachine_InitParam =
		std::_Bit_cast<CVirtualMachine_InitParam_t>(NWN2_OFFSET_InitParam);
	CVirtualMachine_CleanParam_t CVirtualMachine_CleanParam =
		std::_Bit_cast<CVirtualMachine_CleanParam_t>(NWN2_OFFSET_CleanParam);


	CVirtualMachine*
		GetNwn2VirtualMachine()
	{
		const auto vm = std::bit_cast<CVirtualMachine**>(NWN2_OFFSET_CVIRTUALMACHINE);
		return *vm;
	}



	//Entry @ for hook

	EnhancedFeatures* enhancedFeats;


	////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////// ItemPrprty Patch ////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
#define OFFS_ItemPrptLog	0x005C7F46

unsigned long ReturnItemPropertyFix = 0x005C7F4C;

	__declspec(naked) void FixItemPropertyLoad()
	{
		__asm
		{
			MOV		[EAX+0xB10], ECX

			PUSH	0x0
			PUSH	0x0
			PUSH	0x0

			JMP		dword ptr[ReturnItemPropertyFix]
		}
	}



	//Patch SetTag
	Patch _ItemPropertyLogPatch[] =
	{
		Patch((DWORD)OFFS_ItemPrptLog, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6),
		Patch(OFFS_ItemPrptLog + 1, (relativefunc)FixItemPropertyLoad),

		Patch()
	};

	Patch *ItemPropertyLogPatch = _ItemPropertyLogPatch;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////// SetTag Patch //////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////


#define OFFS_SetTagPatch 0x006abbd4
	//Patch SetTag
	Patch _SetTagPatch[] =
	{
		/*PatchContext(true, (char*)"\x8d\x54\x24\x14", */Patch((DWORD)OFFS_SetTagPatch, (char*)"\x8D\x54\x24\x1C", (int)4),

		Patch()
	};

	Patch *SetTagPatch = _SetTagPatch;
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////// End SetTag Patch ////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////


	void PutNWNExoString(NWN::CExoString* pointedExoStr, char* sValue)
	{
		if (pointedExoStr != NULL)
		{
			NWN::CExoString* OutStr = pointedExoStr;
			NWN::CExoString CpyStr;
			CpyStr.m_nBufferLength = OutStr->m_nBufferLength;
			CpyStr.m_sString = OutStr->m_sString;

			unsigned long length = strlen(sValue)+1;
			NWN2_HeapMgr *pHeapMgr = NWN2_HeapMgr::Instance();
			NWN2_Heap *pHeap = pHeapMgr->GetDefaultHeap();
			char* strWanted = (char*)pHeap->Allocate(length+1);
			sprintf_s(strWanted, length, sValue);
			OutStr->m_sString = strWanted;
			OutStr->m_nBufferLength = length;

			if (CpyStr.m_sString != NULL)
			{

				if (NWN2Heap_Deallocate)
					NWN2Heap_Deallocate( CpyStr.m_sString );
				//FreeNwn2Heap( OutStr->m_sString );
				CpyStr.m_sString = NULL;
			}
		}
	}

	void __fastcall SetNameAreaFixed(uint8_t *areaPtr, NWN::CExoString* exoStrPtr)
	{
		uint8_t** prepareExoStr = *(uint8_t***)(areaPtr + 0xBC0);
		NWN::CExoString* myCurrentExoStr = (NWN::CExoString*) ((*prepareExoStr)+0x4);

		PutNWNExoString(myCurrentExoStr, exoStrPtr->m_sString);
	}

	unsigned long ReturnAfterSetNameArea = 0x006a5d8d;
	#define OFFS_SetFirstNameAreaPatch 0x006a5d88
	
	__declspec (naked) void CallSetNameArea()
	{
		_asm
		{
			CALL	SetNameAreaFixed
			ADD		ESP, 0x4
			JMP		dword ptr[ReturnAfterSetNameArea]
		}
	}

	Patch _SetFirstNameAreaPatch[] =
	{
		Patch((DWORD)OFFS_SetFirstNameAreaPatch, (char*)"\xe9\x00\x00\x00\x00", (int)5), //JMP
		Patch(OFFS_SetFirstNameAreaPatch + 1, (relativefunc)CallSetNameArea),
		Patch()
	};

	Patch *SetFirstNameAreaPatch = _SetFirstNameAreaPatch;












	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////// Destroy Patch //////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Must destroy Area entry ptr
#define OFFS_EntryDestroyArea 0x00606dea
#define OFFS_EntryDestroyInventory 0x005d3b60

	unsigned long ReturnAfterDestroyArea = 0x00606DF1;
	unsigned long ReturnAfterDestroyInventory = 0x005d3b67;
	unsigned long DestroyArea = 0x0060fc30;
	unsigned long DestroyItem_int = 0x005d8500;

	unsigned long GetItemFromId_ = 0x0054a1a0;

#define OFFSET_UTILITY_PTR_OBJ  0x0086442C


	__declspec (naked) void DestroyInventory()
	{
		__asm
		{
			PUSH	ESI
			PUSH	EDI
			//			PUSH	EAX
			MOV		ESI, dword ptr [ECX + 0x8]
			MOV		EDI, dword ptr [ECX + 0xC]
			PUSH	ECX
			TestInventorySlot:
			CMP		EDI, 0x00
				JE EndOfDestroyInventory

				//Cst first param
				MOV		ECX, [OFFSET_UTILITY_PTR_OBJ]
				MOV		ECX, [ECX + 0x4]

				//EAX : param2, object id.
				MOV		EAX, dword ptr [ESI]
				PUSH	EAX

				CALL dword ptr[GetItemFromId_]

				TEST	EAX, EAX
				JZ AFTER_ITEM_SLOT_DELETE

				MOV		ECX, EAX
				PUSH	0x1

				CALL	dword ptr[DestroyItem_int]
				MOV		dword ptr[ESI], 0x7F000000

				AFTER_ITEM_SLOT_DELETE:
			ADD		ESI, 0x4
				SUB		EDI, 0x1

				JMP TestInventorySlot
				EndOfDestroyInventory:
			POP		ECX
				POP		EDI
				POP		ESI

				JMP		dword ptr[ReturnAfterDestroyInventory]
		}
	}

	__declspec(naked) int __fastcall DeleteObjectByPtr(void* pObject)
	{
		__asm
		{
			push	EDX
			mov		EDX, dword ptr[ECX]
			add		EDX, 0xC
			//mov		EDX, dword ptr[EDX]
			push	0x1

			CALL	dword ptr[EDX]
			pop		EDX
			ret
		}
	}

	void DeleteObjectById(int idObj)
	{
		NWN::OBJECTID      ObjectId;
		GameObject        *Object;
		GameObjectManager m_ObjectManager;

		ObjectId = (NWN::OBJECTID) idObj;

		if ((ObjectId & NWN::INVALIDOBJID) != 0)
			ObjectId &= ~(NWN::LISTTYPE_MASK);

		if ((Object = m_ObjectManager.GetGameObject(ObjectId)) != NULL)
		{
			//enhancedFeats->logger->Info("* On lance le delete !");
			DeleteObjectByPtr((void*)Object);
		}
	}

	void __fastcall RemoveAreaFromModuleList(char* ptrArea)
	{
		GameObject        *ModuleObject;
		GameObjectManager m_ObjectManager;
		NWN::OBJECTID      areaID = *(NWN::OBJECTID*)(ptrArea+0xA0);

		if ((ModuleObject = m_ObjectManager.GetGameObject(0)) != NULL)
		{
			//ObjectAsModule
			ModuleObject = (GameObject*)((int)ModuleObject - 0x30);

			//GetNumberOfArea and AreaList
			uint32_t iNumberOfArea = *(uint32_t*)((int)ModuleObject + 0x118);
			NWN::OBJECTID* pAreaIDList = *(NWN::OBJECTID**)((int)ModuleObject + 0x114);

			for (uint32_t i = 0; i < iNumberOfArea; i++)
			{
				//This is the area we want to remove
				if (pAreaIDList[i] == areaID)
				{
					//Reduce number of area
					iNumberOfArea--;
					*(uint32_t*)((int)ModuleObject + 0x118) = iNumberOfArea;

					//"Stack" area list
					for (uint32_t j = i; j < iNumberOfArea; j++)
					{
						pAreaIDList[j] = pAreaIDList[j + 1];
					}
					break;
				}
			}
		}
	}

	void __fastcall DeleteAllInArea(char* ptrArea)
	{
		NWN::OBJECTID      ObjectId;
		GameObject        *Object;
		GameObjectManager m_ObjectManager;


		int* ptrObjectList;
		ptrObjectList = *(int**)(ptrArea - 0xad0 + 0xa90);
		int	iNumberOfObject;
		iNumberOfObject = *(int*)(ptrArea - 0xad0 + 0xa98);

		int iNbNotDeleted = 0;
		for (int i = 0; i < iNumberOfObject; i++)
		{

			// The delete object will remove the objid from the list, so we
			// will just delete the first one in chain
			//Update : If we have a problem with the object, it will stay first of the list.
			//			So add an index.
			ObjectId = (NWN::OBJECTID) ptrObjectList[iNbNotDeleted];

			if ((ObjectId & NWN::INVALIDOBJID) != 0)
				ObjectId &= ~(NWN::LISTTYPE_MASK);

			if ((Object = m_ObjectManager.GetGameObject(ObjectId)) != NULL)
			{
				//TODO, if TURD, remove it from Module TURD List (or dont delete it?)
				if (*(uint32_t*)Object == 0x00806064)
				{
					iNbNotDeleted++;
				}
				else
				{
					DeleteObjectByPtr((void*)Object);
				}
			}
			else {
				iNbNotDeleted++;
			}
		}
	}



	__declspec (naked) void CallDestroyArea()
	{
		__asm
		{
			//Get back the area ptr
			pop edi
			//redo the stack
			push edi

			//set ptr as param1
			mov ecx, edi

			//Call delete all in area
			call    DeleteAllInArea


			//Remove AreaFrom the ModuleAreaList TODO
			mov ecx, edi
			call	RemoveAreaFromModuleList

			//param2 = 1
			push 0x1
			//set ptr as param1
			mov ecx, edi

			//CaLL FNCT !!
			call dword ptr[DestroyArea]



			//Finish the function as before the hook.
			pop edi
			pop esi
			pop ebp
			pop ebx
			add esp, 0x3C
			JMP dword ptr[ReturnAfterDestroyArea]
		}

	}


	Patch _DestroyPatches[] =
	{
		//Link the destroy area function.
		Patch((DWORD)OFFS_EntryDestroyArea, (char*)"\xe9\x00\x00\x00\x00\x90\x90", (int)7), //JMP /NOP /NOP
		Patch(OFFS_EntryDestroyArea + 1, (relativefunc)CallDestroyArea),

		//Fix memleak when creature destroyed (destroy inventory) 
		Patch((DWORD)OFFS_EntryDestroyInventory, (char*)"\xe9\x00\x00\x00\x00\x90\x90", (int)7), //JMP /NOP /NOP
		Patch(OFFS_EntryDestroyInventory + 1, (relativefunc)DestroyInventory),

		Patch()
	};
	Patch *destroyPatches = _DestroyPatches;


	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////// End Destroy Patch ////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////// CloneArea Patch /////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////

#define OFFS_CloneAreaFromCopy		0x00612868
#define OFFS_CloneAreaSaveParentID	0x0061294f

	unsigned long ReturnCloneAreaStep1 = 0x0061286f;
	unsigned long GoBackStartCloneArea = 0x006127d0;
	unsigned long ReturnCloneAreaStep2 = 0x00612955;
	uint32_t parentAreaID = 0xFFFFFFFF;


	__declspec(naked) void CloneAreaStart()
	{
		__asm
		{
			MOV		EAX, dword ptr[ESI + 0xB70]
			MOV		parentAreaID, EAX

			MOV		EAX, dword ptr[ESI + 0xBF4]
			MOV		EAX, dword ptr[EAX + 0x4]
			CMP		EAX, 0xFFFFFFFF
			JE		NoParent
			JMP		dword ptr[GoBackStartCloneArea]
NoParent:
			CMP     byte ptr [ESI + 0xa4d], 0x0
			JMP		dword ptr[ReturnCloneAreaStep1]
		}
	}


	__declspec(naked) void CloneAreaEnd()
	{
		__asm
		{
			PUSH	ECX
			MOV		EAX, dword ptr[ESI + 0xBF4]
			MOV		ECX, parentAreaID
			MOV		[EAX + 0x4], ECX
			POP		ECX
			MOV		EAX,dword ptr [ESI + 0xB70]

			JMP		dword ptr[ReturnCloneAreaStep2]

		}
	}


	Patch _CloneArea[] =
	{
		Patch((DWORD)OFFS_CloneAreaFromCopy, (char*)"\xe9\x00\x00\x00\x00\x90\x90", (int)7), //JMP, NOP NOP
		Patch((DWORD)OFFS_CloneAreaFromCopy + 1, (relativefunc)CloneAreaStart),
		
		Patch((DWORD)OFFS_CloneAreaSaveParentID, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6), //JMP, NOP
		Patch((DWORD)OFFS_CloneAreaSaveParentID + 1, (relativefunc)CloneAreaEnd),
		
		Patch()
	};
	Patch* cloneArea = _CloneArea;


	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////////////////////////////////////////// End CloneArea Patch ///////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	auto plugin = std::unique_ptr<EnhancedFeatures>();
} // namespace

DLLEXPORT Plugin*
GetPluginPointerV2()
{
	return plugin.get();
}

BOOL APIENTRY
DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		plugin = std::make_unique<EnhancedFeatures>();

		char szPath[MAX_PATH];
		GetModuleFileNameA(hModule, szPath, MAX_PATH);
		plugin->SetPluginFullPath(szPath);
	} else if (ul_reason_for_call == DLL_PROCESS_DETACH) {
		plugin.reset();
	}
	return TRUE;
}

EnhancedFeatures::EnhancedFeatures()
{
	description = "This plugin provides fix and enhanced features.";

	subClass = FunctionClass;
	version  = PLUGIN_VERSION;
}

EnhancedFeatures::~EnhancedFeatures(void) {}

LogNWNX *myLog;

DWORD WINAPI LancementTest(LPVOID lpParam)
{
	TstVersionPlg testPlugin(enhancedFeats->nwnxStringHome, enhancedFeats->GetVersion(), enhancedFeats->GetSubClass());
	testPlugin.TestVersionPlugin();
	return 0;
}

bool
EnhancedFeatures::Init(char* nwnxhome)
{
	nwnxStringHome = nwnxhome;
	m_sSpeedFeatFile = "";
	m_sSkillHookFile = "";

	/* Log file */
	std::string logfile(nwnxhome);
	logfile.append("\\");
	logfile.append(GetPluginFileName());
	logfile.append(".txt");

	/* Ini file */
	std::string inifile(nwnxhome);
	inifile.append("\\");
	inifile.append(GetPluginFileName());
	inifile.append(".ini");

	std::string header = "NWNX EnhancedFeatures Plugin v"+version+". \n"
		"(c) 2023 by Septirage\n"
		"visit us at http://septirage.com/nwn2/ \n"
		"visit nwnx project at http://www.nwnx.org\n";

	logger = std::make_unique<LogNWNX>(logfile);
	
	logger->Info(header.c_str());

	logger->Info("* reading inifile %s", inifile.c_str());

	config = new SimpleIniConfig(inifile);

	logger->Configure(config);


	HMODULE nwn2mm   = GetModuleHandleA("NWN2_MemoryMgr_amdxp.dll");

	if (nwn2mm)
		NWN2Heap_Deallocate = (NWN2Heap_Deallocate_Proc)GetProcAddress( nwn2mm, "?Deallocate@NWN2_Heap@@SAXPAX@Z" );

	if (!NWN2Heap_Deallocate)
		logger->Info(  "* WARNING: Failed to locate NWN2_Heap::Deallocate."  );



	//AreaTypeBitX
	{
		int iBit = 0;
		std::string sFieldAreaBit = "AreaTypeBit";

		for (int iBit = 0; iBit < 32; iBit++)
		{
			std::string sAreaBit = "";
			std::string sFieldX = sFieldAreaBit + std::to_string(iBit);

			config->Read(sFieldX, &sAreaBit, std::string(""));
			if (sAreaBit == "")
				break;

			uint32_t uMaskValue = (1 << iBit);
			RuleParser::m_areaTypeMap[sAreaBit] = uMaskValue;
		}
	}

	//SpeedFeats
	std::string sSpeedFeatFile = "";
	config->Read("SpeedFeatsFile", &sSpeedFeatFile, std::string(""));
	if(sSpeedFeatFile != "")
	{
		std::string nxhome(nwnxhome);
		m_sSpeedFeatFile = sSpeedFeatFile;
		initSpeedFeat(nxhome, sSpeedFeatFile);
	}

	//SkillHook
	std::string sSkillHookFile = "";
	config->Read("SkillFeatsFile", &sSkillHookFile, std::string(""));
	if(sSkillHookFile != "")
	{
		std::string nxhome(nwnxhome);
		m_sSkillHookFile = sSkillHookFile;
		initSkillHooks(nxhome, sSkillHookFile);
	}

	std::string sWeaponFinesseFile = "";
	config->Read("WeaponFinesseFile", &sWeaponFinesseFile, std::string(""));
	if (sWeaponFinesseFile != "")
	{
		std::string nxhome(nwnxhome);
		m_sWeaponFinesseFile = sWeaponFinesseFile;
		initWeaponFinesse(nxhome, sWeaponFinesseFile);
	}

	std::string sReduceSpeedFile = "";
	config->Read("ReduceSpeedFile", &sReduceSpeedFile, std::string(""));
	if (sReduceSpeedFile != "")
	{
		std::string nxhome(nwnxhome);
		m_sReduceSpeedFile = sReduceSpeedFile;
		initReduceSpeedHooks(nxhome, sReduceSpeedFile);
	}


	//Call Small Functions. (will be improved latter)
	SmallPatchFunctions(config);

	SmallHookFunctions(config);

	//CallStore/Retrieve 
	StoreRetrieveConfigAndApply(config);

	int iTest = 0;
	int i = 0;
	config->Read("PatchDestroy", &iTest, 0);
	if (iTest != 0)
	{
		logger->Info("* Patch DestroyObject");

		bool bCanIapp = true;
		if (bCanIapp)
		{
			i = 0;
			while (destroyPatches[i].Apply()) {
				i++;
			}
		}
	}

	iTest = 0;
	config->Read("FixItemPropertyLoad", &iTest, 1);
	if (iTest != 0)
	{
		//5c7f51
		i = 0;
		while (ItemPropertyLogPatch[i].Apply()) {
			i++;
		}
		logger->Info("* ItemProperty on PC Load fixed !");
	}


	config->Read("PatchSetTag", &iTest, 0);
	if (iTest != 0)
	{
		logger->Info("* PatchSetTag");

		bool bCanIapp = true;

		if (bCanIapp)
		{
			i = 0;
			while (SetTagPatch[i].Apply()) {
				i++;
			}

			i = 0;
			while (SetFirstNameAreaPatch[i].Apply()) {
				i++;
			}
		}
	}

	//Patch CloneArea
	config->Read("PatchCloneArea", &iTest, 0);
	if (iTest != 0)
	{
		logger->Info("* Patch CreateInstancedAreaFromSource");
		i = 0;
		while (cloneArea[i].Apply()) {
			i++;
		}
	}

	logger->Info("* Plugin initialized.");

	enhancedFeats = this;

	{
		DWORD dwThreadId;
		HANDLE hThread;
		hThread = CreateThread(nullptr, 0, LancementTest, nullptr, 0, &dwThreadId);
	}

	return true;
}


int
EnhancedFeatures::GetInt(char* sFunction, [[maybe_unused]] char* sParam1, int nParam2)
{
	std::string function{sFunction};
	std::string sMagicFunc = "Magic|";
	std::string logTxt =
		"EnhancedFeatures_GetInt(" + function + "," + sParam1 + "," + std::to_string(nParam2) + ")";

	logger->Trace(logTxt.c_str());
	/*
	if (function == "removefa" || function == "addtoa")
	{
		NWN::OBJECTID       ObjectId;
		GameObject        *Object;
		GameObject        *Area;
		GameObjectManager m_ObjectManager;

		ObjectId = (NWN::OBJECTID) nParam2;

		if ((ObjectId & NWN::INVALIDOBJID) != 0)
			ObjectId &= ~(NWN::LISTTYPE_MASK);

		if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
			return NULL;

		NWN::OBJECTID AreaId = *(NWN::OBJECTID*)((int)Object + 0x144);

		if ((AreaId & NWN::INVALIDOBJID) != 0)
			AreaId &= ~(NWN::LISTTYPE_MASK);

		if ((Area = m_ObjectManager.GetGameObject( AreaId )) == NULL)
			return NULL;

		void* AreaPtr = (void*)((int)Area - 0xad0);

		if (function == "removefa")
			RemoveFromArea(AreaPtr, NULL, (void*)Object);
		else
			AddToArea(AreaPtr, NULL, (void*)Object);
		return 0;
	}
	*/

	if (function.length() > sMagicFunc.length() && function.substr(0, sMagicFunc.length()) == "Magic|")
	{
		return GetIntMagicFunctions(function.substr(sMagicFunc.length()), sParam1, nParam2);
	}
	
	if(function == "SecretDelete")
		DeleteObjectById(nParam2);

	return 0;
}

void EnhancedFeatures::SetInt([[maybe_unused]] char* sFunction,
	[[maybe_unused]] char* sParam1,
	[[maybe_unused]] int nParam2,
	[[maybe_unused]] int nValue)
{
	std::string function{sFunction};
	std::string sMagicFunc = "Magic|";
	std::string logTxt =
		"EnhancedFeatures_SetInt(" + function + "," + sParam1 + "," + std::to_string(nParam2) + "," + std::to_string(nValue) + ")";


	logger->Trace(logTxt.c_str());

	if (function.length() > sMagicFunc.length() && function.substr(0, sMagicFunc.length()) == "Magic|")
	{
		return SetIntMagicFunctions(function.substr(sMagicFunc.length()), sParam1, nParam2, nValue);
	}
	else if (function == "ReloadSpeedFile" && m_sSpeedFeatFile != "")
	{
		desinitSpeedFeat();
		return initSpeedFeat(nwnxStringHome, m_sSpeedFeatFile);
	}
	else if (function == "ReloadSkillFile" && m_sSkillHookFile != "")
	{
		desinitSkillHooks();
		return initSkillHooks(nwnxStringHome, m_sSkillHookFile);
	}
	else if (function == "ReloadWpnFinesseFile" && m_sWeaponFinesseFile != "")
	{
		desinitWeaponFinesse();
		return initWeaponFinesse(nwnxStringHome, m_sWeaponFinesseFile);
	}
	else if (function == "ReloadReduceSpeedFile" && m_sReduceSpeedFile != "")
	{
		desinitReduceSpeedHooks();
		return initReduceSpeedHooks(nwnxStringHome, m_sReduceSpeedFile);
	}

	return;
}


void
EnhancedFeatures::SetFloat([[maybe_unused]] char* sFunction,
	[[maybe_unused]] char* sParam1,
	[[maybe_unused]] int nParam2,
	[[maybe_unused]] float fValue)
{
	std::string function{sFunction};
	std::string logTxt =
		"EnhancedFeatures_SetFloat(" + function + "," + sParam1 + "," + std::to_string(nParam2) + "," + std::to_string(fValue) + ")";

	logger->Trace(logTxt.c_str());
}

float
EnhancedFeatures::GetFloat([[maybe_unused]] char* sFunction,
	[[maybe_unused]] char* sParam1,
	[[maybe_unused]] int nParam2)
{
	return 1.0f;
}

char*
EnhancedFeatures::GetString([[maybe_unused]] char* sFunction,
	[[maybe_unused]] char* sParam1,
	[[maybe_unused]] int nParam2)
{
	std::string function{sFunction};
	std::string ret("");
	std::string logTxt = "EnhancedFeatures_GetString(" + function + "," + sParam1 + "," +
		std::to_string(nParam2) + ")";


	return (ret.data());
}


void
EnhancedFeatures::SetString([[maybe_unused]] char* sFunction,
	[[maybe_unused]] char* sParam1,
	[[maybe_unused]] int nParam2,
	[[maybe_unused]] char* sValue)
{
	std::string function{sFunction};
	std::string ret("");
	std::string logTxt = "EnhancedFeatures_SetString(" + function + "," + sParam1 + "," +
		std::to_string(nParam2) + "," + sValue + ")";

	return;
}


void
EnhancedFeatures::GetFunctionClass(char* fClass)
{
	static constexpr auto cls = std::string_view(FunctionClass);
	strncpy_s(fClass, 128, cls.data(), std::size(cls));
}




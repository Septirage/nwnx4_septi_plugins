#include "appListManagement.h"
#include "aspectManager.h"
#include "manageObject.h"
#include "manageArea.h"
#include "manageCreature.h"
#include "manageLight.h"
#include "manageItem.h"
#include "messageManagement.h"
#include "aspectManagerUtils.h"
#include "refreshObject.h"
#include "objectSpatial.h"
#include "functionFixes.h"

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
#include <stdio.h>
#include <locale>
#include "../../septutil/srvadmin.h"
#include "nwn2heap.h"
#include "../../septutil/NwN2DataPos.h"

typedef void (__cdecl * NWN2Heap_Deallocate_Proc)(void *p);

NWN2Heap_Deallocate_Proc NWN2Heap_Deallocate;

namespace {
	AspectManager* aManager;

	unsigned long ReturnFctOnEquip			= 0x00601f3d; //Return after OnEquipHook
	unsigned long ReturnFctOnUnEquip		= 0x00601F0D; //Return after OnUnequipHook
	unsigned long CallRemoveFromEquipedList	= 0x00601ee0; //Ptr to RemoveFromEquipedList Function 
	unsigned long ReturnCallUnEquip			= 0x006ca39f; //Return after UnequipHook
	unsigned long ReturnChangePtrVisual		= 0x0074fec8; //Return to originalCode after ChangePtrVisualHook



	unsigned long ReturnCreatureVisualPatch = 0x0074feb2; //Return to the end of the CreatureVisual Function
	unsigned long ContinueCreatureVisual = 0x0074fe00;	//Continue normally CreatureVisual after the hook
	unsigned long CallPrepareVisualArmor = 0x00776100;	
	unsigned long WholeVisualArmorFct = 0x0074fde0;

	int g_iPatchVisualCreaMode = 0;	//Used to detect the origin of PatchVisualCreature call


	//Used to set g_iPatchVisualCreaMode
	#define OFFS_PrepareVisualPatch1 0x005e04ef
	#define OFFS_PrepareVisualPatch2 0x005e056d
	//Used to return to NWN2 after setted g_iPatchVisualCreaMode
	unsigned long ReturnPrepareVisualPatch1 = 0x05e04f4;
	unsigned long ReturnPrepareVisualPatch2 = 0x005e0572;

	

	#define OFFS_BuildMsgOnEquip	0x00601F33
	#define OFFS_BuildMsgOnUnEquip	0x00601f05

	#define OFFS_CallUnEquip		0x006ca39a

	#define OFFS_CServerExoApp_CreateVisual 0x00776130 // Create Visual
	#define OFFS_CallCheckEquipmentMustShow 0x005e04b0	//

	#define OFFS_CreatureVisualPatch 0x0074fdfb 
	#define OFFS_ChangePtrVisual	 0x0074fec2


	//Get the corresponding armorPart for the given inventorySlot
	int inventorySlotToArmorPart(int inventorySlot)
	{
		switch (inventorySlot) {
			case INVENTORY_SLOT_HEAD: return ARMOR_MODEL_PART_HELM;
			case INVENTORY_SLOT_BOOTS: return ARMOR_MODEL_PART_BOOTS;
			case INVENTORY_SLOT_ARMS: return ARMOR_MODEL_PART_GLOVES;
			case INVENTORY_SLOT_CLOAK: return ARMOR_MODEL_PART_CLOAK;
			case INVENTORY_SLOT_BELT: return ARMOR_MODEL_PART_BELT;
			default: return -1; //Not a valid armorpart
		}
	}

	//Wrapper to call function that will check which equipment must be shown
	__declspec(naked) void __fastcall CallUpdateBaseVisual(__in uint8_t* CreaObject)
	{
		__asm
		{
			mov		edx, OFFS_CallCheckEquipmentMustShow; prepare the jump
			jmp		edx;	branch to target procedure 
		}
	}

	uint8_t* __fastcall UseGenericVisual(uint8_t* ObjPointer) {
		GameObject* Object;
		GameObject* CreaObject;
		GameObjectManager m_ObjectManager;

		//Is this the chest ?
		uint8_t objcType;

		try
		{
			uint32_t verifType = *(uint32_t*)(((char*)ObjPointer) - 8);
			//Try the "signature Function" of Items
			if (verifType == 0x802118)
			{
				objcType = *(uint8_t*)(((char*)ObjPointer) + 0x728 + 0xA4);
				if (objcType != NWN::OBJECT_TYPE_ITEM)
				{
					//Normally, don't happen
					return ObjPointer;
				}
				uint32_t idItem = *(uint32_t*)(((char*)ObjPointer) + 0x728 + 0xA0);
				if ((Object = m_ObjectManager.GetGameObject(idItem)) == NULL)
				{
					return ObjPointer;
				}

				uint32_t* AppItm = ((uint32_t*)(((char*)Object) - 0x728));

				//Just to be sure that we really manage the right object
				if ((uint8_t*)AppItm != ObjPointer)
				{
					return ObjPointer;
				}

				uint32_t objType = *(uint32_t*)(((char*)Object) - 0x730 + 0x4);

				//If not an armor, we are not updating, goodbye
				//TODO, allow other object than armor... ?) (objectType other than 16 on chest)
				if (objType != 16)
				{
					return ObjPointer;
				}
				//Get the player who have it equiped . If not equiped, we don't want to patch.
				uint32_t idCreature = *(uint32_t*)(((char*)Object) + 0x3E0);
				if ((CreaObject = m_ObjectManager.GetGameObject(idCreature)) == NULL)
				{
					return ObjPointer;
				}

				//So we have an up to date thing before 
				CallUpdateBaseVisual((uint8_t*)CreaObject);

				ObjPointer = (uint8_t*)CreaObject + 0x183C;
			}
		}
		catch (...) {
			return ObjPointer;
		}

		return ObjPointer;
	}


	int __fastcall FixArmorPartChgEquipSt(uint8_t* objEquipdListPtr, uint32_t idEquipedObject, uint32_t iEmplacementObj, uint8_t* CreaturePtr)
	{
		NWN::OBJECTID       ObjectId;
		GameObject* Object;
		NWN::CNWSItem* Item;
		GameObjectManager m_ObjectManager;

		int iArmorModelPart = inventorySlotToArmorPart(iEmplacementObj);
		if(iArmorModelPart == -1 && iEmplacementObj != 1)
		{
			return iEmplacementObj;
		}

		if (iArmorModelPart != -1)
		{
			//Honestly... with current configuration we can just test if iArmorModelPart == BELT and beltFix is true
			//Test if we want to fix items for this ArmorModelPart
			if (aManager->m_FixPart[iArmorModelPart].size() == 0)
			{
				return iEmplacementObj;
			}
		}


		if ((Object = m_ObjectManager.GetGameObject(idEquipedObject)) == NULL)
		{
			//Normally, don't happen
			return iEmplacementObj;
		}

		int iItemType = *(((uint8_t*)Object) - 0x730 + 0x4);
		Item = Object->AsItem();

		if (iArmorModelPart != -1)
		{
			//Honestly... with current configuration only belt (and classical belts) are managed...
			//Test if we want to fix items for this ArmorModelPart and baseItem
			if (std::find(aManager->m_FixPart[iArmorModelPart].begin(), aManager->m_FixPart[iArmorModelPart].end(), iItemType) == aManager->m_FixPart[iArmorModelPart].end()) {
				return iEmplacementObj;
			}
		}


		uint32_t ObjectCreaId = *(uint32_t*)(((char*)CreaturePtr) + 0xA0);

		if (ObjectCreaId == 0 || ObjectCreaId == NWN::INVALIDOBJID)
			return iEmplacementObj;


		//The creature have the "NeverShowArmor" checked. Don't show anything
		uint8_t iNeverShow = *(((uint8_t*)CreaturePtr) + AmCrtNeverShowArmor);
		iNeverShow &= 0x1;
		if (iNeverShow != 0) {
			return iEmplacementObj;
		}

		//Extra verification
		ObjectCreaId &= ~(NWN::LISTTYPE_MASK);
		if ((Object = m_ObjectManager.GetGameObject(ObjectCreaId)) == NULL)
		{
			return iEmplacementObj;
		}

		//really check that the object is equiped
		uint32_t* ObjEquipdPtrCalc = *(uint32_t**)((((char*)Object) + AmCrtCSEquipedObjectTable));
		if (((char*)ObjEquipdPtrCalc) != ((char*)objEquipdListPtr))
		{
			return iEmplacementObj;
		}

		//This creature must be refresh
		refreshForEveryConcerned(ObjectCreaId);
		return iEmplacementObj;
	}

	

	__declspec(naked) void ChangePtrVisual()
	{
		__asm
		{
			call	UseGenericVisual
			MOV		ECX, EAX
			MOV		EDI, [ESP+0x0C]
			MOV		ESI, ECX
			jmp dword ptr[ReturnChangePtrVisual]
		}
	}

	__declspec(naked) void PrepareCreaturePtr()
	{
		//EAX = Equiped Object Slot
		//EDX = ID Item
		//ECX = Pointer to list of equiped objects
		__asm
		{
			push esi
			mov esi, ebx
			push edi
			call dword ptr[CallRemoveFromEquipedList]
			pop esi
			jmp dword ptr[ReturnCallUnEquip]
		}
	}



	__declspec(naked) void FixArmorPartOnEquip()
	{
		//EAX = Equiped Object Slot
		//EDX = Pointer to Equiped Item, then Item ID
		//ECX = Pointer to list of equiped objects
		__asm
		{
			mov edx, dword ptr ds : [edx + 7D0h]			//Get the ID of Equiped item
			mov dword ptr ds : [ecx + eax * 4 + 4] , edx	//Put this item in the equiped list
			push esi										//Creature ptr
			push eax										//Item Slot
			call FixArmorPartChgEquipSt						//Will refresh if needed
			jmp dword ptr[ReturnFctOnEquip]
		}
	}

	__declspec(naked) void FixArmorPartOnUnEquip()
	{
		//EAX = Equiped object Slot
		//EDX = ID Item
		//ECX = Pointer to list of equiped objects
		__asm
		{
			pop esi
			mov edx, dword ptr ds:[ecx+eax*4+4]				//Get the ID of Unequiped Item
			mov dword ptr ds:[ecx+eax*4+4],7F000000h		//Remove this item from the equiped list
			push esi										//creature ptr
			push eax										//Item slot
			call FixArmorPartChgEquipSt						//Will refresh if needed
			push esi
			jmp dword ptr[ReturnFctOnUnEquip]
		}
	}

	void ArmorPartVisual(uint8_t* Destination, uint8_t* Source)
	{
		*(Destination + 4) = *(Source + 4);
		*(Destination + 5) = *(Source + 5);


		memcpy_s(Destination + 8, 0x30, Source + 8, 0x30);
	}


	int __fastcall PatchVisualCrea(uint8_t* ObjDest, void* _, uint8_t* ObjSource)
	{
		if (g_iPatchVisualCreaMode == 0)
			return 1;

		
		GameObjectManager m_ObjectManager;
		bool isItem = (g_iPatchVisualCreaMode == 0x2);
		g_iPatchVisualCreaMode = 0;
		ObjDest = ObjDest - 0x5B8;
		ObjSource = ObjSource - 0x5B8;

		if (!isItem) {
			uint8_t iNeverShow = *(ObjDest - 0x183c + AmCrtNeverShowArmor);
			iNeverShow = iNeverShow & 0x1;
			if (iNeverShow != 0)
			{
				return 1;
			}
		}

		uint8_t maskArmorPart = ObjSource[0x6D0];
		//Head
		if (!isItem || maskArmorPart & 0x1)
			ArmorPartVisual(ObjDest + 0x5B8, ObjSource + 0x5B8);
		//Gloves
		if (!isItem || maskArmorPart & 0x2)
			ArmorPartVisual(ObjDest + 0x5F0, ObjSource + 0x5F0);
		//Boots
		if (!isItem || maskArmorPart & 0x4)
			ArmorPartVisual(ObjDest + 0x628, ObjSource + 0x628);
		//Belt
		if (!isItem || maskArmorPart & 0x8)
			ArmorPartVisual(ObjDest + 0x660, ObjSource + 0x660);
		//Cloak
		if (!isItem || maskArmorPart & 0x10)
			ArmorPartVisual(ObjDest + 0x698, ObjSource + 0x698);

		bool hasBelt = false;
		//Belt fix !
		{
			uint32_t* objEquiped = *((uint32_t**)(ObjDest - 0x183c + AmCrtCSEquipedObjectTable));
			NWN::OBJECTID beltEq = (NWN::OBJECTID)objEquiped[11];

			//We have belt, change the visual
			if (beltEq != (NWN::OBJECTID)0x7F000000)
			{
				hasBelt = true;

				GameObject* ObjBeltEqp;
				NWN::CNWSItem* Item;

				if ((beltEq & NWN::INVALIDOBJID) != 0)
					beltEq &= ~(NWN::LISTTYPE_MASK);

				if ((ObjBeltEqp = m_ObjectManager.GetGameObject(beltEq)) == NULL)
					return 1;

				if (ObjBeltEqp->GetObjectType() != NWN::OBJECT_TYPE::OBJECT_TYPE_ITEM)
					return 1;

				Item = ObjBeltEqp->AsItem();



				uint8_t falseBloc[56] = { 0 };
				falseBloc[0] = 0x4C;
				falseBloc[1] = 0x2A;
				falseBloc[2] = 0x81;

				falseBloc[4] = ((uint8_t*)Item)[0xC];
				falseBloc[5] = ((uint8_t*)Item)[0xD];


				memcpy_s((void*)(falseBloc + 8), 48, (void*)((uint8_t*)Item + 0x6DC), 0x30);
				ArmorPartVisual(ObjDest + 0x660, (uint8_t*)falseBloc);
			}
		}


		//Set the armorPart Mask
		uint8_t* tempPtr = *(uint8_t**)(ObjDest - 0x183c + AmCrtPtrAppBlock);
		uint8_t myArmorPartMask = *(tempPtr + 0x6F4 + 0x6D0);

		myArmorPartMask = myArmorPartMask & 0x1F;

		if (isItem) {
			myArmorPartMask |= maskArmorPart;
			myArmorPartMask = myArmorPartMask & 0x1F;
		}

		if (hasBelt) {
			myArmorPartMask |= 0x8;
		}


		ObjDest[0x6D0] = myArmorPartMask;


		return 0;
	}

	__declspec(naked) void CreatureVisualPatch()
	{
		__asm
		{
			PUSH	ECX
			PUSH	EDX
			PUSH	EAX
			call	PatchVisualCrea

			cmp		EAX, 0
			POP		EDX
			POP		ECX
			je		EndCreatureVisualPatch

			call	dword ptr[CallPrepareVisualArmor]
			jmp		dword ptr[ContinueCreatureVisual]

EndCreatureVisualPatch:
			POP		EAX
			POP		EDI
			MOV		EAX,ESI
			POP		ESI
			jmp dword ptr[ReturnCreatureVisualPatch]
		}
	}


	//Set g_iPatchVisualCreaMode to "CREATURE"
	__declspec(naked) void PrepareCreatureVisualPatch1()
	{
		__asm
		{
			MOV g_iPatchVisualCreaMode, 0x1
			call	dword ptr[WholeVisualArmorFct]
			jmp		dword ptr[ReturnPrepareVisualPatch1]
		}
	}

	//Set g_iPatchVisualCreaMode to "ITEM"
	__declspec(naked) void PrepareCreatureVisualPatch2()
	{
		__asm
		{
			MOV g_iPatchVisualCreaMode, 0x2
			call	dword ptr[WholeVisualArmorFct]
			jmp		dword ptr[ReturnPrepareVisualPatch2]
		}
	}



	Patch _itemsPatches[] =
	{
		//Patch to equip/Unequip and make refresh for armor

		//Detect the OnEquip action to see if we must refresh the creature
		Patch((DWORD)OFFS_BuildMsgOnEquip, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6), //JMP NOP...
		Patch(OFFS_BuildMsgOnEquip + 1, (relativefunc)FixArmorPartOnEquip),

		//Detect the OnUnEquip action to see if we must refresh the creature
		Patch((DWORD)OFFS_BuildMsgOnUnEquip, (char*)"\xe9\x00\x00\x00\x00\x90\x90\x90", (int)8), //JMP NOP NOP..
		Patch(OFFS_BuildMsgOnUnEquip + 1, (relativefunc)FixArmorPartOnUnEquip),

		//Prepare Creature to be ready for possible refresh
		Patch((DWORD)OFFS_CallUnEquip, (char*)"\xe9\x00\x00\x00\x00", (int)5), //JMP
		Patch(OFFS_CallUnEquip + 1, (relativefunc)PrepareCreaturePtr),
		

		//Detect that we want to patch the creature visual in "Creature" mode
		Patch((DWORD)OFFS_PrepareVisualPatch1, (char*)"\xe9\x00\x00\x00\x00", (int)5), //JMP
		Patch((DWORD)OFFS_PrepareVisualPatch1+1, (relativefunc)PrepareCreatureVisualPatch1),

		//Detect that we want to patch the creature visual in "Item" mode
		Patch((DWORD)OFFS_PrepareVisualPatch2, (char*)"\xe9\x00\x00\x00\x00", (int)5), //JMP
		Patch((DWORD)OFFS_PrepareVisualPatch2+1, (relativefunc)PrepareCreatureVisualPatch2),

		//Keep the CurrentVisual in a good state. Taking in account belt AND "NA" armor part
		Patch((DWORD)OFFS_CreatureVisualPatch, (char*)"\xe9\x00\x00\x00\x00", (int)5), //JMP
		Patch(OFFS_CreatureVisualPatch+1, (relativefunc)CreatureVisualPatch),

		//As we have updated the "CurrentVisual", use it.
		Patch((DWORD)OFFS_ChangePtrVisual, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6), //JMP //NOP
		Patch(OFFS_ChangePtrVisual+1, (relativefunc)ChangePtrVisual),

		Patch()
	};

	Patch *itemPatches = _itemsPatches;

	auto plugin = std::unique_ptr<AspectManager>();
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
		plugin = std::make_unique<AspectManager>();

		char szPath[MAX_PATH];
		GetModuleFileNameA(hModule, szPath, MAX_PATH);
		plugin->SetPluginFullPath(szPath);
	} else if (ul_reason_for_call == DLL_PROCESS_DETACH) {
		plugin.reset();
	}
	return TRUE;
}

//TODO improve that
int RemoveFirstClass(int nParam2)
{

	char* retValue;
	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;

	static unsigned char myClass[1168];
	memset(myClass, 0, sizeof(myClass));

	ObjectId = (NWN::OBJECTID) nParam2;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return 0;

	if (Object->GetObjectType() != NWN::OBJECT_TYPE::OBJECT_TYPE_CREATURE)
		return 0;

	char* ptrAppBlock = creatureApparenceBlock((char*)Object);
	char* ptrClass = ptrAppBlock + 0x10C;
	uint8_t NbClass = *(uint8_t*)(ptrAppBlock + 0x14);
	if (NbClass <= 1)
	{
		return 0;
	}
	int iSizeClassKeep = 0x124 * (NbClass - 1);
	std::memcpy ( myClass, ptrClass+0x124, iSizeClassKeep);

	std::memcpy(myClass + iSizeClassKeep, ptrClass, 0x124);

	myClass[iSizeClassKeep + 4] = 0xFF;
	myClass[iSizeClassKeep + 5] = 0;

	*(uint8_t*)(ptrAppBlock + 0x14) = NbClass - 1;

	std::memcpy ( ptrClass, myClass, iSizeClassKeep+0x124);

	char* listPtr = *(char**)(ptrAppBlock + 0x6C);
	uint32_t iLvlKeep = *(uint32_t*)(ptrAppBlock + 0x70);

	//Should never happens
	if (NbClass <= 1)
		return 1;

	iLvlKeep--;
	*(uint32_t*)(ptrAppBlock + 0x70) = iLvlKeep;
	//Keep the base ptr here.
	void* baseLvl = *(void**)listPtr;



	memmove(listPtr, listPtr+4, iLvlKeep*4);

	if (NWN2Heap_Deallocate)
		NWN2Heap_Deallocate( baseLvl );



	return 1;
}


AspectManager::AspectManager()
{
	description = "This plugin provides various script access to manage and modify appareance (item/character/placeable/area/...).";

	subClass = FunctionClass;
	static std::string stest(PLUGIN_VERSION);
	version  = stest;
}

AspectManager::~AspectManager(void)
{
	logger->Info("* Plugin unloaded.");
}

void
AspectManager::GetFunctionClass(char* fClass)
{
	static constexpr auto cls = std::string_view(FunctionClass);
	strncpy_s(fClass, 128, cls.data(), std::size(cls));
}

bool
AspectManager::Init(char* nwnxhome)
{
	/* Log file */
	nwnxStringHome = nwnxhome;
	std::string logfile(nwnxhome);
	logfile.append("\\");
	logfile.append(GetPluginFileName());
	logfile.append(".txt");

	/* Ini file */

	std::string inifile(nwnxhome);
	inifile.append("\\");
	inifile.append(GetPluginFileName());
	inifile.append(".ini");

	std::string header = "NWNX AspectManager Plugin v" + version + "\n"
		"(c) 2025 by Septirage\n"
		"visit us at http://septirage.com/nwn2/ \n"
		"visit nwnx project at http://www.nwnx.org\n";
	logger             = new LogNWNX(logfile);
	logger->Info(header.c_str());

	logger->Trace("* reading inifile %s", inifile.c_str());

	config = new SimpleIniConfig(inifile);

	logger->Configure(config);

	HMODULE nwn2mm   = GetModuleHandleA("NWN2_MemoryMgr_amdxp.dll");

	if (nwn2mm)
		NWN2Heap_Deallocate = (NWN2Heap_Deallocate_Proc)GetProcAddress( nwn2mm, "?Deallocate@NWN2_Heap@@SAXPAX@Z" );

	if (!NWN2Heap_Deallocate)
		logger->Info(  "* WARNING: Failed to locate NWN2_Heap::Deallocate."  );

	setLoggerMsg(logger);

	bool bFixBelt = false;
	int iFixBelt = 0;
	config->Read("FixBelt", &iFixBelt, 0);
	bFixBelt = (iFixBelt != 0);

	if (bFixBelt)
	{
		logger->Info(  "* Start to apply patch for visual items..."  );
		int i = 0;
		while(itemPatches[i].Apply()) {
			i++;
		}
		logger->Info(  "* ...Done."  );


		m_FixPart[3].push_back(21);
	}
	

	//QuickPatches
	int iQuickPatch = 1;
	config->Read("PatchDoorScale", &iQuickPatch, 1);

	if(iQuickPatch != 0)
	{
		logger->Info(  "* Start to apply quickpatches..."  );
		initAspectManagerFctFixes();
		logger->Info(  "* ...Done."  );
	}


	int iUseListSyst = 1;
	config->Read("UseListSystem", &iUseListSyst, 1);
	bUseListSyst = (iUseListSyst != 0);

	if(bUseListSyst)
	{
		int iScanFile = 0;
		config->Read("ScanFiles", &iScanFile, 1);
		if (iScanFile == 2)
		{
			std::string newPath(nwnxhome);
			newPath.append("\\");
			newPath.append("AspectManagerScan");
			if (std::filesystem::exists(newPath))
			{
				iScanFile = 1;
				std::remove(newPath.c_str());
			}
			else
			{
				iScanFile = 0;
			}
		}
		bScanFile = (iScanFile==1);

		config->Read("PathToListFile", &pathListFile, std::string("listeHakZip.ini"));

		std::string newPath(nwnxhome);
		newPath.append("\\");
		newPath.append(pathListFile);

		pathListFile = newPath;

		config->Read("PathToSaveFile", &pathFileSave, std::string("saveFileAspectManager.txt"));

		newPath = nwnxhome;
		newPath.append("\\");
		newPath.append(pathFileSave);

		pathFileSave = newPath;
		bool fileSaveExist = false;
		{
			std::ifstream ifile;
			ifile.open(pathFileSave);
			if(ifile) {
				fileSaveExist = true;
			} else {
				fileSaveExist = false;
			}
		}

		int iNumberAV;
		std::string sDefaultName = "";
		bool bDefaultName = config->Read("defaultName", &sDefaultName);
		config->Read("ArmorVisualMaxNumber", &iNumberAV, 10);
		int iCour = 0;
		for (iCour = 0; iCour <= iNumberAV; iCour++)
		{
			std::string sKey = std::to_string(iCour);
			std::string sPref = "";

			if (config->Read(sKey, &sPref))
			{
				std::vector<std::string> portion;
				tokenize(sPref, "@", portion);
				if (portion.size() > 1)
				{
					sPref = portion[0];
					prefixToVisual[sPref] = portion[1];
				}
				else
				{
					prefixToVisual[sPref] = sPref;
				}
				if (bDefaultName && fileSaveExist)
				{
					prefixToVisual[sPref] = sDefaultName;
				}
				prefixToIndex[sPref] = iCour;
				indexToPrefix[iCour] = sPref;
			}
		}

		//Create the prefixRegexMask
		{
			prefixRegexMask = "";

			for (auto kv : prefixToVisual) {
				prefixRegexMask += kv.first;
				prefixRegexMask += "|";
			}

			//We have some
			if (prefixRegexMask != "") {
				// remove last |
				prefixRegexMask.pop_back();
			}
		}



		nwnxhome_ = nwnxhome;
		Initialisation();
	}
	logger->Info("* Plugin initialized.");
	aManager = this;

	DWORD dwThreadId;
	HANDLE hThread;
	hThread = CreateThread(nullptr, 0, LaunchTestVersion, nullptr, 0, &dwThreadId);



	return true;
}


int AspectManager::GetInt(char* sFunction, [[maybe_unused]] char* sParam1, int nParam2) {
	std::string stFunction(sFunction);
	int iRet = 0;
	logger->Trace("* Plugin GetInt(%s, %s, %d)", sFunction, sParam1, nParam2);
	if(stFunction.rfind(S_LIST_SYSTEM, 0) == 0) {
		if (!bUseListSyst) {
			return 0;
		}
		iRet = ListSystemGetInt(stFunction.substr(S_LIST_SYSTEM.size()), sParam1, nParam2);
	}
	else if(stFunction == "area") {
		iRet = AreaGetInt(sParam1, nParam2);
	}
	else if(stFunction == "creature") {
		iRet = CreatureGetInt(sParam1, nParam2);
	}
	else if (stFunction == "light") {
		iRet = LightGetInt(sParam1, nParam2);
	}
	else if (stFunction == "item") {
		iRet = ItemGetInt(sParam1, nParam2);
	}
	else if (stFunction == "RemoveFirstClass") {
		iRet = RemoveFirstClass(nParam2);
	}
	else if (stFunction == "IsBaseFaction") {
		/*
		We going to get factions pointer...

		*/
		void* p1 = *(void**)OFFS_g_pAppManager;
		p1 = *(void**)(((char*)p1) + 4);
		p1 = *(void**)(((char*)p1) + 4);

		void* FactionPtr = *(void**)(((char*)p1) + 0x1007C);
		if (FactionPtr != NULL)
		{
			if ((0 < nParam2) && (nParam2 <= *(int*)((int)FactionPtr + 0x14))) {
				return 1;
			}
		}
		return 0;
	}
	else if (stFunction == "placeable")
	{
		std::string sCommand(sParam1);
		if (sCommand == "DynamicCl") {
			NWN::OBJECTID       ObjectId;
			GameObject* Object;
			GameObjectManager m_ObjectManager;

			ObjectId = (NWN::OBJECTID) nParam2;

			if ((ObjectId & NWN::INVALIDOBJID) != 0)
				ObjectId &= ~(NWN::LISTTYPE_MASK);

			if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
				return 0;
			if (Object->GetObjectType() != NWN::OBJECT_TYPE::OBJECT_TYPE_PLACEABLE)
				return 0;

			iRet = *(uint32_t*)(((char*)Object) + AmPlcDynamicCol);
		}
	}
	else {
		iRet = ObjectGetInt(sFunction, nParam2);
	}

	logger->Trace("Return Value : %d", iRet);
	return iRet;
}

void
AspectManager::SetInt([[maybe_unused]] char* sFunction,
	[[maybe_unused]] char* sParam1,
	[[maybe_unused]] int nParam2,
	[[maybe_unused]] int nValue)
{
	logger->Trace("* Plugin SetInt(%s, %s, %d, %d)", sFunction, sParam1, nParam2, nValue);
	std::string stFunction(sFunction);
	std::string sCommand(sParam1);
	if (stFunction == "refreshNoArmor") {
		refreshNude_Verif(nParam2, nValue);
	}
	else if (stFunction == "refreshBelt") {
		refreshBelt(nParam2, nValue);
	}
	else if (stFunction == "refreshObject") {
		refreshObject(nParam2, nValue);
	}
	else if (stFunction == "maskObject") {
		refreshObject(nParam2, nValue, 1);
	}
	else if (stFunction == "showObject") {
		refreshObject(nParam2, nValue, 2);
	}
	else if (stFunction.rfind("refreshView", 0) == 0) {
		std::string Valeur = stFunction.substr(11);
		int iInventorySlot = strtol(Valeur.c_str(), NULL, 10);
		if(iInventorySlot >=  INVENTORY_SLOT_HEAD && iInventorySlot <= INVENTORY_SLOT_CLOAK)
			refreshItemApp(iInventorySlot, nParam2, strtol(sParam1, NULL, 10), nValue);
	}
	else if (stFunction == "area")
		AreaSetInt(sParam1, nParam2, nValue);
	else if (stFunction == "creature")
		CreatureSetInt(sParam1, nParam2, nValue);
	else if (stFunction == "item")
		ItemSetInt(sParam1, nParam2, nValue);
	else if (stFunction == "light")
		LightSetInt(sParam1, nParam2, nValue);
	else if (stFunction == "placeable")
	{
		if (sCommand == "DynamicCl")
		{
			SetDynamicColision(nParam2, nValue);
		}
	}
	else if(stFunction == "musicMessage") {
		int iParameters;
		std::istringstream(sCommand) >> iParameters;
		sendMusicMessage(iParameters / 10, iParameters % 10, nValue, nParam2);
	}
	else if(stFunction.rfind(S_LIST_SYSTEM, 0) == 0) {
		if (!bUseListSyst) {
			return;
		}
		return ListSystemSetInt(stFunction.substr(S_LIST_SYSTEM.size()), sParam1, nParam2, nValue);
	}
	else
		ObjectSetInt(sFunction, nParam2, nValue);
}


float
AspectManager::GetFloat([[maybe_unused]] char* sFunction,
	[[maybe_unused]] char* sParam1,
	[[maybe_unused]] int nParam2)
{
	float fRet = 0;
	logger->Trace("* Plugin GetFloat(%s, %s, %d)", sFunction, sParam1, nParam2);
	std::string stFunction(sFunction);
	if (stFunction == "area")
		fRet= AreaGetFloat(sParam1, nParam2);
	else if (stFunction == "creature")
		fRet = CreatureGetFloat(sParam1, nParam2);
	else if (stFunction == "item")
		fRet = ItemGetFloat(sParam1, nParam2);
	else if (stFunction == "light")
		fRet= LightGetFloat(sParam1, nParam2);
	else
		fRet = ObjectGetFloat(sFunction, nParam2);

	logger->Trace("RetVal : %f", fRet);

	return fRet;
}

void
AspectManager::SetFloat([[maybe_unused]] char* sFunction,
	[[maybe_unused]] char* sParam1,
	[[maybe_unused]] int nParam2,
	[[maybe_unused]] float fValue)
{
	logger->Trace("* Plugin SetFloat(%s, %s, %d, %f)", sFunction, sParam1, nParam2, fValue);
	std::string stFunction(sFunction);
	std::string emtpy = "";
	sprintf_s(returnBuffer, MAX_BUFFER, emtpy.c_str());

	if (stFunction == "SetFacing")
		ObjectSetOrientation(nParam2, fValue);
	else if (stFunction == "area")
		AreaSetFloat(sParam1, nParam2, fValue);
	else if (stFunction == "creature")
		CreatureSetFloat(sParam1, nParam2, fValue);
	else if (stFunction == "item")
		ItemSetFloat(sParam1, nParam2, fValue);
	else if (stFunction == "light")
		LightSetFloat(sParam1, nParam2, fValue);
	else
		ObjectSetFloat(sFunction, nParam2, fValue);

}

bool parseGeometry(std::string sInputGeometry, std::vector<float>& vGeometry) {
	std::istringstream iss(sInputGeometry);
	int n;
    if (!(iss >> n)) {
		return false;
	}

	vGeometry.resize(n * 3);

	for (int i = 0; i < n * 3; i++) {
        if (!(iss >> vGeometry[i])) {
            return false;
        }
    }

	return true;
}


void AspectManager::SetString([[maybe_unused]] char* sFunction,
	[[maybe_unused]] char* sParam1,
	[[maybe_unused]] int nParam2,
	[[maybe_unused]] char* sValue)
{
	logger->Trace("* Plugin SetString(%s, %s, %d, %s)", sFunction, sParam1, nParam2, sValue);
	std::string stFunction(sFunction);

	if (stFunction == "SetPositionAndFacing") {
		char* szOrbits = sValue;
		char* pEnd;
		char* pEnd2;
		float fX, fY, fZ, fFacing;
		fX = strtof(szOrbits, &pEnd);
		fY = strtof(pEnd, &pEnd2);
		fZ = strtof(pEnd2, &pEnd);
		fFacing = strtof(pEnd, NULL);
		ObjectSetPositionAndOrientation(nParam2, fX, fY, fZ, fFacing);
	}
	else if (stFunction == "SetPosition") {
		char* szOrbits = sValue;
		char* pEnd;
		char* pEnd2;
		float fX, fY, fZ;
		fX = strtof(szOrbits, &pEnd);
		fY = strtof(pEnd, &pEnd2);
		fZ = strtof(pEnd2, NULL);
		ObjectSetPosition(nParam2, fX, fY, fZ);
	}
	else if (stFunction == "Trigger") {
		std::string sTrigFunc(sParam1);
		if (sTrigFunc == "Geometry") {
			std::string sInputGeometry(sValue);
			std::vector<float> vGeometry;
			if (parseGeometry(sInputGeometry, vGeometry)) {
				TriggerChangeGeometry(nParam2, vGeometry);
			} else {
				//Error
				logger->Err("Error when parsing the new Trigger Geometry");
			}
		}
	}
	else if (stFunction == "area")
		AreaSetString(sParam1, nParam2, sValue);
	else if (stFunction == "creature")
		CreatureSetString(sParam1, nParam2, sValue);
	else if (stFunction == "item")
		ItemSetString(sParam1, nParam2, sValue);
	else if (stFunction == "light")
		LightSetString(sParam1, nParam2, sValue);
	else if(stFunction.rfind(S_LIST_SYSTEM, 0) == 0) {
		if (!bUseListSyst) {
			return;
		}
		return ListSystemSetString(stFunction.substr(S_LIST_SYSTEM.size()), sParam1, nParam2, sValue);
	}
	else
		ObjectSetString(sFunction, sParam1, nParam2, sValue);

}


char*
AspectManager::GetString([[maybe_unused]] char* sFunction,
	[[maybe_unused]] char* sParam1,
	[[maybe_unused]] int nParam2)
{
	std::string stFunction(sFunction);
	std::string sRetVal = "";
	sprintf_s(returnBuffer, MAX_BUFFER, sRetVal.c_str());


	logger->Trace("* Plugin GetString(0x%x, %s, %s, %d)", 0x0, sFunction, sParam1, nParam2);

	//else 
	if (stFunction == "area")
		sRetVal = AreaGetString(sParam1, nParam2);
	else if (stFunction == "creature")
		sRetVal = CreatureGetString(sParam1, nParam2);
	else if (stFunction == "item")
		sRetVal = ItemGetString(sParam1, nParam2);
	else if (stFunction == "light")
		sRetVal = LightGetString(sParam1, nParam2);
	/* usefull for debug stuff */
	else if(stFunction == "getItemAddr")
	{
		NWN::OBJECTID       ObjectId;
		GameObject        *Object;
		GameObjectManager m_ObjectManager;

		ObjectId = (NWN::OBJECTID) nParam2;

		if ((ObjectId & NWN::INVALIDOBJID) != 0)
			ObjectId &= ~(NWN::LISTTYPE_MASK);

		if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
			return returnBuffer;

		const void * address = static_cast<const void*>(Object);
		std::stringstream ss;
		ss << address;  
		sRetVal = ss.str();
	}
	else if (stFunction == "Trigger") {
		std::string sTrigFunc(sParam1);
		if (sTrigFunc == "Geometry") {
			sRetVal = TriggerGetGeometry(nParam2);
		}
	}
	/***************************/
	else if(stFunction.rfind(S_LIST_SYSTEM, 0) == 0) {
		if (!bUseListSyst) {
			return returnBuffer;
		}
		sRetVal = ListSystemGetString(stFunction.substr(S_LIST_SYSTEM.size()), sParam1, nParam2);
	} else {
		sRetVal = ObjectGetString(stFunction, sParam1, nParam2);
	}



	logger->Trace(sRetVal.c_str());
	sprintf_s(returnBuffer, MAX_BUFFER, sRetVal.c_str());

	return returnBuffer;
}


DWORD WINAPI LaunchTestVersion(LPVOID lpParam)
{
	TstVersionPlg testPlugin(aManager->nwnxStringHome, aManager->GetVersion(), aManager->GetSubClass());
	testPlugin.TestVersionPlugin();
	return 0;
}

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

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

typedef void (__cdecl * NWN2Heap_Deallocate_Proc)(void *p);

NWN2Heap_Deallocate_Proc NWN2Heap_Deallocate;

namespace {
	AspectManager* aManager;


	unsigned long ReturnFctOnEquip			= 0x00601f3d;
	unsigned long ReturnFctOnUnEquip		= 0x00601F0D;
	unsigned long CallUnEquip				= 0x00601ee0;
	unsigned long ReturnCallUnEquip			= 0x006ca39f;

	//00601f2a
#define OFFS_BuildMsgOnEquip	0x00601F33
#define OFFS_BuildMsgOnUNEquip	0x00601f05

#define OFFS_CallUnEquip		0x006ca39a

#define OFFS_CServerExoApp_CreateVisual 0x00776130 // Create Visual



	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define OFFS_AVOIDPART		0x0056e6d0

#define OFFS_SETFACING1		0x006ac2d3
#define OFFS_SETFACING2		0x006ac353
#define OFFS_SETFACINGDOOR	0x006ac3b7

	Patch _tempPatch[] =
	{
		//List of TempPatch for SetFacing
		//Patch to make the SetFacing work outside AssignCommand
		Patch((DWORD)OFFS_SETFACING1, (char*)"\x89\x01\x58\x90\x90", (int)5), //MOV [ECX], EAX;  POP EAX; NOP; NOP;
		Patch((DWORD)OFFS_SETFACING2, (char*)"\x89\x01\x58\x90\x90", (int)5), //MOV [ECX], EAX;  POP EAX; NOP; NOP;
		//Patch to allow door (ugly patch, willprobably allow lot of things but ok because temporaryPatch) for SetFacing
		Patch((DWORD)OFFS_SETFACINGDOOR, (char*)"\x8B\xC8\x90\x90\x90\x90\x90\x90", (int)8), //MOV EAX, ECX; NOP; NOP; NOP; NOP; NOP; NOP;


	};

	Patch *tempPatch = _tempPatch;

	void refreshObject(int nParam2,int  nValue, int iOption=0)
	{
		//
		NWN::OBJECTID oToRefresh = (NWN::OBJECTID)nValue;
		NWN::OBJECTID oToView = (NWN::OBJECTID)nParam2;

		unsigned long iMustView = GetObjectToPlayerId(oToView);
		if (iMustView == NWN::PLAYERID_INVALIDID)
			return;

		//Get m_ObjectManager.GetGameObject(idEquipedObject)
		GameObjectManager m_ObjectManager;
		GameObject* ObjectToRefr;

		if ((ObjectToRefr = m_ObjectManager.GetGameObject(oToRefresh)) == NULL)
		{
			return;
		}

		//Just be sure to refresh on the same area.
		{
			//For now, dont allow area, creature, item, waypoint
			NWN::OBJECT_TYPE tObjToRefresh = (NWN::OBJECT_TYPE)*((char*)ObjectToRefr + AmObjectType);

			//For now.. Some object cannot be refreshed. Nothing impossible. Will come with time
			if (tObjToRefresh == NWN::OBJECT_TYPE_AREA || tObjToRefresh == NWN::OBJECT_TYPE_CREATURE || tObjToRefresh == NWN::OBJECT_TYPE_ITEM || tObjToRefresh == NWN::OBJECT_TYPE_WAYPOINT)
				return;
			NWN::OBJECTID oArea1 = *(NWN::OBJECTID*)((char*)ObjectToRefr + AmCommonArea);
			GameObject* ObjectToView;
			if ((ObjectToView = m_ObjectManager.GetGameObject(oToView)) == NULL)
			{
				return;
			}

			NWN::OBJECTID oArea2 = *(NWN::OBJECTID*)((char*)ObjectToView + AmCommonArea);

			//Be sure that the ObjectToView and ObjectToRefresh are on the same Area
			if (oArea1 != oArea2)
				return;
		}


		//Remove the previous view (only if we don't ask for "show only"
		if(iOption != 2)
		{
			
			uint8_t objectType = ((uint8_t*)ObjectToRefr)[0xA4];
			uint8_t RemoveMsg[0xE] = { 0x50, 0x05, 0x01, 0x0D, 0x00, 0x00, 0x00, 0x44, objectType, 0x00, 0x00, 0x00, 0x00, 0x75 };
			uint32_t* RemoveMsgOID = (uint32_t*)(RemoveMsg + 0x9);


			*RemoveMsgOID = oToRefresh;

			RemoveMsg[0xC] |= 0x80;
			PrepaSendMessageToPlayer(iMustView, (unsigned char*) RemoveMsg, 0xE, 0);

			//If we ask for "mask only", stop here 
			if (iOption == 1)
				return; 
		}

		uint8_t myMessage[0x500] = {};
		uint32_t sizeToSend = addObjectToWorld(oToView, ObjectToRefr, myMessage);


		/*
		// Tree ... No
		// Environment object : No
		// Sound ... : No
		*/
		PrepaSendMessageToPlayer(iMustView, myMessage, sizeToSend, 0);

		return;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	int inventorySlotToArmorPatch(int iSlot)
	{
		int iArmorModelPart = -1;

		if (iSlot == 0)
		{
			iArmorModelPart = 0;
		}
		else if (iSlot == 2)
		{
			iArmorModelPart = 2;
		}
		else if (iSlot == 3)
		{
			iArmorModelPart = 1;
		}
		else if (iSlot == 6)
		{
			iArmorModelPart = 4;
		}
		else if (iSlot == 10)
		{
			iArmorModelPart = 3;
		}

		return iArmorModelPart;
	}

	__declspec(naked) void __fastcall UpdateBaseVisual(__in uint8_t* CreaObject)
	{
		__asm
		{
			mov		edx, 0x005e04b0; prepare the jump
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
			UpdateBaseVisual((uint8_t*)CreaObject);

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

		int iArmorModelPart = inventorySlotToArmorPatch(iEmplacementObj);
		if(iArmorModelPart == -1 && iEmplacementObj != 1)
		{
			return iEmplacementObj;
		}

		if (iArmorModelPart != -1)
		{
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
			if (std::find(aManager->m_FixPart[iArmorModelPart].begin(), aManager->m_FixPart[iArmorModelPart].end(), iItemType) == aManager->m_FixPart[iArmorModelPart].end()) {
				return iEmplacementObj;
			}
		}


		uint32_t ObjectCreaId = *(uint32_t*)(((char*)CreaturePtr) + 0xA0);

		if (ObjectCreaId == 0 || ObjectCreaId == NWN::INVALIDOBJID)
			return iEmplacementObj;
		ObjectCreaId &= ~(NWN::LISTTYPE_MASK);
		if ((Object = m_ObjectManager.GetGameObject(ObjectCreaId)) == NULL)
		{
			return iEmplacementObj;
		}
		uint32_t* ObjEquipdPtrCalc = *(uint32_t**)((((char*)Object) + AmCrtCSEquipedObjectTable));
		if (((char*)ObjEquipdPtrCalc) != ((char*)objEquipdListPtr))
		{
			return iEmplacementObj;
		}
		/*
		if(ObjEquipdPtrCalc[iEmplacementObj+1] == 0x7F000000)
			finishRemove(ObjectCreaId, (uint8_t*)Item);
		*/
		refreshForEveryConcerned(ObjectCreaId, false);
		return iEmplacementObj;
	}

	unsigned long ReturnChangePtrVisual = 0x0074fec8;

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
			call dword ptr[CallUnEquip]
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
			mov edx, dword ptr ds : [edx + 7D0h]
			mov dword ptr ds : [ecx + eax * 4 + 4] , edx
			push esi
			push eax
			call FixArmorPartChgEquipSt
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
			mov edx, dword ptr ds:[ecx+eax*4+4]
			mov dword ptr ds:[ecx+eax*4+4],7F000000h
			push esi
			push eax
			call FixArmorPartChgEquipSt
			push esi
			jmp dword ptr[ReturnFctOnUnEquip]
		}
	}

	unsigned long ReturnCreatureVisualPatch = 0x0074feb2;
	unsigned long ContinueCreatureVisual = 0x0074fe00;
	unsigned long CallPrepareVisualArmor = 0x00776100;
	unsigned long WholeVisualArmorFct = 0x0074fde0;
	int g_iPatchVisualCreaMode = 0;

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

#define OFFS_PrepareVisualPatch1 0x005e04ef
#define OFFS_PrepareVisualPatch2 0x005e056d

	unsigned long ReturnPrepareVisualPatch1 = 0x05e04f4;
	unsigned long ReturnPrepareVisualPatch2 = 0x005e0572;

	__declspec(naked) void PrepareCreatureVisualPatch1()
	{
		__asm
		{
			MOV g_iPatchVisualCreaMode, 0x1
			call	dword ptr[WholeVisualArmorFct]
			jmp		dword ptr[ReturnPrepareVisualPatch1]
		}
	}


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

		Patch((DWORD)OFFS_BuildMsgOnEquip, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6), //JMP NOP...
		Patch(OFFS_BuildMsgOnEquip + 1, (relativefunc)FixArmorPartOnEquip),

		Patch((DWORD)OFFS_BuildMsgOnUNEquip, (char*)"\xe9\x00\x00\x00\x00\x90\x90\x90", (int)8), //JMP NOP NOP..
		Patch(OFFS_BuildMsgOnUNEquip + 1, (relativefunc)FixArmorPartOnUnEquip),


		Patch((DWORD)OFFS_CallUnEquip, (char*)"\xe9\x00\x00\x00\x00", (int)5), //JMP
		Patch(OFFS_CallUnEquip + 1, (relativefunc)PrepareCreaturePtr),
		

		Patch((DWORD)OFFS_PrepareVisualPatch1, (char*)"\xe9\x00\x00\x00\x00", (int)5), //JMP
		Patch((DWORD)OFFS_PrepareVisualPatch1+1, (relativefunc)PrepareCreatureVisualPatch1),

		Patch((DWORD)OFFS_PrepareVisualPatch2, (char*)"\xe9\x00\x00\x00\x00", (int)5), //JMP
		Patch((DWORD)OFFS_PrepareVisualPatch2+1, (relativefunc)PrepareCreatureVisualPatch2),

		//Keep the CurrentVisual in a good state. Taking in account belt AND "NA" armor part
		Patch((DWORD)0x0074fdfb, (char*)"\xe9\x00\x00\x00\x00", (int)5), //JMP
		Patch(0x0074fdfc, (relativefunc)CreatureVisualPatch),

		//As we have updated the "CurrentVisual", use it.
		Patch((DWORD)0x0074fec2, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6), //JMP //NOP
		Patch(0x0074fec3, (relativefunc)ChangePtrVisual),

		Patch()
	};

	Patch *itemPatches = _itemsPatches;

#define OFF_GetScaleHook	0x006AA61E
#define OFF_SetScaleHook	0x006AA51C

	unsigned long RetfixGetScaleIsDoor = 0x006AA636;
	unsigned long RetfixGetScaleIsNotDoor = 0x006AA624;
	unsigned long RetfixSetScaleIsDoor = 0x006AA534;
	unsigned long RetfixSetScaleIsNotDoor = 0x006AA522;

	//0x006AA61E
	__declspec(naked) void FixGetScale()
	{
		__asm
		{
			MOV		AL, [ESI + 0xA4]
			CMP		AL, 0xA
			JNE		GetScaleNotDoor

			MOV		EAX, ESI
			JMP		dword ptr[RetfixGetScaleIsDoor]

GetScaleNotDoor:
			MOV		EAX, dword ptr [EDX + 0x8C]
			JMP		dword ptr[RetfixGetScaleIsNotDoor]
		}

	}
	

	//0x006AA51C
	__declspec(naked) void FixSetScale()
	{
		__asm
		{
			MOV		AL, [ESI + 0xA4]
			CMP		AL, 0xA
			JNE		SetScaleNotDoor

			MOV		EAX, ESI
			JMP		dword ptr[RetfixSetScaleIsDoor]

SetScaleNotDoor:
			MOV		EAX, dword ptr [EDX + 0x8C]
			JMP		dword ptr[RetfixSetScaleIsNotDoor]
		}

	}


	Patch _quickPatches[] =
	{
		//Patch on getScale, to allow door too
		Patch((DWORD)OFF_GetScaleHook, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6), //JMP NOP
		Patch(OFF_GetScaleHook + 1, (relativefunc)FixGetScale),

		//Patch on setScale, to allow door too
		Patch((DWORD)OFF_SetScaleHook, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6), //JMP NOP
		Patch(OFF_SetScaleHook + 1, (relativefunc)FixSetScale),

		Patch()
	};

	Patch *quickPatches = _quickPatches;


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

/////////////////////////////////////// Position And Orientation ///////////////////////////////////////
__declspec(naked) void __fastcall SetPlaceableColision(__in void* ptrPlc, __in void* Unused, __in float* position, __in uint32_t zero)
{
	__asm
	{
		mov		edx, 0x0057F360;
		jmp		edx;
	}
}

__declspec(naked) void __fastcall SetOrientationQ(__in void* idObjBlock, __in void* Unused, __in uint32_t vA, __in uint32_t v2)
{
	__asm
	{
		mov		edx, 0x006ac290;
		jmp		edx;
	}
}


void* PrepareObjectRepositionning(GameObject* Object)
{
	GameObjectManager m_ObjectManager;
	GameObject* Area;
	void* AreaPtr = NULL;

	uint8_t objType = *(uint8_t*)((int)Object + 0xA4);

	if(objType == NWN::OBJECT_TYPE_PLACEABLE || objType == NWN::OBJECT_TYPE_DOOR)
	{
		NWN::OBJECTID AreaId = *(NWN::OBJECTID*)((int)Object + 0x144);

		if ((AreaId & NWN::INVALIDOBJID) != 0)
			AreaId &= ~(NWN::LISTTYPE_MASK);

		if ((Area = m_ObjectManager.GetGameObject( AreaId )) == NULL)
			return AreaPtr;

		AreaPtr = (void*)((int)Area - 0xad0);

		//Remove Old "position"
		RemoveFromArea(AreaPtr, (void*)Object);
	}

	return AreaPtr;
}

void FinishObjectRepositionning(GameObject* Object, void* AreaPtr)
{
	uint8_t objType = *(uint8_t*)((int)Object + 0xA4);

	if(AreaPtr == NULL)
		return;

	if(objType == NWN::OBJECT_TYPE_PLACEABLE || objType == NWN::OBJECT_TYPE_DOOR)
	{
		//Recalculate newPosition
		ColisionLoSModelName((void*)Object);

		//Add new "position"
		AddToArea(AreaPtr, (void*)Object);
	}
}

void ObjectSetPosition(int iObjectId, float fX, float fY, float fZ)
{
	NWN::OBJECTID ObjectId = (NWN::OBJECTID)iObjectId;
	GameObject* Object;
	GameObjectManager m_ObjectManager;
	void* AreaPtr;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return;

	AreaPtr = PrepareObjectRepositionning(Object);


	if (Object->GetObjectType() == NWN::OBJECT_TYPE_TRIGGER)
	{
		float fDiffX = *(float*)(((char*)Object) + AmCommonPosX);
		fDiffX = fDiffX - fX;
		float fDiffY = *(float*)(((char*)Object) + AmCommonPosY);
		fDiffY = fDiffY - fY;
		float fDiffZ = *(float*)(((char*)Object) + AmCommonPosZ);
		fDiffZ = fDiffZ - fZ;

		uint32_t iNbPoint = *(uint32_t*)(((char*)Object) + AmTrigGeometrySize);

		float* fPoints = *(float**)(((char*)Object) + AmTrigGeometryPtr);

		for (uint32_t iCur = 0; iCur < iNbPoint; iCur++)
		{
			float fCurX = fPoints[iCur*3];
			fPoints[iCur*3] = fCurX - fDiffX;
			float fCurY = fPoints[iCur*3+1];
			fPoints[iCur*3+1] = fCurY - fDiffY;
			float fCurZ = fPoints[iCur*3+2];
			fPoints[iCur*3+2] = fCurZ - fDiffZ;
		}
	}

	//Set the new position
	*(float*)(((char*)Object) + AmCommonPosX) = fX;
	*(float*)(((char*)Object) + AmCommonPosY) = fY;
	*(float*)(((char*)Object) + AmCommonPosZ) = fZ;


	FinishObjectRepositionning(Object, AreaPtr);

	//TODO Send Refresh Message ? 
}

void ObjectSetOrientation(int iObjectId, float fPos)
{
	// It will allow to take care of the collisions when rotating
	NWN::OBJECTID ObjectId = (NWN::OBJECTID)iObjectId;
	GameObject* Object;
	GameObjectManager m_ObjectManager;
	void* AreaPtr;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return;

	AreaPtr = PrepareObjectRepositionning(Object);

	float fPreviousFacing;

	if(Object->GetObjectType() == NWN::OBJECT_TYPE_TRIGGER)
	{
		float fCos = *(float*)((char*)Object + 0x1C);
		float fSin = *(float*)((char*)Object + 0x20);

		float fRad = acosf(fCos);
		float fDeg = fRad * 180.0f / M_PI;

		if (fSin < 0)
		{
			fDeg = fDeg - 360.0f;
			fDeg = fDeg * -1.0f;
		}


		fPreviousFacing = fDeg;
	}

	//Block parameters
	float aFloat[2];
	aFloat[1] = fPos;
	uint32_t vmPtr = 0x864424;
	uint32_t* ptrBlock = *(uint32_t**)vmPtr;
	ptrBlock += 0x5E;

	unsigned char saved[0x1C];

	memcpy_s(saved, 0x1C, ptrBlock, 0x1C);

	memset(ptrBlock, 0, 0x1C);

	((uint8_t*)ptrBlock)[0] = 2;

	ptrBlock[5] = (uint32_t)ptrBlock;
	((uint8_t*)ptrBlock)[0x18] = 1;
	ptrBlock[4] = (uint32_t)aFloat;

	//Block "Caller"
	uint8_t blockCaller[16] = { 0x58, 0xF3, 0x6D, 0x0B, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	blockCaller[8] = (iObjectId & 0xFF);
	blockCaller[9] = (iObjectId & 0xFF00) >> 8;
	blockCaller[10] = (iObjectId & 0xFF0000) >> 16;
	blockCaller[11] = (iObjectId & 0xFF000000) >> 24;


	//Patch the function
	tempPatch[0].Apply();
	tempPatch[1].Apply();
	tempPatch[2].Apply();


	//Call it
	SetOrientationQ(blockCaller, NULL, 0xA, 0x2);

	tempPatch[0].Remove();
	tempPatch[1].Remove();
	tempPatch[2].Remove();


	//Restore Function


	//Restore
	memcpy_s(ptrBlock, 0x1C, saved, 0x1C);


	//if trigger, need to calculate all new points
	if(Object->GetObjectType() == NWN::OBJECT_TYPE_TRIGGER)
	{

		uint32_t iNbPoint = *(uint32_t*)(((char*)Object) + AmTrigGeometrySize);

		float* fPoints = *(float**)(((char*)Object) + AmTrigGeometryPtr);

		float oX = *(float*)(((char*)Object) + AmCommonPosX);
		float oY = *(float*)(((char*)Object) + AmCommonPosY);

		for (uint32_t iCur = 0; iCur < iNbPoint; iCur++)
		{
			float fCurX = fPoints[iCur*3];
			float fCurY = fPoints[iCur*3+1];

			//The real change of angle
			float fAngle = fPos - fPreviousFacing;
			fAngle = fAngle * (M_PI / 180.0f);

			float xM = fCurX - oX;
			float yM = fCurY - oY;

			float x = xM * cos(fAngle) + yM * sin(fAngle) + oX;
			float y = 0 - xM * sin(fAngle) + yM * cos(fAngle) + oY;

			fPoints[iCur * 3] = x;
			fPoints[iCur * 3 + 1] = y;
		}
	}



	FinishObjectRepositionning(Object, AreaPtr);
}

void ObjectSetPositionAndOrientation(int iObjectId, float fX, float fY, float fZ, float fFacing)
{
	NWN::OBJECTID ObjectId = (NWN::OBJECTID)iObjectId;
	GameObject* Object;
	GameObjectManager m_ObjectManager;
	void* AreaPtr;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return;

	AreaPtr = PrepareObjectRepositionning(Object);


	if (Object->GetObjectType() == NWN::OBJECT_TYPE_TRIGGER)
	{
		float fDiffX = *(float*)(((char*)Object) + AmCommonPosX);
		fDiffX = fDiffX - fX;
		float fDiffY = *(float*)(((char*)Object) + AmCommonPosY);
		fDiffY = fDiffY - fY;
		float fDiffZ = *(float*)(((char*)Object) + AmCommonPosZ);
		fDiffZ = fDiffZ - fZ;

		uint32_t iNbPoint = *(uint32_t*)(((char*)Object) + AmTrigGeometrySize);

		float* fPoints = *(float**)(((char*)Object) + AmTrigGeometryPtr);

		for (uint32_t iCur = 0; iCur < iNbPoint; iCur++)
		{
			float fCurX = fPoints[iCur*3];
			fPoints[iCur*3] = fCurX - fDiffX;
			float fCurY = fPoints[iCur*3+1];
			fPoints[iCur*3+1] = fCurY - fDiffY;
			float fCurZ = fPoints[iCur*3+2];
			fPoints[iCur*3+2] = fCurZ - fDiffZ;
		}
	}

	//Set the new position
	*(float*)(((char*)Object) + AmCommonPosX) = fX;
	*(float*)(((char*)Object) + AmCommonPosY) = fY;
	*(float*)(((char*)Object) + AmCommonPosZ) = fZ;

	ObjectSetOrientation(iObjectId, fFacing);

	FinishObjectRepositionning(Object, AreaPtr);

	//TODO Send Refresh Message ? 
}

std::string TriggerGetGeometry(int iObjectId) {

	std::string retGeometry = "";
	NWN::OBJECTID ObjectId = (NWN::OBJECTID)iObjectId;
	GameObject* Object;
	GameObjectManager m_ObjectManager;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return retGeometry;


	if (Object->GetObjectType() == NWN::OBJECT_TYPE_TRIGGER)
	{
		uint32_t iNbPoint = *(uint32_t*)(((char*)Object) + AmTrigGeometrySize);
		uint32_t iSizeBuff = *(uint32_t*)(((char*)Object) + AmTrigGeometryBufSize);
		float* fPoints = *(float**)(((char*)Object) + AmTrigGeometryPtr);

		std::stringstream ss;
		for (size_t i = 0; i < iNbPoint; i++) {
			ss << fPoints[i] << " ";
		}

		retGeometry = ss.str();
	}

	return retGeometry;
}

void TriggerChangeGeometry(int iObjectId, std::vector<float>& vGeometry) {

	NWN::OBJECTID ObjectId = (NWN::OBJECTID)iObjectId;
	GameObject* Object;
	GameObjectManager m_ObjectManager;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return;


	if (Object->GetObjectType() == NWN::OBJECT_TYPE_TRIGGER)
	{
		uint32_t iNbPoint = *(uint32_t*)(((char*)Object) + AmTrigGeometrySize);
		uint32_t iSizeBuff = *(uint32_t*)(((char*)Object) + AmTrigGeometryBufSize);

		float* fPoints;
		float* fOldPoints;
		if (iSizeBuff*3 < vGeometry.size())
		{
			//Need to create the datablock.
			NWN2_HeapMgr *pHeapMgr = NWN2_HeapMgr::Instance();
			NWN2_Heap *pHeap = pHeapMgr->GetDefaultHeap();
			fPoints = (float*)pHeap->Allocate(4*vGeometry.size()*3);

			//Need to save the old datablock addr
			fOldPoints = *(float**)(((char*)Object) + AmTrigGeometryPtr);
		} else {
			fPoints = *(float**)(((char*)Object) + AmTrigGeometryPtr);
		}
		//Change the datablock used, free the old one.
		iNbPoint = vGeometry.size()/3;

		for (uint32_t iCur = 0; iCur < iNbPoint; iCur++)
		{
			fPoints[iCur*3] = vGeometry[iCur*3];
			fPoints[iCur*3+1] = vGeometry[iCur*3+1];
			fPoints[iCur*3+2] = vGeometry[iCur*3+2];
		}

		if(iSizeBuff*3 < vGeometry.size())
		{

			//free the old one
			if (NWN2Heap_Deallocate)
				NWN2Heap_Deallocate( fOldPoints );

			//Replace size & ptr
			*(float**)(((char*)Object) + AmTrigGeometryPtr) = fPoints;
			*(uint32_t*)(((char*)Object) + AmTrigGeometryBufSize) = iNbPoint;
		}
		*(uint32_t*)(((char*)Object) + AmTrigGeometrySize) = iNbPoint;
		*(uint32_t*)(((char*)Object) + AmTrigGeometrySize2) = iNbPoint;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////

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
		"(c) 2024 by Septirage\n"
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
		int i = 0;
		while(quickPatches[i].Apply()) {
			i++;
		}
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
			if (std::filesystem::exists("AspectManagerScan"))
			{
				iScanFile = 1;
				std::remove("AspectManagerScan");
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
		NWN::OBJECTID       ObjectId;
		GameObject* Object;
		GameObjectManager m_ObjectManager;

		ObjectId = (NWN::OBJECTID) nParam2;

		if ((ObjectId & NWN::INVALIDOBJID) != 0)
			ObjectId &= ~(NWN::LISTTYPE_MASK);

		if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
			return;
		if (Object->GetObjectType() != NWN::OBJECT_TYPE::OBJECT_TYPE_PLACEABLE)
			return;

		if (sCommand == "DynamicCl")
		{
			uint32_t* ptrDynamicCol	= (uint32_t*)(((char*)Object) + AmPlcDynamicCol);
			uint32_t bColBefore = *ptrDynamicCol;
			char* pointedFlags = (((char*)Object) + AmPlcColisionFlag);

			if (bColBefore != (uint32_t)nValue)
			{
				//We change it, so need to call all the fct
				*ptrDynamicCol = (uint32_t)nValue;

				void* AreaPtr = PrepareObjectRepositionning(Object);

				char* MyFlagsAddr = *(char**)(pointedFlags);
				MyFlagsAddr = (MyFlagsAddr + 0x10);
				if (nValue == 0)
				{
					*(uint32_t*)MyFlagsAddr = (*(uint32_t*)MyFlagsAddr) & 0xfeffffff;
				}
				else
				{
					*(uint32_t*)MyFlagsAddr = (*(uint32_t*)MyFlagsAddr) | 0x1000000;
				}


				FinishObjectRepositionning(Object, AreaPtr);
			}
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

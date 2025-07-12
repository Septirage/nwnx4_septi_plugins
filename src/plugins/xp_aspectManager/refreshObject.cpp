#include "aspectManager.h"
#include "messageManagement.h"
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
#include "../../septutil/NwN2DataPos.h"

__declspec(naked) void __fastcall RemoveFromArea_Intern(__in void* ptrArea, __in void* Unused, __in void* ptrObject)
{
	__asm
	{
		mov             edx, FCT_REMOVEFROMAREA;
		jmp             edx;
	}
}

__declspec(naked) void __fastcall AddToArea_Intern(__in void* ptrArea, __in void* Unused, __in void* ptrObject)
{
	__asm
	{
		mov             edx, FCT_ADDTOAREA;
		jmp             edx;
	}
}

/*
#define FCT_VISUALANDCOLPOSITION 0x0057f360
__declspec(naked) void __fastcall VisualAndCollisionPosition(__in void* ptrObjet, __in void* Unused, __in float* arrayPositionS, __in int param_3)
{
__asm
{
mov             edx, FCT_VISUALANDCOLPOSITION;
jmp             edx;
}
}
*/

__declspec(naked) void __fastcall ColisionLoSModelName_Intern(__in void* ptrObjet)
{
	__asm
	{
		mov             edx, FCT_COLISIONANDLOS;
		jmp             edx;
	}
}

__declspec(naked) void __fastcall ColisionLosModelNameDoor_Intern(__in void* ptrObjet)
{
	__asm
	{
		mov             edx, FCT_COLISIONANDLOSDOOR;
		jmp             edx;
	}
}

void RemoveFromArea(void* ptrArea, void* ptrObject)
{
	RemoveFromArea_Intern(ptrArea, NULL, ptrObject);
}


void AddToArea(void* ptrArea, void* ptrObject)
{
	AddToArea_Intern(ptrArea, NULL, ptrObject);
}

void ColisionLoSModelName(void* ptrObject)
{
	uint8_t objectType = ((uint8_t*)ptrObject)[0xA4];
	if (objectType == 0xA)
		ColisionLosModelNameDoor_Intern(ptrObject);
	else if(objectType == 0x9)
		ColisionLoSModelName_Intern(ptrObject);
}




void refreshObject(int nParam2, int nValue, int iOption)
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

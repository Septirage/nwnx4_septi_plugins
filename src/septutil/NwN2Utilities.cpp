#include "NwN2Utilities.h"
#include "NwN2DataPos.h"
#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include <cstdint>


NWN::OBJECTID GetModuleID()
{
	int ptr = *(int*)OFFS_g_pAppManager;
	ptr = *(int*)(ptr + 4);
	ptr = *(int*)(ptr + 4);
	NWN::OBJECTID result = *(NWN::OBJECTID*)(ptr + 0x10088);
	return result;
}


uint32_t GetPCIDFromCreature(uint32_t oCreature)
{
	uint32_t playerID = NWN::PLAYERID_INVALIDID;
	//Get it quick if controlled
	NWN::CGameObject * Object;
	NWN::CNWSCreature * Creature;
	GameObjectManager m_ObjectManager;

	Object = m_ObjectManager.GetGameObject( (NWN::OBJECTID) oCreature );
	if (Object == NULL)
		return NWN::PLAYERID_INVALIDID;

	Creature = Object->AsCreature();

	if (Creature == NULL)
		return NWN::PLAYERID_INVALIDID;

	//We have a creature. Is it a controlled Creature or not?
	playerID = Creature->GetControllingPlayerId();

	if (playerID != NWN::PLAYERID_INVALIDID)
		return playerID;

	//Not a controlled creature. Test if its an owned one.
	// 
	//Test if it has a master. 
	uint32_t oMaster = *(uint32_t*)(((char*)Creature) + AmCrtMasterID);
	if (oMaster != 0x7F000000)
	{
		oCreature = oMaster;
	}

	//Go trhough all Players

	uint32_t eax = *(uint32_t*)OFFS_g_pAppManager;
	eax = *(uint32_t*)(((char*)eax) + 4);
	eax = *(uint32_t*)(((char*)eax) + 4);

	eax = *(uint32_t*)(((char*)eax) + 0x1008c);
	if (eax == 0)
	{
		return playerID;
	}

	//We are on the "list". So ... [ptrToFirst][ptrToLast][Size]

	//First array element.
	elementArray* currentElement  = *(elementArray**)eax;
	//For array elements we have... [previous][next][element]

	while (currentElement  != NULL)
	{
		//Error, we stop here
		if (currentElement->ptrPlayerStruct == NULL)
			return playerID;

		CNWSPlayerStruct* playerStruct = currentElement->ptrPlayerStruct;

		uint32_t oTestedOwned = (uint32_t)playerStruct->ownedCreature;

		if (oTestedOwned == oCreature)
			return playerStruct->playerID;

		currentElement = currentElement->next;

	}

	return playerID;
}

int* GetCNWSMessage()
{
	int ptr = *(int*)OFFS_g_pAppManager;
	ptr     = *(int*)(ptr + 4);
	ptr     = *(int*)(ptr + 4);
	return *(int**)(ptr + 0x10020);
}

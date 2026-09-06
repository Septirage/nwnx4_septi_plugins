#include "NwN2Utilities.h"
#include "NwN2DataPos.h"
#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include <cstdint>


NWN::OBJECTID GetModuleID()
{
	NWN::CAppManager* pAppManager = *(NWN::CAppManager**)OFFS_g_pAppManager;
	if (pAppManager == NULL)
		return NWN::INVALIDOBJID;

	NWN::CServerExoApp* pServerExoApp = pAppManager->m_pServerExoApp;
	if (pServerExoApp == NULL)
		return NWN::INVALIDOBJID;

	NWN::CServerExoAppInternal* pExoAppInternal = pServerExoApp->m_pcExoAppInternal;
	if (pExoAppInternal == NULL)
		return NWN::INVALIDOBJID;

	uint8_t* pBase = (uint8_t*)pExoAppInternal;
	NWN::OBJECTID result = *(NWN::OBJECTID*)(pBase + 0x10088);

	return result;
}

//OFFS_g_pAppManager
//Must be redone to be based on struct instead ugly ptr management
CNetBigPlayerInfo* GetCNetPlayerInfo(uint8_t idPlayer)
{
	NWN::CAppManager* pAppManager = *(NWN::CAppManager**)OFFS_g_pAppManager;
	if (pAppManager == NULL)
		return NULL;

	NWN::CServerExoApp* pServerExoApp = pAppManager->m_pServerExoApp;
	if (pServerExoApp == NULL)
		return NULL;

	NWN::CServerExoAppInternal* pExoAppInternal = pServerExoApp->m_pcExoAppInternal;
	if (pExoAppInternal == NULL)
		return NULL;

	uint8_t* pBase = (uint8_t*)pExoAppInternal;
	void* pNetLayerWrapper = *(void**)(pBase + 0x10068);
	if (pNetLayerWrapper == NULL)
		return NULL;

	CShortNetLayerInternal* pNetLayer = *(CShortNetLayerInternal**)pNetLayerWrapper;
	if (pNetLayer == NULL)
		return NULL;

	if (idPlayer >= NWN2_MAX_PLAYERS)
		return NULL;

	return &pNetLayer->Players[idPlayer];
}




elementArray* getBasePCBlockList() {
	uint32_t eax = *(uint32_t*)OFFS_g_pAppManager;
	eax = *(uint32_t*)(eax + 4);
	eax = *(uint32_t*)(eax + 4);
	eax = *(uint32_t*)(eax + 0x1008C);

	if (eax == 0) return nullptr;
	return *(elementArray**)eax;
}

//






CNWSPlayerStruct* GetPCBlockFromCreature(uint32_t oCreature)
{
	uint32_t playerID = NWN::PLAYERID_INVALIDID;
	//Get it quick if controlled
	NWN::CGameObject * Object;
	NWN::CNWSCreature * Creature;
	GameObjectManager m_ObjectManager;

	Object = m_ObjectManager.GetGameObject( (NWN::OBJECTID) oCreature );
	if (Object == NULL)
		return NULL;

	Creature = Object->AsCreature();

	if (Creature == NULL)
		return NULL;

	//We have a creature. Is it a controlled Creature or not?
	playerID = Creature->GetControllingPlayerId();

	if (playerID != NWN::PLAYERID_INVALIDID)
	{
		//Not a controlled creature. Test if its an owned one.
		// 
		//Test if it has a master. 
		uint32_t oMaster = *(uint32_t*)(((char*)Creature) + AmCrtMasterID);
		if (oMaster != 0x7F000000)
		{
			oCreature = oMaster;
		}
	}



	//Go trhough all Players

	uint32_t eax = *(uint32_t*)OFFS_g_pAppManager;
	eax = *(uint32_t*)(((char*)eax) + 4);
	eax = *(uint32_t*)(((char*)eax) + 4);

	eax = *(uint32_t*)(((char*)eax) + 0x1008c);
	if (eax == 0)
	{
		return NULL;
	}

	//We are on the "list". So ... [ptrToFirst][ptrToLast][Size]

	//First array element.
	elementArray* currentElement  = *(elementArray**)eax;
	//For array elements we have... [previous][next][element]

	while (currentElement  != NULL)
	{
		//Error, we stop here
		if (currentElement->ptrPlayerStruct == NULL)
			return NULL;

		CNWSPlayerStruct* playerStruct = currentElement->ptrPlayerStruct;

		uint32_t oTestedOwned = (uint32_t)playerStruct->ownedCreature;

		if (oTestedOwned == oCreature)
			return playerStruct;

		currentElement = currentElement->next;

	}

	return NULL;
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

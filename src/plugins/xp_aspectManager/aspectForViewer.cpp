#include "aspectForViewer.h"

#include <misc/Patch.h>
#include <misc/ini.h>
#include <misc/log.h>
#include <plugins/plugin.h>
#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include <hook/scriptManagement.h>
#include "../../septutil/NwN2DataPos.h"
#include "../../septutil/NwN2Utilities.h"
#include "../../septutil/NwN2Message.h"
#include "../../septutil/intern2daMgt.h"
#include "messageManagement.h"


#include <nwn2heap.h>

#include <unordered_set>







#define OFFS_PLAYERLISTADD 0x00558000


__declspec(naked) bool __fastcall SendServerToPlayerList_Add(__in void* MsgCreator, __in void* Unused, uint32_t receiverPCID, void* pcBlockPtr)
{
	__asm
	{
		mov		edx, OFFS_PLAYERLISTADD;
		jmp		edx;
	}
}

typedef NWN::CExoString* (__thiscall *NWN2ReturnFullName_t)(NWN::CNWSCreatureStatsCore* pCreaStat, NWN::CExoString* pExoStr);

NWN2ReturnFullName_t const NWN2ReturnFullName = (NWN2ReturnFullName_t)0x0059d770;


static uint32_t* fakeLocalizedString;
static uint32_t* fakeCELocString;
static uint32_t* fakeLocalizedString2;
static uint32_t* fakeCELocString2;
static uint32_t* ptrFakeLString;
static uint32_t* ptrFakeLString2;

void InitializeCELocString()
{
	static bool bInitialized = false;

	if(!bInitialized)
	{
		fakeCELocString = new uint32_t[3];
		fakeCELocString[0] = 0;
		ptrFakeLString = new uint32_t[1];
		ptrFakeLString[0] = (uint32_t)fakeCELocString;

		fakeLocalizedString = new uint32_t[4];
		fakeLocalizedString[0] = 0xFFFFFFFF;
		fakeLocalizedString[1] = (uint32_t)ptrFakeLString;
		fakeLocalizedString[2] = 1;
		fakeLocalizedString[3] = 1;

		fakeCELocString2 = new uint32_t[3];
		fakeCELocString2[0] = 0;
		ptrFakeLString2 = new uint32_t[1];
		ptrFakeLString2[0] = (uint32_t)fakeCELocString2;

		fakeLocalizedString2 = new uint32_t[4];
		fakeLocalizedString2[0] = 0xFFFFFFFF;
		fakeLocalizedString2[1] = (uint32_t)ptrFakeLString2;
		fakeLocalizedString2[2] = 1;
		fakeLocalizedString2[3] = 1;
		bInitialized = true;
	}

}


#define HOOK_PrepareGUIMsgParty 0x00631a98
#define HOOK_PrepareMsgPartyE	0x005772d1

unsigned long ReturnToMsgPartyx14x15 = 0x00631a9d;
unsigned long ReturnToPrepareMsgPartyE = 0x005772d7;
unsigned long ReturnAfterPrepareMsgPartyE = 0x005772ee;


bool __fastcall PreparePlayerName(NWN::CNWSCreature *ptrCrea)
{
	InitializeCELocString();
	NWN::OBJECTID objID = ptrCrea->GetObjectId();


	CNWSPlayerStruct* pStructChanged = GetPCBlockFromCreature(objID);
		
	if (pStructChanged == NULL || pStructChanged->playerID > 0x60)
	{
		//Ou peut etre prendre le nom et prenom comme ca on ne s'embete meme pas apres ?
		return false;
	}

	uint32_t playerID = pStructChanged->playerID;
	CNetBigPlayerInfo* pPlayerInfo = GetCNetPlayerInfo(playerID);

	fakeCELocString[1] = (uint32_t) (pPlayerInfo->m_sPlayerName.m_sString);
	fakeCELocString[2] = (uint32_t) (pPlayerInfo->m_sPlayerName.m_nBufferLength);

	fakeLocalizedString2[1] = 0;
	fakeLocalizedString2[2] = 0;
	fakeLocalizedString2[3] = 0;

	return true;
}

NWN::CExoString* __fastcall PreparePlayerFullName(NWN::CNWSCreatureStatsCore* ptrCreaStat, NWN::CExoString* ptrRes)
{
	InitializeCELocString();
	NWN::CNWSCreature* Object = *(NWN::CNWSCreature**)((uint32_t)ptrCreaStat + 0xA4);

	CNWSPlayerStruct* pStructChanged = GetPCBlockFromCreature(Object->GetObjectId());
	NWN::CExoString* ptrExoStr2;
	if (pStructChanged == NULL || pStructChanged->playerID > 0x60)
	{
		ptrExoStr2 = NWN2ReturnFullName(ptrCreaStat, ptrRes);
	}
	else
	{
		uint32_t playerID = pStructChanged->playerID;
		CNetBigPlayerInfo* pPlayerInfo = GetCNetPlayerInfo(playerID);

		fakeCELocString[1] = (uint32_t) (pPlayerInfo->m_sPlayerName.m_sString);
		fakeCELocString[2] = (uint32_t) (pPlayerInfo->m_sPlayerName.m_nBufferLength);

		fakeLocalizedString2[1] = 0;
		fakeLocalizedString2[2] = 0;
		fakeLocalizedString2[3] = 0;

		//Update creature firstname/lastname
		uint32_t uBaseFirstName[4] = { 0 };
		uint32_t uBaseLastName[4] = { 0 };
		memcpy(uBaseFirstName, (void*)(((uint32_t)Object) + 0x2d4), 4 * 4);
		memcpy(uBaseLastName, (void*)(((uint32_t)Object) + 0x2e4), 4 * 4);

		memcpy((void*)(((uint32_t)Object) + 0x2d4), fakeLocalizedString, 4 * 4);
		memcpy((void*)(((uint32_t)Object) + 0x2e4), fakeLocalizedString2, 4 * 4);

		//Ok, now call the function
		ptrExoStr2 = NWN2ReturnFullName(ptrCreaStat, ptrRes);


		//restore creature firstname/lastname

		memcpy((void*)(((uint32_t)Object) + 0x2d4), uBaseFirstName, 4 * 4);
		memcpy((void*)(((uint32_t)Object) + 0x2e4), uBaseLastName, 4 * 4);
	}

	return ptrExoStr2;
}


__declspec(naked) void PatchUserNameForGUIParty()
{
	__asm
	{
		CALL	PreparePlayerFullName
		JMP		dword ptr[ReturnToMsgPartyx14x15]
	}
}

__declspec(naked) void PatchUserNameForMsgPartyE()
{
	__asm
	{
		PUSH	ECX
		PUSH	EDX
		MOV		ECX, EBX
		CALL	PreparePlayerName
		POP		EDX
		POP		ECX
		MOV		dword ptr [ESP + 0x10], ECX
		TEST	EAX, EAX
		JNZ		GotoSetFalseLocStr

		
		MOV		EAX, dword ptr [EDX + 0x14c]

		JMP		dword ptr[ReturnToPrepareMsgPartyE]
		//0x005772ee

	GotoSetFalseLocStr:
		MOV		EAX, [fakeLocalizedString]
		PUSH	EAX
		JMP		dword ptr[ReturnAfterPrepareMsgPartyE]
	}
}



bool SendUpdatePCName(uint32_t oChanged, uint32_t oReceiver, std::string sFirstName, std::string sLastName, bool bSetName)
{
	InitializeCELocString();

	//558000
	//GetPCBlock "changed"
	uint32_t receiverPCID = GetPCIDFromCreature(oReceiver);

	CNWSPlayerStruct* pStructChanged = GetPCBlockFromCreature(oChanged);
	NWN::OBJECTID oIdChanged;

	if (receiverPCID == NWN::PLAYERID_INVALIDID)
		return false;


	if (pStructChanged == NULL)
	{
		if (bSetName)
			oIdChanged = oChanged;
		else
			return false;

	}
	else
	{
		oIdChanged = pStructChanged->ownedCreature;
	}

	//GetCreature "changed"
	GameObjectManager m_ObjectManager;
	NWN::CGameObject *Object = m_ObjectManager.GetGameObject( (NWN::OBJECTID) oIdChanged );
	if (Object == NULL)
		return false;

	NWN::OBJECT_TYPE possessorType = Object->GetObjectType();
	if (possessorType != NWN::OBJECT_TYPE_CREATURE)
	{
		return false;
	}

	int* ptrMsg = GetCNWSMessage();

	bool bResult = true;

	fakeCELocString[1] = (uint32_t)sFirstName.data();
	fakeCELocString[2] = std::size(sFirstName) + 1;


	fakeLocalizedString2[1] = (uint32_t)ptrFakeLString2;
	fakeLocalizedString2[2] = 1;
	fakeLocalizedString2[3] = 1;

	fakeCELocString2[1] = (uint32_t)sLastName.data();
	fakeCELocString2[2] = std::size(sLastName) + 1;

	if (bSetName)
	{
		InitMessageBlock(ptrMsg, 0x400, receiverPCID, 1);
		AddX8ToMsg('U', (unsigned char*)ptrMsg);
		AddX8ToMsg(5, (unsigned char*)ptrMsg);
		AddX32ToMsg(oIdChanged | 0x80000000, (unsigned char*)ptrMsg);
		AddX32ToMsg(0x100, (unsigned char*)ptrMsg);

		AddLocalizedNameToMsg(ptrMsg, fakeLocalizedString, 0);
		AddLocalizedNameToMsg(ptrMsg, fakeLocalizedString2, 0);

		uint8_t* MsgData;
		uint32_t Size;
		PrepareEndOfMsg((void*)ptrMsg, (void*)&MsgData, (void*)&Size);

		MsgData[0] = 0x50;
		if(bSetName) {
			MsgData[1] = 0x5;
			MsgData[2] = 0x1;
		}
		else {
			MsgData[1] = 0xA;
			MsgData[2] = 0x2;
		}


		bResult = PrepaSendMessageToPlayer(receiverPCID, MsgData, Size, 0);
	}
	else
	{
		//Update creature firstname/lastname
		uint32_t uBaseFirstName[4] = {0};
		uint32_t uBaseLastName[4] = {0};
		memcpy(uBaseFirstName, (void*)(((uint32_t)Object) + 0x2d4), 4 * 4);
		memcpy(uBaseLastName, (void*)(((uint32_t)Object) + 0x2e4), 4 * 4);

		memcpy((void*)(((uint32_t)Object) + 0x2d4), fakeLocalizedString, 4 * 4);
		memcpy((void*)(((uint32_t)Object) + 0x2e4), fakeLocalizedString2, 4 * 4);

		//Ok, now call the function
		bResult = SendServerToPlayerList_Add(ptrMsg, NULL, receiverPCID, pStructChanged);


		//restore creature firstname/lastname

		memcpy((void*)(((uint32_t)Object) + 0x2d4), uBaseFirstName, 4 * 4);
		memcpy((void*)(((uint32_t)Object) + 0x2e4), uBaseLastName, 4 * 4);
	}

	return bResult;
}



Patch _PatchUseUserNameOnOOCMsg[] =
{
	Patch(HOOK_PrepareMsgPartyE, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6),
	Patch(HOOK_PrepareMsgPartyE + 1, (relativefunc)PatchUserNameForMsgPartyE),

	Patch(HOOK_PrepareGUIMsgParty, (char*)"\xe9\x00\x00\x00\x00", (int)5),
	Patch(HOOK_PrepareGUIMsgParty + 1, (relativefunc)PatchUserNameForGUIParty),

	Patch()
};
Patch* PatchUseUserNameOnOOCMsg = _PatchUseUserNameOnOOCMsg;



void PatchUserNameForOOCMsg()
{
	InitializeCELocString();

	int i = 0;
	while (PatchUseUserNameOnOOCMsg[i].Apply()) {
		i++;
	}
}

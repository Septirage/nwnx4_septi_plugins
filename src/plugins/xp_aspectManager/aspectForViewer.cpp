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
#include <unordered_map>







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

#define OFFS_USERNAME_SHOUT	0x0056395b
#define OFFS_USERNAME_PARTY	0x005634de
#define OFFS_USERNAME_TELL	0x005637de
#define OFFS_USERNAME_PARTYINVITE	0x005670be

#define OFFS_MSGTALK_DM			0x0056b188
#define OFFS_MSGWHISPER_DM		0x0056b2d8



unsigned long ReturnToMsgPartyx14x15 = 0x00631a9d;
unsigned long ReturnToPrepareMsgPartyE = 0x005772d7;
unsigned long ReturnAfterPrepareMsgPartyE = 0x005772ee;



unsigned long ReturnToPrepareMsgShout = 0x00563961;
unsigned long ReturnAfterPrepareMsgShout = 0x00563a2d;

unsigned long ReturnToPrepareMsgParty = 0x005634e4;
unsigned long ReturnAfterPrepareMsgParty = 0x00563508;

unsigned long ReturnToPrepareMsgTell = 0x005637e4;
unsigned long ReturnAfterPrepareMsgTell = 0x00563808;

unsigned long ReturnToPartyInviteName = 0x005670c4;
unsigned long ReturnAfterPartyInviteName = 0x005670f9;


//Return an int (not a bool) : the asm callers test the full EAX register
int __fastcall PreparePlayerName(NWN::CNWSCreature *ptrCrea)
{
	InitializeCELocString();
	NWN::OBJECTID objID = ptrCrea->GetObjectId();


	CNWSPlayerStruct* pStructChanged = GetPCBlockFromCreature(objID);
		
	if (pStructChanged == NULL || pStructChanged->playerID > 0x60)
	{
		//Ou peut etre prendre le nom et prenom comme ca on ne s'embete meme pas apres ?
		return 0;
	}

	uint32_t playerID = pStructChanged->playerID;
	CNetBigPlayerInfo* pPlayerInfo = GetCNetPlayerInfo(playerID);

	fakeCELocString[1] = (uint32_t) (pPlayerInfo->m_sPlayerName.m_sString);
	fakeCELocString[2] = (uint32_t) (pPlayerInfo->m_sPlayerName.m_nBufferLength);

	fakeLocalizedString2[1] = 0;
	fakeLocalizedString2[2] = 0;
	fakeLocalizedString2[3] = 0;

	return 1;
}

NWN::CExoString* __fastcall PreparePlayerFullName(NWN::CNWSCreatureStatsCore* ptrCreaStat, void* UNUSED, NWN::CExoString* ptrRes)
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
		MOV		EAX, [fakeLocalizedString2]
		JMP		dword ptr[ReturnAfterPrepareMsgPartyE]
	}
}

__declspec(naked) void UserNameForShout()
{
	__asm
	{
		MOV		ECX, EDI
		PUSH	EDX
		CALL	PreparePlayerName
		POP		EDX

		TEST	EAX, EAX
		JNZ		GotoSetFalseLocStrShout

		MOV		EAX, dword ptr[EDX + 0x148]
		JMP		dword ptr[ReturnToPrepareMsgShout]

	GotoSetFalseLocStrShout:

		PUSH	0
		MOV		EAX, [fakeLocalizedString]
		PUSH	EAX
		MOV		ECX, ESI
		MOV		EAX, 0x005b7010
		CALL	EAX

		PUSH	0
		MOV		EAX, [fakeLocalizedString2]
		PUSH	EAX
		MOV		ECX, ESI
		MOV		EAX, 0x005b7010
		CALL	EAX
		
		JMP		dword ptr[ReturnAfterPrepareMsgShout]
	}
}

__declspec(naked) void UserNameForParty()
{
	__asm
	{
		MOV		ECX, EDI
		PUSH	EDX
		CALL	PreparePlayerName
		POP		EDX

		TEST	EAX, EAX
		JNZ		GotoSetFalseLocStrParty

		MOV		EAX, dword ptr[EDX + 0x148]
		JMP		dword ptr[ReturnToPrepareMsgParty]

	GotoSetFalseLocStrParty:

		PUSH	0
		MOV		EAX, [fakeLocalizedString]
		PUSH	EAX
		MOV		ECX, ESI
		MOV		EAX, 0x005b7010
		CALL	EAX

		PUSH	0
		MOV		EAX, [fakeLocalizedString2]
		PUSH	EAX
		MOV		ECX, ESI
		MOV		EAX, 0x005b7010
		CALL	EAX

		JMP		dword ptr[ReturnAfterPrepareMsgParty]
	}
}

__declspec(naked) void UserNameForTell()
{
	__asm
	{
		MOV		ECX, EDI
		PUSH	EDX
		CALL	PreparePlayerName
		POP		EDX

		TEST	EAX, EAX
		JNZ		GotoSetFalseLocStrTell

		MOV		EAX, dword ptr[EDX + 0x148]
		JMP		dword ptr[ReturnToPrepareMsgTell]

	GotoSetFalseLocStrTell:

		PUSH	0
		MOV		EAX, [fakeLocalizedString]
		PUSH	EAX
		MOV		ECX, ESI
		MOV		EAX, 0x005b7010
		CALL	EAX

		PUSH	0
		MOV		EAX, [fakeLocalizedString2]
		PUSH	EAX
		MOV		ECX, ESI
		MOV		EAX, 0x005b7010
		CALL	EAX

		JMP		dword ptr[ReturnAfterPrepareMsgTell]
	}
}


//Party invitation stored in the GameObjUpdate of the invited creature (flag 0x40000).
//EBP = inviter creature, ESI = message. The "PUSH 0" (2nd param of the first AddLocalizedNameToMsg) is already done.
__declspec(naked) void UserNameForPartyInvite()
{
	__asm
	{
		MOV		ECX, EBP
		CALL	PreparePlayerName

		TEST	EAX, EAX
		JNZ		GotoSetFalseLocStrInvite

		MOV		ECX, dword ptr[EBP + 0x1FC4]
		JMP		dword ptr[ReturnToPartyInviteName]

	GotoSetFalseLocStrInvite:

		MOV		EAX, [fakeLocalizedString]
		PUSH	EAX
		MOV		ECX, ESI
		MOV		EAX, 0x005b7010
		CALL	EAX

		PUSH	0
		MOV		EAX, [fakeLocalizedString2]
		PUSH	EAX

		//will do MOV ECX, ESI and CALL 0x5b7010
		JMP		dword ptr[ReturnAfterPartyInviteName]
	}
}



/******************************* Keep Name For Viewer *******************************/

struct NameForViewer
{
	std::string sFirstName;
	std::string sLastName;
};

//Owned creature of the viewer -> (Creature -> Name)
static std::unordered_map<NWN::OBJECTID, std::unordered_map<NWN::OBJECTID, NameForViewer>> namesForViewer;
static bool bKeepNameForViewer = false;


static void SetFakeNames(const std::string& sFirstName, const std::string& sLastName)
{
	fakeCELocString[1] = (uint32_t)sFirstName.data();
	fakeCELocString[2] = std::size(sFirstName) + 1;

	fakeLocalizedString[1] = (uint32_t)ptrFakeLString;
	fakeLocalizedString[2] = 1;
	fakeLocalizedString[3] = 1;

	fakeLocalizedString2[1] = (uint32_t)ptrFakeLString2;
	fakeLocalizedString2[2] = 1;
	fakeLocalizedString2[3] = 1;

	fakeCELocString2[1] = (uint32_t)sLastName.data();
	fakeCELocString2[2] = std::size(sLastName) + 1;
}


//Return 1 (and prepare fakeLocalizedString/fakeLocalizedString2) if a name is kept for this viewer
int __fastcall PrepareNameForViewer(NWN::CNWSCreature* ptrCrea, CNWSPlayerStruct* pViewer)
{
	if (namesForViewer.empty() || pViewer == NULL)
		return 0;

	auto itViewer = namesForViewer.find(pViewer->ownedCreature);
	if (itViewer == namesForViewer.end())
		return 0;

	auto itCrea = itViewer->second.find(ptrCrea->GetObjectId());
	if (itCrea == itViewer->second.end())
		return 0;

	InitializeCELocString();
	SetFakeNames(itCrea->second.sFirstName, itCrea->second.sLastName);

	return 1;
}


#define OFFS_NAMEFORVIEWER_UPDATE	0x0056583d
#define OFFS_NAMEFORVIEWER_REFRESH	0x00555a86
#define OFFS_NAMEFORVIEWER_EXAMINE	0x00567728

unsigned long ReturnToNameUpdate = 0x00565845;
unsigned long ReturnAfterNameUpdate = 0x00565869;

unsigned long ReturnToNameRefresh = 0x00555a8c;
unsigned long ReturnAfterNameRefresh = 0x00555ab0;

unsigned long ReturnToNameExamine = 0x00567730;
unsigned long ReturnAfterNameExamine = 0x00567754;


//BuildMsgPartCrea, name changed (flag 0x100). EDI = creature, ESI = message, [ESP+0x68] = receiver CNWSPlayer
__declspec(naked) void NameForViewerUpdate()
{
	__asm
	{
		MOV		EDX, dword ptr[ESP + 0x68]
		MOV		ECX, EDI
		CALL	PrepareNameForViewer

		TEST	EAX, EAX
		JNZ		GotoSetNameForViewerUpdate

		MOV		EDX, dword ptr[EDI]
		MOV		EAX, dword ptr[EDX + 0x148]
		JMP		dword ptr[ReturnToNameUpdate]

	GotoSetNameForViewerUpdate:

		PUSH	0
		MOV		EAX, [fakeLocalizedString]
		PUSH	EAX
		MOV		ECX, ESI
		MOV		EAX, 0x005b7010
		CALL	EAX

		PUSH	0
		MOV		EAX, [fakeLocalizedString2]
		PUSH	EAX
		MOV		ECX, ESI
		MOV		EAX, 0x005b7010
		CALL	EAX

		JMP		dword ptr[ReturnAfterNameUpdate]
	}
}

//RefreshCrea, full creature (flag 0x400, first appearance too). EDI = creature, ESI = message.
//Only called by BuildMsgPartCrea : [ESP+0x90] = receiver CNWSPlayer (1st param of BuildMsgPartCrea)
__declspec(naked) void NameForViewerRefresh()
{
	__asm
	{
		MOV		EDX, dword ptr[ESP + 0x90]
		MOV		ECX, EDI
		CALL	PrepareNameForViewer

		TEST	EAX, EAX
		JNZ		GotoSetNameForViewerRefresh

		MOV		ECX, dword ptr[EDI + 0x1FC4]
		JMP		dword ptr[ReturnToNameRefresh]

	GotoSetNameForViewerRefresh:

		PUSH	0
		MOV		EAX, [fakeLocalizedString]
		PUSH	EAX
		MOV		ECX, ESI
		MOV		EAX, 0x005b7010
		CALL	EAX

		PUSH	0
		MOV		EAX, [fakeLocalizedString2]
		PUSH	EAX
		MOV		ECX, ESI
		MOV		EAX, 0x005b7010
		CALL	EAX

		JMP		dword ptr[ReturnAfterNameRefresh]
	}
}

//MsgExamineGui_CreatureData. ESI = creature, EDI = message, EBP = receiver CNWSPlayer
__declspec(naked) void NameForViewerExamine()
{
	__asm
	{
		MOV		EDX, EBP
		MOV		ECX, ESI
		CALL	PrepareNameForViewer

		TEST	EAX, EAX
		JNZ		GotoSetNameForViewerExamine

		MOV		EDX, dword ptr[ESI]
		MOV		EAX, dword ptr[EDX + 0x148]
		JMP		dword ptr[ReturnToNameExamine]

	GotoSetNameForViewerExamine:

		PUSH	0
		MOV		EAX, [fakeLocalizedString]
		PUSH	EAX
		MOV		ECX, EDI
		MOV		EAX, 0x005b7010
		CALL	EAX

		PUSH	0
		MOV		EAX, [fakeLocalizedString2]
		PUSH	EAX
		MOV		ECX, EDI
		MOV		EAX, 0x005b7010
		CALL	EAX

		JMP		dword ptr[ReturnAfterNameExamine]
	}
}


//Send the name of oChanged to oReceiver.
//bRealName : send the current names of the creature, else the fake ones (already prepared)
//bSetName : creature name (GameObjUpdate) else player list name
static bool SendNameToViewer(uint32_t oChanged, uint32_t oReceiver, bool bRealName, bool bSetName)
{
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

	if (bSetName)
	{
		void* pFirstName = fakeLocalizedString;
		void* pLastName = fakeLocalizedString2;
		if (bRealName)
		{
			pFirstName = (void*)(((uint32_t)Object) + 0x2d4);
			pLastName = (void*)(((uint32_t)Object) + 0x2e4);
		}

		InitMessageBlock(ptrMsg, 0x400, receiverPCID, 1);
		AddX8ToMsg('U', (unsigned char*)ptrMsg);
		AddX8ToMsg(5, (unsigned char*)ptrMsg);
		AddX32ToMsg(oIdChanged | 0x80000000, (unsigned char*)ptrMsg);
		AddX32ToMsg(0x100, (unsigned char*)ptrMsg);

		AddLocalizedNameToMsg(ptrMsg, pFirstName, 0);
		AddLocalizedNameToMsg(ptrMsg, pLastName, 0);

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
	else if (bRealName)
	{
		bResult = SendServerToPlayerList_Add(ptrMsg, NULL, receiverPCID, pStructChanged);
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


bool SendUpdatePCName(uint32_t oChanged, uint32_t oReceiver, std::string sFirstName, std::string sLastName, bool bSetName)
{
	InitializeCELocString();
	SetFakeNames(sFirstName, sLastName);

	return SendNameToViewer(oChanged, oReceiver, false, bSetName);
}


//Keep the name of oCreature for oViewer (if the patches are active) and send it now.
//For a PC, the player list name (used by the chat) is also sent.
bool SetCreatureNameForViewer(uint32_t oCreature, uint32_t oViewer, std::string sFirstName, std::string sLastName)
{
	//Only kept for the owned character of a player (key used by the patches)
	if (bKeepNameForViewer && GetPCBlockFromCreature(oViewer) != NULL)
	{
		NameForViewer& name = namesForViewer[oViewer][oCreature];
		name.sFirstName = sFirstName;
		name.sLastName = sLastName;
	}

	bool bResult = SendUpdatePCName(oCreature, oViewer, sFirstName, sLastName, true);

	if (GetPCBlockFromCreature(oCreature) != NULL)
		bResult = SendUpdatePCName(oCreature, oViewer, sFirstName, sLastName, false) && bResult;

	return bResult;
}


//Forget the kept name of oCreature for oViewer and send back the real name.
bool ClearCreatureNameForViewer(uint32_t oCreature, uint32_t oViewer)
{
	auto itViewer = namesForViewer.find(oViewer);
	if (itViewer != namesForViewer.end())
	{
		itViewer->second.erase(oCreature);
		if (itViewer->second.empty())
			namesForViewer.erase(itViewer);
	}

	bool bResult = SendNameToViewer(oCreature, oViewer, true, true);

	if (GetPCBlockFromCreature(oCreature) != NULL)
		bResult = SendNameToViewer(oCreature, oViewer, true, false) && bResult;

	return bResult;
}


//Forget everything about oCreature : as a viewer and as a viewed creature. Nothing is sent.
void ForgetCreatureNames(uint32_t oCreature)
{
	namesForViewer.erase(oCreature);

	for (auto itViewer = namesForViewer.begin(); itViewer != namesForViewer.end();)
	{
		itViewer->second.erase(oCreature);
		if (itViewer->second.empty())
			itViewer = namesForViewer.erase(itViewer);
		else
			++itViewer;
	}
}



Patch _PatchUseUserNameOnOOCMsg[] =
{
	Patch(HOOK_PrepareMsgPartyE, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6),
	Patch(HOOK_PrepareMsgPartyE + 1, (relativefunc)PatchUserNameForMsgPartyE),

	Patch(HOOK_PrepareGUIMsgParty, (char*)"\xe9\x00\x00\x00\x00", (int)5),
	Patch(HOOK_PrepareGUIMsgParty + 1, (relativefunc)PatchUserNameForGUIParty),

	Patch(OFFS_MSGTALK_DM, (char*)"\x90\x90", (int)2),
	Patch(OFFS_MSGWHISPER_DM, (char*)"\x90\x90", (int)2),

	Patch(OFFS_USERNAME_SHOUT, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6),
	Patch(OFFS_USERNAME_SHOUT+1, (relativefunc)UserNameForShout),

	Patch(OFFS_USERNAME_PARTY, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6),
	Patch(OFFS_USERNAME_PARTY+1, (relativefunc)UserNameForParty),

	Patch(OFFS_USERNAME_TELL, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6),
	Patch(OFFS_USERNAME_TELL+1, (relativefunc)UserNameForTell),

	Patch(OFFS_USERNAME_PARTYINVITE, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6),
	Patch(OFFS_USERNAME_PARTYINVITE+1, (relativefunc)UserNameForPartyInvite),

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



Patch _PatchKeepNameForViewer[] =
{
	Patch(OFFS_NAMEFORVIEWER_UPDATE, (char*)"\xe9\x00\x00\x00\x00\x90\x90\x90", (int)8),
	Patch(OFFS_NAMEFORVIEWER_UPDATE+1, (relativefunc)NameForViewerUpdate),

	Patch(OFFS_NAMEFORVIEWER_REFRESH, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6),
	Patch(OFFS_NAMEFORVIEWER_REFRESH+1, (relativefunc)NameForViewerRefresh),

	Patch(OFFS_NAMEFORVIEWER_EXAMINE, (char*)"\xe9\x00\x00\x00\x00\x90\x90\x90", (int)8),
	Patch(OFFS_NAMEFORVIEWER_EXAMINE+1, (relativefunc)NameForViewerExamine),

	Patch()
};
Patch* PatchKeepNameForViewerList = _PatchKeepNameForViewer;



void PatchKeepNameForViewer()
{
	InitializeCELocString();

	int i = 0;
	while (PatchKeepNameForViewerList[i].Apply()) {
		i++;
	}

	bKeepNameForViewer = true;
}

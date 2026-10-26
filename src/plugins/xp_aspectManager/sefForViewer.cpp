#include "sefForViewer.h"
#include "aspectForViewer.h"
#include "messageManagement.h"

#include <misc/Patch.h>
#include <misc/log.h>
#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include "../../septutil/NwN2DataPos.h"
#include "../../septutil/NwN2Utilities.h"
#include "../../septutil/NwN2Message.h"

#include <algorithm>
#include <cstring>
#include <utility>
#include <vector>

/*
SEF visible by a single player (the viewer).

Unlike the names, the engine never writes a SEF list when the client creates an object: we can't insert ours in its
message, we have to send our own one (GameObjUpdate with only the SEF list, flag 0x40).
- Set : we keep the SEF, and send it if the client knows the object.
- Clear : we send its 'D' if the client knows the object, and forget it.
- The client forgets the SEFs when it recreates an object: we send them again, right after the GameObjUpdate that
  creates the object (the new LUO only queues the object, the wrapper of the GameObjUpdate sends).
  Each resend is 'D' old id + 'A' new id: an id is never used twice (the client doesn't check duplicates).
*/

struct SEFForViewerEntry
{
	uint32_t      uHandle;	//id given to the script
	NWN::PLAYERID uViewer;
	NWN::OBJECTID oObject;
	std::string   sName;
	NWN::OBJECTID oTarget;
	float         fPos[3];	//target position
	uint32_t      uSentId;	//network id of the last 'A' sent, 0 = never sent
};

static std::vector<SEFForViewerEntry> sefEntries;
//(viewer, object) created in the GameObjUpdate being built, to send once it's sent
static std::vector<std::pair<NWN::PLAYERID, NWN::OBJECTID>> sefToResend;
//Player whose GameObjUpdate is being built
static CNWSPlayerStruct* pUpdatingPlayer = NULL;

static uint32_t lastHandle = 0;
//Server SEF ids go up from 0x80000000, client ones are 1..0x7FFFFFFE : ours go down from 0xFFFFFFFF
static uint32_t nextSEFId = 0xFFFFFFFF;

static bool bSEFForViewer = false;

#define UPDATE_OBJECT_SEF_FLAG	0x40
#define MAX_SEF_NAME_LENGTH		32


/******************************* Tools *******************************/

//Creature, item, placeable or door
static NWN::CGameObject* GetObjectForSEF(NWN::OBJECTID oObject)
{
	GameObjectManager m_ObjectManager;
	NWN::CGameObject* Object = m_ObjectManager.GetGameObject(oObject);
	if (Object == NULL)
		return NULL;

	NWN::OBJECT_TYPE type = Object->GetObjectType();
	if (type == NWN::OBJECT_TYPE_CREATURE || type == NWN::OBJECT_TYPE_ITEM
		|| type == NWN::OBJECT_TYPE_PLACEABLE || type == NWN::OBJECT_TYPE_DOOR)
		return Object;

	return NULL;
}

//Only if the client knows the object (else it misreads the message) and the player isn't in transition
//(xp_bugfix drops the GameObjUpdate during an area load)
static bool CanSendNow(CNWSPlayerStruct* pPlayer, NWN::OBJECTID oObject)
{
	if (pPlayer == NULL || pPlayer->controlledCreature == NWN::INVALIDOBJID)
		return false;

	return IsKnowByPlayer(pPlayer->playerID, oObject);
}


/******************************* Message *******************************/

//One line of the SEF list: 'A' (add) or 'D' (delete, only the id is used)
static void AddSEFLineToMsg(int* ptrMsg, char cOperation, const std::string& sName, NWN::OBJECTID oTarget,
                            const float* fPos, uint32_t uSEFId)
{
	AddX8ToMsg(cOperation, (unsigned char*)ptrMsg);

	NWN::CExoString esName;
	esName.m_sString       = const_cast<char*>(sName.c_str());
	esName.m_nBufferLength = (ULONG)(sName.size() + 1);
	AddCExoString(&esName, ptrMsg);

	AddX32ToMsg((oTarget == NWN::INVALIDOBJID) ? NWN::INVALIDOBJID : (oTarget | 0x80000000), (unsigned char*)ptrMsg);

	// here i copy the position
	for (int k = 0; k < 3; k++)
	{
		uint32_t uBits;
		memcpy(&uBits, &fPos[k], sizeof(uBits));
		AddX32ToMsg(uBits, (unsigned char*)ptrMsg);
	}

	AddX32ToMsg(uSEFId, (unsigned char*)ptrMsg);
}

//GameObjUpdate 'U' with only the SEF list (flag 0x40), as written by BuildMsgPartCrea (0x565760).
//uDeleteId : id to delete first (0 = none, a 'D' of an id the client doesn't know is harmless). pAdd : SEF to add (NULL = none)
static void SendSEFUpdate(NWN::PLAYERID uPlayer, NWN::CGameObject* Object, uint32_t uDeleteId, const SEFForViewerEntry* pAdd)
{
	static const std::string emptyName;
	static const float zeroPos[3] = { 0.0f, 0.0f, 0.0f };

	int* ptrMsg = GetCNWSMessage();
	if (ptrMsg == NULL)
		return;

	InitMessageBlock(ptrMsg, 0x400, uPlayer, 1);
	AddX8ToMsg('U', (unsigned char*)ptrMsg);
	AddX8ToMsg(*((uint8_t*)Object + AmObjectType), (unsigned char*)ptrMsg);
	AddX32ToMsg(Object->GetObjectId() | 0x80000000, (unsigned char*)ptrMsg);
	AddX32ToMsg(UPDATE_OBJECT_SEF_FLAG, (unsigned char*)ptrMsg);

	AddX32ToMsg((uDeleteId != 0 ? 1 : 0) + (pAdd != NULL ? 1 : 0), (unsigned char*)ptrMsg);
	if (uDeleteId != 0)
		AddSEFLineToMsg(ptrMsg, 'D', emptyName, NWN::INVALIDOBJID, zeroPos, uDeleteId);
	if (pAdd != NULL)
		AddSEFLineToMsg(ptrMsg, 'A', pAdd->sName, pAdd->oTarget, pAdd->fPos, pAdd->uSentId);

	//After the list the client always sets the AppearanceSEF of the object: send the real one or it's removed
	NWN::CExoString* pAppearanceSEF = (NWN::CExoString*)((char*)Object + AmAppSEF);
	bool bAppearanceSEF = (pAppearanceSEF->m_sString != NULL && pAppearanceSEF->m_sString[0] != '\0');
	AddBoolToMsg(bAppearanceSEF, ptrMsg);
	if (bAppearanceSEF)
		AddCExoString(pAppearanceSEF, ptrMsg);

	uint8_t* MsgData;
	uint32_t Size;
	PrepareEndOfMsg((void*)ptrMsg, (void*)&MsgData, (void*)&Size);

	MsgData[0] = 0x50;
	MsgData[1] = 0x05;
	MsgData[2] = 0x01;

	PrepaSendMessageToPlayer(uPlayer, MsgData, Size, 0);
}

//Send the 'D' of a SEF we forget. If the client doesn't know the object there is nothing to do:
//it stops by itself the SEFs of an object it doesn't have anymore
static void SendDelete(const SEFForViewerEntry& entry, CNWSPlayerStruct* pPlayer)
{
	if (entry.uSentId == 0 || !CanSendNow(pPlayer, entry.oObject))
		return;

	NWN::CGameObject* Object = GetObjectForSEF(entry.oObject);
	if (Object != NULL)
		SendSEFUpdate(entry.uViewer, Object, entry.uSentId, NULL);
}


/******************************* Hooks *******************************/

//Send again the SEFs of the objects created for this player in the GameObjUpdate that has just been sent
static void SendQueued(CNWSPlayerStruct* pPlayer)
{
	NWN::PLAYERID uPlayer = pPlayer->playerID;

	std::vector<NWN::OBJECTID> objects;
	for (auto& queued : sefToResend)
	{
		if (queued.first == uPlayer && std::find(objects.begin(), objects.end(), queued.second) == objects.end())
			objects.push_back(queued.second);
	}
	std::erase_if(sefToResend, [&](const auto& queued) { return queued.first == uPlayer; });

	for (NWN::OBJECTID oObject : objects)
	{
		//In transition: the objects will be created again after the area load
		NWN::CGameObject* Object = GetObjectForSEF(oObject);
		if (Object == NULL || !CanSendNow(pPlayer, oObject))
			continue;

		for (SEFForViewerEntry& entry : sefEntries)
		{
			if (entry.uViewer != uPlayer || entry.oObject != oObject)
				continue;

			//'D' of the old id: if the client reused the object, the old instance is still there
			uint32_t uOldId = entry.uSentId;
			entry.uSentId = nextSEFId--;
			SendSEFUpdate(uPlayer, Object, uOldId, &entry);
		}
	}
}

//Branch "new LUO" of TestObjectUpdateDifferences: the client of pPlayer will create pObject
void __fastcall OnNewLUO(CNWSPlayerStruct* pPlayer, NWN::CGameObject* pObject)
{
	//New LUO created outside of a GameObjUpdate are never sent (script command 0x6a8ff0)
	if (sefEntries.empty() || pPlayer == NULL || pPlayer != pUpdatingPlayer)
		return;

	NWN::OBJECTID oObject = pObject->GetObjectId();
	for (const SEFForViewerEntry& entry : sefEntries)
	{
		if (entry.uViewer == pPlayer->playerID && entry.oObject == oObject)
		{
			sefToResend.push_back({ pPlayer->playerID, oObject });
			return;
		}
	}
}

#define OFFS_NEWLUO_HOOK			0x0056e9b3	//after the call to CreateNewLastUpdateObject (0x56df10, hooked by xp_bugfix)
#define OFFS_GAMEOBJUPDATE_CALL		0x0056f25a	//call SendServerToPlayerGameObjUpdate (0x56ee80) in 0x56f250
#define FUNC_SENDGAMEOBJUPDATE		0x0056ee80

unsigned long ReturnAfterNewLUO = 0x0056e9b8;

//EAX = new LUO, EBP = CNWSPlayer, ESI = object, EDI = &LUO. Replaces: mov [edi],eax / pop edi / pop esi / pop ebp
__declspec(naked) void NewLUOHook()
{
	__asm
	{
		MOV		dword ptr[EDI], EAX
		PUSH	EAX
		MOV		ECX, EBP
		MOV		EDX, ESI
		CALL	OnNewLUO
		POP		EAX
		POP		EDI
		POP		ESI
		POP		EBP
		JMP		dword ptr[ReturnAfterNewLUO]
	}
}

typedef int (__thiscall *SendGameObjUpdate_t)(void* pMsg, CNWSPlayerStruct* pPlayer, NWN::OBJECTID oFilter);
static SendGameObjUpdate_t const SendGameObjUpdate = (SendGameObjUpdate_t)FUNC_SENDGAMEOBJUPDATE;

//Replaces the call to the thiscall SendServerToPlayerGameObjUpdate(pPlayer, oFilter) (ret 8)
int __fastcall GameObjUpdateWrapper(void* pMsg, void* /*edx*/, CNWSPlayerStruct* pPlayer, NWN::OBJECTID oFilter)
{
	pUpdatingPlayer = pPlayer;
	int iResult = SendGameObjUpdate(pMsg, pPlayer, oFilter);
	pUpdatingPlayer = NULL;

	if (pPlayer != NULL && !sefToResend.empty())
		SendQueued(pPlayer);

	return iResult;
}

Patch _PatchSEFForViewer[] =
{
	Patch(OFFS_NEWLUO_HOOK, (char*)"\xe9\x00\x00\x00\x00", (int)5),
	Patch(OFFS_NEWLUO_HOOK + 1, (relativefunc)NewLUOHook),

	Patch(OFFS_GAMEOBJUPDATE_CALL + 1, (relativefunc)GameObjUpdateWrapper),

	Patch()
};
Patch* PatchSEFForViewerList = _PatchSEFForViewer;


/******************************* Disconnection *******************************/

typedef void (__stdcall *OnPlayerConnectionCloseProc)(unsigned long PlayerId, void* Context);
typedef BOOL (__stdcall *OnPlayerConnectionReceiveProc)(unsigned long PlayerId, const unsigned char* Data, size_t Length, void* Context);
typedef BOOL (__stdcall *OnPlayerConnectionSendProc)(unsigned long PlayerId, unsigned char* Data, unsigned long Size, unsigned long Flags, void* Context);
typedef void (__stdcall *SetPacketFilterCallouts_t)(void* Context, OnPlayerConnectionCloseProc OnClose,
	OnPlayerConnectionReceiveProc OnReceive, OnPlayerConnectionSendProc OnSend);

//The player left: forget what it saw. Nothing to send, its client is gone (and its PlayerId will be reused)
void __stdcall OnViewerConnectionClose(unsigned long PlayerId, void* /*Context*/)
{
	std::erase_if(sefEntries, [&](const SEFForViewerEntry& entry) { return entry.uViewer == PlayerId; });
	std::erase_if(sefToResend, [&](const auto& queued) { return queued.first == PlayerId; });
	ForgetNamesForPlayer(PlayerId);
}

bool InitViewerCleanup(LogNWNX* pLogger)
{
	HMODULE XPBugfix = LoadLibraryA("xp_bugfix.dll");
	if (!XPBugfix)
	{
		pLogger->Info("* ViewerCleanup: failed to load xp_bugfix.dll");
		return false;
	}

	SetPacketFilterCallouts_t SetPacketFilterCallouts_ = (SetPacketFilterCallouts_t)GetProcAddress(XPBugfix, "SetPacketFilterCallouts");
	if (!SetPacketFilterCallouts_)
	{
		pLogger->Info("* ViewerCleanup: unable to resolve xp_bugfix!SetPacketFilterCallouts");
		return false;
	}

	SetPacketFilterCallouts_(nullptr, OnViewerConnectionClose, nullptr, nullptr);
	return true;
}


/******************************* API *******************************/

bool InitSEFForViewer()
{
	int i = 0;
	while (PatchSEFForViewerList[i].Apply()) {
		i++;
	}

	bSEFForViewer = true;
	return true;
}

uint32_t SetSEFForViewer(uint32_t oObject, uint32_t oViewer, const std::string& sSEF,
                         uint32_t oTarget, float fX, float fY, float fZ)
{
	if (!bSEFForViewer || sSEF.empty() || sSEF.size() > MAX_SEF_NAME_LENGTH)
		return 0;

	NWN::CGameObject* Object = GetObjectForSEF((NWN::OBJECTID)oObject);
	if (Object == NULL)
		return 0;

	NWN::PLAYERID uViewer = GetPCIDFromCreature(oViewer);
	if (uViewer == NWN::PLAYERID_INVALIDID)
		return 0;

	SEFForViewerEntry entry;
	entry.uHandle = ++lastHandle;
	entry.uViewer = uViewer;
	entry.oObject = (NWN::OBJECTID)oObject;
	entry.sName   = sSEF;
	entry.oTarget = (NWN::OBJECTID)oTarget;
	entry.fPos[0] = fX;
	entry.fPos[1] = fY;
	entry.fPos[2] = fZ;
	entry.uSentId = 0;

	if (CanSendNow(GetPCBlockFromPlayerId(uViewer), entry.oObject))
	{
		entry.uSentId = nextSEFId--;
		SendSEFUpdate(uViewer, Object, 0, &entry);
	}

	sefEntries.push_back(entry);
	return entry.uHandle;
}

void ClearSEFForViewer(uint32_t oObject, uint32_t oViewer, const std::string& sSEF)
{
	NWN::PLAYERID uViewer = GetPCIDFromCreature(oViewer);
	if (uViewer == NWN::PLAYERID_INVALIDID)
		return;

	CNWSPlayerStruct* pPlayer = GetPCBlockFromPlayerId(uViewer);

	for (auto it = sefEntries.begin(); it != sefEntries.end();)
	{
		if (it->uViewer == uViewer && it->oObject == (NWN::OBJECTID)oObject
			&& _stricmp(it->sName.c_str(), sSEF.c_str()) == 0)
		{
			SendDelete(*it, pPlayer);
			it = sefEntries.erase(it);
		}
		else
			++it;
	}
}

void ClearSEFForViewerById(uint32_t uHandle)
{
	for (auto it = sefEntries.begin(); it != sefEntries.end(); ++it)
	{
		if (it->uHandle == uHandle)
		{
			SendDelete(*it, GetPCBlockFromPlayerId(it->uViewer));
			sefEntries.erase(it);
			return;
		}
	}
}

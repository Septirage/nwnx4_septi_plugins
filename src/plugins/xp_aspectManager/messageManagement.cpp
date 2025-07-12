
#pragma warning(disable : 4828)
#include "messageManagement.h"
#include "aspectManagerUtils.h"

#include "../../NWN2Lib/NWN2.h"
#include "../../NWN2Lib/NWN2Common.h"
#include "../../misc/log.h"
#include "nwn2heap.h"
#include "../../septutil/NwN2DataPos.h"
#include "..\..\misc\Patch.h"


#include <string>


#define FUNC_PREPAREMSG			0x0056df15
#define FUNC_CREATE55PART		0x00565760
#define FUNC_ENDPART			0x0056ded0
#define FUNC_FINISHPREPARATION	0x0074E080

#define OFFSET_UTILITY_PTR_PC 0x0086442C

#define FUNC_CALCULATEAC	  0x005e01d0
#define FUNC_CALCSTRANGE	  0x005d68b0
#define FUNC_BUILDVISUAL	  0x0074fec0
#define FUNC_BUILDPARTIENOM	  0x00550EF0

#define FUNC_GETBASEITMFROMID 0x00750b20

#define FUNC_PREPAREENDOFMSG  0x0074e080


#define FUNC_INITMSGBLOCK	0x74ed00
#define FUNC_BUILDALLVISUAL 0x551090
#define FUNC_UPDATEMSGHASH  0x74e530
#define FUNC_BUILDMSGITEM   0x551090



void AddX8ToMsg(uint8_t uData, unsigned char* msgBlock)
{
	uint32_t* accessMBlock = (uint32_t*)msgBlock;
	uint8_t* msgData       = (uint8_t*)(accessMBlock[1]);
	msgData[accessMBlock[3]] = uData;
	accessMBlock[3]++;
	accessMBlock[7]++;
}

void AddX32ToMsg(uint32_t uData, unsigned char* msgBlock)
{
	uint32_t* accessMBlock = (uint32_t*)msgBlock;
	uint8_t* msgData = (uint8_t*)(accessMBlock[1]);
	*(uint32_t*)(msgData + accessMBlock[3]) = uData;
	accessMBlock[3] += 4;
	accessMBlock[7] += 4;
}

__declspec(naked) void __fastcall InitMessageBlock(__in void* MsgCreator, __in void* Unused, __in uint32_t uSize, __in uint32_t param1, __in uint32_t param2)
{
	__asm
	{
		mov		edx, FUNC_INITMSGBLOCK;
		jmp		edx;
	}
}



//OFFS_g_pAppManager


HMODULE XPBugfix; 

typedef
BOOL
(__stdcall *
	SendMessageToPlayer)(
		__in unsigned long PlayerId,
		__in_bcount(Size) unsigned char* Data,
		__in unsigned long Size,
		__in unsigned long Flags
	);

SendMessageToPlayer     SendMessageToPlayer_;

LogNWNX* msgLogger = NULL;



static unsigned char MsgCreation[0x58];
static unsigned char UnknowMsg[0x80];


void setLoggerMsg(LogNWNX* myLogger)
{
	msgLogger = myLogger;
}



void prepareMsgBlock(unsigned char* DataDest, uint32_t sizeOfDataDest, uint32_t uSize, uint32_t param1, uint32_t param2)
{
	memset(MsgCreation, 0, sizeof(MsgCreation));
	*(uint32_t*)(MsgCreation)        = 0x008042C0;
	*(uint32_t*)(MsgCreation + 0x50) = 0xffffffff;
	*(uint32_t*)(MsgCreation + 0x54) = 0x7f000000;

	memset(UnknowMsg, 0, sizeof(UnknowMsg));

	// preparation of Visual
	*(uint32_t*)(MsgCreation + 0x8)        = sizeOfDataDest;
	*(unsigned char**)(MsgCreation + 0x4)  = DataDest;
	*(uint32_t*)(MsgCreation + 0x14)       = 0x80;
	*(unsigned char**)(MsgCreation + 0x10) = UnknowMsg;

	InitMessageBlock(MsgCreation, NULL, uSize, param1, param2);
}

bool PrepaSendMessageToPlayer(unsigned long PlayerId, unsigned char* Data, unsigned long Size, unsigned long Flags)
{
	static bool bOk = false;
	if (!bOk)
	{
		bOk = LoadNetLayer(msgLogger);
	}
	if (bOk)
	{
		return SendMessageToPlayer_(PlayerId, Data, Size, Flags);
	}
	else
	{
		return false;
	}
}

__declspec(naked) uint8_t* __fastcall GetBaseItemFromId_(__in void* BaseItmId, __in void* Unused, __in int item_id)
{
	__asm
	{
		mov		edx, FUNC_GETBASEITMFROMID;
		jmp		edx;
	}
}

uint8_t* GetBaseItemFromId(int item_id)
{
	uint8_t* baseItmId = *(uint8_t**)0x86443C;
	baseItmId = *(uint8_t**)(baseItmId + 0x8C);

	return GetBaseItemFromId_((void*)baseItmId, NULL, item_id);
}

__declspec(naked) void __fastcall CreateObjectAppearance(__in void* MsgCreator, __in void* Unused, void* ItmPtr)
{
	__asm
	{
		mov		edx, FUNC_BUILDALLVISUAL;
		jmp		edx;
	}
}

__declspec(naked) void __fastcall BuildMsgItem(__in void* MsgCreator, __in void* Unused, void* ItemPtr)
{
	__asm
	{
		mov		edx, FUNC_BUILDMSGITEM;
		jmp		edx;
	}
}

__declspec(naked) void __fastcall BuildMsgNamePart(__in void* MsgCreator, __in void* Unused, __in void* ItmPtr, __in void* CreaPtr)
{
	__asm
	{
		mov		edx, FUNC_BUILDPARTIENOM;
		jmp		edx;
	}
}

__declspec(naked) NWN::CNWSPlayer * __fastcall GetClientObjectByPlayerId(__in struct NWN::CServerExoApp * This, __in void * Unused, __in NWN::PLAYERID PlayerId, __in UCHAR ExpansionMask)
{
	__asm
	{
		mov     edx, OFFS_CServerExoApp_GetClientObjectByPlayerId ; set jump target
		jmp     edx ; branch to target procedure
	}
}

__declspec(naked) int __fastcall CalculAndReturnAC(void* pCreature)
{
	__asm
	{
		mov		edx, FUNC_CALCULATEAC;
		jmp		edx;
	}
}

__declspec(naked) void __fastcall BuildChestVisual(uint8_t* Visual, uint8_t* Unused, uint8_t* MsgCreation)
{
	__asm
	{
		mov edx, FUNC_BUILDVISUAL;
		jmp edx;
	}
}

__declspec(naked) void __fastcall PrepareEndOfMsg(__in void* pMsgCreator, __in void* Unused, __in void* pData, __in void* pSize)
{
	__asm
	{
		mov		edx, FUNC_PREPAREENDOFMSG;
		jmp		edx;
	}
}

/**************************************************/

__declspec(naked) int* __fastcall FonctionPrepare(void *messageBlock, void* Unused, int unknowBlock,int *ObjectUpdated,uint32_t *param_3,void *param_4)
{
	__asm
	{
		push	ecx;
		push	ebx;
		push	ebp;
		push	esi;
		push	edi;


		mov		edx, FUNC_PREPAREMSG;
		jmp		edx;
	}
}

__declspec(naked) void __fastcall  CreateMessage55Part(void* messageBlock, void* Unused, int* param_1, int* param_2, uint32_t* param_3, uint32_t param_4, uint32_t param_5)
{
	__asm
	{
		mov		edx, FUNC_CREATE55PART;
		jmp		edx;
	}
}

__declspec(naked) void __fastcall  EndMessagePart(void* messageBlock, void* Unused, int* param_1, uint32_t* monRet, int* ObjectToRefr, uint32_t flagsToSet, uint32_t unknowField)
{
	__asm
	{
		mov		edx, FUNC_ENDPART;
		jmp		edx;
	}
}

__declspec(naked) void __fastcall FinishMessagePreparation(void* messageBlock, void* Unused, void* ptrToMsg, void* sizeToSend) {
	__asm
	{
		mov edx, FUNC_FINISHPREPARATION;
		jmp edx;
	}
}

__declspec(naked) void __fastcall UpdateMsgHash(void* messageBlock, void* Unused, uint32_t p1, uint32_t p2, uint32_t p3)
{
	__asm
	{
		mov edx, FUNC_UPDATEMSGHASH;
		jmp edx;
	}
}

Patch _tempPatch[] =
{
	//Patch that will avoid call of some unwanted extra work when we start to prepare refreshObject message
	Patch((DWORD)0x0056e6d0, (char*)"\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", (int)13), //NOP*13
};

Patch *tempPatch = _tempPatch;


namespace NWN
{
	NWN::CAppManager *& g_pAppManager = *(NWN::CAppManager **) OFFS_g_pAppManager;
}


int GetModelTypeFromItemID(int item_id)
{
	uint8_t* myBaseItem = GetBaseItemFromId(item_id);
	if (myBaseItem == 0x0)
		return -1;
	int iModelType = myBaseItem[0x1C];
	return iModelType;
}



unsigned long BuildMyChestVisual(uint8_t* DataDest, uint8_t* DataSource)
{
	memset(MsgCreation, 0, sizeof(MsgCreation));
	memset(UnknowMsg, 0, sizeof(UnknowMsg));

	//preparation of Visual

	MsgCreation[0] = 0xC0;
	MsgCreation[1] = 0x42;
	MsgCreation[2] = 0x80;

	*(unsigned char**)(MsgCreation + 0x4) = DataDest;
	*(uint32_t*)(MsgCreation + 0x8) = 0x400;

	*(unsigned char**)(MsgCreation + 0x10) = UnknowMsg;
	*(uint32_t*)(MsgCreation + 0x14) = 0x80;

	MsgCreation[0x20] = 0x03;



	BuildChestVisual(DataSource, NULL, (uint8_t*)MsgCreation);

	unsigned long iSizeArmorVisual = *(unsigned long*)((unsigned char*)MsgCreation + 0xC);

	return iSizeArmorVisual;
}

void prepareEndOfMessage(unsigned char* pMsgCreation, unsigned char* pData, unsigned long* pSize) {

	*(unsigned long*)((unsigned char*)pMsgCreation + 0x1C) = *pSize;
	*(unsigned long*)((unsigned char*)pMsgCreation + 0xC) = *pSize;
	*(unsigned char**)(pMsgCreation + 0x4) = pData;
	unsigned char pDataC;
	PrepareEndOfMsg((void*)pMsgCreation, NULL, (void*)(pMsgCreation + 0x4), (void*)pSize);
}


void BuildGIA(uint32_t idObj, NWN::CNWSItem* Item, GameObject* iAffectObject, uint32_t iSlot)
{
	AddX8ToMsg('G', MsgCreation);
	AddX8ToMsg('I', MsgCreation);
	AddX8ToMsg('A', MsgCreation);

	AddX32ToMsg(iSlot, MsgCreation);
	AddX32ToMsg(idObj | 0x80000000, MsgCreation);
	CreateObjectAppearance(MsgCreation, NULL, (void*)Item);
	BuildMsgNamePart(MsgCreation, NULL, (void*)Item, iAffectObject);
}



void BuildGRD(uint32_t idObj)
{
	AddX8ToMsg('G', MsgCreation);
	AddX8ToMsg('R', MsgCreation);
	AddX8ToMsg('D', MsgCreation);

	AddX32ToMsg(idObj | 0x80000000, MsgCreation);

	// Unexistant slot
	AddX32ToMsg(0x0F00, MsgCreation);
}


uint32_t VAR_0C0x10158[34] = { 0 };

uint32_t GetPCIntern(bool first, bool bOwnedCharacter, int varIdx)
{
	uint32_t eax = *(uint32_t*)OFFS_g_pAppManager;
	eax = *(uint32_t*)(((char*)eax) + 4);
	eax = *(uint32_t*)(((char*)eax) + 4);
	//Ici, on est bien.

	if (first)
	{
		eax = *(uint32_t*)(((char*)eax) + 0x1008c);
		eax = *(uint32_t*)(eax);

		if (eax == 0) return 0x7F000000;

		VAR_0C0x10158[varIdx] = eax;

		eax = *(uint32_t*)(((char*)eax) + 8);

		if (eax == 0) return 0x7F000000;

		if (bOwnedCharacter)
			eax = *(uint32_t*)(((char*)eax) + 0x44);
		else
			eax = *(uint32_t*)(((char*)eax) + 0x34);

		if (eax != 0x7F000000) return eax;
	}
	while (VAR_0C0x10158[varIdx] != 0)
	{
		eax = VAR_0C0x10158[varIdx];
		if(eax == 0)  return 0x7F000000;

		eax = *(uint32_t*)(((char*)eax) + 4);
		if (eax == 0)
		{
			VAR_0C0x10158[varIdx] = 0;
			return 0x7F000000;
		}

		VAR_0C0x10158[varIdx] = eax;

		eax = *(uint32_t*)(((char*)eax) + 8);
		if(eax == 0)  return 0x7F000000;

		if(bOwnedCharacter)
			eax = *(uint32_t*)(((char*)eax) + 0x44);
		else
			eax = *(uint32_t*)(((char*)eax) + 0x34);

		if(eax != 0x7F000000)  return eax;
	}
	return 0x7F000000;
	/* }
	else
	{
	if(VAR_0C0x10158 == 0) return 0x7F0000;

	eax = VAR_0C0x10158;
	eax = *(uint32_t*)(eax + 4);

	if (eax == 0)
	{
	VAR_0C0x10158 = 0;
	return 0x7F000000;
	}

	VAR_0C0x10158 = eax;

	eax = *(uint32_t*)(eax + 8);

	if(bOwnedCharacter)
	eax = *(uint32_t*)(eax + 0x44);
	else
	eax = *(uint32_t*)(eax + 0x34);


	if(eax != 0x7F000000)  return eax;


	}
	*/
}


unsigned long GetObjectToPlayerId(int iParam)
{
	NWN::CGameObject * Object;
	NWN::CNWSCreature * Creature;
	GameObjectManager m_ObjectManager;

	Object = m_ObjectManager.GetGameObject( (NWN::OBJECTID) iParam );
	if (Object == NULL)
		return NWN::PLAYERID_INVALIDID;

	Creature = Object->AsCreature( );
	if (Creature == NULL)
		return NWN::PLAYERID_INVALIDID;

	return Creature->GetControllingPlayerId();
}

bool __fastcall IsKnowByPlayer(__in NWN::PLAYERID PlayerId, __in NWN::OBJECTID ObjectId)
{
	NWN::CNWSPlayer * Player = GetClientObjectByPlayerId(NWN::g_pAppManager->m_pServerExoApp, NULL, PlayerId, 0 );
	NWN::NWN2_DynamicArrayBlittable<NWN::BugFix_LUOMap> * LUOMap;
	NWN::BugFix_LUOMap * LUOEntry;
	NWN::BugFix_LUOMap * LUOTable;
	LONG High;
	LONG Low;
	LONG Middle;

	if (Player == NULL)
		return false;

	//
	// Check for NULL first because otherwise the LUO map is not guaranteed to
	// be initialized.
	//

	if ((Player->m_pActiveObjectsLastUpdate == NULL) ||
		(Player->m_pActiveObjectsLastUpdate->m_nCount == 0))
	{
		return false;
	}

	//
	// Perform a binary search against the look up table.
	//

	LUOMap = Player->BugFix_LUOMap;
	LUOTable = LUOMap->m_Array;
	LUOEntry = NULL;

	Low = 0;
	High = LUOMap->m_nUsedSize - 1;

	while (High >= Low)
	{
		Middle = (Low + High) >> 1;
		LUOEntry = &LUOTable[ Middle ];

		if (ObjectId < LUOEntry->ObjectId)
		{
			if (Middle == 0)
			{
				return false;
			}

			High = Middle - 1;
		}
		else if (ObjectId > LUOEntry->ObjectId)
		{
			Low = Middle + 1;
		}
		else
		{
			break;
		}
	}

	if (High < Low)
	{
		return false;
	}

	PreFetchCacheLine( PF_TEMPORAL_LEVEL_1, LUOEntry->LUO );

	return (LUOEntry->LUO != NULL);
}


bool LoadNetLayer(LogNWNX* _logger)
{
	//
	// Wire up the dllimports.
	//

	_logger->Info("* Here we Load bugfix access...");
	struct { bool Required; const char *Name; void **Import; } DllImports[] =
	{
		{ true , "SendMessageToPlayer",          (void**)&SendMessageToPlayer_          },
	};
	XPBugfix = LoadLibraryA("xp_bugfix.dll");

	if (!XPBugfix)
	{
		_logger->Info("* Failed to load xp_bugfix.dll");
		return false;
	}

	for (int i = 0; i < sizeof(DllImports)/sizeof(DllImports[0]); i += 1)
	{
		*DllImports[i].Import = (void *)GetProcAddress(XPBugfix, DllImports[i].Name);

		if (!*DllImports[i].Import)
		{
			if (!DllImports[i].Required)
			{
				_logger->Info(
					"* Warning: You need to update your xp_bugfix.dll; missing optional entrypoint xp_bugfix!%s",
					DllImports[i].Name);
				continue;
			}
			_logger->Info("* Unable to resolve XPBugfix!%s", DllImports[i].Name);
			return false;
		}
	}
	_logger->Info("* ...Bugfix access Loaded");
	return true;
}


uint32_t getBeltEquiped(int iAffected)
{
	uint32_t retVal = 0;

	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;

	ObjectId = (NWN::OBJECTID) iAffected;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return 0;

	if (Object->GetObjectType() != NWN::OBJECT_TYPE::OBJECT_TYPE_CREATURE)
		return 0;

	uint32_t* objEquiped = *((uint32_t**)(((char*)Object) + AmCrtCSEquipedObjectTable));
	//we pass the 98 2F 80 00 and other object to go to belt
	
	retVal = objEquiped[11];
	if (retVal == 0x7F000000)
	{
		return 0;
	}
	else
		return retVal;
}

uint32_t getArmorEquiped(int iAffected)
{
	uint32_t retVal = 0;

	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;

	ObjectId = (NWN::OBJECTID) iAffected;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return 0;

	if (Object->GetObjectType() != NWN::OBJECT_TYPE::OBJECT_TYPE_CREATURE)
		return 0;

	uint32_t* objEquiped = *((uint32_t**)(((char*)Object) + AmCrtCSEquipedObjectTable));
	//we pass the 98 2F 80 00 and other object to go to belt

	retVal = objEquiped[2];
	if (retVal == 0x7F000000)
	{
		return 0;
	}
	else
		return retVal;
}

unsigned long createNudeApp(int iAffected, unsigned char* Data)
{
	unsigned long size = 0x12C;


	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;

	ObjectId = (NWN::OBJECTID) iAffected;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return 0x12C+5;

	if (Object->GetObjectType() != NWN::OBJECT_TYPE::OBJECT_TYPE_CREATURE)
		return 0x12C+5;

	char* objApp =  (((char*)Object) + 0x183C);

	// + 183c => 7C 2B 81 00 
	// +++++ => 4C 2A 81 00 (chapeau, puis machin puis machin)

	unsigned long iSizeNudeApp = BuildMyChestVisual((uint8_t*)Data, (uint8_t*)objApp);//(uint8_t*)objEmp-4); // - 4 avec l'objet objEmp
	return iSizeNudeApp;
}

bool ramasserObjet(int iAffected, int mustView, int iItem)
{
	GameObjectManager m_ObjectManager;
	GameObject        *Object;
	static unsigned char Data[0x35];
	memset(Data, 0, sizeof(Data));

	Object = m_ObjectManager.GetGameObject((NWN::OBJECTID)iItem);
	std::memcpy ( Data, ramasserEquipement, 0x25 );


	*(uint32_t*)(Data+0x09) = (*(uint32_t*)(((char*)Object) + 0xA0))|0x80000000;
	*((unsigned long*)(Data + 0x0F)) = iAffected|0x80000000;
	

	unsigned long iMustView = GetObjectToPlayerId(mustView);
	return PrepaSendMessageToPlayer(iMustView, Data, 0x25, 0);
}

bool removeVisual(int iInventorySlot, int iAffected, int mustView)
{
	unsigned long Size= 0;
	static unsigned char Data[0x900];
	memset(Data, 0, sizeof(Data));
	bool result = false;
	if (iInventorySlot == INVENTORY_SLOT_CHEST)
	{
		Size = 17;
		std::memcpy ( Data, removeArmor, Size );
		Size += createNudeApp(iAffected, (Data + Size));
		std::memcpy ( Data+Size, removeArmorMiddle, 10 );
		Size += 10;
		Size += createNudeApp(iAffected, (Data + Size));
		std::memcpy ( Data+Size, removeArmorEnd, 16 );
		*((unsigned long*)(Data + 0x09)) = iAffected|0x80000000;
		*((unsigned long*)(Data + Size+2)) = iAffected|0x80000000;
		Size += 16;


		*((unsigned long*)(Data + 3)) = (Size - 6);

		/*Size = 0x28D;
		std::memcpy ( Data, removeArmor, Size );
		*((unsigned long*)(Data + 0x09)) = iAffected|0x80000000;
		*((unsigned long*)(Data + 0x27F)) = iAffected|0x80000000;*/
	}
	else if (iInventorySlot == INVENTORY_SLOT_CLOAK)
	{
		Size = 0x25;
		std::memcpy ( Data, removeCloak, Size );
		*((unsigned long*)(Data + 0x09)) = iAffected|0x80000000;
		*((unsigned long*)(Data + 0x1D)) = iAffected|0x80000000;
	}
	else if (iInventorySlot == INVENTORY_SLOT_LEFTHAND || iInventorySlot == INVENTORY_SLOT_RIGHTHAND)
	{
		Size = 0x26;
		std::memcpy ( Data, removeFromHand, Size );
		*((unsigned long*)(Data + 0x09)) = iAffected|0x80000000;
		*((unsigned long*)(Data + 0x1D)) = iAffected|0x80000000;
		unsigned char val;

		if(iInventorySlot == INVENTORY_SLOT_LEFTHAND)
		{
			val = 0x20;
		}
		else
		{
			val = 0x10;
		}
		Data[0x17] = val;
	}
	else
	{
		Size = 17;
		std::memcpy ( Data, removeEquipment, Size );
		*((unsigned long*)(Data + 0x09)) = iAffected|0x80000000;


		Size += createNudeApp(iAffected, (Data + 17));

		std::memcpy(Data + Size, removeEquipmentEndPart, 24);

		uint16_t val = 1;
		if(iInventorySlot == INVENTORY_SLOT_BOOTS)
		{
			val = 0x04;
		}
		else if(iInventorySlot == INVENTORY_SLOT_ARMS)
		{
			val = 0x08;
		}
		else if(iInventorySlot == INVENTORY_SLOT_HEAD)
		{
			val = 0x01;
		}
		else if(iInventorySlot == INVENTORY_SLOT_BELT)
		{
			val = 0x400;
		}
		*(uint16_t*)(Data + Size + 6) = val;
		*((unsigned long*)(Data + Size + 0xC)) = iAffected|0x80000000;
		Size += 24;

		*((unsigned long*)(Data + 3)) = (Size - 4);
		//Data[0x148] = val;
		//*((unsigned long*)(Data + 0x14E)) = iAffected|0x80000000;
	}
	if (Size != 0)
	{
		unsigned long iMustView = GetObjectToPlayerId(mustView);
		if (iMustView != NWN::PLAYERID_INVALIDID)
		{
			unsigned char* mesData = Data;
			result = PrepaSendMessageToPlayer(iMustView, mesData, Size, 0);
		}
	}
	return result;
}

uint32_t addObjectToWorld(int mustView, GameObject* Object, uint8_t* myMessage)
{
	uint8_t* playerBlock = NULL;
	uint8_t messageBlock[0x100] = { };
	uint8_t myFiller[0x200] = {};
	uint32_t flagsToSet;

	//Find
	{
		//Find start point
		uint32_t* tempPtr = *(uint32_t**)OFFSET_UTILITY_PTR_PC;
		tempPtr = (uint32_t*)(tempPtr[1]);
		tempPtr = (uint32_t*)(tempPtr[1]);
		tempPtr = (uint32_t*)(tempPtr[0x1008c / 4]);

		uint32_t* myHeaderBloc = (uint32_t*)(tempPtr[0]); //First one
		uint32_t nbPC = tempPtr[2];


		int iCount = 0;
		while (playerBlock == NULL && iCount < nbPC) {
			uint32_t* myPCBlock = (uint32_t*)myHeaderBloc[2];
			if (myPCBlock[0xD] == mustView || myPCBlock[0x11] == mustView) {
				playerBlock = (uint8_t*)myPCBlock;
			}
			else {
				iCount++;
				myHeaderBloc = (uint32_t*)myHeaderBloc[1];
			}
		}
		//Should never occur because mustview should be an already verified pc
		if (playerBlock == NULL) {
			return 0;
		}
	}

	int playerBlock2 = (int)playerBlock;
	int unknowField = 0;

	//prepare messageBlock
	*(uint32_t*)(messageBlock) = (uint32_t)0x008042C0;
	*(uint8_t**)(messageBlock + 0x4) = myMessage;
	*(uint32_t*)(messageBlock + 0x8) = 0x800;
	*(uint32_t*)(messageBlock + 0xC) = 0x7;
	*(uint8_t**)(messageBlock + 0x10) = myFiller;
	*(uint32_t*)(messageBlock + 0x14) = 0x200;


	*(uint32_t*)(messageBlock + 0x1C) = 0x7;
	*(uint32_t*)(messageBlock + 0x20) = 0x3;

	myMessage[0] = 0x50;

	tempPatch[0].Apply();

	int* monRet = FonctionPrepare((void*)messageBlock, (void*)NULL, (int)playerBlock, (int*)Object, &flagsToSet, (void*)&unknowField);

	tempPatch[0].Remove();

	CreateMessage55Part((void*)messageBlock, (void*)NULL, (int *)playerBlock2, (int *)Object, (uint32_t *)monRet, (uint32_t)flagsToSet, (uint32_t)unknowField);

	EndMessagePart((void*)messageBlock, (void*)NULL, (int *)playerBlock2,  (uint32_t *)monRet, (int*)Object, (uint32_t)flagsToSet, (uint32_t)unknowField);

	*(uint32_t*)(myMessage + 3) = *(uint32_t*)(messageBlock + 0xC);
	myMessage[0] = 0x50;
	myMessage[1] = 0x05;
	myMessage[2] = 0x01;

	uint32_t ptrToMsg = 0;
	uint32_t sizeToSend = 0;
	FinishMessagePreparation((void*)messageBlock, (void*)NULL, (void*)&ptrToMsg, (void*)&sizeToSend);

	return sizeToSend;
}

bool deposeObject(int iInventorySlot, int iAffected, int mustView, int iItem)
{
	GameObjectManager m_ObjectManager;
	GameObject        *Object;
	NWN::CNWSItem    *Item;
	NWN::CGameObject * ObjectC;

	NWN::CNWSCreature * Creature;

	Object = m_ObjectManager.GetGameObject((NWN::OBJECTID)iItem);
	Item = Object->AsItem();
	ObjectC = m_ObjectManager.GetGameObject( (NWN::OBJECTID) iAffected );

	unsigned long Size = 0;
	bool result = false;

	static uint8_t myMessage[0x800] = {};
	memset((void*)myMessage, 0, 0x800);
	{


		float x = Object->m_WorldTrans.m[3][0];
		float y = Object->m_WorldTrans.m[3][1];
		float z = Object->m_WorldTrans.m[3][2];


		Object->m_WorldTrans.m[3][0] = ObjectC->m_WorldTrans.m[3][0];
		Object->m_WorldTrans.m[3][1] = ObjectC->m_WorldTrans.m[3][1];
		Object->m_WorldTrans.m[3][2] = ObjectC->m_WorldTrans.m[3][2];

		Size = addObjectToWorld(mustView, Object, myMessage);

		Object->m_WorldTrans.m[3][0] = x;
		Object->m_WorldTrans.m[3][1] = y;
		Object->m_WorldTrans.m[3][2] = z;

		unsigned long iMustView = GetObjectToPlayerId(mustView);
		if (iMustView != NWN::PLAYERID_INVALIDID)
		{
			result = PrepaSendMessageToPlayer(iMustView, myMessage, Size, 0);
		}

		return result;
	}
}

uint32_t valFromType(int iInventorySlot)
{
	uint32_t val = 0;

	switch (iInventorySlot) {
		case INVENTORY_SLOT_CHEST:
			val = 0x2;
			break;
		case INVENTORY_SLOT_HEAD:
			val = 0x1;
			break;
		case INVENTORY_SLOT_BOOTS:
			val = 0x4;
			break;
		case INVENTORY_SLOT_ARMS:
			val = 0x8;
			break;
		case INVENTORY_SLOT_CLOAK:
			val = 0x40;
			break;
		case INVENTORY_SLOT_LEFTHAND:
			val = 0x20;
			break;
		case INVENTORY_SLOT_RIGHTHAND:
			val = 0x10;
			break;
		case INVENTORY_SLOT_BELT:
			val = 0x400;
			break;
	}

	return val;
}

////////////////////////////////////////////////////////////////////////////////

bool showObject(int iInventorySlot, int iAffected, int iItem, int mustView)
{
	bool result        = false;
	unsigned long Size = 0;
	static unsigned char Data[0x1000];
	memset(Data, 0, sizeof(Data));

	GameObjectManager m_ObjectManager;
	GameObject* Object  = m_ObjectManager.GetGameObject((NWN::OBJECTID)iItem);
	GameObject* CreaPtr = m_ObjectManager.GetGameObject((NWN::OBJECTID)iAffected);
	NWN::CNWSItem* Item  = Object->AsItem();
	uint8_t* pAppearance = (((uint8_t*)CreaPtr) + 0x183C);

	////////////// TAKED BACK ///////////////
	// Prepare start (msg type, subtype, ....)
	Data[0] = 0x50;
	Data[1] = 0x05;
	Data[2] = 0x01;

	// Init msgBlock
	prepareMsgBlock(Data, sizeof(Data), 0x400, mustView, 1);

	AddX8ToMsg('P', MsgCreation);
	AddX8ToMsg(0x5, MsgCreation);
	AddX32ToMsg(iAffected | 0x80000000, MsgCreation);

	if (iInventorySlot != INVENTORY_SLOT_CLOAK && iInventorySlot != INVENTORY_SLOT_LEFTHAND && iInventorySlot != INVENTORY_SLOT_RIGHTHAND) {
		AddX32ToMsg(0x0300, MsgCreation);
		BuildChestVisual(pAppearance, NULL, (uint8_t*)MsgCreation);
	} else {
		AddX32ToMsg(0x0200, MsgCreation);
	}

	uint32_t valType = valFromType(iInventorySlot);

	AddX8ToMsg(0x2, MsgCreation);
	AddX8ToMsg('D', MsgCreation);
	AddX32ToMsg(0x7F000000, MsgCreation);
	AddX32ToMsg(valType, MsgCreation);

	AddX8ToMsg('A', MsgCreation);
	AddX32ToMsg(iItem | 0x80000000, MsgCreation);
	AddX32ToMsg(valType, MsgCreation);

	BuildMsgItem((void*)MsgCreation, NULL,
	             (void*)Item); // changed here. Before always do modelType 3
	BuildMsgNamePart(MsgCreation, NULL, Item, CreaPtr);

	if (iInventorySlot == INVENTORY_SLOT_CHEST)
	{
		BuildChestVisual(pAppearance, NULL, (uint8_t*)MsgCreation);
	}

	AddX8ToMsg('U', MsgCreation);
	AddX8ToMsg(0x5, MsgCreation);
	AddX32ToMsg(iAffected | 0x80000000, MsgCreation);
	AddX32ToMsg(0x0, MsgCreation);

	if (iAffected == mustView) {

		if (iInventorySlot == INVENTORY_SLOT_CHEST)
		{
			AddX8ToMsg('I', MsgCreation);
			AddX32ToMsg(iAffected | 0x80000000, MsgCreation);
			AddX8ToMsg(0x1, MsgCreation);
			AddX8ToMsg(0x0, MsgCreation);
			UpdateMsgHash(MsgCreation, NULL, 1, 0, 1); //Depend of if its currently controlled creature or not

			uint16_t iAC = CalculAndReturnAC((void*)CreaPtr);
			AddX8ToMsg((uint8_t)(iAC & 0xFF), MsgCreation);
			AddX8ToMsg((uint8_t)((iAC & 0xFF00) >> 8), MsgCreation);

			uint32_t tempVal = *(uint32_t*)((uint8_t*)CreaPtr + 0xEE8); // gold
			AddX32ToMsg(tempVal, MsgCreation);

			tempVal = *(uint32_t*)((uint8_t*)CreaPtr + 0xF9C); // Weight
			AddX32ToMsg(tempVal, MsgCreation);
		}

		// Do GIA part
		BuildGIA(iItem, Item, CreaPtr, valType);

		// Now, GRD
		BuildGRD(iItem);
	}

	uint32_t MsgData;
	PrepareEndOfMsg((void*)MsgCreation, NULL, (void*)&MsgData, (void*)&Size);

	unsigned long iMustView = GetObjectToPlayerId(mustView);
	if (iMustView != NWN::PLAYERID_INVALIDID) {
		unsigned char* mesData = Data;
		result                 = PrepaSendMessageToPlayer(iMustView, mesData, Size, 0);
	}

	return result;
}

bool refreshSelfBelt(int iAffected, int mustView)
{
	bool result = false;
	static unsigned char DataToSend[0x1000];
	memset(DataToSend, 0, sizeof(DataToSend));
	unsigned long  Size;
	uint32_t idBelt = getBeltEquiped(iAffected);
	if (idBelt == 0)
		return true;
	unsigned long iMustView = GetObjectToPlayerId(mustView);

	NWN::OBJECTID       itObjectId = (NWN::OBJECTID)idBelt;
	GameObject        *itObject;
	NWN::CNWSItem    *Item;
	GameObjectManager m_ObjectManager;
	GameObject* iAffectObject = m_ObjectManager.GetGameObject(iAffected);
	if ((itObject = m_ObjectManager.GetGameObject(itObjectId)) == NULL)
		return true;
	if (itObject->GetObjectType() != NWN::OBJECT_TYPE::OBJECT_TYPE_ITEM)
		return true;
	Item = itObject->AsItem();

	int idPourModelType = *(((uint8_t*)Item) + 4);

	int iModelType = GetModelTypeFromItemID(idPourModelType);
	if (iModelType == -1)
		return false;

	deposeObject(INVENTORY_SLOT_BELT, iAffected, mustView, idBelt);



	////////////// BELT DROPPED ///////////////
	ramasserObjet(iAffected, mustView, idBelt);


	////////////// TAKED BACK ///////////////
	// Prepare start (msg type, subtype, ....)
	DataToSend[0] = 0x50;
	DataToSend[1] = 0x05;
	DataToSend[2] = 0x01;
	
	//Init msgBlock
	prepareMsgBlock(DataToSend, sizeof(DataToSend), 0x400, iMustView, 1);


	// We don't care about I part (feats, armor, etc)

	// Do GIA part
	BuildGIA(idBelt, Item, iAffectObject, 0x400);

	// Now, GRD
	BuildGRD(idBelt);


	uint32_t MsgData;
	PrepareEndOfMsg((void*)MsgCreation, NULL, (void*)&MsgData, (void*)&Size);


	if (iMustView != NWN::PLAYERID_INVALIDID)
	{
		unsigned char* mesData = DataToSend;
		result = PrepaSendMessageToPlayer(iMustView, mesData, Size, 0);
	}

	return result;
}

bool refreshNude_Verif(int iAffected, int mustView)
{
	if (IsKnowByPlayer(GetObjectToPlayerId(mustView), iAffected))
	{
		return refreshNude(iAffected, mustView);
	}
	return true;
}

bool SendRefreshCreatureApp(int iAffected, int mustview)
{
	static unsigned char Data[0x50];
	memset(Data, 0, sizeof(Data));
	unsigned long Size = 0;
	bool result = false;
	float fCol;
	uint8_t uCol;

	NWN::OBJECTID     ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;

	ObjectId = (NWN::OBJECTID) iAffected;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject(ObjectId)) == NULL)
		return false;

	Data[0] = 0x50;
	Data[1] = 0x05;
	Data[2] = 0x01;
	Data[3] = 0x44;
	Data[7] = 0x50;
	Data[8] = 0x05;
	*((unsigned long*)(Data + 0x09)) = iAffected|0x80000000;
	Data[0xD] = 0x98;
	Data[0xE] = 0xD8;

	Data[0x11] = ((char*)Object)[AmCrtHeadVariation];
	Data[0x12] = ((char*)Object)[AmCrtTailVariation];
	Data[0x13] = ((char*)Object)[AmCrtWingVariation];

	/////////// Couleur  1 RGBA///////////
	{
		fCol = *(float*)((uint8_t*)Object + AmCrtTint);
		uCol = std::lround(fCol*255.0);
		Data[0x14] = uCol;
		fCol = *(float*)((uint8_t*)Object + AmCrtTint + 0x4);
		uCol = std::lround(fCol*255.0);
		Data[0x15] = uCol;
		fCol = *(float*)((uint8_t*)Object + AmCrtTint + 0x8);
		uCol = std::lround(fCol*255.0);
		Data[0x16] = uCol;
		fCol = *(float*)((uint8_t*)Object + AmCrtTint + 0xC);
		uCol = std::lround(fCol*255.0);
		Data[0x17] = uCol;
	}

	/////////// Couleur  2 RGBA ///////////
	{
		fCol = *(float*)((uint8_t*)Object + AmCrtTint + 0x10);
		uint8_t uCol = std::lround(fCol * 255.0);
		Data[0x18] = uCol;
		fCol = *(float*)((uint8_t*)Object + AmCrtTint + 0x14);
		uCol = std::lround(fCol * 255.0);
		Data[0x19] = uCol;
		fCol = *(float*)((uint8_t*)Object + AmCrtTint + 0x18);
		uCol = std::lround(fCol * 255.0);
		Data[0x1A] = uCol;
		fCol = *(float*)((uint8_t*)Object + AmCrtTint + 0x1C);
		uCol = std::lround(fCol * 255.0);
		Data[0x1B] = uCol;
	}

	/////////// Couleur  3 RGBA ///////////
	{
		fCol = *(float*)((uint8_t*)Object + AmCrtTint + 0x20);
		uint8_t uCol = std::lround(fCol * 255.0);
		Data[0x1C] = uCol;
		fCol = *(float*)((uint8_t*)Object + AmCrtTint + 0x24);
		uCol = std::lround(fCol * 255.0);
		Data[0x1D] = uCol;
		fCol = *(float*)((uint8_t*)Object + AmCrtTint + 0x28);
		uCol = std::lround(fCol * 255.0);
		Data[0x1E] = uCol;
		fCol = *(float*)((uint8_t*)Object + AmCrtTint + 0x2C);
		uCol = std::lround(fCol * 255.0);
		Data[0x1F] = uCol;
	}

	//////////// HEAD COLORS ///////////////

	/////////// Couleur  SKIN RGBA ///////////
	{
		fCol = *(float*)((uint8_t*)Object + AmCrtHeadTint);
		uCol = std::lround(fCol * 255.0);
		Data[0x20] = uCol;
		fCol = *(float*)((uint8_t*)Object + AmCrtHeadTint + 0x4);
		uCol = std::lround(fCol * 255.0);
		Data[0x21] = uCol;
		fCol = *(float*)((uint8_t*)Object + AmCrtHeadTint + 0x8);
		uCol = std::lround(fCol * 255.0);
		Data[0x22] = uCol;
		fCol = *(float*)((uint8_t*)Object + AmCrtHeadTint + 0xC);
		uCol = std::lround(fCol * 255.0);
		Data[0x23] = uCol;
	}

	/////////// Couleur  EYE RGBA ///////////
	{
		fCol = *(float*)((uint8_t*)Object + AmCrtHeadTint + 0x10);
		uint8_t uCol = std::lround(fCol * 255.0);
		Data[0x24] = uCol;
		fCol = *(float*)((uint8_t*)Object + AmCrtHeadTint + 0x14);
		uCol = std::lround(fCol * 255.0);
		Data[0x25] = uCol;
		fCol = *(float*)((uint8_t*)Object + AmCrtHeadTint + 0x18);
		uCol = std::lround(fCol * 255.0);
		Data[0x26] = uCol;
		fCol = *(float*)((uint8_t*)Object + AmCrtHeadTint + 0x1C);
		uCol = std::lround(fCol * 255.0);
		Data[0x27] = uCol;
	}

	/////////// Couleur BROW RGBA ///////////
	{
		fCol = *(float*)((uint8_t*)Object + AmCrtHeadTint + 0x20);
		uint8_t uCol = std::lround(fCol * 255.0);
		Data[0x28] = uCol;
		fCol = *(float*)((uint8_t*)Object + AmCrtHeadTint + 0x24);
		uCol = std::lround(fCol * 255.0);
		Data[0x29] = uCol;
		fCol = *(float*)((uint8_t*)Object + AmCrtHeadTint + 0x28);
		uCol = std::lround(fCol * 255.0);
		Data[0x2A] = uCol;
		fCol = *(float*)((uint8_t*)Object + AmCrtHeadTint + 0x2C);
		uCol = std::lround(fCol * 255.0);
		Data[0x2B] = uCol;
	}



	//////////// HAIR COLORS ///////////////

	/////////// Couleur  HAIR1 RGBA ///////////
	{
		fCol = *(float*)((uint8_t*)Object + AmCrtHairTint);
		uCol = std::lround(fCol * 255.0);
		Data[0x2C] = uCol;
		fCol = *(float*)((uint8_t*)Object + AmCrtHairTint + 0x4);
		uCol = std::lround(fCol * 255.0);
		Data[0x2D] = uCol;
		fCol = *(float*)((uint8_t*)Object + AmCrtHairTint + 0x8);
		uCol = std::lround(fCol * 255.0);
		Data[0x2E] = uCol;
		fCol = *(float*)((uint8_t*)Object + AmCrtHairTint + 0xC);
		uCol = std::lround(fCol * 255.0);
		Data[0x2F] = uCol;
	}

	/////////// Couleur  HAIR1 RGBA ///////////
	{
		fCol = *(float*)((uint8_t*)Object + AmCrtHairTint + 0x10);
		uint8_t uCol = std::lround(fCol * 255.0);
		Data[0x30] = uCol;
		fCol = *(float*)((uint8_t*)Object + AmCrtHairTint + 0x14);
		uCol = std::lround(fCol * 255.0);
		Data[0x31] = uCol;
		fCol = *(float*)((uint8_t*)Object + AmCrtHairTint + 0x18);
		uCol = std::lround(fCol * 255.0);
		Data[0x32] = uCol;
		fCol = *(float*)((uint8_t*)Object + AmCrtHairTint + 0x1C);
		uCol = std::lround(fCol * 255.0);
		Data[0x33] = uCol;
	}

	/////////// Couleur HACC RGBA ///////////
	{
		fCol = *(float*)((uint8_t*)Object + AmCrtHairTint + 0x20);
		uint8_t uCol = std::lround(fCol * 255.0);
		Data[0x34] = uCol;
		fCol = *(float*)((uint8_t*)Object + AmCrtHairTint + 0x24);
		uCol = std::lround(fCol * 255.0);
		Data[0x35] = uCol;
		fCol = *(float*)((uint8_t*)Object + AmCrtHairTint + 0x28);
		uCol = std::lround(fCol * 255.0);
		Data[0x36] = uCol;
		fCol = *(float*)((uint8_t*)Object + AmCrtHairTint + 0x2C);
		uCol = std::lround(fCol * 255.0);
		Data[0x37] = uCol;
	}

	Data[0x38] = ((char*)Object)[AmCrtHairVariation];
	Data[0x39] = ((char*)Object)[AmCrtFacialHairVariation];

	Data[0x3A] = 0x55;
	Data[0x3B] = 0x05;

	*((unsigned long*)(Data + 0x3C)) = iAffected|0x80000000;

	Data[0x44] = 0x70;

	unsigned long iMustView = GetObjectToPlayerId(mustview);
	if (iMustView != NWN::PLAYERID_INVALIDID)
	{
		unsigned char* mesData = Data;
		result = PrepaSendMessageToPlayer(iMustView, mesData, 0x45, 0);
	}

	return result;
}


bool refreshNude(int iAffected, int mustView)
{
	static unsigned char Data[0x300];

	if (!SendRefreshCreatureApp(iAffected, mustView))
		return false;

	memset(Data, 0, sizeof(Data));
	unsigned long Size = 0;
	unsigned long SizeBefore = 0;
	bool result = false;

	std::memcpy ( Data, refreshNudeP1, 17);
	*((unsigned long*)(Data + 0x09)) = iAffected|0x80000000;
	Size += 17;
	
	
	{
		NWN::OBJECTID       ObjectId;
		GameObject        *Object;
		GameObjectManager m_ObjectManager;

		ObjectId = (NWN::OBJECTID) iAffected;

		if ((ObjectId & NWN::INVALIDOBJID) != 0)
			ObjectId &= ~(NWN::LISTTYPE_MASK);

		if ((Object = m_ObjectManager.GetGameObject(ObjectId)) != NULL) {
			char uGender = ((char*)Object)[AmCrtGenderVis]; //Set the gender here is the good things to do, but it can cause crash on non 0/1 gender.
			if (uGender != 0 && uGender != 1)
				uGender = 0;
			Data[Size] = uGender;
		}
		Size++;
	}


	Size += createNudeApp(iAffected, Data + Size);
	std::memcpy ( Data+Size, refreshNudeP2, 10);

	*((unsigned long*)(Data + Size + 0x02)) = iAffected|0x80000000;


	//call CalcEndOfMessage avec MsgCreation, datatosend, sizetosend
	Size += 10;
	prepareEndOfMessage(MsgCreation, Data, &Size);
	//Size += 14;


	//*((unsigned long*)(Data + 3)) = (Size - 4);

	unsigned long iMustView = GetObjectToPlayerId(mustView);
	if (iMustView != NWN::PLAYERID_INVALIDID)
	{
		unsigned char* mesData = Data;
		result = PrepaSendMessageToPlayer(iMustView, mesData, Size, 0);
	}
	return result;
}

bool refreshBelt(int iAffected, int mustView)
{
	if (IsKnowByPlayer(GetObjectToPlayerId(mustView), iAffected))
	{
		uint32_t idArmor = getArmorEquiped(iAffected);
		if (iAffected == mustView)
		{
			refreshSelfBelt(iAffected, mustView);
		}
		if (idArmor != 0)
		{
			return refreshItemApp(INVENTORY_SLOT_CHEST, iAffected, idArmor, mustView);
		}
		else
		{
			return refreshNude(iAffected, mustView);
		}
	}
	return false;
}

bool refreshItemApp(int iInventorySlot, int iAffected, int iItem, int mustView)
{
	if (IsKnowByPlayer(GetObjectToPlayerId(mustView),iAffected))
	{
		//if (removeVisual(iInventorySlot, iAffected, mustView))
		{
			if (deposeObject(iInventorySlot, iAffected, mustView, iItem))
			{
				if (ramasserObjet(iAffected, mustView, iItem))
				{
					return showObject(iInventorySlot, iAffected, iItem, mustView);
				}
			}
		}
	}
	return false;
}

void removeForEveryConcerned(uint32_t iAffected, int iInventorySlot)
{
	uint32_t iMustView = GetPCIntern(true, false, 32);
	while (iMustView != NWN::INVALIDOBJID)
	{
		if (IsKnowByPlayer(GetObjectToPlayerId(iMustView), iAffected) || iMustView == iAffected)
		{
			removeVisual(iInventorySlot, iAffected, iMustView);
		}
		iMustView = GetPCIntern(false, false, 32);
	}
}

void refreshForEveryConcerned(uint32_t iAffected)
{
	uint32_t iMustView = GetPCIntern(true, false);
	uint32_t iArmor = getArmorEquiped(iAffected);
	while (iMustView != NWN::INVALIDOBJID)
	{
		if (IsKnowByPlayer(GetObjectToPlayerId(iMustView), iAffected) || iMustView == iAffected)
		{
			if (iArmor == 0)
			{
				refreshNude(iAffected, iMustView);
			}
			else
			{
				refreshItemApp(INVENTORY_SLOT_CHEST, iAffected, iArmor, iMustView);
			}
		}
		iMustView = GetPCIntern(false, false);
	}
}


bool sendMusicMessage(int iType, int iOption, int iValue, int mustView)
{
	unsigned long Size= 0;
	static unsigned char Data[0x20];
	memset(Data, 0, sizeof(Data));
	bool result = false;

	if (iType == 0) //Play/stop
	{
		uint8_t iCode = 0;
		//50 28 01 07 00 00 00
		Data[0] = 0x50;
		Data[1] = 0x28;
		if (iOption != 2)
		{
			Data[2] = 0x01;
		}
		else
		{
			Data[2] = 0x04;
		}
		Data[3] = 0x07;
		Data[4] = 0x00;
		Data[5] = 0x00;
		Data[6] = 0x00;
		if (iOption != 2) // Background
		{
			iCode = 0x2;
		}

		if (iValue != 0)
		{
			iCode |= 0x90;
		}
		else
		{
			iCode |= 0x80;
		}
		Data[7] = iCode;
		Size = 8;
	}
	else if (iType == 1) //Change
	{
		uint8_t iCode = 0;
		uint8_t iFirstCode = 0;
		if (iOption == 0) //Day
		{
			iCode = 0x92;
			iFirstCode = 0x03;
		}
		else if (iOption == 1) //Night
		{
			iCode = 0x82;
			iFirstCode = 0x03;
		}
		else if (iOption == 2) //Battle
		{
			iCode = 0x70;
			iFirstCode = 0x05;
		}

		Data[0] = 0x50;
		Data[1] = 0x28;
		Data[2] = iFirstCode;
		Data[3] = 0x0B;
		Data[4] = 0x00;
		Data[5] = 0x00;
		Data[6] = 0x00;

		*((unsigned long*)(Data + 7)) = iValue;

		Data[11] = iCode;

		Size = 12;

	}

	unsigned long iMustView = GetObjectToPlayerId(mustView);
	if (iMustView != NWN::PLAYERID_INVALIDID)
	{
		unsigned char* mesData = Data;
		result = PrepaSendMessageToPlayer(iMustView, mesData, Size, 0);
	}
	return result;
}



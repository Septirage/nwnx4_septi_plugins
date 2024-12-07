
#pragma warning(disable : 4828)
#include "messageManagement.h"
#include "aspectManagerUtils.h"

#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include <misc/log.h>
#include <misc/Patch.h>
#include <nwn2heap.h>
#include "../../septutil/NwN2DataPos.h"


#include <string>


#define FUNC_PREPAREMSG			0x0056df15
#define FUNC_CREATE55PART		0x00565760
#define FUNC_ENDPART			0x0056ded0
#define FUNC_FINISHPREPARATION	0x0074E080

#define OFFSET_UTILITY_PTR_PC 0x0086442C

#define FUNC_CALCULATEAC	  0x005e01d0
#define FUNC_CALCSTRANGE	  0x005d68b0
#define FUNC_BUILDVISUAL	  0x0074fec0
#define FUNC_BUILDCOMPVISUAL  0x00551090
#define FUNC_BUILDPARTIENOM	  0x00550EF0

#define FUNC_GETBASEITMFROMID 0x00750b20

#define FUNC_PREPAREENDOFMSG  0x0074e080;
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



static unsigned char MsgCreation[0x50];
static unsigned char UnknowMsg[0x80];


void setLoggerMsg(LogNWNX* myLogger)
{
	msgLogger = myLogger;
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

__declspec(naked) void __fastcall BuildCompVisual(__in void* MsgCreator, __in void* Unused, __in void* itmPtr)
{
	__asm
	{
		mov		edx, FUNC_BUILDCOMPVISUAL;
		jmp		edx;
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

__declspec(naked) int __fastcall CalcStrange(void* pItem)
{
	__asm
	{
		mov		edx, FUNC_CALCSTRANGE;
		jmp     edx;
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

unsigned long BuildMyNamePart(uint8_t* DataDest, uint8_t* ItmPtr, uint8_t* CreaPtr)
{
	memset(MsgCreation, 0, sizeof(MsgCreation));
	memset(UnknowMsg, 0, sizeof(UnknowMsg));

	//preparation of Visual
	*(uint32_t*)(MsgCreation + 0x8) = 0x300;
	*(unsigned char**)(MsgCreation + 0x4) = DataDest;
	*(uint32_t*)(MsgCreation + 0x14) = 0x80;
	*(unsigned char**)(MsgCreation + 0x10) = UnknowMsg;

	BuildMsgNamePart(MsgCreation, NULL, ItmPtr, CreaPtr);

	unsigned long iSizeArmorVisual = *(unsigned long*)((unsigned char*)MsgCreation + 0xC);

	return iSizeArmorVisual;
}

unsigned long BuildMyCompVisual(uint8_t* DataDest, uint8_t* ItmPtr)
{
	memset(MsgCreation, 0, sizeof(MsgCreation));
	memset(UnknowMsg, 0, sizeof(UnknowMsg));


	*(uint32_t*)(MsgCreation + 0x8) = 0x300;
	*(unsigned char**)(MsgCreation + 0x4) = DataDest;
	*(uint32_t*)(MsgCreation + 0x14) = 0x80;
	*(unsigned char**)(MsgCreation + 0x10) = UnknowMsg;

	BuildCompVisual((void*)MsgCreation, NULL, (void*)ItmPtr);

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


uint32_t getHelmEquiped(int iAffected)
{
	uint32_t retVal = 0;

	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;

	ObjectId = (NWN::OBJECTID) iAffected;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return 0x7F000000;

	if (Object->GetObjectType() != NWN::OBJECT_TYPE::OBJECT_TYPE_CREATURE)
		return 0x7F000000;

	uint32_t* objEquiped = *((uint32_t**)(((char*)Object) + AmCrtCSEquipedObjectTable));
	//we pass the 98 2F 80 00 to go to helm

	retVal = objEquiped[1];
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

	char* objEmp    = (((char*)Object) + CrStats);
	char* objEmpP = *((char**)objEmp);
	uint32_t idBelt = getBeltEquiped(iAffected);
	objEmp = objEmpP + 0x6F8;
	char* objApp =  (((char*)Object) + 0x183C);

	// + 183c => 7C 2B 81 00 
	// +++++ => 4C 2A 81 00 (chapeau, puis machin puis machin)

	unsigned long iSizeNudeApp = BuildMyChestVisual((uint8_t*)Data, (uint8_t*)objApp);//(uint8_t*)objEmp-4); // - 4 avec l'objet objEmp
	return iSizeNudeApp;


	Data[0] = objEmp[0];
	Data[1] = objEmp[1];

	for (int i = 0; i < 12; i++)
	{
		float fCoul = *(float*)((objEmp + 4 + (i * 4)));
		uint8_t bCoul = std::lround(fCoul * 255.0);
		Data[2 + i] = (unsigned char)bCoul;
	}
	objEmp = objEmp + 0x34;
	for (int i = 0; i < 22; i++)
	{
		Data[14 + (i * 13)] = objEmp[0];

		for (int j = 0; j < 12; j++)
		{
			float fCoul = *(float*)((objEmp + 4 + (j * 4)));
			uint8_t bCoul = std::lround(fCoul * 255.0);
			Data[14 + (i * 13)+1+j] = (unsigned char)bCoul;
		}

		objEmp = objEmp + 64;
	}

	unsigned char maskAff = objEmpP[0xDC4];
	unsigned char maskTab[5] = { 0x1, 0x2, 0x4, 0x8, 0x10 };
	unsigned char orderAff[5] = { 0, 2, 1, 3, 4 };
	objEmp = objEmpP + 0xCAC;



	for (int u = 0; u < 5; u++)
	{
		int uPos = orderAff[u] * 56;
		if (u == 3 && idBelt != 0)
		{
			NWN::OBJECTID       itObjectId;
			GameObject        *itObject;
			NWN::CNWSItem    *Item;

			itObjectId = (NWN::OBJECTID) idBelt;

			if ((itObjectId & NWN::INVALIDOBJID) != 0)
				itObjectId &= ~(NWN::LISTTYPE_MASK);

			if ((itObject = m_ObjectManager.GetGameObject(itObjectId)) == NULL)
			{
				Data[size] = 0;
				size++;
				continue;
			}

			if (itObject->GetObjectType() != NWN::OBJECT_TYPE::OBJECT_TYPE_ITEM)
			{
				Data[size] = 0;
				size++;
				continue;
			}

			Item = itObject->AsItem();


			Data[size] = 1;
			Data[size + 1] = *(((unsigned char*)Item) + 0xC);
			Data[size + 2] = *(((unsigned char*)Item) + 0xD);

			for (int i = 0; i < 12; i++)
			{
				float fCoul = *(float*)(((unsigned char*)Item) + 0x6DC  + (i * 4));
				uint8_t bCoul = std::lround(fCoul * 255.0);
				Data[size+3+i] = (unsigned char)bCoul;
			}

			size += 0xF;
		}
		else if(maskAff & maskTab[u])
		{
			Data[size] = 1;
			Data[size + 1] = objEmp[uPos+4];
			Data[size + 2] = objEmp[uPos+5];

			for (int i = 0; i < 12; i++)
			{
				float fCoul = *(float*)((objEmp + uPos + 8 + (i * 4)));
				uint8_t bCoul = std::lround(fCoul * 255.0);
				Data[size+3 + i] = (unsigned char)bCoul;
			}

			size += 0xF;
		}
		else
		{
			Data[size] = 0;
			size++;
		}
		//objEmp = objEmp + 56;
	}

	return size;
}

unsigned long transformColor(unsigned char* color)
{
	float* myColor = (float*)color;
	unsigned long   ColorData;
	unsigned __int8 Data;


	ColorData = 0;
	Data = (unsigned __int8)(myColor[3] * 255.0);
	//Data = (unsigned __int8)(myColor->r * 255.0);
	ColorData = Data;
	ColorData = (ColorData << 8) & 0xFF00;
	Data = (unsigned __int8)(myColor[2] * 255.0);
	ColorData |= Data;
	ColorData = (ColorData << 8) & 0xFFFF00;
	Data = (unsigned __int8)(myColor[1] * 255.0);
	ColorData |= Data;
	ColorData = (ColorData << 8) & 0xFFFFFF00;
	Data = (unsigned __int8)(myColor[0] * 255.0);
	ColorData |= Data;

	return ColorData;
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

unsigned long VfxPartConstruct(unsigned char* VfxPart,  GameObject * Object, NWN::CNWSItem* Item)
{
	unsigned long vfxPartSize = 0;
	{
		VfxPart[0] = *(((unsigned char*)Item) + AmItmFXWpn);
		vfxPartSize = 1;
		const NWN::CExoString* VfxName = (NWN::CExoString*)(((unsigned char*)Object) + AmAppSEF);

		if ((VfxName->m_sString == NULL) || (VfxName->m_nBufferLength == 0))
		{
		}
		else
		{
			*((ULONG*)(VfxPart + vfxPartSize)) = VfxName->m_nBufferLength -1;
			vfxPartSize += 4;
			std::memcpy ( VfxPart+vfxPartSize, VfxName->m_sString, VfxName->m_nBufferLength-1);
			vfxPartSize += VfxName->m_nBufferLength-1;
		}
	}
	//vfxPartSize += 6;
	return vfxPartSize;
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
		else if(iInventorySlot == INVENTORY_SLOT_BELT) //TODO, belt
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
	if (Size != 0)
	{
		unsigned long iMustView = GetObjectToPlayerId(mustView);
		if (iMustView != NWN::PLAYERID_INVALIDID)
		{
			unsigned char* mesData = myMessage;
			result = PrepaSendMessageToPlayer(iMustView, mesData, Size, 0);
		}
	}
	return result;
}





unsigned long ExtraArmorConstruct(char* itemArmor, unsigned long extraArmorSize, unsigned char* ExtraArmor)
{
	unsigned long ValTest = *(unsigned long*)(itemArmor + 0x8);
	if (ValTest != 0x40400000)
	{
		ExtraArmor[extraArmorSize] = 1;
		ExtraArmor[extraArmorSize + 1] = itemArmor[4];
		ExtraArmor[extraArmorSize + 2] = itemArmor[5];
		extraArmorSize += 3;
		unsigned long ulColor = transformColor((unsigned char*)itemArmor+8);
		ExtraArmor[extraArmorSize] = (unsigned char) (ulColor & 0xFF);
		ExtraArmor[extraArmorSize + 1] = (unsigned char) ((ulColor >> 8) & 0xFF);
		ExtraArmor[extraArmorSize + 2] = (unsigned char) ((ulColor >> 16) & 0xFF);
		ExtraArmor[extraArmorSize + 3] = (unsigned char) ((ulColor >> 24) & 0xFF);
		extraArmorSize += 4;
		ulColor = transformColor((unsigned char*)itemArmor+24);
		ExtraArmor[extraArmorSize] = (unsigned char) (ulColor & 0xFF);
		ExtraArmor[extraArmorSize + 1] = (unsigned char) ((ulColor >> 8) & 0xFF);
		ExtraArmor[extraArmorSize + 2] = (unsigned char) ((ulColor >> 16) & 0xFF);
		ExtraArmor[extraArmorSize + 3] = (unsigned char) ((ulColor >> 24) & 0xFF);
		extraArmorSize += 4;
		ulColor = transformColor((unsigned char*)itemArmor+40);
		ExtraArmor[extraArmorSize] = (unsigned char) (ulColor & 0xFF);
		ExtraArmor[extraArmorSize + 1] = (unsigned char) ((ulColor >> 8) & 0xFF);
		ExtraArmor[extraArmorSize + 2] = (unsigned char) ((ulColor >> 16) & 0xFF);
		ExtraArmor[extraArmorSize + 3] = (unsigned char) ((ulColor >> 24) & 0xFF);
		extraArmorSize += 4;
		/* *(unsigned long*)(((char*)ExtraArmor+3)) = transformColor((unsigned char*)itemArmor+8);
		*(unsigned long*)(((char*)ExtraArmor+7))  = transformColor((unsigned char*)itemArmor+24);
		*(unsigned long*)(((char*)ExtraArmor+11))  = transformColor((unsigned char*)itemArmor+40);
		extraArmorSize += 15;*/
	}
	else
	{
		extraArmorSize++;
	}
	return extraArmorSize;
}

unsigned char valFromType(int iInventorySlot)
{
	unsigned char val = 0;

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
	}

	return val;
}


//Prepare the Part2
unsigned long Part2Construct(unsigned char* Data, GameObject * Object, NWN::CNWSItem * Item, int iInventorySlot)
{
	unsigned char DataPart2[0x30];
	std::memcpy ( DataPart2, equipeArmorP2, 0x1F);

	*(uint32_t*)(DataPart2+0x0B) = (*(uint32_t*)(((char*)Object) + 0xA0))|0x80000000;

	unsigned char val = valFromType(iInventorySlot);

	DataPart2[0x0F] = val;
	DataPart2[0x06] = val;

	*(uint32_t*)(DataPart2 + 0x13) = *(uint32_t*)(((char*)Item) + AmItmType);

	*(uint32_t*)(DataPart2 + 0x17) = *(uint32_t*)(((char*)Item) + AmItmIcon);

	*(uint32_t*)(DataPart2 + 0x1B) = *(uint32_t*)(((char*)Item) + AmItmArmorRulesType);

	//Add the part2
	std::memcpy ( Data, DataPart2, 0x1F);
	return 0x1F;
}

unsigned long  DataColorConstruct(unsigned char* DataColor, NWN::CNWSItem* Item)
{
	unsigned long DataColorSize = 0;
	{
		*((unsigned long*)(DataColor + DataColorSize)) = transformColor(((unsigned char*)Item)+0x6DC);
		*((unsigned long*)(DataColor + DataColorSize+4)) = transformColor(((unsigned char*)Item)+0x6EC);
		*((unsigned long*)(DataColor + DataColorSize+8)) = transformColor(((unsigned char*)Item)+0x6FC);
		DataColorSize += 12;
		char UVScroll = *(((char*)Item) + 0x70C);
		if (UVScroll != 0)
		{
			*(float*)(DataColor + DataColorSize) = *(float*)(((char*)Item) + 0x710);
			*(float*)(DataColor + DataColorSize+4) = *(float*)(((char*)Item) + 0x714);
			DataColorSize += 8;
		}
	}
	return DataColorSize;
}

unsigned long ExtraArmorPartConstruct(unsigned char* ExtraArmor, NWN::CNWSItem* Item, int iAffected = 0)
{
	bool mustPass = true;
	unsigned long extraArmorSize = 5;
	{
		extraArmorSize = 0;
		extraArmorSize = ExtraArmorConstruct(((char*)Item) + 0x5C0, extraArmorSize, (unsigned char*)ExtraArmor);
		extraArmorSize = ExtraArmorConstruct(((char*)Item) + 0x630, extraArmorSize, (unsigned char*)ExtraArmor);
		extraArmorSize = ExtraArmorConstruct(((char*)Item) + 0x5F8, extraArmorSize, (unsigned char*)ExtraArmor);
		if (iAffected != 0)
		{
			uint32_t iBelt = getBeltEquiped(iAffected);
			if (iBelt != 0)
			{
				NWN::OBJECTID       itObjectId;
				GameObject        *itObject;
				NWN::CNWSItem    *Item;
				GameObjectManager m_ObjectManager;

				itObjectId = (NWN::OBJECTID) iBelt;

				if ((itObjectId & NWN::INVALIDOBJID) != 0)
					itObjectId &= ~(NWN::LISTTYPE_MASK);

				if ((itObject = m_ObjectManager.GetGameObject(itObjectId)) == NULL)
				{
				}
				else if (itObject->GetObjectType() != NWN::OBJECT_TYPE::OBJECT_TYPE_ITEM)
				{
				}
				else
				{
					Item = itObject->AsItem();


					ExtraArmor[extraArmorSize] = 1;
					ExtraArmor[extraArmorSize + 1] = *(((unsigned char*)Item) + 0xC);
					ExtraArmor[extraArmorSize + 2] = *(((unsigned char*)Item) + 0xD);

					for (int i = 0; i < 12; i++)
					{
						float fCoul = *(float*)(((unsigned char*)Item) + AmItmBaseColor + (i * 4));
						uint8_t bCoul = std::lround(fCoul * 255.0);
						ExtraArmor[extraArmorSize+3+i] = (unsigned char)bCoul;
					}

					extraArmorSize += 0xF;
					mustPass = false;
				}
			}
		}
		if(mustPass)
			extraArmorSize = ExtraArmorConstruct(((char*)Item) + 0x668, extraArmorSize, (unsigned char*)ExtraArmor);
		extraArmorSize = ExtraArmorConstruct(((char*)Item) + 0x6A0, extraArmorSize, (unsigned char*)ExtraArmor);
	}
	return extraArmorSize;
}

unsigned long DataVisualConstructor(unsigned char* DataVisual, NWN::CNWSItem* Item)
{
	unsigned long SizeVisual = 0;

	//first part of visual:
	DataVisual[0] = *(((char*)Item) + 0xC);
	DataVisual[1] = *(((char*)Item) + 0xD);
	for (int i = 2; i < 0xE; i++)
	{
		DataVisual[i] = 0xFF;
	}
	SizeVisual = 14;

	//We are at each of element part
	for (int iC = 0; iC < 22; iC++)
	{
		int iItem0 = ((iC + 1) * 0x40);
		DataVisual[SizeVisual] = *(((char*)Item) + iItem0);
		*((unsigned long*)(DataVisual + SizeVisual+1)) = transformColor(((unsigned char*)Item)+iItem0+0x4);
		*((unsigned long*)(DataVisual + SizeVisual+5)) = transformColor(((unsigned char*)Item)+iItem0+0x14);
		*((unsigned long*)(DataVisual + SizeVisual+9)) = transformColor(((unsigned char*)Item)+iItem0+0x24);
		SizeVisual += 13;
		char UVScroll = *(((char*)Item) + iItem0 + 0x34);
		if (UVScroll != 0)
		{
			*(float*)(DataVisual + SizeVisual) = *(float*)(((char*)Item) + iItem0 + 0x38);
			*(float*)(DataVisual + SizeVisual+4) = *(float*)(((char*)Item) + iItem0 + 0x3C);
			SizeVisual += 8;
		}
	}
	return SizeVisual;
}

////////////////////////////////////////////////////////////////////////////////

bool afficherObjet2(int iInventorySlot, int iAffected, int iItem, int mustView)
{
	bool result = false;
	unsigned long Size = 0;
	static unsigned char Data[0x1000];
	memset(Data, 0, sizeof(Data));

	GameObjectManager m_ObjectManager;
	GameObject        *Object = m_ObjectManager.GetGameObject((NWN::OBJECTID ) iItem ) ;
	GameObject        *CreaPtr = m_ObjectManager.GetGameObject((NWN::OBJECTID ) iAffected ) ;
	NWN::CNWSItem    *Item = Object->AsItem();

	std::memcpy ( Data, equipeArmorP1, 17);
	Size = 17;

	//correction of P1
	*((unsigned long*)(Data + 0x09)) = iAffected|0x80000000;

	//preparation of Visual
	unsigned char DataVisual[0x200];
	unsigned long SizeVisual = DataVisualConstructor( DataVisual, Item);

	//prepare the extra part of armor
	static unsigned char ExtraArmor[75];
	memset(ExtraArmor, 0, sizeof(ExtraArmor));
	unsigned long extraArmorSize = ExtraArmorPartConstruct( ExtraArmor, Item);


	if(iInventorySlot != INVENTORY_SLOT_CLOAK)
	{
		Size += createNudeApp(iAffected, (Data + 17)); //Not really nude but the trick work
	}
	else
	{
		Data[0x0E] = 0x02;
	}

	Size += Part2Construct(Data + Size, Object, Item, iInventorySlot);



	std::memcpy ( Data+Size, DataVisual, SizeVisual);
	Size += SizeVisual;
	// extra part of armor
	std::memcpy ( Data+Size, ExtraArmor, extraArmorSize);
	Size += extraArmorSize;
	

	static unsigned char DataColor[27];
	memset(DataColor, 0, sizeof(DataColor));
	unsigned long DataColorSize = DataColorConstruct(DataColor, Item);
	std::memcpy ( Data+Size, DataColor, DataColorSize);
	Size += DataColorSize;

	static unsigned char DataVfxPart[300];
	memset(DataVfxPart, 0, sizeof(DataVfxPart));
	unsigned long DataVfxPartSize = VfxPartConstruct(DataVfxPart, Object, Item);
	std::memcpy ( Data+Size, DataVfxPart, DataVfxPartSize);
	Size += DataVfxPartSize;

	////////////////////////////////////////////////////////////////////////////////

	static unsigned char TLKName[300];
	memset(TLKName, 0, sizeof(TLKName));
	unsigned long TLKNameSize = BuildMyNamePart((uint8_t*) TLKName, (uint8_t*)Item, (uint8_t*)CreaPtr);

	std::memcpy ( Data+Size, TLKName, TLKNameSize);
	Size += TLKNameSize;

	if(iAffected != mustView)
	{
		std::memcpy ( Data+Size, equipeArmorP4, 0x11);

		*((unsigned long*)(Data + Size + 2)) = iAffected|0x80000000;

		if(iInventorySlot != INVENTORY_SLOT_CLOAK)
		{

			if (TLKNameSize > 0x38 && DataVfxPartSize > 0x4)
			{
				Data[Size + 0x0A] = 0xC0;//0xE0; //C0
				Data[Size + 0x10] = 0xB0;//0xD8; //B0
			}
			else if (DataVfxPartSize > 0x4)
			{
				Data[Size + 0x0A] = 0xE0;//0x6D; //E0
				Data[Size + 0x10] = 0xD8;//0x8D; //D8
			}
			else if (TLKNameSize > 0x38)
			{
				Data[Size + 0x0A] = 0xC0;
				Data[Size + 0x10] = 0x30;
			}
			else
			{
				Data[Size + 0x0A] = 0xE0;
				Data[Size + 0x10] = 0x58; //59
			}


			Size += 17;
			*((unsigned long*)(Data + 3)) = (Size - 7);
		}
		else 
		{

			int iMinus = 5;
			if (TLKNameSize > 0x38 && DataVfxPartSize > 0x4)
			{
				Data[Size + 0xA] = 0x00;

				Data[Size + 0xD] = 0x2C;
				Size--;
				iMinus = 4;
			}
			else if (DataVfxPartSize > 0x4)
			{
				Data[Size + 0xA] = 0x20;

				Data[Size + 0xD] = 0x36;
				Data[Size + 0xE] = 0x00;
			}
			else if (TLKNameSize > 0x38)
			{
				Data[Size + 0xA] = 0x00;

				Data[Size + 0xD] = 0x0C;
				Size--;
				iMinus = 4;
			}
			else
			{
				Data[Size + 0xA] = 0x20;

				Data[Size + 0xD] = 0x16;
				Data[Size + 0xE] = 0x00;
			}

			Size += 0xF;
			*((unsigned long*)(Data + 3)) = (Size - iMinus);
		}
	}
	else
	{
		if(iInventorySlot == INVENTORY_SLOT_CLOAK)
		{
			std::memcpy(Data+Size, equipeArmorP4Sb, 0x21);
			*((unsigned long*)(Data + Size + 2)) = iAffected|0x80000000;

			Data[Size + 0x0D] = valFromType(iInventorySlot);
			Data[Size + 0x0E] = 0;
			Data[Size + 0x0F] = 0;
			Data[Size + 0x10] = 0;

			*(uint32_t*)(Data+Size+0x11) = (*(uint32_t*)(((char*)Object) + 0xA0))|0x80000000;

			Data[Size + 0x15] = 0x50;
			Data[Size+0x16] = 0;
			Data[Size+0x17] = 0;
			Data[Size+0x18] = 0;

			Data[Size+0x19] = *(((char*)Item)+0xAF4);
			Data[Size+0x1A] = *(((char*)Item)+0xAF5);
			Data[Size+0x1B] = *(((char*)Item)+0xAF6);
			Data[Size+0x1C] = *(((char*)Item)+0xAF7);

			Size += 0x21;
		}
		else
		{
			std::memcpy ( Data+Size, equipeObjP4S, 0x21);
			*((unsigned long*)(Data + Size + 2)) = iAffected|0x80000000;

			Data[Size+0xD] = valFromType(iInventorySlot);
			Data[Size + 0xE] = 0;
			Data[Size + 0xF] = 0;

			*(uint32_t*)(Data+Size+0x11) = (*(uint32_t*)(((char*)Object) + 0xA0))|0x80000000;

			Data[Size+0x15] = *(((char*)Item)+0x04);
			Data[Size+0x16] = 0;
			Data[Size+0x17] = 0;
			Data[Size+0x18] = 0;

			Data[Size+0x19] = *(((char*)Item)+0xAF4);
			Data[Size+0x1A] = *(((char*)Item)+0xAF5);
			Data[Size+0x1B] = *(((char*)Item)+0xAF6);
			Data[Size+0x1C] = *(((char*)Item)+0xAF7);

			{
				Data[Size+0x1D] = 0;
				Data[Size+0x1E] = 0;
				Data[Size+0x1F] = 0;
				Data[Size+0x20] = 0;
			}

			Size += 0x21;

		}


		//Add the datavisual 
		std::memcpy ( Data+Size, DataVisual, SizeVisual);
		Size += SizeVisual;
		// extra part of armor
		std::memcpy ( Data+Size, ExtraArmor, extraArmorSize);
		Size += extraArmorSize;

		std::memcpy ( Data+Size, DataColor, DataColorSize);
		Size += DataColorSize;

		std::memcpy ( Data+Size, DataVfxPart, DataVfxPartSize);
		Size += DataVfxPartSize;

		std::memcpy ( Data+Size, TLKName, TLKNameSize);
		Size += TLKNameSize;


		std::memcpy ( Data+Size, equipeObjectSelfEnd+0x2A, 0x16);

		*(uint32_t*)(Data+Size+0x3) = (*(uint32_t*)(((char*)Object) + 0xA0))|0x80000000;


		if(iInventorySlot == INVENTORY_SLOT_CLOAK)
		{


			if (TLKNameSize > 0x38)
			{
				if(DataVfxPartSize>4)
				{
					//name  + vfx
					Data[Size + 0x0B] = 0xA0;
					Data[Size + 0x0E] = 0x2C;
					Data[Size + 0x11] = 0x01;
					Data[Size + 0x12] = 0x60;
				}
				else
				{
					//name only
					Data[Size + 0x0B] = 0xA0;
					Data[Size + 0x0E] = 0x0C;
					Data[Size + 0x12] = 0x60;

				}
			}
			else if (DataVfxPartSize > 4)
			{
				//VFX only
				Data[Size + 0x0B] = 0xE0;
				Data[Size + 0x0E] = 0x36;
				Data[Size + 0x12] = 0xD8;
			}
			else
			{
				//aucun
				Data[Size + 0x0B] = 0xE0;
				Data[Size + 0x0E] = 0x16;
				Data[Size + 0x12] = 0x58;
			}

			Size += 0x13;
			*((unsigned long*)(Data + 3)) = (Size - 0x8);
		}
		else
		{

			if (TLKNameSize > 0x38)
			{
				if(DataVfxPartSize>4)
				{
					//name only + vfx
					Data[Size + 0x0B] = 0x60;
					Data[Size + 0x11] = 0xB0;
					Data[Size + 0x14] = 0x05;
					Data[Size + 0x15] = 0x80;
				}
				else
				{

					//name only
					Data[Size + 0x0B] = 0x60;
					Data[Size + 0x11] = 0x30;
					Data[Size + 0x14] = 0x01;
					Data[Size + 0x15] = 0x80;

				}
			}
			else if (DataVfxPartSize > 4)
			{
				//VFX only
				Data[Size + 0x0B] = 0xA0;
				Data[Size + 0x11] = 0xD8;
				Data[Size + 0x14] = 0x03;
				Data[Size + 0x15] = 0x60;
			}
			else
			{
				//aucun
				Data[Size + 0x0B] = 0xA0;
				Data[Size + 0x11] = 0x58;
				Data[Size + 0x14] = 0x01;
				Data[Size + 0x15] = 0x60;
			}

			
			Size += 0x16;
			*((unsigned long*)(Data + 3)) = (Size - 0xB);
		}



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

bool afficherObjet(int iInventorySlot, int iAffected, int iItem, int mustView)
{
	bool result = false;
	unsigned long Size = 0;
	static unsigned char Data[0x1000];
	static unsigned char DataArmorVisual[0x400];

	memset(Data, 0, sizeof(Data));
	memset(DataArmorVisual, 0, sizeof(DataArmorVisual));

	GameObjectManager m_ObjectManager;
	GameObject        *Object = m_ObjectManager.GetGameObject((NWN::OBJECTID ) iItem ) ;
	GameObject        *CreaPtr = m_ObjectManager.GetGameObject((NWN::OBJECTID ) iAffected ) ;
	NWN::CNWSItem    *Item = Object->AsItem();

	std::memcpy ( Data, equipeArmorP1, 17);
	Size = 17;

	//correction of P1
	*((unsigned long*)(Data + 0x09)) = iAffected|0x80000000;

	unsigned long iSizeArmorVisual = BuildMyChestVisual((uint8_t*)DataArmorVisual, ((uint8_t*)Item + 8)); //Pas tout à fait ça normalement, mais osef ?

	//Add the datavisual 
	std::memcpy ( Data+Size, DataArmorVisual, iSizeArmorVisual);
	Size += iSizeArmorVisual;

	Size += Part2Construct(Data + Size, Object, Item, INVENTORY_SLOT_CHEST);

	//Add the datavisual 
	std::memcpy ( Data+Size, DataArmorVisual, iSizeArmorVisual);
	Size += iSizeArmorVisual;


	static unsigned char DataColor[27];
	memset(DataColor, 0, sizeof(DataColor));
	unsigned long DataColorSize = DataColorConstruct(DataColor, Item);
	///////////////////// preparation of armor color + UVSCROLL ////////////////////
	std::memcpy ( Data+Size, DataColor, DataColorSize);
	Size += DataColorSize;


	static unsigned char DataVfxPart[300];
	memset(DataVfxPart, 0, sizeof(DataVfxPart));
	unsigned long DataVfxPartSize = VfxPartConstruct(DataVfxPart, Object, Item);
	std::memcpy ( Data+Size, DataVfxPart, DataVfxPartSize);
	Size += DataVfxPartSize;
	////////////////////////////////////////////////////////////////////////////////

	static unsigned char TLKName[300];
	memset(TLKName, 0, sizeof(TLKName));
	unsigned long TLKNameSize = BuildMyNamePart((uint8_t*) TLKName, (uint8_t*)Item, (uint8_t*)CreaPtr);

	////////////////////////////////////////////////////////////////////////////////

	std::memcpy ( Data+Size, TLKName, TLKNameSize);
	Size += TLKNameSize;



	//Prepare the Part3
	////unsigned char DataPart3[0x2A];
	////std::memcpy (DataPart3, equipeArmorP3, 0x2A);
	//*((unsigned long*)(DataPart3 + 0x06)) = iAffected|0x80000000;

	////std::memcpy ( Data+Size, DataPart3, 0x2A);
	////Size += 0x2A;


	//Add the datavisual 
	std::memcpy ( Data+Size, DataArmorVisual, iSizeArmorVisual);
	Size += iSizeArmorVisual;


	if(iAffected != mustView)
	{
		std::memcpy ( Data+Size, equipeArmorP4, 17);

		*((unsigned long*)(Data + Size + 2)) = iAffected|0x80000000;

		if (TLKNameSize > 0x38 && DataVfxPartSize > 4)
		{
			Data[Size + 0xA] = 0x80;
			Data[Size + 0x10] = 0xB0;

		}
		else if (TLKNameSize > 0x38)
		{
			Data[Size + 0xA] = 0x80;
			Data[Size + 0x10] = 0x30;
		}
		else if (DataVfxPartSize > 0x04)
		{
			Data[Size + 0x10] = 0xD8;
		}

		Size += 20;
		*((unsigned long*)(Data + 3)) = (Size - 10);
	}
	else 
	{
		int iDecalage = 0x0;
		if (TLKNameSize > 0x38 || DataVfxPartSize > 4)
		{
			std::memcpy ( Data+Size, equipeArmorP4Sb, 0x21);
			iDecalage = 0x11;

		}
		else
		{
			std::memcpy ( Data+Size, equipeArmorP4S, 0x32);
			*((unsigned long*)(Data + Size + 0xB)) = iAffected|0x80000000;


			GameObject        *objCreaAffected = m_ObjectManager.GetGameObject((NWN::OBJECTID ) iAffected );

			int  iAC = CalculAndReturnAC((void*)objCreaAffected);
			*(uint16_t*)(Data + Size + 0x11) = (uint16_t)iAC;
			*(uint32_t*)(Data + Size + 0x13) = *(uint32_t*)((char*)objCreaAffected + 0xEE8); //Gold
			*(uint32_t*)(Data + Size + 0x17) = *(uint32_t*)((char*)objCreaAffected + 0xF9C); //Weight carried
		}
		*((unsigned long*)(Data + Size + 0x2)) = iAffected|0x80000000;


		//std::memcpy ( Data+Size, equipeArmorP4Sb, 0x21);

		*(uint32_t*)(Data+Size+0x22 - iDecalage) = (*(uint32_t*)(((char*)Object) + 0xA0))|0x80000000;


		//debut FUN_00551090 
		*(uint32_t*)(Data + Size + 0x26 - iDecalage) = (*(uint32_t*)(((char*)Object) - 0x730 + AmItmType)); // item id
		*(uint32_t*)(Data + Size + 0x2A - iDecalage) = (*(uint32_t*)(((char*)Object) - 0x730 + AmItmIcon)); //icon
		*(uint32_t*)(Data + Size + 0x2E - iDecalage) = (*(uint32_t*)(((char*)Object) - 0x730 + AmItmArmorRulesType)); // 

		Size += 0x32 - iDecalage;

		//Add the datavisual 
		std::memcpy ( Data+Size, DataArmorVisual, iSizeArmorVisual);
		Size += iSizeArmorVisual;


		std::memcpy ( Data+Size, DataColor, DataColorSize);
		Size += DataColorSize;

		std::memcpy ( Data+Size, DataVfxPart, DataVfxPartSize);
		Size += DataVfxPartSize;

		std::memcpy ( Data+Size, TLKName, TLKNameSize);
		Size += TLKNameSize;


		std::memcpy ( Data+Size, equipeArmorSelfEnd, 0x19);
		*(uint32_t*)(Data+Size+0x03) = (*(uint32_t*)(((char*)Object) + 0xA0))|0x80000000;

		if (TLKNameSize > 0x38)
		{
			if(DataVfxPartSize>4)
			{
				Data[Size + 0x0B] = 0x20;
				Data[Size + 0x11] = 0xB0;
				Data[Size + 0x14] = 0x00;
				Data[Size + 0x17] = 0x16;
				Data[Size + 0x18] = 0x58;
			}
			else
			{
				Data[Size + 0x35-0x2A] = 0x20;
				Data[Size + 0x3B-0x2A] = 0x30;
				Data[Size + 0x3E-0x2A] = 0x00;
				Data[Size + 0x41-0x2A] = 0x06;
				Data[Size + 0x42-0x2A] = 0x00;

			}
		}
		else if (DataVfxPartSize > 4)
		{
			Data[Size + 0x0B] = 0x60;
			Data[Size + 0x11] = 0xD8;
			Data[Size + 0x14] = 0x00;
			Data[Size + 0x17] = 0x0D;
			Data[Size + 0x18] = 0x98;
		}

		Size += 0x19;



		*((unsigned long*)(Data + 3)) = (Size - 0xE);
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

bool afficherObjetMain(int iInventorySlot, int iAffected, int iItem, int mustView)
{
	bool result = false;
	bool bTLK = true;
	unsigned long Size = 0;
	static unsigned char Data[0x1000];
	memset(Data, 0, sizeof(Data));

	GameObjectManager m_ObjectManager;
	GameObject        *Object = Object = m_ObjectManager.GetGameObject((NWN::OBJECTID ) iItem ) ;
	GameObject        *ObjCrea = m_ObjectManager.GetGameObject((NWN::OBJECTID ) iAffected ) ;
	NWN::CNWSItem    *Item = Object->AsItem();

	uint8_t item_id = *(((uint8_t*)Item) + 0x4);
	uint8_t* myBaseItem = GetBaseItemFromId(item_id);
	if (myBaseItem == 0x0)
		return false;
	int iModelType = myBaseItem[0x1C];

	std::memcpy ( Data, equipeHandP1, 0x1C);
	Size = 0x1C;

	unsigned char hand;
	
	if(iInventorySlot == INVENTORY_SLOT_LEFTHAND)
		hand = 0x20;
	else
		hand = 0x10;

	//correction of P1
	*((unsigned long*)(Data + 0x09)) = iAffected|0x80000000;
	Data[0x17] = hand;
	*(uint32_t*)(Data+Size) = (*(uint32_t*)(((char*)Object) + 0xA0))|0x80000000;
	Size += 4;
	Data[Size] = hand;
	Data[Size+4] = *(((unsigned char*)Item) + 0x4);
	Size += 8;
	//iconeetc
	Data[Size] = *(((unsigned char*)Item) + 0xAF4);
	Data[Size+1] = *(((unsigned char*)Item) + 0xAF5);
	Data[Size+2] = *(((unsigned char*)Item) + 0xAF6);
	Data[Size+3] = *(((unsigned char*)Item) + 0xAF7);
	Size += 4;

	Data[Size] = *(((unsigned char*)Item) + 0xAA4);
	Size++;
	if (iModelType == 2)
	{
		Data[Size] = *(((unsigned char*)Item) + AmItmWpnPart2);
		Data[Size+1] = *(((unsigned char*)Item) + AmItmWpnPart3);
		Size += 2;
	}

	static unsigned char DataColor[27];
	memset(DataColor, 0, sizeof(DataColor));
	unsigned long DataColorSize = DataColorConstruct(DataColor, Item);
	std::memcpy ( Data+Size, DataColor, DataColorSize);
	Size += DataColorSize;
	


	static unsigned char DataVfxPart[300];
	memset(DataVfxPart, 0, sizeof(DataVfxPart));
	unsigned long DataVfxPartSize = VfxPartConstruct(DataVfxPart, Object, Item);
	std::memcpy ( Data+Size, DataVfxPart, DataVfxPartSize);
	Size += DataVfxPartSize;
	
	//TLK 
	static unsigned char TLKName[300];
	memset(TLKName, 0, sizeof(TLKName));
	unsigned long TLKNameSize = BuildMyNamePart((uint8_t*)TLKName, (uint8_t*)Item, (uint8_t*)ObjCrea);
	std::memcpy ( Data+Size, TLKName, TLKNameSize);
	Size += TLKNameSize;


	std::memcpy ( Data + Size, equipeHandP2, 0x0C);
	*(uint32_t*)(Data+Size+0x02) = iAffected|0x80000000;
	Size += 0x0C;

	if (mustView == iAffected)
	{
		Size -= 0x02;
		// ajout de trucs
		std::memcpy(Data + Size, equipeHandSelf1, 0x7);
		Data[Size + 3] = hand;

		Size += 0x7;
		*(uint32_t*)(Data+Size) = (*(uint32_t*)(((char*)Object) + 0xA0))|0x80000000;
		Size += 4;
		Data[Size] = *(((unsigned char*)Item) + 0x4);
		Size += 4;

		//iconeetc
		Data[Size] = *(((unsigned char*)Item) + 0xAF4);
		Data[Size+1] = *(((unsigned char*)Item) + 0xAF5);
		Data[Size+2] = *(((unsigned char*)Item) + 0xAF6);
		Data[Size+3] = *(((unsigned char*)Item) + 0xAF7);
		Size += 4;

		Data[Size] = *(((unsigned char*)Item) + AmItmWpnPart1);
		Size++;
		if (iModelType == 2)
		{
			Data[Size] = *(((unsigned char*)Item) + AmItmWpnPart2);
			Data[Size+1] = *(((unsigned char*)Item) + AmItmWpnPart3);
			Size += 2;
		}

		//unsigned long DataColorSize = DataColorConstruct(DataColor, Item);
		std::memcpy ( Data+Size, DataColor, DataColorSize);
		Size += DataColorSize;

		std::memcpy ( Data+Size, DataVfxPart, DataVfxPartSize);
		Size += DataVfxPartSize;

		std::memcpy ( Data+Size, TLKName, TLKNameSize);
		Size += TLKNameSize;

		std::memcpy(Data + Size, equipeHandSelf2, 0x03);
		Size += 0x03;

		*(uint32_t*)(Data+Size) = (*(uint32_t*)(((char*)Object) + 0xA0))|0x80000000;
		Size += 4;

		Data[Size+1] = 0xF; //emplacement inventaire, on tape sur l'emplacement 3840 pour être sûr
		Size += 4;

		if (TLKNameSize > 0x38 && DataVfxPartSize > 0x4)
		{
			Data[Size] = 0x2B;
			Data[Size + 1] = 0x16;
			Data[Size + 2] = 0x00;
		}
		else if (DataVfxPartSize > 0x4)
		{
			Data[Size] = 0x6D;
			Data[Size + 1] = 0x8D;
			Data[Size + 2] = 0x80;
		}
		else if (TLKNameSize > 0x38)
		{
			Data[Size] = 0x23;
			Data[Size + 1] = 0x06;
			Data[Size + 2] = 0x80;
		}
		else
		{
			Data[Size] = 0x65;
			Data[Size + 1] = 0x85;
			Data[Size + 2] = 0x00;
		}


		if (iModelType == 0)
		{
			Data[Size + 2] += 0x10;
		}

		Size += 3;
		*((unsigned long*)(Data + 3)) = (Size - 3);
	}
	else
	{
		if (TLKNameSize > 0x38 && DataVfxPartSize > 0x4)
		{
			Data[Size-2] = 0x4B;
			Data[Size -1] = 0x06;
		}
		else if (DataVfxPartSize > 0x4)
		{
			Data[Size-2] = 0x6D;
			Data[Size -1] = 0x8D;
			if (iModelType == 2)
				Data[Size - 1] = 0x96;
		}
		else if (TLKNameSize > 0x38)
		{
			Data[Size-2] = 0x43;
			Data[Size -1] = 0x0D;
		}
		else
		{
			Data[Size-2] = 0x65;
			Data[Size -1] = 0x96;
		}
		*((unsigned long*)(Data + 3)) = (Size - 2);
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


	Size = 0;
	memset(Data, 0, sizeof(Data));

	Data[0x00] = 0x50;
	Data[0x01] = 0x05;
	Data[0x02] = 0x01;

	Data[0x07] = 0x50;
	Data[0x08] = 0x05;

	*((unsigned long*)(Data + 0x09)) = iAffected|0x80000000;

	Data[0x0D] = 0x00;
	Data[0x0E] = 0x02;
	Data[0x0F] = 0x00;
	Data[0x10] = 0x00;
	Data[0x11] = 0x01;
	Size = 0x12;


	Data[Size] = 0x55;
	Size++;
	*(uint32_t*)(Data+Size) = (*(uint32_t*)(((char*)Object) + 0xA0))|0x80000000;
	Size += 4;

	Data[Size] = hand;
	Size += 4;

	Data[Size] = *((unsigned char*)Item + 0xB02);

	Size++;

	if(DataVfxPartSize > 0x4)
	{
		std::memcpy ( Data+Size, DataVfxPart+1, DataVfxPartSize-1);
		Size += DataVfxPartSize-1;
	}

	Data[Size] = 0x55;
	Size++;
	Data[Size] = 0x05;
	Size++;

	*((unsigned long*)(Data + Size)) = iAffected|0x80000000;

	Size += 4;
	Size += 4;

	if(DataVfxPartSize > 0x4)
	{
		Data[Size] = 0x9D;
	}
	else
	{
		Data[Size] = 0x8D;
	}
	Size++;

	*((unsigned long*)(Data + 0x03)) = Size - 1;

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


bool refreshSelfBelt(int iAffected, int mustView)
{
	bool result = false;
	static unsigned char DataToSend[0x1000];
	memset(DataToSend, 0, sizeof(DataToSend));
	static unsigned char DataCentralBloc[0x300];
	memset(DataCentralBloc, 0, sizeof(DataCentralBloc));
	unsigned long CentralSize = 0;
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

	removeVisual(INVENTORY_SLOT_BELT, iAffected, iAffected);

	{
		NWN::OBJECTID       ObjectId;
		GameObject        *Object;
		GameObjectManager m_ObjectManager;

		ObjectId = (NWN::OBJECTID) iAffected;

		if ((ObjectId & NWN::INVALIDOBJID) != 0)
			ObjectId &= ~(NWN::LISTTYPE_MASK);

		if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
			return true;

		if (Object->GetObjectType() != NWN::OBJECT_TYPE::OBJECT_TYPE_CREATURE)
			return true;

		uint32_t* objEquiped = *((uint32_t**)(((char*)Object) + AmCrtCSEquipedObjectTable));
		//we pass the 98 2F 80 00 and other object to go to belt
		objEquiped[11] = NWN::INVALIDOBJID;
		refreshNude(iAffected, iAffected);
		uint32_t iArmor = getArmorEquiped(iAffected);
		if(iArmor != 0)
			refreshItemApp(INVENTORY_SLOT_CHEST, iAffected, iArmor, iAffected);
		objEquiped[11] = idBelt;
	}

	//preparation of Visual
	static unsigned char DataVisual[0x200];
	memset(DataVisual, 0, sizeof(DataVisual));
	unsigned long SizeVisual;
	

	//prepare the extra part of armor
	static unsigned char ExtraArmor[75];
	memset(ExtraArmor, 0, sizeof(ExtraArmor));
	unsigned long extraArmorSize;

	if (iModelType == 0 || iModelType == 1)
	{
		extraArmorSize = 0;
		SizeVisual = 0;
		DataVisual[0] = *(unsigned char*)(((uint8_t*)Item) + 0xAF4);
	}
	else
	{
		SizeVisual = DataVisualConstructor(DataVisual, Item);
		extraArmorSize = ExtraArmorPartConstruct(ExtraArmor, Item);
	}


	//prepare the color
	static unsigned char DataColor[27];
	memset(DataColor, 0, sizeof(DataColor));
	unsigned long DataColorSize = DataColorConstruct(DataColor, Item);

	//prepare vfx part
	static unsigned char DataVfxPart[300];
	memset(DataVfxPart, 0, sizeof(DataVfxPart));
	unsigned long DataVfxPartSize = VfxPartConstruct(DataVfxPart, itObject, Item);

	//prepare name part
	static unsigned char TLKName[300];
	memset(TLKName, 0, sizeof(TLKName));
	unsigned long TLKNameSize = BuildMyNamePart((uint8_t*) TLKName, (uint8_t*)Item, (uint8_t*)iAffectObject);

	if (!(TLKNameSize > 0x38) && (DataVfxPartSize > 0x4))
	{
		DataToSend[0x0] = 0x50;
		DataToSend[0x1] = 0x05;
		DataToSend[0x2] = 0x01;
		//DataToSend[0x3] = 0x50;

		//DataToSend[0x4] = 0x50;
		//DataToSend[0x5] = 0x50;
		//DataToSend[0x6] = 0x50;
		DataToSend[0x7] = 0x41;

		DataToSend[0x8] = 0x06;

		*((unsigned long*)(DataToSend + 0x09)) = idBelt|0x80000000;
		//DataToSend[0x9] = 0x50;
		//DataToSend[0xA] = 0x50;
		//DataToSend[0xB] = 0x50;

		//DataToSend[0xC] = 0x50;
		//////////////////////////DataToSend[0xD] = *(((char*)Item) + AmItmType);
		*(uint32_t*)(DataToSend + 0xD) = *(uint32_t*)(((char*)Item) + AmItmType);
		//DataToSend[0xE] = 0x50;
		//DataToSend[0xF] = 0x50;

		//DataToSend[0x10] = 0x50;
		*(uint32_t*)(DataToSend + 0x11) = *(uint32_t*)(((char*)Item) + AmItmIcon);

		//DataToSend[0x15] = 0x50;
		//DataToSend[0x16] = 0x50;
		//DataToSend[0x17] = 0x50;

		//DataToSend[0x18] = 0x50;
		Size = 0x19;
	}
	else
	{

		std::memcpy(DataToSend, deposeEquipment, 0x30);

		*((unsigned long*)(DataToSend + 0x09)) = iAffected|0x80000000;

		*((unsigned long*)(DataToSend + 0x20)) = idBelt|0x80000000;

		DataToSend[0x24] = 0x15;

		*(uint32_t*)(DataToSend + 0x28) = *(uint32_t*)(((char*)Item) + AmItmIcon);

		Size = 0x30;
	}


	// 
	std::memcpy ( DataToSend+Size, DataVisual, SizeVisual);
	Size += SizeVisual;

	// extra part of armor
	std::memcpy ( DataToSend+Size, ExtraArmor, extraArmorSize);
	Size += extraArmorSize;

	std::memcpy ( DataToSend+Size, DataColor, DataColorSize);
	Size += DataColorSize;

	std::memcpy ( DataToSend+Size, DataVfxPart, DataVfxPartSize);
	Size += DataVfxPartSize;

	////////////////////////////////////////////////////////////////////////////////

	std::memcpy ( DataToSend+Size, TLKName, TLKNameSize);
	Size += TLKNameSize;





	/* ********************************************************* */

	DataToSend[Size] = 0x55;
	DataToSend[Size + 1] = 0x06;
	Size += 2;

	*(uint32_t*)(DataToSend+Size) = (*(uint32_t*)(((char*)itObject) + 0xA0))|0x80000000;


	Size += 4;

	DataToSend[Size] = 0xB3;
	DataToSend[Size + 1] = 0xFF;
	DataToSend[Size + 2] = 0xFF;
	DataToSend[Size + 3] = 0xF7;

	Size = Size + 0x4;
	*(float*)(DataToSend+Size) = iAffectObject->m_WorldTrans.m[3][0];
	Size = Size + 0x4;
	*(float*)(DataToSend+Size) = iAffectObject->m_WorldTrans.m[3][1];
	Size = Size + 0x4;
	*(float*)(DataToSend+Size) = iAffectObject->m_WorldTrans.m[3][2];
	Size = Size + 0x4;

	std::memcpy ( ((uint8_t*)DataToSend) + Size, TLKName+6, TLKNameSize-0x2E-6);



	//BuildMyNamePart(, (uint8_t*)Item, (uint8_t*)ObjectC);
	Size += TLKNameSize - 0x2E-6;

	Size += 0x8;


	DataToSend[Size] = 0xFF;
	DataToSend[Size + 1] = 0x7F;
	DataToSend[Size + 2] = 0xFF;
	DataToSend[Size + 3] = 0xBF;
	DataToSend[Size + 4] = 0xFF;
	DataToSend[Size + 5] = 0x7F;

	unsigned long blocSize;

	if (iModelType < 2)
	{
		if (TLKNameSize > 0x38 && DataVfxPartSize > 0x4)
		{
			DataToSend[Size + 0x27] = 0xC0;
			DataToSend[Size + 0x2A] = 0x16;
			DataToSend[Size + 0x2B] = 0x08;

		}
		else if (DataVfxPartSize > 0x4)
		{
			DataToSend[Size + 0x27] = 0xE0;
			DataToSend[Size + 0x2A] = 0x36;
			DataToSend[Size + 0x2B] = 0x44;
		}
		else if (TLKNameSize > 0x38)
		{
			DataToSend[Size + 0x27] = 0xC0;
			DataToSend[Size + 0x2A] = 0x06;
			DataToSend[Size + 0x2B] = 0x0A;
		}
		else
		{
			DataToSend[Size + 0x2A] = 0x0B;
			DataToSend[Size + 0x2B] = 0x22;
		}

		Size += 0x2C;
		blocSize = Size - 5;
	}
	else {

		blocSize = Size + 0x27;
		if (TLKNameSize > 0x38 && DataVfxPartSize > 0x4)
		{
			DataToSend[Size + 0x27] = 0x05;
			DataToSend[Size + 0x28] = 0x82;
			Size += 0x29;
		}
		else if (DataVfxPartSize > 0x4)
		{
			DataToSend[Size + 0x27] = 0x46;
			DataToSend[Size + 0x28] = 0xC8;
			DataToSend[Size + 0x29] = 0xA0;
			Size += 0x2A;
		}
		else if (TLKNameSize > 0x38)
		{
			DataToSend[Size + 0x27] = 0x01;
			DataToSend[Size + 0x28] = 0x82;
			Size += 0x29;
		}
		else
		{
			DataToSend[Size + 0x27] = 0x42;
			DataToSend[Size + 0x28] = 0xC8;
			DataToSend[Size + 0x29] = 0xA0;
			Size += 0x2A;
		}
	}


	*((unsigned long*)(DataToSend + 3)) = blocSize;


	deposeObject(INVENTORY_SLOT_BELT, iAffected, mustView, idBelt);



	////////////// BELT DROPPED ///////////////



	ramasserObjet(iAffected, mustView, idBelt);




	////////////// TAKED BACK ///////////////

	memset(DataToSend, 0, sizeof(DataToSend));

	DataToSend[0] = 0x50;
	DataToSend[1] = 0x05;
	DataToSend[2] = 0x01;
	DataToSend[3] = 0x00;
	DataToSend[4] = 0x00;
	DataToSend[5] = 0x00;
	DataToSend[6] = 0x00;
	DataToSend[7] = 0x47;
	DataToSend[8] = 0x49;
	DataToSend[9] = 0x41;
	DataToSend[0xA] = 0x00;
	DataToSend[0xB] = 0x04;
	DataToSend[0xC] = 0x00;
	DataToSend[0xD] = 0x00;

	*((unsigned long*)(DataToSend + 0x0E)) = idBelt|0x80000000;


	//////////////////////////DataToSend[0x12] = *(((char*)Item) + AmItmType);
	*(uint32_t*)(DataToSend + 0x12) = *(uint32_t*)(((char*)Item) + AmItmType);

	DataToSend[0x16] = *(((char*)Item)+0xAF4);
	DataToSend[0x17] = *(((char*)Item)+0xAF5);
	DataToSend[0x18] = *(((char*)Item)+0xAF6);

	DataToSend[0x19] = *(((char*)Item)+0xAF7);

	Size = 0x1E;


	// 
	std::memcpy ( DataToSend+Size, DataVisual, SizeVisual);
	Size += SizeVisual;

	// extra part of armor
	std::memcpy ( DataToSend+Size, ExtraArmor, extraArmorSize);
	Size += extraArmorSize;

	std::memcpy ( DataToSend+Size, DataColor, DataColorSize);
	Size += DataColorSize;

	std::memcpy ( DataToSend+Size, DataVfxPart, DataVfxPartSize);
	Size += DataVfxPartSize;

	std::memcpy ( DataToSend+Size, TLKName, TLKNameSize);
	Size += TLKNameSize;



	/*
	std::memcpy(DataToSend + 14, DataCentralBloc, Size);
	*((unsigned long*)(DataToSend + 3)) = Size+25;*/

	DataToSend[Size] = 0x47;
	DataToSend[Size+1] = 0x52;
	DataToSend[Size+0x2] = 0x44;

	*((unsigned long*)(DataToSend + Size + 0x3)) = idBelt|0x80000000;

	DataToSend[Size+0x7] = 0x00;
	DataToSend[Size+0x8] = 0x0F;
	DataToSend[Size+0x9] = 0x00;
	DataToSend[Size+0xA] = 0x00;


	Size += 0xB;



	*((unsigned long*)(DataToSend + 3)) = Size;
	if (iModelType < 2)
	{
		if (TLKNameSize > 0x38 && DataVfxPartSize > 0x4)
		{
			DataToSend[Size] = 0x4B;
			DataToSend[Size + 0x01] = 0x1F;
		}
		else if (DataVfxPartSize > 0x4)
		{
			DataToSend[Size] = 0x6D;
			DataToSend[Size + 0x01] = 0x9F;
		}
		else if (TLKNameSize > 0x38)
		{
			DataToSend[Size] = 0x4F;
			DataToSend[Size + 0x01] = 0x1F;
		}
		else
		{
			DataToSend[Size] = 0x65;
			DataToSend[Size + 0x01] = 0x9F;
		}
		Size += 2;
	}
	else
	{
		if (TLKNameSize > 0x38 && DataVfxPartSize > 0x4)
		{
			DataToSend[Size + 0x03] = 0x2C;
			Size += 4;

		}
		else if (DataVfxPartSize > 0x4)
		{
			DataToSend[Size] = 0x20;
			DataToSend[Size + 0x03] = 0x36;
			DataToSend[Size + 0x04] = 0x44;
			Size += 5;
		}
		else if (TLKNameSize > 0x38)
		{
			DataToSend[Size + 0x03] = 0x0C;
			Size += 4;
		}
		else
		{
			DataToSend[Size] = 0x20;
			DataToSend[Size + 0x03] = 0x16;
			DataToSend[Size + 0x04] = 0x44;
			Size += 5;
		}
	}




	if (iMustView != NWN::PLAYERID_INVALIDID)
	{
		unsigned char* mesData = DataToSend;
		result = PrepaSendMessageToPlayer(iMustView, mesData, Size, 0);
	}


	////////////  SHOWED ////////////////
	{
		NWN::OBJECTID       ObjectId;
		GameObject        *Object;
		GameObjectManager m_ObjectManager;

		ObjectId = (NWN::OBJECTID) iAffected;

		if ((ObjectId & NWN::INVALIDOBJID) != 0)
			ObjectId &= ~(NWN::LISTTYPE_MASK);

		if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
			return true;

		if (Object->GetObjectType() != NWN::OBJECT_TYPE::OBJECT_TYPE_CREATURE)
			return true;

		uint32_t* objEquiped = *((uint32_t**)(((char*)Object) + AmCrtCSEquipedObjectTable));
		//we pass the 98 2F 80 00 and other object to go to belt
		objEquiped[11] = NWN::INVALIDOBJID;
		refreshNude(iAffected, iAffected);
		uint32_t iArmor = getArmorEquiped(iAffected);
		if(iArmor != 0)
			refreshItemApp(INVENTORY_SLOT_CHEST, iAffected, iArmor, iAffected);
		objEquiped[11] = idBelt;
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
			char uGender = ((char*)Object)[AmCrtGenderVis];
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
					if (iInventorySlot == INVENTORY_SLOT_CHEST)
					{						
						return afficherObjet(iInventorySlot, iAffected, iItem, mustView);
					}
					else if (iInventorySlot == INVENTORY_SLOT_LEFTHAND || iInventorySlot == INVENTORY_SLOT_RIGHTHAND)
					{
						return afficherObjetMain(iInventorySlot, iAffected, iItem, mustView);
					}
					else
						return afficherObjet2(iInventorySlot, iAffected, iItem, mustView);
				}
			}
		}
	}
	return false;
}

void finishRemove(uint32_t idCrea, uint8_t* ptrItm)
{
	char RemovePart[0xC] = { (char)0x50, (char)0x0C, (char)0x07, (char)0x0B, (char)0x00, 
		(char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x00, (char)0x88 };
	uint32_t idObjct = *(uint32_t*)(ptrItm + 0x730 + 0xA0);
	idObjct = idObjct | 0x80000000;
	
	*(uint32_t*)((uint8_t*)RemovePart + 0x7) = idObjct;

	unsigned long iMustView = GetObjectToPlayerId(idCrea);
	if (iMustView != NWN::PLAYERID_INVALIDID)
	{
		unsigned char* mesData = (unsigned char*)RemovePart;
		PrepaSendMessageToPlayer(iMustView, mesData, 0xC, 0);
	}

	//char DataRemove[0x17] = { 0x50, 0x05, 0x01, 0x16, 0x00, 0x00, 0x00, 0x47, 0x52, 0x55, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xBD };
	//Set ID object a 0x0A 
	//Set calcul chiant à 0x0x12

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

void refreshForEveryConcerned(uint32_t iAffected, bool bSmall)
{
	uint32_t iMustView = GetPCIntern(true, false);
	uint32_t iArmor = getArmorEquiped(iAffected);
	while (iMustView != NWN::INVALIDOBJID)
	{
		if (IsKnowByPlayer(GetObjectToPlayerId(iMustView), iAffected) || iMustView == iAffected)
		{
			if(bSmall)
			{
				//Must not happens, but.. eh.
				if (iArmor == 0) return;
					if (deposeObject(INVENTORY_SLOT_CHEST, iAffected, iMustView, iArmor))
					{
						if (ramasserObjet(iAffected, iMustView, iArmor))
						{
						}
					}
			}
			else
			{
				//50 05 01 16 00 00 00 47 52 55 D6 00 00 80 01 00 00 00 72 C6 00 00 BD
				if (iArmor == 0)
				{
					refreshNude(iAffected, iMustView);
				}
				else
				{
					refreshItemApp(INVENTORY_SLOT_CHEST, iAffected, iArmor, iMustView);
				}
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



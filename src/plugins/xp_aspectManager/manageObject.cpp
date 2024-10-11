#include <cstdint>
#include <string>
#include "../../septutil/NwN2DataPos.h"
#include "manageObject.h"
#include "../../NWN2Lib/NWN2.h"
#include "../../NWN2Lib/NWN2Common.h"
#include "aspectManagerUtils.h"

//HP Stuff
int GetHP(GameObject* Object) {
	return *(int*)(((char*)Object) + AmCurrentHP);
}

void SetHP(GameObject* Object, int newHP) {
	*(int*)(((char*)Object) + AmCurrentHP) = newHP & 0xFFFF;
}

int GetMaxHP(GameObject* Object) {
	return *(int*)(((char*)Object) + AmMaxHP);
}

void SetMaxHP(GameObject* Object, int newHP) {
	*(int*)(((char*)Object) + AmMaxHP) = newHP & 0xFFFF;
}

int GetTempHP(GameObject* Object) {
	return *(int*)(((char*)Object) + AmTempHP);
}

void SetTempHP(GameObject* Object, int newHP) {
	*(int*)(((char*)Object) + AmTempHP) = newHP & 0xFFFF;
}

int GetModMaxHP(GameObject* Object) {
	return *(int*)(((char*)Object) + AmModMaxHP);
}

void SetModMaxHP(GameObject* Object, int newHP) {
	*(int*)(((char*)Object) + AmModMaxHP) = newHP & 0xFFFF;
}


//Appearance (Door/Placeable)
int GetAppearance(GameObject* Object) {
	int iType = Object->GetObjectType();

	if (iType == NWN::OBJECT_TYPE_PLACEABLE)
		return *(uint16_t*)((char*)Object + AmPlcApp);
	else if (iType == NWN::OBJECT_TYPE_DOOR)
		return  *(uint16_t*)((char*)Object + AmDApparence);
	else
		return 0;
}

void SetAppearance(GameObject* Object, int iApp) {
	int iType = Object->GetObjectType();

	if (iType == NWN::OBJECT_TYPE_PLACEABLE)
		*(uint16_t*)((char*)Object + AmPlcApp) = (uint16_t)(iApp & 0xFFFF);
	else if (iType == NWN::OBJECT_TYPE_DOOR)
		*(uint16_t*)((char*)Object + AmDApparence) = (uint16_t)(iApp & 0xFFFF);
}


//Lock
int GetIsAutoRemoveKey(GameObject* Object) {
	int iType = Object->GetObjectType();

	if (iType == NWN::OBJECT_TYPE_PLACEABLE) {
		int iRet = *(int*)((char*)Object + AmPlcIsAutoRemoveKey);
		return (iRet & 0xFF);
	} else if (iType == NWN::OBJECT_TYPE_DOOR) {
		int iRet = *(int*)((char*)Object + AmDIsAutoRemoveKey);
		return (iRet & 0xFF);
	}

	return 0;
}

void SetIsAutoRemoveKey(GameObject* Object, int bAutoRemK) {
	int iType = Object->GetObjectType();

	if (iType == NWN::OBJECT_TYPE_PLACEABLE)
		*(int*)((char*)Object + AmPlcIsAutoRemoveKey) = bAutoRemK;
	else if (iType == NWN::OBJECT_TYPE_DOOR)
		*(int*)((char*)Object + AmDIsAutoRemoveKey) = bAutoRemK;
}


//Inventory (Placeable only)
struct inventoryStruct
{
	int32_t id;
	int32_t maxSize; //Funny stuff, the hard max size is due to client bad implementation.
	int32_t* contentArray;
	int32_t allocatedContent;
};

int GetHasInventory(GameObject* Object) {
	int iType = Object->GetObjectType();
	if (iType == NWN::OBJECT_TYPE_PLACEABLE)
		return *(int*)((char*)Object + AmPlcHasInventory);
	return 0;
}

void SetHasInventory(GameObject* Object, int bHasInventory) {
	int iType = Object->GetObjectType();
	if (iType == NWN::OBJECT_TYPE_PLACEABLE)
		*(int*)((char*)Object + AmPlcHasInventory) = bHasInventory & 0xFF;
}

int GetInventoryMaxSize(GameObject* Object) {
	int iType = Object->GetObjectType();
	if (iType == NWN::OBJECT_TYPE_PLACEABLE) {
		inventoryStruct* invS = *(inventoryStruct**)((char*)Object + AmPlcInventory);
		return invS->maxSize;
	}

	return 0;
}

void SetInventoryMaxSize(GameObject* Object, int iSize) {
	if (iSize > 0x8E)
		iSize = 0x8E;

	int iType = Object->GetObjectType();
	if (iType == NWN::OBJECT_TYPE_PLACEABLE) {
		inventoryStruct* invS = *(inventoryStruct**)((char*)Object + AmPlcInventory);
		invS->maxSize = iSize;
	}
}

int GetNumberItemInInventory(GameObject* Object) {

	int iType = Object->GetObjectType();
	if (iType != NWN::OBJECT_TYPE_PLACEABLE)
		return 0;

	inventoryStruct* invS = *(inventoryStruct**)((char*)Object + AmPlcInventory);
	if(invS == NULL)
		return 0;

	int32_t maxCount = invS->allocatedContent;
	int32_t* arrayItem = invS->contentArray;
	int count = 0;

	if (arrayItem != 0)
	{
		for (int i = 0; i < maxCount; i++)
		{
			if(arrayItem[i] != 0x7F000000)
				count++;
		}
	}

	return 0;
}


/*
	void switchInventory(int nObj1, int nObj2)
	{
		inventoryStruct* inv1 = *((inventoryStruct**)pointedPlaceableValue("invPtr", nObj1));
		inventoryStruct* inv2 = *((inventoryStruct**)pointedPlaceableValue("invPtr", nObj2));

		int32_t ptr1 = *(int32_t*)pointedPlaceableValue("invPtr", nObj1);
		int32_t ptr2 = *(int32_t*)pointedPlaceableValue("invPtr", nObj2);

		inv1->id = nObj2;
		inv2->id = nObj1;

		*((int32_t*)pointedPlaceableValue("invPtr", nObj1)) = ptr2;
		*((int32_t*)pointedPlaceableValue("invPtr", nObj1)) = ptr1;
	}
*/


//Transitions
int GetIsInvisibleTransition(GameObject* Object) {
	int iType = Object->GetObjectType();

	if (iType != NWN::OBJECT_TYPE_DOOR)
		return 0;

	return *((uint8_t*)Object + AmDIsInvisibleTrans);	
}

void SetIsInvisibleTransition(GameObject* Object, int bInvisibleTransition) {
	int iType = Object->GetObjectType();

	if (iType != NWN::OBJECT_TYPE_DOOR)
		return;

	*((uint8_t*)Object + AmDIsInvisibleTrans) = (uint8_t)(bInvisibleTransition & 0xFF);
}

int GetIsPartyTransition(GameObject* Object) {
	int iType = Object->GetObjectType();

	if (iType == NWN::OBJECT_TYPE_DOOR)
		return *((uint8_t*)Object + AmDIsGroupTransition);
	else if (iType == NWN::OBJECT_TYPE_TRIGGER)
		return *((uint8_t*)Object + AmTrigPartyTransition);

	return 0;
}

void SetIsPartyTransition(GameObject* Object, int bIsGroupTransition) {
	int iType = Object->GetObjectType();

	if (iType == NWN::OBJECT_TYPE_DOOR)
		*((uint8_t*)Object + AmDIsGroupTransition) = (uint8_t)(bIsGroupTransition & 0xFF);
	else if(iType == NWN::OBJECT_TYPE_TRIGGER)
		*((uint8_t*)Object + AmTrigPartyTransition) = (uint8_t)(bIsGroupTransition & 0xFF);
}

int GetTransitionFlags(GameObject* Object) {
	int iType = Object->GetObjectType();

	if (iType == NWN::OBJECT_TYPE_DOOR)
		return *((uint8_t*)Object + AmDTransitionType);
	else if (iType == NWN::OBJECT_TYPE_TRIGGER)
		return *((uint8_t*)Object + AmTrigTransitionType);

	return 0;
}

void SetTransitionFlags(GameObject* Object, int transitionFlags) {
	int iType = Object->GetObjectType();

	if (iType == NWN::OBJECT_TYPE_DOOR)
		*((uint8_t*)Object + AmDTransitionType) = (uint8_t)(transitionFlags & 0xFF);
	else if(iType == NWN::OBJECT_TYPE_TRIGGER)
		*((uint8_t*)Object + AmTrigTransitionType) = (uint8_t)(transitionFlags & 0xFF);
}

int GetLoadScreenID(GameObject* Object) {
	int iType = Object->GetObjectType();

	if (iType == NWN::OBJECT_TYPE_DOOR)
		return *(uint16_t*)((char*)Object + AmDLoadScreenId);
	else if (iType == NWN::OBJECT_TYPE_TRIGGER)
		return *(uint16_t*)((char*)Object + AmTrigLoadScreenID);
	else if (iType == NWN::OBJECT_TYPE_AREA)
		return *(uint16_t*)((char*)Object + AmAreaLoadScreenID);

	return 0;
}

void SetLoadScreenID(GameObject* Object, int iValue) {
	int iType = Object->GetObjectType();
	
	uint16_t shValue = (uint16_t)(iValue & 0xFFFF);
	if (iType == NWN::OBJECT_TYPE_DOOR)
		*(uint16_t*)((char*)Object + AmDLoadScreenId) = shValue;
	else if (iType == NWN::OBJECT_TYPE_TRIGGER)
		*(uint16_t*)((char*)Object + AmTrigLoadScreenID)= shValue;
	else if (iType == NWN::OBJECT_TYPE_AREA)
		*(uint16_t*)((char*)Object + AmAreaLoadScreenID) = shValue;
}

 
std::string GetTransitionDestination(GameObject* Object) {
	std::string sResult = "";
	int iType = Object->GetObjectType();

	NWN::CExoString* exoStr = NULL;

	if (iType == NWN::OBJECT_TYPE_DOOR)
		exoStr = (NWN::CExoString*) (((char*)Object) + AmDTransitionDest);
	else if (iType == NWN::OBJECT_TYPE_TRIGGER)
		exoStr = (NWN::CExoString*) (((char*)Object) + AmTrigTransitionDest);

	if (exoStr != NULL) {
		sResult = ExoStringToString(exoStr);
	}

	return sResult;
}

void SetTransitionDestination(GameObject* Object, char* cValue) {
	int iType = Object->GetObjectType();

	char* ptrExo = NULL;

	if (iType == NWN::OBJECT_TYPE_DOOR)
		ptrExo = (((char*)Object) + AmDTransitionDest);
	else if (iType == NWN::OBJECT_TYPE_TRIGGER)
		ptrExo = (((char*)Object) + AmTrigTransitionDest);


	if (ptrExo != NULL) {
		PutNWNExoString(ptrExo, cValue);
	}
}


//Saving Throw
void SetWillSavingThrow(GameObject* Object, int iValue) {
	uint8_t byteValue = iValue & 0xFF;

	int iType = Object->GetObjectType();
	if (iType == NWN::OBJECT_TYPE_CREATURE) {
		char* ptrAppBlock = creatureApparenceBlock((char*)Object);
		*(ptrAppBlock + AmCrtABModWill) = byteValue;
	}
	else if (iType == NWN::OBJECT_TYPE_DOOR) {
		*(((char*)Object) + AmDWill) = byteValue;
	}
	else if (iType == NWN::OBJECT_TYPE_PLACEABLE) {
		*(((char*)Object) + AmPlcWill) = byteValue;
	}
}

void SetReflexSavingThrow(GameObject* Object, int iValue) {
	uint8_t byteValue = iValue & 0xFF;

	int iType = Object->GetObjectType();
	if (iType == NWN::OBJECT_TYPE_CREATURE) {
		char* ptrAppBlock = creatureApparenceBlock((char*)Object);
		*(ptrAppBlock + AmCrtABModReflex) = byteValue;
	}
	else if (iType == NWN::OBJECT_TYPE_DOOR) {
		*(((char*)Object) + AmDRef) = byteValue;
	}
	else if (iType == NWN::OBJECT_TYPE_PLACEABLE) {
		*(((char*)Object) + AmPlcRef) = byteValue;
	}
}

void SetFortitudeSavingThrow(GameObject* Object, int iValue) {
	uint8_t byteValue = iValue & 0xFF;

	int iType = Object->GetObjectType();
	if (iType == NWN::OBJECT_TYPE_CREATURE) {
		char* ptrAppBlock = creatureApparenceBlock((char*)Object);
		*(ptrAppBlock + AmCrtABModFortitude) = byteValue;
	}
	else if (iType == NWN::OBJECT_TYPE_DOOR) {
		*(((char*)Object) + AmDFort) = byteValue;
	}
	else if (iType == NWN::OBJECT_TYPE_PLACEABLE) {
		*(((char*)Object) + AmPlcFort) = byteValue;
	}
}

//MagicResistance
int GetMagicResistance(GameObject* Object) {
	int iType = Object->GetObjectType();
	if (iType == NWN::OBJECT_TYPE_CREATURE) {
		char* ptrAppBlock = creatureApparenceBlock((char*)Object);
		return *(ptrAppBlock + AmCrtABMagicResistance);
	}
	return 0;
}

void SetMagicResistance(GameObject* Object, int iValue) {
	uint8_t byteValue = iValue & 0xFF;

	int iType = Object->GetObjectType();
	if (iType == NWN::OBJECT_TYPE_CREATURE) {
		char* ptrAppBlock = creatureApparenceBlock((char*)Object);
		*(ptrAppBlock + AmCrtABMagicResistance) = byteValue;
	}
}


int GetMagicResistanceReduc(GameObject* Object) {
	int iType = Object->GetObjectType();
	if (iType == NWN::OBJECT_TYPE_CREATURE) {
		char* ptrAppBlock = creatureApparenceBlock((char*)Object);
		return *(ptrAppBlock + AmCrtABReducMagicResistance);
	}
	return 0;
}

void SetMagicResistanceReduc(GameObject* Object, int iValue) {
	uint8_t byteValue = iValue & 0xFF;

	int iType = Object->GetObjectType();
	if (iType == NWN::OBJECT_TYPE_CREATURE) {
		char* ptrAppBlock = creatureApparenceBlock((char*)Object);
		*(ptrAppBlock + AmCrtABReducMagicResistance) = byteValue;
	}
}


int GetFactionID(GameObject* Object) {
	int iType = Object->GetObjectType();
	int iRet = -1;
	
	if (iType == NWN::OBJECT_TYPE_CREATURE) {
		char* ptrAppBlock = creatureApparenceBlock((char*)Object);
		iRet = *(int*)(ptrAppBlock + AmCrtABFactionID);
	}
	else if (iType == NWN::OBJECT_TYPE_PLACEABLE) {
		iRet = *(int*)((char*)Object + AmPlcFactionID);
	}
	else if (iType == NWN::OBJECT_TYPE_DOOR) {
		iRet = *(int*)((char*)Object + AmDFactionID);
	}
	else if (iType == NWN::OBJECT_TYPE_TRIGGER) {
		iRet = *(int*)((char*)Object + AmTrigFactionID);
	}

	return iRet;
}


void SetIsUVScroll(GameObject* Object, bool bValue) {
	int iType = Object->GetObjectType();

	if (iType == NWN::OBJECT_TYPE_PLACEABLE) {
		*(int*)((char*)Object + AmPlcHasUVScroll) = (bValue?1:0);
	}
}

int GetIsUVScroll(GameObject* Object) {
	int iType = Object->GetObjectType();
	int iRet = 0;

	if (iType == NWN::OBJECT_TYPE_PLACEABLE) {
		iRet = *(int*)((char*)Object + AmPlcHasUVScroll);
		if(iRet != 0)
			iRet = 1;
	}
	return iRet;
}

//The sended "update" message is useless and cause severe freeze on client.
// I can't fix the client right now, so just avoid to send this message
void avoidPlacedEffectSEFUpdate(NWN::OBJECTID vfxID, char* cValue) {

	//Start to list PC. 
	//In it, we will search for vfxID

	//Find start point
	uint32_t* tempPtr = *(uint32_t**)0x0086442C;
	tempPtr = (uint32_t*)(tempPtr[1]);
	tempPtr = (uint32_t*)(tempPtr[1]);
	tempPtr = (uint32_t*)(tempPtr[0x1008c / 4]);

	uint32_t* myHeaderBloc = (uint32_t*)(tempPtr[0]); //First one
	uint32_t nbPC = tempPtr[2];

	int iCount = 0;
	//For each PC
	while (iCount < nbPC) {
		uint32_t* myPCBlock = (uint32_t*)myHeaderBloc[2];
		uint32_t* ptrObjList = (uint32_t*)myPCBlock[0x45];
		uint32_t nbObject = ptrObjList[0x3];
		ptrObjList = (uint32_t*)ptrObjList[0];

		for (uint32_t i = 0; i < nbObject; i++) {
			if ((NWN::OBJECTID)ptrObjList[i * 2] == vfxID) {
				char* visuObject = (char*)ptrObjList[i * 2 + 1];
				PutNWNExoString(visuObject + 0x9D0, cValue);
				//We have found our object, no need to continue
				break;
			}
		}

		iCount++;
		myHeaderBloc = (uint32_t*)myHeaderBloc[1];
	}
}

void SetAppearanceSEF(GameObject* Object, char* cValue) {
	int iType = Object->GetObjectType();
	//Seems to only work with those so no need to do stuff for other
	if(iType == NWN::OBJECT_TYPE_CREATURE || iType == NWN::OBJECT_TYPE_PLACEABLE || iType == NWN::OBJECT_TYPE_ITEM) {
		PutNWNExoString(((char*)Object) + AmAppSEF, cValue);
	}
	else if (iType == NWN::OBJECT_TYPE_PLACED_EFFECT) {
		char* ptrVFX = ((char*)Object) + AmVFXSEF;

		if (strlen(cValue) <= 32) {
			memset(ptrVFX, 0, 0x20);
			memcpy_s(ptrVFX, 0x20, cValue, strlen(cValue));
			
			NWN::OBJECTID myID = Object->GetObjectId();
			avoidPlacedEffectSEFUpdate(myID, cValue);
		}
	}
}

std::string GetAppearanceSEF(GameObject* Object) {
	std::string result = "";
	int iType = Object->GetObjectType();
	//Seems to only work with those so no need to do stuff for other
	if(iType == NWN::OBJECT_TYPE_CREATURE || iType == NWN::OBJECT_TYPE_PLACEABLE || iType == NWN::OBJECT_TYPE_ITEM)
		result = ExoStringToString((NWN::CExoString*)(((char*)Object) + AmAppSEF));
	else if (iType == NWN::OBJECT_TYPE_PLACED_EFFECT)
	{
		char* ptrVFX = ((char*)Object) + AmVFXSEF;
		std::string str2(ptrVFX, 0x20);
		result = str2;
	}

	return result;
}


NWN::D3DXCOLOR* GetObjectColorPtr(GameObject* Object, char* cParam1) {
	NWN::D3DXCOLOR* ObjColor = NULL;
	int iType = Object->GetObjectType();
	std::string sParam1(cParam1);

	if(iType == NWN::OBJECT_TYPE_PLACEABLE) {
		ObjColor = (NWN::D3DXCOLOR*)((char*)Object + AmPlcColor);
	} else if (iType == NWN::OBJECT_TYPE_DOOR) {
		ObjColor = (NWN::D3DXCOLOR*)((char*)Object + AmDColor);
	}


	if(ObjColor != NULL) {
		if (sParam1 == "2") {
			ObjColor = (ObjColor +1);
		} else if (sParam1 == "3")  {
			ObjColor = (ObjColor +2);
		}
	}

	return ObjColor;
}

void SetObjectColor(GameObject* Object, char* cParam1, char* cValue) {
	NWN::D3DXCOLOR* ObjColor = GetObjectColorPtr(Object, cParam1);

	if(ObjColor != NULL) {
		NWN::D3DXCOLOR myColor;
		//Color is Ok, we can apply it
		if(StringToColor(cValue, &myColor)) {
			memcpy(ObjColor, &myColor, sizeof(myColor));
		}
	}
}

std::string GetObjectColor(GameObject* Object, char* cParam1) {
	NWN::D3DXCOLOR* ObjColor = GetObjectColorPtr(Object, cParam1);
	std::string sResult = "";

	if(ObjColor != NULL) {
		sResult = ColorToString(ObjColor);
	}

	return sResult;
}


float GetObjectUScroll(GameObject* Object) {
	float fRet = 0;
	int iType = Object->GetObjectType();

	if(iType == NWN::OBJECT_TYPE_PLACEABLE) {
		fRet = *(float*)(((char*)Object) + AmPlcValueUScroll);
	}

	return fRet;
}

float GetObjectVScroll(GameObject* Object) {
	float fRet = 0;
	int iType = Object->GetObjectType();

	if(iType == NWN::OBJECT_TYPE_PLACEABLE) {
		fRet = *(float*)(((char*)Object) + AmPlcValueVScroll);
	}

	return fRet;
}

void SetObjectUScroll(GameObject* Object, float fValue) {
	int iType = Object->GetObjectType();

	if(iType == NWN::OBJECT_TYPE_PLACEABLE) {
		*(float*)(((char*)Object) + AmPlcValueUScroll) = fValue;
	}
}

void SetObjectVScroll(GameObject* Object, float fValue) {
	int iType = Object->GetObjectType();

	if(iType == NWN::OBJECT_TYPE_PLACEABLE) {
		*(float*)(((char*)Object) + AmPlcValueVScroll) = fValue;
	}
}


/* Trigger */

int GetTriggerType(GameObject* Object) {
	int iType = 0;

	if (*(((uint8_t*)Object) + AmTrigTypeTransition) == 1)
		return 1;
	else if (*(((uint8_t*)Object) + AmTrigTypeTrap) == 1)
		return 2;

	return 0;
}

void SetTriggerType(GameObject* Object, int iValue) {
	//SetEverything to 0, then set the right value to 1
	*(((uint8_t*)Object) + AmTrigTypeTransition) = 0;
	*(((uint8_t*)Object) + AmTrigTypeTrap) = 0;
	*(((uint8_t*)Object) + AmTrigTypeWalkmesh) = 0;

	if(iValue == 1)
		*(((uint8_t*)Object) + AmTrigTypeTransition) = 1;
	else if(iValue == 2)
		*(((uint8_t*)Object) + AmTrigTypeTrap) = 1;
}


int ObjectGetInt(char* cCommand, int iObjectID) {
	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;
	std::string		sCommand(cCommand);

	ObjectId = (NWN::OBJECTID) iObjectID;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return 0;
	
	if (sCommand == "hp")
		return GetHP(Object);
	else if (sCommand == "tempHp")
		return GetTempHP(Object);
	else if (sCommand == "maxHp")
		return GetMaxHP(Object);
	else if (sCommand == "modMaxHp")
		return GetModMaxHP(Object);
	else if (sCommand == "Appearance")
		return GetAppearance(Object);

	else if (sCommand == "UVScroll")
		return GetIsUVScroll(Object);

	//Inventory
	else if (sCommand == "HasInventory")
		return GetHasInventory(Object);
	else if (sCommand == "InventoryMaxSize")
		return GetInventoryMaxSize(Object);
	else if (sCommand == "InventoryNbUsed")
		return GetNumberItemInInventory(Object);

	//Lock
	else if (sCommand == "AutoRemoveKey")
		return GetIsAutoRemoveKey(Object);

	//Transition Stuff
	else if (sCommand == "InvisibleTransition")
		return GetIsInvisibleTransition(Object);
	else if (sCommand == "PartyTransition")
		return GetIsPartyTransition(Object);
	else if (sCommand == "TransitionType")
		return GetTransitionFlags(Object);
	else if (sCommand == "LoadScreenID")
		return GetLoadScreenID(Object);

	else if (sCommand == "MagicResistance")
		return GetMagicResistance(Object);
	else if (sCommand == "MagicResistanceReduc")
		return GetMagicResistanceReduc(Object);
	else if (sCommand == "FactionID")
		return GetFactionID(Object);

	else if (sCommand == "TriggerType")
		return GetTriggerType(Object);
	return -1;
}

void ObjectSetInt(char* cCommand, int iObjectID, int iValue) {
	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;
	std::string		sCommand(cCommand);

	ObjectId = (NWN::OBJECTID) iObjectID;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return;

	if (sCommand == "hp")
		return SetHP(Object, iValue);
	else if (sCommand == "tempHp")
		return SetTempHP(Object, iValue);
	else if (sCommand == "maxHp")
		return SetMaxHP(Object, iValue);
	else if (sCommand == "modMaxHp")
		return SetModMaxHP(Object, iValue);
	else if (sCommand == "Appearance")
		return SetAppearance(Object, iValue);

	else if (sCommand == "UVScroll")
		return SetIsUVScroll(Object, iValue != 0);

	//Inventory
	else if (sCommand == "HasInventory")
		return SetHasInventory(Object, iValue);
	else if (sCommand == "InventoryMaxSize")
		return SetInventoryMaxSize(Object, iValue);

	//Lock
	else if (sCommand == "AutoRemoveKey")
		return SetIsAutoRemoveKey(Object, iValue);


	//Transition Stuff
	else if (sCommand == "InvisibleTransition")
		return SetIsInvisibleTransition(Object, iValue);
	else if (sCommand == "PartyTransition")
		return SetIsPartyTransition(Object, iValue);
	else if (sCommand == "TransitionType")
		return SetTransitionFlags(Object, iValue);
	else if (sCommand == "LoadScreenID")
		return SetLoadScreenID(Object, iValue);

	else if (sCommand == "MagicResistance")
		return SetMagicResistance(Object, iValue);
	else if (sCommand == "MagicResistanceReduc")
		return SetMagicResistanceReduc(Object, iValue);

	else if (sCommand == "ReflexSavingThrow")
		SetReflexSavingThrow(Object, iValue);
	else if (sCommand == "FortitudeSavingThrow")
		SetFortitudeSavingThrow(Object, iValue);
	else if (sCommand == "WillSavingThrow")
		SetWillSavingThrow(Object, iValue);

	else if (sCommand == "TriggerType")
		SetTriggerType(Object, iValue);
}



float ObjectGetFloat(char* cCommand, int iObjectID) {
	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;
	std::string		sCommand(cCommand);

	ObjectId = (NWN::OBJECTID) iObjectID;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return 0;


	if (sCommand == "ValueUScroll")
		return GetObjectUScroll(Object);
	else if (sCommand == "ValueVScroll")
		return GetObjectVScroll(Object);

	return 0;
}

void ObjectSetFloat(char* cCommand, int iObjectID, float fValue) {
	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;
	std::string		sCommand(cCommand);

	ObjectId = (NWN::OBJECTID) iObjectID;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return;


	if (sCommand == "ValueUScroll")
		return SetObjectUScroll(Object, fValue);
	else if (sCommand == "ValueVScroll")
		return SetObjectVScroll(Object, fValue);

}


std::string ObjectGetString(std::string cCommand, char* cParam1, int iObjectID) {
	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;
	std::string		sCommand(cCommand);

	ObjectId = (NWN::OBJECTID) iObjectID;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return "";

	if(sCommand == "AppSEF")
		return GetAppearanceSEF(Object);
	else if (sCommand == "TransitionDest")
		return GetTransitionDestination(Object);
	else if (sCommand == "Color")
		return GetObjectColor(Object, cParam1);

	return "";
}

void ObjectSetString(char* cCommand, char* cParam1, int iObjectID, char* sValue) {
	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;
	std::string		sCommand(cCommand);

	ObjectId = (NWN::OBJECTID) iObjectID;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return;

	if(sCommand == "AppSEF")
		SetAppearanceSEF(Object, sValue);
	else if (sCommand == "TransitionDest")
		SetTransitionDestination(Object, sValue);
	else if (sCommand == "Color")
		SetObjectColor(Object, cParam1, sValue);
}


#include "aspectManagerUtils.h"
#include "../../NWN2Lib/NWN2.h"
#include "../../NWN2Lib/NWN2Common.h"
#include "../../septutil/NwN2DataPos.h"

char* GetItemModelPartPtr(char* ItemPtr, int iModelPart) {
	char* retValue = NULL;

	switch(iModelPart) {
		case ARMOR_MODEL_PART_HELM:
			retValue = (ItemPtr + AmItmHelm);
			break;
		case ARMOR_MODEL_PART_GLOVES:
			retValue = (ItemPtr + AmItmGloves);
			break;
		case ARMOR_MODEL_PART_BOOTS:
			retValue = (ItemPtr + AmItmBoots);
			break;
		case ARMOR_MODEL_PART_BELT:
			retValue = (ItemPtr + AmItmBelt);
			break;
		case ARMOR_MODEL_PART_CLOAK:
			retValue = (ItemPtr + AmItmCloak);
			break;
		case ARMOR_MODEL_PART_ARMOR:
			retValue = (ItemPtr + AmItmArmors);
			break;
	}

	return retValue;
}

char* GetItemModelPiecePtr(char* ItemPtr, int iModelPiece) {
	char* retValue = NULL;

	switch(iModelPiece) {
		case ARMOR_MODEL_PIECE_LEFT_SHOULDER: 
			retValue = (ItemPtr + AmItmLShoulder);
			break;
		case ARMOR_MODEL_PIECE_RIGHT_SHOULDER: 
			retValue = (ItemPtr + AmItmRShoulder);
			break;
		case ARMOR_MODEL_PIECE_LEFT_BRACER: 
			retValue = (ItemPtr + AmItmLBracer);
			break;
		case ARMOR_MODEL_PIECE_RIGHT_BRACER: 
			retValue = (ItemPtr + AmItmRBracer);
			break;
		case ARMOR_MODEL_PIECE_LEFT_ELBOW: 
			retValue = (ItemPtr + AmItmLElbow);
			break;
		case ARMOR_MODEL_PIECE_RIGHT_ELBOW: 
			retValue = (ItemPtr + AmItmRElbow);
			break;
		case ARMOR_MODEL_PIECE_LEFT_ARM: 
			retValue = (ItemPtr + AmItmLUpArm);
			break;
		case ARMOR_MODEL_PIECE_RIGHT_ARM: 
			retValue = (ItemPtr + AmItmRUpArm);
			break;
		case ARMOR_MODEL_PIECE_LEFT_HIP: 
			retValue = (ItemPtr + AmItmLHip);
			break;
		case ARMOR_MODEL_PIECE_RIGHT_HIP: 
			retValue = (ItemPtr + AmItmRHip);
			break;
		case ARMOR_MODEL_PIECE_FRONT_HIP: 
			retValue = (ItemPtr + AmItmFHip);
			break;
		case ARMOR_MODEL_PIECE_BACK_HIP: 
			retValue = (ItemPtr + AmItmBHip);
			break;
		case ARMOR_MODEL_PIECE_LEFT_LEG: 
			retValue = (ItemPtr + AmItmLUpLeg);
			break;
		case ARMOR_MODEL_PIECE_RIGHT_LEG: 
			retValue = (ItemPtr + AmItmRUpLeg);
			break;
		case ARMOR_MODEL_PIECE_LEFT_SHIN: 
			retValue = (ItemPtr + AmItmLLowLeg);
			break;
		case ARMOR_MODEL_PIECE_RIGHT_SHIN: 
			retValue = (ItemPtr + AmItmRLowLeg);
			break;
		case ARMOR_MODEL_PIECE_LEFT_KNEE: 
			retValue = (ItemPtr + AmItmLKnee);
			break;
		case ARMOR_MODEL_PIECE_RIGHT_KNEE: 
			retValue = (ItemPtr + AmItmRKnee);
			break;
		case ARMOR_MODEL_PIECE_LEFT_FOOT: 
			retValue = (ItemPtr + AmItmLFoot);
			break;
		case ARMOR_MODEL_PIECE_RIGHT_FOOT: 
			retValue = (ItemPtr + AmItmRFoot);
			break;
		case ARMOR_MODEL_PIECE_LEFT_ANKLE: 
			retValue = (ItemPtr + AmItmLAnkle);
			break;
		case ARMOR_MODEL_PIECE_RIGHT_ANKLE: 
			retValue = (ItemPtr + AmItmRAnkle);
			break;
	}

	return retValue;
}

NWN::D3DXCOLOR* GetItemColorPtr(char* ItmPtr, std::string sCommand) {
	char* retVal = NULL;
		if (sCommand == "BaseColor1")
			retVal = (ItmPtr + AmItmBaseColor);
		else if (sCommand == "BaseColor2")
			retVal = (ItmPtr + AmItmBaseColor + 0x10);
		else if (sCommand == "BaseColor3")
			retVal = (ItmPtr + AmItmBaseColor + 0x20);
		else if (sCommand.rfind(cst_ModelPiece, 0) == 0) {
			sCommand = sCommand.substr(cst_ModelPiece.size());
			char* pEnd;
			int iModelPiece = strtol(sCommand.c_str(), &pEnd, 10);

			sCommand = pEnd;

			char* ptrTemp = GetItemModelPiecePtr(ItmPtr, iModelPiece);
			if (ptrTemp != NULL) {
				if (sCommand == "Color2")
					retVal = (ptrTemp + AmItmModelPieceColor2);
				else if (sCommand == "Color3")
					retVal = (ptrTemp + AmItmModelPieceColor3);
				else
					retVal = (ptrTemp + AmItmModelPieceColor1);
			}
		}
		else if (sCommand.rfind(cst_ModelPart, 0) == 0) {
			sCommand = sCommand.substr(cst_ModelPart.size());
			char* pEnd;
			int iModelPart = strtol(sCommand.c_str(), &pEnd, 10);

			sCommand = pEnd;
			char* ptrTemp = GetItemModelPartPtr(ItmPtr, iModelPart);
			if (ptrTemp != NULL) {
				if (sCommand == "Color2")
					retVal = (ptrTemp + AmItmModelPartColor2);
				else if (sCommand == "Color3")
					retVal = (ptrTemp + AmItmModelPartColor3);
				else
					retVal = (ptrTemp + AmItmModelPartColor1);
			}
		}

	return (NWN::D3DXCOLOR*) retVal;
}


void SetItemBaseItem(char* ItemPtr, int iValue) {
	*(uint32_t*)(ItemPtr + AmItmType) = iValue;
}


void SetItemAppVariation(char* ItemPtr, int iValue) {
	*(uint8_t*)(ItemPtr + AmItmVariation) = (uint8_t)iValue;
}

int GetItemAppVariation(char* ItemPtr) {
	return *(uint8_t*)(ItemPtr + AmItmVariation);
}

void SetItemAppCategory(char* ItemPtr, int iValue) {
	*(uint8_t*)(ItemPtr + AmItmCategory) = (uint8_t)iValue;
}

int GetItemAppCategory(char* ItemPtr) {
	return *(uint8_t*)(ItemPtr + AmItmCategory);
}



void SetItemFXWpn(char* ItemPtr, int iValue) {
	*(uint8_t*)(ItemPtr + AmItmFXWpn) = (uint8_t)iValue;
}

int GetItemFXWpn(char* ItemPtr) {
	return *(uint8_t*)(ItemPtr + AmItmFXWpn);
}



void SetItemModelPieceAccessory(char* ItemPtr, int iModelPiece, int iValue) {
	char* ptrModelPiece = GetItemModelPiecePtr(ItemPtr, iModelPiece);
	if (ptrModelPiece != NULL)
		*(uint8_t*)ptrModelPiece = (uint8_t)iValue;
}

int GetItemModelPieceAccessory(char* ItemPtr, int iModelPiece) {
	char* ptrModelPiece = GetItemModelPiecePtr(ItemPtr, iModelPiece);
	if (ptrModelPiece != NULL)
		return *(uint8_t*)ptrModelPiece;

	return 0;
}

void SetItemModelPieceUVScroll(char* ItemPtr, int iModelPiece, bool bValue) {
	char* ptrModelPiece = GetItemModelPiecePtr(ItemPtr, iModelPiece);
	if (ptrModelPiece != NULL)
		*(uint8_t*)(ptrModelPiece + AmItmModelPieceUVSCroll) = (uint8_t)bValue?1:0;
}

int GetItemModelPieceUVScroll(char* ItemPtr, int iModelPiece) {
	char* ptrModelPiece = GetItemModelPiecePtr(ItemPtr, iModelPiece);
	if (ptrModelPiece != NULL)
		return *(uint8_t*)(ptrModelPiece + AmItmModelPieceUVSCroll);

	return 0;
}

void SetItemModelPartVariation(char* ItemPtr, int iModelPart, int iValue) {
	char* ptrModelPiece = GetItemModelPartPtr(ItemPtr, iModelPart);
	if (ptrModelPiece != NULL)
		*(uint8_t*)(ptrModelPiece + AmItmModelPartVariation) = (uint8_t)iValue;
}

int GetItemModelPartVariation(char* ItemPtr, int iModelPart) {
	char* ptrModelPiece = GetItemModelPartPtr(ItemPtr, iModelPart);
	if (ptrModelPiece != NULL)
		return *(uint8_t*)(ptrModelPiece + AmItmModelPartVariation);

	return 0;
}

void SetItemModelPartCategory(char* ItemPtr, int iModelPart, int iValue) {
	char* ptrModelPiece = GetItemModelPartPtr(ItemPtr, iModelPart);
	if (ptrModelPiece != NULL)
		*(uint8_t*)(ptrModelPiece + AmItmModelPartCategory) = (uint8_t)iValue;
}

int GetItemModelPartCategory(char* ItemPtr, int iModelPart) {
	char* ptrModelPiece = GetItemModelPartPtr(ItemPtr, iModelPart);
	if (ptrModelPiece != NULL)
		return *(uint8_t*)(ptrModelPiece + AmItmModelPartCategory);

	return 0;
}


void SetItemWpnPart1(char* ItemPtr, int iValue) {
	*(uint8_t*)(ItemPtr + AmItmWpnPart1) = (uint8_t)(iValue & 0xFF);
}

int GetItemWpnPart1(char* ItemPtr) {
	return *(uint8_t*)(ItemPtr + AmItmWpnPart1);
}


void SetItemWpnPart2(char* ItemPtr, int iValue) {
	*(uint8_t*)(ItemPtr + AmItmWpnPart2) = (uint8_t)(iValue & 0xFF);
}

int GetItemWpnPart2(char* ItemPtr) {
	return *(uint8_t*)(ItemPtr + AmItmWpnPart2);
}


void SetItemWpnPart3(char* ItemPtr, int iValue) {
	*(uint8_t*)(ItemPtr + AmItmWpnPart3) = (uint8_t)(iValue & 0xFF);
}

int GetItemWpnPart3(char* ItemPtr) {
	return *(uint8_t*)(ItemPtr + AmItmWpnPart3);
}

int GetItemModelPartMask(char* ItemPtr) {
	return *(uint8_t*)(ItemPtr + AmItmArmors);
}

void SetItemModelPartMask(char* ItemPtr, int iValue) {
	*(uint8_t*)(ItemPtr + AmItmArmors) = (uint8_t)(iValue & 0xFF);
}


void SetItemModelPieceUScroll(char* ItemPtr, int iModelPiece, float fValue) {
	char* ptrModelPiece = GetItemModelPiecePtr(ItemPtr, iModelPiece);
	if (ptrModelPiece != NULL)
		*(float*)(ptrModelPiece + AmItmModelPieceUScroll) = fValue;
}

float GetItemModelPieceUScroll(char* ItemPtr, int iModelPiece) {
	char* ptrModelPiece = GetItemModelPiecePtr(ItemPtr, iModelPiece);
	if (ptrModelPiece != NULL)
		return *(float*)(ptrModelPiece + AmItmModelPieceUScroll);
	return 0;
}

void SetItemModelPieceVScroll(char* ItemPtr, int iModelPiece, float fValue) {
	char* ptrModelPiece = GetItemModelPiecePtr(ItemPtr, iModelPiece);
	if (ptrModelPiece != NULL)
		*(float*)(ptrModelPiece + AmItmModelPieceVScroll) = fValue;
}

float GetItemModelPieceVScroll(char* ItemPtr, int iModelPiece) {
	char* ptrModelPiece = GetItemModelPiecePtr(ItemPtr, iModelPiece);
	if (ptrModelPiece != NULL)
		return *(float*)(ptrModelPiece + AmItmModelPieceVScroll);
	return 0;
}


void SetItemDescription(char* ItemPtr, char* sValue) {
	setDescription((ItemPtr + AmItmDescrIdent), sValue);
}

std::string GetItemDescription(char* ItemPtr) {
	return getDescription((ItemPtr + AmItmDescrIdent));
}

void SetItemDescriptionNonIdentified(char* ItemPtr, char* sValue) {
	setDescription((ItemPtr + AmItmDescrNonIdent), sValue);
}

std::string GetItemDescriptionNonIdentified(char* ItemPtr) {
	return getDescription((ItemPtr + AmItmDescrNonIdent));
}

int ItemGetInt(char* cCommand, int iObjectID) {
	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;
	std::string sCommand(cCommand);

	ObjectId = (NWN::OBJECTID) iObjectID;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return 0;

	if(Object->GetObjectType() != NWN::OBJECT_TYPE_ITEM)
		return 0;

	char* ItemPtr = (char*)Object->AsItem();

	if (sCommand == "appVariation")
		return GetItemAppVariation(ItemPtr);
	else if (sCommand == "appCategory")
		return GetItemAppCategory(ItemPtr);
	else if (sCommand == "appFXwpn")
		return GetItemFXWpn(ItemPtr);
	else if (sCommand == "WpnPart0")
		return GetItemWpnPart1(ItemPtr);
	else if (sCommand == "WpnPart1")
		return GetItemWpnPart2(ItemPtr);
	else if (sCommand == "WpnPart2")
		return GetItemWpnPart3(ItemPtr);
	else if (sCommand == "ModelPartMask")
		return GetItemModelPartMask(ItemPtr);
	else if (sCommand.rfind(cst_ModelPiece, 0) == 0) {
		sCommand = sCommand.substr(cst_ModelPiece.size());
		char* pEnd;
		int iModelPiece = strtol(sCommand.c_str(), &pEnd, 10);

		sCommand = pEnd;
		if (sCommand == "UVScroll")
			return GetItemModelPieceUVScroll(ItemPtr, iModelPiece);
		else if (sCommand == "Accessory")
			return GetItemModelPieceAccessory(ItemPtr, iModelPiece);
	}
	else if (sCommand.rfind(cst_ModelPart, 0) == 0) {
		sCommand = sCommand.substr(cst_ModelPart.size());
		char* pEnd;
		int iModelPart = strtol(sCommand.c_str(), &pEnd, 10);

		sCommand = pEnd;

		if (sCommand == "Variation")
			return GetItemModelPartVariation(ItemPtr, iModelPart);
		else if(sCommand == "Category")
			return GetItemModelPartCategory(ItemPtr, iModelPart);
	}

	return 0;
}

void ItemSetInt(char* cCommand, int iObjectID, int iValue) {
	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;
	std::string sCommand(cCommand);

	ObjectId = (NWN::OBJECTID) iObjectID;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return;

	if(Object->GetObjectType() != NWN::OBJECT_TYPE_ITEM)
		return;

	char* ItemPtr = (char*)Object->AsItem();



	if (sCommand == "itemType")
		SetItemBaseItem(ItemPtr, iValue);
	else if (sCommand == "appVariation")
		SetItemAppVariation(ItemPtr, iValue);
	else if (sCommand == "appCategory")
		SetItemAppCategory(ItemPtr, iValue);
	else if (sCommand == "appFXwpn")
		SetItemFXWpn(ItemPtr, iValue);
	else if (sCommand == "WpnPart0")
		SetItemWpnPart1(ItemPtr, iValue);
	else if (sCommand == "WpnPart1")
		SetItemWpnPart2(ItemPtr, iValue);
	else if (sCommand == "WpnPart2")
		SetItemWpnPart3(ItemPtr, iValue);
	else if (sCommand == "ModelPartMask")
		SetItemModelPartMask(ItemPtr, iValue);
	else if (sCommand.rfind(cst_ModelPiece, 0) == 0) {
		sCommand = sCommand.substr(cst_ModelPiece.size());
		char* pEnd;
		int iModelPiece = strtol(sCommand.c_str(), &pEnd, 10);

		sCommand = pEnd;

		if (sCommand == "UVScroll")
			SetItemModelPieceUVScroll(ItemPtr, iModelPiece, iValue != 0);
		else if (sCommand == "Accessory")
			SetItemModelPieceAccessory(ItemPtr, iModelPiece, iValue);
	}
	else if (sCommand.rfind(cst_ModelPart, 0) == 0) {
		sCommand = sCommand.substr(cst_ModelPart.size());
		char* pEnd;
		int iModelPart = strtol(sCommand.c_str(), &pEnd, 10);

		sCommand = pEnd;

		if (sCommand == "Variation")
			SetItemModelPartVariation(ItemPtr, iModelPart, iValue);
		else if(sCommand == "Category")
			SetItemModelPartCategory(ItemPtr, iModelPart, iValue);
	}



}

float ItemGetFloat(char* cCommand, int iObjectID) {
	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;
	std::string sCommand(cCommand);

	ObjectId = (NWN::OBJECTID) iObjectID;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return 0;

	if(Object->GetObjectType() != NWN::OBJECT_TYPE_ITEM)
		return 0;

	char* ItemPtr = (char*)Object->AsItem();
	if (sCommand.rfind(cst_ModelPiece, 0) == 0) {
		sCommand = sCommand.substr(cst_ModelPiece.size());
		char* pEnd;
		int iModelPiece = strtol(sCommand.c_str(), &pEnd, 10);

		sCommand = pEnd;

		if (sCommand == "UScroll")
			return GetItemModelPieceUScroll(ItemPtr, iModelPiece);
		else if (sCommand == "VScroll")
			return GetItemModelPieceVScroll(ItemPtr, iModelPiece);
	}

	return 0;

}

void ItemSetFloat(char* cCommand, int iObjectID, float fValue) {
	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;
	std::string sCommand(cCommand);

	ObjectId = (NWN::OBJECTID) iObjectID;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject(ObjectId)) == NULL)
		return;

	if(Object->GetObjectType() != NWN::OBJECT_TYPE_ITEM)
		return;

	char* ItemPtr = (char*)Object->AsItem();

	if (sCommand.rfind(cst_ModelPiece, 0) == 0) {
		sCommand = sCommand.substr(cst_ModelPiece.size());
		char* pEnd;
		int iModelPiece = strtol(sCommand.c_str(), &pEnd, 10);

		sCommand = pEnd;

		if (sCommand == "UScroll")
			SetItemModelPieceUScroll(ItemPtr, iModelPiece, fValue);
		else if (sCommand == "VScroll")
			SetItemModelPieceVScroll(ItemPtr, iModelPiece, fValue);
	}
}

std::string ItemGetString(char* cCommand, int iObjectID) {
	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;
	std::string sCommand(cCommand);
	std::string sResult = "";

	ObjectId = (NWN::OBJECTID) iObjectID;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject(ObjectId)) == NULL)
		return sResult;

	if(Object->GetObjectType() != NWN::OBJECT_TYPE_ITEM)
		return sResult;

	char* ItemPtr = (char*)Object->AsItem();

	if (sCommand == "Description")
		return GetItemDescription(ItemPtr);
	else if (sCommand == "DescriptionNI")
		return GetItemDescriptionNonIdentified(ItemPtr);
	else if (sCommand.find("Color") != std::string::npos)
	{
		NWN::D3DXCOLOR* ObjColor = GetItemColorPtr(ItemPtr, sCommand);
		if(ObjColor != NULL) {
			sResult = ColorToString(ObjColor);
		}
	}

	return sResult;
}

void ItemSetString(char* cCommand, int iObjectID, char* sValue) {
	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;
	std::string sCommand(cCommand);

	ObjectId = (NWN::OBJECTID) iObjectID;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject(ObjectId)) == NULL)
		return;

	if(Object->GetObjectType() != NWN::OBJECT_TYPE_ITEM)
		return;

	char* ItemPtr = (char*)Object->AsItem();

	if (sCommand == "Description")
		SetItemDescription(ItemPtr, sValue);
	else if (sCommand == "DescriptionNI")
		SetItemDescriptionNonIdentified(ItemPtr, sValue);
	else if (sCommand.find("Color") != std::string::npos)
	{

		NWN::D3DXCOLOR* ObjColor = GetItemColorPtr(ItemPtr, sCommand);
		if(ObjColor != NULL) {
			NWN::D3DXCOLOR myColor;
			//Color is Ok, we can apply it
			if(StringToColor(sValue, &myColor)) {
				memcpy(ObjColor, &myColor, sizeof(myColor));
			}
		}
	}
}





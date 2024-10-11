#include <cstdint>
#include <string>
#include "../../septutil/NwN2DataPos.h"
#include "../../NWN2Lib/NWN2.h"
#include "../../NWN2Lib/NWN2Common.h"
#include "aspectManagerUtils.h"

int GetLightIsLerp(GameObject* Object) {
	uint8_t isLerp = *(uint8_t*)((char*)Object + AmLightLerpFlick);
	uint8_t iMask = 0x1;
	isLerp = (isLerp & iMask);
	return isLerp;
}

void SetLightIsLerp(GameObject* Object, bool bValue) {
	uint8_t isLerp = *(uint8_t*)((char*)Object + AmLightLerpFlick);
	if (bValue)
		isLerp = isLerp | 0x1;
	else
		isLerp = isLerp & ~0x1;

	*(uint8_t*)((char*)Object + AmLightLerpFlick) = isLerp;
}

int GetLightIsFlicker(GameObject* Object) {
	uint8_t isFlick = *(uint8_t*)((char*)Object + AmLightLerpFlick);
	uint8_t iMask = 0x2;
	isFlick = (isFlick & iMask);
	isFlick = isFlick >> 1;
	return isFlick;
}

void SetLightIsFlicker(GameObject* Object, bool bValue) {
	uint8_t isFlick = *(uint8_t*)((char*)Object + AmLightLerpFlick);
	if (bValue)
		isFlick = isFlick | 0x2;
	else
		isFlick = isFlick & ~0x2;

	*(uint8_t*)((char*)Object + AmLightLerpFlick) = isFlick;
}

int GetLightFlickerType(GameObject* Object) {
	uint8_t iFlickType = *(uint8_t*)((char*)Object + AmLightFlickerType);

	return (iFlickType & 0x7);
}

void SetLightFlickerType(GameObject* Object, int iValue) {
	uint8_t newFlickTypeMask = iValue & 0x7;
	uint8_t iFlickType = *(uint8_t*)((char*)Object + AmLightFlickerType);

	//Remove old value
	iFlickType &= ~0x7;
	
	//Set New Values
	iFlickType |= newFlickTypeMask;
	*(uint8_t*)((char*)Object + AmLightFlickerType) = iFlickType;
}

int GetLightCastShadow(GameObject* Object) {
	uint8_t iShadow = *(uint8_t*)((char*)Object + AmLightCastShadow);

	iShadow = (iShadow == 0) ? 0 : 1;

	return iShadow;
}

void SetLightCastShadow(GameObject* Object, bool bValue) {
	*(uint8_t*)((char*)Object + AmLightCastShadow) = (bValue ? 1 : 0);
}





void SetLightColorIntensity(GameObject* Object, float fValue) {
	*(float*)((char*)Object + AmLightColor + AmLightCompositeIntensity) = fValue;
}

float GetLightColorIntensity(GameObject* Object) {
	return *(float*)((char*)Object + AmLightColor + AmLightCompositeIntensity);
}



void SetLightLerpIntensity(GameObject* Object, float fValue) {
	*(float*)((char*)Object + AmLightLerpColor + AmLightCompositeIntensity) = fValue;
}

float GetLightLerpIntensity(GameObject* Object) {
	return *(float*)((char*)Object + AmLightLerpColor + AmLightCompositeIntensity);
}



void SetLightLerpPeriod(GameObject* Object, float fValue) {
	*(float*)((char*)Object + AmLightLerpPeriod) = fValue;
}

float GetLightLerpPeriod(GameObject* Object) {
	return *(float*)((char*)Object + AmLightLerpPeriod);
}



void SetLightFlickerVariance(GameObject* Object, float fValue) {
	*(float*)((char*)Object + AmLightFlickerVariance) = fValue;
}

float GetLightFlickerVariance(GameObject* Object) {
	return *(float*)((char*)Object + AmLightFlickerVariance);
}



void SetLightFlickerRate(GameObject* Object, float fValue) {
	*(float*)((char*)Object + AmLightFlickerRate) = fValue;
}

float GetLightFlickerRate(GameObject* Object) {
	return *(float*)((char*)Object + AmLightFlickerRate);
}



void SetLightShadowIntensity(GameObject* Object, float fValue) {
	*(float*)((char*)Object + AmLightShadowIntensity) = fValue;
}

float GetLightShadowIntensity(GameObject* Object) {
	return *(float*)((char*)Object + AmLightShadowIntensity);
}



void SetLightRange(GameObject* Object, float fValue) {
	*(float*)((char*)Object + AmLightRange) = fValue;
}

float GetLightRange(GameObject* Object) {
	return *(float*)((char*)Object + AmLightRange);
}


NWN::D3DXCOLOR* GetLightColorPtr(std::string sCommand, GameObject* Object) {
	
	int iValue = 0;
	
	if(sCommand.rfind("Lerp", 0) == 0) {
		iValue = AmLightLerpColor;
		sCommand = sCommand.substr(4);
	}
	else {
		iValue = AmLightColor;
	}

	if (sCommand == "Specular") {
		iValue += AmLightCompositeSpecular;
	}
	else if (sCommand == "Ambiant") {
		iValue += AmLightCompositeAmbiant;
	}
	else if (sCommand ==  "Diffuse") {
		iValue += AmLightCompositeDiffuse;
	} else {
		return NULL;
	}

	return (NWN::D3DXCOLOR*)((char*)Object + iValue);
}

void LightSetInt(char* cCommand, int iObjectID, int iValue) {
	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;
	std::string		sCommand(cCommand);

	ObjectId = (NWN::OBJECTID) iObjectID;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return;

	if(Object->GetObjectType() != NWN::OBJECT_TYPE_LIGHT)
		return;


	if (sCommand == "Lerp")
		SetLightIsLerp(Object, iValue != 0);
	else if (sCommand == "Flicker")
		SetLightIsFlicker(Object, iValue != 0);
	else if (sCommand == "FlickerType")
		SetLightFlickerType(Object, iValue);
	else if (sCommand == "CastShadow")
		SetLightCastShadow(Object, iValue != 0);

}

int LightGetInt(char* cCommand, int iObjectID) {
	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;
	std::string		sCommand(cCommand);

	ObjectId = (NWN::OBJECTID) iObjectID;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return 0;

	if(Object->GetObjectType() != NWN::OBJECT_TYPE_LIGHT)
		return 0;

	if (sCommand == "Lerp")
		return GetLightIsLerp(Object);
	else if (sCommand == "Flicker")
		return GetLightIsFlicker(Object);
	else if (sCommand == "FlickerType")
		return GetLightFlickerType(Object);
	else if (sCommand == "CastsShadow")
		return GetLightCastShadow(Object);

	return 0;
}


void LightSetFloat(char* cCommand, int iObjectID, float fValue) {
	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;
	std::string		sCommand(cCommand);

	ObjectId = (NWN::OBJECTID) iObjectID;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return;

	if(Object->GetObjectType() != NWN::OBJECT_TYPE_LIGHT)
		return;


	if (sCommand == "ColorIntensity")
		SetLightColorIntensity(Object, fValue);
	else if (sCommand == "LerpIntensity")
		SetLightLerpIntensity(Object, fValue);
	else if (sCommand == "LerpPeriod")
		SetLightLerpPeriod(Object, fValue);
	else if (sCommand == "FlickerVariance")
		SetLightFlickerVariance(Object, fValue);
	else if (sCommand == "FlickerRate")
		SetLightFlickerRate(Object, fValue);
	else if (sCommand == "ShadowIntensity")
		SetLightShadowIntensity(Object, fValue);
	else if (sCommand == "Range")
		SetLightRange(Object, fValue);


}

float LightGetFloat(char* cCommand, int iObjectID) {
	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;
	std::string		sCommand(cCommand);

	ObjectId = (NWN::OBJECTID) iObjectID;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return 0;

	if(Object->GetObjectType() != NWN::OBJECT_TYPE_LIGHT)
		return 0;

	if (sCommand == "ColorIntensity")
		return GetLightColorIntensity(Object);
	else if (sCommand == "LerpIntensity")
		return GetLightLerpIntensity(Object);
	else if (sCommand == "LerpPeriod")
		return GetLightLerpPeriod(Object);
	else if (sCommand == "FlickerVariance")
		return GetLightFlickerVariance(Object);
	else if (sCommand == "FlickerRate")
		return GetLightFlickerRate(Object);
	else if (sCommand == "ShadowIntensity")
		return GetLightShadowIntensity(Object);
	else if (sCommand == "Range")
		return GetLightRange(Object);
	return 0;
}


void LightSetString(char* cCommand, int iObjectID, char* cValue) {
	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;
	std::string sCommand(cCommand);

	ObjectId = (NWN::OBJECTID) iObjectID;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return;

	if(Object->GetObjectType() != NWN::OBJECT_TYPE_LIGHT)
		return;


	NWN::D3DXCOLOR* ObjColor = GetLightColorPtr(sCommand, Object);
	if(ObjColor != NULL) {
		NWN::D3DXCOLOR myColor;
		//Color is Ok, we can apply it
		if(StringToColor(cValue, &myColor)) {
			memcpy(ObjColor, &myColor, sizeof(myColor));
		}
	}
}

std::string LightGetString(char* cCommand, int iObjectID) {
	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;
	std::string sCommand(cCommand);
	std::string sResult = "";

	ObjectId = (NWN::OBJECTID) iObjectID;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return sResult;

	if(Object->GetObjectType() != NWN::OBJECT_TYPE_LIGHT)
		return sResult;


	NWN::D3DXCOLOR* ObjColor = GetLightColorPtr(sCommand, Object);
	if(ObjColor != NULL) {
		sResult = ColorToString(ObjColor);
	}
	return sResult;
}
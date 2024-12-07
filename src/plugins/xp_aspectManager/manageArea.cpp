#include <cstdint>
#include <string>
#include "manageArea.h"
#include "../../septutil/NwN2DataPos.h"
#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include "aspectManagerUtils.h"


char* GetDNCPtr(char* areaPtr, int iDNCIdx)
{
	if(iDNCIdx < 0 || iDNCIdx > DAYNIGHT_DEFAULT)
		return NULL;

	return ((areaPtr + AmAreaDNCStart) + (iDNCIdx * AmAreaDNCSize));	
}

//Flag for interior, natural, subterran...
void SetAreaFlag(char* areaPtr, int iValue) {
	*(uint32_t*)(areaPtr + AmAreaFlag) = iValue;
}

int GetAreaFlag(char* areaPtr) {
	return *(uint32_t*)(areaPtr + AmAreaFlag);
}


void SetCreatorID(char* areaPtr, int iValue)
{
	uint32_t areaIdBlock = *(uint32_t*)(areaPtr + AmAreaPtrBlockId);
	*(int32_t*)(areaIdBlock + AmAreaBlockIdCreatorID) = iValue;
}

int GetCreatorID(char* areaPtr)
{
	uint32_t areaIdBlock = *(uint32_t*)(areaPtr + AmAreaPtrBlockId);
	return *(int32_t*)(areaIdBlock + AmAreaBlockIdCreatorID);
}


void SetAreaHasDayNightCycle(char* areaPtr, bool bValue) {
	*(uint8_t*)(areaPtr + AmAreaDayNightCycle) = (uint8_t)(bValue?1:0);
}

int GetAreaHasDayNightCycle(char* areaPtr) {
	return *(uint8_t*)(areaPtr + AmAreaDayNightCycle);
}

void SetAreaIsDirectionalLightCastShadows(char* areaPtr, bool bValue) {
	*(uint8_t*)(areaPtr + AmAreaDayNightShadows) = (uint8_t)(bValue?1:0);
}

int GetAreaIsDirectionalLightCastShadows(char* areaPtr) {
	return *(uint8_t*)(areaPtr + AmAreaDayNightShadows);
}

void SetAreaIsAlwaysNight(char* areaPtr, bool bValue) {
	*(uint8_t*)(areaPtr + AmAreaIsNight) = (uint8_t)(bValue?1:0);

}

int GetAreaIsAlwaysNight(char* areaPtr) {
	return *(uint8_t*)(areaPtr + AmAreaIsNight);
}

void SetAreaUseDayNightFog(char* areaPtr, bool bValue) {
	*(uint8_t*)(areaPtr + AmAreaUseDayNightFog) = (uint8_t)(bValue?1:0);
}

int GetAreaUseDayNightFog(char* areaPtr) {
	return *(uint8_t*)(areaPtr + AmAreaUseDayNightFog);
}

void SetAreaHasDirectionalLight(char* areaPtr, bool bValue) {
	*(uint8_t*)(areaPtr + AmAreaHasDirLight) = (uint8_t)(bValue?1:0);

}

int GetAreaHasDirectionalLight(char* areaPtr) {
	return *(uint8_t*)(areaPtr + AmAreaHasDirLight);
}

void SetAreaIsNoRest(char* areaPtr, bool bValue) {
	*(uint8_t*)(areaPtr + AmAreaNoRest) = (uint8_t)(bValue?1:0);
}

int GetAreaIsNoRest(char* areaPtr) {
	return *(uint8_t*)(areaPtr + AmAreaNoRest);
}

void SetAreaModSpotCheck(char* areaPtr, int iValue) {
	*(int32_t*)(areaPtr + AmAreaModSpotCheck) = (int32_t)iValue;
}

int GetAreaModSpotCheck(char* areaPtr) {
	return *(int32_t*)(areaPtr + AmAreaModSpotCheck);
}

void SetAreaModListenCheck(char* areaPtr, int iValue) {
	*(int32_t*)(areaPtr + AmAreaModListenCheck) = (int32_t)iValue;
}

int GetAreaModListenCheck(char* areaPtr) {
	return *(int32_t*)(areaPtr + AmAreaModListenCheck);
}

void SetAreaPVPMode(char* areaPtr, int iValue) {
	*(uint8_t*)(areaPtr + AmAreaPVPMode) = (uint8_t)(iValue & 0xFF);

}

int GetAreaPVPMode(char* areaPtr) {
	return *(uint8_t*)(areaPtr + AmAreaPVPMode);
}

void SetAreaSkyRing(char* areaPtr, int iIdx, int iValue) {
	char* pSkyRing = NULL;

	switch(iIdx) {
		case 0:
			pSkyRing = (areaPtr + AmAreaSkyRingN);
			break;
		case 1:
			pSkyRing = (areaPtr + AmAreaSkyRingN);
			break;
		case 2:
			pSkyRing = (areaPtr + AmAreaSkyRingN);
			break;
		case 3:
			pSkyRing = (areaPtr + AmAreaSkyRingN);
			break;
		default:
			return;
	}

	*(uint16_t*)pSkyRing = (uint16_t) (iValue & 0xFFFF);
}

int GetAreaSkyRing(char* areaPtr, int iIdx) {
	char* pSkyRing = NULL;

	switch(iIdx) {
		case 0:
			pSkyRing = (areaPtr + AmAreaSkyRingN);
			break;
		case 1:
			pSkyRing = (areaPtr + AmAreaSkyRingN);
			break;
		case 2:
			pSkyRing = (areaPtr + AmAreaSkyRingN);
			break;
		case 3:
			pSkyRing = (areaPtr + AmAreaSkyRingN);
			break;
		default:
			return 0;
	}

	return *(uint16_t*)pSkyRing;
}



void SetAreaDNCDesaturateColor(char* areaPtr, int iDNCIdx, bool bValue) {
	char* myPtr = GetDNCPtr(areaPtr, iDNCIdx);

	if(myPtr == NULL)
		return;


	*(uint8_t*)(myPtr + AmAreaDNCDesaturateColor) = (uint8_t)(bValue?1:0);
}

int GetAreaDNCDesaturateColor(char* areaPtr, int iDNCIdx) {
	char* myPtr = GetDNCPtr(areaPtr, iDNCIdx);

	if(myPtr == NULL)
		return 0;

	uint8_t iValue = *(uint8_t*)(myPtr + AmAreaDNCDesaturateColor);
	return (iValue==0)?0:1;
}


void SetDNCSkyLightIntensity(char* dncPtr, float fValue){
	*(float*)(dncPtr + AmAreaDNCSkyLight + AmLightCompositeIntensity) = fValue;
}

float GetDNCSkyLightIntensity(char* dncPtr){
	return *(float*)(dncPtr + AmAreaDNCSkyLight + AmLightCompositeIntensity);
}

void SetDNCSunMoonIntensity(char* dncPtr, float fValue){
	*(float*)(dncPtr + AmAreaDNCSunMoon + AmLightCompositeIntensity) = fValue;
}

float GetDNCSunMoonIntensity(char* dncPtr){
	return *(float*)(dncPtr + AmAreaDNCSunMoon + AmLightCompositeIntensity);
}

void SetDNCGroundLightIntensity(char* dncPtr, float fValue){
	*(float*)(dncPtr + AmAreaDNCGroundLight + AmLightCompositeIntensity) = fValue;
}

float GetDNCGroundLightIntensity(char* dncPtr){
	return *(float*)(dncPtr + AmAreaDNCGroundLight + AmLightCompositeIntensity);
}

void SetDNCFogStart(char* dncPtr, float fValue){
	*(float*)(dncPtr + AmAreaDNCFogStart) = fValue;
}

float GetDNCFogStart(char* dncPtr){
	return *(float*)(dncPtr + AmAreaDNCFogStart);
}

void SetDNCFogEnd(char* dncPtr, float fValue){
	*(float*)(dncPtr + AmAreaDNCFogEnd) = fValue;
}

float GetDNCFogEnd(char* dncPtr){
	return *(float*)(dncPtr + AmAreaDNCFogEnd);
}

void SetDNCFogFarClip(char* dncPtr, float fValue){
	*(float*)(dncPtr + AmAreaDNCFarClip) = fValue;
}

float GetDNCFogFarClip(char* dncPtr){
	return *(float*)(dncPtr + AmAreaDNCFarClip);
}

void SetDNCCloudMoveX(char* dncPtr, float fValue){
	*(float*)(dncPtr + AmAreaDNCCloudMoveX) = fValue;
}

float GetDNCCloudMoveX(char* dncPtr){
	return *(float*)(dncPtr + AmAreaDNCCloudMoveX);
}

void SetDNCCloudMoveY(char* dncPtr, float fValue){
	*(float*)(dncPtr + AmAreaDNCCloudMoveY) = fValue;
}

float GetDNCCloudMoveY(char* dncPtr){
	return *(float*)(dncPtr + AmAreaDNCCloudMoveY);
}

void SetDNCShadowIntensity(char* dncPtr, float fValue){
	*(float*)(dncPtr + AmAreaDNCShadowIntensity) = fValue;
}

float GetDNCShadowIntensity(char* dncPtr){
	return *(float*)(dncPtr + AmAreaDNCShadowIntensity);
}

void SetDNCCoronaIntensity(char* dncPtr, float fValue){
	*(float*)(dncPtr + AmAreaDNCCoronaIntensity) = fValue;
}

float GetDNCCoronaIntensity(char* dncPtr){
	return *(float*)(dncPtr + AmAreaDNCCoronaIntensity);
}

void SetDNCDesaturateFactor(char* dncPtr, float fValue){
	*(float*)(dncPtr + AmAreaDNCDesaturateFactor) = fValue;
}

float GetDNCDesaturateFactor(char* dncPtr){
	return *(float*)(dncPtr + AmAreaDNCDesaturateFactor);
}

void SetDNCBloomSCIntensity(char* dncPtr, float fValue){
	*(float*)(dncPtr + AmAreaDNCBloomSCIntensity) = fValue;
}

float GetDNCBloomSCIntensity(char* dncPtr){
	return *(float*)(dncPtr + AmAreaDNCBloomSCIntensity);
}

void SetDNCBloomGLIntensity(char* dncPtr, float fValue){
	*(float*)(dncPtr + AmAreaDNCBloomGLIntensity) = fValue;
}

float GetDNCBloomGLIntensity(char* dncPtr){
	return *(float*)(dncPtr + AmAreaDNCBloomGLIntensity);
}

void SetDNCBloomHLThreshold(char* dncPtr, float fValue){
	*(float*)(dncPtr + AmAreaDNCBloomHLThreshold) = fValue;
}

float GetDNCBloomHLThreshold(char* dncPtr){
	return *(float*)(dncPtr + AmAreaDNCBloomHLThreshold);
}

void SetDNCBloomHLIntensity(char* dncPtr, float fValue){
	*(float*)(dncPtr + AmAreaDNCBloomHLIntensity) = fValue;
}

float GetDNCBloomHLIntensity(char* dncPtr){
	return *(float*)(dncPtr + AmAreaDNCBloomHLIntensity);
}

void SetDNCBloomBLRadius(char* dncPtr, float fValue){
	*(float*)(dncPtr + AmAreaDNCBloomBLRadius) = fValue;
}

float GetDNCBloomBLRadius(char* dncPtr){
	return *(float*)(dncPtr + AmAreaDNCBloomBLRadius);
}

void SetDNCHDRHighlight(char* dncPtr, float fValue){
	*(float*)(dncPtr + AmAreaDNCHDRHighlight) = fValue;
}

float GetDNCHDRHighlight(char* dncPtr){
	return *(float*)(dncPtr + AmAreaDNCHDRHighlight);
}

void SetDNCHDRMax(char* dncPtr, float fValue){
	*(float*)(dncPtr + AmAreaDNCHDRMax) = fValue;
}

float GetDNCHDRMax(char* dncPtr){
	return *(float*)(dncPtr + AmAreaDNCHDRMax);
}

void SetDNCHDRAvg(char* dncPtr, float fValue){
	*(float*)(dncPtr + AmAreaDNCHDRAvg) = fValue;
}

float GetDNCHDRAvg(char* dncPtr){
	return *(float*)(dncPtr + AmAreaDNCHDRAvg);
}

void SetDNCExposure(char* dncPtr, float fValue){
	*(float*)(dncPtr + AmAreaDNCExposure) = fValue;
}

float GetDNCExposure(char* dncPtr){
	return *(float*)(dncPtr + AmAreaDNCExposure);
}

void SetDNCCloudCover(char* dncPtr, float fValue){
	*(float*)(dncPtr + AmAreaDNCCloudCover) = fValue;
}

float GetDNCCloudCover(char* dncPtr){
	return *(float*)(dncPtr + AmAreaDNCCloudCover);
}

void SetDNCSkyRingLERP(char* dncPtr, float fValue) {
	*(float*)(dncPtr + AmAreaDNCSkyRingLERP) = fValue;
}

float GetDNCSkyRingLERP(char* dncPtr) {
	return *(float*)(dncPtr + AmAreaDNCSkyRingLERP);
}

void SetDNCSunMoonDir(char* dncPtr, int iDir, float fValue) {
	float* ptr = NULL;
	if(iDir == 0)
		ptr = (float*)(dncPtr + AmAreaDNCSunMoonDirX);
	else if(iDir == 1)
		ptr = (float*)(dncPtr + AmAreaDNCSunMoonDirY);
	else
		ptr = (float*)(dncPtr + AmAreaDNCSunMoonDirZ);

	*ptr = fValue;
}

float GetDNCSunMoonDir(char* dncPtr, int iDir) {
	if(iDir == 0)
		return *(float*)(dncPtr + AmAreaDNCSunMoonDirX);
	else if(iDir == 1)
		return *(float*)(dncPtr + AmAreaDNCSunMoonDirY);
	else
		return *(float*)(dncPtr + AmAreaDNCSunMoonDirZ);
}



void SetSkyDomeModel(char* dncPtr, char* cValue) {
	PutNWNExoString(dncPtr + AmAreaDNCSkyDomeModel, cValue);
}

std::string GetSkyDomeModel(char* dncPtr) {
	std::string sResult = ExoStringToString((NWN::CExoString*)(dncPtr + AmAreaDNCSkyDomeModel));
	return sResult;
}

void SetFogColor(char* dncPtr, char* cValue) {
	NWN::D3DXCOLOR myColor;
	//Color is Ok, we can apply it
	if(StringToColor(cValue, &myColor)) {
		*(float*)(dncPtr + AmAreaDNCFogColor) = myColor.r;
		*(float*)(dncPtr + AmAreaDNCFogColor + 4) = myColor.g;
		*(float*)(dncPtr + AmAreaDNCFogColor + 8) = myColor.b;
	}
}


std::string GetFogColor(char* dncPtr) {
	std::string sResult = "";
	NWN::D3DXCOLOR* ObjColor = (NWN::D3DXCOLOR*)(dncPtr + AmAreaDNCFogColor);
	NWN::D3DXCOLOR MyColor;

	MyColor.r = ObjColor->r;
	MyColor.g = ObjColor->g;
	MyColor.b = ObjColor->b;
	MyColor.a = 1;

	sResult = ColorToString(&MyColor);
	return sResult;
}

NWN::D3DXCOLOR* GetAreaColorPtr(char* dncPtr, std::string sCommand) {
	char* retVal = NULL;

	if (sCommand == "SkyHorizon")
		retVal = (dncPtr + AmAreaDNCSkyHorizon);
	else if (sCommand == "SkyZenith")
		retVal = (dncPtr + AmAreaDNCSkyZenith);
	else {
		if (sCommand.rfind("SkyLight", 0) == 0) {
			sCommand = sCommand.substr(8);
			retVal = (dncPtr + AmAreaDNCSkyLight);
		}
		else if (sCommand.rfind("SunMoon", 0) == 0) {
			sCommand = sCommand.substr(7);
			retVal = (dncPtr + AmAreaDNCSunMoon);
		}
		else if (sCommand.rfind("GroundLight", 0) == 0) {
			sCommand = sCommand.substr(11);
			retVal = (dncPtr + AmAreaDNCGroundLight);
		}
		else {
			return (NWN::D3DXCOLOR*) retVal;
		}

		if (sCommand == "Ambiant") {
			retVal = retVal + AmLightCompositeAmbiant;
		}
		else if (sCommand == "Diffuse") {
			retVal = retVal + AmLightCompositeDiffuse;
		}
		else {
			retVal = retVal + AmLightCompositeSpecular;
		}
	}
	return (NWN::D3DXCOLOR*) retVal;
}

char* ptrToArea(int iObjectID) {
	char* retPtr = NULL;
	

	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;

	ObjectId = (NWN::OBJECTID) iObjectID;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return retPtr;

	if(Object->GetObjectType() != NWN::OBJECT_TYPE_AREA)
		return retPtr;

	retPtr = (char*)Object->AsArea();

	return retPtr;	
}


void AreaSetString(char* cCommand, int iObjectID, char* sValue) {
	std::string sCommand(cCommand);
	char* areaPtr = ptrToArea(iObjectID);

	if(areaPtr == NULL)
		return;

	//NonDNC (if exist first)
	// 
	//Non color first 
	if (sCommand.rfind("DNC", 0) == 0) {
		sCommand = sCommand.substr(3);
		char* pEnd;
		int iDNCIdx = strtol(sCommand.c_str(), &pEnd, 10);

		sCommand = pEnd;
		char* dncPtr = GetDNCPtr(areaPtr, iDNCIdx);
		if (dncPtr == NULL)
			return;

		if (sCommand == "SkyDomeModel")
			SetSkyDomeModel(dncPtr, sValue);
		else if (sCommand == "FogColor")
			SetFogColor(dncPtr, sValue);
		else {
			NWN::D3DXCOLOR* ObjColor = GetAreaColorPtr(areaPtr, sCommand);
			if (ObjColor != NULL) {
				NWN::D3DXCOLOR myColor;
				//Color is Ok, we can apply it
				if (StringToColor(sValue, &myColor)) {
					memcpy(ObjColor, &myColor, sizeof(myColor));
				}
			}
		}
	}

}


std::string AreaGetString(char* cCommand, int iObjectID) {
	std::string sCommand(cCommand);
	char* areaPtr = ptrToArea(iObjectID);

	std::string sResult = "";

	if(areaPtr == NULL)
		return "";

	//NonDNC (if exist first)
	// 
	//Non color first 
	if (sCommand.rfind("DNC", 0) == 0) {
		sCommand = sCommand.substr(3);
		char* pEnd;
		int iDNCIdx = strtol(sCommand.c_str(), &pEnd, 10);

		sCommand = pEnd;
		char* dncPtr = GetDNCPtr(areaPtr, iDNCIdx);
		if (dncPtr == NULL)
			return "";

		if (sCommand == "SkyDomeModel")
			return GetSkyDomeModel(dncPtr);
		else if (sCommand == "FogColor")
			return GetFogColor(dncPtr);
		else
		{
			NWN::D3DXCOLOR* ObjColor = GetAreaColorPtr(dncPtr, sCommand);
			if (ObjColor != NULL) {
				sResult = ColorToString(ObjColor);
			}
		}
	}
	return sResult;
}


void AreaSetFloat(char* cCommand, int iObjectID, float fValue) {
	std::string sCommand(cCommand);
	char* areaPtr = ptrToArea(iObjectID);
	
	if(areaPtr == NULL)
		return;
	
		if(sCommand.rfind("DNC", 0) == 0) {
			sCommand = sCommand.substr(3);
			char* pEnd;
			int iDNCIdx = strtol(sCommand.c_str(), &pEnd, 10);

			sCommand = pEnd;
			char* dncPtr = GetDNCPtr(areaPtr, iDNCIdx);
			if(dncPtr == NULL)
				return;

			if(sCommand == "SkylightIntensity")
				SetDNCSkyLightIntensity(dncPtr, fValue);
			else if(sCommand == "SunMoonIntensity")
				SetDNCSunMoonIntensity(dncPtr, fValue);
			else if(sCommand == "GroundLightIntensity")
				SetDNCGroundLightIntensity(dncPtr, fValue);
			else if(sCommand == "FogStart")
				SetDNCFogStart(dncPtr, fValue);
			else if(sCommand == "FogEnd")
				SetDNCFogEnd(dncPtr, fValue);
			else if(sCommand == "FogFarClip")
				SetDNCFogFarClip(dncPtr, fValue);
			else if(sCommand == "CloudMoveX")
				SetDNCCloudMoveX(dncPtr, fValue);
			else if(sCommand == "CloudMoveY")
				SetDNCCloudMoveY(dncPtr, fValue);
			else if(sCommand == "ShadowIntensity")
				SetDNCShadowIntensity(dncPtr, fValue);
			else if(sCommand == "CoronaIntensity")
				SetDNCCoronaIntensity(dncPtr, fValue);
			else if(sCommand == "DesaturateFactor")
				SetDNCDesaturateFactor(dncPtr, fValue);
			else if(sCommand == "BloomSCIntensity")
				SetDNCBloomSCIntensity(dncPtr, fValue);
			else if(sCommand == "BloomGLIntensity")
				SetDNCBloomGLIntensity(dncPtr, fValue);
			else if(sCommand == "BloomHLThreshold")
				SetDNCBloomHLThreshold(dncPtr, fValue);
			else if(sCommand == "BloomHLIntensity")
				SetDNCBloomHLIntensity(dncPtr, fValue);
			else if(sCommand == "BloomBLRadius")
				SetDNCBloomBLRadius(dncPtr, fValue);
			else if(sCommand == "HDRHighlight")
				SetDNCHDRHighlight(dncPtr, fValue);
			else if(sCommand == "HDRMax")
				SetDNCHDRMax(dncPtr, fValue);
			else if(sCommand == "HDRAvg")
				SetDNCHDRAvg(dncPtr, fValue);
			else if(sCommand == "Exposure")
				SetDNCExposure(dncPtr, fValue);
			else if(sCommand == "CloudCover")
				SetDNCCloudCover(dncPtr, fValue);
			else if(sCommand == "SkyringLerp")
				SetDNCSkyRingLERP(dncPtr, fValue);
			else if (sCommand == "SunMoonDirX")
				SetDNCSunMoonDir(dncPtr, 0, fValue);
			else if (sCommand == "SunMoonDirY")
				SetDNCSunMoonDir(dncPtr, 1, fValue);
			else if (sCommand == "SunMoonDirZ")
				SetDNCSunMoonDir(dncPtr, 2, fValue);
		}
}

float AreaGetFloat(char* cCommand, int iObjectID) {
	std::string sCommand(cCommand);
	char* areaPtr = ptrToArea(iObjectID);
	
	if(areaPtr == NULL)
		return 0;
	
		if(sCommand.rfind("DNC", 0) == 0) {
			sCommand = sCommand.substr(3);
			char* pEnd;
			int iDNCIdx = strtol(sCommand.c_str(), &pEnd, 10);

			sCommand = pEnd;
			char* dncPtr = GetDNCPtr(areaPtr, iDNCIdx);
			if(dncPtr == NULL)
				return 0;

			if (sCommand == "SkylightIntensity")
				return GetDNCSkyLightIntensity(dncPtr);
			else if (sCommand == "SunMoonIntensity")
				return GetDNCSunMoonIntensity(dncPtr);
			else if (sCommand == "GroundLightIntensity")
				return GetDNCGroundLightIntensity(dncPtr);
			else if (sCommand == "FogStart")
				return GetDNCFogStart(dncPtr);
			else if (sCommand == "FogEnd")
				return GetDNCFogEnd(dncPtr);
			else if (sCommand == "FogFarClip")
				return GetDNCFogFarClip(dncPtr);
			else if (sCommand == "CloudMoveX")
				return GetDNCCloudMoveX(dncPtr);
			else if (sCommand == "CloudMoveY")
				return GetDNCCloudMoveY(dncPtr);
			else if (sCommand == "ShadowIntensity")
				return GetDNCShadowIntensity(dncPtr);
			else if (sCommand == "CoronaIntensity")
				return GetDNCCoronaIntensity(dncPtr);
			else if (sCommand == "DesaturateFactor")
				return GetDNCDesaturateFactor(dncPtr);
			else if (sCommand == "BloomSCIntensity")
				return GetDNCBloomSCIntensity(dncPtr);
			else if (sCommand == "BloomGLIntensity")
				return GetDNCBloomGLIntensity(dncPtr);
			else if (sCommand == "BloomHLThreshold")
				return GetDNCBloomHLThreshold(dncPtr);
			else if (sCommand == "BloomHLIntensity")
				return GetDNCBloomHLIntensity(dncPtr);
			else if (sCommand == "BloomBLRadius")
				return GetDNCBloomBLRadius(dncPtr);
			else if (sCommand == "HDRHighlight")
				return GetDNCHDRHighlight(dncPtr);
			else if (sCommand == "HDRMax")
				return GetDNCHDRMax(dncPtr);
			else if (sCommand == "HDRAvg")
				return GetDNCHDRAvg(dncPtr);
			else if (sCommand == "Exposure")
				return GetDNCExposure(dncPtr);
			else if (sCommand == "CloudCover")
				return GetDNCCloudCover(dncPtr);
			else if (sCommand == "SkyringLerp")
				return GetDNCSkyRingLERP(dncPtr);
			else if (sCommand == "SunMoonDirX")
				return GetDNCSunMoonDir(dncPtr, 0);
			else if (sCommand == "SunMoonDirY")
				return GetDNCSunMoonDir(dncPtr, 1);
			else if (sCommand == "SunMoonDirZ")
				return GetDNCSunMoonDir(dncPtr, 2);
		}

		return 0;
}

void AreaSetInt(char* cCommand, int iObjectID, int iValue) {
	std::string sCommand(cCommand);
	char* areaPtr = ptrToArea(iObjectID);
	
	if(areaPtr == NULL)
		return;

	std::string sDNCDesaturate = "DNCDesaturateColor";

	if (sCommand == "areaFlag")
		SetAreaFlag(areaPtr, iValue);
//	else if (sCommand == "creatorID")
//		SetCreatorID(areaPtr, iValue);
	else if(sCommand == "hasDayNightCycle")
		SetAreaHasDayNightCycle(areaPtr, iValue != 0);
	else if(sCommand == "isAlwaysNight")
		SetAreaIsAlwaysNight(areaPtr, iValue != 0);
	else if(sCommand == "useDayNightFog")
		SetAreaUseDayNightFog(areaPtr, iValue != 0);
	else if(sCommand == "hasDirLight")
		SetAreaHasDirectionalLight(areaPtr, iValue != 0);
	else if(sCommand == "isDirLightCastShadows")
		SetAreaIsDirectionalLightCastShadows(areaPtr, iValue != 0);
	else if(sCommand == "noRest")
		SetAreaIsNoRest(areaPtr, iValue != 0);
	else if(sCommand == "PVPMode")
		SetAreaPVPMode(areaPtr, iValue);
	else if(sCommand == "modSpotCheck")
		SetAreaModSpotCheck(areaPtr, iValue);
	else if(sCommand == "modListenCheck")
		SetAreaModListenCheck(areaPtr, iValue);
	else if(sCommand == "SkyRingN")
		SetAreaSkyRing(areaPtr, 0, iValue);
	else if(sCommand == "SkyRingE")
		SetAreaSkyRing(areaPtr, 1, iValue);
	else if(sCommand == "SkyRingS")
		SetAreaSkyRing(areaPtr, 2, iValue);
	else if(sCommand == "SkyRingW")
		SetAreaSkyRing(areaPtr, 3, iValue);
	else if(sCommand.rfind(sDNCDesaturate,0) == 0)
		SetAreaDNCDesaturateColor(areaPtr, strtol(sCommand.substr(sDNCDesaturate.size()).c_str(), NULL, 10), iValue!=0);

}

int AreaGetInt(char* cCommand, int iObjectID) {
	std::string sCommand(cCommand);
	char* areaPtr = ptrToArea(iObjectID);
	
	if(areaPtr == NULL)
		return 0;

	std::string sDNCDesaturate = "DNCDesaturateColor";

	if(sCommand == "areaFlag")
		return GetAreaFlag(areaPtr);
	else if (sCommand == "creatorID")
		return GetCreatorID(areaPtr);
	else if(sCommand == "hasDayNightCycle")
		return GetAreaHasDayNightCycle(areaPtr);
	else if(sCommand == "isAlwaysNight")
		return GetAreaIsAlwaysNight(areaPtr);
	else if(sCommand == "useDayNightFog")
		return GetAreaUseDayNightFog(areaPtr);
	else if(sCommand == "hasDirLight")
		return GetAreaHasDirectionalLight(areaPtr);
	else if(sCommand == "isDirLightCastShadows")
		return GetAreaIsDirectionalLightCastShadows(areaPtr);
	else if(sCommand == "noRest")
		return GetAreaIsNoRest(areaPtr);
	else if(sCommand == "PVPMode")
		return GetAreaPVPMode(areaPtr);
	else if(sCommand == "modSpotCheck")
		return GetAreaModSpotCheck(areaPtr);
	else if(sCommand == "modListenCheck")
		return GetAreaModListenCheck(areaPtr);
	else if(sCommand == "SkyRingN")
		return GetAreaSkyRing(areaPtr, 0);
	else if(sCommand == "SkyRingE")
		return GetAreaSkyRing(areaPtr, 1);
	else if(sCommand == "SkyRingS")
		return GetAreaSkyRing(areaPtr, 2);
	else if(sCommand == "SkyRingW")
		return GetAreaSkyRing(areaPtr, 3);
	else if(sCommand.rfind(sDNCDesaturate,0) == 0)
		return GetAreaDNCDesaturateColor(areaPtr, strtol(sCommand.substr(sDNCDesaturate.size()).c_str(), NULL, 10));

	return 0;
}


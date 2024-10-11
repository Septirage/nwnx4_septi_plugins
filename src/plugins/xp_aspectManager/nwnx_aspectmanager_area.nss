//////////////////////////////////////////////////////////////////////////////////////////////
// nwnx_aspectmanager_area - area specific functions of the AspectManager plugin
// Original Scripter:  Septirage
//--------------------------------------------------------------------------------------------
// Last Modified By:	Septirage			2024-09-08	Add function GetAreaCreatorID  (1.4.5)
//						Septirage           2024-02-28
//--------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////
#include "nwnx_aspectmanager"



//Set the AreaFlags (define Exterior, Natural, Subteran or not)
void SetAreaFlags_xpAM(object oArea, int iFlags);

//Set the AreaFlags (define Exterior, Natural, Subteran or not)
int GetAreaFlags_xpAM(object oArea);



//Enable or disable possibility of Rest in oArea
void SetAreaNoRest_xpAM(object oArea, int bNoRest);

//Get the possibility of Rest in oArea
int GetAreaNoRest_xpAM(object oArea);



//Set the PVP Mode of oArea
//iPVPMode : PVP_SETTING_* constant
void SetAreaPVPMode_xpAM(object oArea, int iPvPMode);

//Get the PVP Mode of oArea
int GetAreaPVPMode_xpAM(object oArea);



//Set the Spot Modificator for this Area
void SetAreaModSpotCheck_xpAM(object oArea, int iModSpot);

//Get the Spot Modificator for this Area
int GetAreaModSpotCheck_xpAM(object oArea);



//Set the Listen Modificator for this Area
void SetAreaModListenCheck_xpAM(object oArea, int iModListen);

//Get the Listen Modificator for this Area
int GetAreaModListenCheck_xpAM(object oArea);

//Get the CreatorID of this Area
void GetAreaCreatorID_xpAM(object oArea);

//Set the SkyRing for  oArea
//sSkyRing : "N", "S", "E" or "W"
//iVal : desired Skyring.2DA id
void SetAreaSkyRing_xpAM(object oArea, string sSkyRing, int iVal);

//Get the SkyRing for  oArea
//sSkyRing : "N", "S", "E" or "W"
int GetAreaSkyRing_xpAM(object oArea, string sSkyRing);



//Enable or Disable the DirectionalLight for oArea
void SetAreaHasDirectionalLight_xpAM(object oArea, int bHasDirLight);

//Get the status of the DirectionalLight of oArea
int GetAreaHasDirectionalLight_xpAM(object oArea);



//Choose if the area DirectionalLight cast Shadow or not
void SetAreaIsDirLightCastShadow_xpAM(object oArea, int bDirLShadow);

//Get if the area DirectionalLight cast shadows or not
int GetAreaIsDirLightCastShadow_xpAM(object oArea);





/****************************************************************************/
/***************************** Day/Night Cycle ******************************/
/****************************************************************************/

//Enable or Disable the Day/Night Cycle for oArea
void SetAreaHasDayNightCycle_xpAM(object oArea, int bHasDNC);

//Get the activation status of Day/Night Cycle for oArea
int GetAreaHasDayNightCycle_xpAM(object oArea);



//If Day/Night Cycle is set to false, choose if oArea is always in Night not
void SetAreaIsAlwaysNight_xpAM(object oArea, int bNight);

//If Day/Night Cycle is set to false, get if oArea is always in Night not
int GetAreaIsAlwaysNight_xpAM(object oArea);



//If set to TRUE then the fog color will update as time passes.
void SetAreaUseDayNightFog_xpAM(object oArea, int bUseDNF);

//Get if fog color update with cycle or not
int GetAreaUseDayNightFog_xpAM(object oArea);



//Set the Sun (or moon) Corona Intensity during iDNC
//iDNC: DAYNIGHT_* constant
void SetAreaDNCCoronaIntensity_xpAM(object oArea, int iDNC, float fIntensity);

//Get the Sun (or moon) Corona Intensity during this iDNC
//iDNC: DAYNIGHT_* constant
float GetAreaDNCCoronaIntensity_xpAM(object oArea, int iDNC);



//Set the Shadow Intensity during iDNC
//iDNC: DAYNIGHT_* constant
void SetAreaDNCShadowIntensity_xpAM(object oArea, int iDNC, float fIntensity);

//Get the Shadow Intensity during iDNC
//iDNC: DAYNIGHT_* constant
float GetAreaDNCShadowIntensity_xpAM(object oArea, int iDNC);


//Set the "speed" of transition between Horizon and Zenith Color 
//iDNC: DAYNIGHT_* constant
void SetAreaDNCSkyRingColorInterpretation_xpAM(object oArea, int iDNC, float fLerp);

//Get the "speed" of transition between Horizon and Zenith Color 
//iDNC: DAYNIGHT_* constant
float GetAreaDNCSkyRingColorInterpretation_xpAM(object oArea, int iDNC);


//Set the SkyDomeModel of oArea during iDNC
//iDNC: DAYNIGHT_* constant
void SetAreaDNCSkyDomeModel_xpAM(object oArea, int iDNC, string sSkyDome);

//Set the SkyDomeModel of oArea during iDNC
//iDNC: DAYNIGHT_* constant
string GetAreaDNCSkyDomeModel_xpAM(object oArea, int iDNC);



/********************************** Bloom ***********************************/


void SetAreaDNCBloomBLRadius_xpAM(object oArea, int iDNC, float fRadius);
float GetAreaDNCBloomBLRadius_xpAM(object oArea, int iDNC);

void SetAreaDNCBloomGLIntensity_xpAM(object oArea, int iDNC, float fIntensity);
float GetAreaDNCBloomGLIntensity_xpAM(object oArea, int iDNC);

void SetAreaDNCBloomHLIntensity_xpAM(object oArea, int iDNC, float fIntensity);
float GetAreaDNCBloomHLIntensity_xpAM(object oArea, int iDNC);

void SetAreaDNCBloomHLThreshold_xpAM(object oArea, int iDNC, float fIntensity);
float GetAreaDNCBloomHLThreshold_xpAM(object oArea, int iDNC);

void SetAreaDNCBloomSCIntensity_xpAM(object oArea, int iDNC, float fIntensity);
float GetAreaDNCBloomSCIntensity_xpAM(object oArea, int iDNC);



/*********************************** HDR ************************************/

void SetAreaDNCHDRAvg_xpAM(object oArea, int iDNC, float fHDR);
float GetAreaDNCHDRAvg_xpAM(object oArea, int iDNC);

void SetAreaDNCHDRMax_xpAM(object oArea, int iDNC, float fHDR);
float GetAreaDNCHDRMax_xpAM(object oArea, int iDNC);

void SetAreaDNCHDRExposure_xpAM(object oArea, int iDNC, float fHDR);
float GetAreaDNCHDRExposure_xpAM(object oArea, int iDNC);

void SetAreaDNCHDRHighlight_xpAM(object oArea, int iDNC, float fHDR);
float GetAreaDNCHDRHighlight_xpAM(object oArea, int iDNC);



/********************************** Cloud ***********************************/

//Set a Value between 0 and 1,5 that determines the amount of clouds that appear
//	in the sky. Lower values produce greater cloud cover.
//iDNC: DAYNIGHT_* constant
//fCover : value between 0 and 1.5
void SetAreaDNCCloudCover_xpAM(object oArea, int iDNC, float fCover);

//Get the Cloud Cover
//iDNC: DAYNIGHT_* constant
float GetAreaDNCCloudCover_xpAM(object oArea, int iDNC);


//Set Cloud Movement rate in the default east-west direction
//	Positive Value will cause movement toward the west. 
//iDNC: DAYNIGHT_* constant
void SetAreaDNCCloudMoveX_xpAM(object oArea, int iDNC, float fMove);

//Get The East-West cloud movement speed
//iDNC: DAYNIGHT_* constant
float GetAreaDNCCloudMoveX_xpAM(object oArea, int iDNC);

//Set Cloud Movement rate in the default north-south direction
//	Positive Value will cause movement toward the south. 
//iDNC: DAYNIGHT_* constant
void SetAreaDNCCloudMoveY_xpAM(object oArea, int iDNC, float fMove);

//Get The North-South cloud movement speed
//iDNC: DAYNIGHT_* constant
float GetAreaDNCCloudMoveY_xpAM(object oArea, int iDNC);




/******************************** Desaturate ********************************/

//Enable or Disable the Desaturation effect
//iDNC: DAYNIGHT_* constant
void SetAreaDNCDesaturateColor_xpAM(object oArea, int iDNC, int bDesaturate);

//Get the status of desaturation effect
//iDNC: DAYNIGHT_* constant
int GetAreaDNCDesaturateColor_xpAM(object oArea, int iDNC);


//Set the desaturation factor (if desaturateColor is set to TRUE)
//iDNC: DAYNIGHT_* constant
//fFactor: value from 0 to 1
void SetAreaDNCDesaturateFactor_xpAM(object oArea, int iDNC, float fFactor);

//Get the desaturation factor
//iDNC: DAYNIGHT_* constant
float GetAreaDNCDesaturateFactor_xpAM(object oArea, int iDNC);





/******************************* GroundLight ********************************/

void SetAreaDNCGroundLightIntensity_xpAM(object oArea, int iDNC, float fIntensity);

float GetAreaDNCGroundLightIntensity_xpAM(object oArea, int iDNC);



void SetAreaDNCGroundLightColor_xpAM(object oArea, int iDNC, string sColorType, string sRGB);

string GetAreaDNCGroundLightColor_xpAM(object oArea, int iDNC, string sColorType);


/********************************* SkyLight *********************************/

void SetAreaDNCSkylightIntensity_xpAM(object oArea, int iDNC, float fIntensity);

float GetAreaDNCSkylightIntensity_xpAM(object oArea, int iDNC);



void SetAreaDNCSkylightColor_xpAM(object oArea, int iDNC, string sColorType, string sRGB);

string GetAreaDNCSkylightColor_xpAM(object oArea, int iDNC, string sColorType);


/********************************* SunMoon **********************************/

void SetAreaDNCSunMoonIntensity_xpAM(object oArea, int iDNC, float fIntensity);

float GetAreaDNCSunMoonIntensity_xpAM(object oArea, int iDNC);



void SetAreaDNCSunMoonColor_xpAM(object oArea, int iDNC, string sColorType, string sRGB);

string GetAreaDNCSunMoonColor_xpAM(object oArea, int iDNC, string sColorType);



void SetAreaDNCSunMoonDirX_xpAM(object oArea, int iDNC, float fDir);

float GetAreaDNCSunMoonDirX_xpAM(object oArea, int iDNC);



void SetAreaDNCSunMoonDirY_xpAM(object oArea, int iDNC, float fDir);

float GetAreaDNCSunMoonDirY_xpAM(object oArea, int iDNC);



void SetAreaDNCSunMoonDirZ_xpAM(object oArea, int iDNC, float fDir);

float GetAreaDNCSunMoonDirZ_xpAM(object oArea, int iDNC);



/*********************************** Fog ************************************/

void SetAreaDNCFogStart_xpAM(object oArea, int iDNC, float fStart);

float GetAreaDNCFogStart_xpAM(object oArea, int iDNC);



void SetAreaDNCFogEnd_xpAM(object oArea, int iDNC, float fEnd);

float GetAreaDNCFogEnd_xpAM(object oArea, int iDNC);



void SetAreaDNCFogFarClip_xpAM(object oArea, int iDNC, float fFarClip);

float GetAreaDNCFogFarClip_xpAM(object oArea, int iDNC);



void SetAreaDNCFogColor_xpAM(object oArea, int iDNC, string sRGB);

string GetAreaDNCFogColor_xpAM(object oArea, int iDNC);


/******************************* Other Colors *******************************/


void SetAreaDNCSkyHorizonColor_xpAM(object oArea, int iDNC, string sRGB);

string GetAreaDNCSkyHorizonColor_xpAM(object oArea, int iDNC);



void SetAreaDNCSkyZenithColor_xpAM(object oArea, int iDNC, string sRGB);

string GetAreaDNCSkyZenithColor_xpAM(object oArea, int iDNC);


/*****************************************************************************/

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////// CONSTANTS //////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/*****************************************************************************/


const int PVP_SETTING_NO					= 0;
const int PVP_SETTING_GROUP					= 1;
const int PVP_SETTING_TOTAL					= 2;
const int PVP_SETTING_SERVER				= 3;

const int DAYNIGHT_SUNRISE					= 0;
const int DAYNIGHT_DAYTIME					= 1;
const int DAYNIGHT_SUNSET					= 2;
const int DAYNIGHT_MOONRISE					= 3;
const int DAYNIGHT_NIGHTTIME				= 4;
const int DAYNIGHT_MOONSET					= 5;
const int DAYNIGHT_TRANSITION				= 6;
const int DAYNIGHT_DEFAULT					= 7;

/*****************************************************************************/

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////// IMPLEMENTATIONS ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/*****************************************************************************/

void SetAreaFlags_xpAM(object oArea, int iFlags)
{
	NWNXSetInt("AspectManager", "area", "areaFlag", ObjectToInt(oArea), iFlags);
}

int GetAreaFlags_xpAM(object oArea)
{
	return NWNXGetInt("AspectManager", "area", "areaFlag", ObjectToInt(oArea));
}



void SetAreaNoRest_xpAM(object oArea, int bNoRest)
{
	NWNXSetInt("AspectManager", "area", "noRest", ObjectToInt(oArea), bNoRest);
}

int GetAreaNoRest_xpAM(object oArea)
{
	return NWNXGetInt("AspectManager", "area", "noRest", ObjectToInt(oArea));
}



void SetAreaPVPMode_xpAM(object oArea, int iPvPMode)
{
	NWNXSetInt("AspectManager", "area", "PVPMode", ObjectToInt(oArea), iPvPMode);
}

int GetAreaPVPMode_xpAM(object oArea)
{
	return NWNXGetInt("AspectManager", "area", "PVPMode", ObjectToInt(oArea));
}



void SetAreaModSpotCheck_xpAM(object oArea, int iModSpot)
{
	NWNXSetInt("AspectManager", "area", "modSpotCheck", ObjectToInt(oArea), iModSpot);
}

int GetAreaModSpotCheck_xpAM(object oArea)
{
	return NWNXGetInt("AspectManager", "area", "modSpotCheck", ObjectToInt(oArea));
}



void SetAreaModListenCheck_xpAM(object oArea, int iModListen)
{
	NWNXSetInt("AspectManager", "area", "modListenCheck", ObjectToInt(oArea), iModListen);
}

int GetAreaModListenCheck_xpAM(object oArea)
{
	return NWNXGetInt("AspectManager", "area", "modListenCheck", ObjectToInt(oArea));
}


void GetAreaCreatorID_xpAM(object oArea)
{
	return NWNXGetInt("AspectManager", "area", "creatorID", ObjectToInt(oArea));
}


void SetAreaSkyRing_xpAM(object oArea, string sSkyRing, int iVal)
{
	NWNXSetInt("AspectManager", "area", "SkyRing"+sSkyRing, ObjectToInt(oArea), iVal);
}

int GetAreaSkyRing_xpAM(object oArea, string sSkyRing)
{
	return NWNXGetInt("AspectManager", "area", "SkyRing"+sSkyRing, ObjectToInt(oArea));
}



void SetAreaHasDirectionalLight_xpAM(object oArea, int bHasDirLight)
{
	NWNXSetInt("AspectManager", "area", "hasDirLight", ObjectToInt(oArea), bHasDirLight);
}

int GetAreaHasDirectionalLight_xpAM(object oArea)
{
	return NWNXGetInt("AspectManager", "area", "hasDirLight", ObjectToInt(oArea));
}



void SetAreaIsDirLightCastShadow_xpAM(object oArea, int bDirLShadow)
{
	NWNXSetInt("AspectManager", "area", "isDirLightCastShadows", ObjectToInt(oArea), bDirLShadow);
}

int GetAreaIsDirLightCastShadow_xpAM(object oArea)
{
	return NWNXGetInt("AspectManager", "area", "isDirLightCastShadows", ObjectToInt(oArea));
}




/****************************************************************************/
/***************************** Day/Night Cycles *****************************/
/****************************************************************************/

void SetAreaHasDayNightCycle_xpAM(object oArea, int bHasDNC)
{
	NWNXSetInt("AspectManager", "area", "hasDayNightCycle", ObjectToInt(oArea), bHasDNC);
}

int GetAreaHasDayNightCycle_xpAM(object oArea)
{
	return NWNXGetInt("AspectManager", "area", "hasDayNightCycle", ObjectToInt(oArea));
}



void SetAreaIsAlwaysNight_xpAM(object oArea, int bNight)
{
	NWNXSetInt("AspectManager", "area", "isAlwaysNight", ObjectToInt(oArea), bNight);
}

int GetAreaIsAlwaysNight_xpAM(object oArea)
{
	return NWNXGetInt("AspectManager", "area", "isAlwaysNight", ObjectToInt(oArea));
}



void SetAreaUseDayNightFog_xpAM(object oArea, int bUseDNF)
{
	NWNXSetInt("AspectManager", "area", "useDayNightFog", ObjectToInt(oArea), bUseDNF);
}

int GetAreaUseDayNightFog_xpAM(object oArea)
{
	return NWNXGetInt("AspectManager", "area", "useDayNightFog", ObjectToInt(oArea));
}



void SetAreaDNCCoronaIntensity_xpAM(object oArea, int iDNC, float fIntensity)
{
	string sCmd = "DNC" + IntToString(iDNC) + "CoronaIntensity";
	NWNXSetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea), fIntensity);
}

float GetAreaDNCCoronaIntensity_xpAM(object oArea, int iDNC)
{
	string sCmd = "DNC" + IntToString(iDNC) + "CoronaIntensity";
	return NWNXGetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea));
}



void SetAreaDNCShadowIntensity_xpAM(object oArea, int iDNC, float fIntensity)
{
	string sCmd = "DNC" + IntToString(iDNC) + "ShadowIntensity";
	NWNXSetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea), fIntensity);
}

float GetAreaDNCShadowIntensity_xpAM(object oArea, int iDNC)
{
	string sCmd = "DNC" + IntToString(iDNC) + "ShadowIntensity";
	return NWNXGetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea));
}

void SetAreaDNCSkyRingColorInterpretation_xpAM(object oArea, int iDNC, float fLerp)
{
	string sCmd = "DNC" + IntToString(iDNC) + "SkyringLerp";
	NWNXSetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea), fLerp);
}

float GetAreaDNCSkyRingColorInterpretation_xpAM(object oArea, int iDNC)
{
	string sCmd = "DNC" + IntToString(iDNC) + "SkyringLerp";
	return NWNXGetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea));
}


void SetAreaDNCSkyDomeModel_xpAM(object oArea, int iDNC, string sSkyDome)
{
	string sCmd = "DNC" + IntToString(iDNC) + "SkyDomeModel";
	NWNXSetString("AspectManager", "area", sCmd, ObjectToInt(oArea), sSkyDome);
}

string GetAreaDNCSkyDomeModel_xpAM(object oArea, int iDNC)
{
	string sCmd = "DNC" + IntToString(iDNC) + "SkyDomeModel";
	return NWNXGetString("AspectManager", "area", sCmd, ObjectToInt(oArea));
}



/********************************** Bloom ***********************************/

void SetAreaDNCBloomBLRadius_xpAM(object oArea, int iDNC, float fRadius)
{
	string sCmd = "DNC" + IntToString(iDNC) + "BloomBLRadius";
	NWNXSetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea), fRadius);
}

float GetAreaDNCBloomBLRadius_xpAM(object oArea, int iDNC)
{
	string sCmd = "DNC" + IntToString(iDNC) + "BloomBLRadius";
	return NWNXGetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea));
}

void SetAreaDNCBloomGLIntensity_xpAM(object oArea, int iDNC, float fIntensity)
{
	string sCmd = "DNC" + IntToString(iDNC) + "BloomGLIntensity";
	NWNXSetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea), fIntensity);
}

float GetAreaDNCBloomGLIntensity_xpAM(object oArea, int iDNC)
{
	string sCmd = "DNC" + IntToString(iDNC) + "BloomGLIntensity";
	return NWNXGetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea));
}


void SetAreaDNCBloomHLIntensity_xpAM(object oArea, int iDNC, float fIntensity)
{
	string sCmd = "DNC" + IntToString(iDNC) + "BloomHLIntensity";
	NWNXSetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea), fIntensity);
}

float GetAreaDNCBloomHLIntensity_xpAM(object oArea, int iDNC)
{
	string sCmd = "DNC" + IntToString(iDNC) + "BloomHLIntensity";
	return NWNXGetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea));
}


void SetAreaDNCBloomHLThreshold_xpAM(object oArea, int iDNC, float fThreshold)
{
	string sCmd = "DNC" + IntToString(iDNC) + "BloomHLThreshold";
	NWNXSetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea), fThreshold);
}

float GetAreaDNCBloomHLThreshold_xpAM(object oArea, int iDNC)
{
	string sCmd = "DNC" + IntToString(iDNC) + "BloomHLThreshold";
	return NWNXGetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea));
}



void SetAreaDNCBloomSCIntensity_xpAM(object oArea, int iDNC, float fIntensity)
{
	string sCmd = "DNC" + IntToString(iDNC) + "BloomSCIntensity";
	NWNXSetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea), fIntensity);
}

float GetAreaDNCBloomSCIntensity_xpAM(object oArea, int iDNC)
{
	string sCmd = "DNC" + IntToString(iDNC) + "BloomSCIntensity";
	return NWNXGetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea));
}




/********************************** Cloud ***********************************/

void SetAreaDNCCloudCover_xpAM(object oArea, int iDNC, float fCover)
{
	string sCmd = "DNC" + IntToString(iDNC) + "CloudCover";
	NWNXSetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea), fCover);
}

float GetAreaDNCCloudCover_xpAM(object oArea, int iDNC)
{
	string sCmd = "DNC" + IntToString(iDNC) + "CloudCover";
	return NWNXGetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea));
}



void SetAreaDNCCloudMoveX_xpAM(object oArea, int iDNC, float fMove)
{
	string sCmd = "DNC" + IntToString(iDNC) + "CloudMoveX";
	NWNXSetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea), fMove);
}

float GetAreaDNCCloudMoveX_xpAM(object oArea, int iDNC)
{
	string sCmd = "DNC" + IntToString(iDNC) + "CloudMoveX";
	return NWNXGetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea));
}



void SetAreaDNCCloudMoveY_xpAM(object oArea, int iDNC, float fMove)
{
	string sCmd = "DNC" + IntToString(iDNC) + "CloudMoveY";
	NWNXSetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea), fMove);
}

float GetAreaDNCCloudMoveY_xpAM(object oArea, int iDNC)
{
	string sCmd = "DNC" + IntToString(iDNC) + "CloudMoveY";
	return NWNXGetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea));
}






/******************************** Desaturate ********************************/

void SetAreaDNCDesaturateColor_xpAM(object oArea, int iDNC, int bDesaturate)
{
	NWNXSetInt("AspectManager", "area", "DNCDesaturateColor"+IntToString(iDNC), ObjectToInt(oArea), bDesaturate);
}

int GetAreaDNCDesaturateColor_xpAM(object oArea, int iDNC)
{
	return NWNXGetInt("AspectManager", "area", "DNCDesaturateColor"+IntToString(iDNC), ObjectToInt(oArea));
}



void SetAreaDNCDesaturateFactor_xpAM(object oArea, int iDNC, float fFactor)
{
	string sCmd = "DNC" + IntToString(iDNC) + "DesaturateFactor";
	NWNXSetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea), fFactor);
}

float GetAreaDNCDesaturateFactor_xpAM(object oArea, int iDNC)
{
	string sCmd = "DNC" + IntToString(iDNC) + "DesaturateFactor";
	return NWNXGetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea));
}




/*********************************** HDR ************************************/

void SetAreaDNCHDRAvg_xpAM(object oArea, int iDNC, float fHDR)
{
	string sCmd = "DNC" + IntToString(iDNC) + "HDRAvg";
	NWNXSetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea), fHDR);
}

float GetAreaDNCHDRAvg_xpAM(object oArea, int iDNC)
{
	string sCmd = "DNC" + IntToString(iDNC) + "HDRAvg";
	return NWNXGetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea));
}



void SetAreaDNCHDRMax_xpAM(object oArea, int iDNC, float fHDR)
{
	string sCmd = "DNC" + IntToString(iDNC) + "HDRMax";
	NWNXSetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea), fHDR);
}

float GetAreaDNCHDRMax_xpAM(object oArea, int iDNC)
{
	string sCmd = "DNC" + IntToString(iDNC) + "HDRMax";
	return NWNXGetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea));
}



void SetAreaDNCHDRExposure_xpAM(object oArea, int iDNC, float fHDR)
{
	string sCmd = "DNC" + IntToString(iDNC) + "Exposure";
	NWNXSetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea), fHDR);
}

float GetAreaDNCHDRExposure_xpAM(object oArea, int iDNC)
{
	string sCmd = "DNC" + IntToString(iDNC) + "Exposure";
	return NWNXGetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea));
}



void SetAreaDNCHDRHighlight_xpAM(object oArea, int iDNC, float fHDR)
{
	string sCmd = "DNC" + IntToString(iDNC) + "HDRHighlight";
	NWNXSetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea), fHDR);
}

float GetAreaDNCHDRHighlight_xpAM(object oArea, int iDNC)
{
	string sCmd = "DNC" + IntToString(iDNC) + "HDRHighlight";
	return NWNXGetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea));
}



/********************************* SkyLight *********************************/

void SetAreaDNCSkylightIntensity_xpAM(object oArea, int iDNC, float fIntensity)
{
	string sCmd = "DNC" + IntToString(iDNC) + "SkylightIntensity";
	NWNXSetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea), fIntensity);
}

float GetAreaDNCSkylightIntensity_xpAM(object oArea, int iDNC)
{
	string sCmd = "DNC" + IntToString(iDNC) + "SkylightIntensity";
	return NWNXGetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea));
}



void SetAreaDNCSkylightColor_xpAM(object oArea, int iDNC, string sColorType, string sRGB)
{
	string sCmd = "DNC" + IntToString(iDNC) + "SkyLight";
	NWNXSetString("AspectManager", "area", sCmd+sColorType, ObjectToInt(oArea), sRGB);
}

string GetAreaDNCSkylightColor_xpAM(object oArea, int iDNC, string sColorType)
{
	string sCmd = "DNC" + IntToString(iDNC) + "SkyLight";
	return NWNXGetString("AspectManager", "area", sCmd+sColorType, ObjectToInt(oArea));
}



/********************************* SunMoon **********************************/

void SetAreaDNCSunMoonIntensity_xpAM(object oArea, int iDNC, float fIntensity)
{
	string sCmd = "DNC" + IntToString(iDNC) + "SunMoonIntensity";
	NWNXSetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea), fIntensity);
}

float GetAreaDNCSunMoonIntensity_xpAM(object oArea, int iDNC)
{
	string sCmd = "DNC" + IntToString(iDNC) + "SunMoonIntensity";
	return NWNXGetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea));
}



void SetAreaDNCSunMoonColor_xpAM(object oArea, int iDNC, string sColorType, string sRGB)
{
	string sCmd = "DNC" + IntToString(iDNC) + "SunMoon";
	NWNXSetString("AspectManager", "area", sCmd+sColorType, ObjectToInt(oArea), sRGB);
}

string GetAreaDNCSunMoonColor_xpAM(object oArea, int iDNC, string sColorType)
{
	string sCmd = "DNC" + IntToString(iDNC) + "SunMoon";
	return NWNXGetString("AspectManager", "area", sCmd+sColorType, ObjectToInt(oArea));
}


void SetAreaDNCSunMoonDirX_xpAM(object oArea, int iDNC, float fDir)
{
	string sCmd = "DNC" + IntToString(iDNC) + "SunMoonDirX";
	NWNXSetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea), fDir);
}

float GetAreaDNCSunMoonDirX_xpAM(object oArea, int iDNC)
{
	string sCmd = "DNC" + IntToString(iDNC) + "SunMoonDirX";
	return NWNXGetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea));
}

void SetAreaDNCSunMoonDirY_xpAM(object oArea, int iDNC, float fDir)
{
	string sCmd = "DNC" + IntToString(iDNC) + "SunMoonDirY";
	NWNXSetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea), fDir);
}

float GetAreaDNCSunMoonDirY_xpAM(object oArea, int iDNC)
{
	string sCmd = "DNC" + IntToString(iDNC) + "SunMoonDirY";
	return NWNXGetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea));
}


void SetAreaDNCSunMoonDirZ_xpAM(object oArea, int iDNC, float fDir)
{
	string sCmd = "DNC" + IntToString(iDNC) + "SunMoonDirZ";
	NWNXSetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea), fDir);
}

float GetAreaDNCSunMoonDirZ_xpAM(object oArea, int iDNC)
{
	string sCmd = "DNC" + IntToString(iDNC) + "SunMoonDirZ";
	return NWNXGetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea));
}



/******************************* GroundLight ********************************/

void SetAreaDNCGroundLightIntensity_xpAM(object oArea, int iDNC, float fIntensity)
{
	string sCmd = "DNC" + IntToString(iDNC) + "GroundLightIntensity";
	NWNXSetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea), fIntensity);
}


float GetAreaDNCGroundLightIntensity_xpAM(object oArea, int iDNC)
{
	string sCmd = "DNC" + IntToString(iDNC) + "GroundLightIntensity";
	return NWNXGetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea));
}



void SetAreaDNCGroundLightColor_xpAM(object oArea, int iDNC, string sColorType, string sRGB)
{
	string sCmd = "DNC" + IntToString(iDNC) + "GroundLight";
	NWNXSetString("AspectManager", "area", sCmd+sColorType, ObjectToInt(oArea), sRGB);
}


string GetAreaDNCGroundLightColor_xpAM(object oArea, int iDNC, string sColorType)
{
	string sCmd = "DNC" + IntToString(iDNC) + "GroundLight";
	return NWNXGetString("AspectManager", "area", sCmd+sColorType, ObjectToInt(oArea));
}


/*********************************** Fog ************************************/

void SetAreaDNCFogStart_xpAM(object oArea, int iDNC, float fStart)
{
	string sCmd = "DNC" + IntToString(iDNC) + "FogStart";
	NWNXSetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea), fStart);
}

float GetAreaDNCFogStart_xpAM(object oArea, int iDNC)
{
	string sCmd = "DNC" + IntToString(iDNC) + "FogStart";
	return NWNXGetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea));
}

void SetAreaDNCFogEnd_xpAM(object oArea, int iDNC, float fEnd)
{
	string sCmd = "DNC" + IntToString(iDNC) + "FogEnd";
	NWNXSetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea), fEnd);
}

float GetAreaDNCFogEnd_xpAM(object oArea, int iDNC)
{
	string sCmd = "DNC" + IntToString(iDNC) + "FogEnd";
	return NWNXGetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea));
}

void SetAreaDNCFogFarClip_xpAM(object oArea, int iDNC, float fFarClip)
{
	string sCmd = "DNC" + IntToString(iDNC) + "FogFarClip";
	NWNXSetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea), fFarClip);
}

float GetAreaDNCFogFarClip_xpAM(object oArea, int iDNC)
{
	string sCmd = "DNC" + IntToString(iDNC) + "FogFarClip";
	return NWNXGetFloat("AspectManager", "area", sCmd, ObjectToInt(oArea));
}

void SetAreaDNCFogColor_xpAM(object oArea, int iDNC, string sRGB)
{
	string sCmd = "DNC" + IntToString(iDNC) + "FogColor";
	NWNXSetString("AspectManager", "area", sCmd, ObjectToInt(oArea), sRGB);
}

string GetAreaDNCFogColor_xpAM(object oArea, int iDNC)
{
	string sCmd = "DNC" + IntToString(iDNC) + "FogColor";
	return NWNXGetString("AspectManager", "area", sCmd, ObjectToInt(oArea));
}


/******************************* Other Colors *******************************/

void SetAreaDNCSkyHorizonColor_xpAM(object oArea, int iDNC, string sRGB)
{
	string sCmd = "DNC" + IntToString(iDNC) + "SkyHorizon";
	NWNXSetString("AspectManager", "area", sCmd, ObjectToInt(oArea), sRGB);
}

string GetAreaDNCSkyHorizonColor_xpAM(object oArea, int iDNC)
{
	string sCmd = "DNC" + IntToString(iDNC) + "SkyHorizon";
	return NWNXGetString("AspectManager", "area", sCmd, ObjectToInt(oArea));
}



void SetAreaDNCSkyZenithColor_xpAM(object oArea, int iDNC, string sRGB)
{
	string sCmd = "DNC" + IntToString(iDNC) + "SkyZenith";
	NWNXSetString("AspectManager", "area", sCmd, ObjectToInt(oArea), sRGB);
}

string GetAreaDNCSkyZenithColor_xpAM(object oArea, int iDNC)
{
	string sCmd = "DNC" + IntToString(iDNC) + "SkyHorizon";
	return NWNXGetString("AspectManager", "area", sCmd, ObjectToInt(oArea));
}
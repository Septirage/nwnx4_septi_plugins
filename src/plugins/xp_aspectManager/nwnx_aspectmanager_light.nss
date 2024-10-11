​​​​​//////////////////////////////////////////////////////////////////////////////////////////////
// nwnx_aspectmanager_light - light specific functions of the AspectManager plugin
// Original Scripter:  Septirage
//--------------------------------------------------------------------------------------------
// Last Modified By:	Septirage           2024-02-28
//--------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////
#include "nwnx_aspectmanager"


//Set the maximum distance that the light's diffuse color will reach
void SetLightRange_xpAM(object oLight, float fRange);

//Get the maximum distance that the light's diffuse  color will reach
float GetLightRange_xpAM(object oLight);


/********************************** Shadow **********************************/

//Set if the light should casts shadows or not
void SetLightCastShadow_xpAM(object oLight, int bCastShadow);

//Get if the light casts shadows or not
int GetLightCastShadow_xpAM(object oLight);


//Set the intensity of shadows cast by the light
void SetLightShadowIntensity_xpAM(object oLight, float fIntensity);

//Get the intensity of shadows cast by the light
float GetLightShadowIntensity_xpAM(object oLight);



/*********************************** Light **********************************/

//Set the intensity (brightness) of the light within it's Range
void SetLightColorIntensity_xpAM(object oLight, float fColorIntensity);

//Get the intensity (brightness) of the light within it's Range
float GetLightColorIntensity_xpAM(object oLight);


//Set the Choosen light color value.
//	sColorType :  LIGHT_COLOR_TYPE_* constant
// sRGB: a RGB string value, on the "#FFFFFF" (or #FFFFFFFF) format
void SetLightColor_xpAM(object oLight, string sColorType, string sRGB);


//Get the Choosen light color value.
//	sColorType :  LIGHT_COLOR_TYPE_* constant
string GetLightColor_xpAM(object oLight, string sColorType);



/*********************************** Lerp ***********************************/

//Enable or Disable Lerp for this light
void SetLightIsLerp_xpAM(object oLight, int bLerp);

//Get the status of Lerp for this light
int GetLightIsLerp_xpAM(object oLight);

//Set the time in seconds for the light to cycle between the Color and Lerp Target Color.
void SetLightLerpPeriod_xpAM(object oLight, float fLerpPeriod);

//Get the time in seconds for the light to cycle between the Color and Lerp Target Color.
float GetLightLerpPeriod_xpAM(object oLight);

//Set the intensity (brightness) of the lerp light within it's Range
void SetLightLerpIntensity_xpAM(object oLight, float fLerpIntensity);

//Get the intensity (brightness) of the lerp light within it's Range
float GetLightLerpIntensity_xpAM(object oLight);

//Set the Choosen light lerp color value.
//	sColorType :  LIGHT_COLOR_TYPE_* constant
// sRGB: a RGB string value, on the "#FFFFFF" (or #FFFFFFFF) format
void SetLightLerpColor_xpAM(object oLight, string sColorType, string sRGB);

//Get the Choosen light lerp color value.
//	sColorType :  LIGHT_COLOR_TYPE_* constant
string GetLightLerpColor_xpAM(object oLight, string sColorType);


/********************************* Flicker **********************************/

//Enable or Disable Flicker for this light
void SetLightIsFlicker_xpAM(object oLight, int bFlicker);

//Get the flicker activation status for this light
int GetLightIsFlicker_xpAM(object oLight);

//Set the light Flicker Type with a FLICKER_TYPE_* value
void SetLightFlickerType_xpAM(object oLight, int iFlickerType);

//Get the FLICKER_TYPE_* corresponding to the Flicker Type
int GetLightFlickerType_xpAM(object oLight);

//Set the Flicker Variance value 
void SetLightFlickerVariance_xpAM(object oLight, float fVariance);

//Get the Flicker Variance value
float GetLightFlickerVariance_xpAM(object oLight);

//Set the flicker rate for this light
void SetLightFlickerRate_xpAM(object oLight, float fRate);

//Get the Flicker Rate for this light
float GetLightFlickerRate_xpAM(object oLight);







/*****************************************************************************/

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////// IMPLEMENTATIONS ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/*****************************************************************************/




//Light


void SetLightRange_xpAM(object oLight, float fRange)
{
	NWNXSetFloat("AspectManager", "light", "Range", ObjectToInt(oLight), fRange);
}

float GetLightRange_xpAM(object oLight)
{
	return NWNXGetFloat("AspectManager", "light", "Range", ObjectToInt(oLight));
}

void SetLightCastShadow_xpAM(object oLight, int bCastShadow)
{
	NWNXSetInt("AspectManager", "light", "CastsShadow", ObjectToInt(oLight), bCastShadow);
}

int GetLightCastShadow_xpAM(object oLight)
{
	return NWNXGetInt("AspectManager", "light", "CastsShadow", ObjectToInt(oLight));
}

void SetLightShadowIntensity_xpAM(object oLight, float fIntensity)
{
	NWNXSetFloat("AspectManager", "light", "ShadowIntensity", ObjectToInt(oLight), fIntensity);
}

float GetLightShadowIntensity_xpAM(object oLight)
{
	return NWNXGetFloat("AspectManager", "light", "ShadowIntensity", ObjectToInt(oLight));
}

void SetLightColorIntensity_xpAM(object oLight, float fColorIntensity)
{
	NWNXSetFloat("AspectManager", "light", "ColorIntensity", ObjectToInt(oLight), fColorIntensity);
}

float GetLightColorIntensity_xpAM(object oLight)
{
	return NWNXGetFloat("AspectManager", "light", "ColorIntensity", ObjectToInt(oLight));
}

void SetLightColor_xpAM(object oLight, string sColorType, string sRGB)
{	
	NWNXSetString("AspectManager", "light", sColorType, ObjectToInt(oLight), sRGB);
}

string GetLightColor_xpAM(object oLight, string sColorType)
{	
	return NWNXGetString("AspectManager", "light", sColorType, ObjectToInt(oLight));
}

void SetLightIsLerp_xpAM(object oLight, int bLerp)
{
	NWNXSetInt("AspectManager", "light", "Lerp", ObjectToInt(oLight), bLerp);
}

int GetLightIsLerp_xpAM(object oLight)
{
	return NWNXGetInt("AspectManager", "light", "Lerp", ObjectToInt(oLight));
}

void SetLightLerpPeriod_xpAM(object oLight, float fLerpPeriod)
{
	NWNXSetFloat("AspectManager", "light", "LerpPeriod", ObjectToInt(oLight), fLerpPeriod);
}

float GetLightLerpPeriod_xpAM(object oLight)
{
	return NWNXGetFloat("AspectManager", "light", "LerpPeriod", ObjectToInt(oLight));
}

void SetLightLerpIntensity_xpAM(object oLight, float fLerpIntensity)
{
	NWNXSetFloat("AspectManager", "light", "LerpIntensity", ObjectToInt(oLight), fLerpIntensity);
}

float GetLightLerpIntensity_xpAM(object oLight)
{
	return NWNXGetFloat("AspectManager", "light", "LerpIntensity", ObjectToInt(oLight));
}

void SetLightLerpColor_xpAM(object oLight, string sColorType, string sRGB)
{
	string sVal = "Lerp"+sColorType;
	
	NWNXSetString("AspectManager", "light", sVal, ObjectToInt(oLight), sRGB);
}

string GetLightLerpColor_xpAM(object oLight, string sColorType)
{
	string sVal = "Lerp"+sColorType;
	
	return NWNXGetString("AspectManager", "light", sVal, ObjectToInt(oLight));
}

void SetLightIsFlicker_xpAM(object oLight, int bFlicker)
{
	NWNXSetInt("AspectManager", "light", "Flicker", ObjectToInt(oLight), bFlicker);	
}

int GetLightIsFlicker_xpAM(object oLight)
{
	return NWNXGetInt("AspectManager", "light", "Flicker", ObjectToInt(oLight));	
}

void SetLightFlickerType_xpAM(object oLight, int iFlickerType)
{
	NWNXSetInt("AspectManager", "light", "FlickerType", ObjectToInt(oLight), iFlickerType);	
}

int GetLightFlickerType_xpAM(object oLight)
{
	return NWNXGetInt("AspectManager", "light", "FlickerType", ObjectToInt(oLight));	
}

void SetLightFlickerVariance_xpAM(object oLight, float fVariance)
{
	NWNXSetFloat("AspectManager", "light", "FlickerVariance", ObjectToInt(oLight), fVariance);
}

float GetLightFlickerVariance_xpAM(object oLight)
{
	return NWNXGetFloat("AspectManager", "light", "FlickerVariance", ObjectToInt(oLight));
}

void SetLightFlickerRate_xpAM(object oLight, float fRate)
{
	NWNXSetFloat("AspectManager", "light", "FlickerRate", ObjectToInt(oLight), fRate);
}

float GetLightFlickerRate_xpAM(object oLight)
{
	return NWNXGetFloat("AspectManager", "light", "FlickerRate", ObjectToInt(oLight));
}
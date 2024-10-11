//////////////////////////////////////////////////////////////////////////////////////////////
// nwnx_encraft - various functions for accessing the CraftEnhanced plugin
// Original Scripter:  Septirage
//--------------------------------------------------------------------------------------------
// Last Modified By:   Septirage           09/05/22
//--------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////
// Beta Version
//////////////////////////////////////////////////////////////////////////////////////////////

/********************************************************
Thanks : To Papillon & Denis-Clinton Carter for their work on xp_craft on wich
			EnhancedCraft is based.

***********************************************************/

#include "include_craft_esthetique"


const string XPEnCraft_DATABASE_PREFIX = "xp_encraft";//set it to whatever you want



string XPEnCraft_GetPCID(object oPC)
{
	return GetPCUID_CE(oPC);
}

/*****************************************************************************/
//Bioware Database related functions

const int XP_CRAFT_VARIATION_LISTS_BASE = 1;

//creates and return the name of the datafile for this PC
//actually using the playre's public CD Key as a sufix to the database filename
//if you're using any kind of PC ID (ie incremental sql unique IDs), feel free to rescript this
string XPEnCraft_GetDatafileName(object oPC);

//destroy the given database
void XPEnCraft_DestroyDataFile(object oPC, string sDatafileName="");

//store the given object into a Bioware database file
//returns TRUE or FALSE whether the operation was successful or not
int XPEnCraft_StoreItemToCraft(object oPC, object oItem, int bSafeStore = TRUE);

//Retrieve a PC's Item from the bioware database and spawn it on the ground at oPC's location
object XPEnCraft_RetrieveCraftedItem(object oPC, location lSpawnLocation);


int XPEnCraft_GetNumHairVarByRRG(string sRaceGender)
{
	return NWNXGetInt( "CraftEnhanced", "NumHairVarByRRG", sRaceGender, 0);
}

int XPEnCraft_GetNumHeadVarByRRG(string sRaceGender)
{
	return NWNXGetInt( "CraftEnhanced", "NumHeadVarByRRG", sRaceGender, 0);
}

int XPEnCraft_GetHairdByRRGIdx(string sRaceGender, int idx)
{
	return NWNXGetInt( "CraftEnhanced", "IndexOfHairByRRG", sRaceGender, idx);
}

int XPEnCraft_GetHeadByRRGIdx(string sRaceGender, int idx)
{
	return NWNXGetInt( "CraftEnhanced", "IndexOfHeadByRRG", sRaceGender, idx);
}

int XPEnCraft_GetNumVATByRRGPart(string sRaceGender, int iPart)
{
	return NWNXGetInt( "CraftEnhanced", "NumOfVATByRRGPart", sRaceGender, iPart);
}

int XPEnCraft_GetNumVARByRRGPartVAT(string sRaceGender, int iPart, string sVat)
{
	return NWNXGetInt( "CraftEnhanced", "NumOfVATByRRGPart", sRaceGender+sVat, iPart);
}

string XPEnCraft_GetVatByRRGPartIdx(string sRaceGender, int iPart, int idx)
{
	return NWNXGetString( "CraftEnhanced", "VatByRRGPartIdx", sRaceGender, iPart*1000+idx);
}

string XPEnCraft_GetVaRByRRGPartVatIdx(string sRaceGender, int iPart, string sVat, int idx)
{
	return NWNXGetString( "CraftEnhanced", "VarByRRGPartVatIdx", sRaceGender+sVat, iPart*1000+idx);
}

string XPEnCraft_GetRealToVisualCat(string sRaceGender, int iPart, int iCat, int iIdx)
{
	return NWNXGetString( "CraftEnhanced", "RealToVisualCat", sRaceGender+IntToString(iCat), iPart*1000+iIdx);
}

int XPEnCraft_GetNumOfPartByRRGPart(string sRaceGender, string sPart)
{
	return NWNXGetInt( "CraftEnhanced", "NumberOfPartByRRGPart", sRaceGender+sPart, 0);
}

int XPEnCraft_GetPartByRRGPartIdx(string sRaceGender, string sPart, int iIdx)
{
	return NWNXGetInt( "CraftEnhanced", "PartByRRGPartIdx", sRaceGender+sPart, iIdx);
}

int XPEnCraft_GetNumOfWeapSPbyW(string sWeaponType)
{
	return NWNXGetInt( "CraftEnhanced", "NumbWeaponSubPbyW", sWeaponType, 0);
}

int XPEnCraft_GetNumOfWPbyWSP(string sWeaponType, string sPart)
{
	return NWNXGetInt( "CraftEnhanced", "NumOfWPartbyWSP", sWeaponType+"#"+sPart, 0);
}

int XPEnCraft_GetWPartByWSPIdx(string sWeaponType, string sPart, int iIdx)
{
	return NWNXGetInt( "CraftEnhanced", "WPartByWSPIdx", sWeaponType+"#"+sPart, iIdx);
}

void XPEnCraft_CreateNewCategory(string sRRG, int iPart, string sNewCat)
{
	NWNXSetString("CraftEnhanced", "CreateNewCat", sRRG, iPart, sNewCat);
}

void XPEnCraft_DeleteCategory(string sRRG, int iPart, string sCatToDel)
{
	NWNXSetString("CraftEnhanced", "DeleteCat", sRRG, iPart, sCatToDel);
}

void XPEnCraft_RenameCategory(string sRRG, int iPart, string sOldNameCategory, string sNewName)
{
	NWNXSetString("CraftEnhanced", "RenameCat", sRRG+sOldNameCategory, iPart, sNewName);
}

void XPEnCraft_ChangeCategory(string sRRG, int iPart, string sCategory, string sApp, string sNewCategory)
{
	NWNXSetString("CraftEnhanced", "ChangeCat", sRRG+sCategory, iPart, sApp+"#"+sNewCategory);
}

int XPEnCraft_SaveFile()
{
	return NWNXGetInt("CraftEnhanced", "SaveFile", "", 0);
}

/*****************************************************************************/
//Color Management (conversion functions)

struct strTint
{
	int iRed;
	int iGreen;
	int iBlue;
	int iAlpha;
};

struct strHLS 
{
	int iHue;
	int iLum;
	int iSat;
};

//returns an integer directly useable by the plugin
int XPEnCraft_RGBAToInt(int iRed, int iGreen, int iBlue, int iAlpha = 255);

//returns a strTint structure
// given Hue, Lum and Sat (got with the color UI)
struct strTint XPEnCraft_HLSToTintStruct(int iHue, int iLum, int iSat);

//part of the conversion from HLS to RGB
float XPEnCraft_QqhToRgb(float q1, float q2, float Hue);

/*****************************************************************************/
//Debuggin function
void XPEnCraft_Debug(object oPC, string sStringToDebug);


/*****************************************************************************/
//Dated Fuctions Kept for compatibility

/************************************/
/* Plugin management              */
/************************************/


//get the element index inthe datafile given it's roadmap
string XPEnCraft_GetIndex(string sRoadMap,string sDatafileName="");

//get the value of any element given it's road map or it's index 
string XPEnCraft_GetValue(string sRoadMap="", int iElementIndex = 0, string sDatafileName="");

//Sets a new value for an element given it's roadmap or index
void XPEnCraft_SetValue(string sNewValue, string sRoadMap="", int iElementIndex = 0, string sDatafileName="");

//Sets a new color value for an item given it's red roadmap or index
//the roadmap to the "red value" of a Tint is usually of the sheme "Tintable|Tint|1|r" for tint 1, "Tintable|Tint|2|r" for tint 2
//however tis path may vary under some circompstances (build-in equipements ie HasBoot, HasGloves etc...) 
void XPEnCraft_SetColor(string sNewColorValue, string sRedRoadMap="", int iRedIndex = 0, string sDatafileName="");

//get the Color of any item given it's red road map or it's index 
//the roadmap to the "red value" of a Tint is usually of the sheme "Tintable|Tint|1|r" for tint 1, "Tintable|Tint|2|r" for tint 2
//however tis path may vary under some circompstances (build-in equipements ie HasBoot, HasGloves etc...) 
string XPEnCraft_GetColor(string sRedRoadMap="", int iRedIndex = 0, string sDatafileName="");

//forces the plugin to read the datafile instead of keeping it into memory
//any script function should end up with a call to that function
//anyway, within a function, you may not use it after each call to the plugin, 
//thus enabling to do multiple operations without reloading the file into memory each time.
//(helps saving performances)
//be aware of one last thing about the clear memory :
//if you decide to do multiple things in one function without calls to a clearmemory, 
//the datafile will be changed but the value you'll get with a "get" won't have changed till you clear mem
void XPEnCraft_ClearMemory();


///////////////////////////////////////////////////////////////////////////////
////////////////////////////// IMPLEMENTATIONS ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/*****************************************************************************/
//Bioware Database related functions

//creates and return the name of the datafile for this PC
string XPEnCraft_GetDatafileName(object oPC)
{
	return XPEnCraft_DATABASE_PREFIX + XPEnCraft_GetPCID(oPC);
}

//destroy the given database
//arguments may be a PC or a Datafile Name
void XPEnCraft_DestroyDataFile(object oPC, string sDatafileName="")
{
	if(sDatafileName=="")
	{
		sDatafileName= XPEnCraft_GetDatafileName(oPC);
	}
	DestroyCampaignDatabase(sDatafileName);
}

//store the given object into a Bioware database file
//returns TRUE or FALSE whether the operation was successful or not
int XPEnCraft_StoreItemToCraft(object oPC, object oItem, int bSafeStore = TRUE)
{
	int bStorageSuccess = FALSE;
	
	if((oPC!=OBJECT_INVALID) && (oItem != OBJECT_INVALID))
	{
		string sDataFileName = XPEnCraft_GetDatafileName(oPC);
		if(bSafeStore)
		{
			XPEnCraft_DestroyDataFile(oPC,sDataFileName); 
		}
		bStorageSuccess = StoreCampaignObject(sDataFileName,"NO_ACCOUNT_NEEDED",oItem);
	}
	
	if(!bStorageSuccess)
	{
		XPEnCraft_Debug(oPC,"Unable to store " + GetName(oItem) + " as a campaign object.");
	}
	return bStorageSuccess;
}


object XPEnCraft_RetrieveCraftedItem(object oPC, location lSpawnLocation)
{
	string sDatafileName = XPEnCraft_GetDatafileName(oPC);
	
	//retrieve item from the database
	object oCampaignObject = RetrieveCampaignObject(sDatafileName,"NO_ACCOUNT_NEEDED",lSpawnLocation);

	if(oCampaignObject==OBJECT_INVALID)
	{
		XPEnCraft_Debug(oPC,"Unable to retrieve object from database.");	
	}

	return oCampaignObject;
}


/*******************************************************************************/
//Color management

int XPEnCraft_RGBAToInt(int iRed, int iGreen, int iBlue, int iAlpha = 255)
{//if outside boudaries values are given as parameters, they will be truncated.
// if you don't want that to happen, just call the function with proper arguments

//the quickest way to ensure that the value will be between 0 and 255 is to truncate to the 8 first bits
// using a "& 255"
//the operator << is used to move the bits to the left 
	return ((iRed & 255)<<24) + ((iGreen & 255)<<16) + ((iBlue & 255)<<8) + (iAlpha & 255);
}

struct strHLS XPEnCraft_RGBToHLS(int iRed, int iGreen, int iBlue)
{
	float fRp = IntToFloat(iRed)/255.0;
	float fGp = IntToFloat(iGreen)/255.0;
	float fBp = IntToFloat(iBlue)/255.0;
	
	float cMax = fRp;
	if(fGp > cMax) { cMax = fGp; }
	if(fBp > cMax) { cMax = fBp; }
	
	float cMin = fRp;
	if(fGp < cMin) { cMin = fGp; }
	if(fBp < cMin) { cMin = fBp; }
	
	float fDelta = cMax - cMin;
	
	float fLum = ((cMax + cMin)/2.0);
	float fHue;
	if(fDelta == 0.0) {
		fHue = 0.0;
	} else if(cMax == fRp)	{
		float fTemp = fGp - fBp;
		fTemp = fTemp / fDelta;
		while(fTemp >= 6.0)
		{
			fTemp -= 6.0;
		}
		fHue = (fTemp * 60.0);			
	} else if(cMax == fGp) {
		fHue = (((fBp - fRp) / fDelta)+2.0)*60.0;
	} else if(cMax == fBp) {
		fHue = (((fRp - fGp) / fDelta)+4.0)*60.0;
	}
	
	float fSaturation = 0.0;
	if(fDelta != 0.0)
	{
		fSaturation = (fDelta/(1-fabs(2*fLum -1)));	
	}
	
	struct strHLS strMyHLS;
	//Add 0.5 in order to round (FloatToInt incorrectly truncate)
	strMyHLS.iHue = FloatToInt(fHue+0.5);
	strMyHLS.iLum = FloatToInt((fLum*100.0)+0.5);
	strMyHLS.iSat = FloatToInt((fSaturation*100.0)+0.5);
	
	return strMyHLS;
}

//Hue is between 0 and 360, lum and sat are between 0 and 100.
struct strTint XPEnCraft_HLSToTintStruct(int iHue, int iLum, int iSat)
{
	struct strTint strMyTint;
	
	float T,L,S;
	T = IntToFloat(iHue);
	L = IntToFloat(iLum) / 100.0;
	S = IntToFloat(iSat) / 100.0;
	
	float R,G,B, q1,q2, C,X,m;
	
	C = (1.0 - fabs((2.0*L)-1.0))*S;
	
	q1 = T/60.0;
	
	while(q1 >= 2.0)
	{
		q1 = q1 - 2.0;	
	}
	
	X = C * (1.0- fabs(q1-1.0));
	
	m = L - (C/2.0);
	
	if(iHue < 60)
	{
		R = C;
		G = X;
		B = 0.0;	
	}
	else if(iHue < 120)
	{
		R = X;
		G = C;
		B = 0.0;
	}
	else if(iHue < 180)
	{
		R = 0.0;
		G = C;
		B = X;
	}
	else if(iHue < 240)
	{
		R = 0.0;
		G = X;
		B = C;
	}
	else if(iHue < 300)
	{
		R = X;
		G = 0.0;
		B = C;
	}
	else if(iHue < 360)
	{
		R = C;
		G = 0.0;
		B = X;
	}
	
	R = (R+m);
	G = (G+m);
	B = (B+m);

	strMyTint.iRed = FloatToInt((R*255.0)+0.5);
	strMyTint.iGreen = FloatToInt((G*255.0)+0.5);
	strMyTint.iBlue = FloatToInt((B*255.0)+0.5);
	strMyTint.iAlpha = 255;
	
	return strMyTint;
}

float XPEnCraft_QqhToRgb(float q1, float q2, float Hue)//Hue = T
{
	if(Hue > 360.0) 		Hue -= 360.0;
	else if(Hue < 0.0) 		Hue += 360.0;
	
	if(Hue <  60.0) 		return (q1 + ((q2 - q1) * (Hue/60.0)));
	else if(Hue < 180.0) 	return (q2);
	else if(Hue < 240.0)	return (q1 + ((q2 - q1) * ((240-Hue)/60.0)));
	else 					return (q1);
}

/*****************************************************************************/
//Debuggin functions

void XPEnCraft_Debug(object oPC, string sStringToDebug)
{
	WriteTimestampedLogEntry(GetName(oPC) + " : " + sStringToDebug);
	SendMessageToPC(oPC,"XPEnhancedCraft : <color=steelblue>" + sStringToDebug + "</color>");
}


/************************************/
/* Plugin management                */
/************************************/


string XPEnCraft_GetIndex(string sRoadMap,string sDatafileName="")
{
	return NWNXGetString("CraftEnhanced", "GetIndex|"+ sDatafileName, sRoadMap,0);
} 

string XPEnCraft_GetValue(string sRoadMap="", int iElementIndex = 0, string sDatafileName="")
{
	return NWNXGetString("CraftEnhanced", "GetValue|"+ sDatafileName, sRoadMap, iElementIndex);
}

void XPEnCraft_SetValue(string sNewValue, string sRoadMap="", int iElementIndex = 0, string sDatafileName="")
{
	NWNXSetString("CraftEnhanced", "SetValue|"+ sDatafileName, sRoadMap, iElementIndex, sNewValue);
}

void XPEnCraft_SetColor(string sNewColorValue, string sRedRoadMap="", int iRedIndex = 0, string sDatafileName="")
{
	NWNXSetString("CraftEnhanced", "SetColor|"+ sDatafileName, sRedRoadMap, iRedIndex, sNewColorValue);
}

string XPEnCraft_GetColor(string sRedRoadMap="", int iRedIndex = 0, string sDatafileName="")
{
	return NWNXGetString("CraftEnhanced", "GetColor|"+ sDatafileName, sRedRoadMap, iRedIndex);
}
void XPEnCraft_ClearMemory()
{
	NWNXSetString("CraftEnhanced","CLEAR_MEMORY","",0,"");
}



												
void XPEnCraft_CraftAndGive(object oPC, object oCible, int iSlot, object obj)
{
	object oCraftedItem =  XPEnCraft_RetrieveCraftedItem(oPC, GetLocation(oCible));
	if(oCraftedItem!=OBJECT_INVALID)
	{
		DestroyObject(obj);
		obj = CopyItem(oCraftedItem,oCible,TRUE);
		AssignCommand(oCible,ActionEquipItem(obj, iSlot));
		DestroyObject(oCraftedItem);	
	}
}

void XPEnCraft_ActionChangeAppareance(object oPC, object oItem, int iNewAVT, int iNewVar)
{
	string sDatafileName = XPEnCraft_GetDatafileName(oPC);

	int iArmorVisualType_ElementIndex = GetLocalInt(oPC,"XC_AVT_ELEMENT_ID");
	
	if(iArmorVisualType_ElementIndex!=0)
	{
		XPEnCraft_SetValue(IntToString(iNewAVT), "", iArmorVisualType_ElementIndex, sDatafileName);
	}
	else
	{		
		XPEnCraft_SetValue(IntToString(iNewAVT), "ArmorVisualType", 0, sDatafileName);		
	}

	int iVariation_ElementIndex = GetLocalInt(oPC,"XC_VARIATION_ELEMENT_ID");

	if(iVariation_ElementIndex!=0)
	{
		XPEnCraft_SetValue(IntToString(iNewVar-XP_CRAFT_VARIATION_LISTS_BASE), "", iVariation_ElementIndex, sDatafileName);
	}
	else
	{		
		XPEnCraft_SetValue(IntToString(iNewVar-XP_CRAFT_VARIATION_LISTS_BASE), "Variation", 0, sDatafileName);		
	}

	XPEnCraft_ClearMemory();
}

void XPEnCraft_ActionCompleteCraft(object oPC, object oItemToCraft, int iNewAVT, int iNewVar, string sColor1, string sColor2, string sColor3, int ajout = FALSE)
{

	string sDatafileName = XPEnCraft_GetDatafileName(oPC);

	int iArmorVisualType_ElementIndex = GetLocalInt(oPC,"XC_AVT_ELEMENT_ID");
	
	if(iArmorVisualType_ElementIndex!=0)
	{
		XPEnCraft_SetValue(IntToString(iNewAVT), "", iArmorVisualType_ElementIndex, sDatafileName);
	}
	else
	{		
		XPEnCraft_SetValue(IntToString(iNewAVT), "ArmorVisualType", 0, sDatafileName);		
	}

	int iVariation_ElementIndex = GetLocalInt(oPC,"XC_VARIATION_ELEMENT_ID");
	if(iVariation_ElementIndex!=0)
	{
		XPEnCraft_SetValue(IntToString(iNewVar-XP_CRAFT_VARIATION_LISTS_BASE), "", iVariation_ElementIndex, sDatafileName);
	}
	else
	{		
		XPEnCraft_SetValue(IntToString(iNewVar-XP_CRAFT_VARIATION_LISTS_BASE), "Variation", 0, sDatafileName);		
	}
	
	XPEnCraft_SetColor(sColor1, "Tintable|Tint|1|r", 0, sDatafileName);
	XPEnCraft_SetColor(sColor2, "Tintable|Tint|2|r", 0, sDatafileName);
	XPEnCraft_SetColor(sColor3, "Tintable|Tint|3|r", 0, sDatafileName);

	XPEnCraft_ClearMemory();
}

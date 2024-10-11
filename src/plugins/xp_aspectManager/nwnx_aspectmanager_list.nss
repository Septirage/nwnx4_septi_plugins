//////////////////////////////////////////////////////////////////////////////////////////////
// nwnx_aspectmanager_list - appearance list functions of the AspectManager plugin
// Original Scripter:  Septirage
//--------------------------------------------------------------------------------------------
// Last Modified By:	Septirage           2024-02-28
//--------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////
#include "nwnx_aspectmanager"


/****************************** HairVariation *******************************/

//Return the number of Hair variation for the selected RRG code
// sRRG: 3 char code, RaceRaceGender : eg HHM
int XPAspMan_GetNumberOfHairVariation(string sRRG);

//Return the Hair Variation Int 
//sRRG: sRRG: 3 char code, RaceRaceGender : eg HHM
//iIdx: Internal List Index
int XPAspMan_GetHairVariation(string sRRG, int iIdx);


/****************************** HeadVariation *******************************/

//Return the number of Head variation for the selected RRG code
// sRRG: 3 char code, RaceRaceGender : eg HHM
int XPAspMan_GetNumberOfHeadVariation(string sRRG);

//Return the Head Variation Int 
//sRRG: sRRG: 3 char code, RaceRaceGender : eg HHM
//iIdx: Internal List Index
int XPAspMan_GetHeadVariation(string sRRG, int iIdx);




/******************************** VisualType ********************************/

//Restrict or unrestrict sVisualType
void XPAspMan_SetRestrictedVisualType(string sVisualType, int bRestricted);

//Is sVisualType restricted ?
int XPAspMan_GetRestrictedVisualType(string sVisualType);

//Get the Number of existing VisualType for this specific sRRG-ModelPart
//sRRG: 3 char code, RaceRaceGender : eg HHM
//iModelPart: ARMOR_MODEL_PART_* constant
int XPAspMan_GetNumberOfVisualType(string sRRG, int iModelPart);

//Get the specific VisualType
//sRRG: 3 char code, RaceRaceGender : eg HHM
//iModelPart: ARMOR_MODEL_PART_* constant
//iIdx: Internal List Index
string XPAspMan_GetVisualType(string sRRG, int iModelPart, int iIdx);



//Get the Number of existing VisualType for the choosen Parameters.
// The difference from XPAspMan_GetNumberOfVisualType, is that it allow "all"/"joker" choice.
//sRR : the racial code : eg human is HH
//int iGender, a GENDER_** cst. If it's neither GENDER_MALE nor GENDER_FEMALE it will be considered as "all"
//int iModelPart: ARMOR_MODEL_PART_* constant. You can use "-1" for "All"
int XPAspMan_GetNumberExtendedVisualType(string sRR, int iGender, int iModelPart);


//Get the specific VisualType
// The difference from XPAspMan_GetVisualType, is that it allow "all"/"joker" choice.
//sRR: the racial code : eg human is HH
//iGender, a GENDER_** cst. If it's neither GENDER_MALE nor GENDER_FEMALE it will be considered as "all"
//iModelPart: ARMOR_MODEL_PART_* constant. You can use "-1" for "All"
//iIdx: Internal List Index
string XPAspMan_GetExtendedVisualType(string sRR, int iGender, int iModelPart, int iIdx);




/*************************** ModelPart Variation ****************************/

//Return Number of Variation in the choosen VisualType
//sRRG: 3 char code, RaceRaceGender : eg HHM
//iModelPart: ARMOR_MODEL_PART_* constant
int XPAspMan_GetNumberOfVariation(string sRRG, int iModelPart, string sVisualType);


//Get the unique Variation Name
//sRRG: 3 char code, RaceRaceGender : eg HHM
//iModelPart: ARMOR_MODEL_PART_* constant
//iIdx: Internal List Index
string XPAspMan_GetVariation(string sRRG, int iModelPart, string sVisualType, int iIdx);

//Get the Number of VisualType for this specific Appearance
//sRRG: 3 char code, RaceRaceGender : eg HHM
//iModelPart: ARMOR_MODEL_PART_* constant
int XPAspMan_GetRealAppNumberOfVisualType(string sRRG, int iModelPart, int iCategory, int iVariation);

//Return the specified VisualType for this specific Appearance
//sRRG: 3 char code, RaceRaceGender : eg HHM
//iModelPart: ARMOR_MODEL_PART_* constant
//iIdx: if this Appearance have several VisualType, allow you to choose which one get
string XPAspMan_GetVisualTypeFromRealApp(string sRRG, int iModelPart, int iCategory, int iVariation, int iIdx = 0); //RealToVisualCat


/******************************** ModelPiece ********************************/

//Number of Variation for this ModelPiece
//sRRG: 3 char code, RaceRaceGender : eg HHM
//iModelPiece: ARMOR_MODEL_PIECE_* constant
int XPAspMan_GetNumberOfModelPieceVariation(string sRRG, int iModelPiece);

//Get the Real ModelPiece Appearance Variation
//sRRG: 3 char code, RaceRaceGender : eg HHM
//iModelPiece: ARMOR_MODEL_PIECE_* constant
//iIdx: Internal List Index
int XPAspMan_GetModelPieceVariation(string sRRG, int iModelPiece, int iIdx);




/********************************** Weapon **********************************/


int XPAspMan_GetNumberOfWeaponPart(string sWeaponType);

int XPAspMan_GetNumberOfWeaponPartVariation(string sWeaponType, int iPart);

int XPAspMan_GetWeaponPartVariation(string sWeaponType, int iPart, int iIdx);



/****************************** ListManagement ******************************/

//Remove the choosen appearance from sVisualType. 
// Will do nothing if sVisualType is the only Visual for this appearance
//sRRG: 3 char code, RaceRaceGender : eg HHM
//iModelPiece: ARMOR_MODEL_PIECE_* constant
void XPAspMan_RemoveFromVisualType(string sRRG, int iModelPart, int iCategory, int iVariation, string sVisualType); 

//Add the choosen appearance in sVisualType
//sRRG: 3 char code, RaceRaceGender : eg HHM
//iModelPiece: ARMOR_MODEL_PIECE_* constant
void XPAspMan_AddToVisualType(string sRRG, int iModelPart, int iCategory, int iVariation, string sVisualType);
 
//Delete the selected sVisualType
//Will do nothing if, in this VisualType, it exists some Apperance that don't have any other VisualType
//sRRG: 3 char code, RaceRaceGender : eg HHM
//iModelPiece: ARMOR_MODEL_PIECE_* constant
void XPAspMan_DeleteVisualType(string sRRG, int iModelPart, string sVisualType);

//Delete the selected sVisualTypeToDelete (extended)
//Will do nothing if, in this VisualType, it exists some Apperance that don't have any other VisualType
//The difference with XPAspMan_DeleteVisualType is that it allow "all"/"joker" choice.
//sRR: the racial code : eg human is HH  ("" for all)
//iGender, a GENDER_** cst. If it's neither GENDER_MALE nor GENDER_FEMALE it will be considered as "all"
//iModelPart: ARMOR_MODEL_PART_* constant. (You can use -1 for "All")
void XPAspMan_DeleteExtendedVisualType(string sRR, int iGender, int iModelPart, string sVisualTypeToDelete);

//Copy the content of the choosen sOriginVisualType to sDestinationVisualType
// You can use default / all value
//sRR: the racial code : eg human is HH  ("" for all)
//iGender, a GENDER_** cst. If it's neither GENDER_MALE nor GENDER_FEMALE it will be considered as "all"
//iModelPart: ARMOR_MODEL_PART_* constant. (You can use -1 for "All")
void XPAspMan_CopyExtendedVisualType(string sRR, int iGender, int iModelPart, string sOriginVisualType, string sDestinationVisualType);

int XPAspMan_SaveFile();

int XPAspMan_LoadFile();

void XPAspMan_ChangeVisualType(string sRRG, int iModelPart, string sVisualType, string sApp, string sNewVisualType);

void XPAspMan_CreateNewVisualType(string sRRG, int iModelPart, string sVisualType);

void XPAspMan_RenameVisualType(string sRRG, int iModelPart, string sOldName, string sNewName);


/*****************************************************************************/

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////// IMPLEMENTATIONS ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/*****************************************************************************/



/******************************* HairVariation ******************************/

int XPAspMan_GetNumberOfHairVariation(string sRRG)
{
	return NWNXGetInt( "AspectManager", "listSystemNumHairVarByRRG", sRRG, 0);
}

int XPAspMan_GetHairVariation(string sRRG, int iIdx)
{
	return NWNXGetInt( "AspectManager", "listSystemIndexOfHairByRRG", sRRG, iIdx);
}


/****************************** HeadVariation *******************************/

int XPAspMan_GetNumberOfHeadVariation(string sRRG)
{
	return NWNXGetInt( "AspectManager", "listSystemNumHeadVarByRRG", sRRG, 0);
}

int XPAspMan_GetHeadVariation(string sRRG, int iIdx)
{
	return NWNXGetInt( "AspectManager", "listSystemIndexOfHeadByRRG", sRRG, iIdx);
}



/******************************** VisualType ********************************/


void XPAspMan_SetRestrictedVisualType(string sVisualType, int bRestricted)
{
	NWNXGetInt("AspectManager", "listSystemSetRestrictedCategory", sVisualType, bRestricted);
}

int XPAspMan_GetRestrictedVisualType(string sVisualType)
{
	return NWNXGetInt("AspectManager", "listSystemGetRestrictedCategory", sVisualType, 0);
}

int XPAspMan_GetNumberOfVisualType(string sRRG, int iModelPart)
{
	return NWNXGetInt( "AspectManager", "listSystemNumOfVATByRRGPart", sRRG, iModelPart);
}

string XPAspMan_GetVisualType(string sRRG, int iModelPart, int iIdx)
{
	return NWNXGetString( "AspectManager", "listSystemVatByRRGPartIdx", sRRG, iModelPart*1000+iIdx);
}


int XPAspMan_GetNumberExtendedVisualType(string sRR, int iGender, int iModelPart)
{
	string sParam = sRR  + "#";
	if(iGender == GENDER_MALE)
		sParam += "M";
	else if(iGender == GENDER_FEMALE)
		sParam += "F";
	
	return NWNXGetInt("AspectManager", "listSystemExtendedNumberOfVisualCat", sParam, iModelPart);
}

string XPAspMan_GetExtendedVisualType(string sRR, int iGender, int iModelPart, int iIdx)
{
	string sParam = sRR + "#";
	if(iGender == GENDER_MALE)
		sParam += "M";
	else if(iGender == GENDER_FEMALE)
		sParam += "F";

	if(iModelPart > 9)
		return "";
	if(iModelPart < 0)
		iModelPart = 9;
	
	int iParam = iIdx*10 + iModelPart;
	
	return NWNXGetString("AspectManager", "listSystemExtendedGetVisualCategory", sParam, iParam);
}

/*************************** ModelPart Variation ****************************/
 
int XPAspMan_GetNumberOfVariation(string sRRG, int iModelPart, string sVisualType)
{
	return NWNXGetInt( "AspectManager", "listSystemNumOfVARByRRGPartVAT", sRRG+sVisualType, iModelPart);
}

string XPAspMan_GetVariation(string sRRG, int iModelPart, string sVisualType, int iIdx)
{
	return NWNXGetString( "AspectManager", "listSystemVarByRRGPartVatIdx", sRRG+sVisualType, iModelPart*1000+iIdx);
}

int XPAspMan_GetRealAppNumberOfVisualType(string sRRG, int iModelPart, int iCategory, int iVariation)
{
	return NWNXGetInt( "AspectManager", "listSystemNumberOfVisualCat", sRRG+IntToString(iCategory), iModelPart*1000+iVariation);
}

string XPAspMan_GetVisualTypeFromRealApp(string sRRG, int iModelPart, int iCategory, int iVariation, int iIdx)
{
	return NWNXGetString( "AspectManager", "listSystemRealToVisualCat", sRRG+IntToString(iCategory), (iIdx*1000+iModelPart)*1000+iVariation);
}


/******************************** ModelPiece ********************************/

int XPAspMan_GetNumberOfModelPieceVariation(string sRRG, int iModelPiece)
{
	return NWNXGetInt( "AspectManager", "listSystemNumberOfPartByRRGPart", sRRG, iModelPiece);
}

int XPAspMan_GetModelPieceVariation(string sRRG, int iModelPiece, int iIdx)
{
	return NWNXGetInt( "AspectManager", "listSystemPartByRRGPartIdx", sRRG, (iIdx*100)+iModelPiece);
}

/********************************** Weapon **********************************/

int XPAspMan_GetNumberOfWeaponPart(string sWeaponType)
{
	return NWNXGetInt( "AspectManager", "listSystemNumbWeaponSubPbyW", sWeaponType, 0);
}

int XPAspMan_GetNumberOfWeaponPartVariation(string sWeaponType, int iPart)
{
	return NWNXGetInt( "AspectManager", "listSystemNumOfWPartbyWSP", sWeaponType, iPart);
}

int XPAspMan_GetWeaponPartVariation(string sWeaponType, int iPart, int iIdx)
{
	if(iPart >= 0 && iPart < 3)
		return NWNXGetInt( "AspectManager", "listSystemWPartByWSPIdx", sWeaponType, (iIdx*10)+iPart);
		
	return -1;
}


/****************************** ListManagement ******************************/

void XPAspMan_RemoveFromVisualType(string sRRG, int iModelPart, int iCategory, int iVariation, string sVisualType)
{
	NWNXSetString("AspectManager", "listSystemRemoveFromCategory", sRRG+IntToString(iCategory), iModelPart*1000+iVariation, sVisualType);
}

void XPAspMan_AddToVisualType(string sRRG, int iModelPart, int iCategory, int iVariation, string sVisualType)
{
	NWNXSetString("AspectManager", "listSystemAddToCategory", sRRG+IntToString(iCategory), iModelPart*1000+iVariation, sVisualType);
}

void XPAspMan_CreateNewVisualType(string sRRG, int iModelPart, string sVisualType)
{
	NWNXSetString("AspectManager", "listSystemCreateNewCat", sRRG, iModelPart, sVisualType);
}

void XPAspMan_DeleteVisualType(string sRRG, int iModelPart, string sVisualType)
{
	NWNXSetString("AspectManager", "listSystemDeleteCat", sRRG, iModelPart, sVisualType);
}

void XPAspMan_RenameVisualType(string sRRG, int iModelPart, string sOldName, string sNewName)
{
	NWNXSetString("AspectManager", "listSystemRenameCat", sRRG+sOldName, iModelPart, sNewName);
}

void XPAspMan_ChangeVisualType(string sRRG, int iModelPart, string sVisualType, string sApp, string sNewVisualType)
{
	NWNXSetString("AspectManager", "listSystemChangeCat", sRRG+sVisualType, iModelPart, sApp+"#"+sNewVisualType);
}

int XPAspMan_SaveFile()
{
	return NWNXGetInt("AspectManager", "listSystemSaveFile", "", 0);
}

int XPAspMan_LoadFile()
{
	return NWNXGetInt("AspectManager", "listSystemLoadFile", "", 0);
}

void XPAspMan_DeleteExtendedVisualType(string sRR, int iGender, int iModelPart, string sVisualTypeToDelete)
{
	string sParam = sRR + "#";
	if(iGender == GENDER_MALE)
		sParam += "M";
	else if(iGender == GENDER_FEMALE)
		sParam += "F";

	
	NWNXSetString("AspectManager", "listSystemExtendedDeleteCat", sParam, iModelPart, sVisualTypeToDelete);
}

void XPAspMan_CopyExtendedVisualType(string sRR, int iGender, int iModelPart, string sOriginVisualType, string sDestinationVisualType)
{
	string sParam = sRR + "#";
	if(iGender == GENDER_MALE)
		sParam += "M";
	else if(iGender == GENDER_FEMALE)
		sParam += "F";

	sParam += "#";
	sParam += sOriginVisualType;

	NWNXSetString("AspectManager", "listSystemExtendedCopyCat", sParam, iModelPart, sDestinationVisualType);
}
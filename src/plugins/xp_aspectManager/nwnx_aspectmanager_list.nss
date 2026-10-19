//////////////////////////////////////////////////////////////////////////////////////////////
// nwnx_aspectmanager_list - appearance list functions of the AspectManager plugin
// Original Scripter:  Septirage
//--------------------------------------------------------------------------------------------
// Last Modified By:	Septirage			2026-10-18	Fix the description of the sRR parameter
//						Septirage			2026-09-06	Fix XPAspMan_SetRestrictedVisualType function
//						Septirage			2025/07/08	Update to AspectManager v1.5
//						Septirage           2024-02-28
//--------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////
#include "nwnx_aspectmanager"


/****************************** HairVariation *******************************/

//Return the number of Hair variation for the selected RRG code
// sRRG: race-gender code from appearance.2DA RaceRaceGender : eg P_HHM
int XPAspMan_GetNumberOfHairVariation(string sRRG);

//Return the Hair Variation Int 
//sRRG: race-gender code from appearance.2DA RaceRaceGender : eg P_HHM
//iIdx: Internal List Index
int XPAspMan_GetHairVariation(string sRRG, int iIdx);


/****************************** HeadVariation *******************************/

//Return the number of Head variation for the selected RRG code
// sRRG: race-gender code from appearance.2DA RaceRaceGender : eg P_HHM
int XPAspMan_GetNumberOfHeadVariation(string sRRG);

//Return the Head Variation Int 
//sRRG: race-gender code from appearance.2DA RaceRaceGender : eg P_HHM
//iIdx: Internal List Index
int XPAspMan_GetHeadVariation(string sRRG, int iIdx);




/******************************** VisualType ********************************/

//Restrict or unrestrict sVisualType. Only a flag for your scripts (the plugin doesn't filter anything), for every visual type with this name
void XPAspMan_SetRestrictedVisualType(string sVisualType, int bRestricted);

//Is sVisualType restricted ?
int XPAspMan_GetRestrictedVisualType(string sVisualType);

//Get the Number of existing VisualType for this specific sRRG-ModelPart
//sRRG: race-gender code from appearance.2DA RaceRaceGender : eg P_HHM
//iModelPart: ARMOR_MODEL_PART_* constant
int XPAspMan_GetNumberOfVisualType(string sRRG, int iModelPart);

//Get the specific VisualType
//sRRG: race-gender code from appearance.2DA RaceRaceGender : eg P_HHM
//iModelPart: ARMOR_MODEL_PART_* constant
//iIdx: Internal List Index
string XPAspMan_GetVisualType(string sRRG, int iModelPart, int iIdx);



//Get the Number of existing VisualType for the choosen Parameters.
// The difference from XPAspMan_GetNumberOfVisualType, is that it allow "all"/"joker" choice.
//sRR: the beginning of the RRG code (as in appearance.2DA RaceRaceGender, eg P_HHM): eg "P_HH" for human, "C_AKACHI". "" for all
//iGender: a GENDER_* cst (it tests the last letter of the RRG code). If it's neither GENDER_MALE nor GENDER_FEMALE it will be considered as "all"
//int iModelPart: ARMOR_MODEL_PART_* constant. You can use "-1" for "All"
int XPAspMan_GetNumberExtendedVisualType(string sRR, int iGender, int iModelPart);


//Get the specific VisualType
// The difference from XPAspMan_GetVisualType, is that it allow "all"/"joker" choice.
//sRR: the beginning of the RRG code (as in appearance.2DA RaceRaceGender, eg P_HHM): eg "P_HH" for human, "C_AKACHI". "" for all
//iGender: a GENDER_* cst (it tests the last letter of the RRG code). If it's neither GENDER_MALE nor GENDER_FEMALE it will be considered as "all"
//iModelPart: ARMOR_MODEL_PART_* constant. You can use "-1" for "All"
//iIdx: Internal List Index
string XPAspMan_GetExtendedVisualType(string sRR, int iGender, int iModelPart, int iIdx);




/*************************** ModelPart Variation ****************************/

//Return Number of Variation in the choosen VisualType
//sRRG: race-gender code from appearance.2DA RaceRaceGender : eg P_HHM
//iModelPart: ARMOR_MODEL_PART_* constant
int XPAspMan_GetNumberOfVariation(string sRRG, int iModelPart, string sVisualType);


//Get the unique Variation Name
//sRRG: race-gender code from appearance.2DA RaceRaceGender : eg P_HHM
//iModelPart: ARMOR_MODEL_PART_* constant
//iIdx: Internal List Index
string XPAspMan_GetVariation(string sRRG, int iModelPart, string sVisualType, int iIdx);

//Get the Number of VisualType for this specific Appearance
//sRRG: race-gender code from appearance.2DA RaceRaceGender : eg P_HHM
//iModelPart: ARMOR_MODEL_PART_* constant
int XPAspMan_GetRealAppNumberOfVisualType(string sRRG, int iModelPart, int iCategory, int iVariation);

//Return the specified VisualType for this specific Appearance
//sRRG: race-gender code from appearance.2DA RaceRaceGender : eg P_HHM
//iModelPart: ARMOR_MODEL_PART_* constant
//iIdx: if this Appearance have several VisualType, allow you to choose which one get
string XPAspMan_GetVisualTypeFromRealApp(string sRRG, int iModelPart, int iCategory, int iVariation, int iIdx = 0); //RealToVisualCat


/******************************** ModelPiece ********************************/

//Number of Variation for this ModelPiece
//sRRG: race-gender code of the accessory models, between A_ and the piece name: eg HHM for A_HHM_LShoulder01 (no P_)
//iModelPiece: ARMOR_MODEL_PIECE_* constant
int XPAspMan_GetNumberOfModelPieceVariation(string sRRG, int iModelPiece);

//Get the Real ModelPiece Appearance Variation
//sRRG: race-gender code of the accessory models, between A_ and the piece name: eg HHM for A_HHM_LShoulder01 (no P_)
//iModelPiece: ARMOR_MODEL_PIECE_* constant
//iIdx: Internal List Index
int XPAspMan_GetModelPieceVariation(string sRRG, int iModelPiece, int iIdx);




/********************************** Weapon **********************************/


//Return the number of parts of this weapon type (1 for a weapon in one part, 3 for parts A, B and C)
//sWeaponType: W_ and the weapon name of the model files (eg W_LSWORD for W_LSword01_a), or I_ and the item name
int XPAspMan_GetNumberOfWeaponPart(string sWeaponType);

//Return the number of models for this part of the weapon type
//iPart: 0 (the only part, or part A), 1 (part B) or 2 (part C)
int XPAspMan_GetNumberOfWeaponPartVariation(string sWeaponType, int iPart);

//Return the model number of this part of the weapon type (sorted by number). -1 if iPart or iIdx is out of range
//iIdx: Internal List Index
int XPAspMan_GetWeaponPartVariation(string sWeaponType, int iPart, int iIdx);



/****************************** ListManagement ******************************/

//Remove the choosen appearance from sVisualType. 
// Will do nothing if sVisualType is the only Visual for this appearance
//sRRG: race-gender code from appearance.2DA RaceRaceGender : eg P_HHM
//iModelPart: ARMOR_MODEL_PART_* constant
void XPAspMan_RemoveFromVisualType(string sRRG, int iModelPart, int iCategory, int iVariation, string sVisualType); 

//Add the choosen appearance in sVisualType (created if it doesn't exist)
//sRRG: race-gender code from appearance.2DA RaceRaceGender : eg P_HHM
//iModelPart: ARMOR_MODEL_PART_* constant
void XPAspMan_AddToVisualType(string sRRG, int iModelPart, int iCategory, int iVariation, string sVisualType);
 
//Delete the selected sVisualType
//Will do nothing if, in this VisualType, it exists some Apperance that don't have any other VisualType
//sRRG: race-gender code from appearance.2DA RaceRaceGender : eg P_HHM
//iModelPart: ARMOR_MODEL_PART_* constant
void XPAspMan_DeleteVisualType(string sRRG, int iModelPart, string sVisualType);

//Delete the selected sVisualTypeToDelete (extended)
//Will do nothing if, in this VisualType, it exists some Apperance that don't have any other VisualType
//The difference with XPAspMan_DeleteVisualType is that it allow "all"/"joker" choice.
//sRR: the beginning of the RRG code (as in appearance.2DA RaceRaceGender, eg P_HHM): eg "P_HH" for human, "C_AKACHI". "" for all
//iGender: a GENDER_* cst (it tests the last letter of the RRG code). If it's neither GENDER_MALE nor GENDER_FEMALE it will be considered as "all"
//iModelPart: ARMOR_MODEL_PART_* constant. (You can use -1 for "All")
void XPAspMan_DeleteExtendedVisualType(string sRR, int iGender, int iModelPart, string sVisualTypeToDelete);

//Copy the content of the choosen sOriginVisualType to sDestinationVisualType
// You can use default / all value
//sRR: the beginning of the RRG code (as in appearance.2DA RaceRaceGender, eg P_HHM): eg "P_HH" for human, "C_AKACHI". "" for all
//iGender: a GENDER_* cst (it tests the last letter of the RRG code). If it's neither GENDER_MALE nor GENDER_FEMALE it will be considered as "all"
//iModelPart: ARMOR_MODEL_PART_* constant. (You can use -1 for "All")
void XPAspMan_CopyExtendedVisualType(string sRR, int iGender, int iModelPart, string sOriginVisualType, string sDestinationVisualType);

//Save the lists in the save file (PathToSaveFile in the ini). The changes made by the list management
//functions are only kept in memory until this call. Return TRUE if saved
int XPAspMan_SaveFile();

//Load the lists from the save file. The changes made since the last XPAspMan_SaveFile are lost.
//Return TRUE if loaded, FALSE if there is no save file
int XPAspMan_LoadFile();

//Move the appearance sApp (code given by XPAspMan_GetVariation, eg CL_003) from sVisualType to sNewVisualType.
//sNewVisualType is created if it doesn't exist
void XPAspMan_ChangeVisualType(string sRRG, int iModelPart, string sVisualType, string sApp, string sNewVisualType);

//Create an empty sVisualType, to fill it later. Does nothing if sRRG has no visual type yet for iModelPart
void XPAspMan_CreateNewVisualType(string sRRG, int iModelPart, string sVisualType);

//Rename sOldName to sNewName for this sRRG and iModelPart. If sNewName already exists, both are merged.
//A restricted visual type stays restricted under its new name
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
	NWNXSetInt("AspectManager", "listSystemSetRestrictedCategory", sVisualType, 0, bRestricted);
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
	return NWNXGetInt( "AspectManager", "listSystemNumOfVARByRRGPartVAT", sRRG+"#"+sVisualType, iModelPart);
}

string XPAspMan_GetVariation(string sRRG, int iModelPart, string sVisualType, int iIdx)
{
	return NWNXGetString( "AspectManager", "listSystemVarByRRGPartVatIdx", sRRG+"#"+sVisualType, iModelPart*1000+iIdx);
}

int XPAspMan_GetRealAppNumberOfVisualType(string sRRG, int iModelPart, int iCategory, int iVariation)
{
	return NWNXGetInt( "AspectManager", "listSystemNumberOfVisualCat", sRRG+"#"+IntToString(iCategory), iModelPart*1000+iVariation);
}

string XPAspMan_GetVisualTypeFromRealApp(string sRRG, int iModelPart, int iCategory, int iVariation, int iIdx)
{
	return NWNXGetString( "AspectManager", "listSystemRealToVisualCat", sRRG+"#"+IntToString(iCategory), (iIdx*1000+iModelPart)*1000+iVariation);
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
	NWNXSetString("AspectManager", "listSystemRemoveFromCategory", sRRG+"#"+IntToString(iCategory), iModelPart*1000+iVariation, sVisualType);
}

void XPAspMan_AddToVisualType(string sRRG, int iModelPart, int iCategory, int iVariation, string sVisualType)
{
	NWNXSetString("AspectManager", "listSystemAddToCategory", sRRG+"#"+IntToString(iCategory), iModelPart*1000+iVariation, sVisualType);
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
	NWNXSetString("AspectManager", "listSystemRenameCat", sRRG+"#"+sOldName, iModelPart, sNewName);
}

void XPAspMan_ChangeVisualType(string sRRG, int iModelPart, string sVisualType, string sApp, string sNewVisualType)
{
	NWNXSetString("AspectManager", "listSystemChangeCat", sRRG+"#"+sVisualType, iModelPart, sApp+"#"+sNewVisualType);
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

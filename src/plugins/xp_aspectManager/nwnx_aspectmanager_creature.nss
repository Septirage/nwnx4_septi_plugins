//////////////////////////////////////////////////////////////////////////////////////////////
// nwnx_aspectmanager_creature - creature specific functions of the AspectManager plugin
// Original Scripter:  Septirage
//--------------------------------------------------------------------------------------------
// Last Modified By:	Septirage			2025-07-08	Add Get/Set NeverDrawHelmet/NeverShowArmor  (1.4.7)
//						Septirage			2024-09-08	Add Test functions Get/SetWeight  (1.4.5)
//						Septirage			2024-07-14	Add Get/SetRace_xpAM (1.4.4)
//						Septirage           2024-02-28
//--------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////
#include "nwnx_aspectmanager"


/*********************************************************************************************
* Global note :
* For some parts, Creatures use a set of two values.
* One is used for the current usage (e.g. the current Hair shown).
* The other one represents the "persistent" value, which will be the only one exported/saved.
*
* To allow you to choose which one to change, the relevant function has a bPersist parameter.
* Set it to FALSE to change the "current" value, and to TRUE to change the persisted one.
*
* Quick example :
*   My oPC has the head value set to 4. Therefore, both the persistent and current head values are set to 4.
*       - I call SetCreatureHead_xpAM(oPC, 5, FALSE). => The head of PC change to 5.
*       - The PC leaves the server and rejoin => the head will be back to 4.
*       - I call SetCreatureHead_xpAM(oPC, 5, TRUE) => nothing visible happens.
*       - The PC leaves the server and rejoins again => the change is saved, new head is 5.
*
*   Note : Function without the bPersist parameter will impact "unique" value, that will affect the
*           current AND be persisted
**********************************************************************************************/



//Set the Age of oCreature
// bPersist: Set to TRUE to change the persisted value, FALSE to change the current value.
void SetCreatureAge_xpAM(object oCreature, int iAge, int bPersist=FALSE);

//Get the Age of oCreature
// bPersist: Set to TRUE to get the persisted value, FALSE to get the current value.
int GetCreatureAge_xpAM(object oCreature, int bPersist=FALSE);

//Set the SoundSet of oCreature
void SetCreatureSoundSet_xpAM(object oCreature, int iSoundSet);

//Get the SoundSet of oCreature
int GetCreatureSoundSet_xpAM(object oCreature);

//Set the Race of oCreature
void SetCreatureRace_xpAM(object oCreature, int iRace);

//Get the Race of oCreature
int GetCreatureRace_xpAM(object oCreature);

//Set the SubRace of oCreature
void SetCreatureSubRace_xpAM(object oCreature, int iSubRace);

//Get the SubRace of oCreature
int GetCreatureSubRace_xpAM(object oCreature);


//Set the Specific Armor Class (AC) of oCreature
// iACType: (AC_TYPE_*) constant representing the AC to be set.
// iValue: a number between -128 and +127 that determines the new value of the AC
// All AC_TYPE except BASE, ARMORVAL, SHIELDVAL have a "ReduceValue".
// Positive iValue will increase the chosen AC if bReduce=FALSE, and decrease it if bReduce=TRUE
// Negative values will decrease the chosen AC if bReduce=FALSE, but increase it if bReduce=TRUE
// 
// Note: If you set a specific AC it will be overwritten by other factors that also set the same AC
// For example, setting to 18 will be overwritten when the oCreature equips armor.
// Another example: when a creature receives Mage Armor, the AC_TYPE_ARMOR_SEPT will 
//      be overwritten but NOT the reduced AC_TYPE_ARMOR_SEPT.
void SetCreatureSpecificAC_xpAM(object oCreature, int iACType, int iValue, int bReduce = FALSE);

//Set the Specific Armor Class (AC) of oCreature
// iACType: (AC_TYPE_*) constant representing the AC to be get.
// All AC_TYPE except BASE, ARMORVAL, SHIELDVAL have a "ReduceValue".
int GetCreatureSpecificAC_xpAM(object oCreature, int iACType, int bReduceVal = FALSE);


//Not working for tattoo but can be used to store a 16bits value.
void SetCreatureTattoos_xpAM(object oCreature, int iValue);

//Not working for tattoo but can be used to get a stored 16bits value.
int GetCreatureTattoos_xpAM(object oCreature);

//Weight functions.
//Note that the TotalWeight cant be setted but you can call RecalculateCreatureTotalWeight to trigger again the calculation.
//Equipment weight however can be setted and will be used for the totalWeight calculation. Every equip/unequip that will follow will add/substract on this number. 

//TotalWeight
void RecalculateCreatureTotalWeight_xpAM(object oCreature, int iValue);

int GetCreatureTotalWeight_xpAm(object oCreature);


//Equipment weight
void SetCreatureEquipmentWeight(object oCreature, int iValue);

int GetCreatureEquipmentWeight(object oCreature);


/**************************** Creature Appearance ***************************/

//Needed if you change the Gender of an creature
void SetCreatureVisualGender_xpAM(object oCreature, int iVisualGender);
int GetCreatureVisualGender_xpAM(object oCreature);

//Change the Head of oCreature. 
// bPersist: Set to TRUE to change the persisted value, FALSE to change the current value.
void SetCreatureHead_xpAM(object oCreature, int iVariation, int bPersist=FALSE);

//Get the Head of oCreature
// bPersist: Set to TRUE to get the persisted value, FALSE to get the current value.
int GetCreatureHead_xpAM(object oCreature, int bPersist=FALSE);


//Change the Hair of oCreature. 
// bPersist: Set to TRUE to change the persisted value, FALSE to change the current value.
void SetCreatureHair_xpAM(object oCreature, int iVariation, int bPersist=FALSE);

//Get the Hair of oCreature
// bPersist: Set to TRUE to get the persisted value, FALSE to get the current value.
int GetCreatureHair_xpAM(object oCreature, int bPersist=FALSE);


//Change the Facial Hair of oCreature. 
// bPersist: Set to TRUE to change the persisted value, FALSE to change the current value.
void SetCreatureFacialHair_xpAM(object oCreature, int iVariation, int bPersist=FALSE);

//Get the Facial Hair of oCreature
// bPersist: Set to TRUE to get the persisted value, FALSE to get the current value.
int GetCreatureFacialHair_xpAM(object oCreature, int bPersist=FALSE);


//Change the Tail of oCreature. 
// bPersist: Set to TRUE to change the persisted value, FALSE to change the current value.
void SetCreatureTail_xpAM(object oCreature, int iVariation, int bPersist=FALSE);

//Get the Tail of oCreature
// bPersist: Set to TRUE to get the persisted value, FALSE to get the current value.
int GetCreatureTail_xpAM(object oCreature, int bPersist=FALSE);


//Change the Wings of oCreature. 
// bPersist: Set to TRUE to change the persisted value, FALSE to change the current value.
void SetCreatureWings_xpAM(object oCreature, int iVariation, int bPersist=FALSE);

//Get the Wings of oCreature
// bPersist: Set to TRUE to get the persisted value, FALSE to get the current value.
int GetCreatureWings_xpAM(object oCreature, int bPersist=FALSE);


//Change the choosen color of oCreature.
// iCreatureColor: CREATURE_COLOR_* constant, the choosen color
// sRGB: a RGB string value, on the "#FFFFFF" (or #FFFFFFFF) format
// bPersist: Set to TRUE to change the persisted value, FALSE to change the current value.
void SetCreatureColor_xpAM(object oCreature, string sCreatureColor, string sRGB, int bPersist=FALSE);

//Get the choosen color of oCreature.
// iCreatureColor: CREATURE_COLOR_* constant, the choosen color
// bPersist: Set to TRUE to get the persisted value, FALSE to get the current value.
string GetCreatureColor_xpAM(object oCreature, string sCreatureColor, int bPersist=FALSE);


//Change the Persisted scale of oCreature based on specific nAxis.
// nAxis: SCALE_* constant, the scale axis to set.
void SetCreaturePersistScale_xpAM(object oCreature, int nAxis, float fScale);

//Get the Persisted scale of the object based on which axis is looked up.
float GetCreaturePersistScale_xpAM(object oCreature, int nAxis);




/****************************************************************************/
/****************************** Creature Visual *****************************/
/****************************************************************************/

//Change the flag NeverShowArmor on oCreature
void SetCreatureNeverShowArmor_xpAM(object oCreature, int bNeverShow);

//Get the flag NeverShowArmor for oCreature
int GetCreatureNeverShowArmor_xpAM(object oCreature);

//Change the flag NeverDrawHelmet on oCreature.
// Note: its recommanded to remove the helm before changing this.
void SetCreatureNeverDrawHelmet_xpAM(object oCreature, int bNeverDraw);

//Get the flag NeverDrawHelmet for oCreature
int GetCreatureNeverDrawHelmet_xpAM(object oCreature);

//Change the iModelPart mask, allowing you to show or hide model part on the 
//  creature visual (so without armor)
// It's bitmask based with : 
// 0x1: Helm, 0x2: Gloves,  0x4: Boots, 0x8: Belt, 0x10: Cloak
void SetCreatureModelPartMask_xpAM(object oCreature, int iMask);


//Get the iModelPart mask, tell you to which model part are shown or hiden 
//  on the creature visual (so without armor)
// It's bitmask based with : 
// 0x1: Helm, 0x2: Gloves,  0x4: Boots, 0x8: Belt, 0x10: Cloak
int GetCreatureModelPartMask_xpAM(object oCreature);



//Set the iVariation for the oCreature model (without armor)
void SetCreatureVisualVariation_xpAM(object oCreature, int iVariation);

//Get the iVariation for the oCreature model (without armor)
int GetCreatureVisualVariation_xpAM(object oCreature);



//Set the Category (armorvisualdata.2da) for the oCreature model (without armor)
void SetCreatureVisualCategory_xpAM(object oCreature, int iCategory);

//Get the Category (armorvisualdata.2da) for the oCreature model (without armor)
int GetCreatureVisualCategory_xpAM(object oCreature);



//Set the visual Color for oCreature (Seems to be persisted but not used)
void SetCreatureVisualColor_xpAM(object oCreature, int iColor, string sRGB);

string GetCreatureVisualColor_xpAM(object oCreature, int iColor);




/***************************** Visual ModelPart *****************************/

//Set the ModelPart Variation for oCreature Visual.
//	iModelPart : ARMOR_MODEL_PART_* constant
void SetCreatureVisualModelPartVariation_xpAM(object oCreature, int iModelPart, int iVariation);

//Get the ModelPart Variation for oCreature Visual.
//	iModelPart : ARMOR_MODEL_PART_* constant
int GetCreatureVisualModelPartVariation_xpAM(object oCreature, int iModelPart);


//Set the ModelPart Visual Category for oCreature Visual (armorvisualdata.2DA ID).
//	iModelPart : ARMOR_MODEL_PART_* constant
void SetCreatureVisualModelPartCategory_xpAM(object oCreature, int iModelPart, int iCategory);

//Get the ModelPart Visual Category for oCreature Visual (armorvisualdata.2DA ID).
//	iModelPart : ARMOR_MODEL_PART_* constant
int GetCreatureVisualModelPartCategory_xpAM(object oCreature, int iModelPart);


//Set the Color of the iModelPart of oCreature Visual
//	iModelPart : ARMOR_MODEL_PART_* constant
//	iColor : 1, 2 or 3
// sRGB: a RGB string value, on the "#FFFFFF" (or #FFFFFFFF) format
void SetCreatureVisualModelPartColor_xpAM(object oCreature, int iModelPart, int iColor, string sRGB);

//Get the Color of the iModelPart of oCreature Visual
//	iModelPart : ARMOR_MODEL_PART_* constant
//	iColor : 1, 2 or 3
string GetCreatureVisualModelPartColor_xpAM(object oCreature, int iModelPart, int iColor);



/***************************** Visual ModelPiece ****************************/

//Set the ModelPiece Variation for oCreature Visual. Use ARMOR_MODEL_PIECE_* constant for iModelPiece
void SetCreatureVisualModelPieceVariation_xpAM(object oCreature, int iModelPiece, int iVariation);

//Get the ModelPiece Variation for oCreature Visual. Use ARMOR_MODEL_PIECE_* constant for iModelPiece
int GetCreatureVisualModelPieceVariation_xpAM(object oCreature, int iModelPiece);


//Set the Color of the iModelPiece of oCreature Visual
//	iModelPiece : ARMOR_MODEL_PIECE_* constant
//	iColor : 1, 2 or 3
// sRGB: a RGB string value, on the "#FFFFFF" (or #FFFFFFFF) format
void SetCreatureVisualModelPieceColor_xpAM(object oCreature, int iModelPiece, int iColor, string sRGB);

//Get the Color of the iModelPiece of oCreature Visual
//	iModelPiece : ARMOR_MODEL_PIECE_* constant
//	iColor : 1, 2 or 3
string GetCreatureVisualModelPieceColor_xpAM(object oCreature, int iModelPiece, int iColor);



//Enable or Disable the UVScroll of the iModelPiece of oCreature Visual
//	iModelPiece : ARMOR_MODEL_PIECE_* constant
void SetCreatureVisualModelPieceIsUVScroll_xpAM(object oCreature, int iModelPiece, int bActivate);

//Get the activation status for the UVScroll of the iModelPiece of oItoCreature Visuale
//	iModelPiece : ARMOR_MODEL_PIECE_* constant
int GetCreatureVisualModelPieceIsUVScroll_xpAM(object oCreature, int iModelPiece);



//Set the UScroll speed for the iModelPiece of oCreature Visual
//	iModelPiece : ARMOR_MODEL_PIECE_* constant
void SetCreatureVisualModelPieceUScroll_xpAM(object oCreature, int iModelPiece, float fValue);

//Get the UScroll speed for the iModelPiece of oCreature Visual
//	iModelPiece : ARMOR_MODEL_PIECE_* constant
float GetCreatureVisualModelPieceUScroll_xpAM(object oCreature, int iModelPiece);


//Set the VScroll speed for the iModelPiece of oCreature Visual
//	iModelPiece : ARMOR_MODEL_PIECE_* constant
void SetCreatureVisualModelPieceVScroll_xpAM(object oCreature, int iModelPiece, float fValue);

//Get the VScroll speed for the iModelPiece of oCreature Visual
//	iModelPiece : ARMOR_MODEL_PIECE_* constant
float GetCreatureVisualModelPieceVScroll_xpAM(object oCreature, int iModelPiece);


/*****************************************************************************/

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////// CONSTANTS //////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/*****************************************************************************/

const string CREATURE_COLOR_1_SEPT 					= "Color1";
const string CREATURE_COLOR_2_SEPT					= "Color2";
const string CREATURE_COLOR_3_SEPT					= "Color3";
const string CREATURE_COLOR_HEAD_SEPT				= "ColorHeadSkin";
const string CREATURE_COLOR_EYE_SEPT				= "ColorHeadEye";
const string CREATURE_COLOR_BROW_SEPT				= "ColorHeadBrow";
const string CREATURE_COLOR_HAIRACCESSORY_SEPT		= "ColorHairHacc";
const string CREATURE_COLOR_HAIR1_SEPT				= "ColorHair1";
const string CREATURE_COLOR_HAIR2_SEPT				= "ColorHair2";


const int AC_TYPE_BASE_SEPT 		= 20;
const int AC_TYPE_ARMORVAL_SEPT 	= 21;
const int AC_TYPE_SHIELDVAL_SEPT	= 22; 
const int AC_TYPE_ARMOR_SEPT 		= 3;
const int AC_TYPE_DEFLECT_SEPT 		= 5;
const int AC_TYPE_SHIELD_SEPT 		= 7;
const int AC_TYPE_NATURAL_SEPT 		= 9;
const int AC_TYPE_DODGE_SEPT 		= 11;

/*****************************************************************************/

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////// IMPLEMENTATIONS ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/*****************************************************************************/


void SetCreatureAge_xpAM(object oCreature, int iAge, int bPersist)
{
	string sType = "Age";
	if(bPersist == TRUE)
		sType += "Persist";

	NWNXSetInt("AspectManager", "creature", sType, ObjectToInt(oCreature), bPersist);
}

int GetCreatureAge_xpAM(object oCreature, int bPersist)
{
	string sType = "Age";
	if(bPersist == TRUE)
		sType += "Persist";
		
	return NWNXGetInt("AspectManager", "creature", sType, ObjectToInt(oCreature));
}


void SetCreatureSoundSet_xpAM(object oCreature, int iSoundSet)
{
	NWNXSetInt("AspectManager", "creature", "SoundSet", ObjectToInt(oCreature), iSoundSet);
}

int GetCreatureSoundSet_xpAM(object oCreature)
{
	return NWNXGetInt("AspectManager", "creature", "SoundSet", ObjectToInt(oCreature));
}


void SetCreatureRace_xpAM(object oCreature, int iRace)
{
	NWNXSetInt("AspectManager", "creature", "Race", ObjectToInt(oCreature), iRace);
}

int GetCreatureRace_xpAM(object oCreature)
{
	return NWNXGetInt("AspectManager", "creature", "Race", ObjectToInt(oCreature));
}


void SetCreatureSubRace_xpAM(object oCreature, int iSubRace)
{
	NWNXSetInt("AspectManager", "creature", "SubRace", ObjectToInt(oCreature), iSubRace);
}

int GetCreatureSubRace_xpAM(object oCreature)
{
	return NWNXGetInt("AspectManager", "creature", "SubRace", ObjectToInt(oCreature));
}

void SetCreatureSpecificAC_xpAM(object oCreature, int iACType, int iValue, int bReduceVal)
{
	if(iACType >= 20)
		iACType = iACType -20;
	else
		iACType += bReduceVal;
	
	NWNXSetInt("AspectManager", "creature", "AC"+IntToString(iACType), ObjectToInt(oCreature), iValue);
}

int GetCreatureSpecificAC_xpAM(object oCreature, int iACType, int bReduceVal = FALSE)
{
	if(iACType >= 20)
		iACType = iACType -20;
	else
		iACType += bReduceVal;
		
	return NWNXGetInt("AspectManager", "creature", "AC"+IntToString(iACType), ObjectToInt(oCreature));
}

//Not working for tattoo but can be used to store a 16bits id.
void SetCreatureTattoos_xpAM(object oCreature, int iValue)
{
	NWNXSetInt("AspectManager", "creature", "Tattoo", ObjectToInt(oCreature), iValue);
}


int GetCreatureTattoos_xpAM(object oCreature)
{
	return NWNXGetInt("AspectManager", "creature", "Tattoo", ObjectToInt(oCreature));
}



void RecalculateCreatureTotalWeight_xpAM(object oCreature, int iValue)
{
	NWNXSetInt("AspectManager", "creature", "WeightTotal", ObjectToInt(oCreature), iValue);
}

int GetCreatureTotalWeight_xpAm(object oCreature)
{
	return NWNXGetInt("AspectManager", "creature", "WeightTotal", ObjectToInt(oCreature));
}

void SetCreatureEquipmentWeight(object oCreature, int iValue)
{
	NWNXSetInt("AspectManager", "creature", "WeightEquipment", ObjectToInt(oCreature), iValue);
}

int GetCreatureEquipmentWeight(object oCreature)
{
	return NWNXGetInt("AspectManager", "creature", "WeightEquipment", ObjectToInt(oCreature));
}


/**************************** Creature Appearance ***************************/

void SetCreatureVisualGender_xpAM(object oCreature, int iVisualGender)
{
	NWNXSetInt("AspectManager", "creature", "VisualGender", ObjectToInt(oCreature), iVisualGender);
}

int GetCreatureVisualGender_xpAM(object oCreature)
{
	return NWNXGetInt("AspectManager", "creature", "VisualGender", ObjectToInt(oCreature));
}

void SetCreatureHead_xpAM(object oCreature, int iVariation, int bPersist)
{
	string sType = "HeadVar";
	if(bPersist == TRUE)
		sType += "P";
	NWNXSetInt("AspectManager", "creature", sType, ObjectToInt(oCreature), iVariation);
}

int GetCreatureHead_xpAM(object oCreature, int bPersist)
{
	string sType = "HeadVar";
	if(bPersist == TRUE)
		sType += "P";
	return NWNXGetInt("AspectManager", "creature", sType, ObjectToInt(oCreature));
}

void SetCreatureHair_xpAM(object oCreature, int iVariation, int bPersist)
{
	string sType = "HairVar";
	if(bPersist == TRUE)
		sType += "P";
	NWNXSetInt("AspectManager", "creature", sType, ObjectToInt(oCreature), iVariation);
}

int GetCreatureHair_xpAM(object oCreature, int bPersist)
{
	string sType = "HairVar";
	if(bPersist == TRUE)
		sType += "P";
	return NWNXGetInt("AspectManager", "creature", sType, ObjectToInt(oCreature));
}

void SetCreatureFacialHair_xpAM(object oCreature, int iVariation, int bPersist)
{
	string sType = "FHairVar";
	if(bPersist == TRUE)
		sType += "P";
	NWNXSetInt("AspectManager", "creature", sType, ObjectToInt(oCreature), iVariation);
}

int GetCreatureFacialHair_xpAM(object oCreature, int bPersist)
{
	string sType = "FHairVar";
	if(bPersist == TRUE)
		sType += "P";
	return NWNXGetInt("AspectManager", "creature", sType, ObjectToInt(oCreature));
}

void SetCreatureTail_xpAM(object oCreature, int iVariation, int bPersist)
{
	string sType = "TailVar";
	if(bPersist == TRUE)
		sType += "P";
	NWNXSetInt("AspectManager", "creature", sType, ObjectToInt(oCreature), iVariation);
}

int GetCreatureTail_xpAM(object oCreature, int bPersist)
{
	string sType = "TailVar";
	if(bPersist == TRUE)
		sType += "P";
	return NWNXGetInt("AspectManager", "creature", sType, ObjectToInt(oCreature));
}

void SetCreatureWings_xpAM(object oCreature, int iVariation, int bPersist)
{
	string sType = "WingVar";
	if(bPersist == TRUE)
		sType += "P";
	NWNXSetInt("AspectManager", "creature", sType, ObjectToInt(oCreature), iVariation);
}

int GetCreatureWings_xpAM(object oCreature, int bPersist)
{
	string sType = "WingVar";
	if(bPersist == TRUE)
		sType += "P";
	return NWNXGetInt("AspectManager", "creature", sType, ObjectToInt(oCreature));
}

void SetCreatureColor_xpAM(object oCreature, string sCreatureColor, string sRGB, int bPersist)
{
	if(bPersist == TRUE)
		sCreatureColor += "P";
		
	NWNXSetString("AspectManager", "creature", sCreatureColor, ObjectToInt(oCreature), sRGB);
}

string GetCreatureColor_xpAM(object oCreature, string sCreatureColor, int bPersist)
{
	if(bPersist == TRUE)
		sCreatureColor += "P";

	return NWNXGetString("AspectManager", "creature", sCreatureColor, ObjectToInt(oCreature));
}


//Change the scale of an object.
void SetCreaturePersistScale_xpAM(object oCreature, int nAxis, float fScale)
{
	string sType = "PersistScale";
	
	if(nAxis == SCALE_X)
		sType += "X";
	else if(nAxis == SCALE_Y)
		sType += "Y";
	else if(nAxis == SCALE_Z)
		sType += "Z";
	
	NWNXSetFloat("AspectManager", "creature", sType, ObjectToInt(oCreature), fScale);
}

//Get the scale of the object based on which axis is looked up. TODO
float GetCreaturePersistScale_xpAM(object oCreature, int nAxis)
{
	string sType = "PersistScale";
	
	if(nAxis == SCALE_X)
		sType += "X";
	else if(nAxis == SCALE_Y)
		sType += "Y";
	else if(nAxis == SCALE_Z)
		sType += "Z";
	
	return NWNXGetFloat("AspectManager", "creature", sType, ObjectToInt(oCreature));
}


/****************************************************************************/
/****************************** Creature Visual *****************************/
/****************************************************************************/

void SetCreatureNeverShowArmor_xpAM(object oCreature, int bNeverShow)
{
	NWNXSetInt("AspectManager", "creature", "NeverShowArmor", ObjectToInt(oCreature), bNeverShow);
}

int GetCreatureNeverShowArmor_xpAM(object oCreature)
{
	return NWNXGetInt("AspectManager", "creature", "NeverShowArmor", ObjectToInt(oCreature));
}

void SetCreatureNeverDrawHelmet_xpAM(object oCreature, int bNeverDraw)
{
	NWNXSetInt("AspectManager", "creature", "NeverDrawHelmet", ObjectToInt(oCreature), bNeverShow);
}

int GetCreatureNeverDrawHelmet_xpAM(object oCreature)
{
	return NWNXGetInt("AspectManager", "creature", "NeverDrawHelmet", ObjectToInt(oCreature));
}

void SetCreatureModelPartMask_xpAM(object oCreature, int iMask)
{
	NWNXSetInt("AspectManager", "creature", "ModelPartMask", ObjectToInt(oCreature), iMask);
}

int GetCreatureModelPartMask_xpAM(object oCreature)
{
	return NWNXGetInt("AspectManager", "creature", "ModelPartMask", ObjectToInt(oCreature));
}

void SetCreatureVisualVariation_xpAM(object oCreature, int iVariation)
{
	NWNXSetInt("AspectManager", "creature", "Variation", ObjectToInt(oCreature), iVariation);
}

int GetCreatureVisualVariation_xpAM(object oCreature)
{
	return NWNXGetInt("AspectManager", "creature", "Variation", ObjectToInt(oCreature));
}

void SetCreatureVisualCategory_xpAM(object oCreature, int iCategory)
{
	NWNXSetInt("AspectManager", "creature", "Category", ObjectToInt(oCreature), iCategory);
}

int GetCreatureVisualCategory_xpAM(object oCreature)
{
	return NWNXGetInt("AspectManager", "creature", "Category", ObjectToInt(oCreature));
}

//Saved but don't seem to have effect ?
void SetCreatureVisualColor_xpAM(object oCreature, int iColor, string sRGB)
{
	NWNXSetString("AspectManager", "creature", "ArmorColor"+IntToString(iColor), ObjectToInt(oCreature), sRGB);
}


string GetCreatureVisualColor_xpAM(object oCreature, int iColor)
{
	return NWNXGetString("AspectManager", "creature", "ArmorColor"+IntToString(iColor), ObjectToInt(oCreature));
}



/***************************** Visual ModelPart *****************************/

void SetCreatureVisualModelPartVariation_xpAM(object oCreature, int iModelPart, int iVariation)
{
	NWNXSetInt("AspectManager", "creature", "ModelPart"+IntToString(iModelPart)+"Variation", ObjectToInt(oCreature), iVariation);
}


int GetCreatureVisualModelPartVariation_xpAM(object oCreature, int iModelPart)
{
	return NWNXGetInt("AspectManager", "creature", "ModelPart"+IntToString(iModelPart)+"Variation", ObjectToInt(oCreature));
}


void SetCreatureVisualModelPartCategory_xpAM(object oCreature, int iModelPart, int iCategory)
{
	NWNXSetInt("AspectManager", "creature", "ModelPart"+IntToString(iModelPart)+"Category", ObjectToInt(oCreature), iCategory);
}

int GetCreatureVisualModelPartCategory_xpAM(object oCreature, int iModelPart)
{
	return NWNXGetInt("AspectManager", "creature", "ModelPart"+IntToString(iModelPart)+"Category", ObjectToInt(oCreature));
}


void SetCreatureVisualModelPartColor_xpAM(object oCreature, int iModelPart, int iColor, string sRGB)
{
	NWNXSetString("AspectManager", "creature", "ModelPart"+IntToString(iModelPart)+"Color"+IntToString(iColor), ObjectToInt(oCreature), sRGB);
}

string GetCreatureVisualModelPartColor_xpAM(object oCreature, int iModelPart, int iColor)
{
	return NWNXGetString("AspectManager", "creature", "ModelPart"+IntToString(iModelPart)+"Color"+IntToString(iColor), ObjectToInt(oCreature));
}


/***************************** Visual ModelPiece ****************************/

void SetCreatureVisualModelPieceVariation_xpAM(object oCreature, int iModelPiece, int iVariation)
{
	NWNXSetInt("AspectManager", "creature", "ModelPiece"+IntToString(iModelPiece)+"Variation", ObjectToInt(oCreature), iVariation);
}

int GetCreatureVisualModelPieceVariation_xpAM(object oCreature, int iModelPiece)
{
	return NWNXGetInt("AspectManager", "creature", "ModelPiece"+IntToString(iModelPiece)+"Variation", ObjectToInt(oCreature));
}


void SetCreatureVisualModelPieceIsUVScroll_xpAM(object oCreature, int iModelPiece, int bActivate)
{
	NWNXSetInt("AspectManager", "creature", "ModelPiece"+IntToString(iModelPiece)+"UVScroll", ObjectToInt(oCreature), bActivate);
}

int GetCreatureVisualModelPieceIsUVScroll_xpAM(object oCreature, int iModelPiece)
{
	return NWNXGetInt("AspectManager", "creature", "ModelPiece"+IntToString(iModelPiece)+"UVScroll", ObjectToInt(oCreature));
}

void SetCreatureVisualModelPieceUScroll_xpAM(object oCreature, int iModelPiece, float fValue)
{
	NWNXSetFloat("AspectManager", "creature", "ModelPiece"+IntToString(iModelPiece)+"UScroll", ObjectToInt(oCreature), fValue);
}

float GetCreatureVisualModelPieceUScroll_xpAM(object oCreature, int iModelPiece)
{
	return NWNXGetFloat("AspectManager", "creature", "ModelPiece"+IntToString(iModelPiece)+"UScroll", ObjectToInt(oCreature));
}

void SetCreatureVisualModelPieceVScroll_xpAM(object oCreature, int iModelPiece, float fValue)
{
	NWNXSetFloat("AspectManager", "creature", "ModelPiece"+IntToString(iModelPiece)+"VScroll", ObjectToInt(oCreature), fValue);
}

float GetCreatureVisualModelPieceVScroll_xpAM(object oCreature, int iModelPiece)
{
	return NWNXGetFloat("AspectManager", "creature", "ModelPiece"+IntToString(iModelPiece)+"VScroll", ObjectToInt(oCreature));
}


void SetCreatureVisualModelPieceColor_xpAM(object oCreature, int iModelPiece, int iColor, string sRGB)
{
	NWNXSetString("AspectManager", "creature", "ModelPiece"+IntToString(iModelPiece)+"Color"+IntToString(iColor), ObjectToInt(oCreature), sRGB);
}

string GetCreatureVisualModelPieceColor_xpAM(object oCreature, int iModelPiece, int iColor)
{
	return NWNXGetString("AspectManager", "creature", "ModelPiece"+IntToString(iModelPiece)+"Color"+IntToString(iColor), ObjectToInt(oCreature));
}
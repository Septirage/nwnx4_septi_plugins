​​​​​//////////////////////////////////////////////////////////////////////////////////////////////
// nwnx_aspectmanager_item - item specific functions of the AspectManager plugin
// Original Scripter:  Septirage
//--------------------------------------------------------------------------------------------
// Last Modified By:	Septirage			2024-05-18	Add Get/SetItemModelPartMask_xpAM (1.4.2)
//						Septirage           2024-02-28
//--------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////
#include "nwnx_aspectmanager"


//Set the base item type (BASE_ITEM_* or Baseitems.2da ID) of oItem.
void SetBaseItemType_xpAM(object oItem, int iItemType);

//Set the Visual Variation of oItem
void SetItemAppearanceVariation_xpAM(object oItem, int iVariation);

//Get the Visual Variation of oItem
int GetItemAppearanceVariation_xpAM(object oItem);

//Set the Visual Category of oItem (armorvisualdata.2DA ID) 
void SetItemAppearanceCategory_xpAM(object oItem, int iCategory);

//Get the Visual Category of oItem (armorvisualdata.2DA ID) 
int GetItemAppearanceCategory_xpAM(object oItem);


//Set the Description of oItem.
//If bIdentified = FALSE : set the Non Identified Description.
void SetItemDescription_xpAM(object oItem, string sDescription, int bIdentified=TRUE);

//Get the Description of oItem.
//If bIdentified = FALSE : get the Non Identified Description.
string GetItemDescription_xpAM(object oItem, int bIdentified=TRUE);

//Set the oItem Color.
// iColor : 1,2 or 3
// sRGB: a RGB string value, on the "#FFFFFF" (or #FFFFFFFF) format
void SetItemColor_xpAM(object oItem, int iColor, string sRGB);

//Get the oItem Color.
// iColor : 1,2 or 3
string GetItemColor_xpAM(object oItem, int iColor);


/****************************** WeaponSpecific ******************************/

//Set the "DamageEffect" of the Weapon (ITEM_VISUAL_* / iprp_visualfx.2da)
void SetItemWeaponFX_xpAM(object oItem, int iWpnFX);

//Get the "DamageEffect" of the Weapon (ITEM_VISUAL_* / iprp_visualfx.2da)
int GetItemWeaponFX_xpAM(object oItem);

//Set the iPart (WEAPON_MODEL_PART_*) of oItem to Variation 
void SetItemWeaponPart_xpAM(object oItem, int iPart, int iVariation);

//Get the Variation of the iPart (WEAPON_MODEL_PART_*) of oItem  
int GetItemWeaponPart_xpAM(object oItem, int iPart);


/******************************** ModelPiece ********************************/

//Set the ModelPiece Variation for oItem. Use ARMOR_MODEL_PIECE_* constant for iModelPiece
void SetItemModelPieceVariation_xpAM(object oItem, int iModelPiece, int iVariation);

//Get the ModelPiece Variation for oItem. Use ARMOR_MODEL_PIECE_* constant for iModelPiece
int GetItemModelPieceVariation_xpAM(object oItem, int iModelPiece);

//Set the Color of the iModelPiece of oItem
//	iModelPiece : ARMOR_MODEL_PIECE_* constant
//	iColor : 1, 2 or 3
// sRGB: a RGB string value, on the "#FFFFFF" (or #FFFFFFFF) format
void SetItemModelPieceColor_xpAM(object oItem, int iModelPiece, int iColor, string sRGB);

//Get the Color of the iModelPiece of oItem
//	iModelPiece : ARMOR_MODEL_PIECE_* constant
//	iColor : 1, 2 or 3
string GetItemModelPieceColor_xpAM(object oItem, int iModelPiece, int iColor);


//Enable or Disable the UVScroll of the iModelPiece of oItem
//	iModelPiece : ARMOR_MODEL_PIECE_* constant
void SetItemModelPieceIsUVScroll_xpAM(object oItem, int iModelPiece, int bUVScroll);

//Get the activation status for the UVScroll of the iModelPiece of oItem
//	iModelPiece : ARMOR_MODEL_PIECE_* constant
int GetItemModelPieceIsUVScroll_xpAM(object oItem, int iModelPiece);


//Set the UScroll speed for the iModelPiece of oItem
//	iModelPiece : ARMOR_MODEL_PIECE_* constant
void SetItemModelPieceUScroll_xpAM(object oItem, int iModelPiece, float fUScroll);

//Get the UScroll speed for the iModelPiece of oItem
//	iModelPiece : ARMOR_MODEL_PIECE_* constant
float GetItemModelPieceUScroll_xpAM(object oItem, int iModelPiece);

//Set the VScroll speed for the iModelPiece of oItem
//	iModelPiece : ARMOR_MODEL_PIECE_* constant
void SetItemModelPieceVScroll_xpAM(object oItem, int iModelPiece, float fVScroll);

//Get the UScroll speed for the iModelPiece of oItem
//	iModelPiece : ARMOR_MODEL_PIECE_* constant
float GetItemModelPieceVScroll_xpAM(object oItem, int iModelPiece);



/******************************** ModelPart *********************************/

//Change the iModelPart mask, allowing you to show or hide model part on the 
//  item visual
// It's bitmask based with : 
// 0x1: Helm, 0x2: Gloves,  0x4: Boots, 0x8: Belt, 0x10: Cloak
void SetItemModelPartMask_xpAM(object oCreature, int iMask);


//Get the iModelPart mask, tell you to which model part are shown or hiden 
//  on the item visual
// It's bitmask based with : 
// 0x1: Helm, 0x2: Gloves,  0x4: Boots, 0x8: Belt, 0x10: Cloak
int GetItemModelPartMask_xpAM(object oCreature);

//Set the ModelPart Variation for oItem.
//	iModelPart : ARMOR_MODEL_PART_* constant
void SetItemModelPartVariation_xpAM(object oItem, int iModelPart, int iVariation);

//Get the ModelPart Variation for oItem.
//	iModelPart : ARMOR_MODEL_PART_* constant
int GetItemModelPartVariation_xpAM(object oItem, int iModelPart);

//Set the ModelPart Visual Category for oItem (armorvisualdata.2DA ID).
//	iModelPart : ARMOR_MODEL_PART_* constant
void SetItemModelPartCategory_xpAM(object oItem, int iModelPart, int iCategory);

//Get the ModelPart Visual Category for oItem (armorvisualdata.2DA ID).
//	iModelPart : ARMOR_MODEL_PART_* constant
int GetItemModelPartCategory_xpAM(object oItem, int iModelPart);

//Set the Color of the iModelPart of oItem
//	iModelPart : ARMOR_MODEL_PART_* constant
//	iColor : 1, 2 or 3
// sRGB: a RGB string value, on the "#FFFFFF" (or #FFFFFFFF) format
void SetItemModelPartColor_xpAM(object oItem, int iModelPart, int iColor, string sRGB);

//Get the Color of the iModelPart of oItem
//	iModelPart : ARMOR_MODEL_PART_* constant
//	iColor : 1, 2 or 3
string GetItemModelPartColor_xpAM(object oItem, int iModelPart, int iColor);





/*****************************************************************************/

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////// IMPLEMENTATIONS ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/*****************************************************************************/




void SetBaseItemType_xpAM(object oItem, int iItemType)
{
	NWNXSetInt("AspectManager", "item","itemType", ObjectToInt(oItem), iItemType);
}

void SetItemAppearanceVariation_xpAM(object oItem, int iVariation)
{
	NWNXSetInt("AspectManager", "item","appVariation", ObjectToInt(oItem), iVariation);
}

int GetItemAppearanceVariation_xpAM(object oItem)
{
	return NWNXGetInt("AspectManager", "item","appVariation", ObjectToInt(oItem));
}


void SetItemAppearanceCategory_xpAM(object oItem, int iCategory)
{
	NWNXSetInt("AspectManager", "item","appCategory", ObjectToInt(oItem), iCategory);
}

int GetItemAppearanceCategory_xpAM(object oItem)
{
	return NWNXGetInt("AspectManager", "item","appCategory", ObjectToInt(oItem));
}


void SetItemDescription_xpAM(object oItem, string sDescription, int bIdentified)
{
	string sDescType;
	if(bIdentified)
		sDescType = "Description";
	else
		sDescType = "DescriptionNI";

	NWNXSetString("AspectManager", "item",sDescType, ObjectToInt(oItem), sDescription);
}


string GetItemDescription_xpAM(object oItem, int bIdentified)
{
	string sDescType;
	if(bIdentified)
		sDescType = "Description";
	else
		sDescType = "DescriptionNI";

	string myDesc = NWNXGetString("AspectManager", "item",sDescType, ObjectToInt(oItem));
	
	//Allow to see difference between a strref and a str description
	// and retrieve the strref value (if needed)
	if(GetStringLeft(myDesc, 1) == "i")
	{
		myDesc = GetStringRight(myDesc, GetStringLength(myDesc)-1);

		int iTlk = StringToInt(myDesc);
		if(iTlk != 0xFFFFFFFF)
		{
			myDesc = GetStringByStrRef(iTlk);
		}
		else
		{
			myDesc = "";
		}
	}
	else
	{
		myDesc = GetStringRight(myDesc, GetStringLength(myDesc)-1);
	}
	return myDesc;
}

void SetItemColor_xpAM(object oItem, int iColor, string sRGB)
{
	NWNXSetString("AspectManager", "item", "BaseColor"+IntToString(iColor), ObjectToInt(oItem), sRGB);
}

string GetItemColor_xpAM(object oItem, int iColor)
{
	return NWNXGetString("AspectManager", "item", "BaseColor"+IntToString(iColor), ObjectToInt(oItem));
}


/****************************** WeaponSpecific ******************************/

void SetItemWeaponFX_xpAM(object oItem, int iWpnFX)
{
	NWNXSetInt("AspectManager", "item","appFXwpn", ObjectToInt(oItem), iWpnFX);
}

int GetItemWeaponFX_xpAM(object oItem)
{
	return NWNXGetInt("AspectManager", "item","appFXwpn", ObjectToInt(oItem));
}

void SetItemWeaponPart_xpAM(object oItem, int iPart, int iVariation)
{
	NWNXSetInt("AspectManager", "item","WpnPart"+IntToString(iPart), ObjectToInt(oItem), iVariation);
}

int GetItemWeaponPart_xpAM(object oItem, int iPart)
{
	return NWNXGetInt("AspectManager", "item","WpnPart"+IntToString(iPart), ObjectToInt(oItem));
}



/******************************** ModelPiece ********************************/

void SetItemModelPieceVariation_xpAM(object oItem, int iModelPiece, int iVariation)
{
	NWNXSetInt("AspectManager", "item", "ModelPiece"+IntToString(iModelPiece)+"Accessory", ObjectToInt(oItem), iVariation);
}


int GetItemModelPieceVariation_xpAM(object oItem, int iModelPiece)
{
	return NWNXGetInt("AspectManager", "item", "ModelPiece"+IntToString(iModelPiece)+"Accessory", ObjectToInt(oItem));
}

void SetItemModelPieceColor_xpAM(object oItem, int iModelPiece, int iColor, string sRGB)
{
	NWNXSetString("AspectManager", "item", "ModelPiece"+IntToString(iModelPiece)+"Color"+IntToString(iColor), ObjectToInt(oItem), sRGB);
}

string GetItemModelPieceColor_xpAM(object oItem, int iModelPiece, int iColor)
{
	return NWNXGetString("AspectManager", "item", "ModelPiece"+IntToString(iModelPiece)+"Color"+IntToString(iColor), ObjectToInt(oItem));
}

void SetItemModelPieceIsUVScroll_xpAM(object oItem, int iModelPiece, int bUVScroll)
{
	NWNXSetInt("AspectManager", "item", "ModelPiece"+IntToString(iModelPiece)+"UVScroll", ObjectToInt(oItem), bUVScroll);
}

int GetItemModelPieceIsUVScroll_xpAM(object oItem, int iModelPiece)
{
	return NWNXGetInt("AspectManager", "item", "ModelPiece"+IntToString(iModelPiece)+"UVScroll", ObjectToInt(oItem));
}

void SetItemModelPieceUScroll_xpAM(object oItem, int iModelPiece, float fUScroll)
{
	NWNXSetFloat("AspectManager", "item", "ModelPiece"+IntToString(iModelPiece)+"UScroll", ObjectToInt(oItem), fUScroll);
}

float GetItemModelPieceUScroll_xpAM(object oItem, int iModelPiece)
{
	return NWNXGetFloat("AspectManager", "item", "ModelPiece"+IntToString(iModelPiece)+"UScroll", ObjectToInt(oItem));
}

void SetItemModelPieceVScroll_xpAM(object oItem, int iModelPiece, float fVScroll)
{
	NWNXSetFloat("AspectManager", "item", "ModelPiece"+IntToString(iModelPiece)+"VScroll", ObjectToInt(oItem), fVScroll);
}

float GetItemModelPieceVScroll_xpAM(object oItem, int iModelPiece)
{
	return NWNXGetFloat("AspectManager", "item", "ModelPiece"+IntToString(iModelPiece)+"VScroll", ObjectToInt(oItem));
}


/******************************** ModelPart *********************************/

void SetItemModelPartMask_xpAM(object oItem, int iMask)
{
	NWNXSetInt("AspectManager", "item", "ModelPartMask", ObjectToInt(oItem), iMask);
}

int GetItemModelPartMask_xpAM(object oItem)
{
	return NWNXGetInt("AspectManager", "item", "ModelPartMask", ObjectToInt(oItem));
}

void SetItemModelPartVariation_xpAM(object oItem, int iModelPart, int iVariation)
{
	NWNXSetInt("AspectManager", "item", "ModelPart"+IntToString(iModelPart)+"Variation", ObjectToInt(oItem), iVariation);
}

int GetItemModelPartVariation_xpAM(object oItem, int iModelPart)
{
	return NWNXGetInt("AspectManager", "item", "ModelPart"+IntToString(iModelPart)+"Variation", ObjectToInt(oItem));
}


void SetItemModelPartCategory_xpAM(object oItem, int iModelPart, int iCategory)
{
	NWNXSetInt("AspectManager", "item", "ModelPart"+IntToString(iModelPart)+"Category", ObjectToInt(oItem), iCategory);
}

int GetItemModelPartCategory_xpAM(object oItem, int iModelPart)
{
	return NWNXGetInt("AspectManager", "item", "ModelPart"+IntToString(iModelPart)+"Category", ObjectToInt(oItem));
}


void SetItemModelPartColor_xpAM(object oItem, int iModelPart, int iColor, string sRGB)
{
	NWNXSetString("AspectManager", "item", "ModelPart"+IntToString(iModelPart)+"Color"+IntToString(iColor), ObjectToInt(oItem), sRGB);
}

string GetItemModelPartColor_xpAM(object oItem, int iModelPart, int iColor)
{
	return NWNXGetString("AspectManager", "item", "ModelPart"+IntToString(iModelPart)+"Color"+IntToString(iColor), ObjectToInt(oItem));
}
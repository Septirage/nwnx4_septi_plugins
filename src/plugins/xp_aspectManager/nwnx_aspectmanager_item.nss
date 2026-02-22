​​​​​//////////////////////////////////////////////////////////////////////////////////////////////
// nwnx_aspectmanager_item - item specific functions of the AspectManager plugin
// Original Scripter:  Septirage
//--------------------------------------------------------------------------------------------
// Last Modified by:	Septirage			2026-02-22  Add SetItemCharges (1.5.3)
//						Septirage			2024-10-29  Add Get/set Cost Function + DmgReduction (1.4.6)
//						Septirage			2024-05-18	Add Get/SetItemModelPartMask_xpAM (1.4.2)
//						Septirage           2024-02-28
//--------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////
#include "nwnx_aspectmanager"


//Set the base item type (BASE_ITEM_* or Baseitems.2da ID) of oItem.
void SetBaseItemType_xpAM(object oItem, int iItemType);

//Get the base material type of oItem. It will refer to a iprp_materials.2da row
int GetItemBaseMaterialType_xpAM(object oItem);

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

// Sets charges left on an item.
// - oItem: item to change
// - nCharges: number of charges.  If value below 0 is passed, # charges will
//   be set to 0.  
// Note: Unless the original SetItemCharges, the item will not be destroyed if 
//		the # of charges drops to 0.
//		And this function also allow you to set a number of charge greater than 50.
// Note: Some context menu can show an invalid amount of usage if the # is greater than 255.
//			But the setted amount will still work correctly.
void SetItemCharges_xpAM(object oItem, int nCharges);


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




/*********************************** Cost ************************************/

//Set the BaseCost of oItem to iCost.
//Note: this BaseCost will be recalculated by the game by several events.
//			See xp_EnhancedFeature pluginfor more information
void SetItemBaseCost_xpAM(object oItem, int iCost);

//Get the BaseCost of oItem.
int GetItemBaseCost_xpAM(object oItem);

//Set the NonIdentified Cost of oItem to iCost.
//Note: this NonIdentifiedCost will be recalculated by game in same time as BaseCost
//			See xp_EnhancedFeature pluginfor more information
void SetItemNonIdentifiedCost_xpAM(object oItem, int iCost);

//Get the NonIdentified Vost of oItem.
int GetItemNonIdentifiedCost_xpAM(object oItem);

//Set the CostModifier of oItem to iCost
void SetItemCostModifier_xpAM(object oItem, int iCost);

//Get the CostModifier of oItem
int GetItemCostModifier_xpAM(object oItem);


/****************************** DamageReduction ******************************/

// Get the number of DamageReduction for this Item
int GetItemNbDamageReduction_xpAM(object oItem);

// Add a new DamageReduction for this Item
//	iAmount : The Amount of Reduction
//	bUseOrLogic : If you add PiercingType, should they work together with a Or logic or no
void AddItemDamageReduction_xpAM(object oItem, int iAmount, int bUseOrLogic);


// Remove a DamageReduction of this Item
//	iDmgRedctIdx : The Idx of the DmgRedct to remove. Should be > 0 and < NbDamageReduction
void RemoveItemDamageReduction_xpAM(object oItem, int iDmgRedctIdx);

// Set the Amount of Damage Reduction for a specific Dmg Redct for this Item
//	iDmgRedctIdx : The Damage Reduction to change. Should be >0 and < NbDamagereduction
//	iAmount : The new Amount to set for the given DmgRedct.
void SetItemDmgReductionAmount_xpAM(object oItem, int iDmgRedctIdx, int iAmount);

// Get the Amount of Damage Reduction for a specific Dmg Redct for this Item
//	iDmgRedctIdx : The Damage Reduction to get the amount for. Should be >0 and < NbDamagereduction
int GetItemDmgReductionAmount_xpAM(object oItem, int iDmgRedctIdx);

// Set the UseOrLogic flag of Damage Reduction for a specific Dmg Redct for this Item
//	iDmgRedctIdx : The Damage Reduction to change. Should be >0 and < NbDamagereduction
//	bUseOrLogic : Set to TRUE to UseOrLogic, FALSE otherwise.
void SetItemDmgReductionUseOr_xpAM(object oItem, int iDmgRedctIdx, int bUseOrLogic);

// Get the UseOrLogic flag of Damage Reduction for a specific Dmg Redct for this Item
//	iDmgRedctIdx : The Damage Reduction to change. Should be >0 and < NbDamagereduction
int GetItemDmgReductionUseOr_xpAM(object oItem, int iDmgRedctIdx);

// Get the Number of Piercing type for a specific DamageReduction of oItem
//	iDmgRedctIdx : The index of the specific DamageReduction
int GetItemDmgRedctNbPiercing_xpAM(object oItem, int iDmgRedctIdx);

// Create and add a new Piercing type for a specific DamageReduction on oItem
//	iDmgRedctIdx : The index of the specific DamageReduction
//	iType : The DR_TYPE_* wanted
//	iSubType : value depend of iType.
//      - DR_TYPE_DMGTYPE:      DAMAGE_TYPE_*
//      - DR_TYPE_GMATERIAL:    GMATERIAL_*
//      - DR_TYPE_ALIGNMENT:    ALIGNMENT_*
//      For other DR_TYPE_ , subtype doesn't matter
void AddItemDmgRedctPiercing_xpAM(object oItem, int iDmgRedctIdx, int iType, int iSubType);

// Remove a specific PiercingType from a specific DamageReduction from oItem
void RemoveItemDmgRedctPiercing_xpAM(object oItem, int iDmgRedctIdx, int iPiercingIdx);

// Change the DR_TYPE_* of a specific PiercingType of a specific DamageReduction from oItem
void SetItemDmgRedctPiercingType_xpAM(object oItem, int iDmgRedctIdx, int iPiercingIdx, int iType);

// Get the DR_TYPE_* value used as a type for the choosen PiercingType
int GetItemDmgRedctPiercingType_xpAM(object oItem, int iDmgRedctIdx, int iPiercingIdx);

// Change the SubType of a specific PiercingType of a specific DamageReduction from oItem
//	iSubType : value depend of iType.
//      - DR_TYPE_DMGTYPE:      DAMAGE_TYPE_*
//      - DR_TYPE_GMATERIAL:    GMATERIAL_*
//      - DR_TYPE_ALIGNMENT:    ALIGNMENT_*
//      For other DR_TYPE_ , subtype doesn't matter
void SetItemDmgRedctPiercingSubType_xpAM(object oItem, int iDmgRedctIdx, int iPiercingIdx, int iSubType);

// Get the subtype value used for the choosen PiercingType.
// Value depend of Type: 
//      - DR_TYPE_DMGTYPE:      DAMAGE_TYPE_*
//      - DR_TYPE_GMATERIAL:    GMATERIAL_*
//      - DR_TYPE_ALIGNMENT:    ALIGNMENT_*
//      For other DR_TYPE_ , subtype doesn't matter
int GetItemDmgRedctPiercingSubType_xpAM(object oItem, int iDmgRedctIdx, int iPiercingIdx);






/*****************************************************************************/

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////// IMPLEMENTATIONS ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/*****************************************************************************/




void SetBaseItemType_xpAM(object oItem, int iItemType)
{
	NWNXSetInt("AspectManager", "item","itemType", ObjectToInt(oItem), iItemType);
}

int GetItemBaseMaterialType_xpAM(object oItem)
{
	return NWNXGetInt("AspectManager", "item", "BaseMaterialType", ObjectToInt(oItem));
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

void SetItemCharges_xpAM(object oItem, int nCharges)
{
	NWNXSetInt("AspectManager", "item", "Charges", ObjectToInt(oItem), nCharges);
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


/*********************************** Cost ************************************/


void SetItemBaseCost_xpAM(object oItem, int iCost)
{
	NWNXSetInt("AspectManager", "item", "BaseCost", ObjectToInt(oItem), iCost);
}

int GetItemBaseCost_xpAM(object oItem)
{
	return NWNXGetInt("AspectManager", "item", "BaseCost", ObjectToInt(oItem));
}

void SetItemNonIdentifiedCost_xpAM(object oItem, int iCost)
{
	NWNXSetInt("AspectManager", "item", "NonIdentifiedCost", ObjectToInt(oItem), iCost);
}

int GetItemNonIdentifiedCost_xpAM(object oItem)
{
	return NWNXGetInt("AspectManager", "item", "NonIdentifiedCost", ObjectToInt(oItem));
}

void SetItemCostModifier_xpAM(object oItem, int iCost)
{
	NWNXSetInt("AspectManager", "item", "ModifyCost", ObjectToInt(oItem), iCost);
}

int GetItemCostModifier_xpAM(object oItem)
{
	return NWNXGetInt("AspectManager", "item", "ModifyCost", ObjectToInt(oItem));
}


/****************************** DamageReduction ******************************/


int GetItemNbDamageReduction_xpAM(object oItem)
{
	return NWNXGetInt("AspectManager", "item", "NumberOfDamageReduction", ObjectToInt(oItem));
}

//DmgRedct
void AddItemDamageReduction_xpAM(object oItem, int iAmount, int bUseOrLogic)
{
	NWNXSetInt("AspectManager", "item", "DmgRedctAddRedct "+IntToString(iAmount), ObjectToInt(oItem), bUseOrLogic);
}

void RemoveItemDamageReduction_xpAM(object oItem, int iDmgRedctIdx)
{
	NWNXSetInt("AspectManager", "item", "RemoveDmgRedct", ObjectToInt(oItem), iDmgRedctIdx);
}

void SetItemDmgReductionAmount_xpAM(object oItem, int iDmgRedctIdx, int iAmount)
{
	NWNXSetInt("AspectManager", "item", "DmgRedctAmount "+IntToString(iDmgRedctIdx), ObjectToInt(oItem), iAmount);
}

int GetItemDmgReductionAmount_xpAM(object oItem, int iDmgRedctIdx)
{
	return NWNXGetInt("AspectManager", "item", "DmgRedctAmount "+IntToString(iDmgRedctIdx), ObjectToInt(oItem));
}

void SetItemDmgReductionUseOr_xpAM(object oItem, int iDmgRedctIdx, int bUseOrLogic)
{
	NWNXSetInt("AspectManager", "item", "DmgRedctUseOrLogic "+IntToString(iDmgRedctIdx), ObjectToInt(oItem), bUseOrLogic);
}

int GetItemDmgReductionUseOr_xpAM(object oItem, int iDmgRedctIdx)
{
	return NWNXGetInt("AspectManager", "item", "DmgRedctUseOrLogic "+IntToString(iDmgRedctIdx), ObjectToInt(oItem));
}

int GetItemDmgRedctNbPiercing_xpAM(object oItem, int iDmgRedctIdx)
{
	return NWNXGetInt("AspectManager", "item", "DmgRedctNumPiercing "+IntToString(iDmgRedctIdx), ObjectToInt(oItem));
}

void AddItemDmgRedctPiercing_xpAM(object oItem, int iDmgRedctIdx, int iType, int iSubType)
{
	string sParam = "DmgRedctAddPiercing "+IntToString(iType)+ " "+IntToString(iSubType);
	NWNXSetInt("AspectManager", "item", sParam, ObjectToInt(oItem), iDmgRedctIdx);
}

void RemoveItemDmgRedctPiercing_xpAM(object oItem, int iDmgRedctIdx, int iPiercingIdx)
{
	string sParam = "DmgRedctRemovePiercing "+IntToString(iDmgRedctIdx);
	NWNXSetInt("AspectManager", "item", sParam, ObjectToInt(oItem), iPiercingIdx);
}

void SetItemDmgRedctPiercingType_xpAM(object oItem, int iDmgRedctIdx, int iPiercingIdx, int iType)
{
	string sParam = "DmgRedctPiercingType "+IntToString(iDmgRedctIdx)+ " "+IntToString(iPiercingIdx);
	NWNXSetInt("AspectManager", "item", sParam, ObjectToInt(oItem), iType);
}

int GetItemDmgRedctPiercingType_xpAM(object oItem, int iDmgRedctIdx, int iPiercingIdx)
{
	string sParam = "DmgRedctPiercingType "+IntToString(iDmgRedctIdx)+ " "+IntToString(iPiercingIdx);
	return NWNXGetInt("AspectManager", "item", sParam, ObjectToInt(oItem));
}

void SetItemDmgRedctPiercingSubType_xpAM(object oItem, int iDmgRedctIdx, int iPiercingIdx, int iSubType)
{
	string sParam = "DmgRedctPiercingSubType "+IntToString(iDmgRedctIdx)+ " "+IntToString(iPiercingIdx);
	NWNXSetInt("AspectManager", "item", sParam, ObjectToInt(oItem), iSubType);
}

int GetItemDmgRedctPiercingSubType_xpAM(object oItem, int iDmgRedctIdx, int iPiercingIdx)
{
	string sParam = "DmgRedctPiercingSubType "+IntToString(iDmgRedctIdx)+ " "+IntToString(iPiercingIdx);
	return NWNXGetInt("AspectManager", "item", sParam, ObjectToInt(oItem));
}

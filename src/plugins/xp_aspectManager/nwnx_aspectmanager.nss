//////////////////////////////////////////////////////////////////////////////////////////////
// nwnx_aspectmanager - various functions for accessing the AspectManager plugin
// Original Scripter:  Septirage
//--------------------------------------------------------------------------------------------
// Last Modified By:	Septirage           2024-04-27  Big Rework for xp_aspectManager v1.4
//                      Septirage			2023-06-04	Add NoArmor management
//						Septirage			2023-03-21  Add getter setter for armormodelpart
// 					   	Septirage           2023-03-08  Add some access as weaponvfx and item desc
// 						Septirage           2022-11-21
//--------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////

/*************************** Hit Points Functions ***************************/

//Set CurrentHitPoints of oObject to iHP.
//	This is the Current "base" HitPoints, without any possible Temporary HitPoints
void SetCurrentHitPoints_xpAM(object oObject, int iHP);


//Get CurrentHitPoints of oObject.
//	This is the Current "base" HitPoints, without any possible Temporary HitPoints
int GetCurrentHitPoints_xpAM(object oObject); //?


//Set Temporary HitPoints of oObject to iHP.
void SetTemporaryHitPoints_xpAM(object oObject, int iHP);

//Get Temporary HitPoints of oObject.
int GetTemporaryHitPoints_xpAM(object oObject);

//Set Max HitPoints of oObject to iHP.
void SetMaxHitPoints_xpAM(object oObject, int iHP);

//Get Max HitPoints of oObject.
int GetMaxHitPoints_xpAM(object oObject);

//Set the Modifier value of Max HitPoints of oObject to iHP.
void SetModMaxHitPoints_xpAM(object oObject, int iHP);

//Set the Modifier value of Max HitPoints of oObject.
int GetModMaxHitPoints_xpAM(object oObject);


/******************************** Appearance ********************************/

//Set Appearance of oObject to iApp. Will be based on respective 2DA tables.
// Work for Door or Placeable
void SetAppearance_xpAM(object oObject, int iApp);

//Get Appearance of oObject. Will be related to respective 2DA tables.
// Work for Door or Placeable
int GetAppearance_xpAM(object oObject);



/********************************* UVScroll *********************************/


//Enable or Disable UVScroll for oObject
//	Work for placeable
void SetUVScroll_xpAM(object oObject, int bActive);

//Get the activation status of UVScroll for oObject
//	Work for placeable
int GetUVScroll_xpAM(object oObject);

//Set UScroll speed for oObject to fUScroll
//	Work for placeable
void SetUScroll_xpAM(object oObject, float fUScroll);

//Get UScroll speed for oObject
//	Work for placeable
float GetUScroll_xpAM(object oObject);

//Set VScroll speed for oObject to fUScroll
//	Work for placeable
void SetVScroll_xpAM(object oObject, float fVScroll);

//Get VScroll speed for oObject
//	Work for placeable
float GetVScroll_xpAM(object oObject);


/******************************** Inventory *********************************/

//Enable or Disable the inventory for oObject Placeable
void SetHasInventory_xpAM(object oObject, int bHasInventory);

// Return the value of HasInventory Flag for oObject Placeable
int GetHasInventory_xpAM(object oObject);

//Set the MaximumInventorySize of Inventory for oObject Placeable to iMaxSize
// iMaxSize max value is 142
void SetInventoryMaxSize_xpAM(object oObject, int iMaxSize);


//Get the MaximumInventorySize of Inventory for oObject Placeable
int GetInventoryMaxSize_xpAM(object oObject);



//Get the number of objects in the oObject inventory
// Work for placeable
int GetInventoryNbItem_xpAM(object oObject);



/*********************************** Lock ***********************************/

//Set the AutoRemoveKey Flag value for oObject
//	Work for Placeable, Door.
void SetAutoRemoveKey_xpAM(object oObject, int bAutoRemove);

//Get the AutoRemoveKey Flag value for oObject
//	Work for Placeable, Door.
int GetAutoRemoveKey_xpAM(object oObject);

/********************************* Trigger **********************************/

//Get the trigger type (return a TRIGGER_TYPE_* constant)
int GetTriggerType_xpAM(object oObject);

//Set trigger type, use TRIGGER_TYPE_* constant
void SetTriggerType_xpAM(object oObject, int iType);

//Set a new Geometry for oTrigger.
// Rules on the sNewGeometry format :
//	NumOfPoint Point1X Point1Y Point1Z Point2X Point2Y Point2Z (etc)
//So, to be clear, start with the number of point. Then the coordinate, 
// each "number" separated by a space. (you can end with a space)
void SetTriggerGeometry_xpAM(object oTrigger, string sNewGeometry);

/******************************** Transition ********************************/

//Set the Invisible Transition flag for oObject
//	Work for Door
void SetIsInvisibleTransition_xpAM(object oObject, int bInvisibleTransition);

//Get the Invisible Transition flag for oObject
//	Work for Door
int GetIsInvisibleTransition_xpAM(object oObject);


//Set the Party Transition flag for oObject
//	Work for Door or Trigger
void SetIsPartyTransition_xpAM(object oObject, int bPartyTransition);

//Get the Party Transition flag for oObject
//	Work for Door or Trigger
int GetIsPartyTransition_xpAM(object oObject);

//Set the Transition Link Object Type of oObject to iType
// iType must be a TRANSITION_TYPE_* constant
//	Work for Door or Trigger
void SetTransitionLinkObjectType_xpAM(object oObject, int iType);

//Get the TRANSITION_TYPE_* used for oObject
//	Work for Door or Trigger
int GetTransitionLinkObjectType_xpAM(object oObject);

//Set the LoadScreenID for oObject to iIDScreen
//	Work for Area, Door, Trigger
void SetLoadScreenID_xpAM(object oObject, int iIDScreen);

//Get the LoadScreenID for oObject
//	Work for Area, Door, Trigger
int GetLoadScreenID_xpAM(object oObject);

//Set the Transition Destination (object Tag) for oObject
//	Work for Door, Trigger 
void SetTransitionDestination_xpAM(object oObject, string sTransitionDest);

//Get the Transition Destination of oObject
//	Work for Door, Trigger 
string GetTransitionDestination_xpAM(object oObject);



/********************************** Saves ***********************************/

// Set the Magic Resistance for oObject. 
// If bReduc is set to TRUE, you will set the Reduction of this Magic Resistance
// Note : If you set bReduc to TRUE, and set an negative iValue, it will increase the magicResistance
//		Work for Creature
void SetMagicResistance_xpAM(object oObject, int iValue, int bReduc = FALSE);

// Get the Magic Resistance for oObject. 
// If bReduc is set to TRUE, you will get the Reduction value of this Magic Resistance
int GetMagicResistance_xpAM(object oObject, int bReduc = FALSE);


// Set the Reflex Saving Throw on a creature, door or placeable.
void SetReflexSavingThrow_xpAM(object oObject, int iSaving);

// Set the Fortitude Saving Throw on a creature, door or placeable.
void SetFortitudeSavingThrow_xpAM(object oObject, int iSaving);

// Set the Will Saving Throw on a creature, door or placeable.
void SetWillSavingThrow_xpAM(object oObject, int iSaving);



/********************************* Faction **********************************/

// Return the FactionID of oObject
//	Work for Creature, Door, Placeable, Trigger
int GetFactionID_xpAM(object oObject);

//Return TRUE if the idFaction is one of the preconfiguredFaction
int GetIsBaseFaction_xpAM(int idFaction);


/********************************** Visual **********************************/

// set sSEF visual effect to oObject. Work for Creature, Item, Placeable, PlacedEffect
void SetAppearanceSEF_xpAM(object oObject, string sSEF);

// get the current visual effect applied to oObject. Work for Creature, Item, Placeable, PlacedEffect
string GetAppearanceSEF_xpAM(object oObject);


// Set the iColor color of oObject to sRGB
//oObject: The object to change. Door or Placeable
//iColor: 1, 2 or 3. The color to change.
//sRGB: color in #RRGGBB format
void SetColor_xpAM(object oObject, int iColor, string sRGB);

// Return the "iColor" color of oObject. In #RRGGBB format
//oObject: a Door or Placeable object
//iColor: 1, 2 or 3. The color to get.
string GetColor_xpAM(object oObject, int iColor);


/*************************** Position and facing ****************************/


//Cause the oObject to imediatly face fFacing
// - fFacing is expressed as anticlockwise degress from Due East 
void SetFacing_xpAM(object oObject, float fFacing);


//Set the position of oObject 
void SetPosition_xpAM(object oObject, float fX, float fY, float fZ);

//Set the position and orientation of oObject
void SetPositionAndFacing_xpAM(object oObject, float fX, float fY, float fZ, float fFacing);



/********************************** Music ***********************************/

// Change the current MusicBackgroundDay to nTrack for oPC in its current area
void MusicBackgroundDayForPC_xpAM(object oPC, int nTrack);

// Change the current MusicBackgroundNight to nTrack for oPC in its current area
void MusicBackgroundNightForPC_xpAM(object oPC, int nTrack);

// Change the current MusicBattle to nTrack for oPC in its current area
void MusicBattleForPC_xpAM(object oPC, int nTrack);


// Start or Stop the current MusicBackground for oPC
void MusicBackgroundRunForPC_xpAM(object oPC, int bStart);

// Start or Stop the current MusicBattle for oPC
void MusicBattleRunForPC_xpAM(object oPC, int bStart);



/**************************** Refresh & Collision ****************************/

// Refresh the visual of oCreatureToRefresh for oPCMustView
void RefreshCreatureForPC_xpAM(object oCreatureToRefresh, object oPCMustView);

// Refresh the visual of oCreatureToRefresh for every PC
void RefreshCreature_xpAM(object oCreatureToRefresh);

// Refresh the visual of an equiped item of oCreatureToRefresh for oPCMustView
// iSlotToRefresh: INVENTORY_SLOT_* constant 
void RefreshEquipedItemForPC_xpAM(int iSlotToRefresh, object oCreatureToRefresh, object oMustView);

// Refresh the visual of an equiped item of oCreatureToRefresh for every PC
// iSlotToRefresh: INVENTORY_SLOT_* constant 
void RefreshEquipedItem_xpAM(int iSlotToRefresh, object oCreatureToRefresh);


// Refresh the visual the object oObjectToRefresh for oPC
void RefreshObjectForPC_xpAM(object oObjectToRefresh, object oPC);

// Refresh the visual the object oObjectToRefresh for every concerned
void RefreshObject_xpAM(object oObjectToRefresh);

void ShowObject_xpAM(object oObjectToShow);
void ShowObjectForPC_xpAM(object oObjectToShow, object oPC);

// Hide the visual of the oObjectToMask for oPC
void MaskObject_xpAM(object oObjectToMask);

// Hide the visual of the object oObjectToMask for every concerned
void MaskObjectForPC_xpAM(object oObjectToMask, object oPC);


// Enable or Disable DynamicCollision of oPlaceable
void SetObjectDynamicCollision_xpAM(object oPlaceable, int bDynamicCollision);

// Get status of DynamicCollision of oPlaceable
int GetObjectDynamicCollision_xpAM(object oPlaceable);



/*****************************************************************************/

///////////////////////////////////////////////////////////////////////////////
////////////////////////////////// CONSTANTS //////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/*****************************************************************************/
const float XPAM_TIME_FOR_HARDREFRESH			= 3.0;

const string LIGHT_COLOR_TYPE_AMBIANT			= "Ambiant";
const string LIGHT_COLOR_TYPE_DIFFUSE			= "Diffuse";
const string LIGHT_COLOR_TYPE_SPECULAR			= "Specular";

const int TRANSITION_TYPE_NONE					= 0;
const int TRANSITION_TYPE_DOOR					= 1;
const int TRANSITION_TYPE_WAYPOINT				= 2;

const int FLICKER_TYPE_LINEAR					= 0;
const int FLICKER_TYPE_BOUNCE					= 1;
const int FLICKER_TYPE_JUMPY					= 2;

const int ARMOR_MODEL_PART_ARMOR				= 5;

const int TRIGGER_TYPE_GENERAL					= 0;
const int TRIGGER_TYPE_TRANSITION				= 1;
const int TRIGGER_TYPE_TRAP						= 2;


/*****************************************************************************/

///////////////////////////////////////////////////////////////////////////////
/////////////////////////////// IMPLEMENTATIONS ///////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/*****************************************************************************/



/*************************** Hit Points Functions ***************************/

void SetCurrentHitPoints_xpAM(object oObject, int iHP)
{
	NWNXSetInt("AspectManager", "hp", "", ObjectToInt(oObject), iHP);
}

int GetCurrentHitPoints_xpAM(object oObject) //?
{
	return NWNXGetInt("AspectManager", "hp", "", ObjectToInt(oObject));
}

void SetTemporaryHitPoints_xpAM(object oObject, int iHP)
{
	NWNXSetInt("AspectManager", "tempHp", "", ObjectToInt(oObject), iHP);
}

int GetTemporaryHitPoints_xpAM(object oObject)
{
	return NWNXGetInt("AspectManager", "tempHp", "", ObjectToInt(oObject));
}


void SetMaxHitPoints_xpAM(object oObject, int iHP)
{
	NWNXSetInt("AspectManager", "maxHp", "", ObjectToInt(oObject), iHP);
}

int GetMaxHitPoints_xpAM(object oObject)
{
	return NWNXGetInt("AspectManager", "maxHp", "", ObjectToInt(oObject));
}

void SetModMaxHitPoints_xpAM(object oObject, int iHP)
{
	NWNXSetInt("AspectManager", "modMaxHp", "", ObjectToInt(oObject), iHP);
}

int GetModMaxHitPoints_xpAM(object oObject)
{
	return NWNXGetInt("AspectManager", "modMaxHp", "", ObjectToInt(oObject));
}

/******************************** UV Scroll *********************************/

void SetUVScroll_xpAM(object oObject, int bActive)
{
	NWNXSetInt("AspectManager", "UVScroll", "", ObjectToInt(oObject), bActive);
}

int GetUVScroll_xpAM(object oObject)
{
	return NWNXGetInt("AspectManager", "UVScroll", "", ObjectToInt(oObject));
}


void SetUScroll_xpAM(object oObject, float fUScroll)
{
	NWNXSetFloat("AspectManager", "ValueUScroll", "", ObjectToInt(oObject), fUScroll);
}

float GetUScroll_xpAM(object oObject)
{
	return NWNXGetFloat("AspectManager", "ValueUScroll", "", ObjectToInt(oObject));
}

void SetVScroll_xpAM(object oObject, float fVScroll)
{
	NWNXSetFloat("AspectManager", "ValueVScroll", "", ObjectToInt(oObject), fVScroll);
}

float GetVScroll_xpAM(object oObject)
{
	return NWNXGetFloat("AspectManager", "ValueVScroll", "", ObjectToInt(oObject));
}

/********************************* Inventory ********************************/

void SetHasInventory_xpAM(object oObject, int bHasInventory)
{
	NWNXSetInt("AspectManager", "HasInventory", "", ObjectToInt(oObject), bHasInventory);
}

int GetHasInventory_xpAM(object oObject)
{
	return NWNXGetInt("AspectManager", "HasInventory", "", ObjectToInt(oObject));
}

void SetInventoryMaxSize_xpAM(object oObject, int iMaxSize)
{
	NWNXSetInt("AspectManager", "InventoryMaxSize", "", ObjectToInt(oObject), iMaxSize);
}

int GetInventoryMaxSize_xpAM(object oObject)
{
	return NWNXGetInt("AspectManager", "InventoryMaxSize", "", ObjectToInt(oObject));
}


int GetInventoryNbItem_xpAM(object oObject)
{
	return NWNXGetInt("AspectManager", "InventoryNbUsed", "", ObjectToInt(oObject));
}



/*********************************** Lock ***********************************/

void SetAutoRemoveKey_xpAM(object oObject, int bAutoRemove)
{
	NWNXSetInt("AspectManager", "AutoRemoveKey", "", ObjectToInt(oObject), bAutoRemove);
}


int GetAutoRemoveKey_xpAM(object oObject)
{
	return NWNXGetInt("AspectManager", "AutoRemoveKey", "", ObjectToInt(oObject));
}

/********************************* Trigger **********************************/

//Get the trigger type
int GetTriggerType_xpAM(object oObject)
{
	return NWNXGetInt("AspectManager", "TriggerType", "", ObjectToInt(oObject));
}

//Set trigger type
void SetTriggerType_xpAM(object oObject, int iType)
{
	NWNXSetInt("AspectManager", "TriggerType", "", ObjectToInt(oObject), iType);
}

void SetTriggerGeometry_xpAM(object oTrigger, string sNewGeometry)
{
	NWNXSetString("AspectManager", "Trigger", "Geometry", ObjectToInt(oTrigger), sNewGeometry);
}

/******************************** Transition ********************************/

void SetIsInvisibleTransition_xpAM(object oObject, int bInvisibleTransition)
{
	NWNXSetInt("AspectManager", "InvisibleTransition", "", ObjectToInt(oObject), bInvisibleTransition);
}

int GetIsInvisibleTransition_xpAM(object oObject)
{
	return NWNXGetInt("AspectManager", "InvisibleTransition", "", ObjectToInt(oObject));
}

void SetIsPartyTransition_xpAM(object oObject, int bPartyTransition)
{
	NWNXSetInt("AspectManager", "PartyTransition", "", ObjectToInt(oObject), bPartyTransition);
}

int GetIsPartyTransition_xpAM(object oObject)
{
	return NWNXGetInt("AspectManager", "PartyTransition", "", ObjectToInt(oObject));
}



void SetTransitionLinkObjectType_xpAM(object oObject, int iType)
{
	NWNXSetInt("AspectManager", "TransitionType", "", ObjectToInt(oObject), iType);
}

int GetTransitionLinkObjectType_xpAM(object oObject)
{
	return NWNXGetInt("AspectManager", "TransitionType", "", ObjectToInt(oObject));
}


void SetLoadScreenID_xpAM(object oObject, int iIDScreen)
{
	NWNXSetInt("AspectManager", "LoadScreenID", "", ObjectToInt(oObject), iIDScreen);
}

int GetLoadScreenID_xpAM(object oObject)
{
	return NWNXGetInt("AspectManager", "LoadScreenID", "", ObjectToInt(oObject));
}


void SetTransitionDestination_xpAM(object oObject, string sTransitionDest)
{
	NWNXSetString("AspectManager", "TransitionDest", "", ObjectToInt(oObject), sTransitionDest);
}


string GetTransitionDestination_xpAM(object oObject)
{
	return NWNXGetString("AspectManager", "TransitionDest", "", ObjectToInt(oObject));
}



/********************************** Saves ***********************************/

void SetMagicResistance_xpAM(object oObject, int iValue, int bReduc = FALSE)
{
	if(bReduc)
		NWNXSetInt("AspectManager", "MagicResistanceReduc", "", ObjectToInt(oObject), iValue);
	else
		NWNXSetInt("AspectManager", "MagicResistance", "", ObjectToInt(oObject), iValue);		
}

int GetMagicResistance_xpAM(object oObject, int bReduc = FALSE)
{
	if(bReduc)
		return NWNXGetInt("AspectManager", "MagicResistanceReduc", "", ObjectToInt(oObject));
	else
		return NWNXGetInt("AspectManager", "MagicResistance", "", ObjectToInt(oObject));
}

void SetReflexSavingThrow_xpAM(object oObject, int iSaving)
{
	NWNXSetInt("AspectManager", "ReflexSavingThrow", "", ObjectToInt(oObject), iSaving);
}

void SetFortitudeSavingThrow_xpAM(object oObject, int iSaving)
{
	NWNXSetInt("AspectManager", "FortitudeSavingThrow", "", ObjectToInt(oObject), iSaving);
}

void SetWillSavingThrow_xpAM(object oObject, int iSaving)
{
	NWNXSetInt("AspectManager", "WillSavingThrow", "", ObjectToInt(oObject), iSaving);
}

// Faction 

int GetFactionID_xpAM(object oObject)
{
	return NWNXGetInt("AspectManager", "FactionID", "", ObjectToInt(oObject));
}

int GetIsBaseFaction_xpAM(int idFaction)
{
	return NWNXGetInt("AspectManager", "IsBaseFaction", "", idFaction);
}

/********************************** Visual **********************************/

void SetAppearance_xpAM(object oObject, int iApp)
{
	NWNXSetInt("AspectManager", "Appearance", "", ObjectToInt(oObject), iApp);
}

int GetAppearance_xpAM(object oObject)
{
	return NWNXGetInt("AspectManager", "Appearance", "", ObjectToInt(oObject));
}



void SetAppearanceSEF_xpAM(object oObject, string sSEF)
{
	if(GetObjectType(oObject) == OBJECT_TYPE_PLACED_EFFECT) {
		NWNXSetString("AspectManager", "AppSEF", "", ObjectToInt(oObject), sSEF);
		MaskObject_xpAM(oObject);
		ShowObject_xpAM(oObject);	
	}
	else {
		NWNXSetString("AspectManager", "AppSEF", "", ObjectToInt(oObject), sSEF);	
	}
}

string GetAppearanceSEF_xpAM(object oObject)
{
	return NWNXGetString("AspectManager", "AppSEF", "", ObjectToInt(oObject));
}



void SetColor_xpAM(object oObject, int iColor, string sRGB)
{
	NWNXSetString("AspectManager", "Color", IntToString(iColor), ObjectToInt(oObject), sRGB);
}

string GetColor_xpAM(object oObject, int iColor)
{
	return NWNXGetString("AspectManager", "Color", IntToString(iColor), ObjectToInt(oObject));
}


// Position and facing 
void SetFacing_xpAM(object oObject, float fFacing)
{
	NWNXSetFloat("AspectManager", "SetFacing", "", ObjectToInt(oObject), fFacing);
}

void SetPosition_xpAM(object oObject, float fX, float fY, float fZ)
{
	string sVal = FloatToString(fX) + " " + FloatToString(fY) + " " + FloatToString(fZ);
	NWNXSetString("AspectManager", "SetPosition", "", ObjectToInt(oObject), sVal);
}

void SetPositionAndFacing_xpAM(object oObject, float fX, float fY, float fZ, float fFacing)
{
	string sVal = FloatToString(fX) + " " + FloatToString(fY) + " ";
	sVal = sVal + FloatToString(fZ) + " " + FloatToString(fFacing);
	NWNXSetString("AspectManager", "SetPositionAndFacing", "", ObjectToInt(oObject), sVal);
}



/********************************** Music ***********************************/

void MusicBackgroundDayForPC_xpAM(object oPC, int nTrack)
{
	NWNXSetInt("AspectManager", "musicMessage", "10",  ObjectToInt(oPC),  nTrack);
}

void MusicBackgroundNightForPC_xpAM(object oPC, int nTrack)
{
	NWNXSetInt("AspectManager", "musicMessage", "11",  ObjectToInt(oPC),  nTrack);
}

void MusicBattleForPC_xpAM(object oPC, int nTrack)
{
	NWNXSetInt("AspectManager", "musicMessage", "12",  ObjectToInt(oPC),  nTrack);
}

void MusicBackgroundRunForPC_xpAM(object oPC, int bStart)
{
	NWNXSetInt("AspectManager", "musicMessage", "00",  ObjectToInt(oPC),  bStart);
}

void MusicBattleRunForPC_xpAM(object oPC, int bStart)
{
	NWNXSetInt("AspectManager", "musicMessage", "02",  ObjectToInt(oPC),  bStart);
}



/**************************** Refresh & Collision ****************************/

void SetObjectDynamicCollision_xpAM(object oPlaceable, int bDynamicCollision)
{
	NWNXSetInt("AspectManager", "placeable", "DynamicCl",  ObjectToInt(oPlaceable),  bDynamicCollision);
}

int GetObjectDynamicCollision_xpAM(object oPlaceable)
{
	return NWNXGetInt("AspectManager", "placeable", "DynamicCl",  ObjectToInt(oPlaceable));
}


void RefreshCreatureForPC_xpAM(object oCreatureToRefresh, object oMustView)
{
	if(GetArea(oCreatureToRefresh) == GetArea(oMustView))
		NWNXSetInt("AspectManager", "refreshNoArmor", "",  ObjectToInt(oCreatureToRefresh),  ObjectToInt(oMustView));
}


void RefreshCreature_xpAM(object oCreatureToRefresh)
{
	object oPC = GetFirstPC(FALSE);

	while(oPC != OBJECT_INVALID)
	{
		RefreshCreatureForPC_xpAM(oCreatureToRefresh, oPC);
		
		oPC = GetNextPC(FALSE);	
	}
}


void RefreshEquipedItemForPC_xpAM(int iSlotToRefresh, object oCreatureToRefresh, object oMustView)
{
	if(GetArea(oCreatureToRefresh) != GetArea(oMustView))
		return;
	if(iSlotToRefresh == INVENTORY_SLOT_BELT) {
		NWNXSetInt("AspectManager", "refreshBelt", "",  ObjectToInt(oCreatureToRefresh),  ObjectToInt(oMustView));
	} else {
		object oObject = GetItemInSlot(iSlotToRefresh, oCreatureToRefresh);
		//No object for this slot, refreshCreature instead.
		if(!GetIsObjectValid(oObject)) {
			RefreshCreatureForPC_xpAM(oCreatureToRefresh, oMustView);
		} else {
			string sRefreshText = "refreshView"+IntToString(iSlotToRefresh);
			NWNXSetInt("AspectManager", sRefreshText, IntToString(ObjectToInt(oObject)), ObjectToInt(oCreatureToRefresh), ObjectToInt(oMustView));
		}
	}
}

void RefreshEquipedItem_xpAM(int iSlotToRefresh, object oCreatureToRefresh)
{
	object oPC = GetFirstPC(FALSE);

	while(oPC != OBJECT_INVALID)
	{
		RefreshEquipedItemForPC_xpAM(iSlotToRefresh, oCreatureToRefresh, oPC);
		
		oPC = GetNextPC(FALSE);	
	}
}

void RefreshObjectForPC_xpAM(object oObjectToRefresh, object oPC)
{
	NWNXSetInt("AspectManager", "refreshObject", "", ObjectToInt(oPC), ObjectToInt(oObjectToRefresh));
}

void MaskObjectForPC_xpAM(object oObjectToMask, object oPC)
{
	NWNXSetInt("AspectManager", "maskObject", "", ObjectToInt(oPC), ObjectToInt(oObjectToMask));
}

void ShowObjectForPC_xpAM(object oObjectToShow, object oPC)
{
	NWNXSetInt("AspectManager", "showObject", "", ObjectToInt(oPC), ObjectToInt(oObjectToShow));
}


void RefreshObject_xpAM(object oObjectToRefresh)
{
	object oPC = GetFirstPC(FALSE);

	while(oPC != OBJECT_INVALID)
	{
		RefreshObjectForPC_xpAM(oObjectToRefresh, oPC);
		
		oPC = GetNextPC(FALSE);	
	}
}

void MaskObject_xpAM(object oObjectToMask)
{
	object oPC = GetFirstPC(FALSE);

	while(oPC != OBJECT_INVALID)
	{
		MaskObjectForPC_xpAM(oObjectToMask, oPC);
		
		oPC = GetNextPC(FALSE);	
	}
}

void ShowObject_xpAM(object oObjectToShow)
{
	object oPC = GetFirstPC(FALSE);

	while(oPC != OBJECT_INVALID)
	{
		ShowObjectForPC_xpAM(oObjectToShow, oPC);
		
		oPC = GetNextPC(FALSE);	
	}
}

void PlaceableHardRefresh(object oObject) {
	object oPC = GetFirstPC(FALSE);

	while(oPC != OBJECT_INVALID)
	{
		MaskObjectForPC_xpAM(oObject, oPC);
		DelayCommand(XPAM_TIME_FOR_HARDREFRESH, ShowObjectForPC_xpAM(oObject, oPC));		
		
		oPC = GetNextPC(FALSE);	
	}
}
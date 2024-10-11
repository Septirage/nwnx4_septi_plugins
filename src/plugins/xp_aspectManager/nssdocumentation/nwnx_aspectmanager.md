xp_aspectManager.nss
=====================

- [Hit Points Functions](#hit-points-functions)
    - [SetCurrentHitPoints\_xpAM](#setcurrenthitpoints_xpam)
    - [GetCurrentHitPoints\_xpAM](#getcurrenthitpoints_xpam)
    - [SetTemporaryHitPoints\_xpAM](#settemporaryhitpoints_xpam)
    - [GetTemporaryHitPoints\_xpAM](#gettemporaryhitpoints_xpam)
    - [SetMaxHitPoints\_xpAM](#setmaxhitpoints_xpam)
    - [GetMaxHitPoints\_xpAM](#getmaxhitpoints_xpam)
    - [SetModMaxHitPoints\_xpAM](#setmodmaxhitpoints_xpam)
    - [GetModMaxHitPoints\_xpAM](#getmodmaxhitpoints_xpam)
- [Appearance](#appearance)
    - [SetAppearance\_xpAM](#setappearance_xpam)
    - [GetAppearance\_xpAM](#getappearance_xpam)
- [UVScroll](#uvscroll)
    - [SetUVScroll\_xpAM](#setuvscroll_xpam)
    - [GetUVScroll\_xpAM](#getuvscroll_xpam)
    - [SetUScroll\_xpAM](#setuscroll_xpam)
    - [GetUScroll\_xpAM](#getuscroll_xpam)
    - [SetVScroll\_xpAM](#setvscroll_xpam)
    - [GetVScroll\_xpAM](#getvscroll_xpam)
- [Inventory](#inventory)
    - [SetHasInventory\_xpAM](#sethasinventory_xpam)
    - [GetHasInventory\_xpAM](#gethasinventory_xpam)
    - [SetInventoryMaxSize\_xpAM](#setinventorymaxsize_xpam)
    - [GetInventoryMaxSize\_xpAM](#getinventorymaxsize_xpam)
    - [GetInventoryNbItem\_xpAM](#getinventorynbitem_xpam)
- [Lock](#lock)
    - [SetAutoRemoveKey\_xpAM](#setautoremovekey_xpam)
    - [GetAutoRemoveKey\_xpAM](#getautoremovekey_xpam)
- [Trigger](#trigger)
    - [GetTriggerType\_xpAM](#gettriggertype_xpam)
    - [SetTriggerType\_xpAM](#settriggertype_xpam)
    - [SetTriggerGeometry\_xpAM](#settriggergeometry_xpam)
- [Transition](#transition)
    - [SetIsInvisibleTransition\_xpAM](#setisinvisibletransition_xpam)
    - [GetIsInvisibleTransition\_xpAM](#getisinvisibletransition_xpam)
    - [SetIsPartyTransition\_xpAM](#setispartytransition_xpam)
    - [GetIsPartyTransition\_xpAM](#getispartytransition_xpam)
    - [SetTransitionLinkObjectType\_xpAM](#settransitionlinkobjecttype_xpam)
    - [GetTransitionLinkObjectType\_xpAM](#gettransitionlinkobjecttype_xpam)
    - [SetLoadScreenID\_xpAM](#setloadscreenid_xpam)
    - [GetLoadScreenID\_xpAM](#getloadscreenid_xpam)
    - [SetTransitionDestination\_xpAM](#settransitiondestination_xpam)
    - [GetTransitionDestination\_xpAM](#gettransitiondestination_xpam)
- [Saves](#saves)
    - [SetMagicResistance\_xpAM](#setmagicresistance_xpam)
    - [GetMagicResistance\_xpAM](#getmagicresistance_xpam)
	- [SetWillSavingThrow\_xpAM](#setwillsavingthrow_xpam)
	- [SetFortitudeSavingThrow\_xpAM](#setfortitudesavingthrow_xpam)
	- [SetReflexSavingThrow\_xpAM](#setreflexsavingthrow_xpam)
- [Faction](#faction)
    - [GetFactionID\_xpAM](#getfactionid_xpam)
    - [GetIsBaseFaction\_xpAM](#getisbasefaction_xpam)
- [Visual](#visual)
    - [SetAppearanceSEF\_xpAM](#setappearancesef_xpam)
    - [GetAppearanceSEF\_xpAM](#getappearancesef_xpam)
    - [SetColor\_xpAM](#setcolor_xpam)
    - [GetColor\_xpAM](#getcolor_xpam)
- [Position and facing](#position-and-facing)
    - [SetFacing\_xpAM](#setfacing_xpam)
    - [SetPosition\_xpAM](#setposition_xpam)
    - [SetPositionAndFacing\_xpAM](#setpositionandfacing_xpam)
- [Music](#music)
    - [MusicBackgroundDayForPC\_xpAM](#musicbackgrounddayforpc_xpam)
    - [MusicBackgroundNightForPC\_xpAM](#musicbackgroundnightforpc_xpam)
    - [MusicBattleForPC\_xpAM](#musicbattleforpc_xpam)
    - [MusicBackgroundRunForPC\_xpAM](#musicbackgroundrunforpc_xpam)
    - [MusicBattleRunForPC\_xpAM](#musicbattlerunforpc_xpam)
- [Refresh, View & Collision](#refresh-view--collision)
    - [RefreshCreatureForPC\_xpAM](#refreshcreatureforpc_xpam)
    - [RefreshCreature\_xpAM](#refreshcreature_xpam)
    - [RefreshEquipedItemForPC\_xpAM](#refreshequipeditemforpc_xpam)
    - [RefreshEquipedItem\_xpAM](#refreshequipeditem_xpam)
    - [RefreshObjectForPC\_xpAM](#refreshobjectforpc_xpam)
    - [RefreshObject\_xpAM](#refreshobject_xpam)
    - [ShowObject\_xpAM](#showobject_xpam)
    - [ShowObjectForPC\_xpAM](#showobjectforpc_xpam)
    - [MaskObject\_xpAM](#maskobject_xpam)
    - [MaskObjectForPC\_xpAM](#maskobjectforpc_xpam)
    - [SetObjectDynamicCollision\_xpAM](#setobjectdynamiccollision_xpam)
    - [GetObjectDynamicCollision\_xpAM](#getobjectdynamiccollision_xpam)
- [Constants](#constants)
    - [Light Color Types (LIGHT\_COLOR\_TYPE\_\*)](#light-color-types-light_color_type_)
    - [Transition Types (TRANSITION\_TYPE\_\*)](#transition-types-transition_type_)
    - [Flicker Types (FLICKER\_TYPE\_\*)](#flicker-types-flicker_type_)
    - [Armor Model Parts (ARMOR\_MODEL\_PART\_*)](#armor-model-parts-armor_model_part_)
    - [Trigger Types (TRIGGER\_TYPE\_\*)](#trigger-types-trigger_type_)


Hit Points Functions
---------------------
---

# SetCurrentHitPoints\_xpAM
```cpp
void SetCurrentHitPoints_xpAM(object oObject, int iHP);
```

Set CurrentHitPoints of oObject to iHP.

## Parameters

* `oObject` - The object to set the CurrentHitPoints for.
* `iHP` - The CurrentHitPoints to set.


## Note

This is the Current "base" HitPoints, without the possible Temporary HitPoints

---

# GetCurrentHitPoints\_xpAM
```cpp
int GetCurrentHitPoints_xpAM(object oObject);
```
Get CurrentHitPoints of oObject.

## Parameters

* `oObject` - The object to get the CurrentHitPoints for.

## Return Value
The CurrentHitPoints of the object.


## Note

This is the Current "base" HitPoints, without the possible Temporary HitPoints

---

# SetTemporaryHitPoints\_xpAM
```cpp
void SetTemporaryHitPoints_xpAM(object oObject, int iHP);
```
Set Temporary HitPoints of oObject to iHP.

## Parameters

* `oObject` - The object to set the Temporary HitPoints for.
* `iHP` - The Temporary HitPoints to set.

---

# GetTemporaryHitPoints\_xpAM
```cpp
int GetTemporaryHitPoints_xpAM(object oObject);
```
Get Temporary HitPoints of oObject.

## Parameters

* `oObject` - The object to get the Temporary HitPoints for.

## Return Value
The Temporary HitPoints of the object.

---

# SetMaxHitPoints\_xpAM
```cpp
void SetMaxHitPoints_xpAM(object oObject, int iHP);
```
Set Max HitPoints of oObject to iHP.

## Parameters

* `oObject` - The object to set the Max HitPoints for.
* `iHP` - The Max HitPoints to set.

---

# GetMaxHitPoints\_xpAM
```cpp
int GetMaxHitPoints_xpAM(object oObject);
```
Get Max HitPoints of oObject.

## Parameters

* `oObject` - The object to get the Max HitPoints for.

## Return Value
The Max HitPoints of the object.

---

# SetModMaxHitPoints\_xpAM
```cpp
void SetModMaxHitPoints_xpAM(object oObject, int iHP);
```
Set the Modifier value of Max HitPoints of oObject to iHP.

## Parameters

* `oObject` - The object to set the Modifier value of Max HitPoints for.
* `iHP` - The Modifier value to set.

---

# GetModMaxHitPoints\_xpAM
```cpp
int GetModMaxHitPoints_xpAM(object oObject);
```
Get the Modifier value of Max HitPoints of oObject.

## Parameters

* `oObject` - The object to get the Modifier value of Max HitPoints for.

## Return Value
The Modifier value of Max HitPoints of the object.

---

Appearance
----------

# SetAppearance\_xpAM
```cpp
void SetAppearance_xpAM(object oObject, int iApp);
```
Set Appearance of oObject to iApp. Will be based on respective 2DA tables. Work for Door or Placeable.

## Parameters

* `oObject` - The object to set the Appearance for. (Door or Placeable)
* `iApp` - The corresponding 2DA index of the appearance to set.

---

# GetAppearance\_xpAM
```cpp
int GetAppearance_xpAM(object oObject);
```
Get Appearance of oObject. Will be related to respective 2DA tables. Work for Door or Placeable.

## Parameters

* `oObject` - The object to get the Appearance for. (Door or Placeable)

## Return Value
The corresponding 2DA index of the appearance of the object.

---

UVScroll
--------

# SetUVScroll\_xpAM
```cpp
void SetUVScroll_xpAM(object oObject, int bActive);
```
Enable or Disable UVScroll for oObject. Work for placeable.

## Parameters

* `oObject` - The object to enable or disable UVScroll for. (Placeable only)
* `bActive` - The flag to enable (TRUE) or disable (FALSE) UVScroll.

---

# GetUVScroll\_xpAM
```cpp
int GetUVScroll_xpAM(object oObject);
```
Get the activation status of UVScroll for oObject. Work for placeable.

## Parameters

* `oObject` - The object to get the UVScroll activation status for. (Placeable only)

## Return Value
The activation status of UVScroll for the object (TRUE if enabled, FALSE if disabled).

---

# SetUScroll\_xpAM
```cpp
void SetUScroll_xpAM(object oObject, float fUScroll);
```
Set UScroll speed for oObject to fUScroll. Work for placeable.

## Parameters

* `oObject` - The object to set the UScroll speed for. (Placeable only)
* `fUScroll` - The UScroll speed to set.

---

# GetUScroll\_xpAM
```cpp
float GetUScroll_xpAM(object oObject);
```
Get UScroll speed for oObject. Work for placeable.

## Parameters

* `oObject` - The object to get the UScroll speed for. (Placeable only)

## Return Value
The UScroll speed of the object.

---

# SetVScroll\_xpAM
```cpp
void SetVScroll_xpAM(object oObject, float fVScroll);
```
Set VScroll speed for oObject to fUScroll. Work for placeable.

## Parameters

* `oObject` - The object to set the VScroll speed for. (Placeable only)
* `fVScroll` - The VScroll speed to set.

---

# GetVScroll\_xpAM
```cpp
float GetVScroll_xpAM(object oObject);
```
Get VScroll speed for oObject. Work for placeable.

## Parameters

* `oObject` - The object to get the VScroll speed for. (Placeable only)

## Return Value
The VScroll speed of the object.

---

Inventory
---------

# SetHasInventory\_xpAM
```cpp
void SetHasInventory_xpAM(object oObject, int bHasInventory);
```
Enable or Disable the inventory for oObject Placeable.

## Parameters

* `oObject` - The Placeable to enable or disable the inventory for.
* `bHasInventory` - The flag to enable (TRUE) or disable (FALSE) the inventory.

---

# GetHasInventory\_xpAM
```cpp
int GetHasInventory_xpAM(object oObject);
```
Return the value of HasInventory Flag for oObject Placeable.

## Parameters

* `oObject` - The Placeable to get the HasInventory flag for.

## Return Value
The value of the HasInventory flag for the object (TRUE if enabled, FALSE if disabled).

---

# SetInventoryMaxSize\_xpAM
```cpp
void SetInventoryMaxSize_xpAM(object oObject, int iMaxSize);
```
Set the MaximumInventorySize of Inventory for oObject Placeable to iMaxSize.

## Parameters

* `oObject` - The object to set the MaximumInventorySize for.
* `iMaxSize` - The MaximumInventorySize to set.

## Note

iMaxSize max value is 142.

---

# GetInventoryMaxSize\_xpAM
```cpp
int GetInventoryMaxSize_xpAM(object oObject);
```
Get the MaximumInventorySize of Inventory for oObject Placeable.

## Parameters

* `oObject` - The object to get the MaximumInventorySize for.

## Return Value
The MaximumInventorySize of the object.

---

# GetInventoryNbItem\_xpAM
```cpp
int GetInventoryNbItem_xpAM(object oObject);
```
Get the number of objects in the oObject inventory. Work for placeable.

## Parameters

* `oObject` - The object to get the number of inventory items for.

## Return Value
The number of items in the inventory of the object.

---

Lock
----

# SetAutoRemoveKey\_xpAM
```cpp
void SetAutoRemoveKey_xpAM(object oObject, int bAutoRemove);
```
Set the AutoRemoveKey Flag value for oObject. Work for Placeable, Door.

## Parameters

* `oObject` - The object to set the AutoRemoveKey flag for.
* `bAutoRemove` - The flag to enable (TRUE) or disable (FALSE) AutoRemoveKey.

---

# GetAutoRemoveKey\_xpAM
```cpp
int GetAutoRemoveKey_xpAM(object oObject);
```
Get the AutoRemoveKey Flag value for oObject. Work for Placeable, Door.

## Parameters

* `oObject` - The object to get the AutoRemoveKey flag for.

## Return Value
The value of the AutoRemoveKey flag for the object (TRUE if enabled, FALSE if disabled).

---

Trigger
-------

# GetTriggerType\_xpAM
```cpp
int GetTriggerType_xpAM(object oObject);
```
Get the trigger type (return a [TRIGGER\_TYPE\_\*](#trigger-types-trigger_type_) constant).

## Parameters

* `oObject` - The object to get the trigger type for.

## Return Value
The trigger type as a [TRIGGER\_TYPE\_\*](#trigger-types-trigger_type_) constant.

---

# SetTriggerType\_xpAM
```cpp
void SetTriggerType_xpAM(object oObject, int iType);
```
Set trigger type, use [TRIGGER\_TYPE\_\*](#trigger-types-trigger_type_) constant.

## Parameters

* `oObject` - The object to set the trigger type for.
* `iType` - The trigger type as a [TRIGGER\_TYPE\_\*](#trigger-types-trigger_type_) constant.

---

# SetTriggerGeometry\_xpAM
```cpp
void SetTriggerGeometry_xpAM(object oTrigger, string sNewGeometry);
```
Set a new Geometry for oTrigger.

## Parameters

* `oTrigger` - The trigger object to set the geometry for.
* `sNewGeometry` - The new geometry in the format "NumOfPoint Point1X Point1Y Point1Z Point2X Point2Y Point2Z ...".

---

Transition
----------

# SetIsInvisibleTransition\_xpAM
```cpp
void SetIsInvisibleTransition_xpAM(object oObject, int bInvisibleTransition);
```
Set the Invisible Transition flag for oObject. Work for Door.

## Parameters

* `oObject` - The object to set the Invisible Transition flag for.
* `bInvisibleTransition` - The flag to enable (TRUE) or disable (FALSE) Invisible Transition.

---

# GetIsInvisibleTransition\_xpAM
```cpp
int GetIsInvisibleTransition_xpAM(object oObject);
```
Get the Invisible Transition flag for oObject. Work for Door.

## Parameters

* `oObject` - The object to get the Invisible Transition flag for.

## Return Value
The value of the Invisible Transition flag for the object (TRUE if enabled, FALSE if disabled).

---

# SetIsPartyTransition\_xpAM
```cpp
void SetIsPartyTransition_xpAM(object oObject, int bPartyTransition);
```
Set the Party Transition flag for oObject. Work for Door or Trigger.

## Parameters

* `oObject` - The object to set the Party Transition flag for.
* `bPartyTransition` - The flag to enable (TRUE) or disable (FALSE) Party Transition.

---

# GetIsPartyTransition\_xpAM
```cpp
int GetIsPartyTransition_xpAM(object oObject);
```
Get the Party Transition flag for oObject. Work for Door or Trigger.

## Parameters

* `oObject` - The object to get the Party Transition flag for.

## Return Value
The value of the Party Transition flag for the object (TRUE if enabled, FALSE if disabled).

---

# SetTransitionLinkObjectType\_xpAM
```cpp
void SetTransitionLinkObjectType_xpAM(object oObject, int iType);
```
Set the Transition Link Object Type of oObject to iType. iType must be a [TRANSITION\_TYPE\_\*](#transition-types-transition_type_) constant. Work for Door or Trigger.

## Parameters

* `oObject` - The object to set the Transition Link Object Type for.
* `iType` - The Transition Link Object Type as a [TRANSITION\_TYPE\_\*](#transition-types-transition_type_) constant.

---

# GetTransitionLinkObjectType\_xpAM
```cpp
int GetTransitionLinkObjectType_xpAM(object oObject);
```
Get the [TRANSITION\_TYPE\_\*](#transition-types-transition_type_) used for oObject. Work for Door or Trigger.

## Parameters

* `oObject` - The object to get the Transition Link Object Type for.

## Return Value
The Transition Link Object Type as a [TRANSITION\_TYPE\_\*](#transition-types-transition_type_) constant.

---

# SetLoadScreenID\_xpAM
```cpp
void SetLoadScreenID_xpAM(object oObject, int iIDScreen);
```
Set the LoadScreenID for oObject to iIDScreen. Work for Area, Door, Trigger.

## Parameters

* `oObject` - The object to set the LoadScreenID for.
* `iIDScreen` - The LoadScreenID to set.

---

# GetLoadScreenID\_xpAM
```cpp
int GetLoadScreenID_xpAM(object oObject);
```
Get the LoadScreenID for oObject. Work for Area, Door, Trigger.

## Parameters

* `oObject` - The object to get the LoadScreenID for.

## Return Value
The LoadScreenID of the object.

---

# SetTransitionDestination\_xpAM
```cpp
void SetTransitionDestination_xpAM(object oObject, string sTransitionDest);
```
Set the Transition Destination (object Tag) for oObject. Work for Door, Trigger.

## Parameters

* `oObject` - The object to set the Transition Destination for.
* `sTransitionDest` - The Transition Destination as an object tag.

---

# GetTransitionDestination\_xpAM
```cpp
string GetTransitionDestination_xpAM(object oObject);
```
Get the Transition Destination of oObject. Work for Door, Trigger.

## Parameters

* `oObject` - The object to get the Transition Destination for.

## Return Value
The Transition Destination as an object tag.

---

Saves
-----

# SetMagicResistance\_xpAM
```cpp
void SetMagicResistance_xpAM(object oObject, int iValue, int bReduc = FALSE);
```
Set the Magic Resistance for oObject. If bReduc is set to TRUE, you will set the Reduction of this Magic Resistance. Note: If you set bReduc to TRUE, and set an negative iValue, it will increase the magicResistance. Work for Creature.

## Parameters

* `oObject` - The object to set the Magic Resistance for.
* `iValue` - The Magic Resistance value to set.
* `bReduc` - The flag to indicate if the value is a reduction (TRUE) or not (FALSE).

---

# GetMagicResistance\_xpAM
```cpp
int GetMagicResistance_xpAM(object oObject, int bReduc = FALSE);
```
Get the Magic Resistance for oObject. If bReduc is set to TRUE, you will get the Reduction value of this Magic Resistance.

## Parameters

* `oObject` - The object to get the Magic Resistance for.
* `bReduc` - The flag to indicate if the value is a reduction (TRUE) or not (FALSE).

## Return Value
The Magic Resistance value or the reduction value if bReduc is TRUE.

---


# SetReflexSavingThrow\_xpAM
```cpp
void SetReflexSavingThrow_xpAM(object oObject, int iSaving);
```
Set the Reflex Saving Throw on a creature, door, or placeable.

## Parameters

* `oObject` - The object to set the Reflex Saving Throw for.
* `iSaving` - The Reflex Saving Throw value to set.

---

# SetFortitudeSavingThrow\_xpAM
```cpp
void SetFortitudeSavingThrow_xpAM(object oObject, int iSaving);
```
Set the Fortitude Saving Throw on a creature, door, or placeable.

## Parameters

* `oObject` - The object to set the Fortitude Saving Throw for.
* `iSaving` - The Fortitude Saving Throw value to set.

---

# SetWillSavingThrow\_xpAM
```cpp
void SetWillSavingThrow_xpAM(object oObject, int iSaving);
```
Set the Will Saving Throw on a creature, door, or placeable.

## Parameters

* `oObject` - The object to set the Will Saving Throw for.
* `iSaving` - The Will Saving Throw value to set.

---


Faction
-------

# GetFactionID\_xpAM
```cpp
int GetFactionID_xpAM(object oObject);
```
Return the FactionID of oObject. Work for Creature, Door, Placeable, Trigger.

## Parameters

* `oObject` - The object to get the FactionID for.

## Return Value
The FactionID of the object.

---

# GetIsBaseFaction\_xpAM
```cpp
int GetIsBaseFaction_xpAM(int idFaction);
```
Return TRUE if the idFaction is one of the preconfiguredFaction.

## Parameters

* `idFaction` - The FactionID to check.

## Return Value
TRUE if the FactionID is a preconfigured faction, FALSE otherwise.

---

Visual
------

# SetAppearanceSEF\_xpAM
```cpp
void SetAppearanceSEF_xpAM(object oObject, string sSEF);
```
Set sSEF visual effect to oObject. Work for Creature, Item, Placeable, PlacedEffect.

## Parameters

* `oObject` - The object to set the visual effect for.
* `sSEF` - The visual effect to set.

---

# GetAppearanceSEF\_xpAM
```cpp
string GetAppearanceSEF_xpAM(object oObject);
```
Get the current visual effect applied to oObject. Work for Creature, Item, Placeable, PlacedEffect.

## Parameters

* `oObject` - The object to get the visual effect for.

## Return Value
The current visual effect applied to the object.

---

# SetColor\_xpAM
```cpp
void SetColor_xpAM(object oObject, int iColor, string sRGB);
```
Set the iColor color of oObject to sRGB. oObject: The object to change. Door or Placeable. iColor: 1, 2 or 3. The color to change. sRGB: color in #RRGGBB format.

## Parameters

* `oObject` - The object to set the color for.
* `iColor` - The color to change (1, 2, or 3).
* `sRGB` - The color in #RRGGBB format.

---

# GetColor\_xpAM
```cpp
string GetColor_xpAM(object oObject, int iColor);
```
Return the "iColor" color of oObject. In #RRGGBB format. oObject: a Door or Placeable object. iColor: 1, 2 or 3. The color to get.

## Parameters

* `oObject` - The object to get the color for.
* `iColor` - The color to get (1, 2, or 3).

## Return Value
The color in #RRGGBB format.

---

Position and facing
--------------------

# SetFacing\_xpAM
```cpp
void SetFacing_xpAM(object oObject, float fFacing);
```
Cause the oObject to immediately face fFacing. fFacing is expressed as anticlockwise degrees from Due East.

## Parameters

* `oObject` - The object to set the facing for.
* `fFacing` - The facing in anticlockwise degrees from Due East.

---

# SetPosition\_xpAM
```cpp
void SetPosition_xpAM(object oObject, float fX, float fY, float fZ);
```
Set the position of oObject.

## Parameters

* `oObject` - The object to set the position for.
* `fX`, `fY`, `fZ` - The coordinates of the new position.

---

# SetPositionAndFacing\_xpAM
```cpp
void SetPositionAndFacing_xpAM(object oObject, float fX, float fY, float fZ, float fFacing);
```
Set the position and orientation of oObject.

## Parameters

* `oObject` - The object to set the position and facing for.
* `fX`, `fY`, `fZ` - The coordinates of the new position.
* `fFacing` - The facing in anticlockwise degrees from Due East.

---

Music
-----

# MusicBackgroundDayForPC\_xpAM
```cpp
void MusicBackgroundDayForPC_xpAM(object oPC, int nTrack);
```
Change the current MusicBackgroundDay to nTrack for oPC in its current area.

## Parameters

* `oPC` - The PC to change the MusicBackgroundDay for.
* `nTrack` - The new MusicBackgroundDay track.

---

# MusicBackgroundNightForPC\_xpAM
```cpp
void MusicBackgroundNightForPC_xpAM(object oPC, int nTrack);
```
Change the current MusicBackgroundNight to nTrack for oPC in its current area.

## Parameters

* `oPC` - The PC to change the MusicBackgroundNight for.
* `nTrack` - The new MusicBackgroundNight track.

---

# MusicBattleForPC\_xpAM
```cpp
void MusicBattleForPC_xpAM(object oPC, int nTrack);
```
Change the current MusicBattle to nTrack for oPC in its current area.

## Parameters

* `oPC` - The PC to change the MusicBattle for.
* `nTrack` - The new MusicBattle track.

---

# MusicBackgroundRunForPC\_xpAM
```cpp
void MusicBackgroundRunForPC_xpAM(object oPC, int bStart);
```
Start or Stop the current MusicBackground for oPC.

## Parameters

* `oPC` - The PC to start or stop the MusicBackground for.
* `bStart` - The flag to start (TRUE) or stop (FALSE) the MusicBackground.

---

# MusicBattleRunForPC\_xpAM
```cpp
void MusicBattleRunForPC_xpAM(object oPC, int bStart);
```
Start or Stop the current MusicBattle for oPC.

## Parameters

* `oPC` - The PC to start or stop the MusicBattle for.
* `bStart` - The flag to start (TRUE) or stop (FALSE) the MusicBattle.

---

Refresh, View & Collision
-------------------

# RefreshCreatureForPC\_xpAM
```cpp
void RefreshCreatureForPC_xpAM(object oCreatureToRefresh, object oPCMustView);
```
Refresh the visual of oCreatureToRefresh for oPCMustView.

## Parameters

* `oCreatureToRefresh` - The creature to refresh the visual for.
* `oPCMustView` - The PC that must view the refreshed visual.

---

# RefreshCreature\_xpAM
```cpp
void RefreshCreature_xpAM(object oCreatureToRefresh);
```
Refresh the visual of oCreatureToRefresh for every PC.

## Parameters

* `oCreatureToRefresh` - The creature to refresh the visual for.

---

# RefreshEquipedItemForPC\_xpAM
```cpp
void RefreshEquipedItemForPC_xpAM(int iSlotToRefresh, object oCreatureToRefresh, object oMustView);
```
Refresh the visual of an equipped item of oCreatureToRefresh for oPCMustView. iSlotToRefresh: INVENTORY\_SLOT\_\* constant.

## Parameters

* `iSlotToRefresh` - The inventory slot to refresh the equipped item for (INVENTORY\_SLOT\_\* constant).
* `oCreatureToRefresh` - The creature to refresh the equipped item visual for.
* `oMustView` - The PC that must view the refreshed visual.

---

# RefreshEquipedItem\_xpAM
```cpp
void RefreshEquipedItem_xpAM(int iSlotToRefresh, object oCreatureToRefresh);
```
Refresh the visual of an equipped item of oCreatureToRefresh for every PC. iSlotToRefresh: INVENTORY\_SLOT\_\* constant.

## Parameters

* `iSlotToRefresh` - The inventory slot to refresh the equipped item for (INVENTORY\_SLOT\_\* constant).
* `oCreatureToRefresh` - The creature to refresh the equipped item visual for.

---

# RefreshObjectForPC\_xpAM
```cpp
void RefreshObjectForPC_xpAM(object oObjectToRefresh, object oPC);
```
Refresh the visual of the object oObjectToRefresh for oPC.

## Parameters

* `oObjectToRefresh` - The object to refresh the visual for.
* `oPC` - The PC that must view the refreshed visual.

---

# RefreshObject\_xpAM
```cpp
void RefreshObject_xpAM(object oObjectToRefresh);
```
Refresh the visual of the object oObjectToRefresh for every concerned.

## Parameters

* `oObjectToRefresh` - The object to refresh the visual for.

---

# ShowObject\_xpAM
```cpp
void ShowObject_xpAM(object oObjectToShow);
```
Show the visual of the object oObjectToShow.

## Parameters

* `oObjectToShow` - The object to show the visual for.

---

# ShowObjectForPC\_xpAM
```cpp
void ShowObjectForPC_xpAM(object oObjectToShow, object oPC);
```
Show the visual of the object oObjectToShow for oPC.

## Parameters

* `oObjectToShow` - The object to show the visual for.
* `oPC` - The PC that must view the shown visual.

---

# MaskObject\_xpAM
```cpp
void MaskObject_xpAM(object oObjectToMask);
```
Hide the visual of the object oObjectToMask for every concerned.

## Parameters

* `oObjectToMask` - The object to hide the visual for.

---

# MaskObjectForPC\_xpAM
```cpp
void MaskObjectForPC_xpAM(object oObjectToMask, object oPC);
```
Hide the visual of the object oObjectToMask for oPC.

## Parameters

* `oObjectToMask` - The object to hide the visual for.
* `oPC` - The PC that must view the hidden visual.

---

# SetObjectDynamicCollision\_xpAM
```cpp
void SetObjectDynamicCollision_xpAM(object oPlaceable, int bDynamicCollision);
```
Enable or Disable DynamicCollision of oPlaceable.

## Parameters

* `oPlaceable` - The placeable object to enable or disable DynamicCollision for.
* `bDynamicCollision` - The flag to enable (TRUE) or disable (FALSE) DynamicCollision.

---

# GetObjectDynamicCollision\_xpAM
```cpp
int GetObjectDynamicCollision_xpAM(object oPlaceable);
```
Get status of DynamicCollision of oPlaceable.

## Parameters

* `oPlaceable` - The placeable object to get the DynamicCollision status for.

## Return Value
The status of DynamicCollision for the object (TRUE if enabled, FALSE if disabled).

---

Constants
---------

# Light Color Types (LIGHT\_COLOR\_TYPE\_\*)

| Name | Value | Description |
| --- | --- | --- |
| LIGHT\_COLOR\_TYPE\_AMBIANT | "Ambiant" |  |
| LIGHT\_COLOR\_TYPE\_DIFFUSE | "Diffuse" |  |
| LIGHT\_COLOR\_TYPE\_SPECULAR | "Specular" |  |

# Transition Types (TRANSITION\_TYPE\_\*)

| Name | Value | Description |
| --- | --- | --- |
| TRANSITION\_TYPE\_NONE | 0 |  |
| TRANSITION\_TYPE\_DOOR | 1 |  |
| TRANSITION\_TYPE\_WAYPOINT | 2 |  |

# Flicker Types (FLICKER\_TYPE\_\*)

| Name | Value | Description |
| --- | --- | --- |
| FLICKER\_TYPE\_LINEAR | 0 |  |
| FLICKER\_TYPE\_BOUNCE | 1 |  |
| FLICKER\_TYPE\_JUMPY | 2 |  |

# Armor Model Parts (ARMOR\_MODEL\_PART\_*)

| Name | Value | Description |
| --- | --- | --- |
| ARMOR\_MODEL\_PART\_HELM | 0 | Already existing in base nwnscript |
| ARMOR\_MODEL\_PART\_GLOVES | 1 | Already existing in base nwnscript |
| ARMOR\_MODEL\_PART\_BOOTS | 2 | Already existing in base nwnscript |
| ARMOR\_MODEL\_PART\_BELT | 3 | Already existing in base nwnscript |
| ARMOR\_MODEL\_PART\_CLOAK | 4 | Already existing in base nwnscript |
| ARMOR\_MODEL\_PART\_ARMOR | 5 | Added by xp_aspectManager |

# Trigger Types (TRIGGER\_TYPE\_\*)

| Name | Value | Description |
| --- | --- | --- |
| TRIGGER\_TYPE\_GENERAL | 0 |  |
| TRIGGER\_TYPE\_TRANSITION | 1 |  |
| TRIGGER\_TYPE\_TRAP | 2 |  |

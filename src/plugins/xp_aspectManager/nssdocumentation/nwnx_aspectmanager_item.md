xp_aspectManager_item.nss
====

- [SetBaseItemType\_xpAM](#setbaseitemtype_xpam)
- [SetItemAppearanceVariation\_xpAM](#setitemappearancevariation_xpam)
- [GetItemAppearanceVariation\_xpAM](#getitemappearancevariation_xpam)
- [SetItemAppearanceCategory\_xpAM](#setitemappearancecategory_xpam)
- [GetItemAppearanceCategory\_xpAM](#getitemappearancecategory_xpam)
- [SetItemDescription\_xpAM](#setitemdescription_xpam)
- [GetItemDescription\_xpAM](#getitemdescription_xpam)
- [SetItemColor\_xpAM](#setitemcolor_xpam)
- [GetItemColor\_xpAM](#getitemcolor_xpam)
- [__WeaponSpecific__](#weaponspecific)
  - [SetItemWeaponFX\_xpAM](#setitemweaponfx_xpam)
  - [GetItemWeaponFX\_xpAM](#getitemweaponfx_xpam)
  - [SetItemWeaponPart\_xpAM](#setitemweaponpart_xpam)
  - [GetItemWeaponPart\_xpAM](#getitemweaponpart_xpam)
- [__ModelPiece__](#modelpiece)
  - [SetItemModelPieceVariation\_xpAM](#setitemmodelpiecevariation_xpam)
  - [GetItemModelPieceVariation\_xpAM](#getitemmodelpiecevariation_xpam)
  - [SetItemModelPieceColor\_xpAM](#setitemmodelpiececolor_xpam)
  - [GetItemModelPieceColor\_xpAM](#getitemmodelpiececolor_xpam)
  - [SetItemModelPieceIsUVScroll\_xpAM](#setitemmodelpieceisuvscroll_xpam)
  - [GetItemModelPieceIsUVScroll\_xpAM](#getitemmodelpieceisuvscroll_xpam)
  - [SetItemModelPieceUScroll\_xpAM](#setitemmodelpieceuscroll_xpam)
  - [GetItemModelPieceUScroll\_xpAM](#getitemmodelpieceuscroll_xpam)
  - [SetItemModelPieceVScroll\_xpAM](#setitemmodelpiecevscroll_xpam)
  - [GetItemModelPieceVScroll\_xpAM](#getitemmodelpiecevscroll_xpam)
- [__ModelPart__](#modelpart)
  - [SetItemModelPartVariation\_xpAM](#setitemmodelpartvariation_xpam)
  - [GetItemModelPartVariation\_xpAM](#getitemmodelpartvariation_xpam)
  - [SetItemModelPartCategory\_xpAM](#setitemmodelpartcategory_xpam)
  - [GetItemModelPartCategory\_xpAM](#getitemmodelpartcategory_xpam)
  - [SetItemModelPartColor\_xpAM](#setitemmodelpartcolor_xpam)
  - [GetItemModelPartColor\_xpAM](#getitemmodelpartcolor_xpam)

---

# SetBaseItemType\_xpAM

```cpp
void SetBaseItemType_xpAM(
    object oItem,
    int iItemType
);
```
Set the base item type (BASE\_ITEM\_* or Baseitems.2da ID) of oItem.

## Parameters

* `oItem` - The item to set the base type for
* `iItemType` - The base item type to set

---

# SetItemAppearanceVariation\_xpAM

```cpp
void SetItemAppearanceVariation_xpAM(
    object oItem,
    int iVariation
);
```
Set the Visual Variation of oItem

## Parameters

* `oItem` - The item to set the visual variation for
* `iVariation` - The visual variation to set

---

# GetItemAppearanceVariation\_xpAM

```cpp
int GetItemAppearanceVariation_xpAM(
    object oItem
);
```
Get the Visual Variation of oItem

## Parameters

* `oItem` - The item to get the visual variation for

## Return Value

The visual variation of oItem

---

# SetItemAppearanceCategory\_xpAM

```cpp
void SetItemAppearanceCategory_xpAM(
    object oItem,
    int iCategory
);
```
Set the Visual Category of oItem (armorvisualdata.2DA ID)

## Parameters

* `oItem` - The item to set the visual category for
* `iCategory` - The visual category to set

---

# GetItemAppearanceCategory\_xpAM

```cpp
int GetItemAppearanceCategory_xpAM(
    object oItem
);
```
Get the Visual Category of oItem (armorvisualdata.2DA ID)

## Parameters

* `oItem` - The item to get the visual category for

## Return Value

The visual category of oItem

---

# SetItemDescription\_xpAM

```cpp
void SetItemDescription_xpAM(
    object oItem,
    string sDescription,
    int bIdentified = TRUE
);
```
Set the Description of oItem.

## Parameters

* `oItem` - The item to set the description for
* `sDescription` - The description to set
* `bIdentified` - If TRUE, set the identified description. If FALSE, set the non-identified description.

---

# GetItemDescription\_xpAM

```cpp
string GetItemDescription_xpAM(
    object oItem,
    int bIdentified = TRUE
);
```
Get the Description of oItem.

## Parameters

* `oItem` - The item to get the description for
* `bIdentified` - If TRUE, get the identified description. If FALSE, get the non-identified description.

## Return Value

The description of oItem

---

# SetItemColor\_xpAM

```cpp
void SetItemColor_xpAM(
    object oItem,
    int iColor,
    string sRGB
);
```
Set the oItem Color.

## Parameters

* `oItem` - The item to set the color for
* `iColor` - 1, 2 or 3
* `sRGB` - A RGB string value, on the "#FFFFFF" (or #FFFFFFFF) format

---

# GetItemColor\_xpAM

```cpp
string GetItemColor_xpAM(
    object oItem,
    int iColor
);
```
Get the oItem Color.

## Parameters

* `oItem` - The item to get the color for
* `iColor` - 1, 2 or 3

## Return Value

The color of oItem in RGB string format

WeaponSpecific
----

---

# SetItemWeaponFX\_xpAM

```cpp
void SetItemWeaponFX_xpAM(
    object oItem,
    int iWpnFX
);
```
Set the "DamageEffect" of the Weapon (ITEM\_VISUAL\_* / iprp\_visualfx.2da)

## Parameters

* `oItem` - The weapon to set the damage effect for
* `iWpnFX` - The damage effect to set

---

# GetItemWeaponFX\_xpAM

```cpp
int GetItemWeaponFX_xpAM(
    object oItem
);
```
Get the "DamageEffect" of the Weapon (ITEM\_VISUAL\_* / iprp\_visualfx.2da)

## Parameters

* `oItem` - The weapon to get the damage effect for

## Return Value

The damage effect of the weapon

---

# SetItemWeaponPart\_xpAM

```cpp
void SetItemWeaponPart_xpAM(
    object oItem,
    int iPart,
    int iVariation
);
```
Set the iPart (WEAPON\_MODEL\_PART\_*) of oItem to Variation

## Parameters

* `oItem` - The weapon to set the part variation for
* `iPart` - The part to set the variation for (WEAPON\_MODEL\_PART\_* constant)
* `iVariation` - The variation to set

---

# GetItemWeaponPart\_xpAM

```cpp
int GetItemWeaponPart_xpAM(
    object oItem,
    int iPart
);
```
Get the Variation of the iPart (WEAPON\_MODEL\_PART\_*) of oItem

## Parameters

* `oItem` - The weapon to get the part variation for
* `iPart` - The part to get the variation for (WEAPON\_MODEL\_PART\_* constant)

## Return Value

The variation of the part

ModelPiece
----

---

# SetItemModelPieceVariation\_xpAM

```cpp
void SetItemModelPieceVariation_xpAM(
    object oItem,
    int iModelPiece,
    int iVariation
);
```
Set the ModelPiece Variation for oItem.

## Parameters

* `oItem` - The item to set the model piece variation for
* `iModelPiece` - The model piece to set the variation for  (ARMOR\_MODEL\_PIECE\_* constant)
* `iVariation` - The variation to set

---

# GetItemModelPieceVariation\_xpAM

```cpp
int GetItemModelPieceVariation_xpAM(
    object oItem,
    int iModelPiece
);
```
Get the ModelPiece Variation for oItem.

## Parameters

* `oItem` - The item to get the model piece variation for 
* `iModelPiece` - The model piece to get the variation for  (ARMOR\_MODEL\_PIECE\_* constant)

## Return Value

The variation of the model piece

---

# SetItemModelPieceColor\_xpAM

```cpp
void SetItemModelPieceColor_xpAM(
    object oItem,
    int iModelPiece,
    int iColor,
    string sRGB
);
```
Set the Color of the iModelPiece of oItem

## Parameters

* `oItem` - The item to set the model piece color for
* `iModelPiece` - The model piece to set the color for (ARMOR\_MODEL\_PIECE\_* constant)
* `iColor` - 1, 2 or 3
* `sRGB` - A RGB string value, on the "#FFFFFF" (or #FFFFFFFF) format

---

# GetItemModelPieceColor\_xpAM

```cpp
string GetItemModelPieceColor_xpAM(
    object oItem,
    int iModelPiece,
    int iColor
);
```
Get the Color of the iModelPiece of oItem

## Parameters

* `oItem` - The item to get the model piece color for
* `iModelPiece` - The model piece to get the color for (ARMOR\_MODEL\_PIECE\_* constant)
* `iColor` - 1, 2 or 3

## Return Value

The color of the model piece in RGB string format

---

# SetItemModelPieceIsUVScroll\_xpAM

```cpp
void SetItemModelPieceIsUVScroll_xpAM(
    object oItem,
    int iModelPiece,
    int bUVScroll
);
```
Enable or Disable the UVScroll of the iModelPiece of oItem

## Parameters

* `oItem` - The item to enable or disable the UVScroll for
* `iModelPiece` - The model piece to enable or disable the UVScroll for (ARMOR\_MODEL\_PIECE\_* constant)
* `bUVScroll` - TRUE to enable UVScroll, FALSE to disable UVScroll

---

# GetItemModelPieceIsUVScroll\_xpAM

```cpp
int GetItemModelPieceIsUVScroll_xpAM(
    object oItem,
    int iModelPiece
);
```
Get the activation status for the UVScroll of the iModelPiece of oItem

## Parameters

* `oItem` - The item to get the UVScroll activation status for
* `iModelPiece` - The model piece to get the UVScroll activation status for (ARMOR\_MODEL\_PIECE\_* constant)

## Return Value

TRUE if UVScroll is enabled, FALSE if UVScroll is disabled

---

# SetItemModelPieceUScroll\_xpAM

```cpp
void SetItemModelPieceUScroll_xpAM(
    object oItem,
    int iModelPiece,
    float fUScroll
);
```
Set the UScroll speed for the iModelPiece of oItem

## Parameters

* `oItem` - The item to set the UScroll speed for
* `iModelPiece` - The model piece to set the UScroll speed for (ARMOR\_MODEL\_PIECE\_* constant)
* `fUScroll` - The UScroll speed to set

---

# GetItemModelPieceUScroll\_xpAM

```cpp
float GetItemModelPieceUScroll_xpAM(
    object oItem,
    int iModelPiece
);
```
Get the UScroll speed for the iModelPiece of oItem

## Parameters

* `oItem` - The item to get the UScroll speed for
* `iModelPiece` - The model piece to get the UScroll speed for (ARMOR\_MODEL\_PIECE\_* constant)

## Return Value

The UScroll speed of the model piece

---

# SetItemModelPieceVScroll\_xpAM

```cpp
void SetItemModelPieceVScroll_xpAM(
    object oItem,
    int iModelPiece,
    float fVScroll
);
```
Set the VScroll speed for the iModelPiece of oItem

## Parameters

* `oItem` - The item to set the VScroll speed for
* `iModelPiece` - The model piece to set the VScroll speed for (ARMOR\_MODEL\_PIECE\_* constant)
* `fVScroll` - The VScroll speed to set

---

# GetItemModelPieceVScroll\_xpAM

```cpp
float GetItemModelPieceVScroll_xpAM(
    object oItem,
    int iModelPiece
);
```
Get the VScroll speed for the iModelPiece of oItem

## Parameters

* `oItem` - The item to get the VScroll speed for
* `iModelPiece` - The model piece to get the VScroll speed for (ARMOR\_MODEL\_PIECE\_* constant)

## Return Value

The VScroll speed of the model piece

ModelPart
----

---

# SetItemModelPartVariation\_xpAM

```cpp
void SetItemModelPartVariation_xpAM(
    object oItem,
    int iModelPart,
    int iVariation
);
```
Set the ModelPart Variation for oItem.

## Parameters

* `oItem` - The item to set the model part variation for
* `iModelPart` - The model part to set the variation for (ARMOR\_MODEL\_PART\_* constant)
* `iVariation` - The variation to set

---

# GetItemModelPartVariation\_xpAM

```cpp
int GetItemModelPartVariation_xpAM(
    object oItem,
    int iModelPart
);
```
Get the ModelPart Variation for oItem.

## Parameters

* `oItem` - The item to get the model part variation for
* `iModelPart` - The model part to get the variation for (ARMOR\_MODEL\_PART\_* constant)

## Return Value

The variation of the model part

---

# SetItemModelPartCategory\_xpAM

```cpp
void SetItemModelPartCategory_xpAM(
    object oItem,
    int iModelPart,
    int iCategory
);
```
Set the ModelPart Visual Category for oItem (armorvisualdata.2DA ID).

## Parameters

* `oItem` - The item to set the model part visual category for
* `iModelPart` - The model part to set the visual category for (ARMOR\_MODEL\_PART\_* constant)
* `iCategory` - The visual category to set

---

# GetItemModelPartCategory\_xpAM

```cpp
int GetItemModelPartCategory_xpAM(
    object oItem,
    int iModelPart
);
```
Get the ModelPart Visual Category for oItem (armorvisualdata.2DA ID).

## Parameters

* `oItem` - The item to get the model part visual category for
* `iModelPart` - The model part to get the visual category for (ARMOR\_MODEL\_PART\_* constant)

## Return Value

The visual category of the model part

---

# SetItemModelPartColor\_xpAM

```cpp
void SetItemModelPartColor_xpAM(
    object oItem,
    int iModelPart,
    int iColor,
    string sRGB
);
```
Set the Color of the iModelPart of oItem

## Parameters

* `oItem` - The item to set the model part color for
* `iModelPart` - The model part to set the color for  (ARMOR\_MODEL\_PART\_* constant)
* `iColor` - 1, 2 or 3
* `sRGB` - A RGB string value, on the "#FFFFFF" (or #FFFFFFFF) format

---

# GetItemModelPartColor\_xpAM

```cpp
string GetItemModelPartColor_xpAM(
    object oItem,
    int iModelPart,
    int iColor
);
```
Get the Color of the iModelPart of oItem

## Parameters

* `oItem` - The item to get the model part color for
* `iModelPart` - The model part to get the color for  (ARMOR\_MODEL\_PART\_* constant)
* `iColor` - 1, 2 or 3

## Return Value

The color of the model part in RGB string format

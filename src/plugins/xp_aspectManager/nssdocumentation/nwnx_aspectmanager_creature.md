xp_aspectManager_creature.nss
===


- [__Global Note__](#global-note)
- [SetCreatureAge\_xpAM](#setcreatureage_xpam)
- [GetCreatureAge\_xpAM](#getcreatureage_xpam)
- [SetCreatureSoundSet\_xpAM](#setcreaturesoundset_xpam)
- [GetCreatureSoundSet\_xpAM](#getcreaturesoundset_xpam)
- [SetCreatureSpecificAC\_xpAM](#setcreaturespecificac_xpam)
- [GetCreatureSpecificAC\_xpAM](#getcreaturespecificac_xpam)
- [SetCreatureTattoos\_xpAM](#setcreaturetattoos_xpam)
- [GetCreatureTattoos\_xpAM](#getcreaturetattoos_xpam)
- [__Creature Appearance__](#creature-appearance)
  - [SetCreatureVisualGender\_xpAM](#setcreaturevisualgender_xpam)
  - [GetCreatureVisualGender\_xpAM](#getcreaturevisualgender_xpam)
  - [SetCreatureHead\_xpAM](#setcreaturehead_xpam)
  - [GetCreatureHead\_xpAM](#getcreaturehead_xpam)
  - [SetCreatureHair\_xpAM](#setcreaturehair_xpam)
  - [GetCreatureHair\_xpAM](#getcreaturehair_xpam)
  - [SetCreatureFacialHair\_xpAM](#setcreaturefacialhair_xpam)
  - [GetCreatureFacialHair\_xpAM](#getcreaturefacialhair_xpam)
  - [SetCreatureTail\_xpAM](#setcreaturetail_xpam)
  - [GetCreatureTail\_xpAM](#getcreaturetail_xpam)
  - [SetCreatureWings\_xpAM](#setcreaturewings_xpam)
  - [GetCreatureWings\_xpAM](#getcreaturewings_xpam)
  - [SetCreatureColor\_xpAM](#setcreaturecolor_xpam)
  - [GetCreatureColor\_xpAM](#getcreaturecolor_xpam)
  - [SetCreaturePersistScale\_xpAM](#setcreaturepersistscale_xpam)
  - [GetCreaturePersistScale\_xpAM](#getcreaturepersistscale_xpam)
- [__Creature Visual__](#creature-visual)
  - [SetCreatureModelPartMask\_xpAM](#setcreaturemodelpartmask_xpam)
  - [GetCreatureModelPartMask\_xpAM](#getcreaturemodelpartmask_xpam)
  - [SetCreatureVisualVariation\_xpAM](#setcreaturevisualvariation_xpam)
  - [GetCreatureVisualVariation\_xpAM](#getcreaturevisualvariation_xpam)
  - [SetCreatureVisualCategory\_xpAM](#setcreaturevisualcategory_xpam)
  - [GetCreatureVisualCategory\_xpAM](#getcreaturevisualcategory_xpam)
  - [SetCreatureVisualColor\_xpAM](#setcreaturevisualcolor_xpam)
  - [GetCreatureVisualColor\_xpAM](#getcreaturevisualcolor_xpam)
  - [__Visual ModelPart__](#visual-modelpart)
    - [SetCreatureVisualModelPartVariation\_xpAM](#setcreaturevisualmodelpartvariation_xpam)
    - [GetCreatureVisualModelPartVariation\_xpAM](#getcreaturevisualmodelpartvariation_xpam)
    - [SetCreatureVisualModelPartCategory\_xpAM](#setcreaturevisualmodelpartcategory_xpam)
    - [GetCreatureVisualModelPartCategory\_xpAM](#getcreaturevisualmodelpartcategory_xpam)
    - [SetCreatureVisualModelPartColor\_xpAM](#setcreaturevisualmodelpartcolor_xpam)
    - [GetCreatureVisualModelPartColor\_xpAM](#getcreaturevisualmodelpartcolor_xpam)
  - [__Visual ModelPiece__](#visual-modelpiece)
    - [SetCreatureVisualModelPieceVariation\_xpAM](#setcreaturevisualmodelpiecevariation_xpam)
    - [GetCreatureVisualModelPieceVariation\_xpAM](#getcreaturevisualmodelpiecevariation_xpam)
    - [SetCreatureVisualModelPieceColor\_xpAM](#setcreaturevisualmodelpiececolor_xpam)
    - [GetCreatureVisualModelPieceColor\_xpAM](#getcreaturevisualmodelpiececolor_xpam)
    - [SetCreatureVisualModelPieceIsUVScroll\_xpAM](#setcreaturevisualmodelpieceisuvscroll_xpam)
    - [GetCreatureVisualModelPieceIsUVScroll\_xpAM](#getcreaturevisualmodelpieceisuvscroll_xpam)
    - [SetCreatureVisualModelPieceUScroll\_xpAM](#setcreaturevisualmodelpieceuscroll_xpam)
    - [GetCreatureVisualModelPieceUScroll\_xpAM](#getcreaturevisualmodelpieceuscroll_xpam)
    - [SetCreatureVisualModelPieceVScroll\_xpAM](#setcreaturevisualmodelpiecevscroll_xpam)
    - [GetCreatureVisualModelPieceVScroll\_xpAM](#getcreaturevisualmodelpiecevscroll_xpam)
- [__Constants__](#constants)
  - [Creature Color Constants (CREATURE\_COLOR\_*)](#creature-color-constants-creature_color_)
  - [Armor Class Type Constants (AC\_TYPE\_*)](#armor-class-type-constants-ac_type_)
  - [Scale Axis Constants (SCALE\_*)](#scale-axis-constants-scale_)

# Global Note
>
> For some parts, Creatures use a set of two values. One is used for the current usage (e.g., the current Hair shown), while the other one represents the "persistent" value, which will be the only one exported/saved.
>
> To allow you to choose which one to change, the relevant function has a `bPersist` parameter. Set it to `FALSE` to change the "current" value, and to `TRUE` to change the persisted one.
>
> **Quick example:**
>
> - My oPC has the head value set to 4. Therefore, both the persistent and current head values are set to 4.
>     - I call `SetCreatureHead_xpAM(oPC, 5, FALSE)`. => The head of PC changes to 5.
>     - The PC leaves the server and rejoins => the head will be back to 4.
>     - I call `SetCreatureHead_xpAM(oPC, 5, TRUE)` => nothing visible happens.
>     - The PC leaves the server and rejoins again => the change is saved, new head is 5.
>
> **Note:** Function without the `bPersist` parameter will impact "unique" value, that will affect the current AND be persisted.


---

# SetCreatureAge\_xpAM

```cpp
void SetCreatureAge_xpAM(
    object oCreature,
    int iAge,
    int bPersist = FALSE
);
```
Set the Age of oCreature

## Parameters

* `oCreature` - The creature to set the age for
* `iAge` - The age to set
* `bPersist` - Set to TRUE to change the persisted value, FALSE to change the current value.

---

# GetCreatureAge\_xpAM

```cpp
int GetCreatureAge_xpAM(
    object oCreature,
    int bPersist = FALSE
);
```
Get the Age of oCreature

## Parameters

* `oCreature` - The creature to get the age for
* `bPersist` - Set to TRUE to get the persisted value, FALSE to get the current value.

## Return Value

The age of the creature.

---

# SetCreatureSoundSet\_xpAM

```cpp
void SetCreatureSoundSet_xpAM(
    object oCreature,
    int iSoundSet
);
```
Set the SoundSet of oCreature

## Parameters

* `oCreature` - The creature to set the sound set for
* `iSoundSet` - The sound set to use

---

# GetCreatureSoundSet\_xpAM

```cpp
int GetCreatureSoundSet_xpAM(
    object oCreature
);
```
Get the SoundSet of oCreature

## Parameters

* `oCreature` - The creature to get the sound set for

## Return Value

The sound set of the creature.

---

# SetCreatureSpecificAC\_xpAM

```cpp
void SetCreatureSpecificAC_xpAM(
    object oCreature,
    int iACType,
    int iValue,
    int bReduce = FALSE
);
```
Set the Specific Armor Class (AC) of oCreature

## Parameters

* `oCreature` - The creature to set the AC for
* `iACType` - ([AC\_TYPE\_*](#armor-class-type-constants-ac_type_)) constant representing the AC to be set.
* `iValue` - a number between -128 and +127 that determines the new value of the AC
* `bReduce` - Set to TRUE to decrease the AC, FALSE to increase it
  -  All AC\_TYPE except BASE, ARMORVAL, SHIELDVAL have a "ReduceValue".
  - Positive `iValue` will increase the chosen AC if `bReduce=FALSE`, and decrease it if `bReduce=TRUE`.
  - Negative values will decrease the chosen AC if `bReduce=FALSE`, but increase it if `bReduce=TRUE`.

## Note
If you set a specific AC, it will be overwritten by other factors that also set the same AC. 
For example, setting AC_TYPE_ARMORVAL_SEPT to 18 will be overwritten when the oCreature equips armor. 
Another example: when a creature receives Mage Armor, the AC_TYPE_ARMOR_SEPT will be overwritten but NOT the reduced AC_TYPE_ARMOR_SEPT.

---

# GetCreatureSpecificAC\_xpAM

```cpp
int GetCreatureSpecificAC_xpAM(
    object oCreature,
    int iACType,
    int bReduceVal = FALSE
);
```
Get the Specific Armor Class (AC) of oCreature

## Parameters

* `oCreature` - The creature to get the AC for
* `iACType` - ([AC\_TYPE\_*](#armor-class-type-constants-ac_type_)) constant representing the AC to be get.
* `bReduceVal` - Set to TRUE to get the reduced value, FALSE to get the normal value
  All AC_TYPE except BASE, ARMORVAL, SHIELDVAL have a "ReduceValue".

## Return Value

The specific armor class (AC) of the creature.

---

# SetCreatureTattoos\_xpAM

```cpp
void SetCreatureTattoos_xpAM(
    object oCreature,
    int iValue
);
```
Not working for tattoo but can be used to store a 16bits value.

## Parameters

* `oCreature` - The creature to store the value for
* `iValue` - The 16bits value to store

---

# GetCreatureTattoos\_xpAM

```cpp
int GetCreatureTattoos_xpAM(
    object oCreature
);
```
Not working for tattoo but can be used to get a stored 16bits value.

## Parameters

* `oCreature` - The creature to get the value for

## Return Value

The stored 16bits value.

---

Creature Appearance
---

---

# SetCreatureVisualGender\_xpAM

```cpp
void SetCreatureVisualGender_xpAM(
    object oCreature,
    int iVisualGender
);
```
Needed if you change the Gender of an creature

## Parameters

* `oCreature` - The creature to set the visual gender for
* `iVisualGender` - The visual gender to set

---

# GetCreatureVisualGender\_xpAM

```cpp
int GetCreatureVisualGender_xpAM(
    object oCreature
);
```
Get the visual gender of a creature

## Parameters

* `oCreature` - The creature to get the visual gender for

## Return Value

The visual gender of the creature.

---

# SetCreatureHead\_xpAM

```cpp
void SetCreatureHead_xpAM(
    object oCreature,
    int iVariation,
    int bPersist = FALSE
);
```
Change the Head of oCreature.

## Parameters

* `oCreature` - The creature to change the head for
* `iVariation` - The head variation to use
* `bPersist` - Set to TRUE to change the persisted value, FALSE to change the current value.

---

# GetCreatureHead\_xpAM

```cpp
int GetCreatureHead_xpAM(
    object oCreature,
    int bPersist = FALSE
);
```
Get the Head of oCreature

## Parameters

* `oCreature` - The creature to get the head for
* `bPersist` - Set to TRUE to get the persisted value, FALSE to get the current value.

## Return Value

The head variation of the creature.

---

# SetCreatureHair\_xpAM

```cpp
void SetCreatureHair_xpAM(
    object oCreature,
    int iVariation,
    int bPersist = FALSE
);
```
Change the Hair of oCreature.

## Parameters

* `oCreature` - The creature to change the hair for
* `iVariation` - The hair variation to use
* `bPersist` - Set to TRUE to change the persisted value, FALSE to change the current value.

---

# GetCreatureHair\_xpAM

```cpp
int GetCreatureHair_xpAM(
    object oCreature,
    int bPersist = FALSE
);
```
Get the Hair of oCreature

## Parameters

* `oCreature` - The creature to get the hair for
* `bPersist` - Set to TRUE to get the persisted value, FALSE to get the current value.

## Return Value

The hair variation of the creature.

---

# SetCreatureFacialHair\_xpAM

```cpp
void SetCreatureFacialHair_xpAM(
    object oCreature,
    int iVariation,
    int bPersist = FALSE
);
```
Change the Facial Hair of oCreature.

## Parameters

* `oCreature` - The creature to change the facial hair for
* `iVariation` - The facial hair variation to use
* `bPersist` - Set to TRUE to change the persisted value, FALSE to change the current value.

---

# GetCreatureFacialHair\_xpAM

```cpp
int GetCreatureFacialHair_xpAM(
    object oCreature,
    int bPersist = FALSE
);
```
Get the Facial Hair of oCreature

## Parameters

* `oCreature` - The creature to get the facial hair for
* `bPersist` - Set to TRUE to get the persisted value, FALSE to get the current value.

## Return Value

The facial hair variation of the creature.

---

# SetCreatureTail\_xpAM

```cpp
void SetCreatureTail_xpAM(
    object oCreature,
    int iVariation,
    int bPersist = FALSE
);
```
Change the Tail of oCreature.

## Parameters

* `oCreature` - The creature to change the tail for
* `iVariation` - The tail variation to use
* `bPersist` - Set to TRUE to change the persisted value, FALSE to change the current value.

---

# GetCreatureTail\_xpAM

```cpp
int GetCreatureTail_xpAM(
    object oCreature,
    int bPersist = FALSE
);
```
Get the Tail of oCreature

## Parameters

* `oCreature` - The creature to get the tail for
* `bPersist` - Set to TRUE to get the persisted value, FALSE to get the current value.

## Return Value

The tail variation of the creature.

---

# SetCreatureWings\_xpAM

```cpp
void SetCreatureWings_xpAM(
    object oCreature,
    int iVariation,
    int bPersist = FALSE
);
```
Change the Wings of oCreature.

## Parameters

* `oCreature` - The creature to change the wings for
* `iVariation` - The wings variation to use
* `bPersist` - Set to TRUE to change the persisted value, FALSE to change the current value.

---

# GetCreatureWings\_xpAM

```cpp
int GetCreatureWings_xpAM(
    object oCreature,
    int bPersist = FALSE
);
```
Get the Wings of oCreature

## Parameters

* `oCreature` - The creature to get the wings for
* `bPersist` - Set to TRUE to get the persisted value, FALSE to get the current value.

## Return Value

The wings variation of the creature.

---

# SetCreatureColor\_xpAM

```cpp
void SetCreatureColor_xpAM(
    object oCreature,
    string sCreatureColor,
    string sRGB,
    int bPersist = FALSE
);
```
Change the choosen color of oCreature.

## Parameters

* `oCreature` - The creature to change the color for
* `sCreatureColor` - [CREATURE\_COLOR\_*](#creature-color-constants-creature_color_) constant, the choosen color
* `sRGB` - a RGB string value, on the "#FFFFFF" (or #FFFFFFFF) format
* `bPersist` - Set to TRUE to change the persisted value, FALSE to change the current value.

---

# GetCreatureColor\_xpAM

```cpp
string GetCreatureColor_xpAM(
    object oCreature,
    string sCreatureColor,
    int bPersist = FALSE
);
```
Get the choosen color of oCreature.

## Parameters

* `oCreature` - The creature to get the color for
* `sCreatureColor` - [CREATURE\_COLOR\_*](#creature-color-constants-creature_color_) constant, the choosen color
* `bPersist` - Set to TRUE to get the persisted value, FALSE to get the current value.

## Return Value

The RGB string value of the chosen color.

---

# SetCreaturePersistScale\_xpAM

```cpp
void SetCreaturePersistScale_xpAM(
    object oCreature,
    int nAxis,
    float fScale
);
```
Change the Persisted scale of oCreature based on specific nAxis.

## Parameters

* `oCreature` - The creature to change the scale for
* `nAxis` - [SCALE\_*](#scale-axis-constants-scale_) constant, the scale axis to set.
* `fScale` - The scale value to use

---

# GetCreaturePersistScale\_xpAM

```cpp
float GetCreaturePersistScale_xpAM(
    object oCreature,
    int nAxis
);
```
Get the Persisted scale of the object based on which axis is looked up.

## Parameters

* `oCreature` - The creature to get the scale for
* `nAxis` - [SCALE\_*](#scale-axis-constants-scale_) constant, the scale axis to get.

## Return Value

The scale value of the creature on the specified axis.

---

Creature Visual
---

---

# SetCreatureModelPartMask\_xpAM

```cpp
void SetCreatureModelPartMask_xpAM(
    object oCreature,
    int iMask
);
```
Change the iModelPart mask, allowing you to show or hide model part on the creature visual (so without armor)

## Parameters

* `oCreature` - The creature to change the model part mask for
* `iMask` - The model part mask to use

## Note
iMask is bitmask based with : 
0x1: Helm, 0x2: Gloves,  0x4: Boots, 0x8: Belt, 0x10: Cloak

---

# GetCreatureModelPartMask\_xpAM

```cpp
int GetCreatureModelPartMask_xpAM(
    object oCreature
);
```
Get the iModelPart mask, tell you to which model part are shown or hiden on the creature visual (so without armor)

## Parameters

* `oCreature` - The creature to get the model part mask for

## Return Value

The model part mask of the creature.

## Note
Return Value is bitmask based with : 
0x1: Helm, 0x2: Gloves,  0x4: Boots, 0x8: Belt, 0x10: Cloak

---

# SetCreatureVisualVariation\_xpAM

```cpp
void SetCreatureVisualVariation_xpAM(
    object oCreature,
    int iVariation
);
```
Set the iVariation for the oCreature model (without armor)

## Parameters

* `oCreature` - The creature to change the model variation for
* `iVariation` - The model variation to use

---

# GetCreatureVisualVariation\_xpAM

```cpp
int GetCreatureVisualVariation_xpAM(
    object oCreature
);
```
Get the iVariation for the oCreature model (without armor)

## Parameters

* `oCreature` - The creature to get the model variation for

## Return Value

The model variation of the creature.

---

# SetCreatureVisualCategory\_xpAM

```cpp
void SetCreatureVisualCategory_xpAM(
    object oCreature,
    int iCategory
);
```
Set the Category (armorvisualdata.2da) for the oCreature model (without armor)

## Parameters

* `oCreature` - The creature to change the model category for
* `iCategory` - The model category to use

---

# GetCreatureVisualCategory\_xpAM

```cpp
int GetCreatureVisualCategory_xpAM(
    object oCreature
);
```
Get the Category (armorvisualdata.2da) for the oCreature model (without armor)

## Parameters

* `oCreature` - The creature to get the model category for

## Return Value

The model category of the creature.

---

# SetCreatureVisualColor\_xpAM

```cpp
void SetCreatureVisualColor_xpAM(
    object oCreature,
    int iColor,
    string sRGB
);
```
Set the visual Color for oCreature (Seems to be persisted but not used)

## Parameters

* `oCreature` - The creature to change the visual color for
* `iColor` - The color to use
* `sRGB` - a RGB string value, on the "#FFFFFF" (or #FFFFFFFF) format

---

# GetCreatureVisualColor\_xpAM

```cpp
string GetCreatureVisualColor_xpAM(
    object oCreature,
    int iColor
);
```
Get the visual Color for oCreature (Seems to be persisted but not used)

## Parameters

* `oCreature` - The creature to get the visual color for
* `iColor` - The color to get

## Return Value

The RGB string value of the visual color.

---

Visual ModelPart
---

---

# SetCreatureVisualModelPartVariation\_xpAM

```cpp
void SetCreatureVisualModelPartVariation_xpAM(
    object oCreature,
    int iModelPart,
    int iVariation
);
```
Set the ModelPart Variation for oCreature Visual.

## Parameters

* `oCreature` - The creature to change the model part variation for
* `iModelPart` - ARMOR\_MODEL\_PART\_* constant
* `iVariation` - The model part variation to use

---

# GetCreatureVisualModelPartVariation\_xpAM

```cpp
int GetCreatureVisualModelPartVariation_xpAM(
    object oCreature,
    int iModelPart
);
```
Get the ModelPart Variation for oCreature Visual.

## Parameters

* `oCreature` - The creature to get the model part variation for
* `iModelPart` - ARMOR\_MODEL\_PART\_* constant

## Return Value

The model part variation of the creature.

---

# SetCreatureVisualModelPartCategory\_xpAM

```cpp
void SetCreatureVisualModelPartCategory_xpAM(
    object oCreature,
    int iModelPart,
    int iCategory
);
```
Set the ModelPart Visual Category for oCreature Visual (armorvisualdata.2DA ID).

## Parameters

* `oCreature` - The creature to change the model part category for
* `iModelPart` - ARMOR\_MODEL\_PART\_* constant
* `iCategory` - The model part category to use

---

# GetCreatureVisualModelPartCategory\_xpAM

```cpp
int GetCreatureVisualModelPartCategory_xpAM(
    object oCreature,
    int iModelPart
);
```
Get the ModelPart Visual Category for oCreature Visual (armorvisualdata.2DA ID).

## Parameters

* `oCreature` - The creature to get the model part category for
* `iModelPart` - ARMOR\_MODEL\_PART\_* constant

## Return Value

The model part category of the creature.

---

# SetCreatureVisualModelPartColor\_xpAM

```cpp
void SetCreatureVisualModelPartColor_xpAM(
    object oCreature,
    int iModelPart,
    int iColor,
    string sRGB
);
```
Set the Color of the iModelPart of oCreature Visual

## Parameters

* `oCreature` - The creature to change the model part color for
* `iModelPart` - ARMOR\_MODEL\_PART\_* constant
* `iColor` - 1, 2 or 3
* `sRGB` - a RGB string value, on the "#FFFFFF" (or #FFFFFFFF) format

---

# GetCreatureVisualModelPartColor\_xpAM

```cpp
string GetCreatureVisualModelPartColor_xpAM(
    object oCreature,
    int iModelPart,
    int iColor
);
```
Get the Color of the iModelPart of oCreature Visual

## Parameters

* `oCreature` - The creature to get the model part color for
* `iModelPart` - ARMOR\_MODEL\_PART\_* constant
* `iColor` - 1, 2 or 3

## Return Value

The RGB string value of the model part color.

---

Visual ModelPiece
---

---

# SetCreatureVisualModelPieceVariation\_xpAM

```cpp
void SetCreatureVisualModelPieceVariation_xpAM(
    object oCreature,
    int iModelPiece,
    int iVariation
);
```
Set the ModelPiece Variation for oCreature Visual. Use ARMOR\_MODEL\_PIECE\_* constant for iModelPiece

## Parameters

* `oCreature` - The creature to change the model piece variation for
* `iModelPiece` - ARMOR\_MODEL\_PIECE\_* constant
* `iVariation` - The model piece variation to use

---

# GetCreatureVisualModelPieceVariation\_xpAM

```cpp
int GetCreatureVisualModelPieceVariation_xpAM(
    object oCreature,
    int iModelPiece
);
```
Get the ModelPiece Variation for oCreature Visual. Use ARMOR\_MODEL\_PIECE\_* constant for iModelPiece

## Parameters

* `oCreature` - The creature to get the model piece variation for
* `iModelPiece` - ARMOR\_MODEL\_PIECE\_* constant

## Return Value

The model piece variation of the creature.

---

# SetCreatureVisualModelPieceColor\_xpAM

```cpp
void SetCreatureVisualModelPieceColor_xpAM(
    object oCreature,
    int iModelPiece,
    int iColor,
    string sRGB
);
```
Set the Color of the iModelPiece of oCreature Visual

## Parameters

* `oCreature` - The creature to change the model piece color for
* `iModelPiece` - ARMOR\_MODEL\_PIECE\_* constant
* `iColor` - 1, 2 or 3
* `sRGB` - a RGB string value, on the "#FFFFFF" (or #FFFFFFFF) format

---

# GetCreatureVisualModelPieceColor\_xpAM

```cpp
string GetCreatureVisualModelPieceColor_xpAM(
    object oCreature,
    int iModelPiece,
    int iColor
);
```
Get the Color of the iModelPiece of oCreature Visual

## Parameters

* `oCreature` - The creature to get the model piece color for
* `iModelPiece` - ARMOR\_MODEL\_PIECE\_* constant
* `iColor` - 1, 2 or 3

## Return Value

The RGB string value of the model piece color.

---

# SetCreatureVisualModelPieceIsUVScroll\_xpAM

```cpp
void SetCreatureVisualModelPieceIsUVScroll_xpAM(
    object oCreature,
    int iModelPiece,
    int bActivate
);
```
Enable or Disable the UVScroll of the iModelPiece of oCreature Visual

## Parameters

* `oCreature` - The creature to enable or disable the UVScroll for
* `iModelPiece` - ARMOR\_MODEL\_PIECE\_* constant
* `bActivate` - Set to TRUE to enable the UVScroll, FALSE to disable it

---

# GetCreatureVisualModelPieceIsUVScroll\_xpAM

```cpp
int GetCreatureVisualModelPieceIsUVScroll_xpAM(
    object oCreature,
    int iModelPiece
);
```
Get the activation status for the UVScroll of the iModelPiece of oCreature Visual

## Parameters

* `oCreature` - The creature to get the UVScroll activation status for
* `iModelPiece` - ARMOR\_MODEL\_PIECE\_* constant

## Return Value

The activation status of the UVScroll for the model piece.

---

# SetCreatureVisualModelPieceUScroll\_xpAM

```cpp
void SetCreatureVisualModelPieceUScroll_xpAM(
    object oCreature,
    int iModelPiece,
    float fValue
);
```
Set the UScroll speed for the iModelPiece of oCreature Visual

## Parameters

* `oCreature` - The creature to change the UScroll speed for
* `iModelPiece` - ARMOR\_MODEL\_PIECE\_* constant
* `fValue` - The UScroll speed to use

---

# GetCreatureVisualModelPieceUScroll\_xpAM

```cpp
float GetCreatureVisualModelPieceUScroll_xpAM(
    object oCreature,
    int iModelPiece
);
```
Get the UScroll speed for the iModelPiece of oCreature Visual

## Parameters

* `oCreature` - The creature to get the UScroll speed for
* `iModelPiece` - ARMOR\_MODEL\_PIECE\_* constant

## Return Value

The UScroll speed of the model piece.

---

# SetCreatureVisualModelPieceVScroll\_xpAM

```cpp
void SetCreatureVisualModelPieceVScroll_xpAM(
    object oCreature,
    int iModelPiece,
    float fValue
);
```
Set the VScroll speed for the iModelPiece of oCreature Visual

## Parameters

* `oCreature` - The creature to change the VScroll speed for
* `iModelPiece` - ARMOR\_MODEL\_PIECE\_* constant
* `fValue` - The VScroll speed to use

---

# GetCreatureVisualModelPieceVScroll\_xpAM

```cpp
float GetCreatureVisualModelPieceVScroll_xpAM(
    object oCreature,
    int iModelPiece
);
```
Get the VScroll speed for the iModelPiece of oCreature Visual

## Parameters

* `oCreature` - The creature to get the VScroll speed for
* `iModelPiece` - ARMOR\_MODEL\_PIECE\_* constant

## Return Value

The VScroll speed of the model piece.


Constants
---

# Creature Color Constants (CREATURE\_COLOR\_*)

| Name | Value | Description |
| --- | --- | --- |
| CREATURE\_COLOR\_1\_SEPT | "Color1" | Creature color 1 |
| CREATURE\_COLOR\_2\_SEPT | "Color2" | Creature color 2 |
| CREATURE\_COLOR\_3\_SEPT | "Color3" | Creature color 3 |
| CREATURE\_COLOR\_HEAD\_SEPT | "ColorHeadSkin" | Head/Skin color |
| CREATURE\_COLOR\_EYE\_SEPT | "ColorHeadEye" | Eye color |
| CREATURE\_COLOR\_BROW\_SEPT | "ColorHeadBrow" | Brow color |
| CREATURE\_COLOR\_HAIRACCESSORY\_SEPT | "ColorHairHacc" | Hair accessory color |
| CREATURE\_COLOR\_HAIR1\_SEPT | "ColorHair1" | Hair color 1 |
| CREATURE\_COLOR\_HAIR2\_SEPT | "ColorHair2" | Hair color 2 |

# Armor Class Type Constants (AC\_TYPE\_*)

| Name | Value | Description |
| --- | --- | --- |
| AC\_TYPE\_BASE\_SEPT | 20 | Base armor class |
| AC\_TYPE\_ARMORVAL\_SEPT | 21 | Armor value |
| AC\_TYPE\_SHIELDVAL\_SEPT | 22 | Shield value |
| AC\_TYPE\_ARMOR\_SEPT | 3 | Armor |
| AC\_TYPE\_DEFLECT\_SEPT | 5 | Deflection |
| AC\_TYPE\_SHIELD\_SEPT | 7 | Shield |
| AC\_TYPE\_NATURAL\_SEPT | 9 | Natural armor |
| AC\_TYPE\_DODGE\_SEPT | 11 | Dodge |

# Scale Axis Constants (SCALE\_*)

| Name | Value | Description |
| --- | --- | --- |
| SCALE\_X | 0 | X-axis scale |
| SCALE\_Y | 1 | Y-axis scale |
| SCALE\_Z | 2 | Z-axis scale |


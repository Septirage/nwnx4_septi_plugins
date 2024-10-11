

- [SetLightRange_xpAM](#setlightrange_xpam)
- [GetLightRange_xpAM](#getlightrange_xpam)
- [__Shadow__](#shadow)
  - [SetLightCastShadow_xpAM](#setlightcastshadow_xpam)
  - [GetLightCastShadow_xpAM](#getlightcastshadow_xpam)
  - [SetLightShadowIntensity_xpAM](#setlightshadowintensity_xpam)
  - [GetLightShadowIntensity_xpAM](#getlightshadowintensity_xpam)
- [__Light__](#light)
  - [SetLightColorIntensity_xpAM](#setlightcolorintensity_xpam)
  - [GetLightColorIntensity_xpAM](#getlightcolorintensity_xpam)
  - [SetLightColor_xpAM](#setlightcolor_xpam)
  - [GetLightColor_xpAM](#getlightcolor_xpam)
- [__Lerp__](#lerp)
  - [SetLightIsLerp_xpAM](#setlightislerp_xpam)
  - [GetLightIsLerp_xpAM](#getlightislerp_xpam)
  - [SetLightLerpPeriod_xpAM](#setlightlerpperiod_xpam)
  - [GetLightLerpPeriod_xpAM](#getlightlerpperiod_xpam)
  - [SetLightLerpIntensity_xpAM](#setlightlerpintensity_xpam)
  - [GetLightLerpIntensity_xpAM](#getlightlerpintensity_xpam)
  - [SetLightLerpColor_xpAM](#setlightlerpcolor_xpam)
  - [GetLightLerpColor_xpAM](#getlightlerpcolor_xpam)
- [__Flicker__](#flicker)
  - [SetLightIsFlicker_xpAM](#setlightisflicker_xpam)
  - [GetLightIsFlicker_xpAM](#getlightisflicker_xpam)
  - [SetLightFlickerType_xpAM](#setlightflickertype_xpam)
  - [GetLightFlickerType_xpAM](#getlightflickertype_xpam)
  - [SetLightFlickerVariance_xpAM](#setlightflickervariance_xpam)
  - [GetLightFlickerVariance_xpAM](#getlightflickervariance_xpam)
  - [SetLightFlickerRate_xpAM](#setlightflickerrate_xpam)
  - [GetLightFlickerRate_xpAM](#getlightflickerrate_xpam)

---

# SetLightRange_xpAM

```cpp
void SetLightRange_xpAM(
    object oLight,
    float fRange
);
```
Set the maximum distance that the light's diffuse color will reach.

## Parameters

* `oLight` - The light object
* `fRange` - The maximum distance for the light's diffuse color

---

# GetLightRange_xpAM

```cpp
float GetLightRange_xpAM(
    object oLight
);
```
Get the maximum distance that the light's diffuse color will reach.

## Parameters

* `oLight` - The light object

## Return Value

The maximum distance that the light's diffuse color will reach.

---

Shadow
----

---

# SetLightCastShadow_xpAM

```cpp
void SetLightCastShadow_xpAM(
    object oLight,
    int bCastShadow
);
```
Set if the light should cast shadows or not.

## Parameters

* `oLight` - The light object
* `bCastShadow` - True if the light should cast shadows, false otherwise

---

# GetLightCastShadow_xpAM

```cpp
int GetLightCastShadow_xpAM(
    object oLight
);
```
Get if the light casts shadows or not.

## Parameters

* `oLight` - The light object

## Return Value

True if the light casts shadows, false otherwise.

---

# SetLightShadowIntensity_xpAM

```cpp
void SetLightShadowIntensity_xpAM(
    object oLight,
    float fIntensity
);
```
Set the intensity of shadows cast by the light.

## Parameters

* `oLight` - The light object
* `fIntensity` - The shadow intensity

---

# GetLightShadowIntensity_xpAM

```cpp
float GetLightShadowIntensity_xpAM(
    object oLight
);
```
Get the intensity of shadows cast by the light.

## Parameters

* `oLight` - The light object

## Return Value

The intensity of shadows cast by the light.

---

Light
----

---

# SetLightColorIntensity_xpAM

```cpp
void SetLightColorIntensity_xpAM(
    object oLight,
    float fColorIntensity
);
```
Set the intensity (brightness) of the light within its range.

## Parameters

* `oLight` - The light object
* `fColorIntensity` - The light color intensity

---

# GetLightColorIntensity_xpAM

```cpp
float GetLightColorIntensity_xpAM(
    object oLight
);
```
Get the intensity (brightness) of the light within its range.

## Parameters

* `oLight` - The light object

## Return Value

The intensity (brightness) of the light within its range.

---

# SetLightColor_xpAM

```cpp
void SetLightColor_xpAM(
    object oLight,
    string sColorType,
    string sRGB
);
```
Set the chosen light color value.

## Parameters

* `oLight` - The light object
* `sColorType` - The LIGHT\_COLOR\_TYPE\_ constant
* `sRGB` - The RGB string value in the "#FFFFFF" (or #FFFFFFFF) format

---

# GetLightColor_xpAM

```cpp
string GetLightColor_xpAM(
    object oLight,
    string sColorType
);
```
Get the chosen light color value.

## Parameters

* `oLight` - The light object
* `sColorType` - The LIGHT\_COLOR\_TYPE\_ constant

## Return Value

The RGB string value of the chosen light color.

---

Lerp
----

---

# SetLightIsLerp_xpAM

```cpp
void SetLightIsLerp_xpAM(
    object oLight,
    int bLerp
);
```
Enable or disable lerp for this light.

## Parameters

* `oLight` - The light object
* `bLerp` - True to enable lerp, false to disable

---

# GetLightIsLerp_xpAM

```cpp
int GetLightIsLerp_xpAM(
    object oLight
);
```
Get the status of lerp for this light.

## Parameters

* `oLight` - The light object

## Return Value

True if lerp is enabled for the light, false otherwise.

---

# SetLightLerpPeriod_xpAM

```cpp
void SetLightLerpPeriod_xpAM(
    object oLight,
    float fLerpPeriod
);
```
Set the time in seconds for the light to cycle between the color and lerp target color.

## Parameters

* `oLight` - The light object
* `fLerpPeriod` - The lerp period in seconds

---

# GetLightLerpPeriod_xpAM

```cpp
float GetLightLerpPeriod_xpAM(
    object oLight
);
```
Get the time in seconds for the light to cycle between the color and lerp target color.

## Parameters

* `oLight` - The light object

## Return Value

The time in seconds for the light to cycle between the color and lerp target color.

---

# SetLightLerpIntensity_xpAM

```cpp
void SetLightLerpIntensity_xpAM(
    object oLight,
    float fLerpIntensity
);
```
Set the intensity (brightness) of the lerp light within its range.

## Parameters

* `oLight` - The light object
* `fLerpIntensity` - The lerp light intensity

---

# GetLightLerpIntensity_xpAM

```cpp
float GetLightLerpIntensity_xpAM(
    object oLight
);
```
Get the intensity (brightness) of the lerp light within its range.

## Parameters

* `oLight` - The light object

## Return Value

The intensity (brightness) of the lerp light within its range.

---

# SetLightLerpColor_xpAM

```cpp
void SetLightLerpColor_xpAM(
    object oLight,
    string sColorType,
    string sRGB
);
```
Set the chosen light lerp color value.

## Parameters

* `oLight` - The light object
* `sColorType` - The LIGHT\_COLOR\_TYPE\_ constant
* `sRGB` - The RGB string value in the "#FFFFFF" (or #FFFFFFFF) format

---

# GetLightLerpColor_xpAM

```cpp
string GetLightLerpColor_xpAM(
    object oLight,
    string sColorType
);
```
Get the chosen light lerp color value.

## Parameters

* `oLight` - The light object
* `sColorType` - The LIGHT\_COLOR\_TYPE\_ constant

## Return Value

The RGB string value of the chosen light lerp color.

---

Flicker
---

---

# SetLightIsFlicker_xpAM

```cpp
void SetLightIsFlicker_xpAM(
    object oLight,
    int bFlicker
);
```
Enable or disable flicker for this light.

## Parameters

* `oLight` - The light object
* `bFlicker` - True to enable flicker, false to disable

---

# GetLightIsFlicker_xpAM

```cpp
int GetLightIsFlicker_xpAM(
    object oLight
);
```
Get the flicker activation status for this light.

## Parameters

* `oLight` - The light object

## Return Value

True if flicker is enabled for the light, false otherwise.

---

# SetLightFlickerType_xpAM

```cpp
void SetLightFlickerType_xpAM(
    object oLight,
    int iFlickerType
);
```
Set the light flicker type with a FLICKER\_TYPE\_ value.

## Parameters

* `oLight` - The light object
* `iFlickerType` - The FLICKER\_TYPE\_ constant

---

# GetLightFlickerType_xpAM

```cpp
int GetLightFlickerType_xpAM(
    object oLight
);
```
Get the FLICKER\_TYPE\_ corresponding to the flicker type.

## Parameters

* `oLight` - The light object

## Return Value

The FLICKER\_TYPE\_ constant corresponding to the flicker type.

---

# SetLightFlickerVariance_xpAM

```cpp
void SetLightFlickerVariance_xpAM(
    object oLight,
    float fVariance
);
```
Set the flicker variance value.

## Parameters

* `oLight` - The light object
* `fVariance` - The flicker variance value

---

# GetLightFlickerVariance_xpAM

```cpp
float GetLightFlickerVariance_xpAM(
    object oLight
);
```
Get the flicker variance value.

## Parameters

* `oLight` - The light object

## Return Value

The flicker variance value.

---

# SetLightFlickerRate_xpAM

```cpp
void SetLightFlickerRate_xpAM(
    object oLight,
    float fRate
);
```
Set the flicker rate for this light.

## Parameters

* `oLight` - The light object
* `fRate` - The flicker rate

---

# GetLightFlickerRate_xpAM

```cpp
float GetLightFlickerRate_xpAM(
    object oLight
);
```
Get the flicker rate for this light.

## Parameters

* `oLight` - The light object

## Return Value

The flicker rate for the light.


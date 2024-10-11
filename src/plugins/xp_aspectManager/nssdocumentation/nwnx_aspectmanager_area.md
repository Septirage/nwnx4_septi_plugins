xp_aspectManager_area.nss
====


- [Area Settings](#area-settings)
  - [SetAreaFlags\_xpAM](#setareaflags_xpam)
  - [GetAreaFlags\_xpAM](#getareaflags_xpam)
  - [SetAreaNoRest\_xpAM](#setareanorest_xpam)
  - [GetAreaNoRest\_xpAM](#getareanorest_xpam)
  - [SetAreaPVPMode\_xpAM](#setareapvpmode_xpam)
  - [GetAreaPVPMode\_xpAM](#getareapvpmode_xpam)
  - [SetAreaModSpotCheck\_xpAM](#setareamodspotcheck_xpam)
  - [GetAreaModSpotCheck\_xpAM](#getareamodspotcheck_xpam)
  - [SetAreaModListenCheck\_xpAM](#setareamodlistencheck_xpam)
  - [GetAreaModListenCheck\_xpAM](#getareamodlistencheck_xpam)
  - [SetAreaSkyRing\_xpAM](#setareaskyring_xpam)
  - [GetAreaSkyRing\_xpAM](#getareaskyring_xpam)
  - [SetAreaHasDirectionalLight\_xpAM](#setareahasdirectionallight_xpam)
  - [GetAreaHasDirectionalLight\_xpAM](#getareahasdirectionallight_xpam)
  - [SetAreaIsDirLightCastShadow\_xpAM](#setareaisdirlightcastshadow_xpam)
  - [GetAreaIsDirLightCastShadow\_xpAM](#getareaisdirlightcastshadow_xpam)
- [Day/Night Cycle](#daynight-cycle)
  - [SetAreaHasDayNightCycle\_xpAM](#setareahasdaynightcycle_xpam)
  - [GetAreaHasDayNightCycle\_xpAM](#getareahasdaynightcycle_xpam)
  - [SetAreaIsAlwaysNight\_xpAM](#setareaisalwaysnight_xpam)
  - [GetAreaIsAlwaysNight\_xpAM](#getareaisalwaysnight_xpam)
  - [SetAreaUseDayNightFog\_xpAM](#setareausedaynightfog_xpam)
  - [GetAreaUseDayNightFog\_xpAM](#getareausedaynightfog_xpam)
  - [SetAreaDNCCoronaIntensity\_xpAM](#setareadnccoronaintensity_xpam)
  - [GetAreaDNCCoronaIntensity\_xpAM](#getareadnccoronaintensity_xpam)
  - [SetAreaDNCShadowIntensity\_xpAM](#setareadncshadowintensity_xpam)
  - [GetAreaDNCShadowIntensity\_xpAM](#getareadncshadowintensity_xpam)
  - [SetAreaDNCSkyRingColorInterpretation\_xpAM](#setareadncskyringcolorinterpretation_xpam)
  - [GetAreaDNCSkyRingColorInterpretation\_xpAM](#getareadncskyringcolorinterpretation_xpam)
  - [SetAreaDNCSkyDomeModel\_xpAM](#setareadncskydomemodel_xpam)
  - [GetAreaDNCSkyDomeModel\_xpAM](#getareadncskydomemodel_xpam)
  - [Bloom](#bloom)
    - [SetAreaDNCBloomBLRadius\_xpAM](#setareadncbloomblradius_xpam)
    - [GetAreaDNCBloomBLRadius\_xpAM](#getareadncbloomblradius_xpam)
    - [SetAreaDNCBloomGLIntensity\_xpAM](#setareadncbloomglintensity_xpam)
    - [GetAreaDNCBloomGLIntensity\_xpAM](#getareadncbloomglintensity_xpam)
    - [SetAreaDNCBloomHLIntensity\_xpAM](#setareadncbloomhlintensity_xpam)
    - [GetAreaDNCBloomHLIntensity\_xpAM](#getareadncbloomhlintensity_xpam)
    - [SetAreaDNCBloomHLThreshold\_xpAM](#setareadncbloomhlthreshold_xpam)
    - [GetAreaDNCBloomHLThreshold\_xpAM](#getareadncbloomhlthreshold_xpam)
    - [SetAreaDNCBloomSCIntensity\_xpAM](#setareadncbloomscintensity_xpam)
    - [GetAreaDNCBloomSCIntensity\_xpAM](#getareadncbloomscintensity_xpam)
  - [__HDR__](#hdr)
    - [SetAreaDNCHDRAvg\_xpAM](#setareadnchdravg_xpam)
    - [GetAreaDNCHDRAvg\_xpAM](#getareadnchdravg_xpam)
    - [SetAreaDNCHDRMax\_xpAM](#setareadnchdrmax_xpam)
    - [GetAreaDNCHDRMax\_xpAM](#getareadnchdrmax_xpam)
    - [SetAreaDNCHDRExposure\_xpAM](#setareadnchdrexposure_xpam)
    - [GetAreaDNCHDRExposure\_xpAM](#getareadnchdrexposure_xpam)
    - [SetAreaDNCHDRHighlight\_xpAM](#setareadnchdrhighlight_xpam)
    - [GetAreaDNCHDRHighlight\_xpAM](#getareadnchdrhighlight_xpam)
  - [__Cloud__](#cloud)
    - [SetAreaDNCCloudCover\_xpAM](#setareadnccloudcover_xpam)
    - [GetAreaDNCCloudCover\_xpAM](#getareadnccloudcover_xpam)
    - [SetAreaDNCCloudMoveX\_xpAM](#setareadnccloudmovex_xpam)
    - [GetAreaDNCCloudMoveX\_xpAM](#getareadnccloudmovex_xpam)
    - [SetAreaDNCCloudMoveY\_xpAM](#setareadnccloudmovey_xpam)
    - [GetAreaDNCCloudMoveY\_xpAM](#getareadnccloudmovey_xpam)
  - [__Desaturate__](#desaturate)
    - [SetAreaDNCDesaturateColor\_xpAM](#setareadncdesaturatecolor_xpam)
    - [GetAreaDNCDesaturateColor\_xpAM](#getareadncdesaturatecolor_xpam)
    - [SetAreaDNCDesaturateFactor\_xpAM](#setareadncdesaturatefactor_xpam)
    - [GetAreaDNCDesaturateFactor\_xpAM](#getareadncdesaturatefactor_xpam)
  - [__GroundLight__](#groundlight)
    - [SetAreaDNCGroundLightIntensity\_xpAM](#setareadncgroundlightintensity_xpam)
    - [GetAreaDNCGroundLightIntensity\_xpAM](#getareadncgroundlightintensity_xpam)
    - [SetAreaDNCGroundLightColor\_xpAM](#setareadncgroundlightcolor_xpam)
    - [GetAreaDNCGroundLightColor\_xpAM](#getareadncgroundlightcolor_xpam)
  - [__SkyLight__](#skylight)
    - [SetAreaDNCSkylightIntensity\_xpAM](#setareadncskylightintensity_xpam)
    - [GetAreaDNCSkylightIntensity\_xpAM](#getareadncskylightintensity_xpam)
    - [SetAreaDNCSkylightColor\_xpAM](#setareadncskylightcolor_xpam)
    - [GetAreaDNCSkylightColor\_xpAM](#getareadncskylightcolor_xpam)
  - [__SunMoon__](#sunmoon)
    - [SetAreaDNCSunMoonIntensity\_xpAM](#setareadncsunmoonintensity_xpam)
    - [GetAreaDNCSunMoonIntensity\_xpAM](#getareadncsunmoonintensity_xpam)
    - [SetAreaDNCSunMoonColor\_xpAM](#setareadncsunmooncolor_xpam)
    - [GetAreaDNCSunMoonColor\_xpAM](#getareadncsunmooncolor_xpam)
    - [SetAreaDNCSunMoonDirX\_xpAM](#setareadncsunmoondirx_xpam)
    - [GetAreaDNCSunMoonDirX\_xpAM](#getareadncsunmoondirx_xpam)
    - [SetAreaDNCSunMoonDirY\_xpAM](#setareadncsunmoondiry_xpam)
    - [GetAreaDNCSunMoonDirY\_xpAM](#getareadncsunmoondiry_xpam)
    - [SetAreaDNCSunMoonDirZ\_xpAM](#setareadncsunmoondirz_xpam)
    - [GetAreaDNCSunMoonDirZ\_xpAM](#getareadncsunmoondirz_xpam)
  - [__Fog__](#fog)
    - [SetAreaDNCFogStart\_xpAM](#setareadncfogstart_xpam)
    - [GetAreaDNCFogStart\_xpAM](#getareadncfogstart_xpam)
    - [SetAreaDNCFogEnd\_xpAM](#setareadncfogend_xpam)
    - [GetAreaDNCFogEnd\_xpAM](#getareadncfogend_xpam)
    - [SetAreaDNCFogFarClip\_xpAM](#setareadncfogfarclip_xpam)
    - [GetAreaDNCFogFarClip\_xpAM](#getareadncfogfarclip_xpam)
    - [SetAreaDNCFogColor\_xpAM](#setareadncfogcolor_xpam)
    - [GetAreaDNCFogColor\_xpAM](#getareadncfogcolor_xpam)
  - [__Other Colors__](#other-colors)
    - [SetAreaDNCSkyHorizonColor\_xpAM](#setareadncskyhorizoncolor_xpam)
    - [GetAreaDNCSkyHorizonColor\_xpAM](#getareadncskyhorizoncolor_xpam)
    - [SetAreaDNCSkyZenithColor\_xpAM](#setareadncskyzenithcolor_xpam)
    - [GetAreaDNCSkyZenithColor\_xpAM](#getareadncskyzenithcolor_xpam)
- [Constants](#constants)
  - [PVP Setting (PVP\_SETTING\_*)](#pvp-setting-pvp_setting_)
  - [Area Flag (AREA\_FLAG\_*)](#area-flag-area_flag_)
  - [DayNights (DAYNIGHT\_*)](#daynights-daynight_)


Area Settings
---

---

# SetAreaFlags\_xpAM

```cpp
void SetAreaFlags_xpAM(object oArea, int iFlags);
```
Set the AreaFlags (define Exterior, Natural, Subteran or not)

## Parameters

* `oArea` - The area to set the flags for
* `iFlags` - The flags to set

## Note

iFlags can be composed using the [AREA\_FLAG\_*](#area-flag-area_flag_) constants. 
Example : AREA_FLAG_NATURAL | AREA_FLAG_INTERIOR will be a natural and interior area.$

---

# GetAreaFlags\_xpAM

```cpp
int GetAreaFlags_xpAM(object oArea);
```
Get the AreaFlags (define Exterior, Natural, Subteran or not)

## Parameters

* `oArea` - The area to get the flags for

## Return Value

The area flags. Will be composed of [AREA\_FLAG\_*](#area-flag-area_flag_) values.
For example, an interior and natural area will return a value equal to AREA_FLAG_NATURAL | AREA_FLAG_INTERIOR

---

# SetAreaNoRest\_xpAM

```cpp
void SetAreaNoRest_xpAM(object oArea, int bNoRest);
```
Enable or disable possibility of Rest in oArea

## Parameters

* `oArea` - The area to set the rest restriction for
* `bNoRest` - Whether to disable rest (TRUE) or not (FALSE)

---

# GetAreaNoRest\_xpAM

```cpp
int GetAreaNoRest_xpAM(object oArea);
```
Get the possibility of Rest in oArea

## Parameters

* `oArea` - The area to get the rest restriction for

## Return Value

TRUE if rest is disabled, FALSE otherwise.

---

# SetAreaPVPMode\_xpAM

```cpp
void SetAreaPVPMode_xpAM(object oArea, int iPvPMode);
```
Set the PVP Mode of oArea

## Parameters

* `oArea` - The area to set the PVP mode for
* `iPvPMode` - The PVP mode to set ([PVP\_SETTING\_*](#pvp-setting-pvp_setting_) constant)

---

# GetAreaPVPMode\_xpAM

```cpp
int GetAreaPVPMode_xpAM(object oArea);
```
Get the PVP Mode of oArea

## Parameters

* `oArea` - The area to get the PVP mode for

## Return Value

The PVP mode ([PVP\_SETTING\_*](#pvp-setting-pvp_setting_) constant).

---

# SetAreaModSpotCheck\_xpAM

```cpp
void SetAreaModSpotCheck_xpAM(object oArea, int iModSpot);
```
Set the Spot Modificator for this Area

## Parameters

* `oArea` - The area to set the spot modifier for
* `iModSpot` - The spot modifier to set

---

# GetAreaModSpotCheck\_xpAM

```cpp
int GetAreaModSpotCheck_xpAM(object oArea);
```
Get the Spot Modificator for this Area

## Parameters

* `oArea` - The area to get the spot modifier for

## Return Value

The spot modifier.

---

# SetAreaModListenCheck\_xpAM

```cpp
void SetAreaModListenCheck_xpAM(object oArea, int iModListen);
```
Set the Listen Modificator for this Area

## Parameters

* `oArea` - The area to set the listen modifier for
* `iModListen` - The listen modifier to set

---

# GetAreaModListenCheck\_xpAM

```cpp
int GetAreaModListenCheck_xpAM(object oArea);
```
Get the Listen Modificator for this Area

## Parameters

* `oArea` - The area to get the listen modifier for

## Return Value

The listen modifier.

---

# SetAreaSkyRing\_xpAM

```cpp
void SetAreaSkyRing_xpAM(object oArea, string sSkyRing, int iVal);
```
Set the SkyRing for oArea

## Parameters

* `oArea` - The area to set the sky ring for
* `sSkyRing` - The sky ring to set ("N", "S", "E" or "W")
* `iVal` - The desired Skyring.2DA id

---

# GetAreaSkyRing\_xpAM

```cpp
int GetAreaSkyRing_xpAM(object oArea, string sSkyRing);
```
Get the SkyRing for oArea

## Parameters

* `oArea` - The area to get the sky ring for
* `sSkyRing` - The sky ring to get ("N", "S", "E" or "W")

## Return Value

The Skyring.2DA id.

---

# SetAreaHasDirectionalLight\_xpAM

```cpp
void SetAreaHasDirectionalLight_xpAM(object oArea, int bHasDirLight);
```
Enable or Disable the DirectionalLight for oArea

## Parameters

* `oArea` - The area to set the directional light for
* `bHasDirLight` - Whether to enable (TRUE) or disable (FALSE) the directional light

---

# GetAreaHasDirectionalLight\_xpAM

```cpp
int GetAreaHasDirectionalLight_xpAM(object oArea);
```
Get the status of the DirectionalLight of oArea

## Parameters

* `oArea` - The area to get the directional light status for

## Return Value

TRUE if the directional light is enabled, FALSE otherwise.

---

# SetAreaIsDirLightCastShadow\_xpAM

```cpp
void SetAreaIsDirLightCastShadow_xpAM(object oArea, int bDirLShadow);
```
Choose if the area DirectionalLight cast Shadow or not

## Parameters

* `oArea` - The area to set the directional light shadow for
* `bDirLShadow` - Whether the directional light should cast shadows (TRUE) or not (FALSE)

---

# GetAreaIsDirLightCastShadow\_xpAM

```cpp
int GetAreaIsDirLightCastShadow_xpAM(object oArea);
```
Get if the area DirectionalLight cast shadows or not

## Parameters

* `oArea` - The area to get the directional light shadow status for

## Return Value

TRUE if the directional light casts shadows, FALSE otherwise.

Day/Night Cycle
---

---

# SetAreaHasDayNightCycle\_xpAM

```cpp
void SetAreaHasDayNightCycle_xpAM(object oArea, int bHasDNC);
```
Enable or Disable the Day/Night Cycle for oArea

## Parameters

* `oArea` - The area to set the day/night cycle for
* `bHasDNC` - Whether to enable (TRUE) or disable (FALSE) the day/night cycle

---

# GetAreaHasDayNightCycle\_xpAM

```cpp
int GetAreaHasDayNightCycle_xpAM(object oArea);
```
Get the activation status of Day/Night Cycle for oArea

## Parameters

* `oArea` - The area to get the day/night cycle status for

## Return Value

TRUE if the day/night cycle is enabled, FALSE otherwise.

---

# SetAreaIsAlwaysNight\_xpAM

```cpp
void SetAreaIsAlwaysNight_xpAM(object oArea, int bNight);
```
If Day/Night Cycle is set to false, choose if oArea is always in Night not

## Parameters

* `oArea` - The area to set the night status for
* `bNight` - Whether the area should be in night (TRUE) or not (FALSE)

---

# GetAreaIsAlwaysNight\_xpAM

```cpp
int GetAreaIsAlwaysNight_xpAM(object oArea);
```
If Day/Night Cycle is set to false, get if oArea is always in Night not

## Parameters

* `oArea` - The area to get the night status for

## Return Value

TRUE if the area is always in night, FALSE otherwise.

---

# SetAreaUseDayNightFog\_xpAM

```cpp
void SetAreaUseDayNightFog_xpAM(object oArea, int bUseDNF);
```
If set to TRUE then the fog color will update as time passes.

## Parameters

* `oArea` - The area to set the day/night fog for
* `bUseDNF` - Whether to enable (TRUE) or disable (FALSE) the day/night fog

---

# GetAreaUseDayNightFog\_xpAM

```cpp
int GetAreaUseDayNightFog_xpAM(object oArea);
```
Get if fog color update with cycle or not

## Parameters

* `oArea` - The area to get the day/night fog status for

## Return Value

TRUE if the day/night fog is enabled, FALSE otherwise.

---

# SetAreaDNCCoronaIntensity\_xpAM

```cpp
void SetAreaDNCCoronaIntensity_xpAM(object oArea, int iDNC, float fIntensity);
```
Set the Sun (or moon) Corona Intensity during iDNC

## Parameters

* `oArea` - The area to set the corona intensity for
* `iDNC` - The time of day to set the intensity for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `fIntensity` - The intensity to set

---

# GetAreaDNCCoronaIntensity\_xpAM

```cpp
float GetAreaDNCCoronaIntensity_xpAM(object oArea, int iDNC);
```
Get the Sun (or moon) Corona Intensity during this iDNC

## Parameters

* `oArea` - The area to get the corona intensity for
* `iDNC` - The time of day to get the intensity for ([DAYNIGHT\_*](#daynights-daynight_) constant)

## Return Value

The corona intensity.

---

# SetAreaDNCShadowIntensity\_xpAM

```cpp
void SetAreaDNCShadowIntensity_xpAM(object oArea, int iDNC, float fIntensity);
```
Set the Shadow Intensity during iDNC

## Parameters

* `oArea` - The area to set the shadow intensity for
* `iDNC` - The time of day to set the intensity for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `fIntensity` - The intensity to set

---

# GetAreaDNCShadowIntensity\_xpAM

```cpp
float GetAreaDNCShadowIntensity_xpAM(object oArea, int iDNC);
```
Get the Shadow Intensity during iDNC

## Parameters

* `oArea` - The area to get the shadow intensity for
* `iDNC` - The time of day to get the intensity for ([DAYNIGHT\_*](#daynights-daynight_) constant)

## Return Value

The shadow intensity.

---

# SetAreaDNCSkyRingColorInterpretation\_xpAM

```cpp
void SetAreaDNCSkyRingColorInterpretation_xpAM(object oArea, int iDNC, float fLerp);
```
Set the "speed" of transition between Horizon and Zenith Color

## Parameters

* `oArea` - The area to set the sky ring color interpretation for
* `iDNC` - The time of day to set the interpretation for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `fLerp` - The "speed" of transition to set

---

# GetAreaDNCSkyRingColorInterpretation\_xpAM

```cpp
float GetAreaDNCSkyRingColorInterpretation_xpAM(object oArea, int iDNC);
```
Get the "speed" of transition between Horizon and Zenith Color

## Parameters

* `oArea` - The area to get the sky ring color interpretation for
* `iDNC` - The time of day to get the interpretation for ([DAYNIGHT\_*](#daynights-daynight_) constant)

## Return Value

The "speed" of transition.

---

# SetAreaDNCSkyDomeModel\_xpAM

```cpp
void SetAreaDNCSkyDomeModel_xpAM(object oArea, int iDNC, string sSkyDome);
```
Set the SkyDomeModel of oArea during iDNC

## Parameters

* `oArea` - The area to set the sky dome model for
* `iDNC` - The time of day to set the model for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `sSkyDome` - The sky dome model to set

---

# GetAreaDNCSkyDomeModel\_xpAM

```cpp
string GetAreaDNCSkyDomeModel_xpAM(object oArea, int iDNC);
```
Set the SkyDomeModel of oArea during iDNC

## Parameters

* `oArea` - The area to get the sky dome model for
* `iDNC` - The time of day to get the model for ([DAYNIGHT\_*](#daynights-daynight_) constant)

## Return Value

The sky dome model.

Bloom
---

---

# SetAreaDNCBloomBLRadius\_xpAM

```cpp
void SetAreaDNCBloomBLRadius_xpAM(object oArea, int iDNC, float fRadius);
```
Set the Bloom Blur Radius

## Parameters

* `oArea` - The area to set the bloom blur radius for
* `iDNC` - The time of day to set the radius for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `fRadius` - The bloom blur radius to set

---

# GetAreaDNCBloomBLRadius\_xpAM

```cpp
float GetAreaDNCBloomBLRadius_xpAM(object oArea, int iDNC);
```
Get the Bloom Blur Radius

## Parameters

* `oArea` - The area to get the bloom blur radius for
* `iDNC` - The time of day to get the radius for ([DAYNIGHT\_*](#daynights-daynight_) constant)

## Return Value

The bloom blur radius.

---

# SetAreaDNCBloomGLIntensity\_xpAM

```cpp
void SetAreaDNCBloomGLIntensity_xpAM(object oArea, int iDNC, float fIntensity);
```
Set the Bloom Glow Intensity

## Parameters

* `oArea` - The area to set the bloom glow intensity for
* `iDNC` - The time of day to set the intensity for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `fIntensity` - The bloom glow intensity to set

---

# GetAreaDNCBloomGLIntensity\_xpAM

```cpp
float GetAreaDNCBloomGLIntensity_xpAM(object oArea, int iDNC);
```
Get the Bloom Glow Intensity

## Parameters

* `oArea` - The area to get the bloom glow intensity for
* `iDNC` - The time of day to get the intensity for ([DAYNIGHT\_*](#daynights-daynight_) constant)

## Return Value

The bloom glow intensity.

---

# SetAreaDNCBloomHLIntensity\_xpAM

```cpp
void SetAreaDNCBloomHLIntensity_xpAM(object oArea, int iDNC, float fIntensity);
```
Set the Bloom Highlight Intensity

## Parameters

* `oArea` - The area to set the bloom highlight intensity for
* `iDNC` - The time of day to set the intensity for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `fIntensity` - The bloom highlight intensity to set

---

# GetAreaDNCBloomHLIntensity\_xpAM

```cpp
float GetAreaDNCBloomHLIntensity_xpAM(object oArea, int iDNC);
```
Get the Bloom Highlight Intensity

## Parameters

* `oArea` - The area to get the bloom highlight intensity for
* `iDNC` - The time of day to get the intensity for ([DAYNIGHT\_*](#daynights-daynight_) constant)

## Return Value

The bloom highlight intensity.

---

# SetAreaDNCBloomHLThreshold\_xpAM

```cpp
void SetAreaDNCBloomHLThreshold_xpAM(object oArea, int iDNC, float fIntensity);
```
Set the Bloom Highlight Threshold

## Parameters

* `oArea` - The area to set the bloom highlight threshold for
* `iDNC` - The time of day to set the threshold for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `fIntensity` - The bloom highlight threshold to set

---

# GetAreaDNCBloomHLThreshold\_xpAM

```cpp
float GetAreaDNCBloomHLThreshold_xpAM(object oArea, int iDNC);
```
Get the Bloom Highlight Threshold

## Parameters

* `oArea` - The area to get the bloom highlight threshold for
* `iDNC` - The time of day to get the threshold for ([DAYNIGHT\_*](#daynights-daynight_) constant)

## Return Value

The bloom highlight threshold.

---

# SetAreaDNCBloomSCIntensity\_xpAM

```cpp
void SetAreaDNCBloomSCIntensity_xpAM(object oArea, int iDNC, float fIntensity);
```
Set the Bloom Scene Intensity

## Parameters

* `oArea` - The area to set the bloom scene intensity for
* `iDNC` - The time of day to set the intensity for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `fIntensity` - The bloom scene intensity to set

---

# GetAreaDNCBloomSCIntensity\_xpAM

```cpp
float GetAreaDNCBloomSCIntensity_xpAM(object oArea, int iDNC);
```
Get the Bloom Scene Intensity

## Parameters

* `oArea` - The area to get the bloom scene intensity for
* `iDNC` - The time of day to get the intensity for ([DAYNIGHT\_*](#daynights-daynight_) constant)

## Return Value

The bloom scene intensity.

HDR
---

---

# SetAreaDNCHDRAvg\_xpAM

```cpp
void SetAreaDNCHDRAvg_xpAM(object oArea, int iDNC, float fHDR);
```
Set the HDR Average Value

## Parameters

* `oArea` - The area to set the HDR average value for
* `iDNC` - The time of day to set the value for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `fHDR` - The HDR average value to set

---

# GetAreaDNCHDRAvg\_xpAM

```cpp
float GetAreaDNCHDRAvg_xpAM(object oArea, int iDNC);
```
Get the HDR Average Value

## Parameters

* `oArea` - The area to get the HDR average value for
* `iDNC` - The time of day to get the value for ([DAYNIGHT\_*](#daynights-daynight_) constant)

## Return Value

The HDR average value.

---

# SetAreaDNCHDRMax\_xpAM

```cpp
void SetAreaDNCHDRMax_xpAM(object oArea, int iDNC, float fHDR);
```
Set the HDR Maximum Value

## Parameters

* `oArea` - The area to set the HDR maximum value for
* `iDNC` - The time of day to set the value for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `fHDR` - The HDR maximum value to set

---

# GetAreaDNCHDRMax\_xpAM

```cpp
float GetAreaDNCHDRMax_xpAM(object oArea, int iDNC);
```
Get the HDR Maximum Value

## Parameters

* `oArea` - The area to get the HDR maximum value for
* `iDNC` - The time of day to get the value for ([DAYNIGHT\_*](#daynights-daynight_) constant)

## Return Value

The HDR maximum value.

---

# SetAreaDNCHDRExposure\_xpAM

```cpp
void SetAreaDNCHDRExposure_xpAM(object oArea, int iDNC, float fHDR);
```
Set the HDR Exposure Value

## Parameters

* `oArea` - The area to set the HDR exposure value for
* `iDNC` - The time of day to set the value for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `fHDR` - The HDR exposure value to set

---

# GetAreaDNCHDRExposure\_xpAM

```cpp
float GetAreaDNCHDRExposure_xpAM(object oArea, int iDNC);
```
Get the HDR Exposure Value

## Parameters

* `oArea` - The area to get the HDR exposure value for
* `iDNC` - The time of day to get the value for ([DAYNIGHT\_*](#daynights-daynight_) constant)

## Return Value

The HDR exposure value.

---

# SetAreaDNCHDRHighlight\_xpAM

```cpp
void SetAreaDNCHDRHighlight_xpAM(object oArea, int iDNC, float fHDR);
```
Set the HDR Highlight Value

## Parameters

* `oArea` - The area to set the HDR highlight value for
* `iDNC` - The time of day to set the value for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `fHDR` - The HDR highlight value to set

---

# GetAreaDNCHDRHighlight\_xpAM

```cpp
float GetAreaDNCHDRHighlight_xpAM(object oArea, int iDNC);
```
Get the HDR Highlight Value

## Parameters

* `oArea` - The area to get the HDR highlight value for
* `iDNC` - The time of day to get the value for ([DAYNIGHT\_*](#daynights-daynight_) constant)

## Return Value

The HDR highlight value.

Cloud
---

---

# SetAreaDNCCloudCover\_xpAM

```cpp
void SetAreaDNCCloudCover_xpAM(object oArea, int iDNC, float fCover);
```
Set the Cloud Cover

## Parameters

* `oArea` - The area to set the cloud cover for
* `iDNC` - The time of day to set the cover for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `fCover` - The cloud cover to set (value between 0 and 1.5)

---

# GetAreaDNCCloudCover\_xpAM

```cpp
float GetAreaDNCCloudCover_xpAM(object oArea, int iDNC);
```
Get the Cloud Cover

## Parameters

* `oArea` - The area to get the cloud cover for
* `iDNC` - The time of day to get the cover for ([DAYNIGHT\_*](#daynights-daynight_) constant)

## Return Value

The cloud cover (value between 0 and 1.5).

---

# SetAreaDNCCloudMoveX\_xpAM

```cpp
void SetAreaDNCCloudMoveX_xpAM(object oArea, int iDNC, float fMove);
```
Set the Cloud Movement in the East-West direction

## Parameters

* `oArea` - The area to set the cloud movement for
* `iDNC` - The time of day to set the movement for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `fMove` - The cloud movement in the East-West direction to set

## Note

A positive values causing movement toward the west. 
Unless the weather is extreme, you will likely want values of 0.1 or less.

---

# GetAreaDNCCloudMoveX\_xpAM

```cpp
float GetAreaDNCCloudMoveX_xpAM(object oArea, int iDNC);
```
Get the Cloud Movement in the East-West direction

## Parameters

* `oArea` - The area to get the cloud movement for
* `iDNC` - The time of day to get the movement for ([DAYNIGHT\_*](#daynights-daynight_) constant)

## Return Value

The cloud movement in the East-West direction.
A positive value represent a movement toward the west.

---

# SetAreaDNCCloudMoveY\_xpAM

```cpp
void SetAreaDNCCloudMoveY_xpAM(object oArea, int iDNC, float fMove);
```
Set the Cloud Movement in the North-South direction

## Parameters

* `oArea` - The area to set the cloud movement for
* `iDNC` - The time of day to set the movement for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `fMove` - The cloud movement in the North-South direction to set

## Note

A positive values causing movement toward the South. 
Unless the weather is extreme, you will likely want values of 0.1 or less.

---

# GetAreaDNCCloudMoveY\_xpAM

```cpp
float GetAreaDNCCloudMoveY_xpAM(object oArea, int iDNC);
```
Get the Cloud Movement in the North-South direction

## Parameters

* `oArea` - The area to get the cloud movement for
* `iDNC` - The time of day to get the movement for ([DAYNIGHT\_*](#daynights-daynight_) constant)

## Return Value

The cloud movement in the North-South direction.
A positive value represent a movement toward the South.

Desaturate
---

---

# SetAreaDNCDesaturateColor\_xpAM

```cpp
void SetAreaDNCDesaturateColor_xpAM(object oArea, int iDNC, int bDesaturate);
```
Enable or Disable the Desaturation effect

## Parameters

* `oArea` - The area to set the desaturation for
* `iDNC` - The time of day to set the effect for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `bDesaturate` - Whether to enable (TRUE) or disable (FALSE) the desaturation effect

---

# GetAreaDNCDesaturateColor\_xpAM

```cpp
int GetAreaDNCDesaturateColor_xpAM(object oArea, int iDNC);
```
Get the status of desaturation effect

## Parameters

* `oArea` - The area to get the desaturation status for
* `iDNC` - The time of day to get the status for ([DAYNIGHT\_*](#daynights-daynight_) constant)

## Return Value

TRUE if desaturation is enabled, FALSE otherwise.

---

# SetAreaDNCDesaturateFactor\_xpAM

```cpp
void SetAreaDNCDesaturateFactor_xpAM(object oArea, int iDNC, float fFactor);
```
Set the desaturation factor

## Parameters

* `oArea` - The area to set the desaturation factor for
* `iDNC` - The time of day to set the factor for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `fFactor` - The desaturation factor to set (value from 0 to 1)

## Note

If DesaturateColor is TRUE, then this value controls the amount of leeching of the color.

---

# GetAreaDNCDesaturateFactor\_xpAM

```cpp
float GetAreaDNCDesaturateFactor_xpAM(object oArea, int iDNC);
```
Get the desaturation factor

## Parameters

* `oArea` - The area to get the desaturation factor for
* `iDNC` - The time of day to get the factor for ([DAYNIGHT\_*](#daynights-daynight_) constant)

## Return Value

The desaturation factor (value from 0 to 1).

GroundLight
---

---

# SetAreaDNCGroundLightIntensity\_xpAM

```cpp
void SetAreaDNCGroundLightIntensity_xpAM(object oArea, int iDNC, float fIntensity);
```
Set the GroundLight Intensity

## Parameters

* `oArea` - The area to set the ground light intensity for
* `iDNC` - The time of day to set the intensity for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `fIntensity` - The ground light intensity to set

---

# GetAreaDNCGroundLightIntensity\_xpAM

```cpp
float GetAreaDNCGroundLightIntensity_xpAM(object oArea, int iDNC);
```
Get the GroundLight Intensity

## Parameters

* `oArea` - The area to get the ground light intensity for
* `iDNC` - The time of day to get the intensity for ([DAYNIGHT\_*](#daynights-daynight_) constant)

## Return Value

The ground light intensity.

---

# SetAreaDNCGroundLightColor\_xpAM

```cpp
void SetAreaDNCGroundLightColor_xpAM(object oArea, int iDNC, string sColorType, string sRGB);
```
Set the GroundLight Color

## Parameters

* `oArea` - The area to set the ground light color for
* `iDNC` - The time of day to set the color for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `sColorType` - The type of color to set ("Ambient", "Diffuse", or "Specular")
* `sRGB` - The RGB value of the color to set (on #RRGGBB format)

---

# GetAreaDNCGroundLightColor\_xpAM

```cpp
string GetAreaDNCGroundLightColor_xpAM(object oArea, int iDNC, string sColorType);
```
Get the GroundLight Color

## Parameters

* `oArea` - The area to get the ground light color for
* `iDNC` - The time of day to get the color for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `sColorType` - The type of color to get ("Ambient", "Diffuse", or "Specular")

## Return Value

The RGB value of the ground light color (on #RRGGGBB format).

SkyLight
---

---

# SetAreaDNCSkylightIntensity\_xpAM

```cpp
void SetAreaDNCSkylightIntensity_xpAM(object oArea, int iDNC, float fIntensity);
```
Set the SkyLight Intensity

## Parameters

* `oArea` - The area to set the skylight intensity for
* `iDNC` - The time of day to set the intensity for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `fIntensity` - The skylight intensity to set

---

# GetAreaDNCSkylightIntensity\_xpAM

```cpp
float GetAreaDNCSkylightIntensity_xpAM(object oArea, int iDNC);
```
Get the SkyLight Intensity

## Parameters

* `oArea` - The area to get the skylight intensity for
* `iDNC` - The time of day to get the intensity for ([DAYNIGHT\_*](#daynights-daynight_) constant)

## Return Value

The skylight intensity.

---

# SetAreaDNCSkylightColor\_xpAM

```cpp
void SetAreaDNCSkylightColor_xpAM(object oArea, int iDNC, string sColorType, string sRGB);
```
Set the SkyLight Color

## Parameters

* `oArea` - The area to set the skylight color for
* `iDNC` - The time of day to set the color for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `sColorType` - The type of color to set ("Ambient", "Diffuse", or "Specular")
* `sRGB` - The RGB value of the color to set (in #RRGGGBB format)

---

# GetAreaDNCSkylightColor\_xpAM

```cpp
string GetAreaDNCSkylightColor_xpAM(object oArea, int iDNC, string sColorType);
```
Get the SkyLight Color

## Parameters

* `oArea` - The area to get the skylight color for
* `iDNC` - The time of day to get the color for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `sColorType` - The type of color to get ("Ambient", "Diffuse", or "Specular")

## Return Value

The RGB value of the skylight color (in #RRGGGBB format).

SunMoon
---

---

# SetAreaDNCSunMoonIntensity\_xpAM

```cpp
void SetAreaDNCSunMoonIntensity_xpAM(object oArea, int iDNC, float fIntensity);
```
Set the Sun/Moon Intensity

## Parameters

* `oArea` - The area to set the sun/moon intensity for
* `iDNC` - The time of day to set the intensity for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `fIntensity` - The sun/moon intensity to set

---

# GetAreaDNCSunMoonIntensity\_xpAM

```cpp
float GetAreaDNCSunMoonIntensity_xpAM(object oArea, int iDNC);
```
Get the Sun/Moon Intensity

## Parameters

* `oArea` - The area to get the sun/moon intensity for
* `iDNC` - The time of day to get the intensity for ([DAYNIGHT\_*](#daynights-daynight_) constant)

## Return Value

The sun/moon intensity.

---

# SetAreaDNCSunMoonColor\_xpAM

```cpp
void SetAreaDNCSunMoonColor_xpAM(object oArea, int iDNC, string sColorType, string sRGB);
```
Set the Sun/Moon Color

## Parameters

* `oArea` - The area to set the sun/moon color for
* `iDNC` - The time of day to set the color for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `sColorType` - The type of color to set ("Ambient", "Diffuse", or "Specular")
* `sRGB` - The RGB value of the color to set (in #RRGGGBB format)

---

# GetAreaDNCSunMoonColor\_xpAM

```cpp
string GetAreaDNCSunMoonColor_xpAM(object oArea, int iDNC, string sColorType);
```
Get the Sun/Moon Color

## Parameters

* `oArea` - The area to get the sun/moon color for
* `iDNC` - The time of day to get the color for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `sColorType` - The type of color to get ("Ambient", "Diffuse", or "Specular")

## Return Value

The RGB value of the sun/moon color (in #RRGGGBB format).

---

# SetAreaDNCSunMoonDirX\_xpAM

```cpp
void SetAreaDNCSunMoonDirX_xpAM(object oArea, int iDNC, float fDir);
```
Set the Sun/Moon Direction in the X axis

## Parameters

* `oArea` - The area to set the sun/moon direction for
* `iDNC` - The time of day to set the direction for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `fDir` - The sun/moon direction in the X axis to set

---

# GetAreaDNCSunMoonDirX\_xpAM

```cpp
float GetAreaDNCSunMoonDirX_xpAM(object oArea, int iDNC);
```
Get the Sun/Moon Direction in the X axis

## Parameters

* `oArea` - The area to get the sun/moon direction for
* `iDNC` - The time of day to get the direction for ([DAYNIGHT\_*](#daynights-daynight_) constant)

## Return Value

The sun/moon direction in the X axis.

---

# SetAreaDNCSunMoonDirY\_xpAM

```cpp
void SetAreaDNCSunMoonDirY_xpAM(object oArea, int iDNC, float fDir);
```
Set the Sun/Moon Direction in the Y axis

## Parameters

* `oArea` - The area to set the sun/moon direction for
* `iDNC` - The time of day to set the direction for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `fDir` - The sun/moon direction in the Y axis to set

---

# GetAreaDNCSunMoonDirY\_xpAM

```cpp
float GetAreaDNCSunMoonDirY_xpAM(object oArea, int iDNC);
```
Get the Sun/Moon Direction in the Y axis

## Parameters

* `oArea` - The area to get the sun/moon direction for
* `iDNC` - The time of day to get the direction for ([DAYNIGHT\_*](#daynights-daynight_) constant)

## Return Value

The sun/moon direction in the Y axis.

---

# SetAreaDNCSunMoonDirZ\_xpAM

```cpp
void SetAreaDNCSunMoonDirZ_xpAM(object oArea, int iDNC, float fDir);
```
Set the Sun/Moon Direction in the Z axis

## Parameters

* `oArea` - The area to set the sun/moon direction for
* `iDNC` - The time of day to set the direction for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `fDir` - The sun/moon direction in the Z axis to set

---

# GetAreaDNCSunMoonDirZ\_xpAM

```cpp
float GetAreaDNCSunMoonDirZ_xpAM(object oArea, int iDNC);
```
Get the Sun/Moon Direction in the Z axis

## Parameters

* `oArea` - The area to get the sun/moon direction for
* `iDNC` - The time of day to get the direction for ([DAYNIGHT\_*](#daynights-daynight_) constant)

## Return Value

The sun/moon direction in the Z axis.

Fog
---

---

# SetAreaDNCFogStart\_xpAM

```cpp
void SetAreaDNCFogStart_xpAM(object oArea, int iDNC, float fStart);
```
Set the Fog Start value

## Parameters

* `oArea` - The area to set the fog start value for
* `iDNC` - The time of day to set the value for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `fStart` - The fog start value to set

---

# GetAreaDNCFogStart\_xpAM

```cpp
float GetAreaDNCFogStart_xpAM(object oArea, int iDNC);
```
Get the Fog Start value

## Parameters

* `oArea` - The area to get the fog start value for
* `iDNC` - The time of day to get the value for ([DAYNIGHT\_*](#daynights-daynight_) constant)

## Return Value

The fog start value.

---

# SetAreaDNCFogEnd\_xpAM

```cpp
void SetAreaDNCFogEnd_xpAM(object oArea, int iDNC, float fEnd);
```
Set the Fog End value

## Parameters

* `oArea` - The area to set the fog end value for
* `iDNC` - The time of day to set the value for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `fEnd` - The fog end value to set

---

# GetAreaDNCFogEnd\_xpAM

```cpp
float GetAreaDNCFogEnd_xpAM(object oArea, int iDNC);
```
Get the Fog End value

## Parameters

* `oArea` - The area to get the fog end value for
* `iDNC` - The time of day to get the value for ([DAYNIGHT\_*](#daynights-daynight_) constant)

## Return Value

The fog end value.

---

# SetAreaDNCFogFarClip\_xpAM

```cpp
void SetAreaDNCFogFarClip_xpAM(object oArea, int iDNC, float fFarClip);
```
Set the Fog Far Clip value

## Parameters

* `oArea` - The area to set the fog far clip value for
* `iDNC` - The time of day to set the value for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `fFarClip` - The fog far clip value to set

---

# GetAreaDNCFogFarClip\_xpAM

```cpp
float GetAreaDNCFogFarClip_xpAM(object oArea, int iDNC);
```
Get the Fog Far Clip value

## Parameters

* `oArea` - The area to get the fog far clip value for
* `iDNC` - The time of day to get the value for ([DAYNIGHT\_*](#daynights-daynight_) constant)

## Return Value

The fog far clip value.

---

# SetAreaDNCFogColor\_xpAM

```cpp
void SetAreaDNCFogColor_xpAM(object oArea, int iDNC, string sRGB);
```
Set the Fog Color

## Parameters

* `oArea` - The area to set the fog color for
* `iDNC` - The time of day to set the color for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `sRGB` - The RGB value of the color to set (in #RRGGGBB format)

---

# GetAreaDNCFogColor\_xpAM

```cpp
string GetAreaDNCFogColor_xpAM(object oArea, int iDNC);
```
Get the Fog Color

## Parameters

* `oArea` - The area to get the fog color for
* `iDNC` - The time of day to get the color for ([DAYNIGHT\_*](#daynights-daynight_) constant)

## Return Value

The RGB value of the fog color (in #RRGGGBB format).

Other Colors
---

---

# SetAreaDNCSkyHorizonColor\_xpAM

```cpp
void SetAreaDNCSkyHorizonColor_xpAM(object oArea, int iDNC, string sRGB);
```
Set the SkyHorizon Color

## Parameters

* `oArea` - The area to set the SkyHorizon color for
* `iDNC` - The time of day to set the color for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `sRGB` - The RGB value of the color to set (in #RRGGGBB format)

---

# GetAreaDNCSkyHorizonColor\_xpAM

```cpp
string GetAreaDNCSkyHorizonColor_xpAM(object oArea, int iDNC);
```
Get the SkyHorizon Color

## Parameters

* `oArea` - The area to get the SkyHorizon color for
* `iDNC` - The time of day to get the color for ([DAYNIGHT\_*](#daynights-daynight_) constant)

## Return Value

The RGB value of the SkyHorizon color (in #RRGGGBB format).

---

# SetAreaDNCSkyZenithColor\_xpAM

```cpp
void SetAreaDNCSkyZenithColor_xpAM(object oArea, int iDNC, string sRGB);
```
Set the SkyZenith Color

## Parameters

* `oArea` - The area to set the SkyZenith color for
* `iDNC` - The time of day to set the color for ([DAYNIGHT\_*](#daynights-daynight_) constant)
* `sRGB` - The RGB value of the color to set (in #RRGGGBB format)

---

# GetAreaDNCSkyZenithColor\_xpAM

```cpp
string GetAreaDNCSkyZenithColor_xpAM(object oArea, int iDNC);
```
Get the SkyZenith Color

## Parameters

* `oArea` - The area to get the SkyZenith color for
* `iDNC` - The time of day to get the color for ([DAYNIGHT\_*](#daynights-daynight_) constant)

## Return Value

The RGB value of the SkyZenith color (in #RRGGGBB format).

---

Constants
---------

# PVP Setting (PVP\_SETTING\_*)

| Name | Value | Description |
| --- | --- | --- |
| PVP\_SETTING\_NO | 0 |  |
| PVP\_SETTING\_GROUP | 1 |  |
| PVP\_SETTING\_TOTAL | 2 |  |
| PVP\_SETTING\_SERVER | 3 |  |

# Area Flag (AREA\_FLAG\_*)

| Name | Value | Description |
| --- | --- | --- |
| AREA\_FLAG\_INTERIOR | 0 |  |
| AREA\_FLAG\_SUBTERRAN | 2 |  |
| AREA\_FLAG\_NATURAL | 4 |  |

# DayNights (DAYNIGHT\_*)

| Name | Value | Description |
| --- | --- | --- |
| DAYNIGHT\_SUNRISE | 0 |  |
| DAYNIGHT\_DAYTIME | 1 |  |
| DAYNIGHT\_SUNSET | 2 |  |
| DAYNIGHT\_MOONRISE | 3 |  |
| DAYNIGHT\_NIGHTTIME | 4 |  |
| DAYNIGHT\_MOONSET | 5 |  |
| DAYNIGHT\_TRANSITION | 6 |  |
| DAYNIGHT\_DEFAULT | 7 |  |

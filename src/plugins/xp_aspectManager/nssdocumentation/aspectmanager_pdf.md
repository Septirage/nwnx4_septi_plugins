


.
.
.
.
.
.
.



> This document will try to compile all functions added by xp_aspectManager, trying to give you useful informations and help on "how to" use them.
> It will be updated regulary.



- [xp_aspectManager.nss](#xp_aspectmanagernss)
- [xp_aspectManager_creature.nss](#xp_aspectmanager_creaturenss)
- [xp_aspectManager_area.nss](#xp_aspectmanager_areanss)
- [xp_aspectManager_item.nss](#xp_aspectmanager_itemnss)
- [xp_aspectManager_light.nss](#xp_aspectmanager_lightnss)
- [Constants List](#constants-list)


---

---

<div style="page-break-after: always;"></div>

xp_aspectManager.nss
=====================
<table border="0" style="font-size: 9px">
 <tr>
<td style="vertical-align:top;">
      
Hit Points Functions  

[SetCurrentHitPoints\_xpAM](#setcurrenthitpoints_xpam)  
[GetCurrentHitPoints\_xpAM](#getcurrenthitpoints_xpam)  
[SetTemporaryHitPoints\_xpAM](#settemporaryhitpoints_xpam)  
[GetTemporaryHitPoints\_xpAM](#gettemporaryhitpoints_xpam)  
[SetMaxHitPoints\_xpAM](#setmaxhitpoints_xpam)  
[GetMaxHitPoints\_xpAM](#getmaxhitpoints_xpam)  
[SetModMaxHitPoints\_xpAM](#setmodmaxhitpoints_xpam)  
[GetModMaxHitPoints\_xpAM](#getmodmaxhitpoints_xpam)  

Appearance  
[SetAppearance\_xpAM](#setappearance_xpam)  
[GetAppearance\_xpAM](#getappearance_xpam)
      
UVScroll    
[SetUVScroll\_xpAM](#setuvscroll_xpam)  
[GetUVScroll\_xpAM](#getuvscroll_xpam)  
[SetUScroll\_xpAM](#setuscroll_xpam)  
[GetUScroll\_xpAM](#getuscroll_xpam)  
[SetVScroll\_xpAM](#setvscroll_xpam)  
[GetVScroll\_xpAM](#getvscroll_xpam)

Inventory  
[SetHasInventory\_xpAM](#sethasinventory_xpam)  
[GetHasInventory\_xpAM](#gethasinventory_xpam)  
[SetInventoryMaxSize\_xpAM](#setinventorymaxsize_xpam)  
[GetInventoryMaxSize\_xpAM](#getinventorymaxsize_xpam)  
[GetInventoryNbItem\_xpAM](#getinventorynbitem_xpam)  

Lock  
[SetAutoRemoveKey\_xpAM](#setautoremovekey_xpam)  
[GetAutoRemoveKey\_xpAM](#getautoremovekey_xpam)  
</td>
<td style="vertical-align:top">

Trigger  
[GetTriggerType\_xpAM](#gettriggertype_xpam)  
[SetTriggerType\_xpAM](#settriggertype_xpam)  
[SetTriggerGeometry\_xpAM](#settriggergeometry_xpam)

Transition  
[SetIsInvisibleTransition\_xpAM](#setisinvisibletransition_xpam)  
[GetIsInvisibleTransition\_xpAM](#getisinvisibletransition_xpam)  
[SetIsPartyTransition\_xpAM](#setispartytransition_xpam)  
[GetIsPartyTransition\_xpAM](#getispartytransition_xpam)  
[SetTransitionLinkObjectType\_xpAM](#settransitionlinkobjecttype_xpam)  
[GetTransitionLinkObjectType\_xpAM](#gettransitionlinkobjecttype_xpam)  
[SetLoadScreenID\_xpAM](#setloadscreenid_xpam)  
[GetLoadScreenID\_xpAM](#getloadscreenid_xpam)  
[SetTransitionDestination\_xpAM](#settransitiondestination_xpam)  
[GetTransitionDestination\_xpAM](#gettransitiondestination_xpam)  
  
Saves    
[SetMagicResistance\_xpAM](#setmagicresistance_xpam)  
[GetMagicResistance\_xpAM](#getmagicresistance_xpam)  
[SetWillSavingThrow\_xpAM](#setwillsavingthrow_xpam)  
[SetFortitudeSavingThrow\_xpAM](#setfortitudesavingthrow_xpam)  
[SetReflexSavingThrow\_xpAM](#setreflexsavingthrow_xpam)  

Faction  
[GetFactionID\_xpAM](#getfactionid_xpam)  
[GetIsBaseFaction\_xpAM](#getisbasefaction_xpam)  

Visual  
[SetAppearanceSEF\_xpAM](#setappearancesef_xpam)  
[GetAppearanceSEF\_xpAM](#getappearancesef_xpam)  
[SetColor\_xpAM](#setcolor_xpam)  
[GetColor\_xpAM](#getcolor_xpam)  

</td>
<td style="vertical-align:top">

Position and facing    
[SetFacing\_xpAM](#setfacing_xpam)  
[SetPosition\_xpAM](#setposition_xpam)  
[SetPositionAndFacing\_xpAM](#setpositionandfacing_xpam)  
  
Music    
[MusicBackgroundDayForPC\_xpAM](#musicbackgrounddayforpc_xpam)  
[MusicBackgroundNightForPC\_xpAM](#musicbackgroundnightforpc_xpam)  
[MusicBattleForPC\_xpAM](#musicbattleforpc_xpam)  
[MusicBackgroundRunForPC\_xpAM](#musicbackgroundrunforpc_xpam)  
[MusicBattleRunForPC\_xpAM](#musicbattlerunforpc_xpam)  
  
Refresh, View & Collision    
[RefreshCreatureForPC\_xpAM](#refreshcreatureforpc_xpam)  
[RefreshCreature\_xpAM](#refreshcreature_xpam)  
[RefreshEquipedItemForPC\_xpAM](#refreshequipeditemforpc_xpam)  
[RefreshEquipedItem\_xpAM](#refreshequipeditem_xpam)  
[RefreshObjectForPC\_xpAM](#refreshobjectforpc_xpam)  
[RefreshObject\_xpAM](#refreshobject_xpam)  
[ShowObject\_xpAM](#showobject_xpam)  
[ShowObjectForPC\_xpAM](#showobjectforpc_xpam)  
[MaskObject\_xpAM](#maskobject_xpam)  
[MaskObjectForPC\_xpAM](#maskobjectforpc_xpam)  
[SetObjectDynamicCollision\_xpAM](#setobjectdynamiccollision_xpam)  
[GetObjectDynamicCollision\_xpAM](#getobjectdynamiccollision_xpam)  
   </td>
 </tr>
</table><div style="page-break-after: always;"></div>

xp_aspectManager_creature.nss
=====================

## Global Note
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


<table border="0" style="font-size: 8px">
 <tr>
<td style="vertical-align:top">

  Racial/SubRacial  
[SetCreatureRace\_xpAM](#setcreaturerace_xpam)  
[GetCreatureRace\_xpAM](#getcreaturerace_xpam)  
[SetCreatureSubRace\_xpAM](#setcreaturesubrace_xpam)  
[GetCreatureSubRace\_xpAM](#getcreaturesubrace_xpam)  

Weight  
[RefreshCreatureTotalWeight\_xpAM](#refreshcreaturetotalweightxpAM)  
[GetCreatureTotalWeight\_xpAM](#getcreaturetotalweightxpAM)  
[GetCreatureEquipmentWeight\_xpAM](#getcreatureequipmentweightxpAM)  
[SetCreatureEquipmentWeight\_xpAM](#setcreatureequipmentweightxpAM)  

Other  
[SetCreatureAge\_xpAM](#setcreatureage_xpam)  
[GetCreatureAge\_xpAM](#getcreatureage_xpam)  
[SetCreatureSoundSet\_xpAM](#setcreaturesoundset_xpam)  
[GetCreatureSoundSet\_xpAM](#getcreaturesoundset_xpam)  
[SetCreatureSpecificAC\_xpAM](#setcreaturespecificac_xpam)  
[GetCreatureSpecificAC\_xpAM](#getcreaturespecificac_xpam)  
[SetCreatureTattoos\_xpAM](#setcreaturetattoos_xpam)  
[GetCreatureTattoos\_xpAM](#getcreaturetattoos_xpam)   

  

</td>
<td style="vertical-align:top">
  
Creature Appearance  
[SetCreatureVisualGender\_xpAM](#setcreaturevisualgender_xpam)  
[GetCreatureVisualGender\_xpAM](#getcreaturevisualgender_xpam)  
[SetCreatureHead\_xpAM](#setcreaturehead_xpam)  
[GetCreatureHead\_xpAM](#getcreaturehead_xpam)  
[SetCreatureHair\_xpAM](#setcreaturehair_xpam)  
[GetCreatureHair\_xpAM](#getcreaturehair_xpam)  
[SetCreatureFacialHair\_xpAM](#setcreaturefacialhair_xpam)  
[GetCreatureFacialHair\_xpAM](#getcreaturefacialhair_xpam)  
[SetCreatureTail\_xpAM](#setcreaturetail_xpam)  
[GetCreatureTail\_xpAM](#getcreaturetail_xpam)  
[SetCreatureWings\_xpAM](#setcreaturewings_xpam)  
[GetCreatureWings\_xpAM](#getcreaturewings_xpam)  
[SetCreatureColor\_xpAM](#setcreaturecolor_xpam)  
[GetCreatureColor\_xpAM](#getcreaturecolor_xpam)  
[SetCreaturePersistScale\_xpAM](#setcreaturepersistscale_xpam)  
[GetCreaturePersistScale\_xpAM](#getcreaturepersistscale_xpam)  

Creature Visual  
[SetCreatureModelPartMask\_xpAM](#setcreaturemodelpartmask_xpam)  
[GetCreatureModelPartMask\_xpAM](#getcreaturemodelpartmask_xpam)  
[SetCreatureVisualVariation\_xpAM](#setcreaturevisualvariation_xpam)  
[GetCreatureVisualVariation\_xpAM](#getcreaturevisualvariation_xpam)  
[SetCreatureVisualCategory\_xpAM](#setcreaturevisualcategory_xpam)  
[GetCreatureVisualCategory\_xpAM](#getcreaturevisualcategory_xpam)  
[SetCreatureVisualColor\_xpAM](#setcreaturevisualcolor_xpam)  
[GetCreatureVisualColor\_xpAM](#getcreaturevisualcolor_xpam)  
</td>
<td style="vertical-align:top">
  
Visual ModelPart    
[SetCreatureVisualModelPartVariation\_xpAM](#setcreaturevisualmodelpartvariation_xpam)  
[GetCreatureVisualModelPartVariation\_xpAM](#getcreaturevisualmodelpartvariation_xpam)  
[SetCreatureVisualModelPartCategory\_xpAM](#setcreaturevisualmodelpartcategory_xpam)  
[GetCreatureVisualModelPartCategory\_xpAM](#getcreaturevisualmodelpartcategory_xpam)  
[SetCreatureVisualModelPartColor\_xpAM](#setcreaturevisualmodelpartcolor_xpam)  
[GetCreatureVisualModelPartColor\_xpAM](#getcreaturevisualmodelpartcolor_xpam)  

Visual ModelPiece    
[SetCreatureVisualModelPieceVariation\_xpAM](#setcreaturevisualmodelpiecevariation_xpam)  
[GetCreatureVisualModelPieceVariation\_xpAM](#getcreaturevisualmodelpiecevariation_xpam)  
[SetCreatureVisualModelPieceColor\_xpAM](#setcreaturevisualmodelpiececolor_xpam)  
[GetCreatureVisualModelPieceColor\_xpAM](#getcreaturevisualmodelpiececolor_xpam)  
[SetCreatureVisualModelPieceIsUVScroll\_xpAM](#setcreaturevisualmodelpieceisuvscroll_xpam)  
[GetCreatureVisualModelPieceIsUVScroll\_xpAM](#getcreaturevisualmodelpieceisuvscroll_xpam)  
[SetCreatureVisualModelPieceUScroll\_xpAM](#setcreaturevisualmodelpieceuscroll_xpam)  
[GetCreatureVisualModelPieceUScroll\_xpAM](#getcreaturevisualmodelpieceuscroll_xpam)  
[SetCreatureVisualModelPieceVScroll\_xpAM](#setcreaturevisualmodelpiecevscroll_xpam)  
[GetCreatureVisualModelPieceVScroll\_xpAM](#getcreaturevisualmodelpiecevscroll_xpam)  
   </td>
 </tr>
</table><div style="page-break-after: always;"></div>



xp_aspectManager_area.nss
====

<table border="0" style="font-size: 8px">
 <tr>
<td style="vertical-align:top;">  
  
Area Settings  
[SetAreaFlags\_xpAM](#setareaflags_xpam)  
[GetAreaFlags\_xpAM](#getareaflags_xpam)  
[SetAreaNoRest\_xpAM](#setareanorest_xpam)  
[GetAreaNoRest\_xpAM](#getareanorest_xpam)  
[SetAreaPVPMode\_xpAM](#setareapvpmode_xpam)  
[GetAreaPVPMode\_xpAM](#getareapvpmode_xpam)  
[SetAreaModSpotCheck\_xpAM](#setareamodspotcheck_xpam)  
[GetAreaModSpotCheck\_xpAM](#getareamodspotcheck_xpam)  
[SetAreaModListenCheck\_xpAM](#setareamodlistencheck_xpam)  
[GetAreaModListenCheck\_xpAM](#getareamodlistencheck_xpam)  
[GetAreaCreatorID\_xpAM](#getareacreatorid_xpam)
[SetAreaSkyRing\_xpAM](#setareaskyring_xpam)  
[GetAreaSkyRing\_xpAM](#getareaskyring_xpam)  
[SetAreaHasDirectionalLight\_xpAM](#setareahasdirectionallight_xpam)  
[GetAreaHasDirectionalLight\_xpAM](#getareahasdirectionallight_xpam)  
[SetAreaIsDirLightCastShadow\_xpAM](#setareaisdirlightcastshadow_xpam)  
[GetAreaIsDirLightCastShadow\_xpAM](#getareaisdirlightcastshadow_xpam)  

Day/Night Cycle  
[SetAreaHasDayNightCycle\_xpAM](#setareahasdaynightcycle_xpam)  
[GetAreaHasDayNightCycle\_xpAM](#getareahasdaynightcycle_xpam)  
[SetAreaIsAlwaysNight\_xpAM](#setareaisalwaysnight_xpam)  
[GetAreaIsAlwaysNight\_xpAM](#getareaisalwaysnight_xpam)  
[SetAreaUseDayNightFog\_xpAM](#setareausedaynightfog_xpam)  
[GetAreaUseDayNightFog\_xpAM](#getareausedaynightfog_xpam)  
[SetAreaDNCCoronaIntensity\_xpAM](#setareadnccoronaintensity_xpam)  
[GetAreaDNCCoronaIntensity\_xpAM](#getareadnccoronaintensity_xpam)  
[SetAreaDNCShadowIntensity\_xpAM](#setareadncshadowintensity_xpam)  
[GetAreaDNCShadowIntensity\_xpAM](#getareadncshadowintensity_xpam)  
[SetAreaDNCSkyRingColorInterpretation\_xpAM](#setareadncskyringcolorinterpretation_xpam)  
[GetAreaDNCSkyRingColorInterpretation\_xpAM](#getareadncskyringcolorinterpretation_xpam)  
[SetAreaDNCSkyDomeModel\_xpAM](#setareadncskydomemodel_xpam)  
[GetAreaDNCSkyDomeModel\_xpAM](#getareadncskydomemodel_xpam)  
  
Day/Night Cycle: Desaturate    
[SetAreaDNCDesaturateColor\_xpAM](#setareadncdesaturatecolor_xpam)  
[GetAreaDNCDesaturateColor\_xpAM](#getareadncdesaturatecolor_xpam)  
[SetAreaDNCDesaturateFactor\_xpAM](#setareadncdesaturatefactor_xpam)  
[GetAreaDNCDesaturateFactor\_xpAM](#getareadncdesaturatefactor_xpam)  
</td>
<td style="vertical-align:top">
  
Day/Night Cycle: GroundLight  
[SetAreaDNCGroundLightIntensity\_xpAM](#setareadncgroundlightintensity_xpam)  
[GetAreaDNCGroundLightIntensity\_xpAM](#getareadncgroundlightintensity_xpam)  
[SetAreaDNCGroundLightColor\_xpAM](#setareadncgroundlightcolor_xpam)  
[GetAreaDNCGroundLightColor\_xpAM](#getareadncgroundlightcolor_xpam)  

Day/Night Cycle: Bloom  
[SetAreaDNCBloomBLRadius\_xpAM](#setareadncbloomblradius_xpam)  
[GetAreaDNCBloomBLRadius\_xpAM](#getareadncbloomblradius_xpam)  
[SetAreaDNCBloomGLIntensity\_xpAM](#setareadncbloomglintensity_xpam)  
[GetAreaDNCBloomGLIntensity\_xpAM](#getareadncbloomglintensity_xpam)  
[SetAreaDNCBloomHLIntensity\_xpAM](#setareadncbloomhlintensity_xpam)  
[GetAreaDNCBloomHLIntensity\_xpAM](#getareadncbloomhlintensity_xpam)  
[SetAreaDNCBloomHLThreshold\_xpAM](#setareadncbloomhlthreshold_xpam)  
[GetAreaDNCBloomHLThreshold\_xpAM](#getareadncbloomhlthreshold_xpam)  
[SetAreaDNCBloomSCIntensity\_xpAM](#setareadncbloomscintensity_xpam)  
[GetAreaDNCBloomSCIntensity\_xpAM](#getareadncbloomscintensity_xpam)  
  
Day/Night Cycle: SunMoon  
[SetAreaDNCSunMoonIntensity\_xpAM](#setareadncsunmoonintensity_xpam)  
[GetAreaDNCSunMoonIntensity\_xpAM](#getareadncsunmoonintensity_xpam)  
[SetAreaDNCSunMoonColor\_xpAM](#setareadncsunmooncolor_xpam)  
[GetAreaDNCSunMoonColor\_xpAM](#getareadncsunmooncolor_xpam)  
[SetAreaDNCSunMoonDirX\_xpAM](#setareadncsunmoondirx_xpam)  
[GetAreaDNCSunMoonDirX\_xpAM](#getareadncsunmoondirx_xpam)  
[SetAreaDNCSunMoonDirY\_xpAM](#setareadncsunmoondiry_xpam)  
[GetAreaDNCSunMoonDirY\_xpAM](#getareadncsunmoondiry_xpam)  
[SetAreaDNCSunMoonDirZ\_xpAM](#setareadncsunmoondirz_xpam)  
[GetAreaDNCSunMoonDirZ\_xpAM](#getareadncsunmoondirz_xpam)  
  
Day/Night Cycle: SkyLight  
[SetAreaDNCSkylightIntensity\_xpAM](#setareadncskylightintensity_xpam)  
[GetAreaDNCSkylightIntensity\_xpAM](#getareadncskylightintensity_xpam)  
[SetAreaDNCSkylightColor\_xpAM](#setareadncskylightcolor_xpam)  
[GetAreaDNCSkylightColor\_xpAM](#getareadncskylightcolor_xpam)  
  
Day/Night Cycle: Other Colors  
[SetAreaDNCSkyHorizonColor\_xpAM](#setareadncskyhorizoncolor_xpam)  
[GetAreaDNCSkyHorizonColor\_xpAM](#getareadncskyhorizoncolor_xpam)  
[SetAreaDNCSkyZenithColor\_xpAM](#setareadncskyzenithcolor_xpam)  
[GetAreaDNCSkyZenithColor\_xpAM](#getareadncskyzenithcolor_xpam)  

</td>
<td style="vertical-align:top">
  
Day/Night Cycle: HDR  
[SetAreaDNCHDRAvg\_xpAM](#setareadnchdravg_xpam)  
[GetAreaDNCHDRAvg\_xpAM](#getareadnchdravg_xpam)  
[SetAreaDNCHDRMax\_xpAM](#setareadnchdrmax_xpam)    
[GetAreaDNCHDRMax\_xpAM](#getareadnchdrmax_xpam)  
[SetAreaDNCHDRExposure\_xpAM](#setareadnchdrexposure_xpam)  
[GetAreaDNCHDRExposure\_xpAM](#getareadnchdrexposure_xpam)  
[SetAreaDNCHDRHighlight\_xpAM](#setareadnchdrhighlight_xpam)  
[GetAreaDNCHDRHighlight\_xpAM](#getareadnchdrhighlight_xpam)  

Day/Night Cycle: Cloud  
[SetAreaDNCCloudCover\_xpAM](#setareadnccloudcover_xpam)  
[GetAreaDNCCloudCover\_xpAM](#getareadnccloudcover_xpam)  
[SetAreaDNCCloudMoveX\_xpAM](#setareadnccloudmovex_xpam)  
[GetAreaDNCCloudMoveX\_xpAM](#getareadnccloudmovex_xpam)  
[SetAreaDNCCloudMoveY\_xpAM](#setareadnccloudmovey_xpam)  
[GetAreaDNCCloudMoveY\_xpAM](#getareadnccloudmovey_xpam) 
  
Day/Night Cycle: Fog  
[SetAreaDNCFogStart\_xpAM](#setareadncfogstart_xpam)  
[GetAreaDNCFogStart\_xpAM](#getareadncfogstart_xpam)  
[SetAreaDNCFogEnd\_xpAM](#setareadncfogend_xpam)  
[GetAreaDNCFogEnd\_xpAM](#getareadncfogend_xpam)  
[SetAreaDNCFogFarClip\_xpAM](#setareadncfogfarclip_xpam)  
[GetAreaDNCFogFarClip\_xpAM](#getareadncfogfarclip_xpam)  
[SetAreaDNCFogColor\_xpAM](#setareadncfogcolor_xpam)  
[GetAreaDNCFogColor\_xpAM](#getareadncfogcolor_xpam)  
   </td>
 </tr>
</table><div style="page-break-after: always;"></div>




xp_aspectManager_item.nss
====


<table border="0" style="font-size: 8px">
 <tr>
<td style="vertical-align:top;">  

[SetBaseItemType\_xpAM](#setbaseitemtype_xpam)  
[SetItemAppearanceVariation\_xpAM](#setitemappearancevariation_xpam)  
[GetItemAppearanceVariation\_xpAM](#getitemappearancevariation_xpam)  
[SetItemAppearanceCategory\_xpAM](#setitemappearancecategory_xpam)   
[GetItemAppearanceCategory\_xpAM](#getitemappearancecategory_xpam)  
[SetItemDescription\_xpAM](#setitemdescription_xpam)  
[GetItemDescription\_xpAM](#getitemdescription_xpam)  
[SetItemColor\_xpAM](#setitemcolor_xpam)  
[GetItemColor\_xpAM](#getitemcolor_xpam)  
  
WeaponSpecific  
[SetItemWeaponFX\_xpAM](#setitemweaponfx_xpam)  
[GetItemWeaponFX\_xpAM](#getitemweaponfx_xpam)  
[SetItemWeaponPart\_xpAM](#setitemweaponpart_xpam)  
[GetItemWeaponPart\_xpAM](#getitemweaponpart_xpam)  
</td>
<td style="vertical-align:top">
  
ModelPiece  
[SetItemModelPieceVariation\_xpAM](#setitemmodelpiecevariation_xpam)  
[GetItemModelPieceVariation\_xpAM](#getitemmodelpiecevariation_xpam)  
[SetItemModelPieceColor\_xpAM](#setitemmodelpiececolor_xpam)  
[GetItemModelPieceColor\_xpAM](#getitemmodelpiececolor_xpam)  
[SetItemModelPieceIsUVScroll\_xpAM](#setitemmodelpieceisuvscroll_xpam)  
[GetItemModelPieceIsUVScroll\_xpAM](#getitemmodelpieceisuvscroll_xpam)  
[SetItemModelPieceUScroll\_xpAM](#setitemmodelpieceuscroll_xpam)  
[GetItemModelPieceUScroll\_xpAM](#getitemmodelpieceuscroll_xpam)  
[SetItemModelPieceVScroll\_xpAM](#setitemmodelpiecevscroll_xpam)    
[GetItemModelPieceVScroll\_xpAM](#getitemmodelpiecevscroll_xpam)  
  
ModelPart  
[SetItemModelPartMask\_xpAM](#setitemmodelpartmask_xpam)  
[GetItemModelPartMask\_xpAM](#getitemmodelpartmask_xpam)  
[SetItemModelPartVariation\_xpAM](#setitemmodelpartvariation_xpam)  
[GetItemModelPartVariation\_xpAM](#getitemmodelpartvariation_xpam)  
[SetItemModelPartCategory\_xpAM](#setitemmodelpartcategory_xpam)  
[GetItemModelPartCategory\_xpAM](#getitemmodelpartcategory_xpam)  
[SetItemModelPartColor\_xpAM](#setitemmodelpartcolor_xpam)  
[GetItemModelPartColor\_xpAM](#getitemmodelpartcolor_xpam)  

</td>
 </tr>
</table><div style="page-break-after: always;"></div>

xp_aspectManager_light.nss
===


<table border="0" style="font-size: 8px">
 <tr>
<td style="vertical-align:top;">  

[SetLightRange_xpAM](#setlightrange_xpam)  
[GetLightRange_xpAM](#getlightrange_xpam)  
  
Shadow  
[SetLightCastShadow_xpAM](#setlightcastshadow_xpam)  
[GetLightCastShadow_xpAM](#getlightcastshadow_xpam)  
[SetLightShadowIntensity_xpAM](#setlightshadowintensity_xpam)  
[GetLightShadowIntensity_xpAM](#getlightshadowintensity_xpam)  
  
Light  
[SetLightColorIntensity_xpAM](#setlightcolorintensity_xpam)  
[GetLightColorIntensity_xpAM](#getlightcolorintensity_xpam)  
[SetLightColor_xpAM](#setlightcolor_xpam)  
[GetLightColor_xpAM](#getlightcolor_xpam)  
</td>
<td style="vertical-align:top">
  
Lerp  
[SetLightIsLerp_xpAM](#setlightislerp_xpam)  
[GetLightIsLerp_xpAM](#getlightislerp_xpam)  
[SetLightLerpPeriod_xpAM](#setlightlerpperiod_xpam)  
[GetLightLerpPeriod_xpAM](#getlightlerpperiod_xpam)  
[SetLightLerpIntensity_xpAM](#setlightlerpintensity_xpam)  
[GetLightLerpIntensity_xpAM](#getlightlerpintensity_xpam)  
[SetLightLerpColor_xpAM](#setlightlerpcolor_xpam)  
[GetLightLerpColor_xpAM](#getlightlerpcolor_xpam)  
  
Flicker    
[SetLightIsFlicker_xpAM](#setlightisflicker_xpam)  
[GetLightIsFlicker_xpAM](#getlightisflicker_xpam)  
[SetLightFlickerType_xpAM](#setlightflickertype_xpam)  
[GetLightFlickerType_xpAM](#getlightflickertype_xpam)  
[SetLightFlickerVariance_xpAM](#setlightflickervariance_xpam)  
[GetLightFlickerVariance_xpAM](#getlightflickervariance_xpam)  
[SetLightFlickerRate_xpAM](#setlightflickerrate_xpam)  
[GetLightFlickerRate_xpAM](#getlightflickerrate_xpam)  

</td>
 </tr>
</table><div style="page-break-after: always;"></div>
    
Constants List
====

- [Constants](#constants)
  - [Light Color Types (LIGHT\_COLOR\_TYPE\_\*)](#light-color-types-light_color_type_)
  - [Transition Types (TRANSITION\_TYPE\_\*)](#transition-types-transition_type_)
  - [Flicker Types (FLICKER\_TYPE\_\*)](#flicker-types-flicker_type_)
  - [Armor Model Parts (ARMOR\_MODEL\_PART\_*)](#armor-model-parts-armor_model_part_)
  - [Trigger Types (TRIGGER\_TYPE\_\*)](#trigger-types-trigger_type_)

- [Constants Creature](#constants-creature)
  - [Creature Color Constants (CREATURE\_COLOR\_*)](#creature-color-constants-creature_color_)
  - [Armor Class Type Constants (AC\_TYPE\_*)](#armor-class-type-constants-ac_type_)
  - [Scale Axis Constants (SCALE\_*)](#scale-axis-constants-scale_)

- [Constants Area](#constants-area)
  - [PVP Setting (PVP\_SETTING\_*)](#pvp-setting-pvp_setting_)
  - [Area Flag (AREA\_FLAG\_*)](#area-flag-area_flag_)
  - [DayNights (DAYNIGHT\_*)](#daynights-daynight_)

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

# SetAppearance\_xpAM
```cpp
void SetAppearance_xpAM(object oObject, int iApp);
```
Set Appearance of oObject to iApp. Will be based on respective 2DA tables. Work for Door or Placeable.

## Parameters

* `oObject` - The object to set the Appearance for. (Door or Placeable)
* `iApp` - The corresponding 2DA index of the appearance to set.

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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


<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

# GetTriggerType\_xpAM
```cpp
int GetTriggerType_xpAM(object oObject);
```
Get the trigger type (return a [TRIGGER\_TYPE\_\*](#trigger-types-trigger_type_) constant).

## Parameters

* `oObject` - The object to get the trigger type for.

## Return Value
The trigger type as a [TRIGGER\_TYPE\_\*](#trigger-types-trigger_type_) constant.

<div style="page-break-after: always;"></div>

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

# SetIsInvisibleTransition\_xpAM
```cpp
void SetIsInvisibleTransition_xpAM(object oObject, int bInvisibleTransition);
```
Set the Invisible Transition flag for oObject. Work for Door.

## Parameters

* `oObject` - The object to set the Invisible Transition flag for.
* `bInvisibleTransition` - The flag to enable (TRUE) or disable (FALSE) Invisible Transition.

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

# GetFactionID\_xpAM
```cpp
int GetFactionID_xpAM(object oObject);
```
Return the FactionID of oObject. Work for Creature, Door, Placeable, Trigger.

## Parameters

* `oObject` - The object to get the FactionID for.

## Return Value
The FactionID of the object.

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

# SetFacing\_xpAM
```cpp
void SetFacing_xpAM(object oObject, float fFacing);
```
Cause the oObject to immediately face fFacing. fFacing is expressed as anticlockwise degrees from Due East.

## Parameters

* `oObject` - The object to set the facing for.
* `fFacing` - The facing in anticlockwise degrees from Due East.

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

# RefreshCreatureTotalWeight\_xpAM

```cpp
void RefreshCreatureTotalWeight_xpAM(
    object oCreature
);
```
Will recalculate inventory weight on creature. Usefull if you use [SetCreatureEquipmentWeight_xpAM](#setcreatureequipmentweight\_xpAM).

## Parameters

* `oCreature` - The creature on which you want to force a refresh

---

# GetCreatureTotalWeight\_xpAM

```cpp
int GetCreatureTotalWeight_xpAM(
    object oCreature
);
```
Get the current weight carried by the creature.

## Parameters

* `oCreature` - The creature to get the carried weight for

## Return Value

The current weight carried by the creature

---

# GetCreatureEquipmentWeight\_xpAM

```cpp
int SetCreatureEquipmentWeight_xpAM(
    object oCreature
);
```
Get the current equiped weight of the creature. If you don't set it, this value will be the sum of all equiped items weight.

## Parameters

* `oCreature` - The creature to get the equipment weight for

## Return Value

The current weight of the equipment carried by the creature

---

# SetCreatureEquipmentWeight\_xpAM

```cpp
int SetCreatureEquipmentWeight_xpAM(
    object oCreature,
    int iWeight
);
```
Set the equipment weight of a creature.  
If you change it, the game will still update it in this way :
* Add the weight of an item you equip
* Substract the weight of an item you unequip.

This will be used to Refresh the total weight. The game will take the Equiped Weight and add to it every item in inventory.

This mean that you can use this function to add weight on a creature, or remove some (with specific feat or other for example).

## Parameters

* `oCreature` - The creature to get the carried weight for

<div style="page-break-after: always;"></div>

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

# SetCreatureRace\_xpAM

```cpp
void SetCreatureRace_xpAM(
    object oCreature,
    int iRace
);
```
Set the Race of oCreature

## Parameters

* `oCreature` - The creature to set the race for
* `iRace` - The Race index from racialtype.2DA to use

<div style="page-break-after: always;"></div>

# GetCreatureRace\_xpAM

```cpp
int GetCreatureRace_xpAM(
    object oCreature
);
```
Get the Race of oCreature

## Parameters

* `oCreature` - The creature to get the race for

## Return Value

The race index from racialtype.2DA of the creature.

---

# SetCreatureSubRace\_xpAM

```cpp
void SetCreatureSubRace_xpAM(
    object oCreature,
    int iSubRace
);
```
Set the SubRace of oCreature

## Parameters

* `oCreature` - The creature to set the race for
* `iSubRace` - The SubRace index from racialsubtype.2DA to use

---

# GetCreatureSubRace\_xpAM

```cpp
int GetCreatureSubRace_xpAM(
    object oCreature
);
```
Get the SubRace of oCreature

## Parameters

* `oCreature` - The creature to get the subrace for

## Return Value

The subrace index from racialsubtype.2DA of the creature.

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

---

# SetAreaHasDayNightCycle\_xpAM

```cpp
void SetAreaHasDayNightCycle_xpAM(object oArea, int bHasDNC);
```
Enable or Disable the Day/Night Cycle for oArea

## Parameters

* `oArea` - The area to set the day/night cycle for
* `bHasDNC` - Whether to enable (TRUE) or disable (FALSE) the day/night cycle

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

---

# SetItemModelPartMask\_xpAM

```cpp
void SetItemModelPartMask_xpAM(
    object oItem,
    int iMask
);
```
Change the iModelPart Mask, allowing you to show or hide model part on the Item (armor) visual.

## Parameters

* `oItem` - The item to set mask for
* `iMask` - The model part mask to use

## Note
iMask is bitmask based with : 
0x1: Helm, 0x2: Gloves,  0x4: Boots, 0x8: Belt, 0x10: Cloak

<div style="page-break-after: always;"></div>

# GetItemModelPartMask\_xpAM

```cpp
int GetItemModelPartMask_xpAM(
    object oItem
);
```

Get the iModelPart mask, tell you to which model part are shown or hiden on the item (armor) visual

## Parameters

* `oItem` - The Item to get the model part mask for

## Return Value

The model part mask of the Item.

## Note
Return Value is bitmask based with : 
0x1: Helm, 0x2: Gloves,  0x4: Boots, 0x8: Belt, 0x10: Cloak

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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
* `sColorType` - The [LIGHT\_COLOR\_TYPE\_*](#light-color-types-light_color_type_) constant
* `sRGB` - The RGB string value in the "#FFFFFF" (or #FFFFFFFF) format

<div style="page-break-after: always;"></div>

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
* `sColorType` - The [LIGHT\_COLOR\_TYPE\_*](#light-color-types-light_color_type_) constant

## Return Value

The RGB string value of the chosen light color.

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
* `sColorType` - The [LIGHT\_COLOR\_TYPE\_*](#light-color-types-light_color_type_) constant
* `sRGB` - The RGB string value in the "#FFFFFF" (or #FFFFFFFF) format

<div style="page-break-after: always;"></div>

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
* `sColorType` - The [LIGHT\_COLOR\_TYPE\_*](#light-color-types-light_color_type_) constant

## Return Value

The RGB string value of the chosen light lerp color.

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
Set the light flicker type with a [FLICKER\_TYPE\_\*](#flicker-types-flicker_type_) value.

## Parameters

* `oLight` - The light object
* `iFlickerType` - The [FLICKER\_TYPE\_\*](#flicker-types-flicker_type_) constant

---

# GetLightFlickerType_xpAM

```cpp
int GetLightFlickerType_xpAM(
    object oLight
);
```
Get the [FLICKER\_TYPE\_\*](#flicker-types-flicker_type_) corresponding to the flicker type.

## Parameters

* `oLight` - The light object

## Return Value

The [FLICKER\_TYPE\_\*](#flicker-types-flicker_type_) constant corresponding to the flicker type.

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

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

<div style="page-break-after: always;"></div>

Constants Creature
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


<div style="page-break-after: always;"></div>

Constants Area
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


---

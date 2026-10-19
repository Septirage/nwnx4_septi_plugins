# Changelog

**v1.5.7 (2026/10/16, DLL, NSS and ini)**

- `SetCreatureNameForViewer_xpAM` now handles PCs too: their name in the player list/chat is also changed for the viewer.
- Names set with `SetCreatureNameForViewer_xpAM` are now kept (through creature refresh, `SetName`/`SetDescription`, examine, ...) until `ClearCreatureNameForViewer_xpAM` or `ForgetCreatureNames_xpAM` is called. The new ini option `KeepNameForViewer` (default 1) enables this; set it to 0 to keep the old behaviour (name sent only once).
- Added `ClearCreatureNameForViewer_xpAM` and `ForgetCreatureNames_xpAM`.
- Fixed `RecalculateCreatureTotalWeight_xpAM`. It now only takes `oCreature` (the value parameter has been removed).
- Fixed a possible memory leak in the message management.
- List system: the armor models removed since the last start are now really removed from the visual types.
- List system: `XPAspMan_RenameVisualType` froze the server when the new name was the same as the old one, and could duplicate entries when the new name already existed. A restricted visual type now stays restricted under its new name.
- List system: `XPAspMan_ChangeVisualType` now creates the new visual type if it doesn't exist yet, and does nothing when the old and the new visual types are the same.
- List system: `XPAspMan_AddToVisualType` and `XPAspMan_RemoveFromVisualType` ignore an unknown category.

**v1.5.6 (2026/09/06, DLL, NSS and ini)**

- Added the `SetSpecificName` functions.
- Added the ini option `UseAccountNameForOOCMsg`: use the player account name instead of the character name in OOC messages.
- Added the ini options `PatchSetIcon` (refresh of the icon on `SetIcon`) and `PatchItemPropertyFct` (new ItemProperty functions).
- Fixed a possible bug in the list management functions.
- Fixed the AreaSkyRing functions: they now manage East/West/South instead of always the North part.
- Fixed AreaDNC colors.
- Fixed `Set/GetCreatureVisualModelPieceIsUVScroll_xpAM`.
- Fixed the damage reduction functions.
- Fixed the light `CastsShadow` setter.
- Fixed typos in the nss files.

**v1.5.3 (2026/02/23, DLL and NSS)**

- Added `Get/SetIsDestroyable_xpAM`, `Get/SetIsRaiseable_xpAM` and `Get/SetIsDeadSelectable_xpAM`.
- Added `SetItemCharges_xpAM`.

**v1.5.2 (2025/10/01, DLL)**

- `GetInventoryNbItem_xpAM` and `Get/SetInventoryMaxSize_xpAM` now work for Placeables, Items and Creatures.

**v1.5.1 (2025/08/28, DLL)**

- Removes the saved TargetObjectID from a door transition when needed.

**v1.5 (2025/08/03, DLL and NSS)**

- Lists every aspect for every creature, not just playable races.
- Optimized `refreshMessages`.
- Added `Get/SetNeverShowArmor_xpAM` and `Get/SetNeverDrawHelmet_xpAM`.
- The SaveList file format is now 1.2. Older files are converted automatically.
- **Warning:** the function signatures in `nwnx_aspectmanager_list` are unchanged, but the `sRRG` parameter of many of them now has a different meaning. Check your usages when updating.

**v1.4.6 (2024/10/30, DLL and NSS)**

- Added the cost functions (`Get/SetItemBaseCost_xpAM`, `Get/SetItemNonIdentifiedCost_xpAM`, `Get/SetItemCostModifier_xpAM`) and several damage reduction functions.

**v1.4.5 (2024/10/10, DLL and NSS)**

- Improved memory management.
- Added the carried weight functions (`Get/SetCreatureEquipmentWeight_xpAM`, `RecalculateCreatureTotalWeight_xpAM`, `GetCreatureTotalWeight_xpAM`) and `GetAreaCreatorID_xpAM`.

**v1.4.4 (2024/07/14, DLL and NSS)**

- Fixed a wrong armor type shown on refresh.
- Added the race and subrace functions (`Get/SetCreatureRace_xpAM`, `Get/SetCreatureSubRace_xpAM`).

**v1.4.3 (2024/05/25, DLL)**

- Better handling of old ArmorSaveFiles (before 1.4).

**v1.4.2 (2024/05/18, DLL and NSS)**

- Fixed a possible client crash with changeform spells/abilities.
- Fixed several Area functions.
- Added `Get/SetItemModelPartMask_xpAM`.

**v1.4.1 (2024/05/16, DLL)**

- Fixed a possible crash when setting the position or orientation of a trigger.

**v1.4 (2024/05/15, DLL and NSS)**

- **Major rework and API change. Not compatible with previous versions.**
- Improved startup appearance scan and more robust list saving.
- Belt fix.
- Many new functions.

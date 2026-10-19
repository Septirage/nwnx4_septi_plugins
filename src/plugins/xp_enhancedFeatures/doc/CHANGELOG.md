# Changelog

**v1.2.0 (2026/10/17, DLL, NSS and ini)**

To check when updating from an older version:

- The `Feat` field of the rules now also counts the feats given by items, like the base game does. This applies to every rule file, including the existing ones.
- Weapon Finesse: unarmed attacks are now handled by the rules, with `ItemType = Unarmed`. With `RemoveBaseWeaponFinesseRules = 1`, add `Unarmed` to one of your rules to keep Weapon Finesse when unarmed. The option `RemoveBaseWpnFinesseRules` is renamed `RemoveBaseWeaponFinesseRules` (the old name still works).
- Every option absent from `xp_enhancedFeatures.ini` now keeps the base game behavior. `FixItemPropertyLoad` and `FixNumberOfFeatUsages` were enabled when absent: they are now disabled. The provided `xp_enhancedFeatures.ini` enables them; check that yours contains `FixItemPropertyLoad = 1` and `FixNumberOfFeatUsages = 1`.
- `SetFirstName` on areas is now fixed by `FixSetFirstName`, with the Triggers, Encounters and Waypoints. It was always applied before.
- With `CustomValuesNb`, the custom values are saved in the character files. An older version of the plugin loses them at the next save.

New:

- Rule system: new rule files for the attack bonus (`BonusAttackFeatsFile`), the saving throws (`SavingThrowFile`), the criticals (`CriticalFile`), the Armor Class (`ArmorClassFile`), the damage (`DamageFile`) and the damage reduction (`DamageReductionFile`, which replaces the damage reduction given by feats in the game). Each one has its reload function.
- Custom values: integers stored on each creature (`CustomValuesNb`), read and written with `GetCustomValue_EFF` / `SetCustomValue_EFF`, usable in the rules with `CustomValue(n)`. They can be computed by rules or evolve with time (`CustomValueRuleFile`, reloaded with `XPEnhancedFeatures_ReloadCustomValueFile`).
- Rule system: new `InCombat` keyword for the `Extra` field.
- Rule system: the function names are no longer case sensitive in the Bonus field (as in the Extra field).
- New fixes: `FixDamageReductionSort`, `FixLowerSRFromFeat`, `FixSRItmPrp`, `FixSpellPenetrationAOE`, `FixParryWithFlurry`.
- New functions: `GetSavingThrowVsType_EFF` (option `AddGetSavingThrowVsTypeFct`) and `RefreshCombatInformation_EFF`.

Fixed:

- A division by zero in a rule no longer crashes the server.
- `SetMemorizableSpellNumber_EFF` could freeze the server on an empty list, and created one slot too many.
- The spell functions now check that the index is not negative.
- `DisableSpellCraftSave` had no effect: the plugin read the option with a wrong name.
- `SkillFeatsEFF.ini`: `Ability = NOP` gave a +6 modifier instead of no ability modifier.
- With `SkillFeatsFile`, the base game feat bonuses were lost for one skill (the last one of the list handled by the game).
- `EnhanceStoreRetrieve`: fixed two low level errors, when a Trigger is retrieved and when a Creature is stored, which could cause unexpected behaviors afterwards.
- Example file `ReduceSpeedEFF.ini`: the Camouflage rule used an unknown area type name (`SUBTERRAN` instead of `UNDERGROUND`), so it was ignored.

**v1.1.16 (2026/02/26, DLL and ini)**

- Added `DisableDamageBonusCap`: option to remove the cap of 100 on the damage bonus and penalty.
- Added `FixBleedingWound`: option to make the damage of Bleeding Wound go through damage resistance.

**v1.1.15 (2026/02/26, DLL and ini)**

- Added `FixImmunityOverflow`: option to fix the bug where too much immunity or vulnerability to a damage type turns into the opposite.

**v1.1.14 (2026/02/02, DLL, ini and NSS)**

- Added `FixHasInfiniteSpellClass`: option to make the game use the 2DA value instead of hardcoded classes for infinite spells.
- Added `DefineInfiniteSpells`: option to define specific rules for infinite spells.
- Added the `Get/SetChatDistance_EFF` nwscript functions.
- Added the `SendChatMessageToPC_EFF` nwscript function.
- Added the `SetTimeForPC_EFF` nwscript function.
- Added the `Get/SetHourForArea_EFF` nwscript functions.

**v1.1.12 (2025/09/23, DLL)**

- Added `FixItmPrpDmgBonus`: option to fix the `ItemPropertyDamageBonus*` functions, which could create a Burst property instead of a normal DamageBonus property.

**v1.1.11 (2025/08/28, DLL and ini)**

- Added `PatchAddNewTag`: option to keep the existing indices when an object is created with an existing tag.

**v1.1.10 (2025/08/03, DLL and NSS)**

- Added `EnhanceGetResRef`: option to make `GetResRef` work on areas.
- Added `FixSetFirstName`: option to fix `SetFirstName`, which did not work on Triggers, Encounters and Waypoints.
- Added the management of hit points in the rule engine (`HitPointFeatsFile`).

**v1.1.8 (2025/04/23, DLL)**

- Added `FixNumberOfFeatUsages`: option to properly restore the feat uses left when a PC logs in.
- Added `FixGetEffectType`: option to fix `GetEffectType`, which could return `EFFECT_TYPE_INVALIDEFFECT`.
- Added `FixCalcSafeLocation`: option to make `CalcSafeLocation` really test the area of its location parameter.
- Added `MaxCalcSafeLocation`: option to change the maximum radius of `CalcSafeLocation` (5.0 by default).

**v1.1.7 (2025/01/11, DLL)**

- Fixed a possible issue with the `DisableTumbleAC` option.

**v1.1.6 (2024/12/19, DLL)**

- Fixed a possible crash.

**v1.1.5 (2024/11/29, DLL)**

- Added `DisableTrapSynergy`: option to disable the Set Trap / Disable Trap synergy.
- Added two functions to the rule engine: `Skill` and `Ability`.
- The `Extra` field of the rules accepts numerical comparators (`<`, `<=`, `>`, `>=`, `==`, `!=`) with constant values and functions, which allows to create or manage skill synergies.

**v1.1.4 (2024/10/31, DLL)**

- Added `KeepLocalVarOnSplit`: option to copy the local variables on the new stack after a split.
- Added `OnSplitStackScript`: script called after a stack is split.

**v1.1.3 (2024/10/30, DLL)**

- Fixed a possible server crash after the destruction of an instanced area.

**v1.1.2 (2024/10/28, DLL)**

- Added `FixItemStacking`: option to check the item properties before allowing two items to stack.
- Added `OnItemStackScript`: script called before two items stack, for more checks or operations.
- Added `OnItemBaseCostCalculationScript`: script giving a direct control on the price of the items.

**v1.1.1 (2024/10/26, DLL)**

- `GlobalOnClientEnter` now matches exactly the OnClientEnter behavior.

**v1.1.0 (2024/10/25, DLL, nwnx_enhancedfeatures.nss and config files)**

- Added the ReduceSpeed management in the rule engine, with the `ReduceSpeedFile` option.
- Added the `XPEnhancedFeatures_ReloadReduceSpeedFile` nwscript function.
- Added the `[General]` / `DisableHook` option for every rule file.
- Added new keywords for the `Extra` field: `TrackMode`, `DetectMode`, `StealthMode`, `EncumbranceNone`, `EncumbranceNormal` and `EncumbranceHeavy`.

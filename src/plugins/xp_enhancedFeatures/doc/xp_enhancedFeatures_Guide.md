---
pdf_options:
  format: A4
  margin: 18mm 16mm 20mm 16mm
  outline: true
  displayHeaderFooter: true
  headerTemplate: "<span></span>"
  footerTemplate: "<div style='width:100%; font-size:9px; font-family:Arial; color:#666; text-align:center;'>xp_enhancedFeatures – <span class='pageNumber'></span> / <span class='totalPages'></span></div>"
---

<style>
/* Long table of contents: less space above it, so that it stays on the cover page */
.cover-title { padding-top: 80px; }
.toc { margin-top: 40px; }
</style>

<div class="cover-title">
<div class="cover-heading">xp_enhancedFeatures</div>
<div class="cover-version">V1.2.0 – 2026-10-17 by Septirage</div>
</div>

<div class="toc">

<div class="toc-title">Table of contents</div>

<!-- TOC -->

</div>

<div class="pagebreak"></div>

# Guide for Admin

## Requirements

To use the xp_enhancedFeatures plugin you will need:

- [Nwnx4](https://github.com/nwn2dev/nwnx4)

The [xp_aspectManager](https://septirage.com/nwn2/d/Plugins?id=xpAM) plugin is not required, but some options work with it: the area types (see [Area types](#area-types)) and the creator of an instanced area (see `PatchCloneArea`).

## Installation of xp_enhancedFeatures

1. Put `xp_enhancedFeatures.dll` and `xp_enhancedFeatures.ini` in your nwnx4 folder.
2. Add `nwnx_enhancedfeatures.nss` and `nwnx_enhancedfeatures_spell.nss` to your module (or import them with `xp_enhancedfeatures.erf`). These files contain all the functions you can use (see [nwscript functions](#nwscript-functions)).
3. If you use the rule system, put the rule files you need in your nwnx4 folder, next to `xp_enhancedFeatures.ini` (see [Rule system files](#rule-system-files)).
4. If you use the script hooks, add the scripts to your module (see [Script hooks](#script-hooks)).

Look at our website (<https://septirage.com/nwn2/d/Plugins?id=xpenhancedfeatures>) for an overview.

The plugin comes with example files that you can adapt to your module:

| Files | Role |
|---|---|
| `SpeedFeatsEFF.ini`, `SkillFeatsEFF.ini`, `WeaponFinesseEFF.ini`, `ReduceSpeedEFF.ini`, `HitPointFeatsEFF.ini`, `BonusAttackFeatsEFF.ini`, `SavingThrowEFF.ini`, `CriticalEFF.ini`, `ArmorClassEFF.ini`, `DamageEFF.ini`, `DamageReductionEFF.ini`, `CustomValueEFF.ini` | Example rule files. |
| `sept_ongoldchange_eff`, `sept_globaloncliententer_eff`, `sept_onstarttransition_eff`, `sept_onitemstack_eff`, `sept_onsplit_eff`, `sept_onitembasecost_eff` | Base scripts for the script hooks. |

## xp_enhancedFeatures.ini

The options are presented in the same groups as in the `xp_enhancedFeatures.ini` file.

The **default** value of an option is the value used when the option is absent from the file. It can differ from the value written in the provided `xp_enhancedFeatures.ini`. An absent option always keeps the base game behavior: every fix and every change is disabled by default. The provided `xp_enhancedFeatures.ini` enables the recommended fixes.

### General

#### loglevel

Default: 3.

Log verbosity, written to `xp_enhancedFeatures.txt`: 0 none, 1 error, 2 warning, 3 info, 4 debug, 5 trace.

### Patch activation

These options extend or fix some nwscript functions.

<!-- MINITOC -->

#### EnhanceStoreRetrieve

Default: 0. Set to 1 to enable.

By default, `StoreCampaignObject` and `RetrieveCampaignObject` only work with Creatures and Items. With this option, they also work with Placeables, Doors, Triggers, Waypoints, Lights and PlacedEffects.

Note: `SQLStoreObject` / `SQLRetrieveObject` rely on `StoreCampaignObject` / `RetrieveCampaignObject`, so they can also store these object types in a database.

#### PatchDestroy

Default: 0. Set to 1 to enable the fix.

Fixes the `DestroyObject` function: it works correctly on the Areas created by `CreateInstancedAreaFromSource`, and no longer leaks memory when an object with an inventory (Creature, Chest…) is destroyed.

Without the fix, when an object with an inventory is destroyed, the items in it are not. They stay in memory, unreachable. This happens on every destruction, not only on `DestroyObject` calls: for example, each time a player logs out, the PC is destroyed and leaks its whole inventory.

#### PatchSetTag

Default: 0. Set to 1 to enable the fix.

Without the fix, after a `SetTag`, the object is returned by `GetObjectByTag(newTag)` **and** by `GetObjectByTag(oldTag)`. With the fix, only the new tag finds it.

#### PatchAddNewTag

Default: 0. Set to 1 to enable the fix.

When several objects have the same tag, `GetObjectByTag` reaches them with its second parameter (index). Transitions use the object at index 0.

Without the fix, an object created with an existing tag (or given an existing tag by `SetTag`) is inserted at index 0: the indices of the other objects change, and transitions can go to the new object. With the fix, it is added at the end, and the existing indices are kept.

#### PatchCloneArea

Default: 0. Set to 1 to enable.

`CreateInstancedAreaFromSource` works with every area, including an area created by a previous call of `CreateInstancedAreaFromSource`. The new area also stores the ID of the source area as its creator, which can be read with `GetAreaCreatorID_xpAM` (xp_aspectManager plugin).

#### FixCalcSafeLocation

Default: 0. Set to 1 to enable the fix.

Without the fix, `CalcSafeLocation` only uses the position of `lTestPosition`: the area tested is always the current area of `oCreature`. With the fix, when `bWalkStraightLineRequired` is FALSE, the whole location is tested, area included.

#### MaxCalcSafeLocation

Default: 5.0.

Maximum value of the `fSearchRadius` parameter of `CalcSafeLocation`.

#### FixGetEffectType

Default: 0. Set to 1 to enable the fix.

Without the fix, `GetEffectType` returns `EFFECT_TYPE_INVALIDEFFECT` for several effects. With the fix, it returns the following values, defined in `nwnx_enhancedfeatures.nss`:

<div class="pairs">

| Constant | Value | Constant | Value |
|---|---|---|---|
| `EFFECT_TYPE_CUTSCENEDOMINATED` | 111 | `EFFECT_TYPE_DARKVISION` | 119 |
| `EFFECT_TYPE_DEATH` | 112 | `EFFECT_TYPE_DISAPPEAR` | 120 |
| `EFFECT_TYPE_KNOCKDOWN` | 113 | `EFFECT_TYPE_APPEAR` | 121 |
| `EFFECT_TYPE_DAMAGE` | 114 | `EFFECT_TYPE_SETSCALE` | 122 |
| `EFFECT_TYPE_HEAL` | 115 | `EFFECT_TYPE_SEETRUEHPS` | 123 |
| `EFFECT_TYPE_LINKEFFECT` | 116 | `EFFECT_TYPE_BABMINIMUM` | 124 |
| `EFFECT_TYPE_MODIFYATTACK` | 117 | `EFFECT_TYPE_SUMMONCOPY` | 125 |
| `EFFECT_TYPE_LOWLIGHTVISION` | 118 | `EFFECT_TYPE_SUMMONCREATURE` | 126 |

</div>

#### EnhanceGetResRef

Default: 0. Set to 1 to enable.

`GetResRef` on an area returns its resref (file name) instead of an empty string.

#### AddGetSavingThrowVsTypeFct

Default: 0. Set to 1 to enable.

Enables the `GetSavingThrowVsType_EFF` function, which returns the saving throw of a creature against a type of save (fear, poison…) and an opponent.

### Functionality fixes

These options fix bugs of the game engine. Set them to 1 to enable them.

<!-- MINITOC -->

#### FixItemPropertyLoad

Default: 0.

Without the fix, when a player logs in with equipped items that give bonuses, the bonuses are not removed when the items are unequipped. With the fix, they are properly removed.

#### FixItmPrpDmgBonus

Default: 0.

Without the fix, the `ItemPropertyDamageBonus*` functions (`ItemPropertyDamageBonus`, `ItemPropertyDamageBonusVsRace`…) could create a DamageBonus **Burst** property. With the fix, they create a normal DamageBonus property.

To create a Burst property on purpose, add `| 0x80000000` to the last parameter (`nDamage`).

#### FixSaveThrow

Default: 0.

Without the fix, Will and Reflex saving throws do not fail automatically on a natural 1: if the total is higher than the DC, the save succeeds.

#### FixWildShapeUsage

Default: 0.

Without the fix, when the PC has the *Extra Wild Shape* feat, *Oaken Resilience* and *Elephant's Hide* restore uses of *Wild Shape* instead of consuming them.

#### FixNumberOfFeatUsages

Default: 0. Also enables `FixWildShapeUsage`, even if `FixWildShapeUsage = 0`: the number of uses restored at login needs the right number of Wild Shape uses.

Without the fix, when the PC has an *Extra XXX* feat, the uses of feat XXX are not restored at login but increased by the value of *Extra XXX* (still capped at the maximum). Example: with *Extra Rage*, each login gives "last number of uses + 2". With the fix, the right number of uses is restored.

#### FixDodgeVSDamageTypeRemoval

Default: 0.

Without the fix, when the PC unequips an item with a *Dodge AC Bonus vs Damage Type* property, the game removes this bonus and also the same amount of Dodge AC. This happens at each unequip: the Dodge AC of the creature goes down again and again.

#### FixItemStacking

Default: 0.

Without the fix, two items can stack when they have the same **number** of passive properties (not "on activation"), even if the properties differ: a stack of +1 piercing arrows can go into a stack of +5 arrows. With the fix, the properties themselves are compared.

#### FixDecreaseSpellOnBonusLoss

Default: 0. Values: 0, 1 or 2.

Losing a bonus spell slot (item or buff, with a spell slot or spellcasting ability bonus) on a Sorcerer, Bard or similar caster takes away one available spell, even if the caster has not rested since the bonus was received. Gaining and losing the bonus again and again uses one more spell each time.

Example: a Sorcerer has 5 level 1 spells per day and a ring giving +1. Unequipping the ring gives 4 available spells, as expected. Equipping and unequipping it again gives 3, and so on.

- 0: base game behavior.
- 1: the number of available spells is reduced only if the new maximum is lower than it.
- 2: the number of available spells is never reduced.

#### KeepLocalVarOnSplit

Default: 0.

When a stack is split, the new stack loses all the local variables. With this option, they are copied to the new stack.

#### FixSetFirstName

Default: 0.

Without the fix, `SetFirstName` does not work properly on Areas, Triggers, Encounters and Waypoints.

#### FixHasInfiniteSpellClass

Default: 0.

Without the fix, the game hardcodes which classes have infinite spells (the Warlock always has them, for example) and ignores the 2DA. With the fix, the 2DA value is used. Needed for `DefineInfiniteSpells`.

#### FixImmunityOverflow

Default: 0.

Without the fix, immunity to a damage type above 127% turns into vulnerability (and vice versa). With the fix, immunity and vulnerability are capped at 100%.

#### FixBleedingWound

Default: 0.

With the fix, the damage of *Bleeding Wound* goes through damage resistance.

#### FixDamageReductionSort

Default: 0.

The engine keeps the damage reductions of a creature sorted from the biggest, and uses this order to find the one that applies. Without the fix, a new damage reduction falling between two existing ones is inserted one place too early: with three damage reductions or more, the best applicable one can be ignored.

#### FixLowerSRFromFeat

Default: 0.

Without the fix, the Spell Resistance given by feats cannot be lowered by Spell Resistance decrease effects. With the fix, the decrease applies correctly to it.

#### FixSRItmPrp

Default: 0.

Without the fix, an item giving Spell Resistance only gives it against the first spell. After that, the item must be unequipped and equipped again to get it back, for one more spell. With the fix, the Spell Resistance of the item always applies.

#### FixSpellPenetrationAOE

Default: 0.

Without the fix, against Spell Resistance, the spells with an area of effect do not use the caster level of their caster and its bonuses (Spell Penetration feats…), but a base value of the spell. With the fix, they use the same caster level as the other spells.

#### FixParryWithFlurry

Default: 0.

Without the fix, the engine groups the attacks of a round into three animated attacks. When an attacker makes several attacks in the same one, a parry check is made only against the first. With the fix, a parry check is made against every attack.

### Customizations

These options change base game rules.

<!-- MINITOC -->

#### DisableTumbleAC

Default: 0. Set to 1 to disable the bonus.

By default, the game gives +1 AC for every 10 ranks in Tumble.

#### DisableSpellCraftSave

Default: 0. Set to 1 to disable the bonus.

By default, the game gives +1 to the saving throws against spells for every 5 ranks in Spellcraft.

#### DisableTrapSynergy

Default: 0. Set to 1 to disable the synergy.

By default, the game gives +2 to Set Trap with at least 5 ranks in Disable Trap, and +2 to Disable Trap with at least 5 ranks in Set Trap.

#### DisableDamageBonusCap

Default: 0. Set to 1 to remove the cap.

By default, the damage bonus and penalty of a creature are capped at 100.

#### MonkWeaponList

Default: base game list.

List of the base item types seen as monk weapons, separated by spaces. The base game list is `36 40 50 59 78`.

#### DefineInfiniteSpells

Default: none.

Defines which spell levels of which classes are infinite. Needs `FixHasInfiniteSpellClass`. Format: `idClass:spellLevel;idClass:spellLevel;…`

Example: `DefineInfiniteSpells = 1:0;9:0` gives infinite cantrips to the Bard and the Sorcerer.

#### AllowSetAreaTime

Default: 0. Set to 1 to enable.

Enables the `SetHourForArea_EFF` and `GetHourForArea_EFF` functions.

#### CustomValuesNb

Default: 0 (disabled).

Number of custom values per creature. Custom values are integers stored on the creatures, read and written with `GetCustomValue_EFF` / `SetCustomValue_EFF`, and usable in the rules with the `CustomValue(n)` function. They are saved with the creature (character file included).

The indices go from 0 to `CustomValuesNb` - 1. Some of them can be computed by rules, or evolve with time: see `CustomValueRuleFile` and [CustomValueEFF](#customvalueeff).

### Area types

#### AreaTypeBitX

Default: `AreaTypeBit0 = INTERIOR`, `AreaTypeBit1 = UNDERGROUND`, `AreaTypeBit2 = NATURAL`.

Gives a name to a bit of the area type (the area type can be read and changed with xp_aspectManager). These names are used in the `Area` field of the rules (see [Common fields](#common-fields)).

You are free to rename them (don't forget to update your rule files), and to add more (`AreaTypeBit3`, `AreaTypeBit4`…, up to `AreaTypeBit31`). The numbers must follow each other: the plugin stops reading at the first missing one.

### Rule system files

Each of these options gives the name of a rule file, in the nwnx4 folder. Comment the option out (or leave it empty) to keep the game behavior for this part. The rule files are described in [Rule system](#rule-system).

<div class="names">

| Option | Example file | Reload function |
|---|---|---|
| `SpeedFeatsFile` | `SpeedFeatsEFF.ini` | `XPEnhancedFeatures_ReloadSpeedFile` |
| `SkillFeatsFile` | `SkillFeatsEFF.ini` | `XPEnhancedFeatures_ReloadSkillFile` |
| `WeaponFinesseFile` | `WeaponFinesseEFF.ini` | `XPEnhancedFeatures_ReloadWpnFinesseFile` |
| `ReduceSpeedFile` | `ReduceSpeedEFF.ini` | `XPEnhancedFeatures_ReloadReduceSpeedFile` |
| `HitPointFeatsFile` | `HitPointFeatsEFF.ini` | `XPEnhancedFeatures_ReloadHitPointFile` |
| `BonusAttackFeatsFile` | `BonusAttackFeatsEFF.ini` | `XPEnhancedFeatures_ReloadBonusAttackFile` |
| `SavingThrowFile` | `SavingThrowEFF.ini` | `XPEnhancedFeatures_ReloadSavingThrowFile` |
| `CriticalFile` | `CriticalEFF.ini` | `XPEnhancedFeatures_ReloadCriticalFile` |
| `ArmorClassFile` | `ArmorClassEFF.ini` | `XPEnhancedFeatures_ReloadArmorClassFile` |
| `DamageFile` | `DamageEFF.ini` | `XPEnhancedFeatures_ReloadDamageFile` |
| `DamageReductionFile` | `DamageReductionEFF.ini` | `XPEnhancedFeatures_ReloadDamageReductionFile` |
| `CustomValueRuleFile` | `CustomValueEFF.ini` | `XPEnhancedFeatures_ReloadCustomValueFile` |

</div>

`CustomValueRuleFile` is only read when `CustomValuesNb` is greater than 0.

### Script hooks

Each of these options gives the name of a script (without extension) called on a new event added by the plugin. Comment the option out (or leave it empty) to disable the event. The scripts are described in [Specific scripts](#specific-scripts).

<div class="names">

| Option | Base script | Called when |
|---|---|---|
| `OnGoldChangeScript` | `sept_ongoldchange_eff` | The gold of a creature changes. Can change the amount. |
| `GlobalOnClientEnterScript` | `sept_globaloncliententer_eff` | A PC enters any area, just after the OnClientEnter script of the area. |
| `StartTransitionScript` | `sept_onstarttransition_eff` | A creature uses a door or a transition trigger. Can cancel the transition. |
| `OnItemStackScript` | `sept_onitemstack_eff` | Two items are about to stack. Can forbid it. |
| `OnSplitStackScript` | `sept_onsplit_eff` | A stack of items is split. |
| `OnItemBaseCostCalculationScript` | `sept_onitembasecost_eff` | The base cost of an item is computed. Can change it. |

</div>

## Rule system

The rule system changes hardcoded parts of the game (speed, skills, saving throws, Armor Class…) with rules written in ini files. Each part of the game has its own rule file, enabled by an option of `xp_enhancedFeatures.ini` (see [Rule system files](#rule-system-files)). The plugin comes with an example of each file.

### Common to all rule files

Most of the syntax is the same in every rule file. This part describes it; the parts below give what is specific to each file.

<!-- MINITOC -->

#### File structure

A rule file contains:

- an optional `[General]` section, with the general options of the file;
- the rules, one section each: `[SpecificNameRule1]`, `[SpecificNameRule2]`… The name before the number depends on the file (`[SpeedRule1]`, `[SaveRule1]`…).

The plugin reads rule 1, then rule 2, and so on, as long as it finds the next number. The numbers must follow each other: after a missing number, the next rules are not read.

A rule with an error is not applied, and the error is written to `xp_enhancedFeatures.txt`. The other rules are still applied.

#### DisableHook

Every rule file accepts `DisableHook = 1` in its `[General]` section. The file is then not used at all, and the game behavior comes back for this part.

It is meant for configuration and testing: change the file and call its reload function (`XPEnhancedFeatures_Reload…File`, see [Rule system files](#rule-system-files)), without restarting the server.

#### Common fields

Every rule can contain these fields:

- **Feat**: logical expression on feats, written with their number in `feat.2da`. Example: `Feat = (1 | 2) & !3` (feat 1 or 2, and not feat 3). A feat given by an item counts, as in the base game.
- **Area**: logical expression on the type of the current area, written with the names defined by the `AreaTypeBitX` options. Example: `Area = !INTERIOR & !UNDERGROUND & NATURAL`.
- **Extra**: logical expression on other conditions: special keywords, other rules (`Rule2`), and comparisons of numbers (`Skill(5) > 10`). See below.
- **Bonus**: mathematical expression giving the value of the rule. Some files name it differently (`Impact`, `Min`, `Max`).

Feat, Area and Extra are optional conditions: the rule applies only if all the conditions it has are true. Most files need the Bonus field; each file gives its required fields.

#### Logical expressions

The Feat, Area and Extra fields accept `|` (or), `&` (and), `^` (exclusive or), `!` (not) and parentheses.

Without parentheses, `!` comes first, then `&`, then `^`, then `|`. Example: `1 | 2 & 3` means `1 | (2 & 3)`.

#### Mathematical expressions

The Bonus field (and its other names) accepts `+`, `-`, `*`, `/`, `//` (integer division), `%` (modulus), parentheses and the functions below.

Every file works with whole numbers, except `SpeedFeatsEFF.ini`, which accepts decimal numbers. With whole numbers, `/` is an integer division, and a decimal number is an error.

A division by 0 gives 0.

#### Functions

These functions can be used in the Bonus field, and in the Extra field with a comparison:

- **ClassLevelSum(class, class…)**: sum of the creature's levels in the listed classes. Example: `ClassLevelSum(5, 45)` gives the Monk + Sacred Fist levels.
- **ClassLevelMax(class, class…)**: the biggest level of the creature among the listed classes.
- **Skill(skill)**: ranks put in the skill, without any bonus. Example: `Skill(5)` gives the ranks in Hide.
- **Ability(ability)**: base ability score, with the racial modifier: 0 STR, 1 DEX, 2 CON, 3 INT, 4 WIS, 5 CHA.
- **CustomValue(index)**: the creature's custom value (see [CustomValueEFF](#customvalueeff)).

#### Extra field

The Extra field accepts the logical operators, and:

- the **special keywords** (upper or lower case):
    - `MonkPower`: true for a monk who keeps the monk abilities (no armor…);
    - `EncumbranceNone`, `EncumbranceNormal`, `EncumbranceHeavy`: the creature's encumbrance level;
    - `TrackMode`, `StealthMode`, `DetectMode`: true when the mode is active;
    - `InCombat`: true when the creature is in combat.
- **the other rules**: `RuleX` is true when the rule X of the same file applies. Write `RuleX`, whatever the name of the sections. A rule can only test the rules before it. Example: a rule that applies only if rule 1 applies and rule 2 does not, and the creature has its monk powers: `Extra = Rule1 & !Rule2 & MonkPower`.
- **numerical comparisons**: `<`, `<=`, `>`, `>=`, `==`, `!=` between functions and numbers. Example: `Extra = Skill(5) > 10`.

A comparison can only contain functions and numbers, and functions and numbers can only be used in a comparison.

### SpeedFeatsEFF

Creature movement speed. **This file replaces the base game speed feats**: the example file reproduces them; start from it to add or change rules.

The `[General]` section can contain `CalculationType`:

- `Factor` (default, like the base game): the bonuses are multiplied. Two bonuses of 10% give a factor of 1.1 × 1.1 = 1.21.
- `Sum`: the bonuses are added. Two bonuses of 10% give 1 + 0.1 + 0.1 = 1.20.

Example:

```ini
[General]
CalculationType = Factor
```

Rules:

| Section | Field | Description |
|---|---|---|
| `[SpeedRuleX]` | `Bonus` (required) | Speed bonus, as a decimal number: 0.2 for +20%. A negative value reduces the speed. |
| `[MinSpeedRuleX]` | `Min` (required) | Minimum speed factor. |
| `[MaxSpeedRuleX]` | `Max` (required) | Maximum speed factor. |

For the minimum and the maximum, only the first rule that applies is used: the order matters. In their Extra field, `RuleX` refers to `[SpeedRuleX]`.

### SkillFeatsEFF

Skill bonuses, and the ability used by a skill.

The `[General]` section can contain `RemoveBaseSkillFeatRules = 1`: the skill bonuses given by the base game feats are then removed, and only your rules apply. The Set Trap / Disable Trap synergy is not concerned (see `DisableTrapSynergy`). Remove the option, or set it to 0, to keep the base game feats.

```ini
[General]
RemoveBaseSkillFeatRules = 1
```

Skill bonus rules, `[SkillRuleX]`:

| Field | Description |
|---|---|
| `Skill` (required) | The skill, by its number in `skills.2da`. One skill only. |
| `Bonus` (required) | Bonus to the skill, a whole number. Negative for a penalty. |

`RuleX` in the Extra field only sees the rules of the same skill.

Ability rules, `[SkillAbilityX]`: change the ability used by a skill (for example Strength for Intimidate with a given feat).

| Field | Description |
|---|---|
| `Skill` (required) | The skill, by its number in `skills.2da`. |
| `Ability` (required) | The ability to use: `STR`, `DEX`, `CON`, `INT`, `WIS` or `CHA`. `NOP`: no ability modifier. |

These rules have no Bonus field, and their Extra field cannot use `RuleX`. Only the first rule that applies is used: the order matters.

### WeaponFinesseEFF

Weapons usable with Weapon Finesse.

The `[General]` section can contain `RemoveBaseWeaponFinesseRules = 1` (the old name `RemoveBaseWpnFinesseRules` still works): the base game rules are then removed, and only your rules apply. Otherwise, the base game rules are tested when none of your rules allows the weapon.

```ini
[General]
RemoveBaseWeaponFinesseRules = 1
```

Rules, `[WpnFinesseRuleX]`:

| Field | Description |
|---|---|
| `ItemType` (required) | The weapons concerned: numbers of `baseitems.2da`, separated by spaces. `Unarmed` stands for the attacks without weapon. |
| `CheckSize` | 1: the weapon must also be light enough for the creature's size. 0 or absent: no size check. |

If one rule applies, Weapon Finesse can be used with the weapon. The rules must test the Weapon Finesse feat themselves (`Feat = 42`), as in the example file. These rules have no Bonus field, and their Extra field cannot use `RuleX`.

### ReduceSpeedEFF

Speed reductions of the modes (stealth, detection, tracking) and of the encumbrance. **This file replaces the base game rules**: the example file reproduces them; start from it to add or change rules.

Rules, `[ReduceSpeedRuleX]`:

| Field | Description |
|---|---|
| `Impact` (required) | Number of speed reductions given by the rule. The game uses 1 or 2. Can be negative. |

The impacts of all the rules that apply are added. As in the base game, 1 gives a reduction of 50%, 2 of 75%, and so on. A total of 0 or less gives no reduction.

### HitPointFeatsEFF

Hit points given by feats: create or adjust feats with an impact on the hit points, such as Toughness or Mind over Body.

Rules, `[HitPointRuleX]`:

| Field | Description |
|---|---|
| `Bonus` (required) | Hit points added to the maximum. Negative to remove some. |

### BonusAttackFeatsEFF

Added to the attack bonus: character sheet, attacks and touch attacks.

Rules, `[BonusAttackRuleX]`:

| Field | Description |
|---|---|
| `Bonus` (required) | Added to the attack bonus. |

When a condition used by these rules changes (custom value, area…), the character sheet is only updated at the game's next combat information update. Call `RefreshCombatInformation_EFF` to update it immediately.

### SavingThrowEFF

Bonus to the saving throws. The bonus counts on the character sheet and on every save.

The `[General]` section can contain `RemoveBaseSaveFeatRules = 1`: the saving throw bonuses given by the base game feats are then removed, and only your rules apply. The other bonuses (class, ability, effects) are kept.

Rules, `[SaveRuleX]`:

| Field | Description |
|---|---|
| `Save` (required) | `FORT`, `REFLEX`, `WILL` or `ALL`. Several saves are separated by spaces: `REFLEX WILL`. |
| `Bonus` (required) | Bonus to the save. |
| `SaveType` | The bonus then only applies to the saves against this type, at the save roll (not on the character sheet). Name of a `SAVING_THROW_TYPE_*` constant (`FEAR`, `POISON`, `MIND_SPELLS`, `DISEASE`, `DEATH`, `TRAP`, `SPELL`…) or its number. `SPELL` also applies when a save of another type comes from a spell. |

`RuleX` in the Extra field only sees the rules of the save being computed: when the Will save is computed, a rule that doesn't cover WILL never applies. Only reference rules that cover the same saves. A rule with a SaveType can reference the rules without SaveType of the same save.

### CriticalEFF

Critical hits. Three kinds of rules, each with its own numbering:

| Section | Effect of the Bonus field (required) |
|---|---|
| `[CriticalThreatRuleX]` | The threat range grows by Bonus: 19-20 with Bonus = 2 gives 17-20. Added after the doubling of Improved Critical. The range never starts below 2. |
| `[CriticalConfirmRuleX]` | Bonus is added to the confirmation roll. |
| `[CriticalMultiplierRuleX]` | Bonus is added to the critical multiplier. Never below 1. |

The rules apply to every weapon and both hands (attack, damage and character sheet). `RuleX` in the Extra field only sees the rules of the same kind.

### ArmorClassEFF

Armor Class. Rules, `[ACRuleX]`:

| Field | Description |
|---|---|
| `Type` (required) | `DODGE`, `NATURAL`, `ARMOR`, `SHIELD`, `DEFLECTION`, `UNTYPED` or `INNATE`. |
| `Bonus` (required) | AC bonus. A negative value is a penalty. |

Each rule behaves like an AC effect of its type:

| Type | Stacks | Lost |
|---|---|---|
| `DODGE` | Yes. The total of the dodge bonuses is capped at 20 by the game. | When the creature is denied its Dexterity (flat-footed…). |
| `NATURAL`, `ARMOR`, `SHIELD` | No: only counts when better than the best bonus of the same type already on the creature (effects, item properties, effects against the attacker, other rules). | On a touch attack. |
| `DEFLECTION` | No, as above. | Never. |
| `UNTYPED` | Yes. | Never. |
| `INNATE` | Yes. Same behavior as the AC of the base game feats (Pale Master, Dragon Disciple…). | On a touch attack. |

Penalties follow the same logic: a non stacking penalty only counts when worse than the worst penalty of the same type already on the creature. Dodge, untyped and innate penalties stack.

Nothing is stored: the AC is computed again for each calculation (character sheet, each attack), so removing an item or an effect takes effect immediately. `RuleX` in the Extra field only sees the rules of the same type.

### DamageEFF

Bonus to the physical damage of the attacks. Rules, `[DamageRuleX]`:

| Field | Description |
|---|---|
| `Bonus` (required) | Damage bonus. Can be negative (the game keeps at least 1 damage). Fixed values only: no dice. |

The bonus behaves like the Strength bonus to damage: added to the damage of the weapon with its damage type, for every weapon and both hands (melee, ranged, unarmed), shown on the character sheet, and multiplied on a critical hit (Bonus = 2 gives +6 on a ×3 critical).

As for the attack bonus, call `RefreshCombatInformation_EFF` to update the character sheet when a condition changes.

### DamageReductionEFF

Damage reduction given by feats. **This file replaces the damage reduction that the base game gives to feats**: the rules give it instead. The example file gives exactly the same damage reduction as the base game; change it as you want. The effects (spells), the item properties and the creature's own damage reduction (blueprint) do not change.

Rules, `[DamageReductionRuleX]`:

| Field | Description |
|---|---|
| `Bonus` (required) | Amount of damage reduction. |
| `Bypass` (required) | What goes through this damage reduction (see below). |

Bypass values (the `DR_TYPE_` prefix is accepted):

| Value | Damage reduction | Goes through with |
|---|---|---|
| `NONE` | x/- | Nothing. |
| `MAGICBONUS n` | x/+n | A weapon with an enhancement bonus of at least n (1 to 20). `MAGICBONUS 1` is x/magic. |
| `EPIC` | x/epic | A weapon with an enhancement bonus above +5. |
| `GMATERIAL n` | x/material | A weapon (or ammunition) of the material n (0 to 255). Game values: 0 non specific, 1 adamantine, 2 cold iron, 3 darksteel, 4 iron, 5 mithral, 6 alchemical silver, 7 duskwood, 8 darkwood, 9 red dragon, 10 salamander, 11 umber hulk, 12 wyvern. The materials added in your 2DA work too. |
| `DMGTYPE x` | x/damage type | The damage type x: a `DAMAGE_TYPE_*` name (`SLASHING`, `DAMAGE_TYPE_PIERCING`…) or its value. |
| `ALIGNMENT x` | x/alignment | `LAWFUL`, `CHAOTIC`, `GOOD` or `EVIL`: an attacker with a damage bonus against the opposite alignment (holy weapon…), or the lawful Ki Strike of the monk. The alignment of the attacker itself does not count (game rule). |
| `NON_RANGED` | x/ranged | Applies only against a creature holding a ranged weapon. |

Several bypasses can be joined, all with `|` (goes through with any of them: `GMATERIAL 6 | GMATERIAL 2`) or all with `&` (goes through only with all of them: `GMATERIAL 6 & ALIGNMENT GOOD`). `|` and `&` cannot be mixed in one rule.

How the rules combine:

- the rules with exactly the same Bypass are added into one damage reduction;
- then, as in the base game, only the best damage reduction that is not bypassed applies (rules, effects and items together);
- to keep only the best of several rules instead of adding them, exclude them with the Feat field (`Feat = 216 & !1541`), as in the example file;
- a total of 0 or less gives no damage reduction.

### CustomValueEFF

Custom values are integers stored on every creature, enabled by `CustomValuesNb` (see [CustomValuesNb](#customvaluesnb)). By default, a custom value is **Raw**: it is only read and written by your scripts, with `GetCustomValue_EFF` and `SetCustomValue_EFF`. This file can make some of them **Computed** or **Evolution**.

The `[General]` section declares them:

- `ComputedValues`: list of indices, separated by spaces. Their value is computed by the `[CustomValueRuleX]` rules.
- `EvolutionValues`: list of `index:period` pairs, separated by spaces (period in seconds). Their value changes by itself over time, with the `[CustomValueEvolutionRuleX]` rules.

An index cannot be both Computed and Evolution.

Computed rules, `[CustomValueRuleX]`:

| Field | Description |
|---|---|
| `CustomValue` (required) | Index of the custom value. Must be declared in `ComputedValues`. |
| `Bonus` (required) | Value given by the rule. |

The value is the sum of the Bonus of every rule that applies, 0 if none, computed at each read. `SetCustomValue_EFF` on a Computed index has no effect: `GetCustomValue_EFF` always returns the computed value. These rules can use `CustomValue(n)`, but never to read another Computed index.

Evolution rules, `[CustomValueEvolutionRuleX]`, with their own numbering:

| Field | Description |
|---|---|
| `CustomValue` (required) | Index of the custom value. Must be declared in `EvolutionValues`. |
| `Bonus` (required) | Value added at each period. |

Each time the period of the index has elapsed, the sum of the Bonus of every rule that applies (0 if none) is added to the stored value. The value can still be set by your scripts. These rules can use `CustomValue(n)` with any index, Computed included.

`XPEnhancedFeatures_ReloadCustomValueFile` reloads this file. The values stored on the creatures are kept; an index that is no longer Computed gives its stored value again. An index that becomes Evolution again may evolve once immediately.

## Specific scripts

These scripts are called on the events added by the plugin. Each one is enabled by an option of `xp_enhancedFeatures.ini` (see [Script hooks](#script-hooks)), and the plugin comes with a base script for each, to use as a template.

<!-- MINITOC -->

#### OnGoldChange

By enabling the `OnGoldChangeScript` option, the script is called each time the amount of gold of a creature changes. Base script: `sept_ongoldchange_eff`.

```c
int StartingConditional(object oCreature, int iChangeOfGold)
```

- `oCreature`: the creature whose amount of gold changes.
- `iChangeOfGold`: the quantity of gold added (positive value) or taken (negative value).

Return value: the quantity of gold really added (positive) or taken (negative) to/from `oCreature`.

#### GlobalOnClientEnter

By enabling the `GlobalOnClientEnterScript` option, the script is called as the OnClientEnter script of an area, but for every area. It is called just after the OnClientEnter script of the area itself. Base script: `sept_globaloncliententer_eff`.

It is a basic `void main()` script, with exactly the same behavior as a normal OnClientEnter. So you can use:

- `GetFirstEnteringPC()`: the first entering PC;
- `GetNextEnteringPC()`: the next entering PC, to use with `GetFirstEnteringPC()` to go through the list of the entering players;
- `FiredFromPartyTransition()`;
- `OBJECT_SELF`: the destination area. `GetArea(oPC)` gives it too, as the PCs are already in the new area.

#### StartTransition

By enabling the `StartTransitionScript` option, the script is called when a creature tries a transition, with a door or a transition trigger. It is not called in other cases (a jump by script, for example). Base script: `sept_onstarttransition_eff`.

```c
int StartingConditional(object oCreature, object oTransitionObject)
```

- `oCreature`: the creature that tries the transition.
- `oTransitionObject`: the object used for the transition (door or transition trigger).

Return value: TRUE to let the transition continue normally, FALSE to stop it.

#### OnItemStack

By enabling the `OnItemStackScript` option, the script is called when two items are about to stack. Base script: `sept_onitemstack_eff`.

```c
int StartingConditional(object oDestinationStack, object oSourceStack)
```

- `oDestinationStack`: the item (stack) that will receive the items of `oSourceStack`.
- `oSourceStack`: the item (stack) that will be merged into `oDestinationStack`.

Return value: TRUE to allow the stack, FALSE to forbid it.

The script is only called when the game has already accepted the stack. Two items can stack when they have:

- the same BaseItemType;
- the same IsIdentified, IsPlot, IsStolen and IsCursed status;
- the same Tag;
- the same LocalizedName;
- the same ModelPart0/1/2 (if applicable);
- the same ArmorRuleType (if applicable);
- the same "on activation" properties;
- the same number of passive properties, or the same passive properties with `FixItemStacking`.

When two items stack, the number of items of `oDestinationStack` (`GetNumStackedItems`) grows by the number of items of `oSourceStack`, and only `oDestinationStack` remains: any other difference between the two is lost. This script lets you add checks, or do operations before allowing the stack.

#### OnSplit

By enabling the `OnSplitStackScript` option, the script is called when a stack is split. Base script: `sept_onsplit_eff`.

```c
int StartingConditional(object oSourceStack, object oNewStack)
```

- `oSourceStack`: the stack that has just been split.
- `oNewStack`: the new stack.

Return value: not used.

#### OnItemBaseCostCalculation

By enabling the `OnItemBaseCostCalculationScript` option, the script is called when the base cost of an item is computed: creation, item properties added or removed, activation, opening or loading of a store… Base script: `sept_onitembasecost_eff`.

```c
int StartingConditional(object oItem, int iCost)
```

- `oItem`: the item.
- `iCost`: the base cost just computed by the game.

Return value: the base cost to apply to `oItem`.

Note: the base cost is not the sell or buy price. The final price depends on the store configuration, the Appraise skill, and BaseCost + ModifierCost (or the price of an unidentified item).

## nwscript functions

The functions are in two files: `nwnx_enhancedfeatures.nss` and `nwnx_enhancedfeatures_spell.nss`. The comments of these files describe each function and its parameters; this part only gives an overview.

<!-- MINITOC -->

#### Reload of the rule files

In `nwnx_enhancedfeatures.nss`, one function per rule file reloads it without restarting the server: `XPEnhancedFeatures_ReloadSpeedFile`, `XPEnhancedFeatures_ReloadSkillFile`, `XPEnhancedFeatures_ReloadWpnFinesseFile`, `XPEnhancedFeatures_ReloadReduceSpeedFile`, `XPEnhancedFeatures_ReloadHitPointFile`, `XPEnhancedFeatures_ReloadBonusAttackFile`, `XPEnhancedFeatures_ReloadSavingThrowFile`, `XPEnhancedFeatures_ReloadCriticalFile`, `XPEnhancedFeatures_ReloadArmorClassFile`, `XPEnhancedFeatures_ReloadDamageFile`, `XPEnhancedFeatures_ReloadDamageReductionFile` and `XPEnhancedFeatures_ReloadCustomValueFile`.

They only work for a file enabled in `xp_enhancedFeatures.ini` when the server started.

#### Custom values

In `nwnx_enhancedfeatures.nss`: `GetCustomValue_EFF` and `SetCustomValue_EFF` read and write a custom value of a creature. Needs `CustomValuesNb` (see [CustomValueEFF](#customvalueeff)).

#### Combat

In `nwnx_enhancedfeatures.nss`:

- `RefreshCombatInformation_EFF`: asks the game to compute the combat information of a creature again (attack bonus, damage… on the character sheet). Use it when something used by your rules changes (custom value, area…).
- `GetSavingThrowVsType_EFF`: saving throw of a creature against a type of save (fear, poison…) and an opponent. Needs `AddGetSavingThrowVsTypeFct`.
- The `EFFECT_TYPE_*` constants returned by `GetEffectType` with `FixGetEffectType`.

#### Chat

In `nwnx_enhancedfeatures.nss`:

- `GetChatDistance_EFF` and `SetChatDistance_EFF`: distance of the messages on the talk and whisper volumes.
- `SendChatMessageToPC_EFF`: sends a chat message to one PC only, without distance check and without calling the chat callback or the OnListen scripts. For the talk and whisper channels, the sender must be in the same area.

#### Time

In `nwnx_enhancedfeatures.nss`, for display purposes:

- `SetTimeForPC_EFF`: changes the hour for one PC only.
- `SetHourForArea_EFF` and `GetHourForArea_EFF`: hour forced for an area. The PCs in this area receive it on loading and at every automatic refresh. Needs `AllowSetAreaTime`.

#### Spells

In `nwnx_enhancedfeatures_spell.nss`, functions to read and change the spell book of a creature:

- memorized spells: `GetMemorizedSpellId_EFF`, `GetMemorizedSpellReady_EFF`, `GetMemorizedSpellMetaMagic_EFF`, `GetMemorizedSpellIsDomainSpell_EFF`, `GetMemorizedSpellCountByLevel_EFF`, `SetMemorizedSpell_EFF`, `SetMemorizedSpellReady_EFF`, `ClearMemorizedSpell_EFF`, `ClearMemorizedSpellBySpellId_EFF`;
- known spells: `GetIsInKnownSpellList_EFF`, `GetKnownSpellCount_EFF`, `GetKnownSpellId_EFF`;
- number of spells: `GetMaxNumberOfSpell_EFF`, `SetMemorizableSpellNumber_EFF`, `GetNumberOfSpellUsesLeft_EFF`, `SetNumberOfSpellUsesLeft_EFF`, `SetNumberOfBonusSpell_EFF`.

<div class="pagebreak"></div>

<!-- INCLUDE CHANGELOG.md -->

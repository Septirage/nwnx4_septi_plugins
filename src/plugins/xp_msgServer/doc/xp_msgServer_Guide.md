---
pdf_options:
  format: A4
  margin: 18mm 16mm 20mm 16mm
  outline: true
  displayHeaderFooter: true
  headerTemplate: "<span></span>"
  footerTemplate: "<div style='width:100%; font-size:9px; font-family:Arial; color:#666; text-align:center;'>xp_msgServer – <span class='pageNumber'></span> / <span class='totalPages'></span></div>"
---

<div class="cover-title">
<div class="cover-heading">xp_msgServer</div>
<div class="cover-version">V1.3.5 – 2026-10-17 by Septirage</div>
</div>

<div class="toc">

<div class="toc-title">Table of contents</div>

<!-- TOC -->

</div>

<div class="pagebreak"></div>

# Guide for Admin

## Requirements

To use the xp_msgServer plugin you will need:

- [Nwnx4](https://github.com/nwn2dev/nwnx4)
- xp_bugfix (included in the nwnx4 package)

## Installation of xp_msgServer

1. Put `xp_msgServer.dll` and `xp_msgServer.ini` in your nwnx4 folder.
2. Add `nwnx_msgServer.nss` to your module (or import it with `xp_msgServer.erf`). This file contains all the functions you can use.
3. The plugin comes with example files, to adapt to your module:

| File | Role |
|---|---|
| `sept_heimdall_demo_onconnect.NSS` | Example of connection script (see [Connection system](#connection-system)). |
| `gui_sept_heimdall_demo.NSS` | Example of script called from the connection GUI. |
| `sept_heimdall_demo_login.xml` | Example of connection GUI panel. |
| `sept_anticheat_creation.NSS` | Example of script called on character creation error. |
| `sept_anticheat_levelup.NSS` | Example of script called on level up error. |

## Configuration of xp_msgServer

### xp_msgServer.ini

<div class="options">

| Option | Default | Description |
|---|---|---|
| `loglevel` | 3 | Log verbosity: 0 none, 1 error, 2 warning, 3 info, 4 debug, 5 trace. |
| `LogEveryMsg` | 0 | Debug only: with a `loglevel` of at least info, logs every message received by the server. Not for production. |
| `UseConnectionSystem` | 1 | Set to 1 to use the whole connection system. |
| `OnConnectionScript` | — | Script called at the first step of the connection, which starts your connection securing process. |
| `UseAnticheatCreation` | 1 | Set to 1 to enable the anticheat for character creation. |
| `StopCreationFirstViolation` | 1 | 1: the script is called as soon as a violation is detected. 0: every violation is listed and the script is called once, at the end. See [When the scripts are called](#when-the-scripts-are-called). |
| `ScriptOnCreationError` | — | Script called when a creation error is detected (without the `.nss` extension). |
| `MinScaleX` / `MinScaleZ` | 0.95 | Minimal scale allowed for a new character. |
| `MaxScaleX` / `MaxScaleZ` | 1.05 | Maximal scale allowed for a new character. |
| `UseAnticheatLvlUp` | 1 | Set to 1 to enable the anticheat for level up. |
| `StopLvlUpFirstViolation` | 1 | Same as `StopCreationFirstViolation`, for level up. |
| `ScriptOnLvlUpError` | — | Script called when a level up error is detected (without the `.nss` extension). |
| `RangerCombatStyleFeats` | 1729 1730 | List of the Ranger combat style feats, separated by spaces. |
| `GrantedCondForEveryFeats` | 0 | Needs changes in the level up panel: some modifications allow the use of `GrantedOnLevel` and `GrantedPrereq` for every feat list type. If you made them, set this option to 1 so the checks stay correct. |

</div>

## Connection system

The connection system triggers a script just after the end of the hak download, before any other possible action. This allows custom authentication and validation logic.

See the provided example files (`sept_heimdall_demo_onconnect.NSS`, `gui_sept_heimdall_demo.NSS` and `sept_heimdall_demo_login.xml`) and the comments of `xp_msgServer.ini` for a practical implementation.

### OnConnection script

This script has 5 parameters:

| Parameter | Description |
|---|---|
| `int iUniquePlayerID` | The UniquePlayerID of this connection. It is needed by the specific msgServer functions. |
| `string sPlayerName` | The incoming player account name (gamespy account, see `GetPCPlayerName`). |
| `string sIP` | The IP of the incoming player (see `GetPCIPAddress`). |
| `string sCDKey` | The public CDKey of the incoming player (see `GetPCPublicCDKey`). |
| `int iPrivileges` | Player/DM/Admin privileges of the incoming player (see `XPMsgServer_GetHasXXXPrivilege`). |

Return value: an `XPMSGSRV_HEIMDALL_RET_*` constant.

- `XPMSGSRV_HEIMDALL_RET_WAIT`: do nothing and wait for further actions before authenticating the player.
- `XPMSGSRV_HEIMDALL_RET_ALLOW`: allow the player to enter the server.
- `XPMSGSRV_HEIMDALL_RET_KICK`: kick the player out of the server (all future messages from this connection are discarded).

Note: the names of the constants contain `HEIMDALL` for historical reasons, but they are used by the connection system whatever your implementation.

### Functions

All these functions are available by including `nwnx_msgServer`. The file also documents each of them.

General management, to enable or disable a part of the plugin:

| Function | Role |
|---|---|
| `XPMsgServer_SetAntiCheatLvlUpSystem` | Enables or disables the anticheat for level up. |
| `XPMsgServer_SetAntiCheatCreationSystem` | Enables or disables the anticheat for character creation. |
| `XPMsgServer_SetConnectionSystem` | Enables or disables the connection system. |

`XPMsgServer_SetAuthorizedGUIScript` allows a script to be called by a player who is not authenticated yet, from a `UIObject_Misc_ExecuteServerScript` GUI callback. The script is executed with 5 pre-inserted parameters given by msgServer (the same as the OnConnection script), followed by everything added by the `UIObject_Misc_ExecuteServerScript` call. `OBJECT_SELF` is the module. The return value works like the OnConnection script one.

GUI functions: they manage the GUI of a player who is not loaded into the module yet and so has no object. They need the `iUniquePlayerID` instead of the usual `oPlayer`, so they must be used in the OnConnection script, in the authorized scripts or in scripts related to them. Apart from that, they are very close to the original GUI functions:

| Function | Original function |
|---|---|
| `XPMsgServer_DisplayGuiScreen` | `DisplayGuiScreen` |
| `XPMsgServer_CloseGUIScreen` | `CloseGUIScreen` |
| `XPMsgServer_SetGUIObjectHidden` | `SetGUIObjectHidden` |
| `XPMsgServer_SetGUIObjectDisable` | `SetGUIObjectDisable` |
| `XPMsgServer_SetGUIObjectText` | `SetGUIObjectText` |
| `XPMsgServer_DisplayMessageInfo` | Reduced version of `DisplayMessageBox` |

Privileges: `XPMsgServer_GetHasPlayerPrivilege`, `XPMsgServer_GetHasDMPrivilege` and `XPMsgServer_GetHasAdminPrivilege` tell if an `iPrivileges` value contains the Player, DM or Admin privileges.

`XPMsgServer_SetAllowPlayer` is for advanced use: it allows a player to enter the module when you are not directly in the OnConnection script or in an authorized script (for example after an `AssignCommand` or a `DelayCommand`).

### Connection flow example

```text
Player connects
    |
OnConnectionScript called
(receives: iUniquePlayerID, sPlayerName, sIP, sCDKey, iPrivileges)
    |
    +--> Return ALLOW
    |      +--> Player can continue on the server normally
    |
    +--> Return KICK
    |      +--> All further messages from this player are ignored
    |
    +--> DisplayGUI --> Return WAIT
           +--> Player remains in pending state, custom processing continues
                  |
              Player interacts with the GUI
                  |
              UIObject_Misc_ExecuteServerScript triggers the AuthorizedScript
              (receives the same 5 parameters as OnConnectionScript + additional parameters)
                  |
                  +--> CloseGUI --> Return ALLOW
                  |      +--> Player can continue on the server normally
                  |
                  +--> CloseGUI --> Return KICK
                  |      +--> All further messages from this player are ignored
                  |
                  +--> Update GUI, etc. --> Return WAIT
                         +--> Loop continues (back to "Player interacts with the GUI")
```

## Anticheat system

The anticheat validates the character creation and the level ups against the game rules. When violations are detected, a list of errors is written in the plugin log and given to the script you configured (`ScriptOnCreationError` and `ScriptOnLvlUpError`). That script decides if the creation or the level up is cancelled or accepted. Without a configured script, any violation cancels the operation.

### The error scripts

Creation script:

```c
int StartingConditional(string sPlayerName, string sErrorList)
```

Level up script:

```c
int StartingConditional(string sPlayerName, object oCharacter, string sErrorList)
```

| Parameter | Description |
|---|---|
| `sPlayerName` | The "Bioware" account of the player. |
| `oCharacter` | (level up only) The character concerned. Nothing has been applied yet. |
| `sErrorList` | The list of all the errors detected so far. The most recent one is the last. See [Error format](#error-format). |

Return value: **FALSE** if this is a real error and the operation must be cancelled, **TRUE** if you tolerate it and the plugin can go on.

### When the scripts are called

- `StopCreationFirstViolation` / `StopLvlUpFirstViolation` set to **1** (default): the script is called each time a new violation is detected. If you tolerate it (TRUE), the plugin goes on and calls the script again at the next violation, with the errors detected so far plus the new one.
- Set to **0**: the plugin lists as many violations as possible and calls the script once, at the end, with all of them.

In both cases, **judge the whole list each time**: return TRUE only if you tolerate every error present in the list.

### Error format

Each error has the form:

```text
#ErrorName:{key1=value1,key2=value2}
```

Errors are simply concatenated:

```text
#TooManySkillPoints:{allowed=12,used=14}#SkillRankTooHigh:{skills=5:3 7:2}
```

A value is a number, or a list of items separated by spaces (`12 40 77`). An item can be a tuple whose elements are separated by `:`; `5:3 7:2` is the list of the tuples `5:3` and `7:2`. An error without parameter is written `#ErrorName:{}`.

The plugin also writes this list as it is in its log.

### Reading the error list

`nwnx_msgServer.nss` provides functions to read `sErrorList`, so you don't have to cut the string yourself:

| Function | Result |
|---|---|
| `XPMsgServer_AC_GetErrorCount(sErrorList)` | Number of errors. |
| `XPMsgServer_AC_GetErrorString(sErrorList, nIndex)` | The nIndex-th error (0 is the first) as text, without the `#`. Useful to log it. |
| `XPMsgServer_AC_GetErrorCode(sErrorList, nIndex)` | The name of the nIndex-th error. |
| `XPMsgServer_AC_HasError(sErrorList, sCode)` | TRUE if an error of this name is present. |
| `XPMsgServer_AC_GetErrorParam(sErrorList, nIndex, sKey)` | The value of a parameter, as text. |
| `XPMsgServer_AC_GetErrorParamInt(sErrorList, nIndex, sKey)` | The value of a parameter, as an integer. |
| `XPMsgServer_AC_GetListSize(sValue)` | Number of items of a list value. |
| `XPMsgServer_AC_GetListItem(sValue, nIndex)` | The nIndex-th item of a list value. |

For a list of tuples, read a tuple first, then its elements:

```c
string sSkills = XPMsgServer_AC_GetErrorParam(sErrorList, 0, "skills");   // "5:3 7:2"
int nTuples = XPMsgServer_AC_GetListSize(sSkills);                         // 2
string sTuple = XPMsgServer_AC_GetListItem(sSkills, 1);                    // "7:2"
int iSkill = StringToInt(XPMsgServer_AC_GetListItem(sTuple, 0, ":"));      // 7
int iRank  = StringToInt(XPMsgServer_AC_GetListItem(sTuple, 1, ":"));      // 2
```

### Example script

This script tolerates the errors listed in `IsTolerated` and refuses everything else:

```c
#include "nwnx_msgServer"

int IsTolerated(string sErrorCode)
{
    if (sErrorCode == "SpellsWithoutSpellPanel")
        return TRUE;
    return FALSE;
}

int StartingConditional(string sPlayerName, object oCharacter, string sErrorList)
{
    int nCount = XPMsgServer_AC_GetErrorCount(sErrorList);
    int i;
    for (i = 0; i < nCount; i++)
    {
        if (!IsTolerated(XPMsgServer_AC_GetErrorCode(sErrorList, i)))
            return FALSE;
    }
    return TRUE;
}
```

The provided examples `sept_anticheat_creation` and `sept_anticheat_levelup` follow the same structure.

### Level up errors

<div class="errors">

| Error | Parameters | Meaning |
|---|---|---|
| `TooManyClasses` | `owned` (4 class ids), `chosen` | The character already has 4 classes and takes a 5th. |
| `ClassNotValid` | `class` | Unknown class id. This error ends the verification: if your script tolerates it, the level up is accepted without any other check. |
| `ClassNotPlayer` | `class` | The class is not a player class. |
| `ClassLevelTooHigh` | `class`, `level`, `max` | The level asked is above the maximum level of the class. |
| `ClassAlignmentRestricted` | `class`, `ge`, `lc` | The class is not allowed for this alignment. |
| `ClassPrereqNotMet` | `class` | The requirements of the class are not met. |
| `RangerCombatStyleMissing` | — | A Ranger reaches level 7 without any combat style feat (see `RangerCombatStyleFeats`). |
| `TooManyKeptSkillPoints` | `kept`, `max` | More unspent skill points kept than allowed. |
| `SkillNotAllowed` | `skills` (skill ids) | Restricted or removed skills received points. |
| `SkillRankTooHigh` | `skills` (tuples `id:rank`) | Skills whose rank after the level up is above their maximum rank. |
| `TooManySkillPoints` | `allowed`, `used` | More skill points used than available. |
| `AutoFeatNotGranted` | `feats` | Automatically granted feats that are not in the message. |
| `FamiliarTypeChanged` | `old`, `new` | The familiar type changed without a legitimate choice. |
| `CompanionTypeChanged` | `old`, `new` | The animal companion type changed without a legitimate choice. |
| `DomainChanged` | `domains` (2 ids), `expected` (2 ids) | The domains changed. |
| `SchoolChanged` | `school`, `expected` | The magic school changed. |
| `TooManyFeats` | `taken`, `allowed` | More feats taken than allowed (bonus + general + domain feats). |
| `FeatSelectionInvalid` | `impossible` (feat ids), `bonusUsed`, `bonusAllowed`, `normalUsed`, `normalAllowed` | Impossible feats, or too many "bonus" or "normal" feats. |
| `FeatPrereqNotMet` | `feats` | Feats whose prerequisites are not met. |
| `SpellSchoolForbidden` | `spell` | A spell of a forbidden school (specialist wizard). |
| `SpellLevelMismatch` | `spell`, `level`, `slot` | A spell put in a slot of a different level than its own. |
| `SpellNotAvailable` | `spell` | The spell does not exist, is not available or has been removed. |
| `SpellClassNotOwned` | `class` | Spells sent for a spell casting class the character does not have. |
| `SpellCasterLevelOutOfRange` | `level` | Caster level outside 1 to 40 with spells sent. |
| `TooManySpellsTaken` | `taken`, `max` | More new spells taken than allowed, all spell levels together. |
| `TooManySpellsTakenAtLevel` | `spellLevel`, `taken`, `max` | More new spells taken than allowed for one spell level. |
| `SpellLevelTooHigh` | `spellLevel`, `max` | A spell of a level above the maximum accessible. |
| `SpellRemovedNotKnown` | `spell` | A removed spell the character does not know. |
| `SpellRemovedLevelTooHigh` | — | A removed spell of a level above the allowed one. |
| `TooManySpellsRemoved` | `removed` | Too many spells removed. |
| `SpellsWithoutSpellPanel` | `class` | Spells sent for a class that has no spell panel. |

</div>

### Character creation errors

<div class="errors">

| Error | Parameters | Meaning |
|---|---|---|
| `RaceNotValid` | `race`, `subrace` | Invalid race or subrace (not playable, inconsistent base race, or unknown subrace). |
| `AbilityScoresOutOfBounds` | `str`, `dex`, `con`, `int`, `wis`, `cha` | A base ability is outside 8 to 18 (the casting ability may go below 8). |
| `AbilityPointsWrong` | `str`, `dex`, `con`, `int`, `wis`, `cha`, `used` | The total of ability points used is not 32. |
| `AlignmentInvalid` | `ge`, `lc` | The alignment is not one of the default values (15, 50 or 85 on each axis). |
| `ClassNotValid` | `class` | Unknown class. |
| `ClassPrereqNotMet` | `class` | The requirements of the class are not met (reported once per class). |
| `SkillListSize` | `listed`, `expected` | The number of skill entries does not match the expected one. |
| `SkillRankTooHigh` | `skills` (tuples `id:rank`) | Skills whose rank is above their maximum rank. |
| `SkillNotAllowed` | `skills` (skill ids) | Forbidden skills received points. |
| `TooManySkillPoints` | `allowed`, `used` | More skill points used than available. |
| `TooManyKeptSkillPoints` | `kept`, `max` | More points kept than unspent points. |
| `RacialFeatNotGranted` | `feats` | Expected racial feats are missing. |
| `AutoFeatNotGranted` | `feats` | Expected automatically granted feats are missing. |
| `BackgroundReqNotMet` | `background` | The requirements (ability minimum and maximum) of the background are not met. |
| `FeatSelectionInvalid` | `impossible` (feat ids), `bonusUsed`, `bonusAllowed`, `normalUsed`, `normalAllowed` | Impossible feats, or too many "bonus" or "normal" feats. |
| `TooManyBackgroundFeats` | — | More than one background feat. |
| `FeatPrereqNotMet` | `feats` | Feats that are not validated. |
| `SpellSchoolForbidden` | `spell` | A spell of a forbidden school (wizard). |
| `SpellLevelMismatch` | `spell`, `level`, `slot` | A spell put in a slot of a different level than its own. |
| `SpellNotAvailable` | `spell` | Unknown or removed spell. |
| `SpellCountPerLevelWrong` | `levels` (tuples `spellLevel:allowed:taken`) | The number of spells taken for a spell level is above the allowed one. |
| `WizardStartingSpellsWrong` | `cantrips`, `level1`, `level1Max`, `declared` | Wizard: invalid cantrips, too many level 1 spells, or a declared total different from cantrips plus level 1. |
| `SpellCountMismatch` | `counted`, `declared` | The number of spells counted differs from the number declared (`counted=0` for a class without spells). |
| `ModelScaleMissing` | — | The `ModelScale` structure is missing. |
| `ScaleXYDifferent` | `x`, `y` | The X and Y scales differ. |
| `ScaleNotANumber` | — | A scale is not a number. |
| `ScaleOutOfBounds` | `x`, `z` | The scales are outside the bounds of the ini file. |

</div>

<div class="pagebreak"></div>

<!-- INCLUDE CHANGELOG.md -->

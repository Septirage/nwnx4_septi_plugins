# Changelog

**v1.3.5 (2026/10/17, DLL, NSS and ini)**

- **Anticheat error format reworked.** Every error now has the form `#ErrorName:{key=value,...}`, with a unique name for each situation and named parameters. See the list of the errors in this guide. Scripts that compared the old error names must be updated.
- The anticheat scripts now receive the list of all the errors detected so far, the most recent one being the last. The level up script is no longer called one more time at the end with the same list.
- Added the `XPMsgServer_AC_*` nwscript functions to read the error list (count, name, parameters, lists).
- Creation: `FeatPrereqNotMet` now lists every feat that is not validated. New error `WizardStartingSpellsWrong`.
- Level up: `TooManySpellsTaken` is split in `TooManySpellsTaken` (total) and `TooManySpellsTakenAtLevel` (per spell level). `DomainChanged` and `SchoolChanged` now give the received and the expected values.
- Level up: when several skills were not allowed or had a too high rank, only the last one was reported. They are now all listed.
- Anticheat checks reviewed to follow the game client behavior more closely.
- The demo scripts `sept_anticheat_creation` and `sept_anticheat_levelup` show how to use the new functions.

**v1.3.4 (2026/02/23, DLL)**

- Fixed the way the game manages the classes flagged `AllSpellsKnow`.

**v1.3.3 (2026/01/11, DLL)**

- Fixed the spell level calculation for the Warlock.
- Fixed the minimum skill points: the plugin now handles the skill point rules at creation (4 base, +4 with Skilled) and per level (1, +1 with Skilled).

**v1.3.2 (2025/11/16, DLL)**

- When a wizard with the Spellcasting Prodigy feat is created, one more spell can be chosen.

**v1.3.1 (2025/09/06, DLL)**

- Fixed a possible crash, in rare cases, when using the level up anticheat.

**v1.3.0 (2025/08/28, DLL and NSS)**

- The whole connection system and its API have been rewritten.

**v1.2.2 (2025/08/02, DLL)**

- Anticheat: the Extra Rage feat sets Survival as a class skill.

**v1.2.1 (2024/11/22, DLL)**

- If an automatically granted feat from a previous level is missing, the game grants it during the current level up.

**v1.2.0 (2024/11/21, DLL)**

- The plugin now uses the `SetPacketFilterCallouts` mechanism of xp_bugfix.
- Added the test of the spells to the anticheat system.

**v1.1.1 (2024/10/13, DLL)**

- Better 2DA management, using the cache when present.

**v1.1.0**

- Complete rework.
- Added the Anticheat system.
- More customization possibilities for the connection system.
- Better script management for the connection system.

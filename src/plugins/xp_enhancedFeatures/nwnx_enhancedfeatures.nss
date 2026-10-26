 //////////////////////////////////////////////////////////////////////////////////////////////
// nwnx_enhancedfeatures - various functions to interact with EnhancedFeatures Plugin
// Original Scripter:  Septirage
//--------------------------------------------------------------------------------------------
// Last Modified By:	Septirage			2026-11-12	v1.2.1 - Add GetDestroyedLocation_EFF and GetSneakAttackConditions_EFF
// 					    Septirage			2026-10-14	v1.2.0 - Add RefreshCombatInformation_EFF, GetSavingThrowVsType_EFF and refresh
//																   functions for AC, BA, Criticals, Damage, DamageReduction, SavingThrow files
// 					    Septirage			2026-08-23	v1.1.17 - Add Get/SetCustomValue functions
// 					    Septirage			2026-02-02	v1.1.14 - Add Time and Chat functions
// 					    Septirage			2025-06-27	v1.1.10	- Add the XPEnhancedFeatures_ReloadHitPointFile function
// 					    Septirage			2025-04-23	v1.1.8	- Add the EFFECT_TYPE_* constants
// 					    Septirage			2024-10-25	v1.1.0	- Add the XPEnhancedFeatures_ReloadReduceSpeedFile function 
// 					    Septirage			2024-09-16	v1.0.1	- Set the reload Configfile functions
// 					    Septirage			2023-10-18	v0.9
//--------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////

const int EFFECT_TYPE_CUTSCENEDOMINATED = 111;
const int EFFECT_TYPE_DEATH = 112;
const int EFFECT_TYPE_KNOCKDOWN = 113;
const int EFFECT_TYPE_DAMAGE = 114;
const int EFFECT_TYPE_HEAL = 115;
const int EFFECT_TYPE_LINKEFFECT = 116;
const int EFFECT_TYPE_MODIFYATTACK = 117;
const int EFFECT_TYPE_LOWLIGHTVISION = 118;
const int EFFECT_TYPE_DARKVISION = 119;
const int EFFECT_TYPE_DISAPPEAR = 120;
const int EFFECT_TYPE_APPEAR = 121;
const int EFFECT_TYPE_SETSCALE = 122;
const int EFFECT_TYPE_SEETRUEHPS = 123;
const int EFFECT_TYPE_BABMINIMUM = 124;
const int EFFECT_TYPE_SUMMONCOPY = 125;
const int EFFECT_TYPE_SUMMONCREATURE = 126;


// Will Reload the configuration of your SpeedFeatsFile
void  XPEnhancedFeatures_ReloadSpeedFile();

// Will Reload the configuration of your WeaponFinesseFile
void XPEnhancedFeatures_ReloadWpnFinesseFile();

// Will Reload the configuration of your SkillFeatsFile
void  XPEnhancedFeatures_ReloadSkillFile();

// Will Reload the configuration of your ReduceSpeedFile
void XPEnhancedFeatures_ReloadReduceSpeedFile();

// Will Reload the configuration of your HitPointFile
void XPEnhancedFeatures_ReloadHitPointFile();

// Will Reload the configuration of your BonusAttackFeatsFile
void XPEnhancedFeatures_ReloadBonusAttackFile();

// Will Reload the configuration of your SavingThrowFile
void XPEnhancedFeatures_ReloadSavingThrowFile();

// Will Reload the configuration of your CriticalFile
void XPEnhancedFeatures_ReloadCriticalFile();

// Will Reload the configuration of your ArmorClassFile
void XPEnhancedFeatures_ReloadArmorClassFile();

// Will Reload the configuration of your DamageFile
void XPEnhancedFeatures_ReloadDamageFile();

// Will Reload the configuration of your DamageReductionFile
void XPEnhancedFeatures_ReloadDamageReductionFile();

// Will Reload the configuration of your CustomValueRuleFile
// The values stored on the creatures are kept. If an index changes of mode (Raw, Computed, Evolution),
// its stored value is used again as soon as it is no longer Computed.
void XPEnhancedFeatures_ReloadCustomValueFile();



/***************************** CustomValues Functions *****************************/

// Get the specified CustomValue of a given creature
// oCreature	- The creature 
// nIdx			- The index of the customValue
int GetCustomValue_EFF(object oCreature, int nIdx);


// Set the specified CustomValue of a given creature
// oCreature	- The creature
// nIdx			- The index of the customValue
// iValue		- The value to set
void SetCustomValue_EFF(object oCreature, int nIdx, int iValue);


/************************** Combat Information Functions **************************/

// Ask the server to recompute the combat information (attack bonus... on the character sheet)
// of oCreature. Use it when something used by your rules changed (CustomValue, area...).
// Done at the next update of the player, and only sent if something really changed.
// oCreature	- The creature
void RefreshCombatInformation_EFF(object oCreature);

// Return the saving throw of oCreature against nSaveType.
// The effect and feat bonuses are capped at +20, as in the save roll.
// oCreature	- The creature
// nSavingThrow	- SAVING_THROW_FORT, SAVING_THROW_REFLEX or SAVING_THROW_WILL
// nSaveType	- SAVING_THROW_TYPE_*
// oVersus		- The object the save is against, as for a save roll: a creature (effects against its race or
//				  alignment), a spell area of effect (bonuses against the spells, spell schools, Spellcraft...),
//				  or OBJECT_INVALID
int GetSavingThrowVsType_EFF(object oCreature, int nSavingThrow, int nSaveType = SAVING_THROW_TYPE_NONE, object oVersus = OBJECT_INVALID);

// Return the sneak attack (or death attack) conditions of the game between oAttacker and oTarget, tested by
// the game itself (as for an attack of oAttacker on oTarget now).
// Needs AddGetSneakAttackConditionsFct = 1 in xp_enhancedFeatures.ini.
// Result mask (0 = not possible, or invalid creatures):
//	1 - possible (attacker not seen, target flat-footed or flanked; within 10 m if bRanged)
//	2 - refused only by an immunity of oTarget (sneak attack or critical hit immunity, plot), not bypassed
//	4 - oTarget is immune but the game bypassed it (Epic Precision, FeatIgnoreCritImmunity of the race,
//		Spirits Ruin). Always with 1. For a sneak attack, the game then divides the sneak attack dice by two;
//		the death attack dice are never divided.
//	1 and 4 can be set together, 2 is never set with 1 or 4.
// The mask does not say which condition succeeded (not seen / flat-footed / flanked).
// oAttacker	- The attacking creature
// oTarget		- The target creature
// bRanged		- TRUE to test as a ranged attack (the game then requires a distance of 10 m or less)
// nFeatTest	- 0: sneak attack, WITHOUT the sneak attack feat test (oAttacker needs no sneak attack feat)
//				  1: sneak attack, oAttacker must have a sneak attack feat (as in the game)
//				  2: death attack, oAttacker must have a death attack feat (as in the game, always tested)
int GetSneakAttackConditions_EFF(object oAttacker, object oTarget, int bRanged = FALSE, int nFeatTest = 0);


/******************************* OnDestroy Functions *******************************/

// Only in the GlobalOnDestroyScript: return the location of OBJECT_SELF just before its destruction
// (when the script runs, the object is already out of its area: GetLocation/GetArea are invalid).
// The area of the location is OBJECT_INVALID for an object destroyed without the destroy event:
// DespawnRosterMember, timed placed effect, DM kill of a trigger or encounter.
location GetDestroyedLocation_EFF();


/********************************* Chat Functions *********************************/

// Return the distance for a message on the specified volume.
// nTalkVolume 	- TALKVOLUME_TALK or TALKVOLUME_WHISPER constant
float GetChatDistance_EFF(int nTalkVolume);

// Set the distance for a message on the specified volume.
// nTalkVolume 	- TALKVOLUME_TALK or TALKVOLUME_WHISPER constant
// fRange 		- The range to set for the specified volume
void SetChatDistance_EFF(int nTalkVolume, float fRange);

// Send a chat message but only for a specified PC without any distance check
// This function is close to SendChatMessage execpt that:
//		- only the oReceiver will receive message
//		- no distance check will be done (but for whisper and talk the sender must be in same area)
//		- It will trigger no callback (onchat) nor any OnListen script
//		
// oSender	 	- The Object who will speak.
// oReceiver 	- The PC who will be receiving the message.
// nChannel 	- CHAT_MODE const indicating the type of message to be sent. 
// sMessage		- actual message text
void SendChatMessageToPC_EFF(object oSender, object oReceiver, int nChannel, string sMessage);


/******************************** Time Functions *********************************/

// Used for visual purpose. It will change the hour only for specified PC
// oPC			- The PC who will be receiving the new hour
// iHour		- The Hour to be setted for the specified PC
void SetTimeForPC_EFF(object oPC, int iHour);


// Used for visual purpose. Will set an hour for oArea.
//		PCs in this area will now receive iHour instead of normal hour on loading 
//				and on every automatic refresh (every game hours)
// oArea		- The Area on which for a specific hour.
// iHour		- The Hour to set and kept for oArea (set to -1 to retrieve normal behavior)
void SetHourForArea_EFF(object oArea, int iHour);

// Will return the iHour forced on the specified Area. -1 if nothing.
// oArea		- The Area from which you want to get the specified hour.
int GetHourForArea_EFF(object oArea);

void  XPEnhancedFeatures_ReloadSpeedFile()
{
	NWNXSetInt( "EnhancedFeatures", "ReloadSpeedFile", "", -1, 0);	
}

void XPEnhancedFeatures_ReloadWpnFinesseFile()
{
	NWNXSetInt( "EnhancedFeatures", "ReloadWpnFinesseFile", "", -1, 0);
}

void  XPEnhancedFeatures_ReloadSkillFile()
{
	NWNXSetInt( "EnhancedFeatures", "ReloadSkillFile", "", -1, 0);	
}

void  XPEnhancedFeatures_ReloadReduceSpeedFile()
{
	NWNXSetInt( "EnhancedFeatures", "ReloadReduceSpeedFile", "", -1, 0);	
}

void  XPEnhancedFeatures_ReloadHitPointFile()
{
	NWNXSetInt( "EnhancedFeatures", "ReloadHitPointFile", "", -1, 0);
}

void  XPEnhancedFeatures_ReloadBonusAttackFile()
{
	NWNXSetInt( "EnhancedFeatures", "ReloadBonusAttackFile", "", -1, 0);
}

void  XPEnhancedFeatures_ReloadSavingThrowFile()
{
	NWNXSetInt( "EnhancedFeatures", "ReloadSavingThrowFile", "", -1, 0);
}

void  XPEnhancedFeatures_ReloadCriticalFile()
{
	NWNXSetInt( "EnhancedFeatures", "ReloadCriticalFile", "", -1, 0);
}

void  XPEnhancedFeatures_ReloadArmorClassFile()
{
	NWNXSetInt( "EnhancedFeatures", "ReloadArmorClassFile", "", -1, 0);
}

void  XPEnhancedFeatures_ReloadDamageFile()
{
	NWNXSetInt( "EnhancedFeatures", "ReloadDamageFile", "", -1, 0);
}

void  XPEnhancedFeatures_ReloadDamageReductionFile()
{
	NWNXSetInt( "EnhancedFeatures", "ReloadDamageReductionFile", "", -1, 0);
}

void  XPEnhancedFeatures_ReloadCustomValueFile()
{
	NWNXSetInt( "EnhancedFeatures", "ReloadCustomValueFile", "", -1, 0);
}


void SetTimeForPC_EFF(object oPC, int iHour)
{
	NWNXSetInt( "EnhancedFeatures", "TimeFunction", "SetHourForPlayer", ObjectToInt(oPC), iHour);	
}


void SetHourForArea_EFF(object oArea, int iHour)
{
	NWNXSetInt( "EnhancedFeatures", "TimeFunction", "SetHourForArea", ObjectToInt(oArea), iHour);	
}

int GetHourForArea_EFF(object oArea)
{
	return NWNXGetInt("EnhancedFeatures", "TimeFunction", "GetHourForArea", ObjectToInt(oArea));
}

void SendChatMessageToPC_EFF(object oSender, object oReceiver, int nChannel, string sMessage)
{
	string sPacked = ObjectToString(oSender) + "|" + ObjectToString(oReceiver) + "|" + sMessage;
	NWNXSetString("EnhancedFeatures", "TalkFunction", "SendSpecific", nChannel, sPacked);
}

void SetChatDistance_EFF(int nTalkVolume, float fRange)
{
	NWNXSetFloat("EnhancedFeatures", "TalkFunction", "ChatDistance", nTalkVolume, fRange);
}

float GetChatDistance_EFF(int nTalkVolume)
{
	return NWNXGetFloat("EnhancedFeatures", "TalkFunction", "ChatDistance", nTalkVolume);
}

int GetCustomValue_EFF(object oCreature, int nIdx)
{
	return NWNXGetInt("EnhancedFeatures", "CustomValue", IntToString(nIdx), ObjectToInt(oCreature));
}

void SetCustomValue_EFF(object oCreature, int nIdx, int iValue)
{
	NWNXSetInt("EnhancedFeatures", "CustomValue", IntToString(nIdx), ObjectToInt(oCreature), iValue);
}

void RefreshCombatInformation_EFF(object oCreature)
{
	NWNXSetInt("EnhancedFeatures", "RefreshCombatInformation", "", ObjectToInt(oCreature), 0);
}

int GetSavingThrowVsType_EFF(object oCreature, int nSavingThrow, int nSaveType, object oVersus)
{
	return NWNXGetInt("EnhancedFeatures", "SavingThrowVsType", IntToString(nSavingThrow) + "|" + IntToString(nSaveType) + "|" + IntToString(ObjectToInt(oVersus)), ObjectToInt(oCreature));
}

int GetSneakAttackConditions_EFF(object oAttacker, object oTarget, int bRanged, int nFeatTest)
{
	return NWNXGetInt("EnhancedFeatures", "SneakAttackConditions", IntToString(ObjectToInt(oTarget)) + "|" + IntToString(bRanged) + "|" + IntToString(nFeatTest), ObjectToInt(oAttacker));
}

location GetDestroyedLocation_EFF()
{
	object oArea = IntToObject(NWNXGetInt("EnhancedFeatures", "DestroyedLocation", "", 0));
	vector vPosition = Vector(NWNXGetFloat("EnhancedFeatures", "DestroyedLocation", "X", 0),
							  NWNXGetFloat("EnhancedFeatures", "DestroyedLocation", "Y", 0),
							  NWNXGetFloat("EnhancedFeatures", "DestroyedLocation", "Z", 0));
	vector vOrientation = Vector(NWNXGetFloat("EnhancedFeatures", "DestroyedLocation", "OrientX", 0),
								 NWNXGetFloat("EnhancedFeatures", "DestroyedLocation", "OrientY", 0), 0.0);
	return Location(oArea, vPosition, VectorToAngle(vOrientation));
}

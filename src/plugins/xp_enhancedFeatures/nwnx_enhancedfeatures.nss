 //////////////////////////////////////////////////////////////////////////////////////////////
// nwnx_enhancedfeatures - various functions to interact with EnhancedFeatures Plugin
// Original Scripter:  Septirage
//--------------------------------------------------------------------------------------------
// Last Modified By:	Septirage			2026-08-23	v1.1.17 - Add Get/SetCustomValue functions
// 					    Septirage			2025-02-02	v1.1.14 - Add Time and Chat functions
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

// Will Reload the configuration of your SkillFeatsFile
void XPEnhancedFeatures_ReloadWpnFinesseFile();

// Will Reload the configuration of your WeaponFinesseFile
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

 //////////////////////////////////////////////////////////////////////////////////////////////
// nwnx_enhancedfeatures - various functions to interact with EnhancedFeatures Plugin
// Original Scripter:  Septirage
//--------------------------------------------------------------------------------------------
// Last Modified By:    Septirage			2025-04-23	v1.1.8	- Add the EFFECT_TYPE_* constants
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

// Will Reload the configuration of your ReloadReduceSpeedFile
void XPEnhancedFeatures_ReloadReduceSpeedFile();


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

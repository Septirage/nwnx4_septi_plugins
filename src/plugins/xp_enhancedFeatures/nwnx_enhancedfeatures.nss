 //////////////////////////////////////////////////////////////////////////////////////////////
// nwnx_enhancedfeatures - various functions to interact with EnhancedFeatures Plugin
// Original Scripter:  Septirage
//--------------------------------------------------------------------------------------------
// Last Modified By:	Septirage			2024-10-25	v1.1.0	- Add the XPEnhancedFeatures_ReloadReduceSpeedFile function 
// 					    Septirage			2024-09-16	v1.0.1	- Set the reload Configfile functions
// 					    Septirage			2023-10-18	v0.9
//--------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////


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

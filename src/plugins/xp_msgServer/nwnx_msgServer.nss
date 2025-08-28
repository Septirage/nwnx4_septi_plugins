////////////////////////////////////////////////////////////////////////////////////////////////
// nwnx_MsgServer - various functions to use the Message server plugin and the connection part
// Original Scripter:  Septirage
//----------------------------------------------------------------------------------------------
// Last Modified By:   Septirage           2025-08-20		Update for v1.3+
// 					   Septirage           2024-10-08		Update for v1.1+
// 					   Septirage           2023-01-31
//----------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////

// List of return Constant to use connections scripts

/// Do nothing and wait for further actions before authenticating the player. Return value used for OnConnection script and related GUI scripts
const int XPMSGSRV_HEIMDALL_RET_WAIT  = 1;
/// Allow player to enter the server. Return value used for OnConnection script and related GUI scripts
const int XPMSGSRV_HEIMDALL_RET_ALLOW   = 2;
/// Kick the player out of the server. Return value used for OnConnection script and related GUI scripts
const int XPMSGSRV_HEIMDALL_RET_KICK = 3;

/**************************** General Management ****************************/

//Set the Anticheat LvlUp System Active or Unactive
// bActivated : TRUE / FALSE
//We recommand to keep it active, unless for debugging  connection issues.
void XPMsgServer_SetAntiCheatLvlUpSystem(int bActivated);

//Set the Anticheat Creation System Active or Unactive
// bActivated : TRUE / FALSE
//We recommand to keep it active, unless for debugging  connection issues.
void XPMsgServer_SetAntiCheatCreationSystem(int bActivated);

//Set the whole connection System Active or Unactive
// bActivated : TRUE / FALSE
//We recommand to keep it active, unless for debugging  connection issues.
void XPMsgServer_SetConnectionSystem(int bActivated);


/// Allows a specific script to be executed by a non authenticated player.
//// This script will have the first 5 arguments provided by MsgSrv, followed by the
/// arguments provided by the UIObject_Misc_ExecuteServerScript GUI callback.
///
/// The arguments provided by MsgSrv are:
/// - int uniquePlayerID   UniquePlayerID that identify this connection. To be used in "ConnectionsFunctions" bellow
/// - string sAccountName  Player account name (gamespy account, see GetPCPlayerName)
/// - string sCurrentIP    Player IP (see GetPCIPAddress)
/// - string sCDKey        Public part of the player CDKey (see GetPCPublicCDKey)
/// - int nPrivileges      Player/DM/Admin privileges (see XPMsgServer_GetHasXXXPrivileges)
void XPMsgServer_SetAuthorizedGUIScript(string sScript);

/*****************************************************************************
*************************** Connections Functions ****************************
**************** To be used in the Connections related scripts ***************
*****************************************************************************/

/// Returns TRUE if iPrivileges contains Player privileges
int XPMsgServer_GetHasPlayerPrivilege(int iPrivileges);

/// Returns TRUE if iPrivileges contains DM privileges
int XPMsgServer_GetHasDMPrivilege(int iPrivileges);

/// Returns TRUE if iPrivileges contains Admin privileges
int XPMsgServer_GetHasAdminPrivilege(int iPrivileges);

/// Opens the given GUI for the player identified by UniquePlayerID.
/// See DisplayGuiScreen
///
/// Works before the player has loaded a character into the module.
void XPMsgServer_DisplayGuiScreen(int uniquePlayerID, string sScreenName, string sFileName);

/// Close the given GUI for the player identified by UniquePlayerID. 
/// See CloseGUIScreen
///
/// Works before the player has loaded a character into the module.
void XPMsgServer_CloseGUIScreen(int uniquePlayerID, string sScreenName);

/// Show or hide a GUI object for  the player identified by UniquePlayerID.
// See SetGUIObjectHidden
//
// Works before the player has loaded a character into the module.
void XPMsgServer_SetGUIObjectHidden(int uniquePlayerID, string sScreenName, string sUIObjectName, int bHidden);

/// Enable or Disable a GUI object for the player identified by UniquePlayerID.
/// See SetGUIObjectDisable
///
/// Works before the player has loaded a character into the module.
void XPMsgServer_SetGUIObjectDisable(int uniquePlayerID, string sScreenName, string sUIObjectName, int bDisabled);

/// Set the text of a GUI UIText for the player identified by UniquePlayerID.
/// See SetGUIObjectText
///
/// Works before the player has loaded a character into the module.
void XPMsgServer_SetGUIObjectText(int uniquePlayerID, string sScreenName, string sUIObjectName, string sText);

/// Displays a message box popup for the player identified by UniquePlayerID.
/// Reduced version of DisplayMessageBox
///
/// Works before the player has loaded a character into the module.
void XPMsgServer_DisplayMessageInfo(int uniquePlayerID, string sScreenName, string sText, string sOkString);


/*****************************************************************************
**************************** Validation Function *****************************
*****************************************************************************/

/// Call this to Allow the uniquePlayerID to enter the module
///
/// Major utility is to use after AssignCommand/DelayCommand function call
void XPMsgServer_SetAllowPlayer(int uniquePlayerID);



/*****************************************************************************/

///////////////////////////////////////////////////////////////////////////////
////////////////////////////// IMPLEMENTATIONS ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/*****************************************************************************/

void XPMsgServer_SetConnectionSystem(int bActivated)
{
	NWNXSetInt("MsgServer", "ConnectionSystem", "", 0, bActivated);
}

void XPMsgServer_SetAntiCheatLvlUpSystem(int bActivated)
{
	NWNXSetInt("MsgServer", "AnticheatLvlUp", "", 0, bActivated);
}

void XPMsgServer_SetAntiCheatCreationSystem(int bActivated)
{
	NWNXSetInt("MsgServer", "AnticheatCreation", "", 0, bActivated);
}


void XPMsgServer_SetAuthorizedGUIScript(string sScript)
{
	NWNXSetString("MsgServer", "SetGUIScript", "", 0, sScript);
}


void XPMsgServer_DisplayGuiScreen(int uniquePlayerID, string sScreenName, string sFileName)
{
	NWNXSetString("MsgServer", "DisplayGUIScreen", sScreenName, uniquePlayerID, sFileName);
}

void XPMsgServer_CloseGUIScreen(int uniquePlayerID, string sScreenName)
{
	NWNXSetString("MsgServer", "CloseGUIScreen", sScreenName, uniquePlayerID, "");
}

void XPMsgServer_SetGUIObjectHidden(int uniquePlayerID, string sScreenName, string sUIObjectName, int bHidden)
{
	NWNXSetInt("MsgServer", "SetGUIObjectHidden", sScreenName + ">" + sUIObjectName, uniquePlayerID, bHidden);
}

void XPMsgServer_SetGUIObjectDisable(int uniquePlayerID, string sScreenName, string sUIObjectName, int bDisabled)
{
	NWNXSetInt("MsgServer", "SetGUIObjectDisable", sScreenName + ">" + sUIObjectName, uniquePlayerID, bDisabled);
}

void XPMsgServer_SetGUIObjectText(int uniquePlayerID, string sScreenName, string sUIObjectName, string sText)
{
	NWNXSetString("MsgServer", "SetGUIObjectText", sScreenName + ">" + sUIObjectName, uniquePlayerID, sText);
}

void XPMsgServer_DisplayMessageInfo(int uniquePlayerID, string sScreenName, string sText, string sOkString)
{
	NWNXSetString("MsgServer", "DisplayMessageInfo", sScreenName + ">" + sOkString, uniquePlayerID, sText);
}


int XPMsgServer_GetHasPlayerPrivilege(int iPrivileges){
	return (iPrivileges & 1) != 0;
}

int XPMsgServer_GetHasDMPrivilege(int iPrivileges){
	return (iPrivileges & 2) != 0;
}

int XPMsgServer_GetHasAdminPrivilege(int iPrivileges){
	return (iPrivileges & 4) != 0;
}

void XPMsgServer_SetAllowPlayer(int uniquePlayerID)
{
	NWNXSetInt("MsgServer", "SetPlayerAllowed", "", uniquePlayerID, 1);
}
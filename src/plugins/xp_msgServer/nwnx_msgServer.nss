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


/*****************************************************************************
************************* Anticheat error list helpers ***********************
*****************************************************************************/

/// The sErrorList given to the anticheat scripts (creation and levelup) is the list of all the
/// errors detected so far. The most recent error is the last one. Each error has the form:
///     #ErrorName:{key1=value1,key2=value2}
/// The errors are simply concatenated:   #ErrorA:{x=1}#ErrorB:{}
/// A value is a number or a list of items separated by spaces ("12 40 77"). An item can be a tuple
/// whose elements are separated by ':' ("5:3 7:2" is the list of the tuples 5:3 and 7:2).
/// See the Guide for the list of all the errors and their parameters.
///
/// The functions below only read that string, they can be used anywhere (not only in the anticheat scripts).

/// Returns the number of errors in sErrorList
int XPMsgServer_AC_GetErrorCount(string sErrorList);

/// Returns the nIndex-th error (0 = first) as text, without the leading '#', for example "TooManySkillPoints:{allowed=12,used=14}".
/// Returns "" if there is no such error. Useful to log an error as it is.
string XPMsgServer_AC_GetErrorString(string sErrorList, int nIndex);

/// Returns the name of the nIndex-th error (0 = first), for example "TooManySkillPoints". "" if there is no such error.
string XPMsgServer_AC_GetErrorCode(string sErrorList, int nIndex);

/// Returns TRUE if at least one error of this name is in sErrorList
int XPMsgServer_AC_HasError(string sErrorList, string sCode);

/// Returns the value of the parameter sKey of the nIndex-th error, as text. "" if there is no such error or parameter.
string XPMsgServer_AC_GetErrorParam(string sErrorList, int nIndex, string sKey);

/// Same as XPMsgServer_AC_GetErrorParam, converted to an integer (0 if there is no such error or parameter).
int XPMsgServer_AC_GetErrorParamInt(string sErrorList, int nIndex, string sKey);

/// Returns the number of items of a list value (see XPMsgServer_AC_GetErrorParam).
int XPMsgServer_AC_GetListSize(string sValue, string sSeparator = " ");

/// Returns the nIndex-th item (0 = first) of a list value. "" if there is none.
/// For a list of tuples, read a tuple first and then its elements:
///   XPMsgServer_AC_GetListItem(XPMsgServer_AC_GetListItem(sValue, nTuple), nElement, ":")
string XPMsgServer_AC_GetListItem(string sValue, int nIndex, string sSeparator = " ");



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


/*********************** Anticheat error list helpers ************************/

int XPMsgServer_AC_GetErrorCount(string sErrorList)
{
	int nCount = 0;
	int nPos = FindSubString(sErrorList, "#");
	while (nPos != -1)
	{
		nCount++;
		nPos = FindSubString(sErrorList, "#", nPos + 1);
	}
	return nCount;
}

string XPMsgServer_AC_GetErrorString(string sErrorList, int nIndex)
{
	int nPos = FindSubString(sErrorList, "#");
	int i;
	for (i = 0; i < nIndex && nPos != -1; i++)
	{
		nPos = FindSubString(sErrorList, "#", nPos + 1);
	}
	if (nPos == -1 || nIndex < 0)
		return "";

	int nNext = FindSubString(sErrorList, "#", nPos + 1);
	if (nNext == -1)
		nNext = GetStringLength(sErrorList);
	return GetSubString(sErrorList, nPos + 1, nNext - nPos - 1);
}

string XPMsgServer_AC_GetErrorCode(string sErrorList, int nIndex)
{
	string sError = XPMsgServer_AC_GetErrorString(sErrorList, nIndex);
	int nColon = FindSubString(sError, ":");
	if (nColon == -1)
		return sError;
	return GetSubString(sError, 0, nColon);
}

int XPMsgServer_AC_HasError(string sErrorList, string sCode)
{
	int nCount = XPMsgServer_AC_GetErrorCount(sErrorList);
	int i;
	for (i = 0; i < nCount; i++)
	{
		if (XPMsgServer_AC_GetErrorCode(sErrorList, i) == sCode)
			return TRUE;
	}
	return FALSE;
}

string XPMsgServer_AC_GetErrorParam(string sErrorList, int nIndex, string sKey)
{
	string sError = XPMsgServer_AC_GetErrorString(sErrorList, nIndex);
	int nOpen = FindSubString(sError, "{");
	if (nOpen == -1)
		return "";

	//Content between the braces, surrounded by commas to find the key whatever its position
	string sInner = GetSubString(sError, nOpen + 1, GetStringLength(sError) - nOpen - 2);
	sInner = "," + sInner + ",";

	string sToken = "," + sKey + "=";
	int nKey = FindSubString(sInner, sToken);
	if (nKey == -1)
		return "";

	int nStart = nKey + GetStringLength(sToken);
	int nEnd = FindSubString(sInner, ",", nStart);
	return GetSubString(sInner, nStart, nEnd - nStart);
}

int XPMsgServer_AC_GetErrorParamInt(string sErrorList, int nIndex, string sKey)
{
	return StringToInt(XPMsgServer_AC_GetErrorParam(sErrorList, nIndex, sKey));
}

int XPMsgServer_AC_GetListSize(string sValue, string sSeparator = " ")
{
	if (sValue == "")
		return 0;

	int nSepLen = GetStringLength(sSeparator);
	int nCount = 1;
	int nPos = FindSubString(sValue, sSeparator);
	while (nPos != -1)
	{
		nCount++;
		nPos = FindSubString(sValue, sSeparator, nPos + nSepLen);
	}
	return nCount;
}

string XPMsgServer_AC_GetListItem(string sValue, int nIndex, string sSeparator = " ")
{
	if (nIndex < 0)
		return "";

	int nSepLen = GetStringLength(sSeparator);
	int nStart = 0;
	int nPos;
	int i;
	for (i = 0; i < nIndex; i++)
	{
		nPos = FindSubString(sValue, sSeparator, nStart);
		if (nPos == -1)
			return "";
		nStart = nPos + nSepLen;
	}

	int nEnd = FindSubString(sValue, sSeparator, nStart);
	if (nEnd == -1)
		nEnd = GetStringLength(sValue);
	return GetSubString(sValue, nStart, nEnd - nStart);
}

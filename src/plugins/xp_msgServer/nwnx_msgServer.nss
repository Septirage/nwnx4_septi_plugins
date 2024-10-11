////////////////////////////////////////////////////////////////////////////////////////////////
// nwnx_MsgServer - various functions to use the Message server plugin and the connection part
// Original Scripter:  Septirage
//----------------------------------------------------------------------------------------------
// Last Modified By:   Septirage           2024-10-08		Update for v1.1+
// 					   Septirage           2023-01-31
//----------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////

// List of return Constant to use in hookScripts
// Note : Ban will "only" block all communication from the 
//			current PlayerName until next serverReboot or call of XPMsgServer_Unblock
const int HEIMDALL_RET_BAN		=  -3;
const int HEIMDALL_RET_KICK		=  -2;
const int HEIMDALL_RET_NOK		=	0;
const int HEIMDALL_RET_OK		=	1;

/**************************** General Management ****************************/

//Set the Anticheat LvlUp System Active or Unactive
// bActivated : TRUE / FALSE
//We recommand to keep it active, unless for debugging  connection issues.
void XPMsgServer_SetAntiCheatLvlUpSystem(int bActivated);

//Set the Anticheat Creation System Active or Unactive
// bActivated : TRUE / FALSE
//We recommand to keep it active, unless for debugging  connection issues.
void XPMsgServer_SetAntiCheatCreationSystem(int bActivated);

//Set the EnforcedSecurity Active or Unactive 
// bActivated : TRUE / FALSE
//We recommand to keep it active, unless for debugging  connection issues.
void XPMsgServer_SetEnforcedSecurity(int bActivated);

//Get the state of the EnforcedSecurity
// TRUE : the enforced security is active. 
// FALSE otherwise
int XPMsgServer_GetEnforcedSecurity();

//Set the whole connection System Active or Unactive
// bActivated : TRUE / FALSE
//We recommand to keep it active, unless for debugging  connection issues.
void XPMsgServer_SetConnectionSystem(int bActivated);


//Unblock the given player
//sPlayerName = "bioware account";
void XPMsgServer_Unblock(string sPlayerName);

//If you use a WelcomScreen, use it to set sPlayerName as "knowed" and so, don't print the screen for they
void XPMsgServer_Knowed(string sPlayerName);

//Allow you to convert the iIP from AutoAuthenticationScript and ConnectionValidScript to "x.x.x.x" format
string XPMsgServer_IpIntToString(int iIP);

/*****************************************************************************
******************* To be used in the ConnectionValidScript ******************
********* Note that these functions can only be used in this script **********
*****************************************************************************/

//Use it to set the resonse text if invalid (for example : "Invalid password")
void XPMsgServer_ScriptResponseMsg(string sMsg);



/*****************************************************************************/

///////////////////////////////////////////////////////////////////////////////
////////////////////////////// IMPLEMENTATIONS ////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

/*****************************************************************************/


string XPMsgServer_IpIntToString(int iIP)
{
	int iA = iIP & 0xFF;
	int iB = (iIP & 0xFF00) >> 8;
	int iC = (iIP & 0xFF0000) >> 16;
	int iD = (iIP & 0xFF000000) >> 24;
	string sIP = IntToString(iA) + ".";
	sIP += IntToString(iB) + "." + IntToString(iC) + "." + IntToString(iD);

	return sIP;
}

void XPMsgServer_SetEnforcedSecurity(int bActivated)
{
	NWNXSetInt("MsgServer", "EnforcedSecurity", "", 0, bActivated);
}

int XPMsgServer_GetEnforcedSecurity()
{
	return NWNXGetInt("MsgServer", "EnforcedSecurity", "", 0);
}

void XPMsgServer_ScriptResponseMsg(string sMsg)
{
	NWNXSetString("MsgServer", "Response", "", 0, sMsg);
}

void XPMsgServer_Unblock(string sPlayerName)
{
	NWNXSetInt("MsgServer", "Unblock", sPlayerName, 0, 0);
}

void XPMsgServer_Knowed(string sPlayerName)
{
	NWNXSetInt("MsgServer", "Knowed", sPlayerName, 0, 1);
}

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


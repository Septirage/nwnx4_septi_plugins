///////////////////////////////////////////////////////////////////////////////////////////
// gui_sept_heimdall_demo - Script used for demo purpose only. Write your own to manage gui callback
// Original Scripter:  Septirage
//////////////////////////////////////////////////////////////////////////////////////////

#include "nwnx_msgServer"


/*
* This function will be called for any new incoming player.
* 
* iUniqueID		-	The UniquePlayerID for this connection. Will be needed for specifics MsgServer functions
* sAccountName     -   The incoming Player account name (gamespy account, see GetPCPlayerName)
* sIp              -   The Ip of incoming player (see GetPCIPAddress)
* sCDKey           -   The public CDKey of the incoming player (see GetPCPublicCDKey)
* iPrivileges      -   Current Player/DM/Admin privileges of the incoming player (see XPMsgServer_GetIsXXX)
* sPlayerLogin     -   The login entered on the Connection GUI
* sPassword        -   The Password entered on the Connection GUI
* sOption          -   The option parameter from Connection GUI (on default GUI, it's "1" if player ask for "remember me", "0" otherwise)
*
* Return value :  XPMSGSRV_HEIMDALL_RET_* constant, depending of the result wanted.
*/

int StartingConditional(int iUniquePlayerID, string sAccountName, string sIP, string sCDKey, int iPrivileges, string sPlayerLogin, string sPassword, string sOption)
{
	// /!\ This is a really dummy implementation, just to show how to work with that script. Don't use it as is.

	//Just for demo purpose: if password = no, refuse connection and prompt "Bad Password"
	if(sPassword == "no")
	{
		XPMsgServer_SetGUIObjectText(iUniquePlayerID, "SEPT_HEIMDALL_LOGIN", "ErrorTX", "Bad Password");
		return XPMSGSRV_HEIMDALL_RET_WAIT;
	}

	//Just for demo purpose : if password = "kick",
	//  kick the player out, with the  message : "Go out bad guy !"
	else if(sPassword == "kick")
	{

		//Show a panel to kick out the player. 
		XPMsgServer_DisplayGuiScreen(iUniquePlayerID, "SEPT_HEIMDALL_KICK", "death_default.xml");
		XPMsgServer_SetGUIObjectHidden(iUniquePlayerID, "SEPT_HEIMDALL_KICK", "BUTTON_RESPAWN", TRUE);
		XPMsgServer_SetGUIObjectText(iUniquePlayerID, "SEPT_HEIMDALL_KICK", "MESSAGE_TEXT", "Go out bad guy !");

		//Close the login screen
		XPMsgServer_CloseGUIScreen(iUniquePlayerID, "SEPT_HEIMDALL_LOGIN");
		
		return XPMSGSRV_HEIMDALL_RET_KICK;
	}

	//Just for demo purpose: in any other case, accept the connection (I told you : dummy solution)
	
	// Player check "Remember me" option so remember their
	if(sOption == "1")
		SetLocalString(GetModule(), "IP_"+sAccountName, sIP);
	else // remove the possible old "remember me" 
		DeleteLocalString(GetModule(), "IP_"+sAccountName);
	
	//Close the login screen
	XPMsgServer_CloseGUIScreen(iUniquePlayerID, "SEPT_HEIMDALL_LOGIN");
	
	return XPMSGSRV_HEIMDALL_RET_ALLOW;
}
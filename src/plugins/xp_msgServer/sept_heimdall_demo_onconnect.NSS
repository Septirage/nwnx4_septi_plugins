///////////////////////////////////////////////////////////////////////////////////////////
// sept_heimdall_demo_autoconnect - Script used for demo purpose only. Write your own to check 
//										if a person should be automatically reconnected.
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
 *
 * Return value :  XPMSGSRV_HEIMDALL_RET_* constant, depending of the result wanted.
 */
int StartingConditional(int iUniquePlayerID, string sAccountName, string sIP, string sCDKey, int iPrivileges)
{
	object oModule = GetModule();

	// /!\ This is a really dummy implementation, just to show how to work with that script. Don't use it as it.
	if (GetLocalInt(oModule, "sept_heimdall_init") == FALSE) {
		XPMsgServer_SetAuthorizedGUIScript("gui_sept_heimdall_demo");
		SetLocalInt(oModule, "sept_heimdall_init", TRUE);
	}
	
	//Just between us... I HATE this guy, don't even allow him to continue further
	if( sAccountName == "I_Love_To_Kill_Gnomes") {
		//Show a panel to kick out the player. 
		XPMsgServer_DisplayGuiScreen(iUniquePlayerID, "SEPT_HEIMDALL_KICK", "death_default.xml");
		XPMsgServer_SetGUIObjectHidden(iUniquePlayerID, "SEPT_HEIMDALL_KICK", "BUTTON_RESPAWN", TRUE);
		XPMsgServer_SetGUIObjectText(iUniquePlayerID, "SEPT_HEIMDALL_KICK", "MESSAGE_TEXT", "Let the Gnome Alone !");
		return XPMSGSRV_HEIMDALL_RET_KICK;
	}

	//I don't want to allow "auto-connection" for a "more than player" connection.
	if(XPMsgServer_GetHasDMPrivilege(iPrivileges) || XPMsgServer_GetHasAdminPrivilege(iPrivileges))
	{
		// This player has asked to remain connected and has not changed ip. We connect them without bothering them.
		if(GetLocalString(GetModule(), "IP_"+sAccountName) == sIP )
			return XPMSGSRV_HEIMDALL_RET_ALLOW;
	}
	
	/***********************************************************/
	
	//For other case, show the connection panel
	XPMsgServer_DisplayGuiScreen(iUniquePlayerID, "SEPT_HEIMDALL_LOGIN", "sept_heimdall_demo_login.xml");

	return XPMSGSRV_HEIMDALL_RET_WAIT;
}
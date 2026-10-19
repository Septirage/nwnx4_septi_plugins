///////////////////////////////////////////////////////////////////////////////////////////
// sept_anticheat_creation - Script used for demo purpose only. Write your own to manage
//										the error case.
// Original Scripter:  Septirage
//////////////////////////////////////////////////////////////////////////////////////////

#include "nwnx_msgServer"

/*
 * This function is called on error detected when a player try to create a new character.
 * (the plugin already writes the errors in its own log file)
 *
 * sPlayerName      -   The "Bioware" Account of the Player
 * sErrorList       -   The list of ALL the errors detected so far for this creation. The most recent one is the
 *                      last one. Look at the XPMsgServer_AC_* functions (nwnx_msgServer.nss) to read it.
 *
 * Depending on the StopCreationFirstViolation option, this script is called:
 *  - 1 (default): each time a new error is detected. If you tolerate it (return TRUE), the plugin goes on and
 *                 calls you again with the errors detected so far plus the next one.
 *  - 0          : one time, at the end, with all the errors.
 * So, judge the whole list every time: return TRUE only if you tolerate every error of the list.
 *
 * Return value :  FALSE if this is a real error and you wish to prevent its creation. TRUE otherwise
 */

//Put here the errors that must be tolerated on your module (false positive for your custom content, ...)
int IsTolerated(string sErrorCode)
{
    //if (sErrorCode == "ScaleOutOfBounds")
    //    return TRUE;
    return FALSE;
}

int StartingConditional(string sPlayerName, string sErrorList)
{
    //here I can log sErrorList with sPlayerName.
    //I can analyse it (if i have a really specific module that cause false positive error)
    //I can also choose to do something to this nasty player !

    //For now.. It's only demo, i will tolerate nothing and just keep it as error
    //(yeah, for that, the best would have been not to configure the script)

    int nCount = XPMsgServer_AC_GetErrorCount(sErrorList);
    int i;
    for (i = 0; i < nCount; i++)
    {
        if (!IsTolerated(XPMsgServer_AC_GetErrorCode(sErrorList, i)))
            return FALSE;
    }

    //Every error is tolerated
    return TRUE;
}

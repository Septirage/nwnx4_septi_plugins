///////////////////////////////////////////////////////////////////////////////////////////
// sept_onitemstack_eff - Base script for the onItemStack event from EnhancedFix&Feature plugin
// Original Scripter:  Septirage
//------------------------------------------------------------------------------
// Last Modified By:   	Septirage			2024/10/28
//------------------------------------------------------------------------------
// oDestinationStack		: The Item(stack) that will receive oSourceStack
// oSourceStack				: The Item(stack) that will be merged in the oDestinationStack
//
//
// Return value					: TRUE if you want to allow the stack. FALSE otherwise.
//////////////////////////////////////////////////////////////////////////////////////////

int StartingConditional(object oDestinationStack, object oSourceStack)
{
	
	return TRUE;
}

/* Note:
 * Before calling this script, the game verifies that oDestination and oSource share the following attributes:
 *     - The same BaseItemType
 *     - The same IsIdentified, IsPlot, IsStolen, and IsCursed status
 *     - The same Tag
 *     - The same LocalizedName
 *     - The same ArmorRuleType (if applicable)
 *     - The same ModelType0/1/2 (if applicable)
 *     - The same list of "on activation" properties
 *     - The same number of "Passive" properties (or the same properties if using the fix)
 *
 * If any of these checks fail, stacking will not occur and the script will not be called.
 */

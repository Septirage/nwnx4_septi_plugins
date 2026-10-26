///////////////////////////////////////////////////////////////////////////////////////////
// sept_oncriticalhit_eff - Base script for the critical event ([CriticalEventRuleX] of the CriticalFile)
//                          from EnhancedFix&Feature plugin
// Original Scripter:  Septirage
//------------------------------------------------------------------------------
// Last Modified By:   	Septirage			2026/11/11
//------------------------------------------------------------------------------
// Called on a critical hit confirmed and not cancelled (critical immunity, Epic Dodge), for every
// [CriticalEventRuleX] whose conditions are true for the attacker. OBJECT_SELF is the attacker.
// The attack is being resolved: its damages are not yet applied, and the critical can't be changed or cancelled.
// Don't destroy oAttacker or oTarget, and don't move them to another area, in this script.
//
// oAttacker					: The creature making the critical hit
// oTarget						: The object hit (usually a creature)
// bOffHand						: TRUE if the attack is made with the off hand
// nMultiplier					: The critical multiplier of the attack
//
// The weapon (if any) is GetItemInSlot(INVENTORY_SLOT_RIGHTHAND, oAttacker), or INVENTORY_SLOT_LEFTHAND with bOffHand
// (a double weapon is in the right hand for both hands).
//
// Return value					: The return value doesn't matter for this specific script
//////////////////////////////////////////////////////////////////////////////////////////

int StartingConditional(object oAttacker, object oTarget, int bOffHand, int nMultiplier)
{
	SendMessageToPC(oAttacker, "Critical hit (x" + IntToString(nMultiplier) + ") on " + GetName(oTarget));

	return TRUE;
}

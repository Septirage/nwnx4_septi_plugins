///////////////////////////////////////////////////////////////////////////////////////////
// sept_globalondestroy_eff - Base script for the global ondestroy hook from EnhancedFix&Feature plugin
// Original Scripter:  Septirage
//------------------------------------------------------------------------------
// Last Modified By:   	Septirage			2026/10/19
//------------------------------------------------------------------------------
// Called just before the real destruction of an object, for the types set in GlobalOnDestroyObjectTypes.
// OBJECT_SELF is the destroyed object. The destruction is certain and can't be cancelled.
//
// Still available: tag, resref, local variables, effects, HP, GetIsDead...
// GetIsDead(OBJECT_SELF) TRUE: the creature died before (OnDeath already done, corpse fading).
// The inventory of a living creature is still there. A dead creature or a placeable already moved it to a loot bag.
// Already out of its area: use GetDestroyedLocation_EFF() instead of GetLocation/GetArea.
//
// Called for: DestroyObject, DM "Destroy" (even with SetIsDestroyable(FALSE)), corpse fading (also after
// a DM "Kill"), item used up (potion, charges, ammo), unsummon, DespawnRosterMember,
// timed area of effect or placed effect, DM "Kill" on a trigger or an encounter.
// Not called for: PC leaving the server (and its items), module unload/server stop,
// items destroyed with their owner, and what the engine doesn't destroy: SetIsDestroyable(FALSE)
// (placeables and stores ignore it), creature controlled by a player, transition door, encounter.
//////////////////////////////////////////////////////////////////////////////////////////

#include "nwnx_enhancedfeatures"

void main()
{
}

///////////////////////////////////////////////////////////////////////////////////////////
// sept_globalondestroy_eff - Base script for the global ondestroy hook from EnhancedFix&Feature plugin
// Original Scripter:  Septirage
//------------------------------------------------------------------------------
// Last Modified By:   	Septirage			2026/10/19
//------------------------------------------------------------------------------
// Called just before the destruction of an object, for the types set in GlobalOnDestroyObjectTypes. Can't be cancelled
// OBJECT_SELF is the destroyed object. Tag, resref, local variables, effects, HP are still available
// Already out of its area : use GetDestroyedLocation_EFF() instead of GetLocation/GetArea
//////////////////////////////////////////////////////////////////////////////////////////

#include "nwnx_enhancedfeatures"

void main()
{
}

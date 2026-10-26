#pragma once

//GetSneakAttackConditions_EFF: the sneak (or death) attack conditions, tested by the engine itself.
//nFeatTest: 0 sneak attack without the feat test, 1 sneak attack with the feat test, 2 death attack.
//Result mask: 1 = possible, 2 = refused by an immunity (sneak attack or critical hit) not bypassed,
//4 = immunity seen and bypassed by the engine (only with 1).

void initSneakAttackConditionsPatch();

int GetSneakAttackConditions(int attackerID, int targetID, int bRanged, int nFeatTest);

//sParams = "target|bRanged|nFeatTest" or "target|bRanged" (from NWNXGetInt)
int GetIntSneakAttackConditions(int attackerID, const char* sParams);

#pragma once

#include <string>
#include <NWN2Lib/NWN2Common.h>

void initCustomValuesHooks(std::string nxhome, std::string sFileName);
//void desinitCustomValuesHooks();

void initCustomValuesNumber(int uNumber);

// Number of CustomValue slots configured via CustomValuesNb (0 if the feature is disabled)
int GetCustomValuesNumber();

enum class CustomValueMode { Raw, Computed, Evolution };

// Mode of a given index. Always Raw for an out of bounds index.
CustomValueMode GetCustomValueMode(int iIdx);


int GetCustomValue(int objectID, int iIdx);
void SetCustomValue(int objectID, int iIdx, int iNumber);



int GetCreatureCustomValue(GameObject* pCreature, int iIdx);


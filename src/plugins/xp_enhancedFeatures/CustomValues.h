#pragma once

#include <string>

//void initCustomValuesHooks(std::string nxhome, std::string sFileName);
//void desinitCustomValuesHooks();

void initCustomValuesNumber(int uNumber);


int GetCustomValue(int objectID, int iIdx);
void SetCustomValue(int objectID, int iIdx, int iNumber);



int GetCreatureCustomValue(GameObject* pCreature, int iIdx);

#pragma once

#include <string>

void initSavingThrowRule(std::string nxhome, std::string sFileName);
void desinitSavingThrowRule();
void initSavingThrowPatch();
int GetSavingThrowVsType(int objectID, int iSaveKind, int iSaveType, int versusID);

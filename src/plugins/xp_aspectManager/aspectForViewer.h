#pragma once

#ifdef WIN32
#include <windows.h>
#endif

#include <stdint.h>
#include <string>

void PatchUserNameForOOCMsg();
void PatchKeepNameForViewer();

bool SetCreatureNameForViewer(uint32_t oCreature, uint32_t oViewer, std::string sFirstName, std::string sLastName);
bool ClearCreatureNameForViewer(uint32_t oCreature, uint32_t oViewer);
void ForgetCreatureNames(uint32_t oCreature);


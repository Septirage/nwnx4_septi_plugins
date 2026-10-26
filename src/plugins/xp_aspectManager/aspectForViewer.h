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
void ForgetNamesForPlayer(uint32_t uPlayerId);

class LogNWNX;

// What is kept for a viewer (names here, SEFs in sefForViewer) is forgotten when the player leaves: every module
// that keeps something per player registers a function receiving the PlayerId.
typedef void (*ViewerForgetCallback)(uint32_t uPlayerId);
void RegisterViewerForgetCallback(ViewerForgetCallback fCallback);

// Register (once) the xp_bugfix disconnection callback. Called by the NWScript entry points before keeping something for a viewer, not at init (xp_bugfix may not be loaded yet)
// Return false if it can't be reached : the script must then call the Clear* functions itself
bool EnsureViewerCleanup(LogNWNX* pLogger);

bool PlayCustomAnimationForViewer(uint32_t oObject, uint32_t oViewer, std::string sAnimation, uint32_t nLooping, float fSpeed);


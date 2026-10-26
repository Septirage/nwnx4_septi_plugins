#pragma once

#ifdef WIN32
#include <windows.h>
#endif

#include <stdint.h>
#include <string>

// SEF visible by a single player (the viewer)

// Apply the SEFForViewer patches and register the SEF cleanup (see RegisterViewerForgetCallback in aspectForViewer.h)
bool InitSEFForViewer();

// Return the handle of the new SEF (> 0), 0 on failure
uint32_t SetSEFForViewer(uint32_t oObject, uint32_t oViewer, const std::string& sSEF,
                         uint32_t oTarget, float fX, float fY, float fZ);

// Remove every SEF named sSEF set on oObject for oViewer
void ClearSEFForViewer(uint32_t oObject, uint32_t oViewer, const std::string& sSEF);

// Remove the SEF returned by SetSEFForViewer
void ClearSEFForViewerById(uint32_t uHandle);

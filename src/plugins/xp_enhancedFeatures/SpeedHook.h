#if !defined(SPT_EFF_ENHANCEDFEAT_SPEEDHOOK)
#define SPT_EFF_ENHANCEDFEAT_SPEEDHOOK


#include <memory>

#ifdef WIN32
#include <windows.h>
#endif

#include "EnhancedFeatures.h"
#include <misc/Patch.h>
#include <misc/ini.h>
#include <misc/log.h>
#include <plugins/plugin.h>
#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>


bool SpeedHookConfigAndApply(SimpleIniConfig* config, LogNWNX* logger);
void SetSpeedFeatFloatValue(float fValue);

#endif
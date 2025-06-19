#pragma once

#include <memory>

#ifdef WIN32
#include <windows.h>
#endif

#include "..\..\misc\Patch.h"
#include "../../misc/ini.h"
#include "../../misc/log.h"
#include "../plugin.h"
#include "../../NWN2Lib/NWN2.h"
#include "../../NWN2Lib/NWN2Common.h"


bool AutoActiveModeDef(SimpleIniConfig* config);
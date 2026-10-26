#pragma once

#include <misc/ini.h>
#include <string_view>


void SmallHookFunctions(SimpleIniConfig* config);

int GetDestroyedArea();
float GetDestroyedLocationFloat(std::string_view sValue);

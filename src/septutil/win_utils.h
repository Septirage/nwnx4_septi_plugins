#pragma once

#include <string>
#include <optional>

/// Returns the ProductVersion string stored in a plugin DLL. nullopt if anything fails.
std::optional<std::string> GetDLLVersion(const char* dllPath);

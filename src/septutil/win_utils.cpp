#include "win_utils.h"

#include <stdint.h>
#include <version>
#include <vector>
#include <windows.h>

// Get the version field from a DLL file
std::optional<std::string> GetDLLVersion(const char* dllPath)
{
	// allocate a block of memory for the version info
	DWORD dummy;
	DWORD dwSize = GetFileVersionInfoSizeA(dllPath, &dummy);
	if (dwSize == 0)
	{
		return std::nullopt;
	}
	std::vector<uint8_t> fileVersionInfo(dwSize);

	// load the version info
	if (!GetFileVersionInfoA(dllPath, NULL, dwSize, fileVersionInfo.data()))
	{
		return std::nullopt;
	}

	// get the version string
	LPVOID pvProductVersion = NULL;
	unsigned int iProductVersionLen = 0;

	if (!VerQueryValueA(fileVersionInfo.data(), "\\StringFileInfo\\040904b0\\ProductVersion", &pvProductVersion, &iProductVersionLen))
	{
		return std::nullopt;
	}

	return std::string((const char*)pvProductVersion, iProductVersionLen);
}

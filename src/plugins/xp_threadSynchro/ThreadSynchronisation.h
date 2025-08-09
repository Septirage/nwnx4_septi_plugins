#if !defined(SEPT_THREADSYNCHRO)
#define SEPT_THREADSYNCHRO

#ifdef WIN32
#include <windows.h>
#endif

#include <misc/ini.h>

bool ThreadPatchFunction(SimpleIniConfig* config);

#endif

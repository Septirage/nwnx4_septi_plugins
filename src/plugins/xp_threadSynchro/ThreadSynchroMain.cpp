#include "ThreadSynchroMain.h"
#include <nwn2heap.h>
#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include <misc/Patch.h>
#include <bit>
#include <cassert>
#include <charconv>
#include <detours/detours.h>
#include <optional>
#include <string_view>
#include <iostream>
#include <fstream>
#include "../../septutil/srvadmin.h"
#include "ThreadSynchronisation.h"

std::unique_ptr<LogNWNX> logger;

namespace {

	//Entry @ for hook

	ThreadSynchro* threadSynch;


	////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////


	auto plugin = std::unique_ptr<ThreadSynchro>();
} // namespace

DLLEXPORT Plugin*
GetPluginPointerV2()
{
	return plugin.get();
}

BOOL APIENTRY
DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		plugin = std::make_unique<ThreadSynchro>();

		char szPath[MAX_PATH];
		GetModuleFileNameA(hModule, szPath, MAX_PATH);
		plugin->SetPluginFullPath(szPath);
	} else if (ul_reason_for_call == DLL_PROCESS_DETACH) {
		plugin.reset();
	}
	return TRUE;
}

ThreadSynchro::ThreadSynchro()
{
	description = "This plugin provides possibility to call execute script on from thread.";

	subClass = FunctionClass;
	version  = PLUGIN_VERSION;
}

ThreadSynchro::~ThreadSynchro(void) {}

LogNWNX *myLog;

DWORD WINAPI LancementTest(LPVOID lpParam)
{
	TstVersionPlg testPlugin(threadSynch->nwnxStringHome, threadSynch->GetVersion(), threadSynch->GetSubClass());
	testPlugin.TestVersionPlugin();
	return 0;
}

bool
ThreadSynchro::Init(char* nwnxhome)
{
	nwnxStringHome = nwnxhome;

	/* Log file */
	std::string logfile(nwnxhome);
	logfile.append("\\");
	logfile.append(GetPluginFileName());
	logfile.append(".txt");

	/* Ini file */
	std::string inifile(nwnxhome);
	inifile.append("\\");
	inifile.append(GetPluginFileName());
	inifile.append(".ini");

	std::string header = "NWNX ThreadSynchro Plugin v"+version+". \n"
		"(c) 2025 by Septirage\n"
		"visit us at http://septirage.com/nwn2/ \n"
		"visit nwnx project at http://www.nwnx.org\n";

	logger = std::make_unique<LogNWNX>(logfile);
	
	logger->Info(header.c_str());

	logger->Info("* reading inifile %s", inifile.c_str());

	config = new SimpleIniConfig(inifile);

	logger->Configure(config);

	ThreadPatchFunction(config);

	logger->Info("* Plugin initialized.");

	threadSynch = this;

	{
		DWORD dwThreadId;
		HANDLE hThread;
		hThread = CreateThread(nullptr, 0, LancementTest, nullptr, 0, &dwThreadId);
	}

	return true;
}


int
ThreadSynchro::GetInt(char* sFunction, [[maybe_unused]] char* sParam1, int nParam2)
{
	std::string function{sFunction};
	std::string sMagicFunc = "Magic|";
	std::string logTxt =
		"ThreadSynchro_GetInt(" + function + "," + sParam1 + "," + std::to_string(nParam2) + ")";

	logger->Trace(logTxt.c_str());

	return 0;
}

void ThreadSynchro::SetInt([[maybe_unused]] char* sFunction,
	[[maybe_unused]] char* sParam1,
	[[maybe_unused]] int nParam2,
	[[maybe_unused]] int nValue)
{
	std::string function{sFunction};
	std::string logTxt =
		"ThreadSynchro_SetInt(" + function + "," + sParam1 + "," + std::to_string(nParam2) + "," + std::to_string(nValue) + ")";

	logger->Trace(logTxt.c_str());

	return;
}


void
ThreadSynchro::SetFloat([[maybe_unused]] char* sFunction,
	[[maybe_unused]] char* sParam1,
	[[maybe_unused]] int nParam2,
	[[maybe_unused]] float fValue)
{
	std::string function{sFunction};
	std::string logTxt =
		"ThreadSynchro_SetFloat(" + function + "," + sParam1 + "," + std::to_string(nParam2) + "," + std::to_string(fValue) + ")";

	logger->Trace(logTxt.c_str());
}

float
ThreadSynchro::GetFloat([[maybe_unused]] char* sFunction,
	[[maybe_unused]] char* sParam1,
	[[maybe_unused]] int nParam2)
{
	return 1.0f;
}

char*
ThreadSynchro::GetString([[maybe_unused]] char* sFunction,
	[[maybe_unused]] char* sParam1,
	[[maybe_unused]] int nParam2)
{
	std::string function{sFunction};
	std::string ret("");
	std::string logTxt = "ThreadSynchro_GetString(" + function + "," + sParam1 + "," +
		std::to_string(nParam2) + ")";

	return (ret.data());
}


void
ThreadSynchro::SetString([[maybe_unused]] char* sFunction,
	[[maybe_unused]] char* sParam1,
	[[maybe_unused]] int nParam2,
	[[maybe_unused]] char* sValue)
{
	std::string function{sFunction};
	std::string ret("");
	std::string logTxt = "ThreadSynchro_SetString(" + function + "," + sParam1 + "," +
		std::to_string(nParam2) + "," + sValue + ")";

	return;
}


void
ThreadSynchro::GetFunctionClass(char* fClass)
{
	static constexpr auto cls = std::string_view(FunctionClass);
	strncpy_s(fClass, 128, cls.data(), std::size(cls));
}




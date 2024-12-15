#pragma once
#define _CRT_SECURE_NO_DEPRECATE
#define DLLEXPORT extern "C" __declspec(dllexport)
// Remembing to define _CRT_RAND_S prior
// to inclusion statement
#define _CRT_RAND_S

#include <memory>

#ifdef WIN32
#include <windows.h>
#include <windowsx.h>
#include <specstrings.h>
#endif

#include <misc/ini.h>
#include <misc/log.h>
#include <plugins/plugin.h>
#include <NWN2Lib/NWN2.h>
#include "appListManagement.h"

#include <codecvt>

__declspec( dllimport )
BOOL
__stdcall
SendMessageToPlayer(
	__in unsigned long PlayerId,
	__in_bcount(Size) unsigned char* Data,
	__in unsigned long Size,
	__in unsigned long Flags
);



DWORD WINAPI LaunchTestVersion(LPVOID);

class AspectManager final : public Plugin, public AppearanceListManagement 
{
  public:
	static constexpr char FunctionClass[] = "AspectManager";

  public:
	AspectManager();
	~AspectManager();

	bool Init(char* nwnxhome) override;
	int GetInt(char* sFunction, char* sParam1, int nParam2) override;
	float GetFloat(char* sFunction, char* sParam1, int nParam2) override;
	char* GetString(char* sFunction, char* sParam1, int nParam2) override;
	void SetInt(char* sFunction, char* sParam1, int nParam2, int nValue) override;
	void SetString(char* sFunction, char* sParam1, int nParam2, char* sValue) override;
	void SetFloat(char* sFunction, char* sParam1, int nParam2, float fValue) override;
	void GetFunctionClass(char* fClass) override;

  std::string GetVersion() { return version; };
	std::string GetSubClass() { return subClass; };

  void RetrieveVersionFromDLL();

  public:

	SimpleIniConfig* config;
	std::string nwnxStringHome;


	bool initialized               = false;
	char returnBuffer[MAX_BUFFER] = {0};

	//List system
	bool bUseListSyst = true;
	std::string S_LIST_SYSTEM = "listSystem";
	std::vector<int> m_FixPart[5];

  private:
	  std::string nwnxhome_ = "";

};

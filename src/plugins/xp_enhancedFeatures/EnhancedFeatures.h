#if !defined(SPT_EFF_ENHANCEDFEAT)
#define SPT_EFF_ENHANCEDFEAT

#define _CRT_SECURE_NO_DEPRECATE
#define DLLEXPORT extern "C" __declspec(dllexport)
// Remembing to define _CRT_RAND_S prior
// to inclusion statement
#define _CRT_RAND_S

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


class EnhancedFeatures final : public Plugin
{
  public:
	static constexpr char FunctionClass[] = "EnhancedFeatures";

  public:
	EnhancedFeatures();
	~EnhancedFeatures();

	bool Init(char* nwnxhome) override;
	int GetInt(char* sFunction, char* sParam1, int nParam2) override;
	void SetInt(char* sFunction, char* sParam1, int nParam2, int nValue) override;
	float GetFloat(char* sFunction, char* sParam1, int nParam2) override;
	void SetFloat(char* sFunction, char* sParam1, int nParam2, float fValue) override;
	char* GetString(char* sFunction, char* sParam1, int nParam2) override;
	void SetString(char* sFunction, char* sParam1, int nParam2, char* sValue) override;
	void GetFunctionClass(char* fClass) override;

	std::string GetVersion() { return version; };
	std::string GetSubClass() { return subClass; };

  public:	
	SimpleIniConfig* config;
	GameObjectManager m_ObjectManager;
	std::string nwnxStringHome;
	std::string m_sSpeedFeatFile;
	std::string m_sSkillHookFile;
	std::string m_sWeaponFinesseFile;


};


#endif

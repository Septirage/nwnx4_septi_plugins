#pragma once
#define _CRT_SECURE_NO_DEPRECATE
#define DLLEXPORT extern "C" __declspec(dllexport)
// Remembing to define _CRT_RAND_S prior
// to inclusion statement
#define _CRT_RAND_S

#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <string_view>

#ifdef WIN32
#include <windows.h>
#endif

#include <misc/ini.h>
#include <misc/log.h>
#include <plugins/plugin.h>
#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>

#include "playerIDManager.h"

#define SCRIPTRESPONSE_BAN		-3
#define SCRIPTRESPONSE_KICK 	-2
#define SCRIPTRESPONSE_ERROR	-1
#define SCRIPTRESPONSE_NOK		0
#define SCRIPTRESPONSE_OK		1


bool LoadNetLayer();


enum MsgServPlayerStatus
{
	Ban = -2,
	Kick = -1,
	Unknow = 0,
	Know = 1,
	Logged = 2
};

struct PlayerConnection {
	bool m_Know = false;
	std::string m_login;
	bool m_BadPassword = false;
	int m_step;
	MsgServPlayerStatus m_status = MsgServPlayerStatus::Unknow;
};

class MsgServ final : public Plugin
{
  public:
	static constexpr char FunctionClass[] = "MsgServer";

  public:
	  MsgServ();
	~MsgServ();

	bool Init(char* nwnxhome) override;
	int GetInt(char* sFunction, char* sParam1, int nParam2) override;
	void SetInt(char* sFunction, char* sParam1, int nParam2, int nValue) override;
	char* GetString(char* sFunction, char* sParam1, int nParam2) override;
	void SetString(char* sFunction, char* sParam1, int nParam2, char* sValue) override;
	void GetFunctionClass(char* fClass) override;



	std::string GetVersion() { return version; };
	std::string GetSubClass() { return subClass; };
	

  public:
	SimpleIniConfig* config;
	std::string nwnxStringHome;
	PlayerIDManager idManager;

	bool bConnectionProcess	= true;
	bool bTraceEveryMsg = false;
	bool bAllowAutoConnect = false;
	bool bWelcomeScreen = false;

	bool b3CDKey = false;


	//

	bool bAnticheatLvlUp = true;
	bool bACLvlUpStopFirstViolation = true;
	bool bGrantedCondForEveryFeats = false;
	bool bCallScriptOnLvlUpError = false;
	std::string ScriptLvlUpError = "";
	std::string ScriptCreationError = "";
	std::list<int> lRangerCombatFeats;

	struct {
		enum ScriptRet{
			WAIT  = 1,
			ALLOW = 2,
			KICK  = 3,
		};

		bool enabled;
		std::string onConnectionScript;

		//std::unordered_map<unsigned long, std::string> connectionGUIScripts;
		std::unordered_set<std::string> connectionGUIScripts;
		std::unordered_set<unsigned long> authorizedPlayerIDs;
		std::unordered_set<unsigned long> kickedPlayerIDs;
	} m_connection;


	void NWNX_DisplayGUIScreen(uint32_t uniquePlayerID, const std::string_view& sceneName, const std::string_view& xmlName);
	void NWNX_CloseGUIScreen(uint32_t uniquePlayerID, const std::string_view& sceneName);
	void NWNX_SetGUIObjectText(uint32_t uniquePlayerID, const std::string_view& sceneName, const std::string_view& objectName, std::string_view text);
	void NWNX_SetGUIObjectHidden(uint32_t uniquePlayerID, const std::string_view& sceneName, const std::string_view& objectName, bool hidden);
	void NWNX_SetGUIObjectDisabled(uint32_t uniquePlayerID, const std::string_view& sceneName, const std::string_view& objectName, bool hidden);
	void NWNX_SendDisplayMessageInfo(uint32_t uniquePlayerID, const std::string_view& sceneName, const std::string_view& sText, const std::string_view& sOk);
};




#include "Connection.h"

#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include <hook/scriptManagement.h>
#include <misc/log.h>
#include <nwn2heap.h>
#include "Anticheat.h"

#include <bit>
#include <cassert>
#include <charconv>
#include <detours/detours.h>
#include <optional>
#include <string_view>
#include <iostream>
#include <array>
#include <span>
#include <fstream>
#include "../../septutil/srvadmin.h"
#include "../../septutil/bytearray.h"

#define MAX_PLAYERS               0x60

#define G_GLOBAL_GAME_STATE		0x86443C

HMODULE XPBugfix;

std::unique_ptr<LogNWNX> logger;

namespace {
	auto plugin = std::unique_ptr<MsgServ>();
}

typedef
BOOL
(__stdcall *
	SendMessageToPlayer)(
		__in unsigned long PlayerId,
		__in_bcount(Size) unsigned char* Data,
		__in unsigned long Size,
		__in unsigned long Flags
		);

SendMessageToPlayer     SendMessageToPlayer_;

typedef
void
(__stdcall * OnPlayerConnectionCloseProc)(
	__in unsigned long PlayerId,
	__in void * Context
	);

typedef
BOOL
(__stdcall * OnPlayerConnectionReceiveProc)(
	__in unsigned long PlayerId,
	__in_bcount( Length ) const unsigned char * Data,
	__in size_t Length,
	__in void * Context
	);

typedef
BOOL
(__stdcall * OnPlayerConnectionSendProc)(
	__in unsigned long PlayerId,
	__in_bcount( Size ) unsigned char * Data,
	__in unsigned long Size,
	__in unsigned long Flags,
	__in void * Context
	);


typedef
void
(__stdcall *
	SetPacketFilterCallouts)(
		__in void* Context,
		__in OnPlayerConnectionCloseProc OnClose,
		__in OnPlayerConnectionReceiveProc OnReceive,
		__in OnPlayerConnectionSendProc OnSend
		);

SetPacketFilterCallouts     SetPacketFilterCallouts_;


struct CPlayerCDKeyInfo
{
	NWN::CExoString m_Key;
	NWN::CExoString m_ValidCode;
	NWN::CExoString m_NotUsed;
};

struct BigPlayerInfo // sizeof = 0x78, CNetLayerPlayerInfo
{
	int            m_bPlayerInUse;              // 00
	NWN::CExoString     m_sPlayerName;               // 04
	char           skip0[0x04];                 // 0c
	unsigned long  m_nSlidingWindowId;          // 10
	int            m_bPlayerPrivileges;         // 14
	int            m_bGameMasterPrivileges;     // 18
	int            m_bServerAdminPrivileges;    // 1c
	char           skip1[0x38];                 // 20
	CPlayerCDKeyInfo* m_lstKeys;				// 58
	int				m_nNumberKeys;				// 5C
	char			skip2[0x18];			// 60
};


struct CShortNetLayerInternal
{
	void         *ServerApp;                 // 00000
	char		 skip0[0x3768C];
	//CExoNet      *Net;                       // 00004
	//char          skip0[0x04];               // 00008
	//SlidingWindow Windows[MAX_PLAYERS];      // 0000c
	//char          skip1[0x04];               // 3768C
	BigPlayerInfo    Players[MAX_PLAYERS];      // 37690
											 // CExoNetExtendableBuffer FrameStorage; // 3A390
};


typedef
const char *
(__stdcall *
GetPlayerAccountName)(
	__in unsigned long PlayerId
);


GetPlayerAccountName     GetPlayerAccountName_;


typedef
bool
(__stdcall*
	GetPlayerConnectionInfo)(
		__in unsigned long PlayerId,
		__out PSOCKADDR_IN Sin
		);

GetPlayerConnectionInfo GetPlayerConnectionInfo_;


	MsgServ* g_msgServ;

NWN::OBJECTID GetModuleID_()
{
	int ptr = *(int*)OFFS_g_pAppManager;
	ptr = *(int*)(ptr + 4);
	ptr = *(int*)(ptr + 4);
	NWN::OBJECTID result = *(NWN::OBJECTID*)(ptr + 0x10088);
	return result;
}

//OFFS_g_pAppManager
//Must be redone to be based on struct instead ugly ptr management
uint8_t* GetPlayerStruct(uint8_t idPlayer)
{
	int var = *(int*)OFFS_g_pAppManager;
	var = var + 4;
	var = *(int*)var;
	int var2 = *(int*)var;
	var2 += 0x1C;
	var2 = *(int*)var2;
	
	//VAR
	var += 4;
	var = *(int*)var;
	var += 0x10068;
	var = *(int*)var;
	var = *(int*)var;



	var += 0x37690;

	var += (idPlayer * 0x78);

	return (uint8_t*)var;
}

//Must be redone to be based on struct instead ugly ptr management
std::string GetCDKey(uint8_t idPlayer)
{
	uint8_t* pMyPlayerStruct = GetPlayerStruct(idPlayer);
	if (pMyPlayerStruct == NULL)
		return "";
	uint8_t* pMyCDKey = (uint8_t*)(((int)pMyPlayerStruct) + 0x58);

	if (pMyCDKey == NULL)
		return "";
	pMyCDKey = *(uint8_t**)(pMyCDKey);


	if (pMyCDKey == NULL)
		return "";

	char* pCDKey1 = *(char**)(pMyCDKey);
	std::string myCdKey = "";
	for (int i = 0; i < 8; i++)
	{
		myCdKey += pCDKey1[i];
	}

	return myCdKey;
}


//Just create a privileges flag based uint8
int GetPlayerPrivileges(int playerid)
{
	uint8_t* pMyPlayerStruct = GetPlayerStruct(playerid);
	int m_bPlayerPrivileges = *(int*)(pMyPlayerStruct + 0x14);
	int m_bGameMasterPrivileges = *(int*)(pMyPlayerStruct + 0x18);
	int m_bServerAdminPrivileges = *(int*)(pMyPlayerStruct + 0x1C);

	int result = 0;
	if (m_bPlayerPrivileges != 0)
		result |= 1;
	if (m_bGameMasterPrivileges != 0)
		result |= 2;
	if (m_bServerAdminPrivileges != 0)
		result |= 4;
	return result;
}

static std::string formatMessage(const std::span<const uint8_t>& message)
{
	std::string sResult = "";
	char buffer[3];
	for (size_t i = 0; i < message.size(); ++i) {
		sprintf(buffer, "%02X ", message[i]);
		sResult += buffer;
	}
	return sResult;
}

void SendOpenGUI(unsigned long playerID, const std::string_view& sceneName, const std::string_view& xmlFileName) {
	//Header + sizeMsg + sizeSceneName + sceneName + sizeXmlName + xmlName
	uint32_t msgDataSize = 3 + 4 + 4 + sceneName.size() + 4 + xmlFileName.size();

	auto msgData = ByteWriter{msgDataSize + 1};

	msgData.write({0x50, 0x24, 0x04});
	msgData.write((uint32_t)msgDataSize);
	msgData.write((uint32_t)sceneName.size());
	msgData.write(sceneName);
	msgData.write((uint32_t)xmlFileName.size());
	msgData.write(xmlFileName);
	msgData.write({0xB3});

	// const auto msgStr = formatMessage(msgData.data);
	// logger->Trace("%s: %s", __FUNCTION__, msgStr.c_str());
	SendMessageToPlayer_(playerID, msgData.data.data(), msgData.data.size(), 0);
}

void SendCloseGUI(unsigned long playerID, const std::string_view& sceneName) {
	//Header + sizeMsg + sizeScene + sceneName
	uint32_t msgDataSize = 3 + 4 + 4 + sceneName.size();

	auto msgData = ByteWriter{msgDataSize + 1};

	msgData.write({0x50, 0x24, 0x07});
	msgData.write((uint32_t)msgDataSize);
	msgData.write((uint32_t)sceneName.size());
	msgData.write(sceneName);
	msgData.write({0x73});

	// const auto msgStr = formatMessage(msgData.data);
	// logger->Trace("%s: %s", __FUNCTION__, msgStr.c_str());
	SendMessageToPlayer_(playerID, msgData.data.data(), msgData.data.size(), 0);
}

void SendSetGuiObjectText(unsigned long playerID, const std::string_view& sceneName, const std::string_view& sUIObjectName, const std::string_view& sText) {
	//Header + sizeMsg + sizeScene + sceneName + sizeUiObject + UIObjectName + sizeTxt + Txt
	uint32_t msgDataSize = 3 + 4 + 4 + sceneName.size() + 4 + sUIObjectName.size() + 4 + sText.size();
	auto msgData = ByteWriter{msgDataSize + 1};

	msgData.write({0x50, 0x24, 0x0A});
	msgData.write((uint32_t)msgDataSize);
	msgData.write((uint32_t)sceneName.size());
	msgData.write(sceneName);
	msgData.write((uint32_t)sUIObjectName.size());
	msgData.write(sUIObjectName);
	msgData.write((uint32_t)sText.size());
	msgData.write(sText);
	msgData.write({0x93});

	// const auto msgStr = formatMessage(msgData.data);
	// logger->Trace("%s: %s", __FUNCTION__, msgStr.c_str());
	SendMessageToPlayer_(playerID, msgData.data.data(), msgData.data.size(), 0);
}

void SendSetGUIObjectHidden(unsigned long playerID, const std::string_view& sceneName, const std::string_view& sUIObjectName, bool hidden) {
	//Header + sizeMsg + sizeScene + sceneName + sizeUIObject + UIObjectName
	uint32_t msgDataSize = 3 + 4 + 4 + sceneName.size() + 4 + sUIObjectName.size();
	auto msgData = ByteWriter{msgDataSize + 1};

	msgData.write({0x50, 0x24, 0x06});
	msgData.write((uint32_t)msgDataSize);
	msgData.write((uint32_t)sceneName.size());
	msgData.write(sceneName);
	msgData.write((uint32_t)sUIObjectName.size());
	msgData.write(sUIObjectName);
	if(hidden)
		msgData.write({0x97});
	else
		msgData.write({0x87});


	// const auto msgStr = formatMessage(msgData.data);
	// logger->Trace("%s: %s", __FUNCTION__, msgStr.c_str());
	SendMessageToPlayer_(playerID, msgData.data.data(), msgData.data.size(), 0);
}

void SendSetGUIObjectDisabled(unsigned long playerID, const std::string_view& sceneName, const std::string_view& sUIObjectName, bool disabled) {
	//Header + sizeMsg + sizeScene + sceneName + sizeUIObject + UIObjectName
	uint32_t msgDataSize = 3 + 4 + 4 + sceneName.size() + 4 + sUIObjectName.size();
	auto msgData = ByteWriter{msgDataSize + 1};

	msgData.write({0x50, 0x24, 0x09});
	msgData.write((uint32_t)msgDataSize);
	msgData.write((uint32_t)sceneName.size());
	msgData.write(sceneName);
	msgData.write((uint32_t)sUIObjectName.size());
	msgData.write(sUIObjectName);
	if(disabled)
		msgData.write({0x97});
	else
		msgData.write({0x87});


	// const auto msgStr = formatMessage(msgData.data);
	// logger->Trace("%s: %s", __FUNCTION__, msgStr.c_str());
	SendMessageToPlayer_(playerID, msgData.data.data(), msgData.data.size(), 0);
}

void SendDisplayMessageInfo(unsigned long playerID, const std::string_view& sceneName, const std::string_view& sText, const std::string_view& sOk) {
	//Header + sizeMsg + sizeTxt + Text + 4 + 4 + 4 + sizeScene + sceneName + sizeOk + OkTxt
	uint32_t msgDataSize = 3 + 4 + sText.size() + 4 + 4 + 4 + 4 + sceneName.size() + 4 + sOk.size();
	auto msgData = ByteWriter{msgDataSize + 1};


	msgData.write({0x50, 0x24, 0x09});
	msgData.write((uint32_t)msgDataSize);
	msgData.write((uint32_t)sText.size());
	msgData.write(sText);
	/* For now, no callbacks, no cancel */
	msgData.write((uint32_t)0);
	msgData.write((uint32_t)0);
	msgData.write((uint32_t)0);

	msgData.write((uint32_t)sceneName.size());
	msgData.write(sceneName);

	msgData.write((uint32_t)sOk.size());
	msgData.write(sOk);

	msgData.write({0x0C});

	// const auto msgStr = formatMessage(msgData.data);
	// logger->Trace("%s: %s", __FUNCTION__, msgStr.c_str());
	SendMessageToPlayer_(playerID, msgData.data.data(), msgData.data.size(), 0);
}

void PrintFullMessage(const unsigned char* Data, size_t size, std::string sCharName)
{
	if(logger->Level() >= LogLevel::info)
	{
		std::string sResult = "";
		char buffer[3];
		for (size_t i = 0; i < size; ++i) {
			sprintf(buffer, "%02X ", Data[i]);
			sResult += buffer;
		}
		logger->Info("Message from %s: \n\t %s", sCharName.c_str(), sResult.c_str());
	}
}


template<typename T>
bool operator==(const std::span<const T> a, const std::initializer_list<T>& b){
	if(a.size() != b.size())
		return FALSE;
	else
		return memcmp(a.data(), std::data(b), a.size()) == 0;
}

void __stdcall MsgServOnClose(
__in unsigned long PlayerId,
__in void* Context
)
{
	//Remove every knowledge about this PlayerID
	g_msgServ->idManager.removeByPlayerID(PlayerId);
	g_msgServ->m_connection.authorizedPlayerIDs.erase(PlayerId);
	g_msgServ->m_connection.kickedPlayerIDs.erase(PlayerId);
}

BOOL __stdcall MsgServOnReceive(
	__in unsigned long playerId,
	__in_bcount(Length) const unsigned char * Data,
	__in size_t Length,
	__in void * Context
)
{
	auto data = std::span{ (const uint8_t*)Data, Length };
	try
	{
		const char* accountName = GetPlayerAccountName_(playerId);

		//Only if we want to trace everything
		if (g_msgServ->bTraceEveryMsg)
		{
			PrintFullMessage(Data, Length, accountName);
		}

		//Only if we want to active loggin module.
		if (g_msgServ->m_connection.enabled)
		{
			//Will not manage bad message
			if (data.size() > 1 && data[0] == 0x70) 
			{
				//Module.ModuleLoaded. "Initial message"
				if (data.size() == 10 && data.subspan(1, 8) == std::initializer_list<uint8_t>{0x03, 0x02, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00})
				{
					//New player
					uint32_t uniqueID = g_msgServ->idManager.onNewPlayer(playerId);
					
					//OnConnection Script
					if (!g_msgServ->m_connection.onConnectionScript.empty())
					{
						// Reset player authorizations (should already be done but, better safe than sorry)
						g_msgServ->m_connection.authorizedPlayerIDs.erase(playerId);
						g_msgServ->m_connection.kickedPlayerIDs.erase(playerId);

						std::string currentIP_ = [&](){
							sockaddr_in sin;
							GetPlayerConnectionInfo_(playerId, &sin);
							const auto ip = sin.sin_addr.s_addr;
							return std::format("{}.{}.{}.{}", ip & 0xFF, (ip>>8) & 0xFF, (ip>>16) & 0xFF, (ip>>24) & 0xFF);
						}();

						std::string currentCdKey_ = GetCDKey(playerId);
						int currentPlayerPriv_ = GetPlayerPrivileges(playerId);

						bool isExecScriptOk = false;

						NWScript::ClearScriptParams();
						NWScript::AddScriptParameterInt((int32_t)uniqueID);
						NWScript::AddScriptParameterString(accountName);
						NWScript::AddScriptParameterString(currentIP_.c_str());
						NWScript::AddScriptParameterString(currentCdKey_.c_str());
						NWScript::AddScriptParameterInt(currentPlayerPriv_);
						int scriptRes = NWScript::ExecuteScriptEnhanced(g_msgServ->m_connection.onConnectionScript.c_str(), GetModuleID_(), true, &isExecScriptOk, true);

						if (!isExecScriptOk)
						{
							//Error on script, log and continue as not autoconnected.
							logger->Err("Failed to execute Heimdall OnConnection Script: %s(%u, %s, %s, %s, %d)",
								g_msgServ->m_connection.onConnectionScript.c_str(),
								uniqueID,
								accountName,
								currentIP_.c_str(),
								currentCdKey_.c_str(),
								currentPlayerPriv_
							);
						}
						else
						{
							logger->Trace("Executed Heimdall OnConnection Script: %s(%u, %s, %s, %s, %d), returned %d",
								g_msgServ->m_connection.onConnectionScript.c_str(),
								uniqueID,
								accountName,
								currentIP_.c_str(),
								currentCdKey_.c_str(),
								currentPlayerPriv_,
								scriptRes
							);
						}

						using ScriptRet = decltype(MsgServ::m_connection)::ScriptRet;
						switch (scriptRes) {
							case ScriptRet::ALLOW:
								g_msgServ->m_connection.authorizedPlayerIDs.insert(playerId);
								break;
							case ScriptRet::WAIT:
								break;
							case ScriptRet::KICK:
								g_msgServ->m_connection.kickedPlayerIDs.insert(playerId);
								return FALSE;
							default:
								//Error on script, log and continue as not autoconnected.
								logger->Err("Heimdall OnConnection Script (%s) returned unknown value %d. params : %u, %s, %s, %s, %d",
									g_msgServ->m_connection.onConnectionScript.c_str(),
									scriptRes,
									uniqueID,
									accountName,
									currentIP_.c_str(),
									currentCdKey_.c_str(),
									currentPlayerPriv_
								);
								break;
						}
					}
				}
				//Manage other messages only for not authorizedPlayers
				else if (!g_msgServ->m_connection.authorizedPlayerIDs.contains(playerId)) 
				{
					//We are in "kick" mode. Don't allow any messages
					if (g_msgServ->m_connection.kickedPlayerIDs.contains(playerId))
					{
						const std::string currentIP_ = [&](){
							sockaddr_in sin;
							GetPlayerConnectionInfo_(playerId, &sin);
							const auto ip = sin.sin_addr.s_addr;
							return std::format("{}.{}.{}.{}", ip & 0xFF, (ip>>8) & 0xFF, (ip>>16) & 0xFF, (ip>>24) & 0xFF);
						}();

						logger->Warn("Kicked player %s (IP: %s) try to send messages.", accountName, currentIP_.c_str());
						return FALSE;
					}

					if (data.size() >= 11 && data.subspan(1, 2) == std::initializer_list<uint8_t>{0x06, 0x30})
					{
						// Parse script name
						auto reader = ByteReader{ data.subspan(7) };
						const auto scriptNameLen = reader.read<uint32_t>();
						if (!scriptNameLen)
							return FALSE; //Bad message

						const auto scriptName = reader.read_string(*scriptNameLen);
						if (!scriptName)
							return FALSE; //Bad message

						const auto scriptNameStr = std::string{ *scriptName };

						if(g_msgServ->m_connection.connectionGUIScripts.contains(scriptNameStr))
						{
							// Script is a connection script
							auto uniquePlayerID = g_msgServ->idManager.getUniquePlayerID(playerId);
							if (!uniquePlayerID) {
								//Should not be here without uniquePlayerID
								const std::string currentIP_ = [&](){
									sockaddr_in sin;
									GetPlayerConnectionInfo_(playerId, &sin);
									const auto ip = sin.sin_addr.s_addr;
									return std::format("{}.{}.{}.{}", ip & 0xFF, (ip>>8) & 0xFF, (ip>>16) & 0xFF, (ip>>24) & 0xFF);
								}();
								logger->Err("Player %s (IP:%s) try to execute a connection script before first step", accountName, currentIP_.c_str());
								return FALSE;
							}

							// Parse additional script args
							std::vector<std::string> extraStringArgs;

							const auto argsCount = reader.read<uint8_t>();
							if (!argsCount)
								return false;

							for (uint8_t i = 0; i < *argsCount; i++) {
								const auto length = reader.read<uint32_t>();
								if (!length)
									return false;
								const auto value = reader.read_string(*length);
								if (!value)
									return false;

								extraStringArgs.push_back(std::string{ *value });
							}

							const std::string currentIP_ = [&](){
								sockaddr_in sin;
								GetPlayerConnectionInfo_(playerId, &sin);
								const auto ip = sin.sin_addr.s_addr;
								return std::format("{}.{}.{}.{}", ip & 0xFF, (ip>>8) & 0xFF, (ip>>16) & 0xFF, (ip>>24) & 0xFF);
							}();

							const std::string currentCdKey_ = GetCDKey(playerId);
							const int currentPlayerPriv_ = GetPlayerPrivileges(playerId);
							bool isExecScriptOk = false;

							NWScript::ClearScriptParams();
							NWScript::AddScriptParameterInt(*uniquePlayerID);
							NWScript::AddScriptParameterString(accountName);
							NWScript::AddScriptParameterString(currentIP_.c_str());
							NWScript::AddScriptParameterString(currentCdKey_.c_str());
							NWScript::AddScriptParameterInt(currentPlayerPriv_);

							// Prevent logger from logging password
							const auto logLevel = logger->Level();
							if (logLevel >= LogLevel::trace) {
								logger->Trace("Prevent logger from logging password, following AddScriptParameter calls will use debug log level");
								logger->SetLogLevel(LogLevel::debug);
							}

							for (const auto& arg : extraStringArgs) {
								NWScript::AddScriptParameterString(arg.c_str());
							}

							if (logLevel >= LogLevel::trace) {
								logger->SetLogLevel(logLevel);
							}

							int scriptRes = NWScript::ExecuteScriptEnhanced(scriptNameStr.c_str(), GetModuleID_(), true, &isExecScriptOk, true);

							if (!isExecScriptOk)
							{
								//Error on script, log and continue as not autoconnected.
								logger->Err("Failed to execute Heimdall GUI Script: %s(%u, %s, %s, %s, %d, +%d GUI args)",
									scriptNameStr.c_str(),
									*uniquePlayerID,
									accountName,
									currentIP_.c_str(),
									currentCdKey_.c_str(),
									currentPlayerPriv_,
									extraStringArgs.size()
									);
							}
							else
							{
								logger->Debug("Executed Heimdall GUI Script: %s(%u, %s, %s, %s, %d, +%d GUI args), returned %d",
									scriptNameStr.c_str(),
									*uniquePlayerID,
									accountName,
									currentIP_.c_str(),
									currentCdKey_.c_str(),
									currentPlayerPriv_,
									extraStringArgs.size(),
									scriptRes
								);
							}

							using ScriptRet = decltype(MsgServ::m_connection)::ScriptRet;
							switch (scriptRes)
							{
							case ScriptRet::ALLOW:
								g_msgServ->m_connection.authorizedPlayerIDs.insert(playerId);
								break;
							case ScriptRet::WAIT:
								break;
							case ScriptRet::KICK:
								g_msgServ->m_connection.kickedPlayerIDs.insert(playerId);
								return FALSE;
							default:
								//Error on script, log and continue as not connected.
								logger->Err("Heimdall GUI Script (%s) returned unknown value %d. params : %u, %s, %s, %s, %d, [redacted]",
									g_msgServ->m_connection.onConnectionScript.c_str(),
									*uniquePlayerID,
									scriptRes,
									accountName,
									currentIP_.c_str(),
									currentCdKey_.c_str(),
									currentPlayerPriv_
								);
								return FALSE;
							}

							//we already executed the script
							return FALSE;
						}
					}
					else
					{
						bool bIllegal = false;
						//Charlist, never allow that without login
						if (data[1] == 0x11)
						{
							bIllegal = true;
						}
						else if (data[1] == 0x2)
						{
							uint8_t cSubType = data[2];
							//Don't allow to create or load character
							if (cSubType == 0x01 || cSubType == 0x02 || cSubType == 0x04 ||
								cSubType == 0x0e || cSubType == 0x0f || cSubType == 0x11 ||
								cSubType == 0x13)
							{
								bIllegal = true;
							}
						}

						if (bIllegal)
						{
							const std::string currentIP_ = [&]() {
								sockaddr_in sin;
								GetPlayerConnectionInfo_(playerId, &sin);
								const auto ip = sin.sin_addr.s_addr;
								return std::format("{}.{}.{}.{}", ip & 0xFF, (ip>>8) & 0xFF, (ip>>16) & 0xFF, (ip>>24) & 0xFF);
							}();

							logger->Warn("Player %s (IP: %s) attempted to access the character list, creation, or loading without being properly authenticated.", accountName, currentIP_.c_str());
							return FALSE; //TODO
						}
					}
				}
			}
		}

		//Only if we want to active Anticheat module.
		{
			//EndOfLevelUp
			if (Data[0] == 0x70 && Data[1] == 0x1D && Data[2] == 0x00 && g_msgServ->bAnticheatLvlUp)
			{
				int iResLvlUp = CheckForLevelUp(playerId, Data, Length, accountName, g_msgServ);
				if (iResLvlUp <= 0)
					return FALSE;
			}
		}
	}
	catch (std::exception& e)
	{
		std::string nomExcp(e.what());
		std::string logTxt2 = "Exception : " + nomExcp;
		logger->Err(logTxt2.c_str());
	}
	catch (...)
	{
		std::string logTxt2 = "Exception ! => " + std::to_string(Length) + ", id : " + std::to_string(playerId) + ")";
		logger->Err(logTxt2.c_str());
	}

	return TRUE;
}

DLLEXPORT Plugin*
GetPluginPointerV2()
{
	return plugin.get();
}

BOOL APIENTRY
DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
		plugin = std::make_unique<MsgServ>();

		char szPath[MAX_PATH];
		GetModuleFileNameA(hModule, szPath, MAX_PATH);
		plugin->SetPluginFullPath(szPath);
	} else if (ul_reason_for_call == DLL_PROCESS_DETACH) {
		plugin.reset();
	}
	return TRUE;
}

MsgServ::MsgServ()
{
	description = "This plugin provides script access to server received msg.";

	subClass = FunctionClass;
	static std::string stest(PLUGIN_VERSION);
	version  = stest;
}

MsgServ::~MsgServ(void) {}


DWORD WINAPI LaunchTestVersion(LPVOID lpParam)
{
	TstVersionPlg testPlugin(g_msgServ->nwnxStringHome, g_msgServ->GetVersion(), g_msgServ->GetSubClass());
	testPlugin.TestVersionPlugin();
	return 0;
}

bool
MsgServ::Init(char* nwnxhome)
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

	std::string header = "NWNX MsgServ Plugin v"+version+"\n"
	                     "(c) 2025 by Septirage\n"
	                     "visit us at http://septirage.com/nwn2/ \n"
	                     "visit nwnx project at http://www.nwnx.org\n";

	logger = std::make_unique<LogNWNX>(logfile);
	//logger = new LogNWNX(logfile);
	logger->Info(header.c_str());

	 logger->Trace("* reading inifile %s", inifile.c_str());

	 config = new SimpleIniConfig(inifile);

	 logger->Configure(config);

	 int i3CDKey = 0;
	 int iConnectionProcess = 0;
	 int iTraceEveryMsg = 0;

	 config->Read("UseConnectionSystem", &iConnectionProcess, 1);
	 m_connection.enabled = (iConnectionProcess != 0);

	 config->Read("OnConnectionScript", &m_connection.onConnectionScript, std::string(""));


	 /*
		#
		# Mostly for debug purpose. If you are at least in loglevel info, it will 
		# trace every messages received by the server.
		# Should probably not be used in prod
		#
		# LogEveryMsg = 0
	*/
	 config->Read("LogEveryMsg", &iTraceEveryMsg, 0);
	 if (iTraceEveryMsg != 0)
	 {
		 bTraceEveryMsg = true;
		 logger->Debug("Will log every message received");
	 }

	 config->Read("MCDKey", &i3CDKey, 0);
	 b3CDKey = (i3CDKey != 0);

	 if (bConnectionProcess) {
		 logger->Debug("UseConnectionSystem set to true.");
	 }
	 else {
		 logger->Debug("UseConnectionSystem set to false.");
	 }

	 if (m_connection.onConnectionScript != "") {
		 logger->Debug("OnConnection script %s", m_connection.onConnectionScript.c_str());
	 }
	 else {
		 logger->Debug("OnConnection un-available (no script passed)");
	 }

	 //Anticheat system
	 {
		 int iAnticheatCreation = 0;

		 config->Read("UseAnticheatCreation", &iAnticheatCreation, 1);
		// if (iAnticheatCreation != 0)
		 {
			 ApplyAntiCheatCreationPatch(config, iAnticheatCreation != 0);
		 }


		 int iAnticheatLvlUp = 0;
		 int iACLvlUpStopFirstViolation = 0;
		 int iGrantedCondForEveryFeats = 0;
		 int iCallScriptOnLvlUpError = 0;

		 config->Read("UseAnticheatLvlUp", &iAnticheatLvlUp, 1);
		 bAnticheatLvlUp = (iAnticheatLvlUp != 0);
		 config->Read("StopLvlUpFirstViolation", &iACLvlUpStopFirstViolation, 1);
		 bACLvlUpStopFirstViolation = (iACLvlUpStopFirstViolation != 0);

		 config->Read("GrantedCondForEveryFeats", &iGrantedCondForEveryFeats, 0);
		 bGrantedCondForEveryFeats = (iGrantedCondForEveryFeats != 0);


		 logger->Debug("UseAnticheatLvlUp set to %s", bAnticheatLvlUp ? "TRUE" : "FALSE");

		 //Script on error ?
		 //config->Read("CallScriptOnLvlUpError", &iCallScriptOnLvlUpError, 0);
		 //bCallScriptOnLvlUpError = (iCallScriptOnLvlUpError != 0);
		 //if (bCallScriptOnLvlUpError)
		 {
			 config->Read("ScriptLvlUpError", &ScriptLvlUpError, std::string(""));
			 if (ScriptLvlUpError == "")
				 bCallScriptOnLvlUpError = false;
			 else
				 bCallScriptOnLvlUpError = true;
		 }
		 

		 std::string RangerCombatFeats = "";
		 config->Read("RangerCombatStyleFeats", &RangerCombatFeats, std::string("1729 1730"));

		 int num = 0;
		 std::istringstream iss(RangerCombatFeats);
		 while (iss >> num)
		 {
			 if (num < 0)
				 break;
			 lRangerCombatFeats.push_back(num);
		 }

		 if (bAnticheatLvlUp)
		 {
			 if (bACLvlUpStopFirstViolation)
				 logger->Debug("StopLvlUp at First Violation");
			 if (bGrantedCondForEveryFeats)
				 logger->Debug("GrantedCondition work for every Feats");
			 if (bCallScriptOnLvlUpError)
				 logger->Debug("Script on LevelUp error : %s", ScriptLvlUpError.c_str());
			 logger->Debug("Ranger CombatStyle Feats : %s", RangerCombatFeats.c_str());
		 }

	 }



	 bool bIsLoadOk = true;

	if (LoadNetLayer())
		logger->Info("* BugFix Link successful");
	else
	{
		logger->Info("* Link failed");
		bIsLoadOk = false;
	}

	if (bIsLoadOk)
	{
		SetPacketFilterCallouts_(nullptr, MsgServOnClose, MsgServOnReceive, nullptr);
		logger->Info("* Plugin initialized.");

		g_msgServ = this;

		DWORD dwThreadId;
		HANDLE hThread;
		hThread = CreateThread(nullptr, 0, LaunchTestVersion, nullptr, 0, &dwThreadId);
	}

	return bIsLoadOk;
}


int version_compare(std::string v1, std::string v2)
{
	size_t i=0, j=0;
	while( i < v1.length() || j < v2.length() )
	{
		int acc1=0, acc2=0;

		while (i < v1.length() && v1[i] != '.') {  acc1 = acc1 * 10 + (v1[i] - '0');  i++;  }
		while (j < v2.length() && v2[j] != '.') {  acc2 = acc2 * 10 + (v2[j] - '0');  j++;  }

		if (acc1 < acc2)  return -1;
		if (acc1 > acc2)  return +1;

		++i;
		++j;
	}
	return 0;
}

bool LoadNetLayer()
{
	//
	// Wire up the dllimports.
	//

	struct { bool Required; const char *Name; void **Import; } DllImports[] =
	{
		{ true , "SendMessageToPlayer",     (void**)&SendMessageToPlayer_     },
		{ true , "GetPlayerAccountName",    (void**)&GetPlayerAccountName_    },
		{ true , "GetPlayerConnectionInfo", (void**)&GetPlayerConnectionInfo_ },
		{ true , "SetPacketFilterCallouts", (void**)&SetPacketFilterCallouts_ },
	};
	XPBugfix = LoadLibraryA("xp_bugfix.dll");

	if (!XPBugfix)
	{
		logger->Info("* Failed to load xp_bugfix.dll");
		return false;
	}

	for (int i = 0; i < sizeof(DllImports)/sizeof(DllImports[0]); i += 1)
	{
		*DllImports[i].Import = (void *)GetProcAddress(XPBugfix, DllImports[i].Name);

		if (!*DllImports[i].Import)
		{
			if (!DllImports[i].Required)
			{
				logger->Info(
					"* Warning: You need to update your xp_bugfix.dll; missing optional entrypoint xp_bugfix!%s",
					DllImports[i].Name);
				continue;
			}
			logger->Info("* Unable to resolve XPBugfix!%s", DllImports[i].Name);
			return false;
		}
	}

	return true;
}

std::tuple<std::string_view, std::string_view> splitSceneObjName(const std::string_view& text){
	const auto pos = text.find('>');
	if(pos != std::string::npos)
		return {text.substr(0, pos), text.substr(pos + 1)};
	return {text, ""};
}

int
MsgServ::GetInt(char* sFunction, [[maybe_unused]] char* sParam1, int nParam2)
{
	std::string function{sFunction};
	std::string logTxt =
		"MsgServ_GetInt(" + function + "," + sParam1 + "," + std::to_string(nParam2) + ")";

	logger->Trace(logTxt.c_str());

	return 0;
}

void MsgServ::SetInt([[maybe_unused]] char* sFunction,
	[[maybe_unused]] char* sParam1,
	[[maybe_unused]] int nParam2,
	[[maybe_unused]] int nValue)
{
	std::string function{sFunction};
	std::string logTxt =
		"MsgServ_SetInt(" + function + "," + sParam1 + "," + std::to_string(nParam2) + "," + std::to_string(nValue) + ")";

	logger->Trace(logTxt.c_str());

	if (function == "ConnectionSystem") {
		bConnectionProcess  = (nValue != 0);
		logTxt = "Set Connection Process to : ";
		if (bConnectionProcess)
			logTxt += "TRUE";
		else
			logTxt += "FALSE";
		logger->Trace(logTxt.c_str());
	}
	else if (function == "AnticheatCreation")
	{
		ChangeCharacterCreationStatus((nValue != 0));
		logTxt = "Set AnticheatCreation System to : ";
		if (nValue != 0)
			logTxt += "TRUE";
		else
			logTxt += "FALSE";
		logger->Trace(logTxt.c_str());
	}
	else if (function == "AnticheatLvlUp")
	{
		bAnticheatLvlUp = (nValue != 0);
		logTxt = "Set AnticheatLevelup System to : ";
		if (bAnticheatLvlUp)
			logTxt += "TRUE";
		else
			logTxt += "FALSE";
		logger->Trace(logTxt.c_str());
	}
	else if (function == "SetGUIObjectHidden") {
		auto [scene, objectName] = splitSceneObjName(sParam1);
		if(!objectName.empty()) {
			NWNX_SetGUIObjectHidden((uint32_t)nParam2, scene, objectName, nValue != 0);
		}
		else {
			logger->Err("malformed scene+object name");
		}
	}
	else if (function == "SetGUIObjectDisable") {
		auto [scene, objectName] = splitSceneObjName(sParam1);
		if(!objectName.empty()) {
			NWNX_SetGUIObjectDisabled((uint32_t)nParam2, scene, objectName, nValue != 0);
		}
		else {
			logger->Err("malformed scene+object name");
		}
	}
	else if (function == "SetPlayerAllowed") {

		auto currentPlayerId = idManager.getPlayerID(nParam2);
		if (currentPlayerId) {
			m_connection.authorizedPlayerIDs.insert(*currentPlayerId);
		}
		else {
			logger->Err("Invalid UniquePlayerID");
		}
	}

	return;
}


char*
MsgServ::GetString([[maybe_unused]] char* sFunction,
	[[maybe_unused]] char* sParam1,
	[[maybe_unused]] int nParam2)
{
	std::string function{sFunction};
	std::string ret("");

	return (ret.data());
}

void
MsgServ::SetString([[maybe_unused]] char* sFunction,
	[[maybe_unused]] char* sParam1,
	[[maybe_unused]] int nParam2,
	[[maybe_unused]] char* sValue)
{
	logger->Trace("MsgServ_SetString(%s, %s, %d, %s)", sFunction, sParam1, nParam2, sValue);

	std::string_view function{sFunction};

	if (function == "DisplayGUIScreen") {
		NWNX_DisplayGUIScreen((uint32_t)nParam2, sParam1, sValue);
	}
	else if (function == "CloseGUIScreen") {
		NWNX_CloseGUIScreen((uint32_t)nParam2, sParam1);
	}
	else if (function == "SetGUIObjectText") {
		auto [scene, objectName] = splitSceneObjName(sParam1);
		if(!objectName.empty()) {
			NWNX_SetGUIObjectText((uint32_t)nParam2, scene, objectName, sValue);
		}
		else {
			logger->Err("malformed scene+object name");
		}
	}
	else if (function == "DisplayMessageInfo") {
		auto [scene, okText] = splitSceneObjName(sParam1);
		if(!okText.empty()) {
			NWNX_SendDisplayMessageInfo((uint32_t)nParam2, scene, sValue, okText);
		}
		else {
			logger->Err("malformed scene+okText name");
		}
	}
	else if (function == "SetGUIScript") {
		m_connection.connectionGUIScripts.insert(sValue);
	}

	return;
}





void MsgServ::NWNX_DisplayGUIScreen(uint32_t uniquePlayerID, const std::string_view& sceneName, const std::string_view& xmlName)
{
	auto currentPlayerId = idManager.getPlayerID(uniquePlayerID);
	if (currentPlayerId) {
		SendOpenGUI(*currentPlayerId, sceneName, xmlName);
	}
	else {
		logger->Err("Invalid UniquePlayerID");
	}
}

void MsgServ::NWNX_CloseGUIScreen(uint32_t uniquePlayerID, const std::string_view& sceneName)
{
	auto currentPlayerId = idManager.getPlayerID(uniquePlayerID);
	if (currentPlayerId) {
		SendCloseGUI(*currentPlayerId, sceneName);
	}
	else {
		logger->Err("Invalid UniquePlayerID");
	}
}

void MsgServ::NWNX_SetGUIObjectText(uint32_t uniquePlayerID, const std::string_view& sceneName, const std::string_view& objectName, std::string_view text) {
	auto currentPlayerId = idManager.getPlayerID(uniquePlayerID);
	if (currentPlayerId) {
		SendSetGuiObjectText(*currentPlayerId, sceneName, objectName, text);
	}
	else {
		logger->Err("Invalid UniquePlayerID");
	}
}

void MsgServ::NWNX_SetGUIObjectHidden(uint32_t uniquePlayerID, const std::string_view& sceneName, const std::string_view& objectName, bool hidden) {
	auto currentPlayerId = idManager.getPlayerID(uniquePlayerID);
	if (currentPlayerId) {
		SendSetGUIObjectHidden(*currentPlayerId, sceneName, objectName, hidden);
	}
	else {
		logger->Err("Invalid UniquePlayerID");
	}
}

void MsgServ::NWNX_SetGUIObjectDisabled(uint32_t uniquePlayerID, const std::string_view& sceneName, const std::string_view& objectName, bool disabled) {
	auto currentPlayerId = idManager.getPlayerID(uniquePlayerID);
	if (currentPlayerId) {
		SendSetGUIObjectDisabled(*currentPlayerId, sceneName, objectName, disabled);
	}
	else {
		logger->Err("Invalid UniquePlayerID");
	}
}

void MsgServ::NWNX_SendDisplayMessageInfo(uint32_t uniquePlayerID, const std::string_view& sceneName, const std::string_view& sText, const std::string_view& sOk)
{
	auto currentPlayerId = idManager.getPlayerID(uniquePlayerID);
	if (currentPlayerId) {
		SendDisplayMessageInfo(*currentPlayerId, sceneName, sText, sOk);
	}
	else {
		logger->Err("Invalid UniquePlayerID");
	}
}
	

void
MsgServ::GetFunctionClass(char* fClass)
{
	static constexpr auto cls = std::string_view(FunctionClass);
	strncpy_s(fClass, 128, cls.data(), std::size(cls));
}

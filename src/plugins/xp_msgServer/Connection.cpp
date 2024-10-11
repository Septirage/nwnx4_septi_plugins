#include "Connection.h"
#include "messageConst.h"

#include "../../NWN2Lib/NWN2.h"
#include "../../NWN2Lib/NWN2Common.h"
#include "../../hook/scriptManagement.h"
#include "../../misc/log.h"
#include "nwn2heap.h"
#include "Anticheat.h"

#include <bit>
#include <cassert>
#include <charconv>
#include <detours/detours.h>
#include <optional>
#include <string_view>
#include <iostream>
#include <fstream>
#include "../../septutil/srvadmin.h"

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
	// 1.23
	// g_pVirtualMachine
	constexpr uintptr_t NWN2_OFFSET_CVIRTUALMACHINE = 0x00864424;
	// CVirtualMachine::ExecuteScript
	constexpr uintptr_t NWN2_OFFSET_EXECUTESCRIPT = 0x0072B380;
	constexpr uintptr_t NWN2_OFFSET_EXECUTESCRIPT_ENH = 0x0072B050;

	constexpr uintptr_t NWN2_OFFSET_InitParam = 0x0055EA40;
	constexpr uintptr_t NWN2_OFFSET_CleanParam = 0x006b5cd0;

	struct CVirtualMachine
	{

	};

	using CVirtualMachine_ExecuteScript_t = BOOL(__thiscall*)(CVirtualMachine* thisVM,
		const NWN::CExoString& scriptName, NWN::OBJECTID objectId, uint32_t unknown1, uint32_t unknown2);
	CVirtualMachine_ExecuteScript_t CVirtualMachine_ExecuteScript =
		std::bit_cast<CVirtualMachine_ExecuteScript_t>(NWN2_OFFSET_EXECUTESCRIPT);

	using CVirtualMachine_ExecuteScriptEnhanced_t = int(__thiscall*)(CVirtualMachine* thisVM,
		const NWN::CExoString& scriptName, NWN::OBJECTID objectID, void* ParamList, uint32_t unknow1, uint32_t unknow2);
	CVirtualMachine_ExecuteScriptEnhanced_t CVirtualMachine_ExecuteScriptEnhanced =
		std::bit_cast<CVirtualMachine_ExecuteScriptEnhanced_t>(NWN2_OFFSET_EXECUTESCRIPT_ENH);

	using CVirtualMachine_InitParam_t = void(__thiscall*)(void* paramLst, uint32_t iNb);
	using CVirtualMachine_CleanParam_t = void(__thiscall*)(void* paramLst);

	CVirtualMachine_InitParam_t CVirtualMachine_InitParam =
		std::_Bit_cast<CVirtualMachine_InitParam_t>(NWN2_OFFSET_InitParam);
	CVirtualMachine_CleanParam_t CVirtualMachine_CleanParam =
		std::_Bit_cast<CVirtualMachine_CleanParam_t>(NWN2_OFFSET_CleanParam);

    CVirtualMachine*
    GetNwn2VirtualMachine()
    {
	    const auto vm = std::bit_cast<CVirtualMachine**>(NWN2_OFFSET_CVIRTUALMACHINE);
	    return *vm;
    }

	

int(__fastcall* OriginalMS)(void* pThis, void* _, int playerId, unsigned char* Data, int size);

DWORD 
FindHookMsgServ()
{
	// 83 EC 0C 8B 44 24 14 8A 50
	char* ptr = (char*)0x400000;
	while (ptr < (char*)0x800000) {
		if ((ptr[0] == (char)0x83) && (ptr[1] == (char)0xEC) && (ptr[2] == (char)0x0C) &&
			(ptr[3] == (char)0x8B) && (ptr[4] == (char)0x44) && (ptr[5] == (char)0x24) &&
			(ptr[6] == (char)0x14) && (ptr[7] == (char)0x8A) && (ptr[8] == (char)0x50) &&
			(ptr[9] == (char)0x02))
			return (DWORD)ptr;
		else
			ptr++;
	}
	return NULL;
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

//Set the text of BadPassword GUI part
void BadPassword(unsigned long iMustView, std::string sMessage)
{
	unsigned long iSizeText = sMessage.size();
	unsigned long iTotalSize = 0x2C + iSizeText + 4;

	unsigned char* Data = new unsigned char[iTotalSize+1];

	std::memcpy(Data, DataLoginBadBase, 44);
	std::memcpy ( Data+0x30, sMessage.c_str(), iSizeText);


	Data[3] = (iTotalSize & 0XFF);
	Data[4] = ((iTotalSize & 0xFF00) >> 8);
	Data[5] = ((iTotalSize & 0xFF0000) >> 16);
	Data[6] = ((iTotalSize & 0xFF000000) >> 24);


	Data[0x2C] = (iSizeText & 0XFF);
	Data[0x2D] = ((iSizeText & 0xFF00) >> 8);
	Data[0x2E] = ((iSizeText & 0xFF0000) >> 16);
	Data[0x2F] = ((iSizeText & 0xFF000000) >> 24);
	Data[iTotalSize] = 0x93;

	SendMessageToPlayer_(iMustView, (unsigned char*)Data, iTotalSize+1, 0);

	delete[] Data;
}

void OpenLoginGui(unsigned long iMustView)
{
	SendMessageToPlayer_(iMustView, (unsigned char*)DataLoginOpen, 0x37, 0);

	if (g_msgServ->bAllowAutoConnect)
	{
		SendMessageToPlayer_(iMustView, (unsigned char*)DataLoginShowRemember, 0x34, 0);
		SendMessageToPlayer_(iMustView, (unsigned char*)DataLoginShowRemBtn, 0x34, 0);
		//Change text latter
		SendMessageToPlayer_(iMustView, (unsigned char*)g_msgServ->DataLogSetRememberMeText, g_msgServ->LengthLogRememberMe, 0);
	}
	BadPassword(iMustView, ""); //Set current BadPassword Text to ""
}

void CloseLoginGui(unsigned long iMustView)
{
	std::string sName = GetPlayerAccountName_(iMustView);
	g_msgServ->_knowedPlayer[sName].m_BadPassword = false;
	SendMessageToPlayer_(iMustView, (unsigned char*)DataLoginClose, 0x22, 0);
}

//We don't want this player to be abble to communicate with the server.
//it will not dont change until unblock/restart
void MsgServBanPlayerName(std::string playerName)
{
	g_msgServ->_knowedPlayer[playerName].m_step = -2;
}

//Launch the gui, change the text (and hide the respawn button if we use the default panel)
bool OpenKickGui(unsigned long iMustView, std::string sText, bool bEnforced = true)
{
	//allow to unactive this verification 
	if (!g_msgServ->bEnforcedBlock && bEnforced)
	{
		return false;
	}

	std::string sName = GetPlayerAccountName_(iMustView);
	
	//Let it be blocked. Never allow anything now (until new connexion)
	//Just be sure to not "unban" someone by trying to kick
	if(g_msgServ->_knowedPlayer[sName].m_step != -2)
		g_msgServ->_knowedPlayer[sName].m_step = -1;

	//Open the panel
	SendMessageToPlayer_(iMustView, (unsigned char*)g_msgServ->DataKickOpen, g_msgServ->LengthKickPanelOpen, 0);


	//We need to load the text and add it at the end. (size of text on 4 bytes, text, then 0x93)
	//Then, change DataText[3] to set the real new size
	
	//MESSAGE_TEXT
	unsigned long iSizeText = sText.size();
	unsigned long iSize = 0x2F + 4 + iSizeText;

	unsigned char* DataKickText = new unsigned char[iSize+1];

	std::memcpy ( DataKickText, DataKickTextBase, 0x2F); //Start of update text msg
	std::memcpy ( DataKickText+0x2F+4, sText.c_str(), iSizeText); //Set up the text

	//setup the new message Size
	DataKickText[3] = (iSize & 0XFF);
	DataKickText[4] = ((iSize & 0xFF00) >> 8);
	DataKickText[5] = ((iSize & 0xFF0000) >> 16);
	DataKickText[6] = ((iSize & 0xFF000000) >> 24);

	//Setup the text size
	DataKickText[0x2F] = (iSizeText & 0XFF);
	DataKickText[0x30] = ((iSizeText & 0xFF00) >> 8);
	DataKickText[0x31] = ((iSizeText & 0xFF0000) >> 16);
	DataKickText[0x32] = ((iSizeText & 0xFF000000) >> 24);
	DataKickText[iSize] = 0x93;

	//DataText builded
	SendMessageToPlayer_(iMustView, (unsigned char*)DataKickText, iSize+1, 0);

	delete[] DataKickText;

	//BUTTON_RESPAWN hide <= Only if we use the default kickPanelValue
	if(g_msgServ->KickPanelXml == "")
		SendMessageToPlayer_(iMustView, (unsigned char*)DataKickHideBtn, 0x32, 0);

	return true;
}

void OpenPopUp(unsigned long iMustView, std::string sMsg, std::string sBtn)
{
	if(g_msgServ->PopUpPanelXml == "")
	{
		unsigned long iSize = 0x0B + sMsg.size() + 0x0C + 0x26 + 0x04 + sBtn.size() + 0x05;

		unsigned char* DataPopUp = new unsigned char[iSize];

		DataPopUp[0] = 0x50;
		DataPopUp[1] = 0x24;
		DataPopUp[2] = 0x05;

		iSize--;
		DataPopUp[3] = (iSize & 0xFF);
		DataPopUp[4] = ((iSize & 0xFF00) >> 8);
		DataPopUp[5] = ((iSize & 0xFF0000) >> 16);
		DataPopUp[6] = ((iSize & 0xFF000000) >> 24);

		DataPopUp[7] = (sMsg.size() & 0xFF);
		DataPopUp[8] = ((sMsg.size() & 0xFF00) >> 8);
		DataPopUp[9] = ((sMsg.size() & 0xFF0000) >> 16);
		DataPopUp[10] = ((sMsg.size() & 0xFF000000) >> 24);

		std::memcpy ( DataPopUp+11, sMsg.c_str(), sMsg.size());


		int curPosition = 0x0B + sMsg.size() + 0x0C ;
		//memcpy de 0x26
		std::memcpy ( DataPopUp+curPosition, DataPopUpCenter, 0x26);
		curPosition += 0x26;

		DataPopUp[curPosition] = (sBtn.size() & 0xFF);
		DataPopUp[curPosition+1] = ((sBtn.size() & 0xFF00) >> 8);
		DataPopUp[curPosition+2] = ((sBtn.size() & 0xFF0000) >> 16);
		DataPopUp[curPosition+3] = ((sBtn.size() & 0xFF000000) >> 24);

		std::memcpy ( DataPopUp+curPosition+4, sBtn.c_str(), sBtn.size());

		DataPopUp[iSize] = 0xC0;

		SendMessageToPlayer_(iMustView, (unsigned char*)DataPopUp, iSize+1, 0);

		delete[] DataPopUp;
	}
	else {
		//Open the panel
		SendMessageToPlayer_(iMustView, (unsigned char*)g_msgServ->DataPopUpOpen, g_msgServ->LengthPopUpPanelOpen, 0);

		//Add the text
		//We need to load the text and add it at the end. (size of text on 4 bytes, text, then 0x93)
		//Then, change DataText[3] to set the real new size

		//MESSAGE_TEXT
		unsigned long iSizeText = sMsg.size();
		unsigned long iSize = 0x30 + 4 + iSizeText;

		unsigned char* DataPopUpText = new unsigned char[iSize+1]();

		DataPopUpText[0] = 0x50;
		DataPopUpText[1] = 0x24;
		DataPopUpText[2] = 0x0A;

		DataPopUpText[3] = (iSize & 0XFF);
		DataPopUpText[4] = ((iSize & 0xFF00) >> 8);
		DataPopUpText[5] = ((iSize & 0xFF0000) >> 16);
		DataPopUpText[6] = ((iSize & 0xFF000000) >> 24);

		DataPopUpText[7] = 0x15;

		std::memcpy(DataPopUpText + 0xB, "SCREEN_SEPT_HEIMPOPUP", 0x15);

		DataPopUpText[0x20] = 0x0C;
		std::memcpy(DataPopUpText + 0x24, "MESSAGE_TEXT", 0xC);


		//Setup the text size
		DataPopUpText[0x30] = (iSizeText & 0XFF);
		DataPopUpText[0x31] = ((iSizeText & 0xFF00) >> 8);
		DataPopUpText[0x32] = ((iSizeText & 0xFF0000) >> 16);
		DataPopUpText[0x33] = ((iSizeText & 0xFF000000) >> 24);
		std::memcpy ( DataPopUpText+0x34, sMsg.c_str(), iSizeText); //Set up the text
		DataPopUpText[iSize] = 0x93;

		//DataText builded
		SendMessageToPlayer_(iMustView, (unsigned char*)DataPopUpText, iSize+1, 0);

		delete[] DataPopUpText;
	}
}

//Called for every received msg
int __fastcall MSHookProc(void* pThis, void* _, int playerId, unsigned char* Data, int size)
{
	try{
		std::string currentName_(GetPlayerAccountName_(playerId));
		unsigned long currentIP_ = 0;

		//Only if we want to trace "everything"
		if(logger->Level() == LogLevel::trace)
		{
			const void * address = static_cast<const void*>(Data);
			std::stringstream ss;
			ss << address;  
			std::string addr = ss.str();

			std::string logTxt =
				"MsgSrv("+ currentName_ + " send @:" + addr + ", size : " + std::to_string(size) + ", id : " + std::to_string(playerId) + ")";
			logger->Trace(logTxt.c_str());
		}

		//Only if we want to active loggin module.
		if (g_msgServ->bConnectionProcess)
		{
			{
				sockaddr_in sin;
				GetPlayerConnectionInfo_(playerId, &sin);
				currentIP_ = sin.sin_addr.s_addr;
			}

			//The only msg type i manage for now
			if (Data[0] == 0x70)
			{
				//First msg, reset the step !
				if (size == 3 && Data[1] == 0x01 && Data[2] == 0x00)
				{
					if(g_msgServ->_knowedPlayer.count(currentName_) > 0)
					{
						//Banned guy, don't let it came in and reset the step.
						if(g_msgServ->_knowedPlayer[currentName_].m_step == -2)
						{
							if(OpenKickGui(playerId, g_msgServ->ErrorMsg))
								return false;
						}
					}
					g_msgServ->_knowedPlayer[currentName_].m_step = 0;
					g_msgServ->_knowedPlayer[currentName_].m_BadPassword = false;
				}
				else
				{
					//Be sure that we know the player (and so, at least, use the reset step)
					if (g_msgServ->_knowedPlayer.count(currentName_) <= 0)
					{
						//Hep ! What are you doing here without even the first step ?!!!
						if (OpenKickGui(playerId, g_msgServ->ErrorMsg))
						{
							return false;
						} 
						//In case we don't have enforcedBlock activated, just init it 
						else {
							g_msgServ->_knowedPlayer[currentName_].m_step = 0; //make it existant
							g_msgServ->_knowedPlayer[currentName_].m_BadPassword = false;
						}

					}

					//This player name is set as "banned", don't accept anything from this account
					if (g_msgServ->_knowedPlayer[currentName_].m_step <= -1)
					{
						if(OpenKickGui(playerId, g_msgServ->ErrorMsg))
							return false;
					}
					
					//Not logged yet.
					if(g_msgServ->_knowedPlayer[currentName_].m_step < 4)
					{
						//just to filtering. Will be second step and ask list (prompt)
						if (size == 3)
						{
							//second connection step //0x70 0x02 0x16 (the player is still on the connexion popup here
							if (Data[1] == 0x02 && Data[2] == 0x16)
							{
								//Must be at the first step. Else... Not good !
								if (g_msgServ->_knowedPlayer[currentName_].m_step == 0)
								{
									g_msgServ->_knowedPlayer[currentName_].m_step = 1;
								}
								else
								{
									//What are you doing here ? Kick
									if(OpenKickGui(playerId, g_msgServ->ErrorMsg))
										return false;
								}
							}
							//Load char list. Can't be here without being already logged
							else if (Data[1] == 0x11 && Data[2] == 0x01)
							{
									if (g_msgServ->_knowedPlayer[currentName_].m_step != 4)
									{
										if(OpenKickGui(playerId, g_msgServ->ErrorMsg))
											return false;
									}
									//we are connected
							}
							//No expulse on script
							else if(Data[1] == 0x06 && Data[2] == 0x30 && g_msgServ->bEnforcedBlock)
							{
								return false;
							}
							//Don't bother with other messages. Probably not happens btw.
							//(or remove them )
							else
							{
								if(OpenKickGui(playerId, g_msgServ->ErrorMsg))
									return false;
							}
						}
						//Other messages
						else
						{
							//Third step of the connection. Last one.   {0x70 0x03 0x02 0x09 0x00 0x00 0x00 0x00 0x00 0x60}
							if (size == 10 && Data[1] == 0x03 && Data[2] == 0x02 && Data[3] == 0x09 && Data[4] == 0x00 &&
								Data[5] == 0x00 && Data[6] == 0x00 && Data[7] == 0x0 && Data[8] == 0x00)
							{
								if (g_msgServ->_knowedPlayer[currentName_].m_step == 1)
								{
										//First time we come here. Check if the player is in "auto connect" mode
										{
											g_msgServ->_knowedPlayer[currentName_].m_BadPassword = false;
											//Need to see if we need to auto-reconnect the player
											if(g_msgServ->ScriptStayConnectedScriptName != "")
											{
												std::string currentCdKey_ = GetCDKey(playerId);
												int currentPlayerPriv_ = GetPlayerPrivileges(playerId);
												int currentValidity_  = 0;
												//int isExecScriptOk = 0;

												bool isExecScriptOk = true;

												NWScript::ClearScriptParams();
												NWScript::AddScriptParameterString(currentName_.c_str());
												NWScript::AddScriptParameterInt((int32_t)currentIP_);
												NWScript::AddScriptParameterString(currentCdKey_.c_str());
												NWScript::AddScriptParameterInt(currentPlayerPriv_);
												currentValidity_ = NWScript::ExecuteScriptEnhanced(g_msgServ->ScriptStayConnectedScriptName.c_str(), 0, true, &isExecScriptOk);

												//isExecScriptOk = ExecuteEnhancedScript_sp(g_msgServ->ScriptStayConnectedScriptName, 0, currentValidity_, currentName_.c_str(), (int)currentIP_, currentCdKey_.c_str(), currentPlayerPriv_);

												if(isExecScriptOk == false || currentValidity_ == SCRIPTRESPONSE_ERROR)
												{
													//Error on script, log and continue as not autoconnected.
													std::string strLog = "Error on StayConnected Script (" +
														g_msgServ->ScriptStayConnectedScriptName + ") params : " + currentName_ +
														", " + std::to_string(currentIP_) + ", " + currentCdKey_ +
														", " + std::to_string(currentPlayerPriv_);

													logger->Err(strLog.c_str());
												}
												//Auto connect
												else if (currentValidity_ == SCRIPTRESPONSE_OK)
												{
													g_msgServ->_knowedPlayer[currentName_].m_step = 4;
												}
												//Block/kick/ban
												else if (currentValidity_ == SCRIPTRESPONSE_KICK || currentValidity_ == SCRIPTRESPONSE_BAN)
												{
													OpenKickGui(playerId, g_msgServ->curResponseString_, false);

													if(currentValidity_ == SCRIPTRESPONSE_BAN)
														MsgServBanPlayerName(currentName_);
													return false;
												}
											}
										}

									if(g_msgServ->_knowedPlayer[currentName_].m_step != 4)
									{
										g_msgServ->_knowedPlayer[currentName_].m_step = 3; //Start the log session
																					 //Send message to open GUI Here//

										OpenLoginGui(playerId);

										//End of send message..//
									}
									//Send welcome/warning pop up here ?
									if (!g_msgServ->_knowedPlayer[currentName_].m_Know)
									{
										if (g_msgServ->bWelcomeScreen)
										{
											// A new player ! Welcome ! Suscribe, yadayada !
											std::string sWelcomeTreated = std::regex_replace(g_msgServ->sWelcome, std::regex("%S"), currentName_);
											OpenPopUp(playerId, sWelcomeTreated, "Ok");
										}
									}
									else
									{
										//Welcome back ?
									}
									//g_msgServ->_knowedPlayer[currentName_].m_step = 2;
								}
								else
								{
									//What are you doing here ?
									if(OpenKickGui(playerId, g_msgServ->ErrorMsg))
										return false;
								}
							}
							//Connection verification ! Only logical on step 3 else.. what are you doing ?
							else if (Data[1] == 0x06 && Data[2] == 0x30 && Data[7] == 0x13 && Data[8] == 0x00 && Data[9] == 0x00 && Data[10] == 0x00 && Data[11] == 0x67 &&
								Data[12] == 0x75 && Data[13] == 0x69 && Data[14] == 0x5F && Data[15] == 0x73 && Data[16] == 0x65 && Data[17] == 0x70 && Data[18] == 0x74 && 
								Data[19] == 0x5F && Data[20] == 0x63 && Data[21] == 0x6F && Data[22] == 0x6E && Data[23] == 0x6E && Data[24] == 0x65 && Data[25] == 0x63 && 
								Data[26] == 0x74 && Data[27] == 0x69 && Data[28] == 0x6F && Data[29] == 0x6E
								&& g_msgServ->_knowedPlayer[currentName_].m_step == 3)
							{
								//We ask for connection
								if(Data[30] == 0x03)
								{

									std::string currentCdKey_(GetCDKey(playerId));
									int currentPlayerPriv_ = GetPlayerPrivileges(playerId);
									//Player Nickname currentName_;
									//Here, split the message and understand it
									uint32_t taille = *(uint32_t*)(Data + 31);
									std::string currentLog_ = "";
									std::string currentPwd_ = "";
									std::string currentOption_ = "";
									int currentValidity_  = 0;
									bool isExecScriptOk = false;
									for (int i = 0; i < taille; i++)
									{
										currentLog_ += Data[35 + i];
									}

									uint32_t taille2 = *(uint32_t*)(Data + taille + 35);
									for (int i = 0; i < taille2; i++)
									{
										currentPwd_ += Data[35 + taille + 4 + i];
									}

									if (g_msgServ->bAllowAutoConnect)
									{
										int iPos = 0x27 + taille + taille2;
										if (!(Data[iPos] == 1 && Data[iPos + 1] == 0 && Data[iPos + 2] == 0 && Data[iPos + 3] == 0))
										{
											//erreur !
											if (OpenKickGui(playerId, g_msgServ->ErrorMsg))
												return false;
										}
										else
										{
											iPos += 4;
											currentOption_ += Data[iPos];
										}
									}

									if (g_msgServ->ScriptConnectionName == "")
									{
										currentValidity_ = 1;
										currentLog_ = currentName_;
										{
											std::string strLog = "Enforced connection by script asked but no script given for it. Allow " + currentPlayerPriv_;
											strLog += ", " + std::to_string(currentIP_) + ", " + currentCdKey_;
											strLog += ", " + std::to_string(currentPlayerPriv_) + " to connect without any further verification.";

											logger->Warn(strLog.c_str());
										}
									}
									else
									{

										NWScript::ClearScriptParams();
										NWScript::AddScriptParameterString(currentName_.c_str());
										NWScript::AddScriptParameterInt((int32_t)currentIP_);
										NWScript::AddScriptParameterString(currentCdKey_.c_str());
										NWScript::AddScriptParameterInt(currentPlayerPriv_);
										NWScript::AddScriptParameterString(currentLog_.c_str());
										NWScript::AddScriptParameterString(currentPwd_.c_str());
										NWScript::AddScriptParameterString(currentOption_.c_str());
										currentValidity_ = NWScript::ExecuteScriptEnhanced(g_msgServ->ScriptConnectionName.c_str(), 0, true, &isExecScriptOk);
										

										//isExecScriptOk = ExecuteEnhancedScript_sp(g_msgServ->ScriptConnectionName, (NWN::OBJECTID)0,  currentValidity_, currentName_.c_str(), (int)currentIP_,
										//									currentCdKey_.c_str(), currentPlayerPriv_, currentLog_.c_str(), currentPwd_.c_str(), currentOption_.c_str());

										std::string strLog = "Return of the " + g_msgServ->ScriptConnectionName;
										strLog += " script : " + std::to_string(isExecScriptOk?1:0) + " = " + std::to_string(currentValidity_);

										logger->Debug(strLog.c_str());
									}


									//ERROR
									if(isExecScriptOk == false || currentValidity_ == SCRIPTRESPONSE_ERROR)
									{
										//Error on script, log and continue as not autoconnected.
										std::string strLog = "Error on Connection Script (";
										strLog += g_msgServ->ScriptConnectionName + ") params : " + currentName_;
										strLog += ", " + std::to_string(currentIP_) + ", " + currentCdKey_;
										strLog += ", " + std::to_string(currentPlayerPriv_);
										strLog += ", " + currentLog_ + ", (size of given pwd:)" + std::to_string(currentPwd_.length());
										strLog += ", " + currentOption_;

										logger->Err(strLog.c_str());

									}
									//OK !
									else if (currentValidity_ == SCRIPTRESPONSE_OK)
									{
										g_msgServ->_knowedPlayer[currentName_].m_step = 4;
										CloseLoginGui(playerId);
									}
									//NOP
									else if(currentValidity_ == SCRIPTRESPONSE_NOK)
									{
										BadPassword(playerId, g_msgServ->curResponseString_);
										g_msgServ->_knowedPlayer[currentName_].m_BadPassword = true;
										return false;
									}
									//Kick/block
									else
									{
										OpenKickGui(playerId, g_msgServ->curResponseString_, false);
										if(currentValidity_ == SCRIPTRESPONSE_BAN)
											MsgServBanPlayerName(currentName_);
										return false;
									}
								} 
								//We close the gui to go back
								else if (Data[30] == 0x00)
								{
									CloseLoginGui(playerId);
								}
							}
							//No expulse on script
							else if(Data[1] == 0x06 && Data[2] == 0x30 && g_msgServ->bEnforcedBlock)
							{
								return false;
							}
							//Always refuse to load PC if not logged
							else if( Data[1] == 0x02 )
							{
								OpenKickGui(playerId, g_msgServ->ErrorMsg, false);
								return false;
							}
							//What are you tring to do ?
							else {
								if(OpenKickGui(playerId, g_msgServ->ErrorMsg))
									return false;
							}
						}
					}
				}
			}

		}

		int i = 0;
		
		//Only if we want to active Anticheat module.
		//if (g_msgServ->bAntiCheat)
		{
			//EndOfLevelUp
			if (Data[0] == 0x70 && Data[1] == 0x1D && Data[2] == 0x00 && g_msgServ->bAnticheatLvlUp)
			{
				int iResLvlUp = CheckForLevelUp(playerId, Data, size, currentName_, g_msgServ);
				if (iResLvlUp <= 0)
					return false;
			}
			//End of CreateChar
			//else if()
			{
			//	CheckForValidCharacter(playerId, Data, size);
			}
		}
		

		//If you link with external stuff, call every external and drop as soon as one ask for drop msg
		if (g_msgServ->bAllowExternal)
		{
			for (auto& [key, value]: g_msgServ->_fctList) 
			{
				logger->Info("*Launching external function : %s", key);
				if (!value(playerId, Data, size))
				{
					logger->Info("Message dropped.");
					return false;
				}
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
		std::string logTxt2 = "Exception ! => " + std::to_string(size) + ", id : " + std::to_string(playerId) + ")";
		logger->Err(logTxt2.c_str());
	}
	return OriginalMS(pThis, _, playerId, Data, size);
}

int
HookMsgServ()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	int msgServ_success, detour_success;
	DWORD msgSer = FindHookMsgServ();
	if (msgSer) {
		logger->Info("o MsgServ located at %x.", msgSer);
		*(DWORD*)&OriginalMS = msgSer;
		msgServ_success           = DetourAttach(&(PVOID&)OriginalMS, MSHookProc) == 0;
	} else {
		logger->Info("! MsgServ locate failed.");
		return 0;
	}

	detour_success = DetourTransactionCommit() == 0;
	return msgServ_success && detour_success;
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
	                     "(c) 2024 by Septirage\n"
	                     "visit us at http://septirage.com/nwn2/ \n"
	                     "visit nwnx project at http://www.nwnx.org\n";

	logger = std::make_unique<LogNWNX>(logfile);
	//logger = new LogNWNX(logfile);
	logger->Info(header.c_str());

	 logger->Trace("* reading inifile %s", inifile.c_str());

	 config = new SimpleIniConfig(inifile);

	 logger->Configure(config);

	 int iEnforcedBlock = 0;
	 int i3CDKey = 0;
	 int iConnectionProcess = 0;
	 int iAllowExternal = 0;

	 config->Read("UseConnectionSystem", &iConnectionProcess, 1);
	 bConnectionProcess = (iConnectionProcess != 0);
	 config->Read("EnforcedSecurity", &iEnforcedBlock, 1);
	 bEnforcedBlock = (iEnforcedBlock != 0);
	 config->Read("RememberMeTxt", &RememberMeText, std::string(""));
	 config->Read("KickPanelXml", &KickPanelXml, std::string(""));
	 config->Read("WelcomePanelXml", &PopUpPanelXml, std::string(""));
	 config->Read("ConnectionValidScript", &ScriptConnectionName, std::string(""));
	 config->Read("AutoAuthenticationScript", &ScriptStayConnectedScriptName, std::string(""));
	 config->Read("ErrorMsg", &ErrorMsg, std::string(""));
	 config->Read("WelcomeMsg", &sWelcome, std::string("")); //Welcome on this demo %S !
	 //config->Read("DownloadScreen", &bDwnloadPanel, false);
	 config->Read("AllowExternalLibHook", &iAllowExternal, 0);

	 if (iAllowExternal != 0)
	 {
		 bAllowExternal = true;
		 logger->Debug("Plugins Hook Allowed.");
	 }
	 else {
		 logger->Debug("Plugins Hook Forbidden.");
	 }

	 config->Read("MCDKey", &i3CDKey, 0);
	 b3CDKey = (i3CDKey != 0);

	 if (bConnectionProcess) {
		 logger->Debug("UseConnectionSystem set to true.");
	 }
	 else {
		 logger->Debug("UseConnectionSystem set to false.");
	 }

	 if (bEnforcedBlock) {
		 logger->Debug("EnforcedSecurity active.");
	 }
	 else {
		 logger->Debug("EnforcedSecurity unactive.");
	 }

	 if (RememberMeText != "") {
		 logger->Debug("Remember Me text : \" %s \"", RememberMeText.c_str());
	 }

	 if(KickPanelXml != "") {
		logger->Debug("KickPanelXML :  \" %s \"", KickPanelXml.c_str());
	 } else {
		 logger->Debug("Use default KickPanel : \" death_default.xml \"");
	 }

	 if (PopUpPanelXml != "") {
		 logger->Debug("PopUpPanelXml :  \" %s \"", PopUpPanelXml.c_str());
	 } else {
		 logger->Debug("Use default PopUpPanelXml");
	 }

	 if (ScriptStayConnectedScriptName != "") {
		 logger->Debug("AutoAuthentication active active with script %s", ScriptStayConnectedScriptName.c_str());
	 }
	 else {
		 logger->Debug("AutoAuthentication un-available (no script passed)");
	 }

	 if (sWelcome != "") {
		 logger->Debug("Welcome message set to : %s", sWelcome.c_str());
		 bWelcomeScreen = true;

		 sWelcome = std::regex_replace(sWelcome, std::regex("%n"), "\n");
		 sWelcome = std::regex_replace(sWelcome, std::regex("%N"), "\n");
	 }
	 else {
		 logger->Debug("No Welcome Popup");
	 }

	 if (ScriptConnectionName != "") {
		 logger->Debug("Connection script set to : %s", ScriptConnectionName.c_str());
	 }
	 else {
		 logger->Debug("No connection script");
	 }

	 logger->Debug("Error Msg set to %s", ErrorMsg.c_str());
	 
	//Prepare Panel
	{
		 //Prepare KickPanel
		{
			std::string  tempPanel = "";

			if (KickPanelXml != "")
			{
				tempPanel = KickPanelXml;
			}
			else
			{
				tempPanel = "death_default.xml";
			}

			unsigned long iSizeXmlName = tempPanel.size();
			LengthKickPanelOpen = 0x1F + 4 + iSizeXmlName;

			DataKickOpen = new unsigned char[LengthKickPanelOpen + 1];

			std::memcpy(DataKickOpen, DataKickOpenStart, 0x1F);
			std::memcpy(DataKickOpen + 0x23, tempPanel.c_str(), iSizeXmlName);

			DataKickOpen[3] = (LengthKickPanelOpen & 0XFF);
			DataKickOpen[4] = ((LengthKickPanelOpen & 0xFF00) >> 8);
			DataKickOpen[5] = ((LengthKickPanelOpen & 0xFF0000) >> 16);
			DataKickOpen[6] = ((LengthKickPanelOpen & 0xFF000000) >> 24);

			DataKickOpen[0x1F] = (iSizeXmlName & 0XFF);
			DataKickOpen[0x20] = ((iSizeXmlName & 0xFF00) >> 8);
			DataKickOpen[0x21] = ((iSizeXmlName & 0xFF0000) >> 16);
			DataKickOpen[0x22] = ((iSizeXmlName & 0xFF000000) >> 24);


			DataKickOpen[LengthKickPanelOpen] = 0xB3;
			LengthKickPanelOpen++;
		}

		 //PreparePopUpPanel
		 if (PopUpPanelXml != "")
		 {
			 unsigned long iSizeXmlName = PopUpPanelXml.size();
			 LengthPopUpPanelOpen = 0x20 + 4 + iSizeXmlName;

			 DataPopUpOpen = new unsigned char[LengthPopUpPanelOpen + 1];

			 std::memcpy(DataPopUpOpen, DataKickOpenStart, 0xB);
			 std::memcpy(DataPopUpOpen + 0xB, "SCREEN_SEPT_HEIMPOPUP", 0x15);
			 std::memcpy(DataPopUpOpen + 0x24, PopUpPanelXml.c_str(), iSizeXmlName);

			 DataPopUpOpen[3] = (LengthPopUpPanelOpen & 0XFF);
			 DataPopUpOpen[4] = ((LengthPopUpPanelOpen & 0xFF00) >> 8);
			 DataPopUpOpen[5] = ((LengthPopUpPanelOpen & 0xFF0000) >> 16);
			 DataPopUpOpen[6] = ((LengthPopUpPanelOpen & 0xFF000000) >> 24);
			 DataPopUpOpen[7] = 0x15;

			 DataPopUpOpen[0x20] = (iSizeXmlName & 0XFF);
			 DataPopUpOpen[0x21] = ((iSizeXmlName & 0xFF00) >> 8);
			 DataPopUpOpen[0x22] = ((iSizeXmlName & 0xFF0000) >> 16);
			 DataPopUpOpen[0x23] = ((iSizeXmlName & 0xFF000000) >> 24);

			 DataPopUpOpen[LengthPopUpPanelOpen] = 0xB3;
			 LengthPopUpPanelOpen++;
		 }
	}

	 if (RememberMeText != "")
	 {
		 bAllowAutoConnect = true;

		 unsigned long iSizeText = RememberMeText.size();
		 LengthLogRememberMe = 0x33 + 4 + iSizeText;

		 DataLogSetRememberMeText = new unsigned char[LengthLogRememberMe+1];

		 std::memcpy ( DataLogSetRememberMeText, DataLoginRememberTxt, 0x33);
		 std::memcpy ( DataLogSetRememberMeText+0x37, RememberMeText.c_str(), iSizeText);


		 DataLogSetRememberMeText[3] = (LengthLogRememberMe & 0XFF);
		 DataLogSetRememberMeText[4] = ((LengthLogRememberMe & 0xFF00) >> 8);
		 DataLogSetRememberMeText[5] = ((LengthLogRememberMe & 0xFF0000) >> 16);
		 DataLogSetRememberMeText[6] = ((LengthLogRememberMe & 0xFF000000) >> 24);

		 DataLogSetRememberMeText[0x33] = (iSizeText & 0XFF);
		 DataLogSetRememberMeText[0x34] = ((iSizeText & 0xFF00) >> 8);
		 DataLogSetRememberMeText[0x35] = ((iSizeText & 0xFF0000) >> 16);
		 DataLogSetRememberMeText[0x36] = ((iSizeText & 0xFF000000) >> 24);

		 DataLogSetRememberMeText[LengthLogRememberMe] = 0x93;
		 LengthLogRememberMe++;
		 

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

		 //AntiCheatStopFirstViolation
		 //ACLvlUpStopFirstViolation

		 config->Read("UseAnticheatLvlUp", &iAnticheatLvlUp, 1);
		 bAnticheatLvlUp = (iAnticheatLvlUp != 0);
		 config->Read("StopLvlUpFirstViolation", &iACLvlUpStopFirstViolation, 1);
		 bACLvlUpStopFirstViolation = (iACLvlUpStopFirstViolation != 0);

		 config->Read("GrantedCondForEveryFeats", &iGrantedCondForEveryFeats, 0);
		 bGrantedCondForEveryFeats = (iGrantedCondForEveryFeats != 0);


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
		if (HookMsgServ())
			logger->Info("* Hooking successful");
		else
		{
			logger->Info("* Hooking failed");
			bIsLoadOk = false;
		}
	}

	if (bIsLoadOk)
	{
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
	};
	XPBugfix = LoadLibraryA("xp_bugfix.dll");

	if (!XPBugfix)
	{
		logger->Info("* Failed to load xp_bugfix.dll");
		return false;
	}

	//ProcessQueryFunction
	// Search for V2 plugin function
	bool bMyBugFixIsOk = false;
	void* pGetPluginPointer = GetProcAddress(XPBugfix, "GetPluginPointerV2");
	if (pGetPluginPointer != nullptr) {
		// Load
		typedef Plugin*(WINAPI * GetPluginPointer)();

		Plugin* pPlugin = ((GetPluginPointer)pGetPluginPointer)();

		if (pPlugin != nullptr) {
			std::string sVersion = pPlugin->ProcessQueryFunction("GET VERSION");
			int iComp = version_compare(sVersion, "1.0.74");
			if (iComp >= 0)
			{
				bMyBugFixIsOk = true;
			}
		}
	}bMyBugFixIsOk = true; //TODO, why the version number dont work ?

	if (!bMyBugFixIsOk)
	{
		logger->Info("* Bad Version of xp_bugfix.dll. You need, at least, version 1.0.74");
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


int
MsgServ::GetInt(char* sFunction, [[maybe_unused]] char* sParam1, int nParam2)
{
	std::string function{sFunction};
	std::string logTxt =
		"MsgServ_GetInt(" + function + "," + sParam1 + "," + std::to_string(nParam2) + ")";

	logger->Trace(logTxt.c_str());

	if (function == "EnforcedSecurity") {
		logTxt = "Return EnforcedSecurity : " + std::to_string(bEnforcedBlock);
		logger->Trace(logTxt.c_str());
		return (bEnforcedBlock ? 1 : 0);
	}

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
	else if (function == "EnforcedSecurity")
	{
		bEnforcedBlock = (nValue != 0);
		logTxt = "Set EnforcedSecurity to : ";
		if (bEnforcedBlock)
			logTxt += "TRUE";
		else
			logTxt += "FALSE";
		logger->Trace(logTxt.c_str());
	}
	else if (function == "AnticheatCreation")
	{
		ChangeCharacterCreationStatus((nValue != 0));
		logTxt = "Set AnticheatCreation System to : ";
		if (bEnforcedBlock)
			logTxt += "TRUE";
		else
			logTxt += "FALSE";
		logger->Trace(logTxt.c_str());
	}
	else if (function == "AnticheatLvlUp")
	{
		bAnticheatLvlUp = (nValue != 0);
		logTxt = "Set AnticheatLevelup System to : ";
		if (bEnforcedBlock)
			logTxt += "TRUE";
		else
			logTxt += "FALSE";
		logger->Trace(logTxt.c_str());
	}
	else if (function == "Unblock")
	{
		std::string sPlayer(sParam1);
		_knowedPlayer[sPlayer].m_step = 0;
	}
	else if (function == "Knowed")
	{
		std::string sPlayer(sParam1);
		_knowedPlayer[sPlayer].m_Know = (nValue != 0);
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
	std::string logTxt = "MsgServ_GetString(" + function + "," + sParam1 + "," +
		std::to_string(nParam2) + ")";

	return (ret.data());
}


void
MsgServ::SetString([[maybe_unused]] char* sFunction,
	[[maybe_unused]] char* sParam1,
	[[maybe_unused]] int nParam2,
	[[maybe_unused]] char* sValue)
{
	std::string function{sFunction};

	std::string logTxt = "MsgServ_SetString(" + function + "," + sParam1 + "," +
		std::to_string(nParam2) + "," + sValue + ")";
	logger->Trace(logTxt.c_str());

	if (function == "Response")
	{
		std::string str(sValue);
		curResponseString_ = str;

		logTxt      = "Set Response to : " + str;
		logger->Trace(logTxt.c_str());
	}

	return;
}


void
MsgServ::GetFunctionClass(char* fClass)
{
	static constexpr auto cls = std::string_view(FunctionClass);
	strncpy_s(fClass, 128, cls.data(), std::size(cls));
}


void RemoveFromMsgServHook(std::string sDllName, std::string sFunctionName)
{
	if (g_msgServ->bAllowExternal)
	{
		std::string sName = sDllName+"!"+sFunctionName;
		g_msgServ->_fctList.erase(sName);
	}
}


bool AddToMsgServHook(std::string sDllName, std::string sFunctionName)
{

	std::string sNameAdded = sDllName + "!" + sFunctionName;
	if (!g_msgServ->bAllowExternal)
	{	
		logger->Info("External Hook not allowed. (Asked : [%s])", sNameAdded);
		return false;
	}

	MySharedHookFunction myNewFunction;
	logger->Info("* New Hook (%s) ...", sNameAdded);
	struct { bool Required; std::string Name; void **Import; } NewDllImports =
	{ true , sFunctionName,          (void**)&myNewFunction };

	HMODULE NewPlugin = LoadLibraryA(sDllName.c_str());
	if (!NewPlugin)
	{
		logger->Info("* Failed to load %s", sDllName);
		return false;
	}

	*NewDllImports.Import = (void *)GetProcAddress(NewPlugin, NewDllImports.Name.c_str());

	if (!*NewDllImports.Import)
	{
		if (!NewDllImports.Required)
		{
			logger->Info(
				"* Warning: missing entrypoint %s",
				sNameAdded);
		}
		logger->Info("* Unable to resolve %s",sNameAdded);
		return false;
	}
	logger->Info("* ...%s access Loaded", sNameAdded);

	g_msgServ->_fctList[sNameAdded] = myNewFunction;
	return true;
}

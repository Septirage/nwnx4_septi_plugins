#include <misc/log.h>
#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include <misc/Patch.h>
#include <misc/ini.h>
#include <hook/scriptManagement.h>

#include "../../septutil/NwN2DataPos.h"
#include "../../septutil/NwN2Utilities.h"
#include "../../septutil/intern2daMgt.h"

#include <nwn2heap.h>

#include <cstdint>
#include <detours/detours.h>



//TODO.. 

//  MsgChat_1(void *param_1_00,undefined *playerID,uint param_3,char **param_4)

#define FCT_MSGCHAT_1	0x563550
#define FCT_MSGCHAT_2	0x5638c0
#define FCT_MSGCHAT_3	0x563c20
#define FCT_MSGCHAT_4	0x563740
#define FCT_MSGCHAT_6	0x563440


float* const pTalkDistance = (float*)0x00802fa4;
float* const pWhisperDistance = (float*)0x00802fa8;

float* const pTalkDistanceSquare = (float*)0x0083ff6c;
float* const pWhisperDistanceSquare = (float*)0x0083ff70;



__declspec(naked) void __fastcall SendMessageChat1_(__in int* ptrCNSWMessage, __in void* Unused, __in uint32_t playerIDReceiver, __in uint32_t oSender, __in NWN::CExoString* myText)
{
	__asm
	{
		mov             edx, FCT_MSGCHAT_1;
		jmp             edx;
	}
}

__declspec(naked) void __fastcall SendMessageChat2_(__in int* ptrCNSWMessage, __in void* Unused, __in uint32_t playerIDReceiver, __in uint32_t oSender, __in NWN::CExoString* myText)
{
	__asm
	{
		mov             edx, FCT_MSGCHAT_2;
		jmp             edx;
	}
}

__declspec(naked) void __fastcall SendMessageChat3_(__in int* ptrCNSWMessage, __in void* Unused, __in uint32_t playerIDReceiver, __in uint32_t oSender, __in NWN::CExoString* myText)
{
	__asm
	{
		mov             edx, FCT_MSGCHAT_3;
		jmp             edx;
	}
}

__declspec(naked) void __fastcall SendMessageChat4_(__in int* ptrCNSWMessage, __in void* Unused, __in uint32_t playerIDReceiver, __in uint32_t oSender, __in NWN::CExoString* myText)
{
	__asm
	{
		mov             edx, FCT_MSGCHAT_4;
		jmp             edx;
	}
}

__declspec(naked) void __fastcall SendMessageChat6_(__in int* ptrCNSWMessage, __in void* Unused, __in uint32_t playerIDReceiver, __in uint32_t oSender, __in NWN::CExoString* myText)
{
	__asm
	{
		mov             edx, FCT_MSGCHAT_6;
		jmp             edx;
	}
}


void SendSpecificChatMessage(uint32_t oSender, uint32_t oReceiver, std::string Message, int iType)
{
	uint32_t receiverPCID = GetPCIDFromCreature(oReceiver);
	if (receiverPCID == NWN::PLAYERID_INVALIDID)
		return;

	int* ptrMsg = GetCNWSMessage();

	NWN::CExoString esMessage = {.m_sString       = Message.data(),
		.m_nBufferLength = std::size((Message)) + 1};

	if (iType == 1)
	{
		SendMessageChat1_(ptrMsg, NULL, receiverPCID, oSender, &esMessage);
	}
	else if (iType == 2)
	{
		SendMessageChat2_(ptrMsg, NULL, receiverPCID, oSender, &esMessage);
	}
	else if (iType == 3)
	{
		SendMessageChat3_(ptrMsg, NULL, receiverPCID, oSender, &esMessage);
	}
	else if (iType == 4)
	{
		SendMessageChat4_(ptrMsg, NULL, receiverPCID, oSender, &esMessage);
	}
	else if (iType == 6)
	{
		SendMessageChat6_(ptrMsg, NULL, receiverPCID, oSender, &esMessage);
	}
}

bool UnPackTalkSpecific(const std::string& input, uint32_t& hex1, uint32_t& hex2, std::string& remainingText)
{
	size_t firstPipe = input.find('|');
	if (firstPipe == std::string::npos)
		return false;

	size_t secondPipe = input.find('|', firstPipe + 1);
	if (secondPipe == std::string::npos)
		return false;

	const char* str = input.c_str();
	char* end;

	hex1 = strtoul(str, &end, 16);
	if (end != str + firstPipe)  // check if everything is consumed
		return false;

	hex2 = strtoul(str + firstPipe + 1, &end, 16);
	if (end != str + secondPipe)
		return false;

	remainingText = input.substr(secondPipe + 1);

	return true;
}


float GetChatDistance(int nTalkVolume)
{
	if (nTalkVolume == 0)
	{
		return *pTalkDistance;
	}
	else if (nTalkVolume == 1) 
	{
		return *pWhisperDistance;
	}

	return 0.0f;
}

void SetChatDistance(int nTalkVolume, float fValue)
{
	if (nTalkVolume == 0)
	{
		*pTalkDistance = fValue;
		*pTalkDistanceSquare = fValue * fValue;
	}
	else if (nTalkVolume == 1)
	{
		*pWhisperDistance = fValue;
		*pWhisperDistanceSquare = fValue * fValue;
	}
	return;
}

float GetFloatTalkFunction(char* cCommand, int nParam2)
{
	std::string sCommand(cCommand);

	if (sCommand == "ChatDistance")
	{
		return GetChatDistance(nParam2);
	}

	return 0.0f;
}

void SetFloatTalkFunction(char* cCommand, int nParam2, float fValue)
{
	std::string sCommand(cCommand);

	if (sCommand == "ChatDistance")
	{
		return SetChatDistance(nParam2, fValue);
	}

	return;
}


void SetStringTalkFunction(char* cCommand, int nParam2, char* cValue)
{
	std::string sCommand(cCommand);
	
	if (sCommand == "SendSpecific")
	{
		std::string sValue(cValue);
		std::string sTextToSay;
		uint32_t oSender, oReceiver;
		//Packed like that : ObjectToString(oSender)+|+ObjectToString(oReceiver)+|+msg
		//Unpack
		if (!UnPackTalkSpecific(sValue, oSender, oReceiver, sTextToSay))
		{
			return; //TODO Error?
		}
		SendSpecificChatMessage(oSender, oReceiver, sTextToSay, nParam2);
	}
}

void InitTalkFunctions()
{
	DWORD oldProtect;
	VirtualProtect(pTalkDistance, 8, PAGE_READWRITE, &oldProtect);
	VirtualProtect(pTalkDistanceSquare, 8, PAGE_READWRITE, &oldProtect);
}
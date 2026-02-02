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
#include <unordered_map>
#include <detours/detours.h>


std::unordered_map<uint32_t, std::tuple<byte, byte>> areaHour;

#define OFFS_WillSendTime 0x00613b9c
#define FCT_SendTime 0x0055b0d0
unsigned long ReturnPrepareSendTime		   = 0x00613ba1;

__declspec(naked) void __fastcall SendTime_(__in int* ptrCNSWMessage, __in void* Unused, __in uint32_t* playerIDFalseStruct, __in byte bHasChanged, 
	__in uint32_t bYear, __in byte month, __in byte day, __in byte hour, __in byte periodOfDay, uint32_t delayDuskOrDawn)
{
	__asm
	{
		mov             edx, FCT_SendTime;
		jmp             edx;
	}
}


void __fastcall CheckAndCallSendTime(__in int* ptrCNSWMessage, __in void* Unused, __in CNWSPlayerStruct* playerStruct, __in byte bHasChanged, 
	__in uint32_t bYear, __in byte month, __in byte day, __in byte hour, __in byte periodOfDay, uint32_t delayDuskOrDawn)
{

	if (playerStruct != NULL)
	{
		if (playerStruct->pCurrentAreaId != NULL)
		{
			NWN::OBJECTID areaID = *(playerStruct->pCurrentAreaId);

			if (areaHour.count(areaID) != 0)
			{
				hour = std::get<0>(areaHour[areaID]);
				periodOfDay = std::get<1>(areaHour[areaID]);
				delayDuskOrDawn = 0;
				bHasChanged |= 0x3;
			}
		}
	}
	SendTime_(ptrCNSWMessage, Unused, (uint32_t*)playerStruct, bHasChanged, bYear, month, day, hour, periodOfDay, delayDuskOrDawn);
}

__declspec(naked) void PrepareSendTimeCalculation()
{
	__asm
	{
		CALL	CheckAndCallSendTime
		JMP		dword ptr[ReturnPrepareSendTime]
	}
}

Patch _OnWillSendTime[] =
{
	Patch(OFFS_WillSendTime, (char*)"\xe9\x00\x00\x00\x00", (int)5),
	Patch(OFFS_WillSendTime + 1, (relativefunc)PrepareSendTimeCalculation),

	Patch()
};

Patch *OnWillSendTime = _OnWillSendTime;



bool PrepareTimePeriod(int iHour, byte& bHour, byte& bPeriod)
{
	NWN::OBJECTID moduleID = GetModuleID();
	NWN::CGameObject * Object;
	GameObjectManager m_ObjectManager;

	Object = m_ObjectManager.GetGameObject(moduleID);
	if (Object == NULL)
		return false;

	uint8_t bDawn = ((uint8_t*)Object)[0x2ec-0x30];
	uint8_t bDusk = ((uint8_t*)Object)[0x2ed-0x30];
	bHour = (byte)(iHour & 0xFF);
	bPeriod = 1;

	if (bDawn == bDusk)
		bPeriod = 1;
	else if (bHour == bDawn)
		bPeriod = 3;
	else if (bHour == bDusk)
		bPeriod = 4;
	else if (bDawn < bDusk)
	{
		if (bHour < bDawn || bDusk < bHour)
			bPeriod = 2;
		else
			bPeriod = 1;
	}
	else
	{
		if (bHour < bDusk || bDawn < bHour)
			bPeriod = 1;
		else
			bPeriod = 2;
	}

	return true;
}

void SetHourForPlayer(int nTarget, int iHour)
{
	uint32_t uPlayerID = GetPCIDFromCreature((uint32_t)nTarget);
	if (uPlayerID == NWN::PLAYERID_INVALIDID)
		return;

	byte bHour,bPeriod;

	if (!PrepareTimePeriod(iHour, bHour, bPeriod))
		return;

	int* ptrMsg = GetCNWSMessage();

	uint32_t falsePlayerStruct[2];
	falsePlayerStruct[1] = uPlayerID;



	SendTime_(ptrMsg, NULL, falsePlayerStruct, 0x03, 0x0, 0x0, 0x1, bHour, bPeriod, 0x0);
}


void SetHourForArea(int nArea, int iHour)
{
	if (iHour == -1)
	{
		areaHour.erase(nArea);
		return;
	}

	byte bHour,bPeriod;

	if (!PrepareTimePeriod(iHour, bHour, bPeriod))
		return;


	areaHour[nArea] = std::make_tuple(bHour, bPeriod);
}

int GetHourForArea(int nArea)
{
	if (areaHour.count(nArea) != 0)
	{
		return std::get<0>(areaHour[nArea]);
	}

	return -1;
}



void SetIntTimeFunction(char* cCommand, int nParam2, int iValue)
{
	std::string sCommand(cCommand);

	if (sCommand == "SetHourForPlayer")
	{
		SetHourForPlayer(nParam2, iValue);
	}
	else if (sCommand == "SetHourForArea")
	{
		SetHourForArea(nParam2, iValue);
	}

}

int GetIntTimeFunction(char* cCommand, int nParam2)
{
	std::string sCommand(cCommand);

	if (sCommand == "GetHourForArea")
	{
		return GetHourForArea(nParam2);
	}

	return -1;
}

void InitTimeFeatures()
{
	int i = 0;
	while (OnWillSendTime[i].Apply()) {
		i++;
	}
}

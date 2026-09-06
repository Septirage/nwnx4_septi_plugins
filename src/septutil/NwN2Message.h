#pragma once

#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include <cstdint>

// Low level primitives to build/finalize a raw CNWSMessage block, shared by
// any plugin code that needs to send a custom server->client message.

void AddX8ToMsg(uint8_t uData, unsigned char* msgBlock);
void AddX32ToMsg(uint32_t uData, unsigned char* msgBlock);
void AddCExoString(NWN::CExoString* Value, void* MessageObject);

typedef void (__thiscall *InitMessageBlock_t)(void* MsgCreator, uint32_t uSize, uint32_t param1, uint32_t param2);
extern InitMessageBlock_t const InitMessageBlock;

typedef void (__thiscall *PrepareEndOfMsg_t)(void* pMsgCreator, void* pData, void* pSize);
extern PrepareEndOfMsg_t const PrepareEndOfMsg;

typedef void (__thiscall *AddLocalizedNameToMsg_t)(void* MsgCreator, void* localizedName, int iVal);
extern AddLocalizedNameToMsg_t const AddLocalizedNameToMsg;

#include "NwN2Message.h"

#define FUNC_ADDLOCALIZEDNAMETOMSG	0x5b7010
#define FUNC_INITMSGBLOCK			0x74ed00
#define FUNC_PREPAREENDOFMSG		0x0074e080

void AddX8ToMsg(uint8_t uData, unsigned char* msgBlock)
{
	uint32_t* accessMBlock = (uint32_t*)msgBlock;
	uint8_t* msgData       = (uint8_t*)(accessMBlock[1]);
	msgData[accessMBlock[3]] = uData;
	accessMBlock[3]++;
	accessMBlock[7]++;
}

void AddX32ToMsg(uint32_t uData, unsigned char* msgBlock)
{
	uint32_t* accessMBlock = (uint32_t*)msgBlock;
	uint8_t* msgData = (uint8_t*)(accessMBlock[1]);
	*(uint32_t*)(msgData + accessMBlock[3]) = uData;
	accessMBlock[3] += 4;
	accessMBlock[7] += 4;
}

// CNWSMessage::WriteCExoString(CExoString* Value, int bitLength), __thiscall.
typedef void (__thiscall *WriteCExoString_t)(void* pThis, NWN::CExoString* Value, int bitLength);
static WriteCExoString_t const WriteCExoString_ = (WriteCExoString_t)OFFS_CNWSMessage_WriteCExoString;

void AddCExoString(NWN::CExoString* Value, void* MessageObject)
{
	WriteCExoString_(MessageObject, Value, 0x20);
}

InitMessageBlock_t const InitMessageBlock = (InitMessageBlock_t)FUNC_INITMSGBLOCK;
PrepareEndOfMsg_t const PrepareEndOfMsg = (PrepareEndOfMsg_t)FUNC_PREPAREENDOFMSG;
AddLocalizedNameToMsg_t const AddLocalizedNameToMsg = (AddLocalizedNameToMsg_t)FUNC_ADDLOCALIZEDNAMETOMSG;

#if !defined(NWN2_INTERN2DA_H)
#define NWN2_INTERN2DA_H


#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include <cstdint>





struct CNWSPlayerStruct
{
	void* vftable;
	uint32_t playerID;

	uint32_t unKnown1;
	char* pUnknown2;
	char* pUnknown3;

	NWN::OBJECTID* pCurrentAreaId; //Can be null ! 

	char unknown4[0x1C];

	NWN::OBJECTID possessedCreature;
	uint64_t timer;
	NWN::OBJECTID controlledCreature; //INVALIDOBJECT during transition
	NWN::OBJECTID ownedCreature;

	uint32_t unknown5[0x1E];

	NWN::OBJECTID targetedObject;

	//More
};

struct elementArray
{
	elementArray* previous;
	elementArray* next;
	CNWSPlayerStruct* ptrPlayerStruct;
};

uint32_t GetPCIDFromCreature(uint32_t oCreature);
int* GetCNWSMessage();
NWN::OBJECTID GetModuleID();


#endif

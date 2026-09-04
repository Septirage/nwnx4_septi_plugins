#if !defined(NWN2_INTERN2DA_H)
#define NWN2_INTERN2DA_H


#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include <cstdint>


struct CNWSPlStrContainer
{
	uint32_t bOpened;
	NWN::OBJECTID currentOpened;
};

struct CNWSPlayerStruct64
{
	uint32_t unknow1;
	NWN::OBJECTID currentCreature;
	uint32_t unknow2;
	uint32_t unknow3;
	uint32_t unknow4; //0x10
	uint32_t unknown5; 
	CNWSPlStrContainer* containerOpenned;
};


struct CNWSPlayerStruct
{
	void** vftable;
	uint32_t playerID;	//0x4

	uint32_t unKnown1;	//0x8
	char* pTable;		//0xC
	char* pUnknown3;	

	NWN::OBJECTID* pCurrentAreaId; //Can be null !	//0x14

	char unknown4[0x1C];	//0x18

	NWN::OBJECTID possessedCreature;		//0x34
	uint64_t timer;							//0x38
	NWN::OBJECTID controlledCreature; //INVALIDOBJECT during transition		//0x40
	NWN::OBJECTID ownedCreature;											//0x44

	uint32_t unknown5[0x7];												//0x48

	CNWSPlayerStruct64* containerStruct;

	uint32_t unknown6[0x16];

	NWN::OBJECTID targetedObject;											//0xC0
};

struct elementArray
{
	elementArray* previous;
	elementArray* next;
	CNWSPlayerStruct* ptrPlayerStruct;
};


elementArray* getBasePCBlockList();

template<typename Func>
void forEachPCBlock(Func callback) {
	elementArray* current = getBasePCBlockList();
	while (current != nullptr) {
		if (current->ptrPlayerStruct == nullptr) return;
		if (!callback(current->ptrPlayerStruct)) return;
		current = current->next;
	}
}

uint32_t GetPCIDFromCreature(uint32_t oCreature);
CNWSPlayerStruct* GetPCBlockFromCreature(uint32_t oCreature);
int* GetCNWSMessage();
NWN::OBJECTID GetModuleID();


#endif

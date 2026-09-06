#if !defined(NWN2_INTERN2DA_H)
#define NWN2_INTERN2DA_H


#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include <cstdint>

#define NWN2_MAX_PLAYERS               0x60



struct CPlayerCDKeyInfo
{
	NWN::CExoString m_Key;
	NWN::CExoString m_ValidCode;
	NWN::CExoString m_NotUsed;
};

struct CNetBigPlayerInfo // sizeof = 0x78, CNetLayerPlayerInfo
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
	CNetBigPlayerInfo    Players[NWN2_MAX_PLAYERS];      // 37690
	// CExoNetExtendableBuffer FrameStorage; // 3A390
};


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
CNetBigPlayerInfo* GetCNetPlayerInfo(uint8_t idPlayer);
int* GetCNWSMessage();
NWN::OBJECTID GetModuleID();


#endif

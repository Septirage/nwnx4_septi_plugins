#if !defined(SPT_EFF_ENHANCEDFEAT_STORERETPATCH)
#define SPT_EFF_ENHANCEDFEAT_STORERETPATCH


#include <memory>

#ifdef WIN32
#include <windows.h>
#endif

#include "EnhancedFeatures.h"
#include "..\..\misc\Patch.h"
#include "../../misc/ini.h"
#include "../../misc/log.h"
#include "../plugin.h"
#include "../../NWN2Lib/NWN2.h"
#include "../../NWN2Lib/NWN2Common.h"

//Store
#define OFFS_StoreObjectEnhanced	0x006b29ae
#define OFFS_TestOfStoreObject		0x006b2829
#define OFFS_ItemIdInPlaceableStore 0x0064cbc3
//#define OFFS_StoreObjectLocalVar	0x006b2a12

//006b2971 just after creature MOV EDX, dword ptr[ESi]; MOV dword [esp+0x10], eax
#define OFFS_StoreCreatureParam		0x006b2951
#define OFFS_StoreCreature			0x005fa993

//Store Specific Functions
#define SaveItemInfo				0x005d7b20
#define SavePlaceableInfo			0x0064c3f0
#define SaveLightInfo				0x006d1530
#define SaveWayPointInfo			0x006d0650
#define SaveTriggerInfo				0x006ca8e0
#define SavePlacedEffectInfo		0x006d2be0
#define SaveDoorInfo				0x00652260

//Retrieve
#define OFFS_RetrieveObjectEnhanced	0x006a504d
#define OFFS_TestOfRetrieveObject	0x006b0a91
#define OFFS_TriggerLoadAtPosition	0x006cc027
#define OFFS_EndOfRetrieveLocalVar	0x006a521b

#define OFFS_RetrieveCreaLocalVar	0x006a4f5d

#define FCT_RetrieveAddData			0x00586210

//Error during retrieve
#define RETRIEVE_ERR				0x006a521b

//Retrieve Specific Functions
#define FCT_RetrievePlaceableInfo	0x0064b030
#define FCT_RetrieveLightInfo		0x006d0f20
#define FCT_RetrieveWaypointInfo	0x006d0000
#define FCT_RetrieveTriggerInfo		0x006cb8c0
#define FCT_RetrievePlacedEffInfo	0x006d2870
#define FCT_RetrieveDoorInfo		0x00653eb0

//Creation
#define OFFS_TemplatePlaceExtension 0x0064fa46
#define OFFS_TemplatePlaceFctCall	0x0064fb0f		

//Initiate Object specific function
#define FCT_InitPlaceableInfo		0x0064f650
#define FCT_InitLightInfo			0x006d0de0
#define FCT_InitWaypointInfo		0x006cfe10
#define FCT_InitTriggerInfo			0x006caf20
#define FCT_InitPlacedEffInfo		0x006d24c0
#define FCT_InitDoorInfo			0x00652e60
/*
#define InitLightInfo				0x006d1530
#define InitWayPointInfo			0x006d0650
#define InitTriggerInfo				0x006ca8e0
#define InitPlacedEffectInfo		0x006d2be0

0064b030
*/



bool StoreRetrieveConfigAndApply(SimpleIniConfig* config);


#endif
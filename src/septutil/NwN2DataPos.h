#if !defined(NWN2_DATAPOS_H)
#define NWN2_DATAPOS_H

#include <cstdint>

//bool
#define AmPlcHasInventory       0x460
#define AmPlcDynamicCol			0x488

//uint16
#define AmPlcApp				0x318

//uint32
#define AmPlcColisionFlag		0xD0
#define AmPlcFactionID			0x358

//List of color
#define AmPlcColor              0x31C
//Bool
#define AmPlcHasUVScroll        0x34C
//float
#define AmPlcValueUScroll       0x350
//float
#define AmPlcValueVScroll       0x354
//bool
#define AmPlcIsAutoRemoveKey    0x3A0
//Bool
#define AmPlcIsTrapped			0x3B0

#define AmPlcFort				0x450
#define AmPlcWill				0x451
#define AmPlcRef				0x452

//pointer
#define AmPlcInventory          0x4B4


//uint8
#define AmObjectType			0xA4	
//pointer CExoString (?)
#define AmAppSEF                0x0C0
//int
#define AmCurrentHP             0x164
//int seem not change dynamicly with that...
#define AmMaxHP                 0x168
//int
#define AmTempHP                0x16C
//int32
#define AmModMaxHP              0x170

//Float 
#define AmCommonPosX			0x70
#define AmCommonPosY			0x74
#define AmCommonPosZ			0x78

#define AmCommonArea			0x144


//uint32
#define AmItmType               0x004
//uint8
#define AmItmVariation          0x00C
//uint8
#define AmItmCategory           0x00D

///////////// armor part /////////////////
//TODO, detailler...
//truc, puis bloc couleur, puis bloc UVSCROLL

//////////////////////////////////////////
#define AmItmLShoulder          0x040
#define AmItmRShoulder          0x080
#define AmItmLBracer            0x0C0
#define AmItmRBracer            0x100
#define AmItmLElbow             0x140
#define AmItmRElbow             0x180
#define AmItmLUpArm             0x1C0
#define AmItmRUpArm             0x200
#define AmItmLHip               0x240
#define AmItmRHip               0x280
#define AmItmFHip               0x2C0
#define AmItmBHip               0x300
#define AmItmLUpLeg             0x340
#define AmItmRUpLeg             0x380
#define AmItmLLowLeg            0x3C0
#define AmItmRLowLeg            0x400
#define AmItmLKnee              0x440
#define AmItmRKnee              0x480
#define AmItmLFoot              0x4C0
#define AmItmRFoot              0x500
#define AmItmLAnkle             0x540
#define AmItmRAnkle             0x580


#define AmItmHelm				0x5C0
#define AmItmGloves				0x5F8
#define AmItmBoots				0x630
#define AmItmBelt				0x668
#define AmItmCloak				0x6A0

#define AmItmArmors				0x6D8

///////////////////////////////////////////////
//bloc couleur
#define AmItmBaseColor          0x6DC
//uint8
#define AmItmWpnPart1           0xAA4
//uint8
#define AmItmWpnPart2           0xAA5
//uint8
#define AmItmWpnPart3           0xAA6


//Pointer to exoString
#define AmItmDescrIdent         0xAB4
//Pointer to exoString
#define AmItmDescrNonIdent      0xAC4
//uint8 (?)
#define AmItmFXWpn              0xB02



#define AmItmPlot				0x8B0 //
#define AmItmIsIdentified		0xA70 //


struct AmItmProperty
{
	uint16_t uPropertyName;
	uint16_t uSubType;
	uint8_t uCostTable;

		uint8_t Padding;
	
	uint16_t uCostValue;
	uint8_t uParam1;
	uint8_t uParam1Value;
	uint8_t uChanceAppear;
	
		uint8_t Padding2;
	
	uint8_t uUseable;
	
		uint8_t Padding3;
		uint16_t Padding4;
	
	uint8_t UsePerDay;

		uint8_t Padding5;
		uint16_t Padding6;
};


#define AmItmPropertyPtr		0xA84
#define AmItmPropertyNb			0xA88
#define AmItmPropertySizeArray	0xA8C


#define AmItmCharges			0xA90 //
#define AmItmCost				0xA94
#define AmItmModifyCost			0xA9C 
#define AmItmIsDropable			0xAE4 //
#define AmItmIsPickpocketable	0xAE8 //
#define AmItmIsCursed			0xAEC //
//int (short)
#define AmItmStackSize			0xAF0 // 
//Int
#define AmItmIcon				0xAF4 //
#define AmItmIsStolen			0xAF8 //

#define AmItmContainerUI		0xB08
#define AmItmIpActPref			0xB0C //

//uint32
#define AmItemEquipedBy			0xB10

//char
#define AmItmContainerPref		0xB14
#define AmItmForceContainer		0xB18


#define AmItmArmorRulesType		0x718 //
#define AmItmGMaterial			0x71C //


#define AmItmModelPieceColor1		0x4
#define AmItmModelPieceColor2		0x14
#define AmItmModelPieceColor3		0x24
#define AmItmModelPieceUVSCroll		0x34
#define AmItmModelPieceUScroll		0x38
#define AmItmModelPieceVScroll		0x3C


#define AmItmModelPartVariation		0x04
#define AmItmModelPartCategory		0x05
#define AmItmModelPartColor1		0x08
#define AmItmModelPartColor2		0x18
#define AmItmModelPartColor3		0x28

///////// Creatures //////////

#define AmCrtAge				0x3CC
#define AmCrtABAge				0xE8

#define AmCrtEffectPtr			0x1A0
#define AmCrtEffectNb			0x1A4
#define AmCrtEffectSize			0x1A8

#define AmCrtPlot				0x180
//uint32
#define AmCrtMonkSpeed			0x778


//
#define AmCrtDetectMode			0x6F8
#define AmCrtStealthMode		0x6F9
#define AmCrtTrackingMode		0x6FA
#define AmCrtEnhVisionMode		0x6FB

#define AmCrtDefCastMode		0x700
#define AmCrtCombatMode			0x701

#define AmCrtHlfrBlstMode		0x710
#define AmCrtHlfrShldMode		0x711


//uint32
#define AmCrtSize				0x730

//uint32
#define AmCrtGoldPiece			0xEE8

//short
#define AmCrtSoundSet			0xEF0
/* 
ID table of equiped object ( Inventory_Slot +1)
// 0 : 98 2F 80 00
// 1 : helm
// 2 : Armor
// .........
// 11 : Belt
*/
#define AmCrtCSEquipedObjectTable	0x0F7C

//ptr to "uint32:id , ???? , ptr:list of id, number, size"
#define AmCrtPtInventory			0x0F80

//uint32
#define AmCrtWeightEq				0x0F8C
//??? 0xF90 ??? 
//??? 0xF94 ??? Surcharge ???
//??? 0xF98 ??? cmp == 1 ?
#define AmCrtWeightTotal			0x0F9C

//#define AmCrtCS????               0x06F8
//uint8 (quasi bool)
#define AmCrtGenderVis          0x1838




/////////////////////////////////////////////////

#define AmCrtOffsetApp				0x180C
#define AmCrtHeadVariation			0x1F10
#define AmCrtTailVariation			0x1F11
#define AmCrtWingVariation			0x1F12
#define AmCrtHairVariation			0x1F13
#define AmCrtFacialHairVariation	0x1F14
#define AmCrtTint					0x1F18
#define AmCrtHeadTint				0x1F48
#define AmCrtHairTint				0x1F78

//uint32 (almost bool : 0 not hide, 1 hide, other strange semi hide)
#define AmCrtHideHelmet				0x1FC0

#define AmCrtPtrAppBlock			0x1FC4


/////////////////////////////////////////////////

//uint16_t
#define AmCrtABRace					0x000A
#define AmCrtABSubRace				0x000C


//uint8_t
#define AmCrtABNbClass				0x0014

//uint32_t
#define AmCrtABIsDM					0x0094

//Pointer to pointer to exostring
#define AmCrtABPtrToDesc			0x00DC

//
#define AmCrtABFactionID			0x00FC

//Not a real "pointer", but a size for following blocks
#define AmCrtAbSizeClass			0x0124

#define AmCrtAbClass0				0x010C
#define AmCrtAbClass1				0x0230
#define AmCrtAbClass2				0x0354
#define AmCrtAbClass3				0x0478

struct AmCrtClassSpellKnow
{
	uint16_t*		lSpellsKnow;
	uint32_t		NbSpellsKnow;
	uint32_t		SizelSpellsKnow;
};

struct AmCrtClassSpellMemorized
{
	uint16_t		spellID;
	uint16_t		Unknow1;

	uint8_t			isReady;
	uint8_t			Unknow2[3];

	uint8_t			isDomain;
	uint8_t			Unknow3[3];

	uint32_t		metaMagic;
};

struct AmCrtClassMemorizedSpells
{
	AmCrtClassSpellMemorized**		lSpellMemorized;
	uint32_t		NbSpellsSlot;
	uint32_t		size;
};


struct AmCrtClass
{
	void*		fctPtr;
	
	uint8_t		classID;		//0x4
	uint8_t		classLvl;		//0x5
	uint8_t		Unknow1_SetTo0;			//See used for monk AC calculation. 
	uint8_t		School;			//0x7

	uint8_t		Domain1;		//0x8
	uint8_t		Domain2;		//0x9
	uint16_t	Unknow2;

	void*		CreatureStatsPtr;	//0xC

	AmCrtClassSpellKnow		SpellsKnow[10];			//0x10

	uint8_t		SpellPerDaysLeft[10];				//0x88

	uint16_t	Unknow3;

	AmCrtClassMemorizedSpells	MemorizedSpells[10];	//0x94


	uint8_t		BonusSpell[10];							//0x10C
	uint8_t		MaxSpellsPerDay[10];					//0x116

	uint32_t	Unknow4;
};

//uint8
//#define AmCrtClassClassId			0x0004
//#define AmCrtClassClassLvl			0x0005



/* Armor table:
//All will come by two (int8), first Its bonus, second reduction
//An negative bonus will reduce the AC, a negative reduction will
//improve it. 0, 1 and 2 value don't have reduce.
// 0 : Base Armor value
//1 : ArmorType / 2 : ShieldType / 3-4 : Armor AC / 5-6 : Deflect
//7-8 : Shield / 9-10 : Natural / 11-12 : Dodge 
*/
#define AmCrtABArmorTable			0x059C

//uint8
#define AmCrtABMagicResistance		0x05D4 
//uint8
#define AmCrtABReducMagicResistance	0x05D5

#define AmCrtABTint					0x0650
#define AmCrtABHeadTint				0x0680
#define AmCrtABHairTint				0x06b0
#define AmCrtABHairVariation		0x06ec
#define AmCrtABFacialHairVariation	0x06ed
//uint16. We can use it for uid
#define AmCrtABTatoo1and2			0x06ee
#define AmCrtABHeadVariation		0x06f2

#define AmCrtABCreatureArmor		0x06F8

#define AmCrtABTailVariation		0x0dc8
#define AmCrtABWingVariation		0x0dc9

#define AmCrtABHideHelmet			0x0DCC

#define AmCrtABPersistScaleX		0x0DD4
#define AmCrtABPersistScaleY		0x0DD8
#define AmCrtABPersistScaleZ		0x0DDC
//int8
#define AmCrtABModFortitude			0x0DEC
#define AmCrtABModWill				0x0DED
#define AmCrtABModReflex			0x0DEE

//uint32
#define AmCrtABAnimalCompanionType	0xDF0
#define AmCrtABFamiliarType			0xDF4

//CString 
#define AmCrtABAnimalCompanionName	0xDF8
#define AmCrtABFamiliarName			0xE00

//////////////// Trigger //////////////////////////////

#define AmTrigTransitionDest	0x320
#define AmTrigTransitionType	0x328

#define AmTrigGeometrySize		0x36C
#define AmTrigGeometryPtr		0x370
#define AmTrigGeometryBufSize	0x374
#define AmTrigGeometrySize2		0x378

#define AmTrigTypeTransition	0x3A4
#define AmTrigPartyTransition	0x3A8
#define AmTrigTypeWalkmesh		0x3AC
#define AmTrigFactionID			0x3B0
#define AmTrigTypeTrap			0x3B4

#define AmTrigAutoRemoveKey		0x3C8
#define AmTrigTrapOneShot		0x3CC
#define AmTrigTrapType			0x3D0

#define AmTrigHighLightHeigh	0x3E0

#define AmTrigCursor			0x3FC

#define AmTrigLoadScreenID		0x408
#define AmTrigTrapRecoverable	0x410


//Name ===> 0x310 & 0x314

//////////
/*
0x70 0x74 0x78
tableau de geometry , x y z. a faire a chaque fois  xGeo - Xbase; yGeo - Ybase; zGeo - Zbase



type gff!
2 = piege
3 = lasso walkmesh
1 = trasition
0 = generique


3a4 ===> transition
3ac ===> lasso walkmesh
3b0 ===> ???
3b4 ===> piege


40c ==> trap ?
410 ?? truc lorsque piege ?




*/


//////////////// Area //////////////////////////////

//1 : Interior , 2 : Subterran, 4: Natural 
#define AmAreaFlag					0x08


#define AmAreaDNCStart				0x4C
//DCN start : 4C + 8*dncsize(13c) => a2c
//Byte
#define AmAreaDayNightCycle			0xA2C
#define AmAreaDayNightShadows		0xA30
#define AmAreaIsNight				0xA34
#define AmAreaOverlandMap			0xA38		//?
//Float
#define AmAreaOverlandCamDis		0xA3C		//?
#define AmAreaOverlandCamPitch		0xA40		//?
#define AmAreaOverlandCamYaw		0xA44		//?
//int
#define AmAreaUseDayNightFog		0xA48
#define AmAreaHasDirLight			0xA4C
//Byte
#define AmAreaSkyBox				0xA4E		//?

//(default values?) 
#define AmAreaRainPower				0xA54 //int
#define AmAreaRainVar				0xA58 //int
#define AmAreaRainChance			0xA5C //Char

#define AmAreaSnowPower				0xA64
#define AmAreaSnowVar				0xA68
#define AmAreaSnowChance			0xA6C

#define AmAreaLightningPower		0xA74
#define AmAreaLightningVar			0xA78
#define AmAreaLightningChance		0xA7C

#define AmAreaWindPower				0xA80 //Char

#define AmAreaNoRest				0xA84
#define AmAreaNorthDirection		0xA88		//?
#define AmAreaUnknowID				0xA8C		//?

//ptr
#define AmAreaPtrBlockId			0xBF4

//int
#define AmAreaBlockIDID				0x0
#define AmAreaBlockIdCreatorID		0x4
//uint
#define AmAreaBlockIdVersion		0x8

//Int
#define AmAreaModSpotCheck			0xC20
#define AmAreaModListenCheck		0xC24


//Weather Overide
//Rain : 0xC68
//Snow : 0xC78
//Lightning : 0xC88
//WeatherOveride(equivalent to wind in base one) : 0xC94


#define AmAreaFogDuplicationStart	0xCA8 //Start of duplication of DNC fogs values
#define AmAreaFogDuplicationStop	0xD64 //Last float ptr of duplication of DNC fogs values

//Byte
#define AmAreaLightingScheme		0xE28		//?
#define AmAreaPVPMode				0xE29
//Short
#define AmAreaLoadScreenID			0xE30
#define AmAreaSkyRingN				0xE32
#define AmAreaSkyRingE				0xE34
#define AmAreaSkyRingS				0xE36
#define AmAreaSkyRingW				0xE38

#define AmAreaEnableWater			0xE80




//Not a pointer, just the size of a DNC block.
#define AmAreaDNCSize				0x13C
//Composite
#define AmAreaDNCSkyLight			0x8
#define AmAreaDNCSunMoon			0x40
#define AmAreaDNCGroundLight		0x78

//Color
#define AmAreaDNCSkyZenith			0xAC
#define AmAreaDNCSkyHorizon			0xBC

//Float
#define AmAreaDNCSunMoonDirX		0xCC
#define AmAreaDNCSunMoonDirY		0xD0
#define AmAreaDNCSunMoonDirZ		0xD4

//SmallColor (RGB only)
#define AmAreaDNCFogColor			0xD8

//Float
#define AmAreaDNCFogStart			0xE4
#define AmAreaDNCFogEnd				0xE8
#define AmAreaDNCFarClip			0xEC
#define AmAreaDNCCloudMoveX			0xF0
#define AmAreaDNCCloudMoveY			0xF4
#define AmAreaDNCShadowIntensity	0xF8
#define AmAreaDNCCoronaIntensity	0xFC
#define AmAreaDNCSkyRingLERP		0x100
//ExoString
#define AmAreaDNCSkyDomeModel		0x104
//byte
#define AmAreaDNCDesaturateColor	0x10C
//Float
#define AmAreaDNCDesaturateFactor	0x110
#define AmAreaDNCBloomSCIntensity	0x114
#define AmAreaDNCBloomGLIntensity	0x118
#define AmAreaDNCBloomHLThreshold	0x11C
#define AmAreaDNCBloomHLIntensity	0x120
#define AmAreaDNCBloomBLRadius		0x124
#define AmAreaDNCHDRHighlight		0x128
#define AmAreaDNCHDRMax				0x12C
#define AmAreaDNCHDRAvg				0x130
#define AmAreaDNCExposure			0x134
#define AmAreaDNCCloudCover			0x138



//////////////// Door //////////////////////////////

#define AmDTrapType             0x434	//Used for detection name, not damage or animation
#define AmDTrapScript			0x358

#define AmDFort					0x3F4
#define AmDRef					0x3F5
#define AmDWill					0x3F6

#define AmDIsTrapped            0x424
#define AmDApparence            0x388
#define AmDColor                0x3B0
#define AmDIsAutoRemoveKey      0x42C
#define AmDIsInvisibleTrans     0x450
//uint8
#define AmDTransitionType       0x47C
#define AmDTransitionDest       0x480
#define AmDIsGroupTransition    0x48C
#define AmDLoadScreenId         0x49F

//uint32
#define AmDFactionID			0x3F0

//////////////////////// VFX ////////////////////////

#define AmVFXSEF				0x310


////////////////// Light ////////////////////////////

#define AmLightColor			0x314 //Composite
#define AmLightLerpColor		0x34C //Composite

#define AmLightCompositeDiffuse		0x0		//RGBA color
#define AmLightCompositeSpecular	0x10	//RGBA color
#define AmLightCompositeAmbiant		0x20	//RGBA color
#define AmLightCompositeIntensity	0x30	//Float

#define AmLightLerpPeriod		0x380 //Float
#define AmLightFlickerVariance	0x384 //Float
#define AmLightFlickerRate		0x388 //Float
#define AmLightRange			0x38C //Float

#define AmLightCastShadow		0x390 //uint (bool)
#define AmLightShadowIntensity	0x394 //Float

#define AmLightIsOn				0x398 //uint 	

#define AmLightLerpFlick		0x39C //0x1 & 0x2 mask
#define AmLightFlickerType		0x3A0 //0x7 mask


struct NwNFloatColor
{
	float m_R;
	float m_G;
	float m_B;
	float m_A;
};

struct NwNUVScroll
{
	uint32_t	m_bActive;
	float		m_fUScroll;
	float		m_fVScroll;
};

struct ABCreatureItemPart
{
	void* ptr;
	uint8_t m_ArmorApp[4];
	
	NwNFloatColor m_Color[3];
	
};

struct ABArmorModelPiece
{
	uint32_t iVariation;
	NwNFloatColor m_Color[3];
	NwNUVScroll	m_UVScroll;
};

struct ABCreatureMainArmor
{
	uint8_t m_ArmorApp[4];

	NwNFloatColor m_Color[3];
	ABArmorModelPiece m_ModelPiece[22];
};

struct ABCreatureArmor
{
	ABCreatureMainArmor m_abMainArmor;
	ABCreatureItemPart m_ItemPart[5];
	
	uint8_t m_ShowItemMask;
};





/*
GENERAL_FT_CAT			: 1
PROFICIENCY_FT_CAT		: 2
SPELLCASTING_FT_CAT		: 3
METAMAGIC_FT_CAT		: 4
ITEMCREATION_FT_CAT		: 5
DIVINE_FT_CAT			: 6
SKILLNSAVE_FT_CAT		: 7
HISTORY_FT_CAT			: 8
BACKGROUND_FT_CAt		: 9
RACIALABILITY_FT_CAT	: 0xA
CLASSABILITY_FT_CAT		: 0xB
EPIC_FT_CAT				: 0xC
HERITAGE_FT_CAT			: 0xD
TEAMWORK_FT_CAT			: 0xE
*/

#endif
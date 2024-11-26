#if !defined(NWN2_INTERN2DA_H)
#define NWN2_INTERN2DA_H


#include "../../NWN2Lib/NWN2.h"
#include "../../NWN2Lib/NWN2Common.h"
#include <cstdint>

/*
#define POSITION2DA_CLASSES			0x168

#define CLASSES2DA_LINESIZE			0x630



#define POSITION2DA_FEATS			0x140

#define FEATS2DA_LINESIZE			0x0B4


154  => feat numbers


158 => MasterFeats number
159 => Classes Number

MasterFeats stuff:
144 => STRREF
148 => DESCRIPTION
14C => ICON

*/

namespace NWN2DA
{


	struct feats2DA_line
	{
		uint16_t							m_featIdx;

		//MinAb
		uint8_t								m_MinStr;
		uint8_t								m_MinDex;
		uint8_t								m_MinCon;
		uint8_t								m_MinInt;
		uint8_t								m_MinWis;
		uint8_t								m_MinCha;

		//MaxAb
		uint8_t								m_MaxStr;
		uint8_t								m_MaxDex;
		uint8_t								m_MaxCon;
		uint8_t								m_MaxInt;
		uint8_t								m_MaxWis;
		uint8_t								m_MaxCha;

		//nice to know, if a prereqFeat is not valid, -1 will be put here instead of the 2DA value
		uint16_t							m_PrereqFeat1;
		uint16_t							m_PrereqFeat2; //0x10		

		uint16_t							m_OrReqFeat0; //0x12
		uint16_t							m_OrReqFeat1; //0x14
		uint16_t							m_OrReqFeat2; //0x16
		uint16_t							m_OrReqFeat3; //0x18
		uint16_t							m_OrReqFeat4; //0x1a
		uint16_t							m_OrReqFeat5; //0x1c

		uint16_t							m_Align1; // 2 bits alignement

		uint32_t							m_feat;				//0x20
		uint32_t							m_description;
		uint32_t							m_category;
		uint32_t							m_MaxCR;
		char								m_Icon[32];  //0x30-0x50

		uint32_t							m_GainMultiple;		//0x50

		uint8_t								m_allClassCanUse;	//0x54

		uint8_t								m_Align2; //1bit alignment
		uint16_t							m_Align3; //2bits alignment

		uint32_t							m_TargetSelf;		//0x58
		uint32_t							m_HostileFeat;		//0x5C


		uint8_t								m_MinAttackBonus; //0x60
		uint8_t								m_MinSpellLvl;
		uint8_t								m_MinCasterLvl;	//0x62
		uint8_t								m_Padding;
		//Todo here (1bit)
		uint16_t							m_ReqSkill;	//0x64
		uint16_t							m_ReqSkillMinRanks;
		uint16_t							m_ReqSkill2;
		uint16_t							m_ReqSkillMinRanks2;
		uint16_t							m_ReqSkillMaxRanks;
		uint16_t							m_ReqSkillMaxRanks2;

		//nice to know, if a successor is not valid, -1 will be put here
		uint16_t							m_Successor; //0x70

		uint16_t							m_Predecessor;

		uint8_t								m_MasterFeat; //0x74
		uint8_t								m_MinLevel;		
		uint8_t								m_MaxLevel;		
		uint8_t								m_MinLevelClass; // 0x77
		uint8_t								m_MinLevelClass_bis; // 0x78 //can't we just store a max level class ? Seem to be used like that
		uint8_t								m_MinFortSave; // 0x79

		uint16_t							m_SpellId;		//0x7A
		uint8_t								m_UsesPerDay;	//0x7C

		uint8_t								m_Align4;		//1bit alignment

		uint16_t							m_UsesMapFeat;  //0x7E

		uint32_t							m_featValid;			//0x80
		uint32_t							m_featRemoved;			//0x84
		uint32_t							m_PreReqEpic;			//0x88
		int32_t								m_FeatCategory;
		uint32_t							m_IsActive;				
		uint32_t							m_ToggleMode;			//0x94				//Strange stuff, TODO: document it
		int32_t								m_Cooldown;				//0x98
		NWN::CExoString						m_Constant;				//0x9C				//should take 64bits, so up to 0xA4

		uint32_t							m_IsPersistent;			//0xA4
		uint32_t							m_DMFeat;				//0xA8

		uint8_t								m_AlignRestrict;		//0xAC

		uint8_t								m_Align5; //1bit alignment
		uint16_t							m_Align6; //2bits alignment

		uint32_t							m_Instant;				//0xB0
	};

	typedef feats2DA_line* feats2DA;

	struct MasterFeats2DA
	{
		uint32_t*							m_StrRefList;
		uint32_t*							m_DescriptionList;
		char*								m_IconsList;			//serie of 32 char arrays.
	};


	//Size of ClsFeat2DA :0xC
	struct clsFeat2DA
	{
		int16_t								m_FeatIndex;
		int8_t								m_GrantedOnLevel; //2
		int8_t								m_List;	//3
		uint32_t							m_OnMenu;	//4

		int16_t								m_GrantedPrereq; //8
		uint16_t								m_Padding;
	};

	struct clsSkills2DA
	{
		uint16_t							m_SkillIndex;
		char								Padding0[2];
		uint8_t								m_ClassSkill;
		char								Padding1[3];
	};

	struct spellKnowTable
	{
		uint8_t*					m_pSpellKnowForLevel[0x28];			//0x28 pointer to list of uint8_t. Size of list : value of NumSpellLevel
	};


	struct classes2DA_line
	{
		uint32_t							m_ClassRow;
		uint32_t							m_Name;
		uint32_t							m_Lower;
		uint32_t							m_Plural;
		uint32_t							m_Description;	//x10

		NWN::CExoString						m_Icon;
		NWN::CExoString						m_BorderedIcon;		//0x1C				//should take 64bits, so up to 0x24

		NWN::CExoString						m_PreReqTable;		//0x24  => up to 2C

		uint8_t								m_AttackBonus[0x28];	//0x2C => 0x54
		uint8_t								m_HitDie;		//0x54
		uint8_t								m_SkillPointBase;//0x55

		uint8_t								m_FortSave[0x28];	//0x56
		uint8_t								m_WillSave[0x28];	//0x7e
		uint8_t								m_RefSave[0x28];	//0xa6 => 0xCE

		uint16_t							m_Unknow;

		spellKnowTable*						m_SpellKnowTable;			//0xD0
		uint8_t								m_NumSpellLevels[0x28];		//0xd4 => 0xFC   //Number of spellLvl for this lvl (example, if you have only access to lvl0 => 1 , if you have access up to 1 => 2 etc)
		uint8_t*							m_SpellLevelsX[0x28];		//0xFC => 19C.  Each pointer is a list of uint8. 1 for each NumSpellLevels. for each lvl , give an array giving the spell number for each lvl

		uint32_t							m_GrantsBonusSpellcasterLvl[0x28];		//0x19C  => 23C			//Table that say when the spellcaster feat will effectively grant a spellcaster lvl
		uint8_t								m_CombinedBonusSpellcasterLvl[0x28];	//0x23C  => 0x264		//Actual spellLvl given at this level. (example... if you give at lvl1 and 3, you will have 1 1 2 in this table)

		uint16_t							m_SpellCasterFeat[0xFE];				//0x264 => 0x460


		/* don't want to bother with that for now, just create the padding space
		uint16_t							m_SpellConversionSpellId0;
		uint16_t							PaddingSC0;
		NWN::CExoString						m_SpellConversionOverlayIcon0;
		uint16_t							m_SpellConversionAltSpellId0;
		uint16_t							PaddingSC0b;
		NWN::CExoString						m_SpellConversionAltOverlayIcon0;
		*/

		uint8_t								m_FullSpellConversion[0xF0];			//0x460  => 0x550





		//Skills Table
		clsSkills2DA*						m_clsSkillsTable;				//0x550
		uint16_t							m_sizeSkillsTable;				//0x554

		//here,2bits
		uint16_t							Padding2b;

		//ClsFeat are added only if valid and not removed.
		clsFeat2DA*							m_clsFeatTable;					//0x558
		uint16_t							m_sizeFeatTable;				//0x55c

		uint8_t								m_BonusFeat[0x28];				//0x55e => 0x586
		uint8_t								m_NormalBFeat[0x28];			//0x586 => 0x5AE

		//here 2bit
		uint16_t							Padding2;

		uint32_t							m_PrimaryAbil;					//0x5B0
		uint32_t							m_SpellAbil;					//0x5B4

		uint8_t								m_Str;
		uint8_t								m_Dex;
		uint8_t								m_Con;
		uint8_t								m_Int;
		uint8_t								m_Wis;
		uint8_t								m_Cha;							

		uint8_t								m_AlignRestrict;				//0x5BE
		uint8_t								m_AlignRestrictType;
		uint8_t								m_InvertRestrict;				//0x5C0

		uint8_t								Padding2bb;
		uint16_t							Padding2c;

		uint8_t								m_EffCRLvlX[20];					//0x5C4   => 5D8

		void*								m_UnknowPtr;

		uint32_t							m_PlayerClass;					//0x5DC
		uint32_t							m_SpellCaster;					//0x5E0

		uint32_t							m_MaxLevel;						//0x5E4
		uint32_t							m_XPPenalty;					
		uint32_t							m_Package;						//0x5EC

		uint32_t							m_MemorizesSpells;				//0x5F0

		uint8_t								m_ArcSpellLvlMod;				//0x5F4
		uint8_t								m_DivSpellLvlMod;
		uint8_t								m_EpicLevel;					//0x5F6

		//here, 1bits
		uint8_t								Padding3;

		uint32_t							m_HasArcane;					//0x5F8
		uint32_t							m_HasDivine;					//0x5FC
		uint32_t							m_AllSpellsKnow;				//0x600
		uint32_t							m_HasDomains;					//0x604
		uint32_t							m_HasSchool;					//0x608
		uint32_t							m_HasFamiliar;
		uint32_t							m_HasAnimalCompanion;
		uint32_t							m_HasSpontaneousSpells;
		uint16_t							m_FeatPracticedSpellCaster;
		uint16_t							m_FeatExtraSlot;
		uint16_t							m_FeatArmoredCaster;

		uint16_t							Padding4;
		uint32_t							m_MetaMagicAllowed;
		uint32_t							m_HasInfiniteSpells;
		uint8_t								m_SpellSwapMinLvl;
		uint8_t								m_SpellSwapLvlInterval;
		uint8_t								m_SpellSwapLvlDiff;
		uint8_t								m_FavoredWeaponProficiency;
		uint8_t								m_FavoredWeaponFocus;
		uint8_t								m_FavoredWeaponSpecialization;		//0x62D

		uint16_t							Padding5;
	};

	typedef classes2DA_line* classes2DA;


	struct skills2DA_line
	{
		uint32_t				m_Name;
		uint32_t				m_Description;	//0x4

		uint32_t				m_Category;		//0x8
		uint32_t				m_MaxCR;		//0xC

		NWN::CExoString			m_Icon;			//0x10

		uint32_t				m_KeyAbility;	//0x18

		uint32_t				m_CosmopolitanFeat;

		uint32_t				m_HostileSkill;	
		uint32_t				m_Untrained;	
		uint32_t				m_ArmorCheckPenalty;	
		uint32_t				m_AllClassesCanUse;		//0x2C

		uint32_t				m_IsAnActiveSkill;		//0x30

		uint32_t				m_ToggleMode;			//0x34

		uint32_t				m_Removed;				//0x38

		uint32_t				m_PlayerOnly;			//0x3C
	};


	//Toggle Mode => param_1 + 0x3e   + uStack_34*2 = row (uint16)  <===== uStack_34 == contenu de toggleMode


	typedef skills2DA_line* skills2DA;


	struct racialSubtypes2DA_line
	{
		uint32_t			Unknow_Unused;	


		int32_t				m_Name;		//0x04
		int32_t				m_ConverName;	//0x08
		int32_t				m_ConverNameLower;//0xC
		int32_t				m_NamePlural; //0x10
		int32_t				m_NameLower;	//0x14
		int32_t				m_NameLowerPlural;	//0x18
		int32_t				m_Description;		//0x1C
		int32_t				m_Biography;			//0x20


		int8_t				m_StrAdjust;			//0x24
		int8_t				m_DexAdjust;
		int8_t				m_ConAdjust;
		int8_t				m_IntAdjust;
		int8_t				m_WisAdjust;
		int8_t				m_ChaAdjust;			//0x29

		int8_t				m_Endurance;			//0x2A

		uint8_t				m_Favored;			//0x2B
		uint32_t			m_HasFavoredClass;	//0x2C
		uint32_t			m_PlayerRace;			//0x30

		uint32_t			Unknow;

		uint16_t			m_NumberRacialFeats;	//0x38
		uint16_t			Padding;
		uint16_t*			m_RacialFeatsList;	//0x3C

		uint32_t			m_Age;				//0x40

		uint32_t			Unused[10];


		uint32_t			m_Index;	   // 6c
		int32_t				m_BaseRace;	//0x70
		int32_t				m_ECL;		//0x74
		int32_t				m_AppearanceIndex; //0x78
		NWN::CExoString		m_Color2DA; // 7c
		NWN::CExoString		m_male_race_icon; // 84
		NWN::CExoString		m_racial_banner; // 8c
		NWN::CExoString		m_Constant; //0x94
	};

	typedef racialSubtypes2DA_line* racialSubtypes2DA;

	struct racialtypes2DA_line
	{
		int32_t				m_Index; //0x00
		int32_t				m_Name;		//0x04
		int32_t				m_ConverName;	//0x08
		int32_t				m_ConverNameLower;//0xC
		int32_t				m_NamePlural; //0x10
		int32_t				m_NameLower;	//0x14
		int32_t				m_NameLowerPlural;	//0x18
		int32_t				m_Description;		//0x1C
		int32_t				m_Biography;			//0x20


		int8_t				m_StrAdjust;			//0x24
		int8_t				m_DexAdjust;
		int8_t				m_ConAdjust;
		int8_t				m_IntAdjust;
		int8_t				m_WisAdjust;
		int8_t				m_ChaAdjust;			//0x29

		int8_t				m_Endurance;			//0x2A

		uint8_t				m_Favored;			//0x2B

		uint32_t			Unknow;				//0x2C			//??
		//0x2C

		uint32_t			m_PlayerRace;			//0x30

		uint32_t			m_isHumanoid;			//0x34

		uint32_t			m_NumberRacialFeats;	//0x38
		uint16_t*			m_RacialFeatsList;	//0x3C
		uint32_t			m_Age;				//0x40

		int8_t				m_DefaultSubRace;		//0x44

		int8_t				Padding1;
		int16_t				Padding2;

		NWN::CExoString		m_male_race_icon; // 0x48
		NWN::CExoString		m_Constant; // 0x50

		int16_t				m_FeatFavoredEnemy;	//0x58
		int16_t				m_FeatImprovedFavoredEnemy;	//0x5A
		int16_t				m_FeatFavoredPowerAttack;		//0x5C
		int16_t				m_FeatIgnoreCritImmunity;		//0x5E

		racialSubtypes2DA_line** m_racialSubtypesList;	//0x60
		uint32_t			m_NumberOfRacialSubtypes;		//0x64
		uint32_t			m_racialSubtypeListAllocatedSize; //0x68
	};

	typedef racialtypes2DA_line* racialtypes2DA;


	struct background2da_line {
		uint32_t		m_Row;
		uint32_t		m_Name;				//0x4
		uint32_t		m_Description;		//0x8

		uint32_t		m_MinAttackBonus;	//0xC

		uint8_t			m_MinAbility[6];	//0x10
		uint8_t			m_MaxAbility[6];	//0x16

		uint32_t		m_Gender;			//0x1C

		uint8_t			m_OrReqClass[3];	//0x20

		uint8_t			Padding;			

		uint16_t		m_DisplayFeat;		//0x24

		uint16_t		m_FeatGained;		//0x26		//0xFFFF if not/ notvalid
		uint8_t			m_MasterFeatGained;	//0x28

		char			m_Icon[32];			// 0x29

		uint8_t			Padding2;
		uint16_t		Padding3;

		uint32_t		m_REMOVED;			//0x4C
	}; //0x50

	typedef background2da_line* background2da;


	// 0x4C
	struct domains2da_line {
		uint32_t		m_Name;
		uint32_t		m_Description;

		char			m_Icon[32];
		uint16_t		m_SpellByLvl[10];
		uint16_t		m_GrantedFeat;		//0x3C

		uint16_t		m_Padding;

		uint32_t		m_isValid;		//??? 0x40

		uint32_t		m_CastableFeat;	//0x44
		uint16_t		m_EpithetFeat; //0x48
		uint16_t		m_Padding2;
	};

	typedef domains2da_line* domains2da;

	struct spells2da_line
	{
		NWN::CExoString	m_Label;
		uint32_t	m_Name;			//0x8
		uint32_t	m_SpellDesc;	//0xC
		char		m_IconResRef[32];	//0x10
		NWN::CExoString	m_SchoolS;	//0x30
		uint8_t		m_SchoolV;		//0x38 //Todo, change to add an enum ? a=1 c=2 D=3 E=4 I=6 N=7 T=8 V=5
		uint8_t		m_Range;	//0x39 //Todo enum? P:0 T:1 S:2 M:3 L:4 I:5 -1
		uint16_t	Padding0;
		NWN::CExoString	m_VS;	//0x3C
		//
		uint16_t	m_TargetType;	//0x44
		uint16_t	Padding1;
		//
		NWN::CExoString	m_ImpactScript;	//0x48
		int8_t		m_Bard;			//0x50
		int8_t		m_Cleric;		//0x51
		int8_t		m_Druid;		//0x52
		int8_t		m_Paladin;		//0x53
		int8_t		m_Ranger;		//0x54
		int8_t		m_WizSorc;		//0x55
		int8_t		m_WizSorc2;		//0x56
		int8_t		m_Warlock;		//0x57
		int8_t		m_Innate;		//0x58
		//
		int8_t		Padding3[3];	//0x59-0x5B
		//
		int32_t		m_ConjTime;		//0x5C
		int16_t		m_ConjAnim;		//0x60				//read:8 party:5 bardsong:7 defensive:4 attack:6 major:3 head:2 hand:1 else:0
		char		m_ConjVisual0[32];	//0x62
		char		m_ConjVisual1[32];	//0x82
		char		m_ConjVisual2[32];	//0xa2
		char		m_LowConjVisual0[32];	//0xC2		
		char		Padding4[0x40];	//? 0x20*2 (lowconjVisual1 & 2 doesnt exist)
		char		m_ConjSoundMale[32];	//0x122
		char		m_ConjSoundFemale[32];	//0x142
		char		m_ConjSoundVFX[32]; //0x162
		uint16_t	Padding5;
		NWN::CExoString		m_ConjSoundOverride; //0x184
		int16_t		m_CastAnim;		//18c creature:8 attack:7 touch:6 general:0xd major:0xc defensive:0xb throw:0x9 lightning:6 (?)  point:6(?) bardsong:a  area:4 turn:3 up:3(?) self:2  spray/out:1
		int16_t		Padding6;
		uint32_t	m_CastTime;	//0x190
		char		m_CastVisual0[32]; //0x194
		char		m_CastVisual1[32];//0x1B4
		char		m_CastVisual2[32];//0x1D4
		char		m_LowCastVisual0[32];//0x1F4
		char		Padding7[0x40]; //LowCastVisual1 & 2 don't exist
		char		m_CastSound[32];	//0x254

		uint32_t	m_Proj;		//0x274
		char		m_ProjModel[32];	//0x278
		NWN::CExoString	m_ProjSEF;	//0x298
		NWN::CExoString m_LowProjSEF;	//0x2a0
		
		uint32_t	m_ProjType;		//0x2A8  thrownballistic:0x11 launchedballistic:0x10 loworbit:0x0f homingspiral:0x0e burst:0x09 bounce:0x08 linked:0x07 spiral:0x06 accelerating:0x05 burstup:0x04 highballistic:0x03 ballistic:0x02 homing:0x01

		uint8_t		m_ProjSpwnPoint;	//0x2AC	 hands:0x1 lhand:0x2 rhand:0x3 head:0x4 chest:0x5 halo:0x6 mouth:0x7 attach:0xa ... Other stuff ?
		char		m_ProjSound[32];	//0x2AD
		uint8_t		m_ProjOrientation;	//0x2CD	path:0x2 target:0x1 else:0
		
		uint16_t	Padding8;

		NWN::CExoString	m_ImpactSEF;	//0x2D0
		NWN::CExoString m_LowImpactSEF;	//0x2D8

		uint8_t		Padding9[8]; //??? 0x2E0

		uint32_t	m_Category;		//0x2E8
		int32_t		m_Innate2;		//0x2EC

		uint16_t*	m_SubRadialList; //0x2F0
		uint8_t		m_SubRadialNb;	//0x2F4
		char		Padding10[3];
		uint32_t	m_UseConcentration;	//0x2F8

		//

		int32_t		m_Master;			//0x2FC (spell_id or value)
		uint32_t	m_Counter1;			//0x300 (0xFFFFFFFF or value)
		uint32_t	m_Counter2;			//0x304 (0xFFFFFFFF or value)
		uint8_t		m_UserType;			//0x308
		
		char		Padding11[3];

		uint32_t	m_SpontaneouslyCast;	//0x30C

		uint32_t	Padding12; //?? 0x310

		uint32_t	m_MetaMagic;			//0x314
		uint32_t	m_AsMetaMagic;			//0x318
		uint32_t	m_AltMessage;			//0x31C
		uint32_t	m_HostileSetting;		//0x320
		uint32_t	m_FeatID;				//0x324
		uint32_t	m_HasProjectile;		//0x328


		uint32_t	m_Available;	//0x32c			If ok, setTo1
		uint32_t	m_Removed;		//0x330


		uint8_t		m_TargetingUI;	//0x334
		bool		m_CastableOnDead;	//0x335


		uint16_t	Padding13;

	};
	typedef spells2da_line* spells2da;

	struct spells2DAredirect_ {
		uint32_t		m_NumberOfSpells;
		spells2da		m_pSpells;
	};
	typedef spells2DAredirect_* spells2DAredirect;

	struct access2da {
		char			Padding[0x138];
		spells2DAredirect m_pSpellStrct;	//0x138
		uint32_t			m_pMetamagic;		//0x13C	TODO
		feats2DA		m_pFeats;		//0x140
		MasterFeats2DA	m_MasterFeat;	//0x144  

		uint16_t*		m_ListOfValidFeats;		//0x150

		uint16_t		m_NumberOfFeats;	//0x154

		uint16_t		m_NumberOfValidFeats;	//0x156

		uint8_t			m_NumberOfMasterFeats;	//0x158
		uint8_t			m_NumberOfClasses;		//0x159

		uint8_t			m_NumberOfRacialTypes;
		uint8_t			m_NumberOfRacialSubTypes;

		uint8_t			m_NumberOfSkills;		//0x15C

		char			Padding5[0x7];

		uint8_t			m_NumberOfDomains;		//0x164

		char			Padding5B[0x3];

		classes2DA		m_pClasses;				//0x168

		racialtypes2DA			m_pRacialTypes;			//0x16C
		racialSubtypes2DA		m_pRacialSubTypes;		//0x170

		skills2DA		m_pSkills;	//0x174
		uint8_t			Craft_Alch_Potns_Number;
		uint8_t			Crft_Alch_Distil_Number;	//0x179
		uint8_t			Crft_Alch_BrkDn_Number;		//0x17A
		uint8_t			Padding6b;

		uint32_t		Craft_Alch_Potns;		//0x17C
		uint32_t		Crft_Alch_Distil;		//0x180
		uint32_t		Crft_Alch_BrkDn;		//0x184

		uint8_t			m_NumberOfBackground;	//0x188
		uint8_t			Padding7;
		uint16_t		Padding7b;
		background2da	m_pBackground;

		uint8_t			Skill_Mounted_Number;	//0x190
		char			Padding8[0x3];

		uint32_t		Skill_Mounted;			//0x194
		domains2da		m_pDomains;
	};

}

NWN2DA::background2da GetBackgroundRow(uint32_t iBackgroundRow);

NWN2DA::domains2da GetDomainRow(uint32_t iDomainRow);

NWN2DA::racialtypes2DA GetRacialRow(uint32_t iRaceRow);

NWN2DA::racialSubtypes2DA GetSubRacialRow(uint32_t iSubRaceRow);

NWN2DA::classes2DA GetClasseRow(uint32_t iClassRow);

NWN2DA::feats2DA GetFeatRow(uint32_t iFeatRow);

NWN2DA::skills2DA GetSkillRow(uint32_t iSkill);

NWN2DA::spells2da GetSpellRow(uint32_t iSpell);

NWN2DA::access2da* GetGlobal2DA();

#endif

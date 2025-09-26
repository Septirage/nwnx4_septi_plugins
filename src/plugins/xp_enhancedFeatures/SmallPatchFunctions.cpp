
#include "SmallPatchFunctions.h"

#include <misc/Patch.h>
#include <misc/ini.h>
#include <misc/log.h>
#include <plugins/plugin.h>
#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include <hook/scriptManagement.h>
#include "../../septutil/NwN2DataPos.h"


#include <nwn2heap.h>

#include <unordered_set>


extern std::unique_ptr<LogNWNX> logger;

typedef void (__cdecl * NWN2Heap_Deallocate_Proc)(void *p);
extern NWN2Heap_Deallocate_Proc NWN2Heap_Deallocate;


#define OFFS_TUMBLEACFCT				0x005a5360
#define OFFS_TUMBLEPRECALL				0x005b57aa
#define OFFS_TUMBLECALL					0x005b57b9

#define OFFS_SETTRAPSYN1				0x005bbc23
#define OFFS_SETTRAPSYN2				0x005bc26a
#define OFFS_SETTRAPSYN3				0x005bcd22
#define OFFS_SETTRAPSYN4				0x005bd5e3
#define OFFS_DISTRAPSYN					0x005ccb2d
#define OFFS_TRAPSYN					0x005687ce

#define OFFS_SPELLCRAFTSAVE				0x00639546

#define OFFS_SaveThrowTestFumble		0x00639a3c
#define OFFS_SaveThrowIndomitableSoul	0x00639a9f 
#define OFFS_SaveThrowSlipperyMind		0x00639a6c

#define OFFS_GetFeatMaxUsages			0x0059f850

#define OFFS_RemoveDodgeACEffect		0x00659367

#define OFFS_GetEffectType				0x00656456


#define OFFS_RemoveSpellBonus			0x006dd5ea
#define OFFS_SpellNumberAbilityDec		0x005a4f34


#define OFFS_MonkSpecialWeaponTest		0x007514d3

#define OFFS_CallSplitCopyLVar			0x005d97b8

#define OFFS_CalcSafeLocationAreaS		0x006b435f
#define OFFS_CalcSafeLocationAreaSize	0x006c2211
#define OFFS_CalcSafeLocationAreaSize2  0x006c23af

#define OFFS_FixRetrieveCreaFeatUses1	0x005b329f
#define OFFS_FixRetrieveCreaFeatUses2	0x005b331e
#define OFFS_FixRetrieveCreaFeatUses3	0x005b3337
#define OFFS_FixRetrieveCreaFeatUses4	0x005b3429


#define OFFS_FixGetEffectType1			0x00656456
#define OFFS_FixGetEffectType2			0x0065647f

#define OFFS_PatchGetResRef				0x006a2997
#define OFFS_FixSetFirstName			0x00584cb8

#define OFFS_FixItmPrpDmgBonus			0x0067e5e9
#define OFFS_FixItmPrpDmgBonusVsAlign	0x0067e62a
#define OFFS_FixItmPrpDmgBonusVsRace	0x0067e66b
#define OFFS_FixItmPrpDmgBonusVsSAlign	0x0067e6ac

unsigned long EndOfFixItmPrpDmgBonus = 0x0067e5ee;
unsigned long EndOfFixItmPrpDmgBonusXXX = 0x0067e62f;


__declspec(naked) void FixItmPrpDmgBonus()
{
	__asm
	{
		MOV		EAX, dword ptr[ESP + 0x30]

		MOV 	ECX, EAX
		SAR 	ECX, 31
		AND 	ECX, 1
		MOV		dword ptr[ESP + 0x34], ECX

		AND		EAX, 0x7FFFFFFF

		MOV		dword ptr[ESP + 0x30], EAX

		PUSH	EAX

		jmp dword ptr[EndOfFixItmPrpDmgBonus]
	}
}


__declspec(naked) void FixItmPrpDmgBonusXXX()
{
	__asm
	{
		MOV		EAX, dword ptr[ESP + 0x34]

		MOV 	ECX, EAX
		SAR 	ECX, 31
		AND 	ECX, 1
		MOV		dword ptr[ESP + 0x38], ECX

		AND		EAX, 0x7FFFFFFF

		MOV		dword ptr[ESP + 0x34], EAX

		PUSH	EAX

		jmp dword ptr[EndOfFixItmPrpDmgBonusXXX]
	}
}


std::unordered_set<uint32_t> g_listMonkWeapon;


uint32_t __fastcall TestIfMonkSpecialWeapon(uint32_t uWeaponType)
{
	return g_listMonkWeapon.contains(uWeaponType);
}

__declspec(naked) void SetMonkSpecialWeapon()
{
	__asm
	{
		//EAX == weapon type
		PUSH	ECX
		PUSH	EDX

		MOV		ECX, EAX
		CALL	TestIfMonkSpecialWeapon

		POP		EDX
		POP		ECX

		RET
	}
}


//0x14 : KNOCK, 0x66 : SCALE 
GameObject* __fastcall GetObjectPtr(uint32_t ObjectId)
{
	GameObjectManager m_ObjectManager;

	return m_ObjectManager.GetGameObject( ObjectId );
}



unsigned long EndOfFixCalcSafeOk = 0x006b4364;
unsigned long EndOfFixCalcSafeKo = 0x006b43dd;

//6b435f FixCalcSafeLocationArea.
// 006b4364 EndOfFixCalcSafeOk
// 006b43dd EndOfFixCalcSafeKo
// OFFS_CalcSafeLocationAreaS
__declspec(naked) void FixCalcSafeLocationArea()
{
	__asm
	{
		//We need to get the areaID from lTestPosition
		//Get the bWalkStraighLineRequired. 
		//If True => oCreature.Area must be == to lTestPosition.Area
		//Else
		//AreaID from lTestPosition
		MOV		ECX, dword ptr [EBX+0x18]
		//AreaID from oCreature
		MOV		EAX, dword ptr [ESI + 0x144]

		//bWalkStraighLineRequired ?
		CMP		dword ptr [ESP + 0x18], 0x1
		JNE		ContinueCalcSafe

		//Yes, we need to be on same area
		CMP		ECX, EAX
		JNE		StopCalcSafe

	ContinueCalcSafe:
		CALL	GetObjectPtr

		CMP		EAX, 0
		JE		StopCalcSafe

		SUB		EAX, 0xad0

		jmp dword ptr[EndOfFixCalcSafeOk]
	StopCalcSafe:
		jmp dword ptr[EndOfFixCalcSafeKo]

	}
}

/*
752daf <=== MAJ du nombre de sort dispo


5a4e20 <=== MAJ max sort en desquipant objet spellBonusSlot
6dd5f2 <== MAK spellLeft en perdant spellBonusSlot

// /!\ one usage of DecrementRemainingSpellUsage (59b3e0 still not fully understanded)

5a4f91 <=== MAJ spell left Perte de carac 
*/

int g_iSpellLossFixChoice = 0;

unsigned long EndOfFixDecreaseSpellBS = 0x006dd5f7;
unsigned long BaseDecreaseSpellBS     = 0x006dd5f0;


//6dd5ea
__declspec(naked) void FixDecreaseOfRemainingSpellOnSpellBonusLoss()
{
	__asm
	{
		MOV ECX, dword ptr[EBP + 0x1fc4]

		CMP g_iSpellLossFixChoice, 0x0
		JE ApplyBaseGameCalculBS

		CMP g_iSpellLossFixChoice, 0x1
		JNE GoToEndOfFixBS

		PUSH EDX
		PUSH EAX

		IMUL EDX, EDI, 0x124
		ADD EDX, ESI
		MOV EAX, EDX
		MOVZX EDX, byte ptr[ECX + EDX + 0x194]

		MOVZX EAX, byte ptr[ECX + EAX + 0x222]

		CMP EAX, EDX
		POP EAX
		POP EDX

		JGE GoToEndOfFixBS

	ApplyBaseGameCalculBS:
		jmp dword ptr[BaseDecreaseSpellBS]


	GoToEndOfFixBS:
		jmp dword ptr[EndOfFixDecreaseSpellBS]
	}

}


unsigned long EndOfFixDecreaseSpellAb    = 0x005a4f3a;

// EBP = spelllvl
// EDI = ptrStat
//5a4f34
__declspec(naked) void FixDecreaseOfRemainingSpellOnAbilityLoss()
{
	__asm
	{
		MOV BL, AL

		CMP g_iSpellLossFixChoice, 0x0
		JE ApplyBaseGameCalcul

		CMP g_iSpellLossFixChoice, 0x1
		JNE GoToEndOfFix

		MOVZX EDX, byte ptr[ESP + 0x14]
		IMUL EDX, EDX, 0x124

		ADD EDX, EBP
		SUB BL,  byte ptr[EDI + 0x10C + EDX + 0x88]

		JMP GoToEndOfFix

	ApplyBaseGameCalcul:
		SUB BL, [ESP + 0x13]


	GoToEndOfFix:
		jmp dword ptr[EndOfFixDecreaseSpellAb]
	}
}



// remove DodgeAC_object 659367 (dont do anything if special AC)

unsigned long BackToRemoveACEffect = 0x0065936d;
unsigned long EndRemoveACEffect    = 0x006594aa;

//Check if we are on a special AC value (not the "magic number" 0x1007 / 4103
__declspec(naked) void FixDodgeVSPhysicalRemove()
{
	__asm
	{
		JNZ GoToEndRemoveACEFfect

		PUSH EAX
		MOV EAX, dword ptr[ESI + 0x40]
		MOV EAX, dword ptr[EAX + 0x14]

		CMP EAX, 0x1007
		POP EAX
		JNE GoToEndRemoveACEFfect

		JMP dword ptr[BackToRemoveACEffect]

	GoToEndRemoveACEffect:
		JMP dword ptr[EndRemoveACEffect]
	}

}




	unsigned long ReturnToTestNoWill = 0x00639a63;
	unsigned long ReturnToCalculation = 0x00639b6a;
	unsigned long ReturnToTestRetry = 0x00639abd;


 //
__declspec(naked) void SaveThrowTestIfFumble()
{
	__asm
	{
		CMP byte ptr[ESP + 0x38], 0x1; Is it FortitudeSaveThrow ?
		JNZ TestNoFortitude

		PUSH 0
		PUSH 1
		PUSH 0x87A

		MOV ECX, dword ptr[ESI + 0x1FC4]
		MOV EDX, dword ptr[ECX]
		MOV EAX, dword ptr[EDX + 0x14]
		CALL EAX							;do we have feat 0x87a (no auto failure)
		TEST EAX, EAX
		JZ	GoToTestNoWill
		JMP	dword ptr[ReturnToCalculation]


TestNoFortitude :
		CMP byte ptr[ESP + 0x38], 0x3; Is it WillSaveThrow ?
		JNZ GoToTestNoWill

		CMP BL, 0x1
		JZ  TestIndomitableSoul
		CMP BL, 0x4
		JNZ GoToTestNoWill ; we already know its not a mind effect, can stop here

TestIndomitableSoul :
		PUSH 0
		PUSH 1
		PUSH 0x879

		MOV ECX, dword ptr[ESI + 0x1FC4]
		MOV EDX, dword ptr[ECX]
		MOV EAX, dword ptr[EDX + 0x14]
		CALL EAX; do we have feat 0x879 ? Allow relaunch so no autofailure

		TEST EAX, EAX
		JNZ GoToTestRetry ; we are allowed to reroll

GoToTestNoWill:
		JMP	dword ptr[ReturnToTestNoWill]

GoToTestRetry:
		JMP dword ptr[ReturnToTestRetry]
	}
}




unsigned long ReturnToSaveThrowFailed = 0x00639a9f;
unsigned long ReturnToGoBackSaveThrow = 0x00639aa5;



// 639a9f Indomitable soul should workonly for Will save
__declspec(naked) void SaveThrowIndomitableSoul()
{
	__asm
	{
		CMP byte ptr[ESP + 0x38], 0x3; Is it WillSaveThrow ?
		JNZ GoToSaveThrowFailed ; No ? So IndomitableSoul don t work

		MOV ECX, dword ptr[ESI + 0x1FC4]

		JMP dword ptr[ReturnToGoBackSaveThrow]

GoToSaveThrowFailed:
		JMP dword ptr[ReturnToSaveThrowFailed]

	}
}




unsigned long ReturnToLastChanceSaveThrow = 0x00639a86;
unsigned long ReturnToGoBackSaveThrowSlippery = 0x00639a72;


//639a6c SlipperyMind should workonly for Will save
__declspec(naked) void SaveThrowSlipperyMind()
{
	__asm
	{
		CMP byte ptr[ESP + 0x38], 0x3; Is it WillSaveThrow ?
		JNZ GoToLastChanceSaveThrow ; No ? So SlipperyMind don t work

		MOV ECX, dword ptr[ESI + 0x1FC4]

		JMP dword ptr[ReturnToGoBackSaveThrowSlippery]

GoToLastChanceSaveThrow:
		JMP dword ptr[ReturnToLastChanceSaveThrow]
	}
}



unsigned long ContinueGetFeatMaxUsage = 0x0059f855;
unsigned long EndOfGetFeatMaxUsage = 0x0059fa6a;

unsigned int iFixNumberOfFeatUsage = 0;

//Correct (Extra) Wild Shape featUsage calculation 
__declspec(naked) void GetMaxFeatsUsageWildShape()
{
	__asm
	{
		CMP EAX, 0x131 //305 - wildshape
		JE	GoToApplyExtraWildShape
		CMP EAX, 0x14F // 335 wildshape x2
		JB	GoToContinueGetFeatMaxUsage
		CMP EAX, 0x153	// 339 wildshape x6
		JBE GoToApplyExtraWildShape 
		CMP EAX, 0x78b // 1931 wildshape 7
		JB GoToContinueGetFeatMaxUsage
		CMP EAX, 0x78D  // 1933 wildshape 9
		JBE GoToApplyExtraWildShape


	GoToContinueGetFeatMaxUsage:
		CMP iFixNumberOfFeatUsage, 0x1
		JNE	GoToGoBackGetFeatMaxUsage

		CMP EAX, 0x125
		JE  GoToApplyExtraRage
		CMP EAX, 0x146
		JB	GoToGoBackGetFeatMaxUsage
		CMP EAX, 0x14b
		JBE GoToApplyExtraRage
		CMP EAX, 0x5a7
		JB	GoToGoBackGetFeatMaxUsage
		CMP EAX, 0x5b0
		JBE GoToApplyExtraRage


	GoToGoBackGetFeatMaxUsage:
		CMP EAX, 0x57B
		JMP dword ptr[ContinueGetFeatMaxUsage]

		GoToApplyExtraRage :
		//Should not cause trouble, but just in case.. save eax/ecx
		PUSH EAX
		PUSH ECX

		//Test if ExtraRage is taken
		MOV EAX, dword ptr[ESI]
		MOV EAX, dword ptr[EAX + 0x14]
		PUSH 0x0
		PUSH 0x1
		PUSH 0x53D
		MOV ECX, ESI
		CALL EAX

		JMP TestApplicationExtraFeat
		

	GoToApplyExtraWildShape:
		//Should not cause trouble, but just in case.. save eax/ecx
		PUSH EAX
		PUSH ECX

		//Test if ExtraWildShape is taken
		MOV EAX, dword ptr [ESI]
		MOV EAX, dword ptr [EAX + 0x14]
		PUSH 0x0
		PUSH 0x1
		PUSH 0x53e
		MOV ECX, ESI
		CALL EAX
	TestApplicationExtraFeat:
		CMP EAX, 0x1
		//In both case.. Restore ecx, eax
		POP ECX
		POP EAX
		JNE EndWildShapeFix

		ADD BL, 0x2
EndWildShapeFix:
		JMP dword ptr[EndOfGetFeatMaxUsage]
	}
}
//53e - 1342 - Extra Wildshape 

//368/369 - 872/873 - Epic Wild Shape Undead/Dragon
// 7d1 - 2001 - EPIC WILD SHAPE MAGICAL BEAST
// 83f - 2111 - PLANT WILD SHAPE

int iNbFeats = 0;
int iFirstLoop = 1;

unsigned long StartRetrieveCreaFtUses = 0x005b32a7;
unsigned long LoopRetrieveCreaFtUses = 0x005b32b0;
unsigned long FLoopRetrieveCreaFtUses = 0x005b3325;
unsigned long NFLoopRetrieveCreaFtUses = 0x005b3337;
unsigned long SLoopRetrieveCreaFtUses = 0x005b333e;
unsigned long NSLoopRetrieveCreaFtUses = 0x005b341c;
unsigned long QLoopRetrieveCreaFtUses = 0x005b342f;

//unsigned long EndOfStartCreaFeatNb = 0x005b32a7;
//unsigned long EndOfEndCreaFeatNb = 0x005b32a7;

__declspec(naked) void FixRetrieveCreaFeatUses1()		//0x005b329f
{
	__asm
	{
		MOV iNbFeats, EAX
		MOV dword ptr [ESP + 0x20], 0x0

		jmp dword ptr[StartRetrieveCreaFtUses]		//0x005b32a7
	}
}



__declspec(naked) void FixRetrieveCreaFeatUses2()		//0x005b331e
{
	__asm
	{
		CMP		iFirstLoop, 0x1
		JNE		FxCreaFeatNbAvoidFBlock

		CMP		dword ptr[EAX + 0x80], 0x0
		jmp		dword ptr[FLoopRetrieveCreaFtUses]

FxCreaFeatNbAvoidFBlock:
		jmp		dword ptr[NFLoopRetrieveCreaFtUses]
	}
}

__declspec(naked) void FixRetrieveCreaFeatUses3()	//0x005b3337
{
	__asm
	{
		CMP		iFirstLoop, 0x0
		JNE		FxCreaFeatNbAvoidSBlock

		CMP		DI, word ptr[ECX + 0x154]
		jmp		dword ptr[SLoopRetrieveCreaFtUses]

FxCreaFeatNbAvoidSBlock:
		jmp		dword ptr[NSLoopRetrieveCreaFtUses]
	}
}

__declspec(naked) void FixRetrieveCreaFeatUses4()		// 0x005b3429
{
	__asm
	{
		JNZ		GoBackCodeCreaNbFt

		CMP		iFirstLoop, 0x1
		JE		FirstLoopEndCreaNbFt

		//End HERE
		MOV		iFirstLoop, 0x1
		jmp		dword ptr[QLoopRetrieveCreaFtUses]

FirstLoopEndCreaNbFt:
		// Loop again
		MOV		iFirstLoop, 0x0
		MOV		EAX, iNbFeats
		MOV		dword ptr [ESP + 0x20], 0x0
		jmp		dword ptr[StartRetrieveCreaFtUses]
GoBackCodeCreaNbFt:
		jmp		dword ptr[LoopRetrieveCreaFtUses]
	}
}

unsigned long GoBackCalcSafeLocAS = 0x006c2217;
unsigned long GoBackCalcSafeLocAS2 = 0x006c23b5;
float fCalcSafeSize = 5.0f;

//OFFS_CalcSafeLocationAreaSize ChangeCalculateSafeLocSize
__declspec(naked) void ApplyNewSizeForCalcSafeLoc()
{
	__asm
	{
		FLD	dword ptr [fCalcSafeSize]
		jmp dword ptr[GoBackCalcSafeLocAS]
	}
}

__declspec(naked) void ApplyNewSizeForCalcSafeLoc2()
{
	__asm
	{
		FLD	dword ptr [fCalcSafeSize]
		jmp dword ptr[GoBackCalcSafeLocAS2]
	}
}

unsigned long GoBackGetEffectTypeP1 = 0x0065645c;
unsigned long GoBackGetEffectTypeP2 = 0x00656485;


uint32_t EFFECT_TYPE_CUTSCENEDOMINATED = 111;
uint32_t EFFECT_TYPE_DEATH = 112;
uint32_t EFFECT_TYPE_KNOCKDOWN = 113;
uint32_t EFFECT_TYPE_DAMAGE = 114;
uint32_t EFFECT_TYPE_HEAL = 115;
uint32_t EFFECT_TYPE_LINKEFFECT = 116;
uint32_t EFFECT_TYPE_MODIFYATTACK = 117;
uint32_t EFFECT_TYPE_LOWLIGHTVISION = 118;
uint32_t EFFECT_TYPE_DARKVISION = 119;
uint32_t EFFECT_TYPE_DISAPPEAR = 120;
uint32_t EFFECT_TYPE_APPEAR = 121;
uint32_t EFFECT_TYPE_SETSCALE = 122;
uint32_t EFFECT_TYPE_SEETRUEHPS = 123;
uint32_t EFFECT_TYPE_BABMINIMUM = 124;
uint32_t EFFECT_TYPE_SUMMONCOPY = 125;
uint32_t EFFECT_TYPE_SUMMONCREATURE = 126;




__declspec(naked) void FixGetEffectTypePart1()
{
	__asm
	{
		CMP EDX, dword ptr ds:[0x00805c08]		//UID_EFFECT_RESURECTION 0x4
		JNE FixGetEffectTypePart1_1
		MOV EAX, 0xE				//EFFECT_TYPE_RESURRECTION already exist
		RET

	FixGetEffectTypePart1_1:
		CMP EDX, dword ptr ds:[0x00805c10]		//UID_EFFECT_SUMMONCREATURE 0x6
		JNE FixGetEffectTypePart1_2
		MOV EAX, EFFECT_TYPE_SUMMONCREATURE
		RET

	FixGetEffectTypePart1_2:
		CMP EDX, dword ptr ds:[0x00805c44]		//UID_EFFECT_DEATH 0x13
		JNE FixGetEffectTypePart1_3
		MOV EAX, EFFECT_TYPE_DEATH
		RET

	FixGetEffectTypePart1_3:
		CMP EDX, dword ptr ds:[0x00805c48]		//UID_EFFECT_KNOCKDOWN
		JNE FixGetEffectTypePart1_4
		MOV EAX, EFFECT_TYPE_KNOCKDOWN
		RET

	FixGetEffectTypePart1_4:
		CMP EDX, dword ptr ds:[0x00805c90]		//UID_EFFECT_DAMAGE 0x26
		JNE FixGetEffectTypePart1_5
		MOV EAX, EFFECT_TYPE_DAMAGE
		RET

	FixGetEffectTypePart1_5:
		CMP EDX, dword ptr ds:[0x00805c94]		//UID_EFFECT_HEAL	0x27
		JNE FixGetEffectTypePart1_6
		MOV EAX, EFFECT_TYPE_HEAL
		RET

	FixGetEffectTypePart1_6:
		CMP EDX, dword ptr ds:[0x00805c98]		//UID_EFFECT_LINKEFFECT	0x28
		JNE FixGetEffectTypePart1_7
		MOV EAX, EFFECT_TYPE_LINKEFFECT
		RET

	FixGetEffectTypePart1_7:
		CMP EDX, dword ptr ds:[0x00805ca4]		//UID_EFFECT_MODIFYATTACK 0x2C
		JNE FixGetEffectTypePart1_8
		MOV EAX, EFFECT_TYPE_MODIFYATTACK
		RET

	FixGetEffectTypePart1_8:
		CMP EDX, dword ptr ds:[0x00805d00]		//UID_EFFECT_LOWLIGHT_DARK_VISION 0x45
		JNE FixGetEffectTypePart1_9

		MOV ECX, dword ptr [ECX + 0x40]
		MOV ECX, dword ptr [ECX]
		CMP ECX, 0x1
		JNE FixGetEffectTypePart1_8b
		MOV EAX, EFFECT_TYPE_LOWLIGHTVISION
		RET
	FixGetEffectTypePart1_8b:
		CMP ECX, 0x2
		JNE FixGetEffectTypePart1_8c
		MOV EAX, EFFECT_TYPE_DARKVISION
	FixGetEffectTypePart1_8c:
		RET							//Error


	FixGetEffectTypePart1_9:
		CMP EDX, dword ptr ds:[0x00805d2c]		//UID_EFFECT_DISAPPEAR 0x50
		JNE FixGetEffectTypePart1_10
		MOV EAX, EFFECT_TYPE_DISAPPEAR
		RET

	FixGetEffectTypePart1_10:
		CMP EDX, dword ptr ds:[0x00805d30]		//UID_EFFECT_APPEAR 0x51
		JNE FixGetEffectTypePart1_11
		MOV EAX, EFFECT_TYPE_APPEAR
		RET

	FixGetEffectTypePart1_11:
		CMP EDX, dword ptr ds:[0x00805d84]		//UID_EFFECT_SETSCALE 0x66
		JNE FixGetEffectTypePart1_12
		MOV EAX, EFFECT_TYPE_SETSCALE
		RET

	FixGetEffectTypePart1_12:
		CMP EDX, dword ptr ds:[0x00805d90]		//UID_EFFECT_SEETRUEHPS 0x69
		JNE FixGetEffectTypePart1_13
		MOV EAX, EFFECT_TYPE_SEETRUEHPS
		RET

	FixGetEffectTypePart1_13:
		CMP EDX, dword ptr ds:[0x00805dc8]		//UID_EFFECT_BABMINIMUM 0x77
		JNE FixGetEffectTypePart1_14
		MOV EAX, EFFECT_TYPE_BABMINIMUM
		RET

	FixGetEffectTypePart1_14:
		CMP EDX, dword ptr ds:[0x00805de4]		//UID_EFFECT_SUMMONCOPY 0x7E
		JNE FixGetEffectTypePart1_15
		MOV EAX, EFFECT_TYPE_SUMMONCOPY
		RET


	FixGetEffectTypePart1_15:
		CMP EDX, dword ptr ds:[0x00805C74]		//UID_EFFECT_AREA_OF_EFFECT
		JMP	dword ptr[GoBackGetEffectTypeP1]
	}
}

__declspec(naked) void FixGetEffectTypePart2() //For cutscenedominated 
{
	__asm
	{
		ADD ECX, -0x1
		CMP ECX, 0x14
		JNE FixGetEffectTypePart2_1
		
		MOV EAX, EFFECT_TYPE_SUMMONCOPY
		RET

	FixGetEffectTypePart2_1:
		CMP ECX, 0x13
		JMP dword ptr[GoBackGetEffectTypeP2]
	}
}

unsigned long GoBackGetResRef = 0x006a299c;

void __fastcall CopyAreaResRefToNWString(char* pAreaBaseObject, NWN::CExoString* cExoStringResult)
{
	char* pAreaResRef = (pAreaBaseObject - 0x24);
	int iSize = 0;

	char* pCurChar = pAreaResRef;
	while (pAreaResRef[iSize] != '\0' && iSize < 0x20)
	{
		iSize++;
	}

	if (cExoStringResult->m_sString != nullptr && ( (cExoStringResult->m_nBufferLength < (iSize + 1)) || (iSize == 0)) )
	{
		if (NWN2Heap_Deallocate)
			NWN2Heap_Deallocate(cExoStringResult->m_sString);
		cExoStringResult->m_nBufferLength = 0;
		cExoStringResult->m_sString = nullptr;
	}

	if (iSize > 0)
	{
		if (cExoStringResult->m_nBufferLength == 0)
		{
			NWN2_HeapMgr *pHeapMgr = NWN2_HeapMgr::Instance();
			NWN2_Heap *pHeap = pHeapMgr->GetDefaultHeap();
			cExoStringResult->m_sString = (char*)pHeap->Allocate(iSize+1);
		}

		int i = 0;
		while (i < iSize)
		{
			cExoStringResult->m_sString[i] = pAreaResRef[i];
			i++;
		}
		cExoStringResult->m_sString[iSize] = '\0';
	}

	return;
}

__declspec(naked) void EnhanceGetResRef()
{
	__asm
	{
		//ECX => osef
		//EDX => listfct
		//EAX => ptr obj

		MOV ECX, EAX

		//here we are in the state we want to use after: EDX => lstfct, EAX => ptrobj

		MOVZX EAX, byte ptr[ECX + 0xA4]
		CMP EAX, 4
		JNE PrepareLeaveGetResRef

		//Ok, area mode here. Prepare the call

		//Get PTR StringResult
		LEA EAX, [ESP]

		//Save those
		PUSH EDX
		PUSH ECX

		//Second parameter
		MOV EDX, EAX
		CALL CopyAreaResRefToNWString

		POP ECX
		POP EDX

		PrepareLeaveGetResRef :
		MOV EAX, dword ptr[EDX + 0x3c]
			JMP dword ptr[GoBackGetResRef]
	}
}

unsigned long GoBackSetFirstName = 0x00584cbe;

__declspec(naked) void FixSetFirstNameCode()
{
	__asm
	{
		PUSH ESI
		MOV ECX, ESI
		MOV ESI, [ESI]
		MOV ESI, [ESI + 0x148]
		CALL ESI

		POP ESI
		MOV ECX, EAX

		JMP dword ptr[GoBackSetFirstName]
	}
}

Patch _FixSetFirstName[] =
{
	Patch(OFFS_FixSetFirstName, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6),
	Patch(OFFS_FixSetFirstName + 1, (relativefunc)FixSetFirstNameCode),

	Patch(),
};

Patch* FixSetFirstName = _FixSetFirstName;


Patch _PatchGetResRef[] =
{
	Patch(OFFS_PatchGetResRef, (char*)"\xe9", (int)1),
	Patch(OFFS_PatchGetResRef + 1, (relativefunc)EnhanceGetResRef),

	Patch(),
};

Patch* PatchGetResRef = _PatchGetResRef;


Patch _FixGetEffectType[] =
{
	Patch(OFFS_FixGetEffectType1, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6),
	Patch(OFFS_FixGetEffectType1 + 1, (relativefunc)FixGetEffectTypePart1),

	Patch(OFFS_FixGetEffectType2, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6),
	Patch(OFFS_FixGetEffectType2 + 1, (relativefunc)FixGetEffectTypePart2),

	Patch()
};

Patch* FixGetEffectType = _FixGetEffectType;


//6c23b5 return
Patch _ChangeCalculateSafeLocSize[] =
{
	Patch(OFFS_CalcSafeLocationAreaSize, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6),
	Patch(OFFS_CalcSafeLocationAreaSize + 1, (relativefunc)ApplyNewSizeForCalcSafeLoc),

	Patch(OFFS_CalcSafeLocationAreaSize2, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6),
	Patch(OFFS_CalcSafeLocationAreaSize2 + 1, (relativefunc)ApplyNewSizeForCalcSafeLoc2),

	Patch()
};

Patch* ChangeCalculateSafeLocSize = _ChangeCalculateSafeLocSize;


Patch _FixRetrieveCreaFeatUses[] =
{

	Patch(OFFS_FixRetrieveCreaFeatUses1, (char*)"\xe9\x00\x00\x00\x00\x90\x90\x90", (int)8),
	Patch(OFFS_FixRetrieveCreaFeatUses1 + 1, (relativefunc)FixRetrieveCreaFeatUses1),

	Patch(OFFS_FixRetrieveCreaFeatUses2, (char*)"\xe9\x00\x00\x00\x00\x90\x90", (int)7),
	Patch(OFFS_FixRetrieveCreaFeatUses2 + 1, (relativefunc)FixRetrieveCreaFeatUses2),

	Patch(OFFS_FixRetrieveCreaFeatUses3, (char*)"\xe9\x00\x00\x00\x00\x90\x90", (int)7),
	Patch(OFFS_FixRetrieveCreaFeatUses3 + 1, (relativefunc)FixRetrieveCreaFeatUses3),

	Patch(OFFS_FixRetrieveCreaFeatUses4, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6),
	Patch(OFFS_FixRetrieveCreaFeatUses4 + 1, (relativefunc)FixRetrieveCreaFeatUses4),

	Patch()
};

Patch* FixRetrieveCreaFeatUses = _FixRetrieveCreaFeatUses;


Patch _FixCalcSafeLocation[] =
{
	Patch(OFFS_CalcSafeLocationAreaS, (char*)"\xe9\x00\x00\x00\x00", (int)5),
	Patch(OFFS_CalcSafeLocationAreaS + 1, (relativefunc)FixCalcSafeLocationArea),

	Patch()
};

Patch* FixCalcSafeLocation = _FixCalcSafeLocation;

Patch _FixRemainingSpellOnBonusLoss[] =
{
	Patch(OFFS_RemoveSpellBonus, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6),
	Patch(OFFS_RemoveSpellBonus + 1, (relativefunc)FixDecreaseOfRemainingSpellOnSpellBonusLoss),

	Patch(OFFS_SpellNumberAbilityDec, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6),
	Patch(OFFS_SpellNumberAbilityDec + 1, (relativefunc)FixDecreaseOfRemainingSpellOnAbilityLoss),

	Patch()
};

Patch* FixRemainingSpellOnBonusLoss = _FixRemainingSpellOnBonusLoss;



Patch _FixDodgeVSPhysicalRemovePatch[] =
{
	Patch(OFFS_RemoveDodgeACEffect, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6),
	Patch(OFFS_RemoveDodgeACEffect + 1, (relativefunc)FixDodgeVSPhysicalRemove),

	Patch()
};

Patch* FixDodgeVSPhysicalRemovePatch = _FixDodgeVSPhysicalRemovePatch;



Patch _FixWildShapePatch[] =
{
	Patch(OFFS_GetFeatMaxUsages, (char*)"\xe9\x00\x00\x00\x00", (int)5),
	Patch(OFFS_GetFeatMaxUsages + 1, (relativefunc)GetMaxFeatsUsageWildShape),

	Patch()
};

Patch* FixWildShapePatch = _FixWildShapePatch;


Patch _FixSaveThrowPatch[] =
{
	Patch(OFFS_SaveThrowTestFumble, (char*)"\xe9\x00\x00\x00\x00", (int)5), 
	Patch(OFFS_SaveThrowTestFumble + 1, (relativefunc)SaveThrowTestIfFumble),


	Patch(OFFS_SaveThrowIndomitableSoul, (char*)"\xe9\x00\x00\x00\x00", (int)5), 
	Patch(OFFS_SaveThrowIndomitableSoul + 1, (relativefunc)SaveThrowIndomitableSoul),

	Patch(OFFS_SaveThrowSlipperyMind, (char*)"\xe9\x00\x00\x00\x00", (int)5), 
	Patch(OFFS_SaveThrowSlipperyMind + 1, (relativefunc)SaveThrowSlipperyMind),

	Patch()
};

Patch* FixSaveThrowPatch = _FixSaveThrowPatch;

Patch _PatchMonkWeaponList[] =
{
	Patch(OFFS_MonkSpecialWeaponTest, (char*)"\xe9\x00\x00\x00\x00", (int)5),
	Patch(OFFS_MonkSpecialWeaponTest +1, (relativefunc)SetMonkSpecialWeapon),

	Patch()
};
Patch* PatchMonkWeaponList = _PatchMonkWeaponList;


Patch _PatchFixItmPrpDmgBonus[] =
{
	Patch(OFFS_FixItmPrpDmgBonus, (char*)"\xe9\x00\x00\x00\x00", (int)5),
	Patch(OFFS_FixItmPrpDmgBonus + 1, (relativefunc)FixItmPrpDmgBonus),
	Patch(OFFS_FixItmPrpDmgBonusVsAlign, (char*)"\xe9\x00\x00\x00\x00", (int)5),
	Patch(OFFS_FixItmPrpDmgBonusVsAlign + 1, (relativefunc)FixItmPrpDmgBonusXXX),
	Patch(OFFS_FixItmPrpDmgBonusVsRace, (char*)"\xe9\x00\x00\x00\x00", (int)5),
	Patch(OFFS_FixItmPrpDmgBonusVsRace + 1, (relativefunc)FixItmPrpDmgBonusXXX),
	Patch(OFFS_FixItmPrpDmgBonusVsSAlign, (char*)"\xe9\x00\x00\x00\x00", (int)5),
	Patch(OFFS_FixItmPrpDmgBonusVsSAlign + 1, (relativefunc)FixItmPrpDmgBonusXXX),

	Patch()
};
Patch* PatchFixItmPrpDmgBonus = _PatchFixItmPrpDmgBonus;


Patch _DisableTumbleACPatch[] =
{
	Patch(OFFS_TUMBLEACFCT, (char*)"\x31\xC0\xC3", (int)3), //XOR EAX,EAX   RET 
	Patch(OFFS_TUMBLEPRECALL, (char*)"\x32\xC0\xEB\x32\x90", (int)5), //XOR AL,AL, JMP  

	Patch()
};

Patch *DisableTumbleACPatch = _DisableTumbleACPatch;

Patch _DisableTrapSynergy[] =
{
	Patch(OFFS_SETTRAPSYN1, (char*)"\x32\xC0\x83\xC4\x0C", (int)5),		//XOR AL, AL NOP NOP NOP
	Patch(OFFS_SETTRAPSYN2, (char*)"\x32\xC0\x83\xC4\x0C", (int)5),		//XOR AL, AL NOP NOP NOP
	Patch(OFFS_SETTRAPSYN3, (char*)"\x32\xC0\x83\xC4\x0C", (int)5),		//XOR AL, AL NOP NOP NOP
	Patch(OFFS_SETTRAPSYN4, (char*)"\x32\xC0\x83\xC4\x0C", (int)5),		//XOR AL, AL NOP NOP NOP
	Patch(OFFS_DISTRAPSYN, (char*)"\x32\xC0\x83\xC4\x0C", (int)5),		//XOR AL, AL NOP NOP NOP
	Patch(OFFS_TRAPSYN, (char*)"\xEB\x23", (int)2),		//JMP 0x23


	Patch()
};
Patch* DisableTrapSynergy = _DisableTrapSynergy;

Patch _DisablespellcraftSavePatch[] =
{
	Patch(OFFS_SPELLCRAFTSAVE, (char*)"\xeb\x15\x90", (int)3), //JMP 0x15, NOP

	Patch()
};

Patch *DisablespellcraftSavePatch = _DisablespellcraftSavePatch;

Patch _KeepLocalVarOnSplitPatch[] =
{
	Patch(OFFS_CallSplitCopyLVar, (char*)"\x6a\x01", (int)2), //PUSH 01

	Patch()
};
Patch* KeepLocalVarOnSplitPatch = _KeepLocalVarOnSplitPatch;


bool SmallPatchFunctions(SimpleIniConfig* config)
{
	int iTest = 0;
	int i = 0;

	bool bWildShape = false;

	config->Read("KeepLocalVarOnSplit", &iTest, 0);
	if (iTest == 1)
	{
		i = 0;
		logger->Info("* LocalVar keept on Split");
		while (KeepLocalVarOnSplitPatch[i].Apply()) {
			i++;
		}
	}

	config->Read("DisableTumbleAC", &iTest, 0);
	if (iTest == 1)
	{
		logger->Info("* AC bonus by Tumble Disabled");
		i = 0;
		while (DisableTumbleACPatch[i].Apply()) {
			i++;
		}
	}

	config->Read("DisableTrapSynergy", &iTest, 0);
	if (iTest == 1)
	{
		logger->Info("* SetTrap, DisableTrap Synergy Disabled");
		i = 0;
		while (DisableTrapSynergy[i].Apply()) {
			i++;
		}
	}

	config->Read("DisablespellcraftSave", &iTest, 0);
	if (iTest == 1)
	{
		i = 0;
		logger->Info("* SpellSave bonus by SpellCraft Disabled");
		while (DisablespellcraftSavePatch[i].Apply()) {
			i++;
		}
	}

	config->Read("FixSaveThrow", &iTest, 0);
	if (iTest == 1)
	{
		i = 0;
		logger->Info("* SaveThrow bugfix Enabled");
		while (FixSaveThrowPatch[i].Apply())
		{
			i++;
		}
	}

	config->Read("FixWildShapeUsage", &iTest, 0);
	if (iTest == 1)
	{
		bWildShape = true;
		i = 0;
		logger->Info("* Wildshape usage bugfix Enabled");
		while (FixWildShapePatch[i].Apply())
		{
			i++;
		}		
	}

	config->Read("FixDodgeVSDamageTypeRemoval", &iTest, 0);
	if (iTest == 1)
	{
		i = 0;
		logger->Info("* DodgeVSDamageType removal bugfix Enabled");
		while (FixDodgeVSPhysicalRemovePatch[i].Apply())
		{
			i++;
		}
	}

	config->Read("FixDecreaseSpellOnBonusLoss", &iTest, 0);
	if (iTest > 0)
	{
		i = 0;
		logger->Info("* Decrease Spell on bonus loss behavior : %d", iTest);
		g_iSpellLossFixChoice = iTest;
		while (FixRemainingSpellOnBonusLoss[i].Apply())
		{
			i++;
		}
	}

	config->Read("FixCalcSafeLocation", &iTest, 0);
	if (iTest == 1)
	{
		i = 0;
		logger->Info("* Correctly manage Area of lTestPosition");
		while (FixCalcSafeLocation[i].Apply())
		{
			i++;
		}
	}

	config->Read("MaxCalcSafeLocation", &fCalcSafeSize, 5.0f);
	if (fabs(fCalcSafeSize - 5.0) > 0.01)
	{
		i = 0;
		//fCalcSafeSize = iTest;
		logger->Info("* Change Size of CalcSafeLocation to %.1f", fCalcSafeSize);
		while (ChangeCalculateSafeLocSize[i].Apply())
		{
			i++;
		}
	}

	config->Read("FixNumberOfFeatUsages", &iTest, 1);
	if(iTest == 1)
	{
		logger->Info("* Login FeatUsages number bugfix Enabled");
		iFixNumberOfFeatUsage = 1;
		i = 0;
		if (!bWildShape)
		{
			logger->Info("* Wildshape usage bugfix Enabled");
			while (FixWildShapePatch[i].Apply())
			{
				i++;
			}
		}

		i = 0;
		while (FixRetrieveCreaFeatUses[i].Apply())
		{
			i++;
		}
	}

	config->Read("FixGetEffectType", &iTest, 0);
	if (iTest == 1)
	{
		logger->Info("* GetEffectType bugfix Enabled");
		i = 0;
		while (FixGetEffectType[i].Apply())
		{
			i++;
		}
	}

	config->Read("EnhanceGetResRef", &iTest, 0);
	if (iTest == 1)
	{
		logger->Info("* Patch GetResRef");
		i = 0;
		while (PatchGetResRef[i].Apply())
		{
			i++;
		}
	}

	config->Read("FixSetFirstName", &iTest, 0);
	if (iTest == 1)
	{
		logger->Info("* Fix SetFirstName");
		i = 0;
		while (FixSetFirstName[i].Apply())
		{
			i++;
		}
	}

	config->Read("FixItmPrpDmgBonus", &iTest, 0);
	if (iTest == 1)
	{
		logger->Info("* Fix ItemPropertyDamageBonusXXX");
		i = 0;
		while (PatchFixItmPrpDmgBonus[i].Apply())
		{
			i++;
		}			
	}

	std::string sList = "";
	config->Read("MonkWeaponList", &sList, std::string(""));
	if(sList != "")
	{
		std::istringstream iss(sList);
		uint32_t number;

		while (iss >> number) {
			g_listMonkWeapon.insert(number);
		}
		logger->Info("* New list of MonkWeapon : %s", sList.c_str());

		//default list : 36 40 50 59 78

		i = 0;
		while (PatchMonkWeaponList[i].Apply())
		{
			i++;
		}
	}

	return true;
}
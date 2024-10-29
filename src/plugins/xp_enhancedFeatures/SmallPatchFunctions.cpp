
#include "SmallPatchFunctions.h"

#include "..\..\misc\Patch.h"
#include "../../misc/ini.h"
#include "../../misc/log.h"
#include "../plugin.h"
#include "../../NWN2Lib/NWN2.h"
#include "../../NWN2Lib/NWN2Common.h"
#include "../../septutil/NwN2DataPos.h"
#include "../../hook/scriptManagement.h"
#include <unordered_set>


extern std::unique_ptr<LogNWNX> logger;

#define OFFS_TUMBLEACFCT				0x005a5360
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

//Correct (Extra) Wild Shape featUsage calculation 
__declspec(naked) void GetMaxFeatsUsageWildShape()
{
	__asm
	{
		CMP EAX, 0x131
		JE	GoToApplyExtraWildShape
		CMP EAX, 0x14F
		JB	GoToContinueGetFeatMaxUsage
		CMP EAX, 0x153
		JBE GoToApplyExtraWildShape
		CMP EAX, 0x78b
		JB GoToContinueGetFeatMaxUsage
		CMP EAX, 0x78D
		JBE GoToApplyExtraWildShape

GoToContinueGetFeatMaxUsage:
		CMP EAX, 0x57B
		JMP dword ptr[ContinueGetFeatMaxUsage]
GoToApplyExtraWildShape:
		ADD BL, 0x2
		JMP dword ptr[EndOfGetFeatMaxUsage]
	}
}

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



Patch _DisableTumbleACPatch[] =
{
	Patch(OFFS_TUMBLEACFCT, (char*)"\x31\xC0\xC3", (int)3), //XOR EAX,EAX   RET

	Patch()
};

Patch *DisableTumbleACPatch = _DisableTumbleACPatch;



Patch _DisablespellcraftSavePatch[] =
{
	Patch(OFFS_SPELLCRAFTSAVE, (char*)"\xeb\x15\x90", (int)3), //JMP 0x15, NOP

	Patch()
};

Patch *DisablespellcraftSavePatch = _DisablespellcraftSavePatch;


bool SmallPatchFunctions(SimpleIniConfig* config)
{
	int iTest = 0;
	int i = 0;

	config->Read("DisableTumbleAC", &iTest, 0);
	if (iTest == 1)
	{
		logger->Info("* AC bonus by Tumble Disabled");
		while (DisableTumbleACPatch[i].Apply()) {
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
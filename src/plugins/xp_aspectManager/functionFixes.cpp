//Should probably be removed and put on EnhancedFix&Feature plugin


#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include <misc\Patch.h>
#include "messageManagement.h"

namespace {
	#define OFF_GetScaleHook	0x006AA61E
	#define OFF_SetScaleHook	0x006AA51C

	unsigned long RetfixGetScaleIsDoor = 0x006AA636;
	unsigned long RetfixGetScaleIsNotDoor = 0x006AA624;
	unsigned long RetfixSetScaleIsDoor = 0x006AA534;
	unsigned long RetfixSetScaleIsNotDoor = 0x006AA522;

	//0x006AA61E
	__declspec(naked) void FixGetScale()
	{
		__asm
		{
			MOV		AL, [ESI + 0xA4]
			CMP		AL, 0xA
			JNE		GetScaleNotDoor

			MOV		EAX, ESI
			JMP		dword ptr[RetfixGetScaleIsDoor]

			GetScaleNotDoor:
			MOV		EAX, dword ptr[EDX + 0x8C]
				JMP		dword ptr[RetfixGetScaleIsNotDoor]
		}

	}


	//0x006AA51C
	__declspec(naked) void FixSetScale()
	{
		__asm
		{
			MOV		AL, [ESI + 0xA4]
			CMP		AL, 0xA
			JNE		SetScaleNotDoor

			MOV		EAX, ESI
			JMP		dword ptr[RetfixSetScaleIsDoor]

			SetScaleNotDoor:
			MOV		EAX, dword ptr[EDX + 0x8C]
				JMP		dword ptr[RetfixSetScaleIsNotDoor]
		}

	}


	Patch _quickPatches[] =
	{
		//Patch on getScale, to allow door too
		Patch((DWORD)OFF_GetScaleHook, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6), //JMP NOP
		Patch(OFF_GetScaleHook + 1, (relativefunc)FixGetScale),

		//Patch on setScale, to allow door too
		Patch((DWORD)OFF_SetScaleHook, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6), //JMP NOP
		Patch(OFF_SetScaleHook + 1, (relativefunc)FixSetScale),

		Patch()
	};

	Patch* quickPatches = _quickPatches;


	#define OFF_SetIconFix	0x006ABCB6

	unsigned long RetfixSetIcon = 0x006ABCBC;
	//0x006ABCB6
	__declspec(naked) void FixSetIcon()
	{
		__asm
		{
			MOV		ECX, EAX
			MOV		EDX, EDI

			CALL	SendIconUpdateMessage


			JMP		dword ptr[RetfixSetIcon]
		}
	}

	Patch _setIconPatch[] =
	{
		//Patch on getScale, to allow door too
		Patch((DWORD)OFF_SetIconFix, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6), //JMP NOP
		Patch(OFF_SetIconFix + 1, (relativefunc)FixSetIcon),

		Patch()
	};

	Patch* setIconPatch = _setIconPatch;

}

void initAspectManagerFctFixes() {
	int i = 0;
	while(quickPatches[i].Apply()) {
		i++;
	}
}

void initSetIconFix() {
	int i = 0;
	while (setIconPatch[i].Apply()) {
		i++;
	}
}

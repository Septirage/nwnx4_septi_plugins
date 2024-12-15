#include "aspectManager.h"
#include "messageManagement.h"
#include "refreshObject.h"
#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include <misc/Patch.h>
#include <bit>
#include <cassert>
#include <charconv>
#include <detours/detours.h>
#include <optional>
#include <string_view>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <locale>
#include "../../septutil/NwN2DataPos.h"

__declspec(naked) void __fastcall RemoveFromArea_Intern(__in void* ptrArea, __in void* Unused, __in void* ptrObject)
{
	__asm
	{
		mov             edx, FCT_REMOVEFROMAREA;
		jmp             edx;
	}
}

__declspec(naked) void __fastcall AddToArea_Intern(__in void* ptrArea, __in void* Unused, __in void* ptrObject)
{
	__asm
	{
		mov             edx, FCT_ADDTOAREA;
		jmp             edx;
	}
}

/*
#define FCT_VISUALANDCOLPOSITION 0x0057f360
__declspec(naked) void __fastcall VisualAndCollisionPosition(__in void* ptrObjet, __in void* Unused, __in float* arrayPositionS, __in int param_3)
{
__asm
{
mov             edx, FCT_VISUALANDCOLPOSITION;
jmp             edx;
}
}
*/

__declspec(naked) void __fastcall ColisionLoSModelName_Intern(__in void* ptrObjet)
{
	__asm
	{
		mov             edx, FCT_COLISIONANDLOS;
		jmp             edx;
	}
}

__declspec(naked) void __fastcall ColisionLosModelNameDoor_Intern(__in void* ptrObjet)
{
	__asm
	{
		mov             edx, FCT_COLISIONANDLOSDOOR;
		jmp             edx;
	}
}

void RemoveFromArea(void* ptrArea, void* ptrObject)
{
	RemoveFromArea_Intern(ptrArea, NULL, ptrObject);
}


void AddToArea(void* ptrArea, void* ptrObject)
{
	AddToArea_Intern(ptrArea, NULL, ptrObject);
}

void ColisionLoSModelName(void* ptrObject)
{
	uint8_t objectType = ((uint8_t*)ptrObject)[0xA4];
	if (objectType == 0xA)
		ColisionLosModelNameDoor_Intern(ptrObject);
	else if(objectType == 0x9)
		ColisionLoSModelName_Intern(ptrObject);
}


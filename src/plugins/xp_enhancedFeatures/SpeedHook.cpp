#include "EnhancedFeatures.h"
#include "SpeedHook.h"
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





float fSpeedFeatValueHooked			= 1.0f;

std::unordered_map<int, std::string> mySpeedFeatScripts;
float fSpeedFeatValue2248			= 1.1f;
float fSpeedFeatValue194			= 1.1f;
float fSpeedFeatValue1337			= 1.05f;
float fSpeedFeatValue1806			= 1.1f;
float fSpeedFeatValue2120			= 1.2f;
float fSpeedFeatValue200			= 1.1f;
float fSpeedFeatValueMin			= 0.125f;
float fSpeedFeatValueMax			= 1.5f;
float fSpeedFeatValueMonkMax		= 2.0f;



#define OFFS_EntrySpeedFeatValue2248		0x005E7E29
#define OFFS_EntrySpeedFeatValue194			0x005E7E51
#define OFFS_EntrySpeedFeatValue1337		0x005E7F3F
#define OFFS_EntrySpeedFeatValue1806		0x005E7F67
#define OFFS_EntrySpeedFeatValue2120		0x005E7F8F
#define OFFS_EntrySpeedFeatValue200			0x005E7FFF
#define OFFS_EntrySpeedFeatValueMin			0x005E800F
#define OFFS_EntrySpeedFeatValueMax			0x005E8043
#define OFFS_EntrySpeedFeatValueMonkMax		0x005E802C

#define OFFS_EntrySpeedFeatTest200			0x005E7FF5
#define OFFS_EntrySpeedFeatEndOf200			0x005E8005
#define OFFS_EntrySpeedFeatHookNewFeat		0x005E7E33


unsigned long ReturnSpeedFeatValue2248		= 0x005E7E2F;
unsigned long ReturnSpeedFeatValue194		= 0x005E7E57;
unsigned long ReturnSpeedFeatValue1337		= 0x005E7F45;
unsigned long ReturnSpeedFeatValue1806		= 0x005E7F6D;
unsigned long ReturnSpeedFeatValue2120		= 0x005E7F95;
unsigned long ReturnSpeedFeatValue200		= 0x005E8005;
unsigned long ReturnSpeedFeatValueMin		= 0x005E8015;
unsigned long ReturnSpeedFeatValueMax		= 0x005E8049;
unsigned long ReturnSpeedFeatValueMonkMax	= 0x005E8032;


unsigned long ReturnSpeedFeatTest200Ok		= 0x005E7FFB;
unsigned long ReturnSpeedFeatTest200NOk		= 0x005E800B;
unsigned long ReturnSpeedFeatEndOf200		= 0x005E800B;
unsigned long ReturnSpeedFeatHookNewFeat	= 0x005E7E39;

#define OFFS_SPEED_FEAT_HOOK_BEFOREEVERYTHING 0x005e7e0b

unsigned long ReturnSpeedFeatBeforeEnd		= 0x005E800F;
unsigned long ReturnSpeedFeatBeforeAfter	= 0x005E7E11;
unsigned long GetAreaPtrByID				= 0x0054A1D0;

uint32_t idArea, idCrea, areaType;

namespace {



	// 1.23
	// g_pVirtualMachine
	constexpr uintptr_t NWN2_OFFSET_CVIRTUALMACHINE = 0x00864424;
	// CVirtualMachine::ExecuteScript
	constexpr uintptr_t NWN2_OFFSET_EXECUTESCRIPT = 0x0072B380;
	constexpr uintptr_t NWN2_OFFSET_EXECUTESCRIPT_ENH = 0x0072B050;


	constexpr uintptr_t NWN2_OFFSET_InitParam = 0x0055EA40;
	constexpr uintptr_t NWN2_OFFSET_CleanParam = 0x006b5cd0;

	struct CVirtualMachine
	{

	};

	using CVirtualMachine_ExecuteScript_t = BOOL(__thiscall*)(CVirtualMachine* thisVM,
		const NWN::CExoString& scriptName, NWN::OBJECTID objectId, uint32_t unknown1, uint32_t unknown2);
	CVirtualMachine_ExecuteScript_t CVirtualMachine_ExecuteScript =
		std::bit_cast<CVirtualMachine_ExecuteScript_t>(NWN2_OFFSET_EXECUTESCRIPT);

	using CVirtualMachine_ExecuteScriptEnhanced_t = BOOL(__thiscall*)(CVirtualMachine* thisVM,
		const NWN::CExoString& scriptName, NWN::OBJECTID objectID, void* ParamList, uint32_t unknow1, uint32_t unknow2);
	CVirtualMachine_ExecuteScriptEnhanced_t CVirtualMachine_ExecuteScriptEnhanced =
		std::bit_cast<CVirtualMachine_ExecuteScriptEnhanced_t>(NWN2_OFFSET_EXECUTESCRIPT_ENH);

	using CVirtualMachine_InitParam_t = void(__thiscall*)(void* paramLst, uint32_t iNb);
	using CVirtualMachine_CleanParam_t = void(__thiscall*)(void* paramLst);

	CVirtualMachine_InitParam_t CVirtualMachine_InitParam =
		std::_Bit_cast<CVirtualMachine_InitParam_t>(NWN2_OFFSET_InitParam);
	CVirtualMachine_CleanParam_t CVirtualMachine_CleanParam =
		std::_Bit_cast<CVirtualMachine_CleanParam_t>(NWN2_OFFSET_CleanParam);


	CVirtualMachine*
		GetNwn2VirtualMachine()
	{
		const auto vm = std::bit_cast<CVirtualMachine**>(NWN2_OFFSET_CVIRTUALMACHINE);
		return *vm;
	}





	__declspec (naked) void SpeedMonkPreparation()
	{
		__asm
		{
		}
	}


	__declspec (naked) void SpeedFeatBeforeEverything()
	{
		__asm
		{
			PUSH	EAX
			PUSH	ECX
			MOV		EAX,dword ptr [ESI + 0x144]
			MOV		idArea, EAX
			MOV		EAX, dword ptr [ESI + 0xA0]
			MOV		idCrea, EAX

			CMP		EAX,0x7f000000			
			JZ		ENDOFFCT_PREP

			MOV		ECX, dword ptr [0x0086442c]
			MOV		ECX, dword ptr [ECX + 0x4]

			MOV		EAX,dword ptr [ESI + 0x144]
			PUSH	EAX
			CALL	dword ptr[GetAreaPtrByID]

			TEST	EAX, EAX
			JZ		ENDOFFCT_PREP

			MOV		EAX, dword ptr [EAX + 0x8]
			MOV		areaType, EAX

			POP		ECX
			POP		EAX

			MOV		ECX, dword ptr [ESI + 0x1FC4]

			JMP		dword ptr[ReturnSpeedFeatBeforeAfter]

			ENDOFFCT_PREP	:
			POP		ECX
				POP		EAX

				FLD		dword ptr [ESP + 0x8]

				JMP		dword ptr[ReturnSpeedFeatBeforeEnd]
		}
	}


	void __fastcall CallSpeedFeatHook(uint32_t iType)
	{
		fSpeedFeatValueHooked = 1.0f;
		//push
		void* param1 = (void*)0x0086F15C;
		uint32_t save[3];

		save[0] = *((uint32_t*)param1);
		save[1] = ((uint32_t*)param1)[1];
		save[2] = ((uint32_t*)param1)[2];


		CVirtualMachine_InitParam(param1,4);
		//On applique les params (idCrea, idArea, areaType, "fctType")
		uint32_t* ptrParams = *(uint32_t**)param1;
		((uint32_t*)param1)[1] = 2;
		//*ptrParams = 0x007FED10;
		ptrParams[1] = (uint32_t)idCrea; //Value
		//ptrParams[2] = 0;
		//ptrParams[3] = 0;
		ptrParams[4] = 4;

		ptrParams[6] = iType;
		ptrParams[9] = 0; //Type Int

		ptrParams[11] = (uint32_t)idArea;
		ptrParams[14] = 4; //Object

		ptrParams[16] = areaType;
		ptrParams[19] = 0; //Int


		const NWN::CExoString script = {.m_sString       = (mySpeedFeatScripts[iType]).data(),
			.m_nBufferLength = std::size((mySpeedFeatScripts[iType])) + 1};


		//On appelle la fonction
		const auto vm                = GetNwn2VirtualMachine();
		CVirtualMachine_ExecuteScriptEnhanced(vm, script, idCrea, (void*)0x0086F15C,1, 1); //TODO Kriagki, ici le module serait mieux


		param1 = (void*)0x0086F15C;
		CVirtualMachine_CleanParam(param1);
		//FUN_006b5cd0((int *)&DAT_0086f15c);


		//Pop
		*((uint32_t*)param1) = save[0];
		((uint32_t*)param1)[1] = save[1];
		((uint32_t*)param1)[2] = save[2];

		return;
	}

	//TODO, finish, and duplicate for other
	__declspec (naked) void SpeedFeatHook2248()
	{
		__asm
		{
			PUSH ECX
			PUSH EAX
			PUSH EDX

			PUSH 2248
			CALL CallSpeedFeatHook

			POP EDX
			POP EAX
			POP ECX

			FMUL dword ptr[fSpeedFeatValueHooked]
			jmp dword ptr[ReturnSpeedFeatValue2248]
		}
	}

	__declspec (naked) void SpeedFeatHook194()
	{
		__asm
		{
			PUSH ECX
			PUSH EAX
			PUSH EDX

			PUSH 194
			CALL CallSpeedFeatHook

			POP EDX
			POP EAX
			POP ECX

			FMUL dword ptr[fSpeedFeatValueHooked]
			jmp dword ptr[ReturnSpeedFeatValue194]
		}
	}

	__declspec (naked) void SpeedFeatHook1337()
	{
		__asm
		{
			PUSH ECX
			PUSH EAX
			PUSH EDX

			PUSH 1337
			CALL CallSpeedFeatHook

			POP EDX
			POP EAX
			POP ECX

			FMUL dword ptr[fSpeedFeatValueHooked]
			jmp dword ptr[ReturnSpeedFeatValue1337]
		}
	}

	__declspec (naked) void SpeedFeatHook1806()
	{
		__asm
		{
			PUSH ECX
			PUSH EAX
			PUSH EDX

			PUSH 194
			CALL CallSpeedFeatHook

			POP EDX
			POP EAX
			POP ECX

			FMUL dword ptr[fSpeedFeatValueHooked]
			jmp dword ptr[ReturnSpeedFeatValue1806]
		}
	}

	__declspec (naked) void SpeedFeatHook2120()
	{
		__asm
		{
			PUSH ECX
			PUSH EAX
			PUSH EDX

			PUSH 194
			CALL CallSpeedFeatHook

			POP EDX
			POP EAX
			POP ECX

			FMUL dword ptr[fSpeedFeatValueHooked]
			jmp dword ptr[ReturnSpeedFeatValue2120]
		}
	}

	__declspec (naked) void SpeedFeatHook200()
	{
		__asm
		{
			FLD dword ptr [ESP + 0x8]
			PUSH ECX
			PUSH EAX
			PUSH EDX

			PUSH 194
			CALL CallSpeedFeatHook

			POP EDX
			POP EAX
			POP ECX

			FMUL dword ptr[fSpeedFeatValueHooked]
			jmp dword ptr[ReturnSpeedFeatValue200]
		}
	}

	__declspec (naked) void SpeedFeatHookMin()
	{
		__asm
		{
			PUSH ECX
			PUSH EAX
			PUSH EDX

			PUSH -1
			CALL CallSpeedFeatHook

			POP EDX
			POP EAX
			POP ECX

			FMUL dword ptr[fSpeedFeatValueHooked]
			jmp dword ptr[ReturnSpeedFeatValueMin]
		}
	}

	__declspec (naked) void SpeedFeatHookMax()
	{
		__asm
		{
			PUSH ECX
			PUSH EAX
			PUSH EDX

			PUSH -2
			CALL CallSpeedFeatHook

			POP EDX
			POP EAX
			POP ECX

			FMUL dword ptr[fSpeedFeatValueHooked]
			jmp dword ptr[ReturnSpeedFeatValueMax]
		}
	}

	__declspec (naked) void SpeedFeatHookMonkMax()
	{
		__asm
		{
			PUSH ECX
			PUSH EAX
			PUSH EDX

			PUSH -3
			CALL CallSpeedFeatHook

			POP EDX
			POP EAX
			POP ECX

			FMUL dword ptr[fSpeedFeatValueHooked]
			jmp dword ptr[ReturnSpeedFeatValueMonkMax]
		}
	}

	////////////////////////////////////////////////////////////

	__declspec (naked) void SpeedFeatHookNewFeat()
	{
		__asm
		{
			FLD dword ptr [ESP + 0x8]
			PUSH ECX
			PUSH EAX
			PUSH EDX

			PUSH -4
			CALL CallSpeedFeatHook

			POP EDX
			POP EAX
			POP ECX


			FMUL dword ptr[fSpeedFeatValueHooked]
			FSTP       dword ptr [ESP + 0x8]

			MOV		ECX, dword ptr[ESI +0x1FC4]
			jmp dword ptr[ReturnSpeedFeatHookNewFeat]
		}
	}

	__declspec (naked) void SpeedFeatEndOf200()
	{
		__asm
		{
			//Store and pop so we can jump
			FSTP dword ptr [ESP + 0x8]
			jmp dword ptr[ReturnSpeedFeatEndOf200]
		}
	}
	////////////////////////////////////////////////////////////


	__declspec (naked) void SpeedFeatValue2248()
	{
		__asm
		{
			FMUL dword ptr[fSpeedFeatValue2248]
			jmp dword ptr[ReturnSpeedFeatValue2248]
		}
	}

	__declspec (naked) void SpeedFeatValue194()
	{
		__asm
		{
			FMUL dword ptr[fSpeedFeatValue194]
			jmp dword ptr[ReturnSpeedFeatValue194]
		}
	}

	__declspec (naked) void SpeedFeatValue1337()
	{
		__asm
		{
			FMUL dword ptr[fSpeedFeatValue1337]
			jmp dword ptr[ReturnSpeedFeatValue1337]
		}
	}

	__declspec (naked) void SpeedFeatValue1806()
	{
		__asm
		{
			FMUL dword ptr[fSpeedFeatValue1806]
			jmp dword ptr[ReturnSpeedFeatValue1806]
		}
	}

	__declspec (naked) void SpeedFeatValue2120()
	{
		__asm
		{
			FMUL dword ptr[fSpeedFeatValue2120]
			jmp dword ptr[ReturnSpeedFeatValue2120]
		}
	}

	__declspec (naked) void SpeedFeatValue200()
	{
		__asm
		{
			FMUL dword ptr[fSpeedFeatValue200]
			jmp dword ptr[ReturnSpeedFeatValue200]
		}
	}

	__declspec (naked) void SpeedFeatValueMin()
	{
		__asm
		{
			FLD dword ptr[fSpeedFeatValueMin]
			jmp dword ptr[ReturnSpeedFeatValueMin]
		}
	}

	__declspec (naked) void SpeedFeatValueMax()
	{
		__asm
		{
			FLD dword ptr[fSpeedFeatValueMax]
			jmp dword ptr[ReturnSpeedFeatValueMax]
		}
	}

	__declspec (naked) void SpeedFeatValueMonkMax()
	{
		__asm
		{
			FLD dword ptr[fSpeedFeatValueMonkMax]
			jmp dword ptr[ReturnSpeedFeatValueMonkMax]
		}
	}




	Patch _ConstUpdate[] =
	{
		/*PatchContext(true, (char*)"\xd8\x0d\xc8\x11\x7f\x00", */Patch((DWORD)OFFS_EntrySpeedFeatValue2248, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6), //JMP /NOP
		/*PatchContext(false, (char*)"\xd8\x0d\xc8\x11\x7f\x00", */Patch((DWORD)OFFS_EntrySpeedFeatValue2248+1, (relativefunc)SpeedFeatValue2248),
		/*PatchContext(false, (char*)"\xd8\x0d\xc8\x11\x7f\x00", */Patch((DWORD)OFFS_EntrySpeedFeatValue2248+1, (relativefunc)SpeedFeatHook2248),

		/*PatchContext(true, (char*)"\xd8\x0d\xc8\x11\x7f\x00", */Patch((DWORD)OFFS_EntrySpeedFeatValue194, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6), //JMP /NOP
		/*PatchContext(false, (char*)"", */Patch((DWORD)OFFS_EntrySpeedFeatValue194+1, (relativefunc)SpeedFeatValue194),
		/*PatchContext(false, (char*)"", */Patch((DWORD)OFFS_EntrySpeedFeatValue194+1, (relativefunc)SpeedFeatHook194),

		/*PatchContext(true, (char*)"\xd8\x0d\x40\x29\x80\x00", */Patch((DWORD)OFFS_EntrySpeedFeatValue1337, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6), //JMP /NOP
		/*PatchContext(false, (char*)"", */Patch((DWORD)OFFS_EntrySpeedFeatValue1337+1, (relativefunc)SpeedFeatValue1337),
		/*PatchContext(false, (char*)"", */Patch((DWORD)OFFS_EntrySpeedFeatValue1337+1, (relativefunc)SpeedFeatHook1337),

		/*PatchContext(true, (char*)"\xd8\x0d\xc8\x11\x7f\x00", */Patch((DWORD)OFFS_EntrySpeedFeatValue1806, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6), //JMP /NOP
		/*PatchContext(false, (char*)"", */Patch((DWORD)OFFS_EntrySpeedFeatValue1806+1, (relativefunc)SpeedFeatValue1806),
		/*PatchContext(false, (char*)"", */Patch((DWORD)OFFS_EntrySpeedFeatValue1806+1, (relativefunc)SpeedFeatHook1806),

		/*PatchContext(true, (char*)"\xd8\x0d\x3c\x29\x80\x00", */Patch((DWORD)OFFS_EntrySpeedFeatValue2120, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6), //JMP /NOP
		/*PatchContext(false, (char*)"", */Patch((DWORD)OFFS_EntrySpeedFeatValue2120+1, (relativefunc)SpeedFeatValue2120),
		/*PatchContext(false, (char*)"", */Patch((DWORD)OFFS_EntrySpeedFeatValue2120+1, (relativefunc)SpeedFeatHook2120),

		/*PatchContext(true, (char*)"\xd9\x05\x14\x28\x80\x00", */Patch((DWORD)OFFS_EntrySpeedFeatValueMin, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6), //JMP /NOP
		/*PatchContext(false, (char*)"", */Patch((DWORD)OFFS_EntrySpeedFeatValueMin+1, (relativefunc)SpeedFeatValueMin),
		/*PatchContext(false, (char*)"", */Patch((DWORD)OFFS_EntrySpeedFeatValueMin+1, (relativefunc)SpeedFeatHookMin),

		/*PatchContext(true, (char*)"\xd9\x05\xf0\xbc\x7e\x00", */Patch((DWORD)OFFS_EntrySpeedFeatValueMax, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6), //JMP /NOP
		/*PatchContext(false, (char*)"", */Patch((DWORD)OFFS_EntrySpeedFeatValueMax+1, (relativefunc)SpeedFeatValueMax),
		/*PatchContext(false, (char*)"", */Patch((DWORD)OFFS_EntrySpeedFeatValueMax+1, (relativefunc)SpeedFeatHookMax),

		/*PatchContext(true, (char*)"\xd9\x05\x80\x9e\x7e\x00", */Patch((DWORD)OFFS_EntrySpeedFeatValueMonkMax, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6), //JMP /NOP
		/*PatchContext(false, (char*)"", */Patch((DWORD)OFFS_EntrySpeedFeatValueMonkMax+1, (relativefunc)SpeedFeatValueMonkMax),
		/*PatchContext(false, (char*)"", */Patch((DWORD)OFFS_EntrySpeedFeatValueMonkMax+1, (relativefunc)SpeedFeatHookMonkMax),

		/*PatchContext(true, (char*)"\xd8\x0d\xc8\x11\x7f\x00", */Patch((DWORD)OFFS_EntrySpeedFeatValue200, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6), //JMP /NOP
		/*PatchContext(false, (char*)"", */Patch((DWORD)OFFS_EntrySpeedFeatValue200+1, (relativefunc)SpeedFeatValue200),

		/*PatchContext(true, (char*)"\x83\x78\x08\x04\x75\x10", */Patch((DWORD)OFFS_EntrySpeedFeatTest200, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6), //JMP /NOP
		//		Patch((DWORD)OFFS_EntrySpeedFeatTest200+1, (relativefunc)SpeedFeatTest200),
		/*PatchContext(false, (char*)"", */Patch((DWORD)OFFS_EntrySpeedFeatTest200+1, (relativefunc)SpeedFeatHook200),

		/*PatchContext(true, (char*)"\x8b\x8e\xc4\x1f\x00\x00", */Patch((DWORD)OFFS_SPEED_FEAT_HOOK_BEFOREEVERYTHING, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6), //JMP /NOP
		/*PatchContext(false, (char*)"", */Patch((DWORD)OFFS_SPEED_FEAT_HOOK_BEFOREEVERYTHING+1, (relativefunc)SpeedFeatBeforeEverything),

		/*PatchContext(true, (char*)"\x8b\x8e\xc4\x1f\x00\x00", */Patch((DWORD)OFFS_EntrySpeedFeatHookNewFeat, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6), //JMP /NOP
		/*PatchContext(false, (char*)"", */Patch((DWORD)OFFS_EntrySpeedFeatHookNewFeat + 1, (relativefunc)SpeedFeatHookNewFeat),


		Patch()
	};
	Patch* constantUpdate = _ConstUpdate;


} // namespace

void SetSpeedFeatFloatValue(float fValue)
{
	fSpeedFeatValueHooked = fValue;
}

bool SpeedHookConfigAndApply(SimpleIniConfig* config, LogNWNX* logger)
{
	int iTest = 0;
	float fVal = 1.0;
	std::string sScriptVal = "";
	config->Read("SpeedFeat2248Type", &iTest, 0);
	int iIndex = 0;
	if (iTest == 1)
	{
		config->Read("SpeedFeat2248", &fVal, 1.0f);
		fSpeedFeatValue2248 = fVal;


		logger->Info("* Apply Speed Value for feat 2248 :%f",fSpeedFeatValue2248);

		//if (constantUpdate[iIndex].canIApplyPatch(logger))
		{
			constantUpdate[iIndex].Apply();
			constantUpdate[iIndex+1].Apply();
		}
	}
	else if (iTest == 2)
	{
		config->Read("SpeedFeat2248", &sScriptVal, std::string(""));
		if (sScriptVal != "")
		{
			logger->Info("* Apply HookScript for Speed Feat 2248 :%s",sScriptVal);
			mySpeedFeatScripts[2248] = sScriptVal;
			//if (constantUpdate[iIndex].canIApplyPatch(logger))
			{
				constantUpdate[iIndex].Apply();
				constantUpdate[iIndex + 2].Apply();
			}
		}
	}

	iIndex += 3;
	config->Read("SpeedFeat194Type", &iTest, 0);
	if (iTest == 1)
	{
		config->Read("SpeedFeat194", &fVal, 1.0f);
		fSpeedFeatValue194 = fVal;

		logger->Info("* Apply Speed Value for feat 194 :%f",fSpeedFeatValue194);

		//if (constantUpdate[iIndex].canIApplyPatch(logger))
		{
			constantUpdate[iIndex].Apply();
			constantUpdate[iIndex + 1].Apply();
		}
	}
	else if (iTest == 2)
	{
		config->Read("SpeedFeat194", &sScriptVal, std::string(""));
		if (sScriptVal != "")
		{
			logger->Info("* Apply HookScript for Speed Feat 194 :%s",sScriptVal);
			mySpeedFeatScripts[194] = sScriptVal;
			//if (constantUpdate[iIndex].canIApplyPatch(logger))
			{
				constantUpdate[iIndex].Apply();
				constantUpdate[iIndex + 2].Apply();
			}
		}
	}


	iIndex += 3;
	config->Read("SpeedFeat1337Type", &iTest, 0);
	if (iTest == 1)
	{
		config->Read("SpeedFeat1337", &fVal, 1.0f);
		fSpeedFeatValue1337 = fVal;


		logger->Info("* Apply Speed Value for feat 1337 :%f",fSpeedFeatValue1337);

		//if (constantUpdate[iIndex].canIApplyPatch(logger))
		{
			constantUpdate[iIndex].Apply();
			constantUpdate[iIndex + 1].Apply();
		}
	}
	else if (iTest == 2)
	{
		config->Read("SpeedFeat1337", &sScriptVal, std::string(""));
		if (sScriptVal != "")
		{
			logger->Info("* Apply HookScript for Speed Feat 1337 :%s",sScriptVal);
			mySpeedFeatScripts[1337] = sScriptVal;
			//if (constantUpdate[iIndex].canIApplyPatch(logger))
			{
				constantUpdate[iIndex].Apply();
				constantUpdate[iIndex + 2].Apply();
			}
		}
	}


	iIndex += 3;
	config->Read("SpeedFeat1806Type", &iTest, 0);
	if (iTest == 1)
	{
		config->Read("SpeedFeat1806", &fVal, 1.0f);
		fSpeedFeatValue1806 = fVal;

		logger->Info("* Apply Speed Value for feat 1806 :%f",fSpeedFeatValue1806);

		//if (constantUpdate[iIndex].canIApplyPatch(logger))
		{
			constantUpdate[iIndex].Apply();
			constantUpdate[iIndex + 1].Apply();
		}
	}
	else if (iTest == 2)
	{
		config->Read("SpeedFeat1806", &sScriptVal, std::string(""));
		if (sScriptVal != "")
		{
			logger->Info("* Apply HookScript for Speed Feat 1806 :%s",sScriptVal);
			mySpeedFeatScripts[1806] = sScriptVal;
			//if (constantUpdate[iIndex].canIApplyPatch(logger))
			{
				constantUpdate[iIndex].Apply();
				constantUpdate[iIndex + 2].Apply();
			}
		}
	}


	iIndex += 3;
	config->Read("SpeedFeat2120Type", &iTest, 0);
	if (iTest == 1)
	{
		config->Read("SpeedFeat2120", &fVal, 1.0f);
		fSpeedFeatValue2120 = fVal;
		logger->Info("* Apply Speed Value for feat 2120 :%f",fSpeedFeatValue2120);
		//if (constantUpdate[iIndex].canIApplyPatch(logger))
		{
			constantUpdate[iIndex].Apply();
			constantUpdate[iIndex + 1].Apply();
		}
	}
	else if (iTest == 2)
	{
		config->Read("SpeedFeat2120", &sScriptVal, std::string(""));
		if (sScriptVal != "")
		{
			mySpeedFeatScripts[2120] = sScriptVal;
			logger->Info("* Apply HookScript for Speed Feat 2120 :%s",sScriptVal);
			//if (constantUpdate[iIndex].canIApplyPatch(logger))
			{
				constantUpdate[iIndex].Apply();
				constantUpdate[iIndex + 2].Apply();
			}
		}
	}


	iIndex += 3;
	config->Read("SpeedFeatMinType", &iTest, 0);
	if (iTest == 1)
	{
		config->Read("SpeedFeatMin", &fVal, 1.0f);
		fSpeedFeatValueMin = fVal;
		logger->Info("* Apply Minimum Cap SpeedFeat :%f",fSpeedFeatValueMin);
		//if (constantUpdate[iIndex].canIApplyPatch(logger))
		{
			constantUpdate[iIndex].Apply();
			constantUpdate[iIndex + 1].Apply();
		}
	}
	else if (iTest == 2)
	{
		config->Read("SpeedFeatMin", &sScriptVal, std::string(""));
		if (sScriptVal != "")
		{
			mySpeedFeatScripts[-1] = sScriptVal;
			logger->Info("* Apply HookScript for Minimum Cap SpeedFeat :%s",sScriptVal);
			//if (constantUpdate[iIndex].canIApplyPatch(logger))
			{
				constantUpdate[iIndex].Apply();
				constantUpdate[iIndex + 2].Apply();
			}
		}
	}


	iIndex += 3;
	config->Read("SpeedFeatMaxType", &iTest, 0);
	if (iTest == 1)
	{
		config->Read("SpeedFeatMax", &fVal, 1.0f);
		fSpeedFeatValueMax = fVal;
		logger->Info("* Apply Maximum Cap for SpeedFeat :%f",fSpeedFeatValueMax);
		//if (constantUpdate[iIndex].canIApplyPatch(logger))
		{
			constantUpdate[iIndex].Apply();
			constantUpdate[iIndex + 1].Apply();
		}
	}
	else if (iTest == 2)
	{
		config->Read("SpeedFeatMax", &sScriptVal, std::string(""));
		if (sScriptVal != "")
		{
			mySpeedFeatScripts[-2] = sScriptVal;
			logger->Info("* Apply HookScript for MaximumCap for SpeedFeat :%s",sScriptVal);
			//if (constantUpdate[iIndex].canIApplyPatch(logger))
			{
				constantUpdate[iIndex].Apply();
				constantUpdate[iIndex + 2].Apply();
			}
		}
	}


	iIndex += 3;
	config->Read("SpeedFeatMonkMaxType", &iTest, 0);
	if (iTest == 1)
	{
		config->Read("SpeedFeatMonkMax", &fVal, 1.0f);
		fSpeedFeatValueMonkMax = fVal;
		logger->Info("* Apply Maximum Cap (for Monk) Value for SpeedFeat :%f",fSpeedFeatValueMonkMax);
		//if (constantUpdate[iIndex].canIApplyPatch(logger))
		{
			constantUpdate[iIndex].Apply();
			constantUpdate[iIndex + 1].Apply();
		}
	}
	else if (iTest == 2)
	{
		config->Read("SpeedFeatMonkMax", &sScriptVal, std::string(""));
		if (sScriptVal != "")
		{
			mySpeedFeatScripts[-3] = sScriptVal;
			logger->Info("* Apply HookScript for Maximum (monk) cap value for SpeedFeat :%s",sScriptVal);
			//if (constantUpdate[iIndex].canIApplyPatch(logger))
			{
				constantUpdate[iIndex].Apply();
				constantUpdate[iIndex + 2].Apply();
			}
		}
	}


	iIndex += 3;
	config->Read("SpeedFeat200Type", &iTest, 0);
	if (iTest == 1)
	{
		config->Read("SpeedFeat200", &fVal, 1.0f);
		fSpeedFeatValue200 = fVal;
		logger->Info("* Apply Speed Value for feat 200 :%f",fSpeedFeatValue200);
		//if (constantUpdate[iIndex].canIApplyPatch(logger))
		{
			constantUpdate[iIndex].Apply();
			constantUpdate[iIndex + 1].Apply();
		}
	}
	else if (iTest == 2)
	{
		config->Read("SpeedFeat200", &sScriptVal, std::string(""));
		if (sScriptVal != "")
		{
			mySpeedFeatScripts[200] = sScriptVal;
			logger->Info("* Apply HookScript for Speed Feat 200 :%s",sScriptVal);
			//if (constantUpdate[iIndex+2].canIApplyPatch(logger))
			{
				constantUpdate[iIndex + 2].Apply();
				constantUpdate[iIndex + 3].Apply();
			}
		}
	}

	//Allways add the pre-function hook
	iIndex += 4;
	//if (constantUpdate[iIndex].canIApplyPatch(logger))
	{
		constantUpdate[iIndex].Apply();
		constantUpdate[iIndex + 1].Apply();
	}

	iIndex += 2;
	config->Read("SpeedFeatNews", &sScriptVal, std::string(""));
	if (sScriptVal != "")
	{
		mySpeedFeatScripts[-4] = sScriptVal;
		logger->Info("* Apply HookScript for New Speed Feat :%s",sScriptVal);
		//if (constantUpdate[iIndex].canIApplyPatch(logger))
		{
			constantUpdate[iIndex].Apply();
			constantUpdate[iIndex + 1].Apply();
		}
	}

	return true;
}
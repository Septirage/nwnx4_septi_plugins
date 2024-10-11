#include "StoreRetrievePatch.h"
#include "EnhancedFeatures.h"
#include "../../NWN2Lib/NWN2.h"
#include "../../NWN2Lib/NWN2Common.h"
#include "..\..\misc\Patch.h"
#include <bit>
#include <cassert>
#include <charconv>
#include <detours/detours.h>
#include <optional>
#include <string_view>
#include <iostream>
#include <fstream>


extern std::unique_ptr<LogNWNX> logger;

unsigned long StoreFunctionStarter = 0x006b282f;
unsigned long NoStoreForMe = 0x006b2a6d;


//Return @ From Retrieve Hook
unsigned long ReturnRetrieveAddr = 0x006a521b;


unsigned long FctWriteVal = 0x00435f80;
unsigned long Fct7955C0 = 0x007955C0;
unsigned long ReturnAfterFct = 0x006b2a0e;


unsigned long DestroyEngineString = 0x00436100;

//Fct
unsigned long ReturnAddressNothing	= 0x006b2a51;


/*
//Creature		5
//Item			6
//Placeable		9
//Light			0x15 (ou F?)
//Trigger			7
//VFX			0x16
//Waypoint		c

*/



//Generic fcts
#define FCT_CompareString			0x00435550
#define FCT_AllocateSpaceForObje	0x00413ad0


int bSaveLocalVar = 1;


namespace {
	typedef void(__cdecl* NWN2Heap_Deallocate_Proc)(void* p);
	extern NWN2Heap_Deallocate_Proc NWN2Heap_Deallocate;

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////Return points////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Return point for TestItemIdInPlaceable(Store)
	unsigned long ResumeItemIdPlaceable				= 0x0064cbc9;
	unsigned long AfterItemIdPlaceable				= 0x0064cbdb;

	unsigned long LoadTriggerPosReturn				= 0x006cc0d9;
	unsigned long LoadTriggerBaseStuffReturn		= 0x006cc02f;
	unsigned long StoreLocalVarReturn				= 0x006b2a18;

	unsigned long ReturnAddrTestIfItmOrSaveLVItm	= 0x005d7b46;
	unsigned long ReturnAddrTestIfItmOrSaveLVLV		= 0x005d7fcb;

	unsigned long StoreCreatureGoBackAfterLocalVar	= 0x005fa99a;


	unsigned long SaveVarAndEffect = 0x0057e710;//0x00679a40;//0x0057e710;
	unsigned long RetrieveItemVar = 0x006a5053;


	//File Header signature
	char sUTP[]  = "UTP ";
	char sULT[] = "ULT ";
	char sUTW[] = "UTW ";
	char sUTT[] = "UTT ";	//Trigger	// 0x420 // 4
	char sUPE[] = "UPE ";
	char sUTI[] = "UTI ";
	char sUTD[] = "UTD ";	//Door	// 0x4b0 //	8
	char sUTR[] = "UTR ";	//Tree	// ...	// 
	char sUTS[] = "UTS ";	//Sound	// ...	// 

	char* pUT;


	int bIsSaveLocalVar = 0;
	int bCurrentlyStoring = 0;
	int pSpecificTreatment = 0;
	int iCurrentType;

	unsigned long FunctionSaveInfo;
	unsigned long FunctionRetrieveInfo;
	unsigned long FunctionInitInfo;

	unsigned long GenericFunctionToCall;



	////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////// Storing Fcts /////////////////////////////////////////////
	__declspec(naked) void StoreObjectEnhanced()
	{
		__asm
		{
			MOV	iCurrentType, 0x0
			JNZ	IsItem
			MOV	bCurrentlyStoring, 0x1
			//Grab the object Type
			MOV EAX, dword ptr[ECX+0xA4]
			//Test if Placeable
			CMP AL, 0x09
			JE IsPlaceable
			//Test If Trigger
			CMP AL, 0x07
			JE IsTrigger
			//Test if Waypoint
			CMP AL, 0x0C
			JE IsWayPoint
			//Test if Light
			CMP AL, 0x15
			JE IsLight
			//Test if PlacedEffect
			CMP AL, 0x16
			JE IsPlacedEffect
			//Test if Door
			CMP AL, 0xA
			JE IsDoor

			JMP NoSaveHere

			IsItem:
			MOV	iCurrentType, 0x2
				LEA ECX, sUTI
				MOV pUT, ECX
				MOV ECX, SaveItemInfo
				MOV FunctionSaveInfo, ECX

				JMP SaveHere
				IsDoor:
			MOV iCurrentType, 0x8
				LEA ECX, sUTD
				MOV pUT, ECX
				MOV ECX, SaveDoorInfo
				MOV FunctionSaveInfo, ECX

				JMP SaveHere

				IsPlaceable:
			LEA ECX, sUTP
				MOV pUT, ECX
				MOV ECX, SavePlaceableInfo
				MOV FunctionSaveInfo, ECX

				JMP SaveHere

				IsTrigger:
			LEA ECX, sUTT
				MOV pUT, ECX
				MOV ECX, SaveTriggerInfo
				MOV FunctionSaveInfo, ECX

				JMP SaveHere

				IsWayPoint:
			LEA ECX, sUTW
				MOV pUT, ECX
				MOV ECX, SaveWayPointInfo
				MOV FunctionSaveInfo, ECX

				JMP SaveHere

				IsLight:
			LEA ECX, sULT
				MOV pUT, ECX
				MOV ECX, SaveLightInfo
				MOV FunctionSaveInfo, ECX

				JMP SaveHere

				IsPlacedEffect:
			LEA ECX, sUPE
				MOV pUT, ECX
				MOV ECX, SavePlacedEffectInfo
				MOV FunctionSaveInfo, ECX

				JMP SaveHere


				SaveHere:

			PUSH 0x007FEDB4
				LEA ECX, [ESP+0x14]
				CALL dword ptr[FctWriteVal]

				PUSH pUT
				LEA ECX, [ESP+0x24]
				CALL dword ptr[FctWriteVal]

				LEA ECX, [ESP + 0x10]
				PUSH ECX
				LEA EDX,[ESP + 0x24]
				PUSH EDX
				PUSH EBX
				MOV ECX,EDI
				CALL dword ptr[Fct7955C0]

				LEA ECX,[ESP+0x20]
				MOV EBP,EAX
				CALL dword ptr[DestroyEngineString]

				LEA ECX,[ESP+0x10]
				CALL dword ptr[DestroyEngineString]

				TEST EBP,EBP
				JZ NoSaveHere

				CMP iCurrentType, 0x2
				JE ContinueForItem
				MOV bIsSaveLocalVar, 0x1
				PUSH 0x0
				PUSH EBX
				PUSH EDI
				MOV	 ECX,ESI
				SUB ECX, 0x730
				JMP	ContinueForNItem
				//JNE	NotAnItem
				ContinueForItem:
			//MOV	iCurrentType, 0x0
			MOV	EAX, dword ptr [ESI]
				MOV	EDX, dword ptr [EAX + 0x70]


				PUSH 0x0
				PUSH EBX
				PUSH EDI
				MOV	 ECX,ESI
				CALL EDX
				MOV	 ECX,EAX
				ContinueForNItem:
			MOV EAX, SaveItemInfo
				MOV GenericFunctionToCall, EAX

				CALL dword ptr[GenericFunctionToCall]

				CMP iCurrentType, 0x2
				JE	EndOfSave

				//JMP	 CallMyStoreFct

			NotAnItem:
			PUSH EBX
				PUSH EDI
				MOV ECX, ESI
				CallMyStoreFct:
			CALL dword ptr[FunctionSaveInfo]
				/*
				CMP bSaveLocalVar, 0xdq1
				JNE EndOfSave

				AddLocalVar:
				PUSH EBX
				PUSH EDI
				MOV ECX, ESI
				CALL dword ptr[SaveVarAndEffect]
				//MOV bSaveLocalVar, 0x0
				*/
			EndOfSave:
				MOV	bCurrentlyStoring, 0x0
					jmp dword ptr[ReturnAfterFct]

					NoSaveHere:
					MOV	bCurrentlyStoring, 0x0
					jmp dword ptr[ReturnAddressNothing]
		}
	}


	//This function will launch the wanted storing fct (if object type is ok)
	__declspec(naked) void TestStoreObjectEnhanced()
	{
		__asm
		{
			jz	NotBaseObject

			WeCanSave:
			jmp dword ptr[StoreFunctionStarter]

				NotBaseObject:
				MOV EAX, dword ptr[ESI+0xA4]

				//Test if it's an object type we know how to save.
				CMP AL, 0x09
				JE WeCanSave
				CMP AL, 0x07
				JE WeCanSave
				CMP AL, 0x0C
				JE WeCanSave
				CMP AL, 0x15
				JE WeCanSave
				CMP AL, 0x16
				JE WeCanSave
				CMP AL, 0x0A
				JE WeCanSave

				//We can't save this object (for now !)
				jmp dword ptr[NoStoreForMe]
		}

	}


	__declspec(naked) void TestItemIdInPlaceableStore()
	{
		__asm
		{
			CMP		bCurrentlyStoring, 0x1
			JE		NoObjIdPlaceableStore	
			MOV		ECX, dword ptr [EAX + 0x7d0]
			JMP     dword ptr[ResumeItemIdPlaceable]

			NoObjIdPlaceableStore :
			JMP		dword ptr[AfterItemIdPlaceable]

		}
	}


	__declspec (naked) void TestIfItemOrSaveLV()
	{
		__asm
		{
			CALL dword ptr[SaveVarAndEffect]

			CMP bIsSaveLocalVar, 0x01
			JE	TestIfItmOrSaveLVLV

			JMP dword ptr[ReturnAddrTestIfItmOrSaveLVItm]

			TestIfItmOrSaveLVLV:
			MOV	bIsSaveLocalVar, 0x0
				JMP dword ptr[ReturnAddrTestIfItmOrSaveLVLV]		

		}
	}



	///////////////////////////////////////////// Retrive Fcts /////////////////////////////////////////////

	__declspec(naked) void RetrieveAddDataObject()
	{
		__asm
		{
			SUB		ESP, 0x44
			PUSH	EBX
			MOV		EBX, dword ptr ss:[ESP+0x4C]
			PUSH	ESI
			PUSH	EDI
			MOV		EDI, dword ptr ss:[ESP+0x58]

			PUSH	EDI
			PUSH	EBX

			MOV		GenericFunctionToCall, FCT_RetrieveAddData
			CALL	GenericFunctionToCall

			POP		EDI
			POP		ESI
			POP		EBX
			ADD		ESP, 0x44
			RET		0x8
		}
	}




	__declspec(naked) void RetrieveObjectEnhanced()
	{
		__asm
		{
			JNZ		RetrieveItem


			LEA EAX, sUTP
			MOV pUT, EAX
			PUSH	pUT
			MOV		GenericFunctionToCall, FCT_CompareString
			LEA     ECX,[ESP + 0x40]
			CALL	GenericFunctionToCall
			TEST	AL, AL
			JNZ		RetrieveUTP
			//TODO, OTHER

			LEA EAX, sUTT
			MOV pUT, EAX
			PUSH	pUT
			LEA     ECX,[ESP + 0x40]
			CALL	GenericFunctionToCall
			TEST	AL, AL
			JNZ		RetrieveUTT

			LEA EAX, sUTW
			MOV pUT, EAX
			PUSH	pUT
			LEA     ECX,[ESP + 0x40]
			CALL	GenericFunctionToCall
			TEST	AL, AL
			JNZ		RetrieveUTW

			LEA EAX, sULT
			MOV pUT, EAX
			PUSH	pUT
			LEA     ECX,[ESP + 0x40]
			CALL	GenericFunctionToCall
			TEST	AL, AL
			JNZ		RetrieveULT

			LEA EAX, sUPE
			MOV pUT, EAX
			PUSH	pUT
			LEA     ECX,[ESP + 0x40]
			CALL	GenericFunctionToCall
			TEST	AL, AL
			JNZ		RetrieveUPE

			LEA	EAX, sUTD
			MOV pUT, EAX
			PUSH	pUT
			LEA		ECX, [ESP + 0x40]
			CALL	GenericFunctionToCall
			TEST	AL, AL
			JNZ		RetrieveUTD




		PasRetrieve:
			JMP		dword ptr[ReturnRetrieveAddr]

				RetrieveUTD:
				MOV		FunctionRetrieveInfo, FCT_RetrieveDoorInfo
				MOV		FunctionInitInfo,	FCT_InitDoorInfo
				PUSH	0x4b0
				MOV		iCurrentType, 8
				JMP		ContinueRetrieve

				RetrieveUTP:
			MOV		FunctionRetrieveInfo, FCT_RetrievePlaceableInfo
				MOV		FunctionInitInfo, FCT_InitPlaceableInfo
				PUSH	0x550
				MOV		iCurrentType, 64
				JMP		ContinueRetrieve

				RetrieveULT:
			MOV		FunctionRetrieveInfo, FCT_RetrieveLightInfo
				MOV		FunctionInitInfo, FCT_InitLightInfo
				PUSH	0x3B0
				MOV		iCurrentType, 512
				JMP		ContinueRetrieve

				RetrieveUTW:
			MOV		FunctionRetrieveInfo, FCT_RetrieveWaypointInfo
				MOV		FunctionInitInfo, FCT_InitWaypointInfo
				PUSH	0x340
				MOV		iCurrentType, 32
				JMP		ContinueRetrieve

				RetrieveUTT:
			MOV		FunctionRetrieveInfo, FCT_RetrieveTriggerInfo
				MOV		FunctionInitInfo, FCT_InitTriggerInfo
				PUSH	0x420
				MOV		iCurrentType, 4
				JMP		ContinueRetrieve

				RetrieveUPE:
			MOV		FunctionRetrieveInfo, FCT_RetrievePlacedEffInfo
				MOV		FunctionInitInfo, FCT_InitPlacedEffInfo
				PUSH	0x350
				MOV		iCurrentType, 1024
				JMP		ContinueRetrieve

				ContinueRetrieve:
			MOV		GenericFunctionToCall, FCT_AllocateSpaceForObje
				CALL	GenericFunctionToCall
				//CALL	FCT_AllocateSpaceForObje

				ADD		ESP, 0x4
				TEST	EAX, EAX
				JZ		NotAllocated

				PUSH	0x7F000000
				MOV		ECX, EAX

				CMP		iCurrentType, 4
				JNE		ContinueRetrieveNoTrig
				CALL	FunctionInitInfo
				LEA		ECX, [ESP + 0x24]
				MOV		pSpecificTreatment, EDI
				PUSH	ECX
				PUSH	EBP
				MOV		ECX, EAX
				MOV		dword ptr[ESP +0x20], EAX
				CALL	FunctionRetrieveInfo
				MOV		pSpecificTreatment, 0x0
				JMP		ContinueRetrieve3
				ContinueRetrieveNoTrig:
			CALL	FunctionInitInfo
				JMP		ContinueRetrieve2
				NotAllocated:
			XOR		EAX, EAX
				NotAllocated2:
			MOV		GenericFunctionToCall, RETRIEVE_ERR
				JMP		dword ptr[GenericFunctionToCall]
				//JMP		RETRIEVE_ERR

				ContinueRetrieve2:
				MOV		ECX, EAX
				CMP		iCurrentType, 8
				JE      DebutRetrieveDoor

				MOV		EAX, ECX
				LEA		ECX,[ESP +0x24]
				PUSH	0x0
				PUSH	ECX
				PUSH	EBP
				MOV		ECX, EAX
				MOV		dword ptr[ESP +0x24], EAX
				JMP		AfterDoor
				DebutRetrieveDoor:
			MOV		EAX, ECX
				LEA		ECX,[ESP +0x24]
				PUSH	ECX
				PUSH	EBP
				MOV		ECX, EAX
				MOV		dword ptr[ESP +0x20], EAX

				AfterDoor:
			/*
			PUSH	ECX
			PUSH	EBP
			MOV		ECX, EAX
			MOV		dword ptr[ESP +0x24], EAX
			*/

			CALL	FunctionRetrieveInfo
				ContinueRetrieve3:
			TEST	EAX, EAX
				JZ		NotAllocated2

				CMP		bSaveLocalVar, 0x1
				JNE		NoRetrieveAddData

				LEA		EAX, [ESP + 0x24]
				MOV		ECX, [ESP + 0x18]
				PUSH	EAX
				PUSH	EBP
				CALL	RetrieveAddDataObject
				NoRetrieveAddData:
			MOV		ECX,dword ptr [ESP + 0x18]
				//pour un item ce serait EAX+0x730, ( voir si des objets enhanced qui pas  +0)
				MOV		EDX,dword ptr [ECX]
				MOV		EAX,dword ptr [EDX + 0x138]

				PUSH	0x0
				PUSH	EDI
				CALL	EAX

				MOV		ECX,dword ptr[ESP + 0x18]
				//pour un item ce serait EAX+0x730, ( voir si des objets enhanced qui pas  +0)
				MOV		EDX,dword ptr [ECX]
				MOV     EDX,dword ptr [EDX + 0x1c]
				LEA     EAX,[EDI + 0xc]
				PUSH	EAX
				CALL	EDX

				//Until here, it was shared
				/*
				CMP		iCurrentType, 512
				JE		RetrieveJmp1
				*/
				FLD		dword ptr [EDI + 0x8]			//Pas pour LIGHT
				RetrieveJmp1:
			MOV		EAX,dword ptr [ESP + 0x18]
				MOV		EDX,dword ptr [EBX]
				//pour un item ce serait EAX+0x730, ( voir si des objets enhanced qui pas  +0)
				MOV		ESI,dword ptr [EAX]		

				MOV 	EAX,dword ptr [EDX + 0x5c]

				/*
				CMP		iCurrentType, 512
				JE		RetrieveJmp3
				CMP		iCurrentType, 1024
				JE		RetrieveJmp2
				*/

				PUSH	0x1
				RetrieveJmp2:
			SUB		ESP,0xc
				FSTP	dword ptr [ESP + 0x8]
				MOV		ECX, EBX
				FLD		dword ptr [EDI + 0x4]
				FSTP	dword ptr [ESP + 0x4]
				FLD		dword ptr [EDI]
				FSTP	dword ptr [ESP]

				CALL	EAX

				MOV		ECX, dword ptr [ESP + 0x28]
				MOV		EDX, dword ptr [ESI + 0x12c]

				JMP		RetrieveJmp4
				RetrieveJmp3:
			CALL	EAX

				MOV		ECX, dword ptr [ESP + 0x28]
				MOV		EDX, dword ptr [ESI + 0x130]

				RetrieveJmp4:
				PUSH	EAX
				//TODO ICI On avait un ecx+= 0x730 si item

				CALL	EDX

				MOV		EAX, dword ptr [ESP + 0x18]
				//For an item it was + 0x7d0 (730+a0)
				MOV		ECX, dword ptr [EAX + 0xA0]
				MOV		dword ptr [ESP + 0x20], ECX

				JMP		dword ptr[ReturnRetrieveAddr]


				RetrieveItem:
			JMP		dword ptr[RetrieveItemVar]
		}
	}



	__declspec(naked) void LoadTriggerAtPosition()
	{
		__asm
		{
			CMP		pSpecificTreatment, 0x0
			JE		LoadTriggerBasicStuff

			MOV		EAX, pSpecificTreatment
			MOV		EAX, dword ptr [EAX]
			MOV		[ESP + 0x34], EAX
			MOV		EAX, pSpecificTreatment
			MOV		EAX, dword ptr [EAX + 0x4]
			MOV		[ESP + 0x38], EAX
			MOV		EAX, pSpecificTreatment
			MOV		EAX, dword ptr [EAX + 0x8]
			MOV		[ESP + 0x3C], EAX

			MOV		EAX, pSpecificTreatment
			MOV		EAX, dword ptr [EAX + 0xC]
			MOV		[ESP + 0x88], EAX
			MOV		EAX, pSpecificTreatment
			MOV		EAX, dword ptr [EAX + 0x10]
			MOV		[ESP + 0x8C], EAX
			MOV		EAX, pSpecificTreatment
			MOV		EAX, dword ptr [EAX + 0x14]
			MOV		[ESP + 0x90], EAX

			JMP		dword ptr[LoadTriggerPosReturn]

			LoadTriggerBasicStuff:
			PUSH	ECX
				FSTP	dword ptr [ESP]
				LEA		EAX, [ESP+0x34]
				JMP		dword ptr[LoadTriggerBaseStuffReturn]
		}
	}




	__declspec (naked) void StoreLocalVarForCreature()
	{
		__asm
		{
			MOV		EAX, dword ptr [ESP + 0xA4]
			CMP		EAX, 0x02
			JNE		DontSaveLocVar

			MOV		dword ptr[ESP + 0xA4], 0x00
			MOV		EBP, dword ptr [ESP + 0x9C]
			MOV		EAX, dword ptr [ESP + 0x98]
			PUSH	EBP
			PUSH	EAX
			MOV		ECX, EDI
			CALL	dword ptr[SaveVarAndEffect]


			DontSaveLocVar:
			//Il faudra faire, dans tous les cas :
			MOV		EBP,dword ptr [ESP + 0x9c]

				//GoBack
				JMP	dword ptr[StoreCreatureGoBackAfterLocalVar]
		}
	}

	unsigned long ReturnAddrRetrieveCreaNot0 = 0x00601310;
	unsigned long ReturnAddrRetrieveCreaAfterLocVar = 0x0060131B;

	__declspec(naked) void CallRetrieveCreaLocalVar2()
	{
		__asm
		{
			TEST	EAX, EAX
			JZ		TestLocalVarCrea
			POP		EDI
			JMP		dword ptr[ReturnAddrRetrieveCreaNot0]

			TestLocalVarCrea:
			MOV		EAX, [ESP + 0x58]
				TEST	EAX, 0x2
				JNZ		TestEndOfLocalVarCrea
				MOV		[ESP + 0x58], 0
				PUSH	ECX
				PUSH	EDI
				PUSH	EBP
				MOV		ECX, ESI
				CALL	RetrieveAddDataObject
				POP		ECX
				TestEndOfLocalVarCrea:
			JMP		dword ptr[ReturnAddrRetrieveCreaAfterLocVar]
		}
	}





	Patch _StoreRetrievePatches[] =
	{
		/*PatchContext(true, (char*)"\x0f\x84\x9d\x00\x00\x00", */Patch((DWORD)OFFS_StoreObjectEnhanced, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6),
		/*PatchContext(false, (char*)"", */Patch(OFFS_StoreObjectEnhanced + 1, (relativefunc)StoreObjectEnhanced),

		// Quick hook to branch on save item or save localVar.
		/*PatchContext(true, (char*)"\xe8\xca\x6b\xfa\xff", */Patch((DWORD)0x005d7b41, (char*)"\xe9\x00\x00\x00\x00", (int)5), // JMP/NOP
		/*PatchContext(false, (char*)"", */Patch(0x005d7b41 + 1, (relativefunc)TestIfItemOrSaveLV),

		/*PatchContext(true, (char*)"\x8b\x88\xd0\x07\x00\x00", */Patch((DWORD)OFFS_ItemIdInPlaceableStore, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6), // JMP/NOP
		/*PatchContext(false, (char*)"", */Patch(OFFS_ItemIdInPlaceableStore + 1, (relativefunc)TestItemIdInPlaceableStore),

		//		\x

		/*PatchContext(true, (char*)"\x6a\x00", */Patch((DWORD)OFFS_StoreCreatureParam, (char*)"\x6a\x02", (int)2), // PUSH 0x02
		/*PatchContext(true, (char*)"\x8b\xac\x24\x9c\x00\x00\x00", */Patch((DWORD)OFFS_StoreCreature, (char*)"\xe9\x00\x00\x00\x00\x90\x90", (int)7), // JMP/NOP/NOP
		/*PatchContext(false, (char*)"", */Patch(OFFS_StoreCreature + 1, (relativefunc)StoreLocalVarForCreature),


		// Hook to test if we can save the current object (and launch the save) or not.
		/*PatchContext(true, (char*)"\x0f\x84\x3e\x02\x00\x00", */Patch((DWORD)OFFS_TestOfStoreObject, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6), // JMP/NOP
		/*PatchContext(false, (char*)"", */Patch(OFFS_TestOfStoreObject + 1, (relativefunc)TestStoreObjectEnhanced),



		// Retrieve PART !
		/*PatchContext(true, (char*)"\x0f\x84\xc8\x01\x00\x00", */Patch((DWORD)OFFS_RetrieveObjectEnhanced, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6), // JMP/NOP
		/*PatchContext(false, (char*)"", */Patch(OFFS_RetrieveObjectEnhanced + 1, (relativefunc)RetrieveObjectEnhanced),


		// For Crea LocalVar
		/*PatchContext(true, (char*)"\x6a\x00", */Patch((DWORD)0x006a4f48, (char*)"\x6a\x02", (int)2),
		/*PatchContext(true, (char*)"\x85\xc0\x74\x0c\x5f", */Patch((DWORD)0x0060130b, (char*)"\xe9\x00\x00\x00\x00", (int)5),
		/*PatchContext(false, (char*)"", */Patch((DWORD)0x0060130b + 1, (relativefunc)CallRetrieveCreaLocalVar2),


		/*PatchContext(true, (char*)"\x51\xd9\x1c\x24\x8d\x44\x24\x34", */Patch((DWORD)OFFS_TriggerLoadAtPosition, (char*)"\xe9\x00\x00\x00\x00\x90\x90\x90", (int)8), //JMP/NOPx3
		/*PatchContext(false, (char*)"", */Patch(OFFS_TriggerLoadAtPosition + 1, (relativefunc)LoadTriggerAtPosition),


		Patch()
	};

	Patch *StoreRetrievePatches = _StoreRetrievePatches;

} // namespace


bool StoreRetrieveConfigAndApply(SimpleIniConfig* config)
{
	int iTest = 0;
	int i = 0;
	config->Read("EnhanceStoreRetrieve", &iTest, 0);
	if (iTest != 0)
	{
		logger->Info("* Patch StoreRetrieve");
		bool bCanIapp = true;

		if (bCanIapp)
		{
			while (StoreRetrievePatches[i].Apply()) {
				i++;
			}
		}
	}

	return true;
}

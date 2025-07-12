#include <string>
#include <vector>
#include <sstream>

#include "../../NWN2Lib/NWN2.h"
#include "../../NWN2Lib/NWN2Common.h"
#include "..\..\misc\Patch.h"

#include "../../septutil/NwN2DataPos.h"
#include "refreshObject.h"

#include "nwn2heap.h"


#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif


typedef void (__cdecl * NWN2Heap_Deallocate_Proc)(void *p);
extern NWN2Heap_Deallocate_Proc NWN2Heap_Deallocate;



#define OFFS_AVOIDPART		0x0056e6d0

#define OFFS_SETFACING1		0x006ac2d3
#define OFFS_SETFACING2		0x006ac353
#define OFFS_SETFACINGDOOR	0x006ac3b7

namespace {
	Patch _tempPatch[] =
	{
		//List of TempPatch for SetFacing
		//Patch to make the SetFacing work outside AssignCommand
		Patch((DWORD)OFFS_SETFACING1, (char*)"\x89\x01\x58\x90\x90", (int)5), //MOV [ECX], EAX;  POP EAX; NOP; NOP;
		Patch((DWORD)OFFS_SETFACING2, (char*)"\x89\x01\x58\x90\x90", (int)5), //MOV [ECX], EAX;  POP EAX; NOP; NOP;
		//Patch to allow door (ugly patch, willprobably allow lot of things but ok because temporaryPatch) for SetFacing
		Patch((DWORD)OFFS_SETFACINGDOOR, (char*)"\x8B\xC8\x90\x90\x90\x90\x90\x90", (int)8), //MOV EAX, ECX; NOP; NOP; NOP; NOP; NOP; NOP;


	};

	Patch* tempPatch = _tempPatch;
}

__declspec(naked) void __fastcall SetPlaceableColision(__in void* ptrPlc, __in void* Unused, __in float* position, __in uint32_t zero)
{
	__asm
	{
		mov		edx, 0x0057F360;
		jmp		edx;
	}
}

__declspec(naked) void __fastcall SetOrientationQ(__in void* idObjBlock, __in void* Unused, __in uint32_t vA, __in uint32_t v2)
{
	__asm
	{
		mov		edx, 0x006ac290;
		jmp		edx;
	}
}

void FinishObjectRepositionning(GameObject* Object, void* AreaPtr)
{
	uint8_t objType = *(uint8_t*)((int)Object + 0xA4);

	if(AreaPtr == NULL)
		return;

	if(objType == NWN::OBJECT_TYPE_PLACEABLE || objType == NWN::OBJECT_TYPE_DOOR)
	{
		//Recalculate newPosition
		ColisionLoSModelName((void*)Object);

		//Add new "position"
		AddToArea(AreaPtr, (void*)Object);
	}
}

void* PrepareObjectRepositioning(GameObject* Object)
{
	GameObjectManager m_ObjectManager;
	GameObject* Area;
	void* AreaPtr = NULL;

	uint8_t objType = *(uint8_t*)((int)Object + 0xA4);

	if(objType == NWN::OBJECT_TYPE_PLACEABLE || objType == NWN::OBJECT_TYPE_DOOR)
	{
		NWN::OBJECTID AreaId = *(NWN::OBJECTID*)((int)Object + 0x144);

		if ((AreaId & NWN::INVALIDOBJID) != 0)
			AreaId &= ~(NWN::LISTTYPE_MASK);

		if ((Area = m_ObjectManager.GetGameObject( AreaId )) == NULL)
			return AreaPtr;

		AreaPtr = (void*)((int)Area - 0xad0);

		//Remove Old "position"
		RemoveFromArea(AreaPtr, (void*)Object);
	}

	return AreaPtr;
}


void SetDynamicColision(int nObject, int nValue) {
	NWN::OBJECTID       ObjectId;
	GameObject* Object;
	GameObjectManager m_ObjectManager;

	ObjectId = (NWN::OBJECTID) nObject;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return;
	if (Object->GetObjectType() != NWN::OBJECT_TYPE::OBJECT_TYPE_PLACEABLE)
		return;

	uint32_t* ptrDynamicCol	= (uint32_t*)(((char*)Object) + AmPlcDynamicCol);
	uint32_t bColBefore = *ptrDynamicCol;
	char* pointedFlags = (((char*)Object) + AmPlcColisionFlag);

	if (bColBefore != (uint32_t)nValue)
	{
		//We change it, so need to call all the fct
		*ptrDynamicCol = (uint32_t)nValue;

		void* AreaPtr = PrepareObjectRepositioning(Object);

		char* MyFlagsAddr = *(char**)(pointedFlags);
		MyFlagsAddr = (MyFlagsAddr + 0x10);
		if (nValue == 0)
		{
			*(uint32_t*)MyFlagsAddr = (*(uint32_t*)MyFlagsAddr) & 0xfeffffff;
		}
		else
		{
			*(uint32_t*)MyFlagsAddr = (*(uint32_t*)MyFlagsAddr) | 0x1000000;
		}


		FinishObjectRepositionning(Object, AreaPtr);
	}
}


void ObjectSetPosition(int iObjectId, float fX, float fY, float fZ)
{
	NWN::OBJECTID ObjectId = (NWN::OBJECTID)iObjectId;
	GameObject* Object;
	GameObjectManager m_ObjectManager;
	void* AreaPtr;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return;

	AreaPtr = PrepareObjectRepositioning(Object);


	if (Object->GetObjectType() == NWN::OBJECT_TYPE_TRIGGER)
	{
		float fDiffX = *(float*)(((char*)Object) + AmCommonPosX);
		fDiffX = fDiffX - fX;
		float fDiffY = *(float*)(((char*)Object) + AmCommonPosY);
		fDiffY = fDiffY - fY;
		float fDiffZ = *(float*)(((char*)Object) + AmCommonPosZ);
		fDiffZ = fDiffZ - fZ;

		uint32_t iNbPoint = *(uint32_t*)(((char*)Object) + AmTrigGeometrySize);

		float* fPoints = *(float**)(((char*)Object) + AmTrigGeometryPtr);

		for (uint32_t iCur = 0; iCur < iNbPoint; iCur++)
		{
			float fCurX = fPoints[iCur*3];
			fPoints[iCur*3] = fCurX - fDiffX;
			float fCurY = fPoints[iCur*3+1];
			fPoints[iCur*3+1] = fCurY - fDiffY;
			float fCurZ = fPoints[iCur*3+2];
			fPoints[iCur*3+2] = fCurZ - fDiffZ;
		}
	}

	//Set the new position
	*(float*)(((char*)Object) + AmCommonPosX) = fX;
	*(float*)(((char*)Object) + AmCommonPosY) = fY;
	*(float*)(((char*)Object) + AmCommonPosZ) = fZ;


	FinishObjectRepositionning(Object, AreaPtr);

	//TODO Send Refresh Message ? 
}

void ObjectSetOrientation(int iObjectId, float fPos)
{
	// It will allow to take care of the collisions when rotating
	NWN::OBJECTID ObjectId = (NWN::OBJECTID)iObjectId;
	GameObject* Object;
	GameObjectManager m_ObjectManager;
	void* AreaPtr;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return;

	AreaPtr = PrepareObjectRepositioning(Object);

	float fPreviousFacing;

	if(Object->GetObjectType() == NWN::OBJECT_TYPE_TRIGGER)
	{
		float fCos = *(float*)((char*)Object + 0x1C);
		float fSin = *(float*)((char*)Object + 0x20);

		float fRad = acosf(fCos);
		float fDeg = fRad * 180.0f / M_PI;

		if (fSin < 0)
		{
			fDeg = fDeg - 360.0f;
			fDeg = fDeg * -1.0f;
		}


		fPreviousFacing = fDeg;
	}

	//Block parameters
	float aFloat[2];
	aFloat[1] = fPos;
	uint32_t vmPtr = 0x864424;
	uint32_t* ptrBlock = *(uint32_t**)vmPtr;
	ptrBlock += 0x5E;

	unsigned char saved[0x1C];

	memcpy_s(saved, 0x1C, ptrBlock, 0x1C);

	memset(ptrBlock, 0, 0x1C);

	((uint8_t*)ptrBlock)[0] = 2;

	ptrBlock[5] = (uint32_t)ptrBlock;
	((uint8_t*)ptrBlock)[0x18] = 1;
	ptrBlock[4] = (uint32_t)aFloat;

	//Block "Caller"
	uint8_t blockCaller[16] = { 0x58, 0xF3, 0x6D, 0x0B, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	blockCaller[8] = (iObjectId & 0xFF);
	blockCaller[9] = (iObjectId & 0xFF00) >> 8;
	blockCaller[10] = (iObjectId & 0xFF0000) >> 16;
	blockCaller[11] = (iObjectId & 0xFF000000) >> 24;


	//Patch the function
	tempPatch[0].Apply();
	tempPatch[1].Apply();
	tempPatch[2].Apply();


	//Call it
	SetOrientationQ(blockCaller, NULL, 0xA, 0x2);

	tempPatch[0].Remove();
	tempPatch[1].Remove();
	tempPatch[2].Remove();


	//Restore Function


	//Restore
	memcpy_s(ptrBlock, 0x1C, saved, 0x1C);


	//if trigger, need to calculate all new points
	if(Object->GetObjectType() == NWN::OBJECT_TYPE_TRIGGER)
	{

		uint32_t iNbPoint = *(uint32_t*)(((char*)Object) + AmTrigGeometrySize);

		float* fPoints = *(float**)(((char*)Object) + AmTrigGeometryPtr);

		float oX = *(float*)(((char*)Object) + AmCommonPosX);
		float oY = *(float*)(((char*)Object) + AmCommonPosY);

		for (uint32_t iCur = 0; iCur < iNbPoint; iCur++)
		{
			float fCurX = fPoints[iCur*3];
			float fCurY = fPoints[iCur*3+1];

			//The real change of angle
			float fAngle = fPos - fPreviousFacing;
			fAngle = fAngle * (M_PI / 180.0f);

			float xM = fCurX - oX;
			float yM = fCurY - oY;

			float x = xM * cos(fAngle) + yM * sin(fAngle) + oX;
			float y = 0 - xM * sin(fAngle) + yM * cos(fAngle) + oY;

			fPoints[iCur * 3] = x;
			fPoints[iCur * 3 + 1] = y;
		}
	}



	FinishObjectRepositionning(Object, AreaPtr);
}

void ObjectSetPositionAndOrientation(int iObjectId, float fX, float fY, float fZ, float fFacing)
{
	NWN::OBJECTID ObjectId = (NWN::OBJECTID)iObjectId;
	GameObject* Object;
	GameObjectManager m_ObjectManager;
	void* AreaPtr;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return;

	AreaPtr = PrepareObjectRepositioning(Object);


	if (Object->GetObjectType() == NWN::OBJECT_TYPE_TRIGGER)
	{
		float fDiffX = *(float*)(((char*)Object) + AmCommonPosX);
		fDiffX = fDiffX - fX;
		float fDiffY = *(float*)(((char*)Object) + AmCommonPosY);
		fDiffY = fDiffY - fY;
		float fDiffZ = *(float*)(((char*)Object) + AmCommonPosZ);
		fDiffZ = fDiffZ - fZ;

		uint32_t iNbPoint = *(uint32_t*)(((char*)Object) + AmTrigGeometrySize);

		float* fPoints = *(float**)(((char*)Object) + AmTrigGeometryPtr);

		for (uint32_t iCur = 0; iCur < iNbPoint; iCur++)
		{
			float fCurX = fPoints[iCur*3];
			fPoints[iCur*3] = fCurX - fDiffX;
			float fCurY = fPoints[iCur*3+1];
			fPoints[iCur*3+1] = fCurY - fDiffY;
			float fCurZ = fPoints[iCur*3+2];
			fPoints[iCur*3+2] = fCurZ - fDiffZ;
		}
	}

	//Set the new position
	*(float*)(((char*)Object) + AmCommonPosX) = fX;
	*(float*)(((char*)Object) + AmCommonPosY) = fY;
	*(float*)(((char*)Object) + AmCommonPosZ) = fZ;

	ObjectSetOrientation(iObjectId, fFacing);

	FinishObjectRepositionning(Object, AreaPtr);

	//TODO Send Refresh Message ? 
}


std::string TriggerGetGeometry(int iObjectId) {

	std::string retGeometry = "";
	NWN::OBJECTID ObjectId = (NWN::OBJECTID)iObjectId;
	GameObject* Object;
	GameObjectManager m_ObjectManager;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return retGeometry;


	if (Object->GetObjectType() == NWN::OBJECT_TYPE_TRIGGER)
	{
		uint32_t iNbPoint = *(uint32_t*)(((char*)Object) + AmTrigGeometrySize);
		uint32_t iSizeBuff = *(uint32_t*)(((char*)Object) + AmTrigGeometryBufSize);
		float* fPoints = *(float**)(((char*)Object) + AmTrigGeometryPtr);

		std::stringstream ss;
		for (size_t i = 0; i < iNbPoint; i++) {
			ss << fPoints[i] << " ";
		}

		retGeometry = ss.str();
	}

	return retGeometry;
}

void TriggerChangeGeometry(int iObjectId, std::vector<float>& vGeometry) {

	NWN::OBJECTID ObjectId = (NWN::OBJECTID)iObjectId;
	GameObject* Object;
	GameObjectManager m_ObjectManager;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return;


	if (Object->GetObjectType() == NWN::OBJECT_TYPE_TRIGGER)
	{
		uint32_t iNbPoint = *(uint32_t*)(((char*)Object) + AmTrigGeometrySize);
		uint32_t iSizeBuff = *(uint32_t*)(((char*)Object) + AmTrigGeometryBufSize);

		float* fPoints;
		float* fOldPoints;
		if (iSizeBuff*3 < vGeometry.size())
		{
			//Need to create the datablock.
			NWN2_HeapMgr *pHeapMgr = NWN2_HeapMgr::Instance();
			NWN2_Heap *pHeap = pHeapMgr->GetDefaultHeap();
			fPoints = (float*)pHeap->Allocate(4*vGeometry.size()*3);

			//Need to save the old datablock addr
			fOldPoints = *(float**)(((char*)Object) + AmTrigGeometryPtr);
		} else {
			fPoints = *(float**)(((char*)Object) + AmTrigGeometryPtr);
		}
		//Change the datablock used, free the old one.
		iNbPoint = vGeometry.size()/3;

		for (uint32_t iCur = 0; iCur < iNbPoint; iCur++)
		{
			fPoints[iCur*3] = vGeometry[iCur*3];
			fPoints[iCur*3+1] = vGeometry[iCur*3+1];
			fPoints[iCur*3+2] = vGeometry[iCur*3+2];
		}

		if(iSizeBuff*3 < vGeometry.size())
		{

			//free the old one
			if (NWN2Heap_Deallocate)
				NWN2Heap_Deallocate( fOldPoints );

			//Replace size & ptr
			*(float**)(((char*)Object) + AmTrigGeometryPtr) = fPoints;
			*(uint32_t*)(((char*)Object) + AmTrigGeometryBufSize) = iNbPoint;
		}
		*(uint32_t*)(((char*)Object) + AmTrigGeometrySize) = iNbPoint;
		*(uint32_t*)(((char*)Object) + AmTrigGeometrySize2) = iNbPoint;
	}
}


#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <cstring>

#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include <misc/Patch.h>

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
		if (MyFlagsAddr == NULL)
		{
			FinishObjectRepositionning(Object, AreaPtr);
			return;
		}
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


// The engine caches the bounding box of a trigger (+0x3E8..+0x3F4, 0x6C9E00) and only computes it again when min == max:
// to be called each time the points of a trigger are moved, turned or replaced.
static void TriggerInvalidateBounds(GameObject* Object)
{
	char* p = (char*)Object;
	*(float*)(p + AmTrigBBoxMinX) = 0.0f;
	*(float*)(p + AmTrigBBoxMaxX) = 0.0f;
	*(float*)(p + AmTrigBBoxMinY) = 0.0f;
	*(float*)(p + AmTrigBBoxMaxY) = 0.0f;
}

void ObjectSetOrientationCore(int iObjectId, GameObject* Object, float fPos);

bool g_bRefuseOutsideZone = true;

#define FCT_AREAUPDATEOBJPOS	0x0060FF90	// int __thiscall (CNWSArea*, CNWSObject*, const float* oldMin, const float* oldMax), ret 0xC
#define VTBL_SETPOSITION		0x138		// void __thiscall (CNWSObject*, const float* pos, int bNoAreaUpdate), ret 8 (0x57F360 in every vtable)

__declspec(naked) int __fastcall AreaUpdateObjectPosition_Intern(__in void* ptrArea, __in void* Unused,
	__in void* ptrObject, __in const float* oldMin, __in const float* oldMax)
{
	__asm
	{
		mov		edx, FCT_AREAUPDATEOBJPOS;
		jmp		edx;
	}
}

typedef void (__fastcall *ObjSetPosition_Proc)(void* ptrObject, void* Unused, const float* pos, int bNoAreaUpdate);

// Area (CNWSArea*) of an object, or NULL (inventory, limbo, unknown)
static void* GetObjectAreaPtr(GameObject* Object)
{
	GameObjectManager m_ObjectManager;
	NWN::OBJECTID AreaId = *(NWN::OBJECTID*)((char*)Object + AmCommonArea);

	if (AreaId == NWN::INVALIDOBJID)
		return NULL;
	if ((AreaId & NWN::INVALIDOBJID) != 0)
		AreaId &= ~(NWN::LISTTYPE_MASK);

	GameObject* Area = m_ObjectManager.GetGameObject( AreaId );
	if (Area == NULL || Area->GetObjectType() != NWN::OBJECT_TYPE_AREA)
		return NULL;
	return (void*)((char*)Area - 0xAD0);
}

static bool IsPositionInArea(void* AreaPtr, float fX, float fY, float fZ)
{
	if (!std::isfinite(fX) || !std::isfinite(fY) || !std::isfinite(fZ))
		return false;

	if (!g_bRefuseOutsideZone)
		return true;

	char* desc = *(char**)((char*)AreaPtr + 0xA9C);
	if (desc == NULL)
		return false;

	int   iW = *(int*)(desc + 0xE0);
	int   iH = *(int*)(desc + 0xDC);
	float fT = *(float*)(desc + 0xE8);
	if (iW <= 0 || iH <= 0 || iW * iH > 0x900 || !std::isfinite(fT) || fT <= 0.0f)
		return false;

	return fX >= 0.0f && fX < iW * fT && fY >= 0.0f && fY < iH * fT;
}

#define OFFS_CREATEOBJECT_GUARD		0x006B09B1

unsigned long ContinueCreateObject = 0x006B09B8;
unsigned long RefuseCreateObject   = 0x006B0A6B;


static bool IsPositionOutside(void* AreaPtr, float fX, float fY, float fZ)
{
	if (!std::isfinite(fX) || !std::isfinite(fY) || !std::isfinite(fZ))
		return true;

	char* desc = *(char**)((char*)AreaPtr + 0xA9C);
	if (desc == NULL)
		return false;

	int   iW = *(int*)(desc + 0xE0);
	int   iH = *(int*)(desc + 0xDC);
	float fT = *(float*)(desc + 0xE8);
	if (iW <= 0 || iH <= 0 || iW * iH > 0x900 || !std::isfinite(fT) || fT <= 0.0f)
		return false;

	return !(fX >= 0.0f && fX < iW * fT && fY >= 0.0f && fY < iH * fT);
}

int __fastcall ShouldRefuseCreateObject(void* pZoneObject, void* pLocation)
{
	if (pZoneObject == NULL || pLocation == NULL)
		return 0;

	void* AreaPtr = (void*)((char*)pZoneObject - 0xAD0);
	float fX = *(float*)((char*)pLocation + 0x0);
	float fY = *(float*)((char*)pLocation + 0x4);
	float fZ = *(float*)((char*)pLocation + 0x8);

	return IsPositionOutside(AreaPtr, fX, fY, fZ) ? 1 : 0;
}

__declspec(naked) void CreateObjectZoneGuard()
{
	__asm
	{
		MOV		EAX, dword ptr[ESP + 0x1C]
		CMP		EAX, 1						//creature : left to the engine
		JE		AllowCreate

		PUSH	EAX
		PUSH	ECX
		PUSH	EDX
		MOV		ECX, dword ptr[ESP + 0x18]
		MOV		EDX, EBX
		CALL	ShouldRefuseCreateObject
		TEST	EAX, EAX
		POP		EDX
		POP		ECX
		POP		EAX
		JNZ		RefuseCreate

	AllowCreate:
		CMP		EAX, 0x40
		JMP		dword ptr[ContinueCreateObject]

	RefuseCreate:
		JMP		dword ptr[RefuseCreateObject]
	}
}

Patch _PatchCreateObjectZoneGuard[] =
{
	Patch((DWORD)OFFS_CREATEOBJECT_GUARD, (char*)"\xe9\x00\x00\x00\x00\x90\x90", (int)7),
	Patch((DWORD)OFFS_CREATEOBJECT_GUARD + 1, (relativefunc)CreateObjectZoneGuard),
	Patch()
};
Patch* PatchCreateObjectZoneGuardList = _PatchCreateObjectZoneGuard;

void PatchCreateObjectZoneGuard()
{
	int i = 0;
	while (PatchCreateObjectZoneGuardList[i].Apply()) {
		i++;
	}
}

static void ResortObjectInArea(void* AreaPtr, GameObject* Object)
{
	static const float fZero[3] = { 0.0f, 0.0f, 0.0f };
	const float* pMin = fZero;
	const float* pMax = fZero;

	char* col = *(char**)((char*)Object + 0xD0);
	if (col != NULL)
	{
		char* hdr = *(char**)(col + 0x4BC);
		if (hdr != NULL)
		{
			char* rec = *(char**)(hdr + 0xC) + (*(int*)(col + 0x4C0)) * 0x68;
			pMin = (const float*)rec;
			pMax = (const float*)(rec + 0xC);
		}
	}
	AreaUpdateObjectPosition_Intern(AreaPtr, NULL, (void*)Object, pMin, pMax);
}

// Moves the points of a trigger by the move of its position. Its cached bounding box is invalidated.
static void TriggerTranslatePoints(GameObject* Object, float fX, float fY, float fZ)
{
	float fDiffX = *(float*)(((char*)Object) + AmCommonPosX) - fX;
	float fDiffY = *(float*)(((char*)Object) + AmCommonPosY) - fY;
	float fDiffZ = *(float*)(((char*)Object) + AmCommonPosZ) - fZ;

	uint32_t iNbPoint = *(uint32_t*)(((char*)Object) + AmTrigGeometrySize);
	float* fPoints = *(float**)(((char*)Object) + AmTrigGeometryPtr);

	if (fPoints != NULL)
	{
		for (uint32_t iCur = 0; iCur < iNbPoint; iCur++)
		{
			fPoints[iCur*3]   = fPoints[iCur*3]   - fDiffX;
			fPoints[iCur*3+1] = fPoints[iCur*3+1] - fDiffY;
			fPoints[iCur*3+2] = fPoints[iCur*3+2] - fDiffZ;
		}
	}
	TriggerInvalidateBounds(Object);
}

static bool IsMovableType(uint8_t t)
{
	return t == 7 || t == 9 || t == 0xA || t == 0xC || t == 0x15 || t == 0x16;	// trigger, placeable, door, waypoint, light, placed effect
}

static void ObjectSetPositionCommon(int iObjectId, float fX, float fY, float fZ, bool bFacing, float fFacing)
{
	NWN::OBJECTID ObjectId = (NWN::OBJECTID)iObjectId;
	GameObject* Object;
	GameObjectManager m_ObjectManager;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return;

	uint8_t tObj = *(uint8_t*)((char*)Object + AmObjectType);
	if (!IsMovableType(tObj))
		return;

	if (bFacing && !std::isfinite(fFacing))
		return;

	void* AreaPtr = GetObjectAreaPtr(Object);
	if (AreaPtr == NULL)
		return;

	//Attached to a parent node
	if (*(uint32_t*)((char*)Object + 0x80) != 0)
		return;

	if (!IsPositionInArea(AreaPtr, fX, fY, fZ))
		return;

	if (tObj == NWN::OBJECT_TYPE_PLACEABLE || tObj == NWN::OBJECT_TYPE_DOOR)
	{
		//Grid : remove, write the position, rebuild the collision, add again, then sort the list of the zone
		AreaPtr = PrepareObjectRepositioning(Object);

		*(float*)(((char*)Object) + AmCommonPosX) = fX;
		*(float*)(((char*)Object) + AmCommonPosY) = fY;
		*(float*)(((char*)Object) + AmCommonPosZ) = fZ;

		if (bFacing)
			ObjectSetOrientationCore(iObjectId, Object, fFacing);

		FinishObjectRepositionning(Object, AreaPtr);

		if (AreaPtr != NULL)
			ResortObjectInArea(AreaPtr, Object);
		return;
	}

	//Not in the grid : the engine writes the position and sorts the list of the zone
	if (tObj == NWN::OBJECT_TYPE_TRIGGER)
		TriggerTranslatePoints(Object, fX, fY, fZ);

	float fPos[3] = { fX, fY, fZ };
	void** vtbl = *(void***)Object;
	((ObjSetPosition_Proc)vtbl[VTBL_SETPOSITION / 4])((void*)Object, NULL, fPos, 0);

	if (bFacing)
		ObjectSetOrientationCore(iObjectId, Object, fFacing);
}

void ObjectSetPosition(int iObjectId, float fX, float fY, float fZ)
{
	ObjectSetPositionCommon(iObjectId, fX, fY, fZ, false, 0.0f);
}

void ObjectSetOrientationCore(int iObjectId, GameObject* Object, float fPos)
{
	float fPreviousFacing;

	if(Object->GetObjectType() == NWN::OBJECT_TYPE_TRIGGER)
	{
		float fCos = *(float*)((char*)Object + 0x1C);
		float fSin = *(float*)((char*)Object + 0x20);

		// atan2f: acosf gives NaN when fCos is slightly over 1 because of rounding
		float fDeg = atan2f(fSin, fCos) * 180.0f / M_PI;

		if (fDeg < 0)
			fDeg = fDeg + 360.0f;


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
		TriggerInvalidateBounds(Object);
	}
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

	NWN::OBJECT_TYPE tObj = Object->GetObjectType();
	if (tObj != NWN::OBJECT_TYPE_TRIGGER && tObj != NWN::OBJECT_TYPE_PLACEABLE && tObj != NWN::OBJECT_TYPE_DOOR &&
		tObj != NWN::OBJECT_TYPE_LIGHT && tObj != NWN::OBJECT_TYPE_PLACED_EFFECT)
		return;

	AreaPtr = PrepareObjectRepositioning(Object);

	ObjectSetOrientationCore(iObjectId, Object, fPos);

	FinishObjectRepositionning(Object, AreaPtr);
}

void ObjectSetPositionAndOrientation(int iObjectId, float fX, float fY, float fZ, float fFacing)
{
	ObjectSetPositionCommon(iObjectId, fX, fY, fZ, true, fFacing);
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
		ss << iNbPoint << " ";
		for (size_t i = 0; i < iNbPoint * 3; i++) {
			ss << fPoints[i] << " ";
		}

		retGeometry = ss.str();
	}

	return retGeometry;
}


#define AM_TRIG_MAX_POINTS	255

// Same rule as the engine when it adds a point (0x420FB0): compared to every point already kept, in 3D, |d| <= 1.1e-7
static uint32_t DedupeLikeEngine(const std::vector<float>& vIn, std::vector<float>& vOut)
{
	const float fEpsilon = 1.1e-7f;		// 0x7E958C
	vOut.clear();
	vOut.reserve(vIn.size());
	for (size_t i = 0; i + 2 < vIn.size(); i += 3)
	{
		bool bDup = false;
		for (size_t j = 0; j < vOut.size() && !bDup; j += 3)
		{
			bDup = fabsf(vOut[j] - vIn[i]) <= fEpsilon &&
				fabsf(vOut[j+1] - vIn[i+1]) <= fEpsilon &&
				fabsf(vOut[j+2] - vIn[i+2]) <= fEpsilon;
		}
		if (!bDup)
		{
			vOut.push_back(vIn[i]);
			vOut.push_back(vIn[i+1]);
			vOut.push_back(vIn[i+2]);
		}
	}
	return (uint32_t)(vOut.size() / 3);
}

bool TriggerChangeGeometry(int iObjectId, std::vector<float>& vInput) {

	NWN::OBJECTID ObjectId = (NWN::OBJECTID)iObjectId;
	GameObject* Object;
	GameObjectManager m_ObjectManager;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return false;

	if (Object->GetObjectType() != NWN::OBJECT_TYPE_TRIGGER)
		return false;

	char* pTrig = (char*)Object;

	for (size_t i = 0; i < vInput.size(); i++)
	{
		if (!std::isfinite(vInput[i]))
			return false;
	}

	std::vector<float> vGeometry;
	uint32_t iNbPoint = DedupeLikeEngine(vInput, vGeometry);
	if (iNbPoint < 3 || iNbPoint > AM_TRIG_MAX_POINTS)
		return false;

	uint32_t  iCapPoints = *(uint32_t*)(pTrig + AmTrigGeometryBufSize);
	float*    fOldPoints = *(float**)(pTrig + AmTrigGeometryPtr);
	uint32_t  iOldIdxNb  = *(uint32_t*)(pTrig + AmTrigIndexNb);
	uint32_t* pOldIdx    = *(uint32_t**)(pTrig + AmTrigIndexPtr);

	NWN2_HeapMgr *pHeapMgr = NWN2_HeapMgr::Instance();
	NWN2_Heap *pHeap = pHeapMgr->GetDefaultHeap();
	if (pHeap == NULL)
		return false;

	float* fNewPoints = NULL;
	if (fOldPoints == NULL || iCapPoints < iNbPoint)
	{
		fNewPoints = (float*)pHeap->Allocate(iNbPoint * 3 * sizeof(float));
		if (fNewPoints == NULL)
			return false;
	}

	uint32_t* pNewIdx = NULL;
	if (pOldIdx == NULL || iOldIdxNb < iNbPoint)
	{
		pNewIdx = (uint32_t*)pHeap->Allocate(iNbPoint * sizeof(uint32_t));
		if (pNewIdx == NULL)
		{
			if (fNewPoints != NULL && NWN2Heap_Deallocate)
				NWN2Heap_Deallocate( fNewPoints );
			return false;
		}
	}

	
	float* fPoints = (fNewPoints != NULL) ? fNewPoints : fOldPoints;
	memcpy(fPoints, vGeometry.data(), iNbPoint * 3 * sizeof(float));

	uint32_t* pIdx = (pNewIdx != NULL) ? pNewIdx : pOldIdx;
	for (uint32_t iCur = 0; iCur < iNbPoint; iCur++)
		pIdx[iCur] = iCur;


	if (fNewPoints != NULL)
	{
		*(float**)(pTrig + AmTrigGeometryPtr) = fNewPoints;
		*(uint32_t*)(pTrig + AmTrigGeometryBufSize) = iNbPoint;
		if (fOldPoints != NULL && NWN2Heap_Deallocate)
			NWN2Heap_Deallocate( fOldPoints );
	}
	if (pNewIdx != NULL)
	{
		*(uint32_t**)(pTrig + AmTrigIndexPtr) = pNewIdx;
		if (pOldIdx != NULL && NWN2Heap_Deallocate)
			NWN2Heap_Deallocate( pOldIdx );
	}

	*(uint32_t*)(pTrig + AmTrigGeometrySize)  = iNbPoint;	// +0x36C
	*(uint32_t*)(pTrig + AmTrigGeometrySize2) = iNbPoint;	// +0x378
	*(uint32_t*)(pTrig + AmTrigIndexNb)       = iNbPoint;	// +0x384

	TriggerInvalidateBounds(Object);
	return true;
}

#include "aspectManagerUtils.h"
#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include <misc/Patch.h>
#include "../../septutil/NwN2DataPos.h"
#include "../../septutil/NwN2Utilities.h"

#include "nwn2heap.h"
#include <algorithm>
#include <sstream>
#include <charconv>

typedef void (__cdecl * NWN2Heap_Deallocate_Proc)(void *p);
extern NWN2Heap_Deallocate_Proc NWN2Heap_Deallocate;

char* GetItemModelPartPtr(char* ItemPtr, int iModelPart) {
	char* retValue = NULL;

	switch(iModelPart) {
		case ARMOR_MODEL_PART_HELM:
			retValue = (ItemPtr + AmItmHelm);
			break;
		case ARMOR_MODEL_PART_GLOVES:
			retValue = (ItemPtr + AmItmGloves);
			break;
		case ARMOR_MODEL_PART_BOOTS:
			retValue = (ItemPtr + AmItmBoots);
			break;
		case ARMOR_MODEL_PART_BELT:
			retValue = (ItemPtr + AmItmBelt);
			break;
		case ARMOR_MODEL_PART_CLOAK:
			retValue = (ItemPtr + AmItmCloak);
			break;
		case ARMOR_MODEL_PART_ARMOR:
			retValue = (ItemPtr + AmItmArmors);
			break;
	}

	return retValue;
}

char* GetItemModelPiecePtr(char* ItemPtr, int iModelPiece) {
	char* retValue = NULL;

	switch(iModelPiece) {
		case ARMOR_MODEL_PIECE_LEFT_SHOULDER: 
			retValue = (ItemPtr + AmItmLShoulder);
			break;
		case ARMOR_MODEL_PIECE_RIGHT_SHOULDER: 
			retValue = (ItemPtr + AmItmRShoulder);
			break;
		case ARMOR_MODEL_PIECE_LEFT_BRACER: 
			retValue = (ItemPtr + AmItmLBracer);
			break;
		case ARMOR_MODEL_PIECE_RIGHT_BRACER: 
			retValue = (ItemPtr + AmItmRBracer);
			break;
		case ARMOR_MODEL_PIECE_LEFT_ELBOW: 
			retValue = (ItemPtr + AmItmLElbow);
			break;
		case ARMOR_MODEL_PIECE_RIGHT_ELBOW: 
			retValue = (ItemPtr + AmItmRElbow);
			break;
		case ARMOR_MODEL_PIECE_LEFT_ARM: 
			retValue = (ItemPtr + AmItmLUpArm);
			break;
		case ARMOR_MODEL_PIECE_RIGHT_ARM: 
			retValue = (ItemPtr + AmItmRUpArm);
			break;
		case ARMOR_MODEL_PIECE_LEFT_HIP: 
			retValue = (ItemPtr + AmItmLHip);
			break;
		case ARMOR_MODEL_PIECE_RIGHT_HIP: 
			retValue = (ItemPtr + AmItmRHip);
			break;
		case ARMOR_MODEL_PIECE_FRONT_HIP: 
			retValue = (ItemPtr + AmItmFHip);
			break;
		case ARMOR_MODEL_PIECE_BACK_HIP: 
			retValue = (ItemPtr + AmItmBHip);
			break;
		case ARMOR_MODEL_PIECE_LEFT_LEG: 
			retValue = (ItemPtr + AmItmLUpLeg);
			break;
		case ARMOR_MODEL_PIECE_RIGHT_LEG: 
			retValue = (ItemPtr + AmItmRUpLeg);
			break;
		case ARMOR_MODEL_PIECE_LEFT_SHIN: 
			retValue = (ItemPtr + AmItmLLowLeg);
			break;
		case ARMOR_MODEL_PIECE_RIGHT_SHIN: 
			retValue = (ItemPtr + AmItmRLowLeg);
			break;
		case ARMOR_MODEL_PIECE_LEFT_KNEE: 
			retValue = (ItemPtr + AmItmLKnee);
			break;
		case ARMOR_MODEL_PIECE_RIGHT_KNEE: 
			retValue = (ItemPtr + AmItmRKnee);
			break;
		case ARMOR_MODEL_PIECE_LEFT_FOOT: 
			retValue = (ItemPtr + AmItmLFoot);
			break;
		case ARMOR_MODEL_PIECE_RIGHT_FOOT: 
			retValue = (ItemPtr + AmItmRFoot);
			break;
		case ARMOR_MODEL_PIECE_LEFT_ANKLE: 
			retValue = (ItemPtr + AmItmLAnkle);
			break;
		case ARMOR_MODEL_PIECE_RIGHT_ANKLE: 
			retValue = (ItemPtr + AmItmRAnkle);
			break;
	}

	return retValue;
}

NWN::D3DXCOLOR* GetItemColorPtr(char* ItmPtr, std::string sCommand) {
	char* retVal = NULL;
		if (sCommand == "BaseColor1")
			retVal = (ItmPtr + AmItmBaseColor);
		else if (sCommand == "BaseColor2")
			retVal = (ItmPtr + AmItmBaseColor + 0x10);
		else if (sCommand == "BaseColor3")
			retVal = (ItmPtr + AmItmBaseColor + 0x20);
		else if (sCommand.rfind(cst_ModelPiece, 0) == 0) {
			sCommand = sCommand.substr(cst_ModelPiece.size());
			char* pEnd;
			int iModelPiece = strtol(sCommand.c_str(), &pEnd, 10);

			sCommand = pEnd;

			char* ptrTemp = GetItemModelPiecePtr(ItmPtr, iModelPiece);
			if (ptrTemp != NULL) {
				if (sCommand == "Color2")
					retVal = (ptrTemp + AmItmModelPieceColor2);
				else if (sCommand == "Color3")
					retVal = (ptrTemp + AmItmModelPieceColor3);
				else
					retVal = (ptrTemp + AmItmModelPieceColor1);
			}
		}
		else if (sCommand.rfind(cst_ModelPart, 0) == 0) {
			sCommand = sCommand.substr(cst_ModelPart.size());
			char* pEnd;
			int iModelPart = strtol(sCommand.c_str(), &pEnd, 10);

			sCommand = pEnd;
			char* ptrTemp = GetItemModelPartPtr(ItmPtr, iModelPart);
			if (ptrTemp != NULL) {
				if (sCommand == "Color2")
					retVal = (ptrTemp + AmItmModelPartColor2);
				else if (sCommand == "Color3")
					retVal = (ptrTemp + AmItmModelPartColor3);
				else
					retVal = (ptrTemp + AmItmModelPartColor1);
			}
		}

	return (NWN::D3DXCOLOR*) retVal;
}


void SetItemBaseCost(char* ItemPtr, int iValue)
{
	*(int32_t*)(ItemPtr + AmItmCost) = iValue;
}

int GetItemBaseCost(char* ItemPtr)
{
	return *(int32_t*)(ItemPtr + AmItmCost);
}

void SetItemNonIdentifiedCost(char* ItemPtr, int iValue)
{
	*(int32_t*)(ItemPtr + AmItmNonIdentifiedPrice) = iValue;
}

int GetItemNonIdentifiedCost(char* ItemPtr)
{
	return *(int32_t*)(ItemPtr + AmItmNonIdentifiedPrice);
}

void SetItemModifyCost(char* ItemPtr, int iValue)
{
	*(int32_t*)(ItemPtr + AmItmModifyCost) = iValue;
}

int GetItemModifyCost(char* ItemPtr)
{
	return *(int32_t*)(ItemPtr + AmItmModifyCost);
}

int GetItemGMaterial(char* ItemPtr)
{
	return *(uint16_t*)(ItemPtr + AmItmGMaterial);
}

int GetItemNumberOfDmgRedct(char* ItemPtr)
{
	return *(uint32_t*)(ItemPtr + AmItmDamageReductionNb);
}

int GetItemDmgReductionAmount(char* ItemPtr, uint32_t iDmgRedctIdx)
{
	if (iDmgRedctIdx < GetItemNumberOfDmgRedct(ItemPtr))
	{
		AmItmDmgReduction* myDmgRedcts = *(AmItmDmgReduction**)(ItemPtr + AmItmDamageReductionPtr);
		return myDmgRedcts[iDmgRedctIdx].iAmount;
	}
	return 0;
}

void SetItemDmgReductionAmount(char* ItemPtr, uint32_t iDmgRedctIdx, int iAmount)
{
	if (iDmgRedctIdx < GetItemNumberOfDmgRedct(ItemPtr))
	{
		AmItmDmgReduction* myDmgRedcts = *(AmItmDmgReduction**)(ItemPtr + AmItmDamageReductionPtr);
		int16_t iShortAmount = static_cast<int16_t>(std::clamp(iAmount, static_cast<int>(INT16_MIN), static_cast<int>(INT16_MAX)));
		myDmgRedcts[iDmgRedctIdx].iAmount = iShortAmount;
	}
}

int GetItmDmgReductionUseOrLogic(char* ItemPtr, uint32_t iDmgRedctIdx)
{
	if (iDmgRedctIdx < GetItemNumberOfDmgRedct(ItemPtr))
	{
		AmItmDmgReduction* myDmgRedcts = *(AmItmDmgReduction**)(ItemPtr + AmItmDamageReductionPtr);
		return myDmgRedcts[iDmgRedctIdx].uDmgRedFlags;
	}
	return 0;
}

void SetItmDmgReductionUseOrLogic(char* ItemPtr, uint32_t iDmgRedctIdx, int iUseOrLogic)
{
	if (iDmgRedctIdx < GetItemNumberOfDmgRedct(ItemPtr))
	{
		AmItmDmgReduction* myDmgRedcts = *(AmItmDmgReduction**)(ItemPtr + AmItmDamageReductionPtr);
		myDmgRedcts[iDmgRedctIdx].uDmgRedFlags = (iUseOrLogic!=0?1:0);
	}
}


int GetItmDmgRedctNumberPiercingType(char* ItemPtr, uint32_t iDmgRedctIdx)
{
	if (iDmgRedctIdx < GetItemNumberOfDmgRedct(ItemPtr))
	{
		AmItmDmgReduction* myDmgRedcts = *(AmItmDmgReduction**)(ItemPtr + AmItmDamageReductionPtr);
		return myDmgRedcts[iDmgRedctIdx].uSizeList;
	}
	return 0;
}

int GetItmDmgRedctPiercingType(char* ItemPtr, uint32_t iDmgRedctIdx, uint32_t iPiercingIdx)
{
	if (iDmgRedctIdx < GetItemNumberOfDmgRedct(ItemPtr))
	{
		AmItmDmgReduction* myDmgRedcts = *(AmItmDmgReduction**)(ItemPtr + AmItmDamageReductionPtr);

		if (iPiercingIdx < myDmgRedcts[iDmgRedctIdx].uSizeList)
		{
			AmItmDmgReductionType* myRdctType = (AmItmDmgReductionType*)(myDmgRedcts[iDmgRedctIdx].pDmgRedTypeList);
			return myRdctType->DmgRedctType;
		}
	}
	return 0;
}

void SetItmDmgRedctPiercingType(char* ItemPtr, uint32_t iDmgRedctIdx, uint32_t iPiercingIdx, int iType)
{
	if (iDmgRedctIdx < GetItemNumberOfDmgRedct(ItemPtr))
	{
		AmItmDmgReduction* myDmgRedcts = *(AmItmDmgReduction**)(ItemPtr + AmItmDamageReductionPtr);

		if (iPiercingIdx < myDmgRedcts[iDmgRedctIdx].uSizeList)
		{
			AmItmDmgReductionType* myRdctType = (AmItmDmgReductionType*)(myDmgRedcts[iDmgRedctIdx].pDmgRedTypeList);
			myRdctType->DmgRedctType = (iType&0xFFFF);
		}
	}
}


int GetItmDmgRedctPiercingSubType(char* ItemPtr, uint32_t iDmgRedctIdx, uint32_t iPiercingIdx)
{
	if (iDmgRedctIdx < GetItemNumberOfDmgRedct(ItemPtr))
	{
		AmItmDmgReduction* myDmgRedcts = *(AmItmDmgReduction**)(ItemPtr + AmItmDamageReductionPtr);

		if (iPiercingIdx < myDmgRedcts[iDmgRedctIdx].uSizeList)
		{
			AmItmDmgReductionType* myRdctType = (AmItmDmgReductionType*)(myDmgRedcts[iDmgRedctIdx].pDmgRedTypeList);
			return myRdctType->DmgRedctSubType;
		}
	}
	return 0;
}

void SetItmDmgRedctPiercingSubType(char* ItemPtr, uint32_t iDmgRedctIdx, uint32_t iPiercingIdx, int iSubType)
{
	if (iDmgRedctIdx < GetItemNumberOfDmgRedct(ItemPtr))
	{
		AmItmDmgReduction* myDmgRedcts = *(AmItmDmgReduction**)(ItemPtr + AmItmDamageReductionPtr);

		if (iPiercingIdx < myDmgRedcts[iDmgRedctIdx].uSizeList)
		{
			AmItmDmgReductionType* myRdctType = (AmItmDmgReductionType*)(myDmgRedcts[iDmgRedctIdx].pDmgRedTypeList);
			myRdctType[iPiercingIdx].DmgRedctSubType = (iSubType & 0xFFFF);
		}
	}
}



void AddDmgRedctPiercingType(char* ItemPtr, uint32_t iDmgRedctIdx, int iType, int iSubType)
{
	if (iDmgRedctIdx < GetItemNumberOfDmgRedct(ItemPtr))
	{
		AmItmDmgReduction* myDmgRedcts = *(AmItmDmgReduction**)(ItemPtr + AmItmDamageReductionPtr);

		//Need to allocate new array.
		if (myDmgRedcts[iDmgRedctIdx].uSizeList >= myDmgRedcts[iDmgRedctIdx].uSizeArray)
		{
			//Calculate Size
			uint32_t iNewSize = myDmgRedcts[iDmgRedctIdx].uSizeArray * 2;
			uint32_t iAllocationSize = iNewSize * sizeof(AmItmDmgReductionType);

			uint32_t iOldSize = myDmgRedcts[iDmgRedctIdx].uSizeArray * sizeof(AmItmDmgReductionType);

			//Allocate new one
			NWN2_HeapMgr *pHeapMgr = NWN2_HeapMgr::Instance();
			NWN2_Heap *pHeap = pHeapMgr->GetDefaultHeap();
			char* NewDmgRedct = (char*)pHeap->Allocate(iAllocationSize);
			memset(NewDmgRedct, 0, iAllocationSize);

			//Copy old one in the new one
			AmItmDmgReductionType* pOldDmgRdctType = (AmItmDmgReductionType*)(myDmgRedcts[iDmgRedctIdx].pDmgRedTypeList);
			memcpy(NewDmgRedct, pOldDmgRdctType, iOldSize);

			//Make the switch
			myDmgRedcts[iDmgRedctIdx].pDmgRedTypeList = (AmItmDmgReductionType*)NewDmgRedct;
			myDmgRedcts[iDmgRedctIdx].uSizeArray = iNewSize;

			//Free the old one
			if (NWN2Heap_Deallocate)
				NWN2Heap_Deallocate( pOldDmgRdctType );
		}

		//Add the new one at the end of the list
		AmItmDmgReductionType* myRdctType = (AmItmDmgReductionType*)(myDmgRedcts[iDmgRedctIdx].pDmgRedTypeList);
		myRdctType[myDmgRedcts[iDmgRedctIdx].uSizeList].DmgRedctType = (iType & 0xFFFF);
		myRdctType[myDmgRedcts[iDmgRedctIdx].uSizeList].DmgRedctSubType = (iSubType & 0xFFFF);
		myDmgRedcts[iDmgRedctIdx].uSizeList++;
	}
}


void RemoveDmgRedctPiercingType(char* ItemPtr, uint32_t iDmgRedctIdx, uint32_t iPiercingIdx)
{
	if (iDmgRedctIdx < GetItemNumberOfDmgRedct(ItemPtr))
	{
		AmItmDmgReduction* myDmgRedcts = *(AmItmDmgReduction**)(ItemPtr + AmItmDamageReductionPtr);
		uint32_t uTypeNb = myDmgRedcts[iDmgRedctIdx].uSizeList;
		if (uTypeNb > iPiercingIdx)
		{
			//Not the last one
			AmItmDmgReductionType* myPtrDest = (AmItmDmgReductionType*)(myDmgRedcts[iDmgRedctIdx].pDmgRedTypeList);
			if ( (iPiercingIdx + 1) < uTypeNb)
			{
				for (int i = 0; (i + 1 + iPiercingIdx) < uTypeNb; i++)
				{
					myPtrDest[iPiercingIdx + i].DmgRedctType = myPtrDest[iPiercingIdx + i + 1].DmgRedctType;
					myPtrDest[iPiercingIdx + i].DmgRedctSubType = myPtrDest[iPiercingIdx + i + 1].DmgRedctSubType;
				}
			}
			myPtrDest[uTypeNb-1].DmgRedctType = 0;
			myPtrDest[uTypeNb-1].DmgRedctSubType = 0;

			myDmgRedcts[iDmgRedctIdx].uSizeList--;
		}
	}
}


void AddDmgRedct(char* ItemPtr, int iAmount, int iUseOrLogic)
{
	uint32_t iArraySize = *(uint32_t*)(ItemPtr + AmItmDamageReductionSizeArray);
	uint32_t iNb = *(uint32_t*)(ItemPtr + AmItmDamageReductionNb);

	if (iNb >= iArraySize)
	{
		uint32_t oldSize = iArraySize * sizeof(AmItmDmgReduction);
		uint32_t newSize = oldSize * 2;

		if (iArraySize == 0)
		{
			oldSize = 0;
			newSize = 0x10 * sizeof(AmItmDmgReduction);
			iArraySize = 0x8;
		}

		//Allocate new one
		NWN2_HeapMgr *pHeapMgr = NWN2_HeapMgr::Instance();
		NWN2_Heap *pHeap = pHeapMgr->GetDefaultHeap();
		char* pNewDmgRect = (char*)pHeap->Allocate(newSize);
		memset(pNewDmgRect, 0, newSize);


		//Copy old one in the new one
		AmItmDmgReduction* pOldDmgRdct;
		if (oldSize != 0)
		{
			pOldDmgRdct = *(AmItmDmgReduction**)(ItemPtr + AmItmDamageReductionPtr);
			memcpy(pNewDmgRect, pOldDmgRdct, oldSize);
		}

		//Make the switch
		*(AmItmDmgReduction**)(ItemPtr + AmItmDamageReductionPtr) = (AmItmDmgReduction*)pNewDmgRect;
		*(uint32_t*)(ItemPtr + AmItmDamageReductionSizeArray) = iArraySize*2;

		//Free the old one
		if (oldSize != 0)
		{
			if (NWN2Heap_Deallocate)
				NWN2Heap_Deallocate(pOldDmgRdct);
		}
	}

	*(uint32_t*)(ItemPtr + AmItmDamageReductionNb) = (iNb + 1);
	AmItmDmgReduction* myDmgRedcts = *(AmItmDmgReduction**)(ItemPtr + AmItmDamageReductionPtr);
	myDmgRedcts[iNb].iAmount = iAmount;
	myDmgRedcts[iNb].uDmgRedFlags = ((iUseOrLogic!=0)?1:0);


	//Allocate the RedctType
	NWN2_HeapMgr *pHeapMgr = NWN2_HeapMgr::Instance();
	NWN2_Heap *pHeap = pHeapMgr->GetDefaultHeap();
	AmItmDmgReductionType* pNewDmgRectType = (AmItmDmgReductionType*)pHeap->Allocate(0x10*sizeof(AmItmDmgReductionType));
	myDmgRedcts[iNb].pDmgRedTypeList = pNewDmgRectType;
	myDmgRedcts[iNb].uSizeArray = 0x10;
	myDmgRedcts[iNb].uSizeList = 0;

}


void RemoveDmgRedct(char* ItemPtr, uint32_t iDmgRedctIdx)
{
	int iNumberDmgRedct = GetItemNumberOfDmgRedct(ItemPtr);
	if (iDmgRedctIdx >= 0 && iDmgRedctIdx < iNumberDmgRedct)
	{
		//Free the DmgReductionType list.
		AmItmDmgReduction* myDmgRedcts = *(AmItmDmgReduction**)(ItemPtr + AmItmDamageReductionPtr);
		AmItmDmgReductionType* pOldDmgRdctType = (AmItmDmgReductionType*)(myDmgRedcts[iDmgRedctIdx].pDmgRedTypeList);
		if (NWN2Heap_Deallocate)
			NWN2Heap_Deallocate( pOldDmgRdctType );


		//If not last one
		if ((iDmgRedctIdx + 1) < iNumberDmgRedct)
		{
			for (int i = 0; (i + 1 + iDmgRedctIdx) < iNumberDmgRedct; i++)
			{
				myDmgRedcts[iDmgRedctIdx+i].iAmount = myDmgRedcts[iDmgRedctIdx+1+i].iAmount;
				myDmgRedcts[iDmgRedctIdx+i].pDmgRedTypeList = myDmgRedcts[iDmgRedctIdx+1+i].pDmgRedTypeList;
				myDmgRedcts[iDmgRedctIdx+i].uDmgRedFlags = myDmgRedcts[iDmgRedctIdx+1+i].uDmgRedFlags;
				myDmgRedcts[iDmgRedctIdx+i].uSizeArray = myDmgRedcts[iDmgRedctIdx+1+i].uSizeArray;
				myDmgRedcts[iDmgRedctIdx+i].uSizeList = myDmgRedcts[iDmgRedctIdx+1+i].uSizeList;
			}

		}

		//last one is now not here.
		myDmgRedcts[iNumberDmgRedct - 1].pDmgRedTypeList = NULL;
		myDmgRedcts[iNumberDmgRedct - 1].uSizeList = 0;
		myDmgRedcts[iNumberDmgRedct - 1].uSizeArray = 0;
		(*(uint32_t*)(ItemPtr + AmItmDamageReductionNb))--;
	}
}


#define FUNC_PREPAREMSGITEMPROPERTY 0x5d8090
__declspec(naked) void __fastcall  CallPrepareMsgItemProperty(void* pItem, void* Unused, int iValue)
{
	__asm
	{
		mov		edx, FUNC_PREPAREMSGITEMPROPERTY;
		jmp		edx;
	}
}

void ChargesNumber(char* ItemPtr, int iCharges)
{
	if (iCharges < 0)
		iCharges = 0;

	int iPrevious =  *(int *)((int)ItemPtr + AmItmCharges);
	*(uint32_t *)((int)ItemPtr + AmItmCharges) = iCharges;


	int iNumberOfProp = *(int*)((int)ItemPtr + AmItmProperty0Nb);
	if ((iPrevious < iCharges) && (0 < iNumberOfProp))
	{
		int iProp = 0;
		for(int iProp=0; iProp < iNumberOfProp; iProp++)
		{
			AmItmProperty* myProperty = &(((AmItmProperty*)((int)ItemPtr + AmItmProperty0Ptr))[iProp]);
			int iTest = 0;
			//If castspell property
			if (myProperty->uPropertyName == 0xF)
			{
				switch (myProperty->uCostValue)
				{
					case 2:
						iTest = 5;
						break;
					case 3 :
						iTest = 4;
						break;
					case 4 : 
						iTest = 3;
						break;
					case 5:
						iTest = 2;
						break;
					case 6:
						iTest = 1;
						break;
				}

				if (iTest > 0 && iCharges >= iTest)
					myProperty->uUseable = 1;
			}
		}
	}
	CallPrepareMsgItemProperty(ItemPtr, NULL, 0);
}




void SetItemBaseItem(char* ItemPtr, int iValue) {
	*(uint32_t*)(ItemPtr + AmItmType) = iValue;
}


void SetItemAppVariation(char* ItemPtr, int iValue) {
	*(uint8_t*)(ItemPtr + AmItmVariation) = (uint8_t)iValue;
}

int GetItemAppVariation(char* ItemPtr) {
	return *(uint8_t*)(ItemPtr + AmItmVariation);
}

void SetItemAppCategory(char* ItemPtr, int iValue) {
	*(uint8_t*)(ItemPtr + AmItmCategory) = (uint8_t)iValue;
}

int GetItemAppCategory(char* ItemPtr) {
	return *(uint8_t*)(ItemPtr + AmItmCategory);
}



void SetItemFXWpn(char* ItemPtr, int iValue) {
	*(uint8_t*)(ItemPtr + AmItmFXWpn) = (uint8_t)iValue;
}

int GetItemFXWpn(char* ItemPtr) {
	return *(uint8_t*)(ItemPtr + AmItmFXWpn);
}



void SetItemModelPieceAccessory(char* ItemPtr, int iModelPiece, int iValue) {
	char* ptrModelPiece = GetItemModelPiecePtr(ItemPtr, iModelPiece);
	if (ptrModelPiece != NULL)
		*(uint8_t*)ptrModelPiece = (uint8_t)iValue;
}

int GetItemModelPieceAccessory(char* ItemPtr, int iModelPiece) {
	char* ptrModelPiece = GetItemModelPiecePtr(ItemPtr, iModelPiece);
	if (ptrModelPiece != NULL)
		return *(uint8_t*)ptrModelPiece;

	return 0;
}

void SetItemModelPieceUVScroll(char* ItemPtr, int iModelPiece, bool bValue) {
	char* ptrModelPiece = GetItemModelPiecePtr(ItemPtr, iModelPiece);
	if (ptrModelPiece != NULL)
		*(uint8_t*)(ptrModelPiece + AmItmModelPieceUVSCroll) = (uint8_t)bValue?1:0;
}

int GetItemModelPieceUVScroll(char* ItemPtr, int iModelPiece) {
	char* ptrModelPiece = GetItemModelPiecePtr(ItemPtr, iModelPiece);
	if (ptrModelPiece != NULL)
		return *(uint8_t*)(ptrModelPiece + AmItmModelPieceUVSCroll);

	return 0;
}

void SetItemModelPartVariation(char* ItemPtr, int iModelPart, int iValue) {
	char* ptrModelPiece = GetItemModelPartPtr(ItemPtr, iModelPart);
	if (ptrModelPiece != NULL)
		*(uint8_t*)(ptrModelPiece + AmItmModelPartVariation) = (uint8_t)iValue;
}

int GetItemModelPartVariation(char* ItemPtr, int iModelPart) {
	char* ptrModelPiece = GetItemModelPartPtr(ItemPtr, iModelPart);
	if (ptrModelPiece != NULL)
		return *(uint8_t*)(ptrModelPiece + AmItmModelPartVariation);

	return 0;
}

void SetItemModelPartCategory(char* ItemPtr, int iModelPart, int iValue) {
	char* ptrModelPiece = GetItemModelPartPtr(ItemPtr, iModelPart);
	if (ptrModelPiece != NULL)
		*(uint8_t*)(ptrModelPiece + AmItmModelPartCategory) = (uint8_t)iValue;
}

int GetItemModelPartCategory(char* ItemPtr, int iModelPart) {
	char* ptrModelPiece = GetItemModelPartPtr(ItemPtr, iModelPart);
	if (ptrModelPiece != NULL)
		return *(uint8_t*)(ptrModelPiece + AmItmModelPartCategory);

	return 0;
}


void SetItemWpnPart1(char* ItemPtr, int iValue) {
	*(uint8_t*)(ItemPtr + AmItmWpnPart1) = (uint8_t)(iValue & 0xFF);
}

int GetItemWpnPart1(char* ItemPtr) {
	return *(uint8_t*)(ItemPtr + AmItmWpnPart1);
}


void SetItemWpnPart2(char* ItemPtr, int iValue) {
	*(uint8_t*)(ItemPtr + AmItmWpnPart2) = (uint8_t)(iValue & 0xFF);
}

int GetItemWpnPart2(char* ItemPtr) {
	return *(uint8_t*)(ItemPtr + AmItmWpnPart2);
}


void SetItemWpnPart3(char* ItemPtr, int iValue) {
	*(uint8_t*)(ItemPtr + AmItmWpnPart3) = (uint8_t)(iValue & 0xFF);
}

int GetItemWpnPart3(char* ItemPtr) {
	return *(uint8_t*)(ItemPtr + AmItmWpnPart3);
}

int GetItemModelPartMask(char* ItemPtr) {
	return *(uint8_t*)(ItemPtr + AmItmArmors);
}

void SetItemModelPartMask(char* ItemPtr, int iValue) {
	*(uint8_t*)(ItemPtr + AmItmArmors) = (uint8_t)(iValue & 0xFF);
}


void SetItemModelPieceUScroll(char* ItemPtr, int iModelPiece, float fValue) {
	char* ptrModelPiece = GetItemModelPiecePtr(ItemPtr, iModelPiece);
	if (ptrModelPiece != NULL)
		*(float*)(ptrModelPiece + AmItmModelPieceUScroll) = fValue;
}

float GetItemModelPieceUScroll(char* ItemPtr, int iModelPiece) {
	char* ptrModelPiece = GetItemModelPiecePtr(ItemPtr, iModelPiece);
	if (ptrModelPiece != NULL)
		return *(float*)(ptrModelPiece + AmItmModelPieceUScroll);
	return 0;
}

void SetItemModelPieceVScroll(char* ItemPtr, int iModelPiece, float fValue) {
	char* ptrModelPiece = GetItemModelPiecePtr(ItemPtr, iModelPiece);
	if (ptrModelPiece != NULL)
		*(float*)(ptrModelPiece + AmItmModelPieceVScroll) = fValue;
}

float GetItemModelPieceVScroll(char* ItemPtr, int iModelPiece) {
	char* ptrModelPiece = GetItemModelPiecePtr(ItemPtr, iModelPiece);
	if (ptrModelPiece != NULL)
		return *(float*)(ptrModelPiece + AmItmModelPieceVScroll);
	return 0;
}


void SetItemDescription(char* ItemPtr, char* sValue) {
	setDescription((ItemPtr + AmItmDescrIdent), sValue);
}

std::string GetItemDescription(char* ItemPtr) {
	return getDescription((ItemPtr + AmItmDescrIdent));
}

void SetItemDescriptionNonIdentified(char* ItemPtr, char* sValue) {
	setDescription((ItemPtr + AmItmDescrNonIdent), sValue);
}

std::string GetItemDescriptionNonIdentified(char* ItemPtr) {
	return getDescription((ItemPtr + AmItmDescrNonIdent));
}


class StringSplitter {
	std::string_view m_input;
	size_t m_pos = 0;
	char m_sep;

	static bool parseInt(std::string_view str, int& result) {
		auto res = std::from_chars(str.data(), str.data() + str.size(), result);
		return res.ec == std::errc() && res.ptr == str.data() + str.size();
	}

	static bool parseFloat(std::string_view str, float& result) {
		auto res = std::from_chars(str.data(), str.data() + str.size(), result);
		return res.ec == std::errc() && res.ptr == str.data() + str.size();
	}

public:
	StringSplitter(std::string_view input, char sep = '|')
		: m_input(input), m_sep(sep) {}

	bool next(std::string_view& token) {
		if (m_pos > m_input.size()) return false;
		size_t end = m_input.find(m_sep, m_pos);
		if (end == std::string_view::npos) end = m_input.size();
		token = m_input.substr(m_pos, end - m_pos);
		m_pos = end + 1;
		return true;
	}

	bool skip() {
		std::string_view token;
		return next(token);
	}

	bool nextInt(int& value) {
		std::string_view token;
		return next(token) && parseInt(token, value);
	}

	bool nextFloat(float& value) {
		std::string_view token;
		return next(token) && parseFloat(token, value);
	}

	bool nextStringView(std::string_view& value) {
		return next(value);
	}

	bool nextString(std::string& value) {
		std::string_view token;
		if (!next(token)) return false;
		value = std::string(token);
		return true;
	}
};


bool UnPackAddPropertyParams(const std::string& input,
	int& property, int& subtype, int& costTable, int& costValue,
	int& param1Table, int& param1Value, int& durationType, float& paramFloat)
{
	StringSplitter s(input);
	if (!s.nextInt(property))     return false;
	if (!s.nextInt(subtype))      return false;
	if (!s.nextInt(costTable))    return false;
	if (!s.nextInt(costValue))    return false;
	if (!s.nextInt(param1Table))  return false;
	if (!s.nextInt(param1Value))  return false;
	if (!s.nextInt(durationType)) return false;

	if (durationType == 0x1) {
		if (!s.nextFloat(paramFloat)) return false;
	}
	else if (durationType != 0x2) {
		return false;
	}

	return true;
}

#define FUNC_ALLOCATEEFFECT			0x00655870
#define FUNC_FILLEFFECT				0x00656950
#define FUNC_ALLOCATEIPARAMEFFECT	0x00655e20
#define FUNC_ADDPROPERTYFROMEFFECT	0x00583c90


__declspec(naked) NWN::CGameEffect* __fastcall CallAllocateGameEffect()
{
	__asm
	{
		mov		edx, FUNC_ALLOCATEEFFECT;
		jmp		edx;
	}
}

__declspec(naked) NWN::CGameEffect* __fastcall CallFillGameEffect(NWN::CGameEffect* pEffect, void* Unused, int iParam2)
{
	__asm
	{
		mov		edx, FUNC_FILLEFFECT;
		jmp		edx;
	}
}

__declspec(naked) void __fastcall CallAllocateEffectIntParam(NWN::CGameEffect* pEffect, void* Unused, int iParam2)
{
	__asm
	{
		mov		edx, FUNC_ALLOCATEIPARAMEFFECT;
		jmp		edx;
	}
}

__declspec(naked) void __fastcall AddPropertyFromEffect(NWN::CGameObject* pObject, void* Unused, NWN::CGameEffect* pEffect, int param3, int param4)
{
	__asm
	{
		mov		edx, FUNC_ADDPROPERTYFROMEFFECT;
		jmp		edx;
	}
}


void AddItemProperty(GameObject* pObject, char* pValue)
{
	std::string sValue(pValue);
	//Parse value
	float fDuration;
	int idProperty, subType, costTable, costValue, paramTable, paramValue, durationType;
	idProperty = subType = costTable = costValue = paramTable = paramValue = durationType = -1;
	
	if (!UnPackAddPropertyParams(sValue, idProperty, subType, costTable, costValue, paramTable, paramValue, durationType, fDuration))
		return;



	NWN::CGameEffect* pEffect = CallAllocateGameEffect();
	if (pEffect == 0x0) {
		return;
	}

	pEffect = CallFillGameEffect(pEffect,NULL, 1);
	CallAllocateEffectIntParam(pEffect, NULL, 9);
	
	pEffect->m_nSubType = pEffect->m_nSubType & 0xFFFA | 2;
	pEffect->m_nType = NWN::EFFECT_ITEMPROPERTY;

	pEffect->m_oidCreator = GetModuleID();

	pEffect->m_nParamInteger[7] = 100;
	pEffect->m_nParamInteger[8] = 1;

	//Ok, define effectVariables
	pEffect->m_nParamInteger[0] = idProperty;
	pEffect->m_nParamInteger[1] = subType;
	pEffect->m_nParamInteger[2] = costTable;
	pEffect->m_nParamInteger[3] = costValue;
	pEffect->m_nParamInteger[4] = paramTable;
	pEffect->m_nParamInteger[5] = paramValue;

	/* if Temporary */
	if (durationType == 1) {
		/* remember as temporary */
		pEffect->m_nSubType = (pEffect->m_nSubType & 0xFFF9) | 1;

		/* save the duration */
		pEffect->m_fDuration = (float)fDuration;
	}
	AddPropertyFromEffect(pObject,NULL, pEffect,0,1);

	return;
}

bool IsValidItemProperty(NWN::CGameEffect* pEffect, bool bCountTemp) {
	if (pEffect == 0x0) return false;
	if (pEffect->m_bExpose == 0 || pEffect->m_nType == 0x43) return false;
	uint16_t dt = pEffect->m_nSubType & 0x7;
	if (dt != 1 && dt != 2) return false;
	if (!bCountTemp && dt == 1) return false;
	return true;
}

int GetItemPropertyCounterFromIdx(char* pItem, int iIdx, bool bCountTemp)
{
	uint32_t uNbEffect = *(uint32_t*)(pItem + AmItmEffectListNb);
	if (iIdx >= uNbEffect)
		return -1;

	NWN::CGameEffect** pListpEffect = *(NWN::CGameEffect***)(pItem + AmItmEffectListPtr);
	int remaining = iIdx;

	for(uint32_t  uNumber = 0; uNumber < uNbEffect; uNumber++)
	{
		NWN::CGameEffect* pEffect = pListpEffect[uNumber];

		if (!IsValidItemProperty(pEffect, bCountTemp))
			continue;

		if (remaining == 0)
		{
			return uNumber;
		}
		remaining--;
	}

	return -1;
}

std::string GetPropertyID(char* pItem,  int iIdx, bool bCountTemp)
{
	int iCounter = GetItemPropertyCounterFromIdx(pItem, iIdx, bCountTemp);
	if (iCounter < 0)
		return "";

	NWN::CGameEffect** pListpEffect = *(NWN::CGameEffect***)(pItem + AmItmEffectListPtr);
	NWN::CGameEffect* pEffect = pListpEffect[iCounter];
	std::string s(20, '\0'); // max digits
	auto res = std::to_chars(s.data(), s.data() + s.size(), pEffect->m_nID);
	if (res.ec != std::errc())
		return "";
	s.resize(res.ptr - s.data());
	return s;
}

int GetPropertyNumber(char* pItem, bool bCountTemp)
{
	uint32_t uNbEffect = *(uint32_t*)(pItem + AmItmEffectListNb);
	int iNb = 0;
	NWN::CGameEffect** pListpEffect = *(NWN::CGameEffect***)(pItem + AmItmEffectListPtr);
	for (uint32_t u=0; u < uNbEffect; u++)
	{
		NWN::CGameEffect* pEffect = pListpEffect[u];

		if (IsValidItemProperty(pEffect, bCountTemp))
			iNb++;
		/*
		if (pEffect->m_nType != NWN::EFFECT_ITEMPROPERTY)
			continue;

		if(bCountTemp || !(pEffect->m_nSubType & 1) )
			iNb++;
		*/
	}

	return iNb;
}

int GetItemPropertyCounterFromID(char* pItem, uint64_t uID)
{
	uint32_t uNbEffect = *(uint32_t*)(pItem + AmItmEffectListNb);
	NWN::CGameEffect** pListpEffect = *(NWN::CGameEffect***)(pItem + AmItmEffectListPtr);


	for(uint32_t  uNumber = 0; uNumber < uNbEffect; uNumber++)
	{
		NWN::CGameEffect* pEffect = pListpEffect[uNumber];

		if (pEffect == 0x0)
			continue;

		//Don't care about type, duration etc. Just test the id
		if (pEffect->m_nID == uID)
			return uNumber;
	}

	return -1;
}


#define OFFS_GetNextItemPropPatchEntry 0x006a58ac
#define OFFS_GetNextItemPropPatchExit 0x006a58fa
#define OFFS_GetItemPropTypeGetIDInit 0x0067e200
#define OFFS_GetItemPropTypeGetID	0x0067e258

unsigned long ReturnGetNextItemPropEntry = 0x006a58b3;

unsigned long ReturnGetNextItemPropExitNotFound = 0x006a58ff;
unsigned long ReturnGetNextItemPropExitFound = 0x006a5950;

unsigned long ReturnGetItemPropTypeGetIDInit = 0x0067e206;
unsigned long ReturnGetItemPropTypeGetID = 0x0067e25d;


struct GetNextItemPatchState {
	int bHooked = 0;
	void* pExpectedItem = 0;
	int iCounter = -1;
} g_specialGetItemProperty;

__declspec(naked) void GetNextItemPropEntryPatch()
{
	__asm
	{
		CMP g_specialGetItemProperty.bHooked, 1
		JNE GetNxtItmPropEntry_NoPatch

		CMP g_specialGetItemProperty.pExpectedItem, EDI
		JNE GetNxtItmPropEntry_WrongItem

		CMP g_specialGetItemProperty.iCounter, -1
		JLE GetNxtItmPropEntry_InvalidIndex

		MOV ECX, g_specialGetItemProperty.iCounter
		JMP GetNxtItmPropEntry_AfterPatch


	GetNxtItmPropEntry_InvalidIndex:
		MOVZX ECX, word ptr [EDI + 0x1a4]
		ADD ECX, 1
		MOV g_specialGetItemProperty.bHooked, 0
		JMP GetNxtItmPropEntry_AfterPatch

	GetNxtItmPropEntry_WrongItem:
		MOV g_specialGetItemProperty.bHooked, 0
	GetNxtItmPropEntry_NoPatch:
		MOVZX ECX, word ptr [EDI + 0x2C4]
	GetNxtItmPropEntry_AfterPatch:
		JMP dword ptr[ReturnGetNextItemPropEntry]
	}
}

__declspec(naked) void GetNextItemPropExitPatch()
{
	__asm
	{
		CMP CX, SI
		POP EBX
		JL GetNxtItmPropExit_Found

		MOV g_specialGetItemProperty.bHooked, 0
		JMP dword ptr[ReturnGetNextItemPropExitNotFound]

	GetNxtItmPropExit_Found:
		MOV EDX, [EDI + 0x1A0]
		MOV ESI, [EDX + ECX*4]

		CMP g_specialGetItemProperty.bHooked, 1		
		JE GetNxtItmPropExit_AfterFound

		ADD ECX, EBP
		MOV word ptr [EDI + 0x2c4],CX


	GetNxtItmPropExit_AfterFound:
		MOV g_specialGetItemProperty.bHooked, 0
		JMP dword ptr[ReturnGetNextItemPropExitFound]
	}
}


struct GetItemPropertyTypeHookID {
	uint32_t uIDP1 = 0;
	uint32_t uIDP2 = 0;
} g_itemPropertyTypeHookID;


__declspec(naked) void GetItemPropTypeExtractIDInit()
{
	__asm
	{
		MOV g_itemPropertyTypeHookID.uIDP1, 0
		MOV g_itemPropertyTypeHookID.uIDP2, 0

		MOV EDX, dword ptr ds:[0x00864424]
		JMP dword ptr[ReturnGetItemPropTypeGetIDInit]
	}
}

__declspec(naked) void GetItemPropTypeExtractID()
{
	__asm
	{
		MOV EAX, dword ptr [ECX]
		MOV g_itemPropertyTypeHookID.uIDP1, EAX

		MOV EAX, dword ptr [ECX + 4]
		MOV g_itemPropertyTypeHookID.uIDP2, EAX

		POP EAX
		MOV EAX, dword ptr [ECX + 0x40]
		MOV EAX, dword ptr [EAX]

		JMP dword ptr[ReturnGetItemPropTypeGetID]
	}
}

Patch _GetSpecificItemPropertyPatches[] =
{
	Patch((DWORD)OFFS_GetNextItemPropPatchEntry, (char*)"\xe9\x00\x00\x00\x00\x90\x90", (int)7),
	Patch(OFFS_GetNextItemPropPatchEntry + 1, (relativefunc)GetNextItemPropEntryPatch),

	Patch((DWORD)OFFS_GetNextItemPropPatchExit, (char*)"\xe9\x00\x00\x00\x00", (int)5),
	Patch(OFFS_GetNextItemPropPatchExit + 1, (relativefunc)GetNextItemPropExitPatch),

	Patch((DWORD)OFFS_GetItemPropTypeGetID, (char*)"\xe9\x00\x00\x00\x00", (int)5),
	Patch(OFFS_GetItemPropTypeGetID +1, (relativefunc)GetItemPropTypeExtractID),

	Patch((DWORD)OFFS_GetItemPropTypeGetIDInit, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6),
	Patch(OFFS_GetItemPropTypeGetIDInit +1, (relativefunc)GetItemPropTypeExtractIDInit),

	Patch()
};

Patch *GetSpecificItemPropertyPatches = _GetSpecificItemPropertyPatches;


std::string GetItemPropertyID()
{
	uint64_t uIDComplet = (uint64_t)g_itemPropertyTypeHookID.uIDP2 << 32;
	uIDComplet |= g_itemPropertyTypeHookID.uIDP1;

	if (uIDComplet == 0)
		return "";

	std::string s(20, '\0'); // max digits
	auto res = std::to_chars(s.data(), s.data() + s.size(), uIDComplet);
	if (res.ec != std::errc())
		return "";
	s.resize(res.ptr - s.data());
	return s;
}



void PatchGetNextItemProperty(char* pObject, int32_t iCounter)
{
	g_specialGetItemProperty.bHooked = 1;
	g_specialGetItemProperty.iCounter = iCounter;
	g_specialGetItemProperty.pExpectedItem = pObject;
}

void PrepareGetPropertyFromID(char* pItem, char* cID)
{
	std::string sID(cID);
	uint64_t uID;
	auto res = std::from_chars(sID.data(), sID.data() + sID.size(), uID);
	int32_t iCounter = -1;
	if (res.ec == std::errc())
		iCounter = GetItemPropertyCounterFromID(pItem, uID);

	PatchGetNextItemProperty((pItem+0x730), iCounter);
}

void PrepareGetPropertyFromIndex(char* pItem, int iIdx, bool bCountTemp)
{
	int32_t iCounter = GetItemPropertyCounterFromIdx(pItem, iIdx, bCountTemp);
	PatchGetNextItemProperty((pItem + 0x730), iCounter);
}


int ItemGetInt(char* cCommand, int iObjectID) {
	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;
	std::string sCommand(cCommand);

	ObjectId = (NWN::OBJECTID) iObjectID;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return 0;

	if(Object->GetObjectType() != NWN::OBJECT_TYPE_ITEM)
		return 0;

	char* ItemPtr = (char*)Object->AsItem();

	if (sCommand == "appVariation")
		return GetItemAppVariation(ItemPtr);
	else if (sCommand == "appCategory")
		return GetItemAppCategory(ItemPtr);
	else if (sCommand == "appFXwpn")
		return GetItemFXWpn(ItemPtr);
	else if (sCommand == "WpnPart0")
		return GetItemWpnPart1(ItemPtr);
	else if (sCommand == "WpnPart1")
		return GetItemWpnPart2(ItemPtr);
	else if (sCommand == "WpnPart2")
		return GetItemWpnPart3(ItemPtr);
	else if (sCommand == "ModelPartMask")
		return GetItemModelPartMask(ItemPtr);
	else if (sCommand == "BaseCost")
		return GetItemBaseCost(ItemPtr);
	else if (sCommand == "NonIdentifiedCost")
		return GetItemNonIdentifiedCost(ItemPtr);
	else if (sCommand == "ModifyCost")
		return GetItemModifyCost(ItemPtr);
	else if (sCommand == "BaseMaterialType")
		return GetItemGMaterial(ItemPtr);
	else if (sCommand == "NumberOfDamageReduction")
		return GetItemNumberOfDmgRedct(ItemPtr);
	else if (sCommand == "NbProperty")
		return GetPropertyNumber(ItemPtr, false);
	else if (sCommand == "NbPropertyT")
		return GetPropertyNumber(ItemPtr, true);
	else if (sCommand.rfind(cst_ModelPiece, 0) == 0) {
		sCommand = sCommand.substr(cst_ModelPiece.size());
		char* pEnd;
		int iModelPiece = strtol(sCommand.c_str(), &pEnd, 10);

		sCommand = pEnd;
		if (sCommand == "UVScroll")
			return GetItemModelPieceUVScroll(ItemPtr, iModelPiece);
		else if (sCommand == "Accessory")
			return GetItemModelPieceAccessory(ItemPtr, iModelPiece);
	}
	else if (sCommand.rfind(cst_ModelPart, 0) == 0) {
		sCommand = sCommand.substr(cst_ModelPart.size());
		char* pEnd;
		int iModelPart = strtol(sCommand.c_str(), &pEnd, 10);

		sCommand = pEnd;

		if (sCommand == "Variation")
			return GetItemModelPartVariation(ItemPtr, iModelPart);
		else if(sCommand == "Category")
			return GetItemModelPartCategory(ItemPtr, iModelPart);
	}
	else if (sCommand.rfind(cst_DmgRedct, 0) == 0) {
		sCommand = sCommand.substr(cst_DmgRedct.size());
		std::string sParam;

		std::size_t spacePos = sCommand.find(' ');
		if (spacePos == std::string::npos) {
			sParam = "";
		}
		else {
			sParam = sCommand.substr(spacePos + 1);
			sCommand = sCommand.substr(0, spacePos);
		}

		std::vector<int> iParameters;
		std::istringstream iss(sParam);
		int number;

		while (iss >> number) {
			iParameters.push_back(number);
		}

		if (iParameters.size() == 2)
		{
			if(sCommand == "PiercingType")
				return GetItmDmgRedctPiercingType(ItemPtr, iParameters.at(0), iParameters.at(1));
			else if(sCommand == "PiercingSubType")
				return GetItmDmgRedctPiercingSubType(ItemPtr, iParameters.at(0), iParameters.at(1));
		}
		else if (iParameters.size() == 1)
		{
			if (sCommand == "NumPiercing")
				return GetItmDmgRedctNumberPiercingType(ItemPtr, iParameters.at(0));
			else if (sCommand == "UseOrLogic")
				return GetItmDmgReductionUseOrLogic(ItemPtr, iParameters.at(0));
			else if (sCommand == "Amount")
				return GetItemDmgReductionAmount(ItemPtr, iParameters.at(0));
		}
	}

	return 0;
}

void ItemSetInt(char* cCommand, int iObjectID, int iValue) {
	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;
	std::string sCommand(cCommand);

	ObjectId = (NWN::OBJECTID) iObjectID;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return;

	if(Object->GetObjectType() != NWN::OBJECT_TYPE_ITEM)
		return;

	char* ItemPtr = (char*)Object->AsItem();

	if (sCommand == "itemType")
		SetItemBaseItem(ItemPtr, iValue);
	else if (sCommand == "appVariation")
		SetItemAppVariation(ItemPtr, iValue);
	else if (sCommand == "appCategory")
		SetItemAppCategory(ItemPtr, iValue);
	else if (sCommand == "appFXwpn")
		SetItemFXWpn(ItemPtr, iValue);
	else if (sCommand == "WpnPart0")
		SetItemWpnPart1(ItemPtr, iValue);
	else if (sCommand == "WpnPart1")
		SetItemWpnPart2(ItemPtr, iValue);
	else if (sCommand == "WpnPart2")
		SetItemWpnPart3(ItemPtr, iValue);
	else if (sCommand == "ModelPartMask")
		SetItemModelPartMask(ItemPtr, iValue);
	else if (sCommand == "BaseCost")
		SetItemBaseCost(ItemPtr, iValue);
	else if (sCommand == "NonIdentifiedCost")
		SetItemNonIdentifiedCost(ItemPtr, iValue);
	else if (sCommand == "ModifyCost")
		SetItemModifyCost(ItemPtr, iValue);
	else if (sCommand == "RemoveDmgRedct")
		RemoveDmgRedct(ItemPtr, iValue);
	else if (sCommand == "Charges")
		ChargesNumber(ItemPtr, iValue);
	else if (sCommand == "PrepareGetPropertyByIdx")
		PrepareGetPropertyFromIndex(ItemPtr, iValue, false);
	else if (sCommand == "PrepareGetPropertyByIdxT")
		PrepareGetPropertyFromIndex(ItemPtr, iValue, true);
	else if (sCommand.rfind(cst_ModelPiece, 0) == 0) {
		sCommand = sCommand.substr(cst_ModelPiece.size());
		char* pEnd;
		int iModelPiece = strtol(sCommand.c_str(), &pEnd, 10);

		sCommand = pEnd;

		if (sCommand == "UVScroll")
			SetItemModelPieceUVScroll(ItemPtr, iModelPiece, iValue != 0);
		else if (sCommand == "Accessory")
			SetItemModelPieceAccessory(ItemPtr, iModelPiece, iValue);
	}
	else if (sCommand.rfind(cst_ModelPart, 0) == 0) {
		sCommand = sCommand.substr(cst_ModelPart.size());
		char* pEnd;
		int iModelPart = strtol(sCommand.c_str(), &pEnd, 10);

		sCommand = pEnd;

		if (sCommand == "Variation")
			SetItemModelPartVariation(ItemPtr, iModelPart, iValue);
		else if(sCommand == "Category")
			SetItemModelPartCategory(ItemPtr, iModelPart, iValue);
	}
	else if (sCommand.rfind(cst_DmgRedct, 0) == 0) {
		sCommand = sCommand.substr(cst_DmgRedct.size());
		std::string sParam;

		std::size_t spacePos = sCommand.find(' ');
		if (spacePos == std::string::npos) {
			sParam = "";
		} else {
			sParam = sCommand.substr(spacePos + 1);
			sCommand = sCommand.substr(0, spacePos);
		}

		std::vector<int> iParameters;
		std::istringstream iss(sParam);
		int number;

		while (iss >> number) {
			iParameters.push_back(number);
		}

		if (iParameters.size() == 2)
		{
			if (sCommand == "AddPiercing")
				AddDmgRedctPiercingType(ItemPtr, iValue, iParameters.at(0), iParameters.at(1));
			else if (sCommand == "PiercingSubType")
				SetItmDmgRedctPiercingSubType(ItemPtr, iParameters.at(0), iParameters.at(1), iValue);
			else if (sCommand == "PiercingType")
				SetItmDmgRedctPiercingType(ItemPtr, iParameters.at(0), iParameters.at(1), iValue);
		}
		else if (iParameters.size() == 1) 
		{
			if (sCommand == "RemovePiercing")
				RemoveDmgRedctPiercingType(ItemPtr, iParameters.at(0), iValue);
			else if (sCommand == "AddRedct")
				AddDmgRedct(ItemPtr, iParameters.at(0), iValue);
			else if (sCommand == "Amount")
				SetItemDmgReductionAmount(ItemPtr, iParameters.at(0), iValue);
			else if (sCommand == "UseOrLogic")
				SetItmDmgReductionUseOrLogic(ItemPtr, iParameters.at(0), iValue);
		}
	}
}

float ItemGetFloat(char* cCommand, int iObjectID) {
	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;
	std::string sCommand(cCommand);

	ObjectId = (NWN::OBJECTID) iObjectID;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return 0;

	if(Object->GetObjectType() != NWN::OBJECT_TYPE_ITEM)
		return 0;

	char* ItemPtr = (char*)Object->AsItem();
	if (sCommand.rfind(cst_ModelPiece, 0) == 0) {
		sCommand = sCommand.substr(cst_ModelPiece.size());
		char* pEnd;
		int iModelPiece = strtol(sCommand.c_str(), &pEnd, 10);

		sCommand = pEnd;

		if (sCommand == "UScroll")
			return GetItemModelPieceUScroll(ItemPtr, iModelPiece);
		else if (sCommand == "VScroll")
			return GetItemModelPieceVScroll(ItemPtr, iModelPiece);
	}

	return 0;

}

void ItemSetFloat(char* cCommand, int iObjectID, float fValue) {
	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;
	std::string sCommand(cCommand);

	ObjectId = (NWN::OBJECTID) iObjectID;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject(ObjectId)) == NULL)
		return;

	if(Object->GetObjectType() != NWN::OBJECT_TYPE_ITEM)
		return;

	char* ItemPtr = (char*)Object->AsItem();

	if (sCommand.rfind(cst_ModelPiece, 0) == 0) {
		sCommand = sCommand.substr(cst_ModelPiece.size());
		char* pEnd;
		int iModelPiece = strtol(sCommand.c_str(), &pEnd, 10);

		sCommand = pEnd;

		if (sCommand == "UScroll")
			SetItemModelPieceUScroll(ItemPtr, iModelPiece, fValue);
		else if (sCommand == "VScroll")
			SetItemModelPieceVScroll(ItemPtr, iModelPiece, fValue);
	}
}

std::string ItemGetString(char* cCommand, int iObjectID) {
	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;
	std::string sCommand(cCommand);
	std::string sResult = "";


	//little ugly for now, but allow kept structs localized
	if (sCommand == "DPropertyID")
	{
		return GetItemPropertyID();
	}

	ObjectId = (NWN::OBJECTID) iObjectID;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject(ObjectId)) == NULL)
		return sResult;

	if(Object->GetObjectType() != NWN::OBJECT_TYPE_ITEM)
		return sResult;

	char* ItemPtr = (char*)Object->AsItem();

	if (sCommand == "Description")
		return GetItemDescription(ItemPtr);
	else if (sCommand == "DescriptionNI")
		return GetItemDescriptionNonIdentified(ItemPtr);
	else if (sCommand.find("PropertyID") != std::string::npos)
	{
		StringSplitter s(sCommand);
		int iIdx, bCountTemp;
		if (s.skip() && s.nextInt(iIdx) && s.nextInt(bCountTemp))
		{
			return GetPropertyID(ItemPtr, iIdx, (bCountTemp == 1));
		}
		return "";
	}
	else if (sCommand.find("Color") != std::string::npos)
	{
		NWN::D3DXCOLOR* ObjColor = GetItemColorPtr(ItemPtr, sCommand);
		if(ObjColor != NULL) {
			sResult = ColorToString(ObjColor);
		}
	}

	return sResult;
}

void ItemSetString(char* cCommand, int iObjectID, char* sValue) {
	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;
	std::string sCommand(cCommand);

	ObjectId = (NWN::OBJECTID) iObjectID;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject(ObjectId)) == NULL)
		return;

	if(Object->GetObjectType() != NWN::OBJECT_TYPE_ITEM)
		return;

	char* ItemPtr = (char*)Object->AsItem();

	if (sCommand == "Description")
		SetItemDescription(ItemPtr, sValue);
	else if (sCommand == "DescriptionNI")
		SetItemDescriptionNonIdentified(ItemPtr, sValue);
	else if (sCommand == "AddProperty")
		AddItemProperty(Object, sValue);
	else if (sCommand == "PrepareGetPropertyByID")
		PrepareGetPropertyFromID(ItemPtr, sValue);
	else if (sCommand.find("Color") != std::string::npos)
	{

		NWN::D3DXCOLOR* ObjColor = GetItemColorPtr(ItemPtr, sCommand);
		if(ObjColor != NULL) {
			NWN::D3DXCOLOR myColor;
			//Color is Ok, we can apply it
			if(StringToColor(sValue, &myColor)) {
				memcpy(ObjColor, &myColor, sizeof(myColor));
			}
		}
	}
}

void InitPatchItemProperty()
{
	int i = 0;
	while(GetSpecificItemPropertyPatches[i].Apply()) {
		i++;
	}
}





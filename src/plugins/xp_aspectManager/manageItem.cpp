#include "aspectManagerUtils.h"
#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include "../../septutil/NwN2DataPos.h"

#include "nwn2heap.h"
#include <algorithm>
#include <sstream>

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





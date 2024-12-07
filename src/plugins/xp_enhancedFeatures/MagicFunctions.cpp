
#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include <misc/log.h>
#include <nwn2heap.h>

#include "../../septutil/NwN2DataPos.h"
#include "../../septutil/intern2daMgt.h"

#include <cstdint>

#include "MagicFunctions.h"

#define CLASS_TYPE_INVALID  255

extern std::unique_ptr<LogNWNX> logger;


typedef void (__cdecl * NWN2Heap_Deallocate_Proc)(void *p);
extern NWN2Heap_Deallocate_Proc NWN2Heap_Deallocate;

GameObject* GetCreature(int oCreature)
{
	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;

	ObjectId = (NWN::OBJECTID) oCreature;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return nullptr;

	if(Object->GetObjectType() != NWN::OBJECT_TYPE_CREATURE)
		return nullptr;

	return Object;
}

AmCrtClass* GetClassBlock(int oCreature, int nClassType)
{
	GameObject* pCreature = GetCreature(oCreature);

	if (pCreature == nullptr)
		return nullptr;

	char* creaBlock = *(char**)((char*)pCreature + AmCrtPtrAppBlock);

	uint8_t nbClass = *(uint8_t*)(creaBlock + AmCrtABNbClass);
	AmCrtClass* classBlock;// = (AmCrtClass*)(creaBlock + AmCrtAbClass0);

	for (int i = 0; i < nbClass; i++)
	{
		classBlock = (AmCrtClass*)(creaBlock + AmCrtAbClass0 + i * AmCrtAbSizeClass);

		uint8_t idClass = classBlock->classID;
		if (idClass == nClassType)
		{
			return classBlock;
		}
	}

	return nullptr;
}

int GetClassNumber(int oCreature, int nClassType)
{
	GameObject* pCreature = GetCreature(oCreature);

	if (pCreature == nullptr)
		return -1;

	char* creaBlock = *(char**)((char*)pCreature + AmCrtPtrAppBlock);

	uint8_t nbClass = *(uint8_t*)(creaBlock + AmCrtABNbClass);
	AmCrtClass* classBlock; // = (AmCrtClass*)(creaBlock + AmCrtAbClass0);

	for (int i = 0; i < nbClass; i++)
	{
		classBlock = (AmCrtClass*)(creaBlock + AmCrtAbClass0 + i * AmCrtAbSizeClass);

		uint8_t idClass = classBlock->classID;
		if (idClass == nClassType)
		{
			return i;
		}
	}

	return -1;
}

void ClearMemorizedSpell(int oCreature, int nClassType, int nSpellLevel, int nIndex)
{
	/*
	int iClassIdx = GetClassNumber(oCreature, nClassType);
	
	if (iClassIdx >= 0)
	{
		uint8_t uClassIdx = iClassIdx & 0xFF;
		uint8_t uSpellLvl = nSpellLevel & 0xFF;
		uint8_t uSpellSlot = nIndex & 0xFF;

		//TODO call function 0x005a0130
	}
	*/

	AmCrtClass* myClass = GetClassBlock(oCreature, nClassType);
	if (myClass == nullptr)
		return;

	if (nSpellLevel >= 10 || nSpellLevel < 0)
		return;

	int iNbSpellSlot = myClass->MemorizedSpells[nSpellLevel].NbSpellsSlot;
	if (nIndex < iNbSpellSlot)
	{
		AmCrtClassSpellMemorized* mySpell = myClass->MemorizedSpells[nSpellLevel].lSpellMemorized[nIndex];

		if (mySpell != 0)
		{
			myClass->MemorizedSpells[nSpellLevel].lSpellMemorized[nIndex] = 0;
			if (NWN2Heap_Deallocate)
				NWN2Heap_Deallocate( mySpell );
		}
	}
}

void ClearMemorizedSpellBySpellId(int oCreature, int nClassType, int nSpellId)
{
	AmCrtClass* myClass = GetClassBlock(oCreature, nClassType);
	if (myClass == nullptr)
		return;

	for (int iLvl = 0; iLvl < 10; iLvl++)
	{
		int iNbSpellSlot = myClass->MemorizedSpells[iLvl].NbSpellsSlot;
		if (iNbSpellSlot > 0)
		{
			for (int iSlot = 0;  iSlot < iNbSpellSlot; iSlot++)
			{
				AmCrtClassSpellMemorized* mySpell = myClass->MemorizedSpells[iLvl].lSpellMemorized[iSlot];

				if (mySpell != 0)
				{
					if (mySpell->spellID == nSpellId)
					{
						myClass->MemorizedSpells[iLvl].lSpellMemorized[iSlot] = 0;
						if (NWN2Heap_Deallocate)
							NWN2Heap_Deallocate( mySpell );
					}
				}
			}
		}
	}
}


void SetMemorizableSpellNumber(int oCreature, int nClassType, int nSpellLevel, int iNumber)
{
	int iResult         = 0;

	AmCrtClass* myClass = GetClassBlock(oCreature, nClassType);
	if (myClass == nullptr)
		return;

	if (nSpellLevel >= 10 || nSpellLevel < 0)
		return;

	//Need to remove memorized spells
	if (myClass->MemorizedSpells[nSpellLevel].NbSpellsSlot > iNumber)
	{
		int iNbSpellsSlot = myClass->MemorizedSpells[nSpellLevel].NbSpellsSlot;
		while (iNbSpellsSlot > iNumber)
		{
			AmCrtClassSpellMemorized* mySpell = myClass->MemorizedSpells[nSpellLevel].lSpellMemorized[iNbSpellsSlot-1];
			//There is a memorized spell
			if (mySpell != 0)
			{
				//Remove it and free memory
				myClass->MemorizedSpells[nSpellLevel].lSpellMemorized[iNbSpellsSlot-1] = 0;
				if (NWN2Heap_Deallocate)
					NWN2Heap_Deallocate( mySpell );
			}

			iNbSpellsSlot--;
		}
	}
	//Need to add slot
	else if (myClass->MemorizedSpells[nSpellLevel].NbSpellsSlot < iNumber)
	{
		//List not long enough,need to allocate a new one
		if (myClass->MemorizedSpells[nSpellLevel].size < iNumber)
		{
			int iSize = myClass->MemorizedSpells[nSpellLevel].size;
			int iNewSize = iSize;
			while (iNewSize < iNumber)
			{
				iNewSize += iSize;
			}

			//Create the new array
			NWN2_HeapMgr *pHeapMgr = NWN2_HeapMgr::Instance();
			NWN2_Heap *pHeap = pHeapMgr->GetDefaultHeap();
			AmCrtClassSpellMemorized** lSpellMemorized = (AmCrtClassSpellMemorized**)pHeap->Allocate(0x4*iNewSize);

			//Copy the old array in the new one
			for (int i = 0; i < iNewSize; i++)
			{
				if (i < iSize) {
					lSpellMemorized[i] = myClass->MemorizedSpells[nSpellLevel].lSpellMemorized[i];
					myClass->MemorizedSpells[nSpellLevel].lSpellMemorized[i] = 0;
				}
				else
					lSpellMemorized[i] = 0;
			}
			//Deallocate old one
			AmCrtClassSpellMemorized** lOldSpellMemorized = myClass->MemorizedSpells[nSpellLevel].lSpellMemorized;
			myClass->MemorizedSpells[nSpellLevel].lSpellMemorized = lSpellMemorized;
			if (NWN2Heap_Deallocate)
				NWN2Heap_Deallocate( lOldSpellMemorized );

			//Don't forget to set the old size
			myClass->MemorizedSpells[nSpellLevel].size = iNewSize;
		}
		//If the array is long enough
		else
		{
			//Just be sure to have new spell slot well initialized
			for (int i = myClass->MemorizedSpells[nSpellLevel].NbSpellsSlot; i <= iNumber; i++)
			{
				myClass->MemorizedSpells[nSpellLevel].lSpellMemorized[i] = 0;
			}
		}
	}
	myClass->MemorizedSpells[nSpellLevel].NbSpellsSlot = iNumber;
}

void SetNumberOfSpellUsesLeft(int oCreature, int nClassType, int nSpellLevel, int nNumber)
{
	AmCrtClass* myClass = GetClassBlock(oCreature, nClassType);
	if (myClass == nullptr)
		return;

	if (nSpellLevel >= 10 || nSpellLevel < 0)
		return;

	myClass->SpellPerDaysLeft[nSpellLevel] = nNumber;
}

//iSlotMemorizeReady = 0 => slot number,  =1 => memorized spell , =2 => spell ready
int GetMemorizedSpellCountByLevel(int oCreature, int nClassType, int nSpellLevel, int iSlotMemorizeReady)
{

	int iResult         = 0;

	AmCrtClass* myClass = GetClassBlock(oCreature, nClassType);
	if (myClass == nullptr)
		return 0;

	if (nSpellLevel >= 10 || nSpellLevel < 0)
		return 0;

	// If we want to only check spell slot
	if (iSlotMemorizeReady == 0)
		return myClass->MemorizedSpells[nSpellLevel].NbSpellsSlot;
	else
	{
		int iNbSpellSlot = myClass->MemorizedSpells[nSpellLevel].NbSpellsSlot;
		for (int i = 0; i < iNbSpellSlot; i++)
		{
			AmCrtClassSpellMemorized* mySpell = myClass->MemorizedSpells[nSpellLevel].lSpellMemorized[i];

			if (mySpell != 0)
			{
				if (iSlotMemorizeReady == 1 || mySpell->isReady != 0)
				{
					iResult++;
				}
			}
		
		}
	}
	return iResult;
}

void SetMemorizedSpellReady(int oCreature, int nClassType, int nSpellLevel, int nIndex, int bReady)
{
	AmCrtClass* myClass = GetClassBlock(oCreature, nClassType);
	if (myClass == nullptr)
		return;

	if (nSpellLevel >= 10 || nSpellLevel < 0)
		return;

	int iNbSpellSlot = myClass->MemorizedSpells[nSpellLevel].NbSpellsSlot;
	if (nIndex < iNbSpellSlot)
	{
		AmCrtClassSpellMemorized* mySpell = myClass->MemorizedSpells[nSpellLevel].lSpellMemorized[nIndex];

		if (mySpell != 0)
		{
			mySpell->isReady = bReady;
		}
	}
}


void SetNumberOfBonusSpell(int oCreature, int nClassType, int nSpellLevel, int nNumber)
{
	AmCrtClass* myClass = GetClassBlock(oCreature, nClassType);
	if (myClass == nullptr)
		return;

	if (nSpellLevel >= 10 || nSpellLevel < 0)
		return;

	myClass->BonusSpell[nSpellLevel] = nNumber;
}

//0059d280
void SetMemorizedSpell(int oCreature, int nClassType, int nSpellLevel, int nIndex, int nSpellId, int bReady, int nMetaMagic, int bIsDomainSpell)
{
	AmCrtClass* myClass = GetClassBlock(oCreature, nClassType);
	if (myClass == nullptr)
		return;

	if (nSpellLevel >= 10 || nSpellLevel < 0)
		return;

	int iNbSpellSlot = myClass->MemorizedSpells[nSpellLevel].NbSpellsSlot;
	if (nIndex < iNbSpellSlot)
	{
		AmCrtClassSpellMemorized* mySpell = myClass->MemorizedSpells[nSpellLevel].lSpellMemorized[nIndex];

		if (mySpell == 0)
		{
			NWN2_HeapMgr *pHeapMgr = NWN2_HeapMgr::Instance();
			NWN2_Heap *pHeap = pHeapMgr->GetDefaultHeap();
			mySpell = (AmCrtClassSpellMemorized*)pHeap->Allocate(0x10);
			myClass->MemorizedSpells[nSpellLevel].lSpellMemorized[nIndex] = mySpell;
		}

		mySpell->spellID = nSpellId;
		mySpell->isReady = bReady;
		mySpell->metaMagic = nMetaMagic;
		mySpell->isDomain = bIsDomainSpell;
	}
}

int GetMemorizedSpellReady(int oCreature, int nClassType, int nSpellLevel, int nIndex)
{
	int result = -1;

	AmCrtClass* myClass = GetClassBlock(oCreature, nClassType);
	if (myClass == nullptr)
		return result;

	if (nSpellLevel >= 10 || nSpellLevel < 0)
		return result;

	int iNbSpellSlot = myClass->MemorizedSpells[nSpellLevel].NbSpellsSlot;
	if (nIndex < iNbSpellSlot)
	{
		AmCrtClassSpellMemorized* mySpell = myClass->MemorizedSpells[nSpellLevel].lSpellMemorized[nIndex];

		if (mySpell != 0)
		{
			result = mySpell->isReady;
		}
	}

	return result;
}


//59b440
//ReadySpellLevel
//TODO
//Add a SetSpellUse function

#define FCT_CalculateBaseSpellNumber	0x0075b690
#define FCT_GetBonusSpellByAbility		0x00758d10
#define FCT_GetIfExtraSlotByFeat		0x00754b40


__declspec(naked) uint8_t __fastcall CalculateBaseSpellNumber(__in void* pCreature, __in void* Unused, __in uint32_t uClassID, __in char uClassLVl, uint32_t uSpellLVl, int iSetTo1)
{
	__asm
	{
		MOV EDX, FCT_CalculateBaseSpellNumber
		JMP EDX
	}
}

__declspec(naked) uint8_t	__fastcall GetBonusSpellByAbility(__in void* param_1_00, __in void* Unused, __in uint32_t uClassID,__in uint32_t uSpellLVl)
{
	__asm
	{
		MOV EDX, FCT_GetBonusSpellByAbility
		JMP EDX
	}
}

__declspec(naked) uint8_t	__fastcall GetIfExtraSlotByFeat(__in void* param_1_00, __in void* Unused, __in uint32_t uClassID,__in uint32_t uSpellLVl)
{
	__asm
	{
		MOV EDX, FCT_GetIfExtraSlotByFeat
		JMP EDX
	}
}





int GetMaxNumberOfSpell(int oCreature, int nClassType, int nSpellLevel, int iFullBonusBase)
{
	int result = 0;

	AmCrtClass* myClass = GetClassBlock(oCreature, nClassType);

	if (myClass == nullptr)
		return result;


	uint32_t nClassID                = myClass->classID;
	uint32_t nClassLvl                = myClass->classLvl;
	NWN2DA::classes2DA myClass2daRow = GetClasseRow(nClassID);

	if (myClass2daRow == NULL)
		return result;

	if (myClass2daRow->m_SpellCaster == 0)
		return result;


	if (nSpellLevel >= 10 || nSpellLevel < 0)
		return result;


	GameObject* pCreature = GetCreature(oCreature);
	char* pCreaStat = *(char**)((char*)pCreature + AmCrtPtrAppBlock);

	//If bonus
	if (iFullBonusBase & 0x8)
	{
		result = myClass->BonusSpell[nSpellLevel];
	}

	if ((iFullBonusBase & 0x1) == 0)
	{
		uint8_t uSpellNumber = CalculateBaseSpellNumber((void*)pCreaStat, NULL, nClassID, nClassLvl, nSpellLevel, 1);
		if (uSpellNumber != 0)
		{
			//result += uSpellNumber;

			if ((iFullBonusBase & 0x2) != 0)
			{
				result += GetBonusSpellByAbility((void*)pCreaStat, NULL, nClassID, nSpellLevel);
			}

			if ((iFullBonusBase & 0x4) != 0)
			{
				result += GetIfExtraSlotByFeat((void*)pCreaStat, NULL, nClassID, nSpellLevel);
			}
		}
	} 
	else
	{
		uint8_t uSpellNumber = CalculateBaseSpellNumber((void*)pCreaStat, NULL, nClassID, nClassLvl, nSpellLevel, 1);
		if (uSpellNumber != 0) 
		{
			result += uSpellNumber;

			if ((iFullBonusBase & 0x2) == 0)
			{
				int iSBonusSpell = GetBonusSpellByAbility((void*)pCreaStat, NULL, nClassID, nSpellLevel);
				result = result - iSBonusSpell;
			}

			if ((iFullBonusBase & 0x4) == 0)
			{
				int iSBonusSpell = GetIfExtraSlotByFeat((void*)pCreaStat, NULL, nClassID, nSpellLevel);
				result = result - iSBonusSpell;
			}
		}
	}

	return result;
}




int GetMemorizedSpellId(int oCreature, int nClassType, int nSpellLevel, int nIndex)
{
	int result = -1;

	AmCrtClass* myClass = GetClassBlock(oCreature, nClassType);
	if (myClass == nullptr)
		return result;

	if (nSpellLevel >= 10 || nSpellLevel < 0)
		return result;

	int iNbSpellSlot = myClass->MemorizedSpells[nSpellLevel].NbSpellsSlot;

	if (nIndex < iNbSpellSlot)
	{
		AmCrtClassSpellMemorized* mySpell = myClass->MemorizedSpells[nSpellLevel].lSpellMemorized[nIndex];
		if (mySpell != 0)
		{
			result = mySpell->spellID;
		}
	}

	return result;
}


int GetMemorizedSpellMetaMagic(int oCreature, int nClassType, int nSpellLevel, int nIndex)
{
	int result = -1;

	AmCrtClass* myClass = GetClassBlock(oCreature, nClassType);
	if (myClass == nullptr)
		return result;

	if (nSpellLevel >= 10 || nSpellLevel < 0)
		return result;

	int iNbSpellSlot = myClass->MemorizedSpells[nSpellLevel].NbSpellsSlot;

	if (nIndex < iNbSpellSlot)
	{
		AmCrtClassSpellMemorized* mySpell = myClass->MemorizedSpells[nSpellLevel].lSpellMemorized[nIndex];
		if (mySpell != 0)
		{
			result = mySpell->metaMagic;
		}
	}

	return result;
}

int GetMemorizedSpellIsDomainSpell(int oCreature, int nClassType, int nSpellLevel, int nIndex)
{
	int result = -1;

	AmCrtClass* myClass = GetClassBlock(oCreature, nClassType);
	if (myClass == nullptr)
		return result;

	if (nSpellLevel >= 10 || nSpellLevel < 0)
		return result;

	int iNbSpellSlot = myClass->MemorizedSpells[nSpellLevel].NbSpellsSlot;

	if (nIndex < iNbSpellSlot)
	{
		AmCrtClassSpellMemorized* mySpell = myClass->MemorizedSpells[nSpellLevel].lSpellMemorized[nIndex];
		if (mySpell != 0)
		{
			result = mySpell->isDomain;
		}
	}

	return result;
}


int GetKnownSpellCount(int oCreature, int nClassType, int nSpellLevel)
{
	AmCrtClass* myClass = GetClassBlock(oCreature, nClassType);
	if (myClass == nullptr)
		return 0;

	if (nSpellLevel >= 10 || nSpellLevel < 0)
		return 0;

	return myClass->SpellsKnow[nSpellLevel].NbSpellsKnow;
}

int GetKnownSpellId(int oCreature, int nClassType, int nSpellLevel, int nIndex)
{
	AmCrtClass* myClass = GetClassBlock(oCreature, nClassType);
	if (myClass == nullptr)
		return -1;

	if (nSpellLevel >= 10 || nSpellLevel < 0)
		return -1;

	if (nIndex < myClass->SpellsKnow[nSpellLevel].NbSpellsKnow)
	{
		return myClass->SpellsKnow[nSpellLevel].lSpellsKnow[nIndex];
	}

	return -1;
}

int GetIsInKnownSpellList(int oCreature, int nClassType, int nSpellId)
{
	AmCrtClass* myClass = GetClassBlock(oCreature, nClassType);
	if (myClass == nullptr)
		return 0;

	for (int iLvl = 0; iLvl < 10; iLvl++)
	{
		int iNbSpellKnow = myClass->SpellsKnow[iLvl].NbSpellsKnow;
		for (int iSpellIdx = 0; iSpellIdx < iNbSpellKnow; iSpellIdx++)
		{
			if (myClass->SpellsKnow[iLvl].lSpellsKnow[iSpellIdx] == nSpellId)
			{
				return 1;
			}
		}
	}

	return 0;
}

int GetNumberOfSpellUsesLeft(int oCreature, int nClassType, int nSpellLevel)
{
	AmCrtClass* myClass = GetClassBlock(oCreature, nClassType);
	if (myClass == nullptr)
		return 0;

	if (nSpellLevel >= 10 || nSpellLevel < 0)
		return 0;

	return myClass->SpellPerDaysLeft[nSpellLevel];
}



// TODO void ( object oCreature, int nClassType, int nSpellId, int nMetaMagic = METAMAGIC_NONE, int nDomainLevel = 0);

// todo int GetSpellLevelByClass( int nClassType, int nSpellId);
//void GetSpellUsesLeft(object oCreature, int nClassType, int nSpellId, int nMetaMagic = METAMAGIC_NONE, int nDomainLevel = 0); //TODO ?




//int GetNumberOfSpellUsesLeft(object oCreature, int nClassType, int nSpellLevel);
//void SetNumberOfSpellUsesLeft(object oCreature, int nClassType, int nSpellLevel, int nNumber);

/*
int GetIsInKnownSpellList(object oCreature, int nClassType, int nSpellId)
int GetKnownSpellCount(object oCreature, int nClassType, int nSpellLevel)

int GetKnownSpellId(object oCreature, int nClassType, int nSpellLevel, int nIndex)
int GetMemorizedSpellIsDomainSpell(object oCreature, int nClassType, int nSpellLevel, int nIndex)
int GetMemorizedSpellMetaMagic(object oCreature, int nClassType, int nSpellLevel, int nIndex)
int GetMemorizedSpellId(object oCreature, int nClassType, int nSpellLevel, int nIndex)
int GetMaxNumberOfSpell(object oCreature, int nClassType, int nSpellLevel, int iFullBonusBase)
int GetMemorizedSpellReady(object oCreature, int nClassType, int nSpellLevel, int nIndex)
int GetMemorizedSpellCountByLevel(object oCreature, int nClassType, int nSpellLevel, int iSlotMemorizeReady)

void SetMemorizableSpellNumber(int oCreature, int nClassType, int nSpellLevel, int iNumber)

void SetMemorizedSpellReady(object oCreature, int nClassType, int nSpellLevel, int nIndex, int bReady)
void SetMemorizedSpell(object oCreature, int nClassType, int nSpellLevel, int nIndex, int nSpellId, int bReady, int nMetaMagic, int bIsDomainSpell)
void ClearMemorizedSpellBySpellId(object oCreature, int nClassType, int nSpellId)
void ClearMemorizedSpell(object oCreature, int nClassType, int nSpellLevel, int nIndex)
void SetNumberOfBonusSpell(object oCreature, int nClassType, int nSpellLevel, int nValue);

//SetNumberOfSpellUse (max pas tres utile, c'est refresh au repos ?)
//SetNumberOfUsableSpells

	*/

void SetIntMagicFunctions(std::string sFunction, char* cParam1, int nObject, int nValue)
{
	std::istringstream iss(cParam1);

	int nClassType, nParam2, nParam3, nParam4, nParam5, nParam6;

	
	if (!(iss >> nClassType)) {
		logger->Err("Function %s don't exist or must have more parameters", sFunction.c_str());
		return;
	}

	if (sFunction == "ClearMemorizedSpellBySpellId")
		return ClearMemorizedSpellBySpellId(nObject, nClassType, nValue);

	
	if (!(iss >> nParam2)) {
		logger->Err("Function %s don't exist or must have more parameters", sFunction.c_str());
		return;
	}

	if (sFunction == "ClearMemorizedSpell")
		return ClearMemorizedSpell(nObject, nClassType, nParam2, nValue);
	else if (sFunction == "SetMemorizableSpellNumber")
		return SetMemorizableSpellNumber(nObject, nClassType, nParam2, nValue);
	else if (sFunction == "SetNumberOfSpellUsesLeft")
		return SetNumberOfSpellUsesLeft(nObject, nClassType, nParam2, nValue);
	else if (sFunction == "SetNumberOfBonusSpell")
		return SetNumberOfBonusSpell(nObject, nClassType, nParam2, nValue);


	if (!(iss >> nParam3)) {
		logger->Err("Function %s don't exist or must have more parameters", sFunction.c_str());
		return;
	}

	if (sFunction == "SetMemorizedSpellReady")
		return SetMemorizedSpellReady(nObject, nClassType, nParam2, nParam3, nValue);

	
	if (!(iss >> nParam4)) {
		logger->Err("Function %s don't exist or must have more parameters", sFunction.c_str());
		return;
	}

	if (!(iss >> nParam5)) {
		logger->Err("Function %s don't exist or must have more parameters", sFunction.c_str());
		return;
	}

	if (!(iss >> nParam6)) {
		logger->Err("Function %s don't exist or must have more parameters", sFunction.c_str());
		return;
	}


	if (sFunction == "SetMemorizedSpell")
		return SetMemorizedSpell(nObject, nClassType, nParam2, nParam3, nParam4, nParam5, nParam6, nValue);


	logger->Err("Function %s don't exist.", sFunction.c_str());

	return;

}


int GetIntMagicFunctions(std::string sFunction, char* sParam1, int nObject)
{
	std::istringstream iss(sParam1);

	int nClassType, nParam2, nParam3, nParam4;

	if (!(iss >> nClassType)) {
		logger->Err("Function %s don't exist or must have more parameters", sFunction.c_str());
		return 0;
	}

	if (!(iss >> nParam2)) {
		logger->Err("Function %s don't exist or must have more parameters", sFunction.c_str());
		return 0;
	}

	if (sFunction == "GetIsInKnownSpellList")
		return GetIsInKnownSpellList(nObject, nClassType, nParam2);
	else if (sFunction == "GetKnownSpellCount")
		return GetKnownSpellCount(nObject, nClassType, nParam2);
	else if (sFunction == "GetNumberOfSpellUsesLeft")
		return GetNumberOfSpellUsesLeft(nObject, nClassType, nParam2);

	
	if (!(iss >> nParam3)) {
		logger->Err("Function %s don't exist or must have more parameters", sFunction.c_str());
		return 0;
	}

	if (sFunction == "GetKnownSpellId")
		return GetKnownSpellId(nObject, nClassType, nParam2, nParam3);
	if (sFunction == "GetMemorizedSpellIsDomainSpell")
		return GetMemorizedSpellIsDomainSpell(nObject, nClassType, nParam2, nParam3);
	if (sFunction == "GetMemorizedSpellMetaMagic")
		return GetMemorizedSpellMetaMagic(nObject, nClassType, nParam2, nParam3);
	if (sFunction == "GetMemorizedSpellId")
		return GetMemorizedSpellId(nObject, nClassType, nParam2, nParam3);
	if (sFunction == "GetMemorizedSpellReady")
		return GetMemorizedSpellReady(nObject, nClassType, nParam2, nParam3);
	if (sFunction == "GetMaxNumberOfSpell")
		return GetMaxNumberOfSpell(nObject, nClassType, nParam2, nParam3);
	if (sFunction == "GetMemorizedSpellCountByLevel")
		return GetMemorizedSpellCountByLevel(nObject, nClassType, nParam2, nParam3);

	logger->Err("Function %s don't exist.", sFunction.c_str());

	return 0;
}

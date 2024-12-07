#include "aspectManagerUtils.h"
#include "manageCreature.h"
#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include "../../septutil/NwN2DataPos.h"

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

/* Creature Visual Stuff */

//Head
int GetHeadVariation(GameObject* Object) {
	return *(uint8_t*)((char*)Object + AmCrtHeadVariation);
}

void SetHeadVariation(GameObject* Object, int iValue) {
	uint8_t byteValue = iValue & 0xFF;

	*((uint8_t*)Object + AmCrtHeadVariation) = byteValue;
}


int GetHeadVariationPersist(GameObject* Object) {
    char* ptrAppBlock = creatureApparenceBlock((char*)Object);
    return *(uint8_t*)(ptrAppBlock + AmCrtABHeadVariation);
}

void SetHeadVariationPersist(GameObject* Object, int iValue) {
	uint8_t byteValue = iValue & 0xFF;

    char* ptrAppBlock = creatureApparenceBlock((char*)Object);
    *(uint8_t*)(ptrAppBlock + AmCrtABHeadVariation) = byteValue;
}


//Hair
int GetHairVariation(GameObject* Object) {
	return *(uint8_t*)((char*)Object + AmCrtHairVariation);
}

void SetHairVariation(GameObject* Object, int iValue) {
	uint8_t byteValue = iValue & 0xFF;

	*((uint8_t*)Object + AmCrtHairVariation) = byteValue;
}


int GetHairVariationPersist(GameObject* Object) {
	char* ptrAppBlock = creatureApparenceBlock((char*)Object);
	return *(uint8_t*)(ptrAppBlock + AmCrtABHairVariation);
}

void SetHairVariationPersist(GameObject* Object, int iValue) {
	uint8_t byteValue = iValue & 0xFF;

	char* ptrAppBlock = creatureApparenceBlock((char*)Object);
	*(uint8_t*)(ptrAppBlock + AmCrtABHairVariation) = byteValue;
}


//FHair
int GetFHairVariation(GameObject* Object) {
	return *(uint8_t*)((char*)Object + AmCrtFacialHairVariation);
}

void SetFHairVariation(GameObject* Object, int iValue) {
	uint8_t byteValue = iValue & 0xFF;

	*((uint8_t*)Object + AmCrtFacialHairVariation) = byteValue;
}


int GetFHairVariationPersist(GameObject* Object) {
	char* ptrAppBlock = creatureApparenceBlock((char*)Object);
	return *(uint8_t*)(ptrAppBlock + AmCrtABFacialHairVariation);
}

void SetFHairVariationPersist(GameObject* Object, int iValue) {
	uint8_t byteValue = iValue & 0xFF;

	char* ptrAppBlock = creatureApparenceBlock((char*)Object);
	*(uint8_t*)(ptrAppBlock + AmCrtABFacialHairVariation) = byteValue;
}



//Tail
int GetTailVariation(GameObject* Object) {
	return *(uint8_t*)((char*)Object + AmCrtTailVariation);
}

void SetTailVariation(GameObject* Object, int iValue) {
	uint8_t byteValue = iValue & 0xFF;

	*((uint8_t*)Object + AmCrtTailVariation) = byteValue;
}


int GetTailVariationPersist(GameObject* Object) {
	char* ptrAppBlock = creatureApparenceBlock((char*)Object);
	return *(uint8_t*)(ptrAppBlock + AmCrtABTailVariation);
}

void SetTailVariationPersist(GameObject* Object, int iValue) {
	uint8_t byteValue = iValue & 0xFF;

	char* ptrAppBlock = creatureApparenceBlock((char*)Object);
	*(uint8_t*)(ptrAppBlock + AmCrtABTailVariation) = byteValue;
}

//Wing
int GetWingVariation(GameObject* Object) {
	return *((uint8_t*)Object + AmCrtWingVariation);
}

void SetWingVariation(GameObject* Object, int iValue) {
	uint8_t byteValue = iValue & 0xFF;

	*((uint8_t*)Object + AmCrtWingVariation) = byteValue;
}


int GetWingVariationPersist(GameObject* Object) {
	char* ptrAppBlock = creatureApparenceBlock((char*)Object);
	return *(uint8_t*)(ptrAppBlock + AmCrtABWingVariation);
}

void SetWingVariationPersist(GameObject* Object, int iValue) {
	uint8_t byteValue = iValue & 0xFF;

	char* ptrAppBlock = creatureApparenceBlock((char*)Object);
	*(uint8_t*)(ptrAppBlock + AmCrtABWingVariation) = byteValue;
}


//Age
int GetCreatureAge(GameObject* Object, bool bPersist) {
	if(bPersist == false)
		return *(int*)((char*)Object + AmCrtAge);
	else {
		char* ptrAppBlock = creatureApparenceBlock((char*)Object);
		return *(int*)(ptrAppBlock + AmCrtABAge);
	}
}

void SetCreatureAge(GameObject* Object, int iAge, bool bPersist) {
	if(bPersist == false)
		*(int*)((char*)Object + AmCrtAge) = iAge;
	else {
		char* ptrAppBlock = creatureApparenceBlock((char*)Object);
		*(int*)(ptrAppBlock + AmCrtABAge) = iAge;
	}
}

//Race
int GetCreatureRace(GameObject* Object)
{
	char* ptrAppBlock = creatureApparenceBlock((char*)Object);
	return *(uint16_t*)(ptrAppBlock + AmCrtABRace);
}

void SetCreatureRace(GameObject* Object, int iValue) 
{
	uint16_t shortValue = iValue & 0xFFFF;

	char* ptrAppBlock = creatureApparenceBlock((char*)Object);
	*(uint16_t*)(ptrAppBlock + AmCrtABRace) = shortValue;
}

//SubRace
int GetCreatureSubRace(GameObject* Object)
{
	char* ptrAppBlock = creatureApparenceBlock((char*)Object);
	return *(uint16_t*)(ptrAppBlock + AmCrtABSubRace);
}

void SetCreatureSubRace(GameObject* Object, int iValue) {
	uint16_t shortValue = iValue & 0xFFFF;

	char* ptrAppBlock = creatureApparenceBlock((char*)Object);
	*(uint16_t*)(ptrAppBlock + AmCrtABSubRace) = shortValue;
}


//Gender (visual)
int GetVisualGender(GameObject* Object) {
	return *((char*)Object + AmCrtGenderVis);
}

void SetVisualGender(GameObject* Object, int iGender) {
	uint8_t byteGender = iGender & 0xFF;

	*((char*)Object + AmCrtGenderVis) = byteGender;
}


int GetCreatureSoundSet(GameObject* Object) { 
	return *(uint16_t*)((char*)Object + AmCrtSoundSet);
}

void SetCreatureSoundSet(GameObject* Object, int iSoundSet) {
	uint16_t shortSoundSet = iSoundSet & 0xFFFF;
	*(uint16_t*)((char*)Object + AmCrtSoundSet) = shortSoundSet;
}

//TattooColor 1&2 : Not used in game, can be used for something else.
int GetCreatureTattoo(GameObject* Object) {
	char* ptrAppBlock = creatureApparenceBlock((char*)Object);
	return *(uint16_t*)(ptrAppBlock + AmCrtABTatoo1and2);
}

void SetCreatureTattoo(GameObject* Object, int iValue) {
	uint16_t shortValue = iValue & 0xFFFF;

	char* ptrAppBlock = creatureApparenceBlock((char*)Object);
	*(uint16_t*)(ptrAppBlock + AmCrtABTatoo1and2) = shortValue;
}



//Persistant Scale
float GetCreaturePersistScaleX(GameObject* Object) {
		char* ptrAppBlock = creatureApparenceBlock((char*)Object);
		return *(float*)(ptrAppBlock + AmCrtABPersistScaleX);
}

void SetCreaturePersistScaleX(GameObject* Object, float fValue) {
		char* ptrAppBlock = creatureApparenceBlock((char*)Object);
		*(float*)(ptrAppBlock + AmCrtABPersistScaleX) = fValue;
}

float GetCreaturePersistScaleY(GameObject* Object) {
		char* ptrAppBlock = creatureApparenceBlock((char*)Object);
		return *(float*)(ptrAppBlock + AmCrtABPersistScaleY);
}

void SetCreaturePersistScaleY(GameObject* Object, float fValue) {
		char* ptrAppBlock = creatureApparenceBlock((char*)Object);
		*(float*)(ptrAppBlock + AmCrtABPersistScaleY) = fValue;
}

float GetCreaturePersistScaleZ(GameObject* Object) {
		char* ptrAppBlock = creatureApparenceBlock((char*)Object);
		return *(float*)(ptrAppBlock + AmCrtABPersistScaleZ);
}

void SetCreaturePersistScaleZ(GameObject* Object, float fValue) {
		char* ptrAppBlock = creatureApparenceBlock((char*)Object);
		*(float*)(ptrAppBlock + AmCrtABPersistScaleZ) = fValue;
}


//AC Stuff
void SetCreatureAC(GameObject* Object, int iAC, int iValue) {
	char* ptrWork = creatureApparenceBlock((char*)Object);
	ptrWork = (ptrWork + AmCrtABArmorTable);
	if (iAC < 0 || iAC > 12)
		iAC = 0;
	ptrWork = (ptrWork + iAC);

	*(int8_t*)ptrWork = (int8_t)(iValue & 0xFF);
}

int GetCreatureAC(GameObject* Object, int iAC) {
	char* ptrWork = creatureApparenceBlock((char*)Object);
	ptrWork = (ptrWork + AmCrtABArmorTable);
	if (iAC < 0 || iAC > 12)
		iAC = 0;
	ptrWork = (ptrWork + iAC);

	return *(int8_t*)ptrWork;
}

//Weight Part
int GetCreatureWeightTotal(GameObject* Object) {
	char* ptrWork = (char*)Object;
	return *(int32_t*)(ptrWork + AmCrtWeightTotal);
}

void SetCreatureWeightTotal(GameObject* Object, int32_t iValue) {
	char* ptrWork = (char*)Object;
	*(int32_t*)(ptrWork + AmCrtWeightTotal) = iValue;
}

int GetCreatureWeightEquipment(GameObject* Object) {
	char* ptrWork = (char*)Object;
	return *(int32_t*)(ptrWork + AmCrtWeightEq);
}

void SetCreatureWeightEquipment(GameObject* Object, int32_t iValue) {
	char* ptrWork = (char*)Object;
	*(int32_t*)(ptrWork + AmCrtWeightEq) = iValue;
}

//Visual "CreatureArmor" Stuff

void SetCreatureModelPartMask(GameObject* Object, int iValue) {
	char* ptrAppBlock = creatureApparenceBlock((char*)Object);
	ABCreatureArmor* myCreaArmor = (ABCreatureArmor*)(ptrAppBlock + AmCrtABCreatureArmor);

	myCreaArmor->m_ShowItemMask = (uint8_t)(iValue & 0xFF);
}

int GetCreatureModelPartMask(GameObject* Object) {
	char* ptrAppBlock = creatureApparenceBlock((char*)Object);
	ABCreatureArmor* myCreaArmor = (ABCreatureArmor*)(ptrAppBlock + AmCrtABCreatureArmor);

	return myCreaArmor->m_ShowItemMask;
}

void SetCreatureArmorVariation(GameObject* Object, int iValue) {
	char* ptrAppBlock = creatureApparenceBlock((char*)Object);
	ABCreatureArmor* myCreaArmor = (ABCreatureArmor*)(ptrAppBlock + AmCrtABCreatureArmor);

	myCreaArmor->m_abMainArmor.m_ArmorApp[0] = (uint8_t)(iValue & 0xFF);
}
int GetCreatureArmorVariation(GameObject* Object) {
	char* ptrAppBlock = creatureApparenceBlock((char*)Object);
	ABCreatureArmor* myCreaArmor = (ABCreatureArmor*)(ptrAppBlock + AmCrtABCreatureArmor);

	return myCreaArmor->m_abMainArmor.m_ArmorApp[0];
}

void SetCreatureArmorCategory(GameObject* Object, int iValue) {
	char* ptrAppBlock = creatureApparenceBlock((char*)Object);
	ABCreatureArmor* myCreaArmor = (ABCreatureArmor*)(ptrAppBlock + AmCrtABCreatureArmor);

	myCreaArmor->m_abMainArmor.m_ArmorApp[1] = (uint8_t)(iValue & 0xFF);
}
int GetCreatureArmorCategory(GameObject* Object) {
	char* ptrAppBlock = creatureApparenceBlock((char*)Object);
	ABCreatureArmor* myCreaArmor = (ABCreatureArmor*)(ptrAppBlock + AmCrtABCreatureArmor);

	return myCreaArmor->m_abMainArmor.m_ArmorApp[1];
}

void SetCreatureArmorPartVariation(GameObject* Object, int iModelPart, int iValue) {
	char* ptrAppBlock = creatureApparenceBlock((char*)Object);
	ABCreatureArmor* myCreaArmor = (ABCreatureArmor*)(ptrAppBlock + AmCrtABCreatureArmor);

	if(iModelPart < 0 || iModelPart > 4)
		return;

	myCreaArmor->m_ItemPart[iModelPart].m_ArmorApp[0] = (uint8_t)(iValue & 0xFF);
}

int GetCreatureArmorPartVariation(GameObject* Object, int iModelPart) {
	char* ptrAppBlock = creatureApparenceBlock((char*)Object);
	ABCreatureArmor* myCreaArmor = (ABCreatureArmor*)(ptrAppBlock + AmCrtABCreatureArmor);

	if(iModelPart < 0 || iModelPart > 4)
		return 0;

	return myCreaArmor->m_ItemPart[iModelPart].m_ArmorApp[0];
}

void SetCreatureArmorPartCategory(GameObject* Object, int iModelPart, int iValue) {
	char* ptrAppBlock = creatureApparenceBlock((char*)Object);
	ABCreatureArmor* myCreaArmor = (ABCreatureArmor*)(ptrAppBlock + AmCrtABCreatureArmor);

	if(iModelPart < 0 || iModelPart > 4)
		return;

	myCreaArmor->m_ItemPart[iModelPart].m_ArmorApp[1] = (uint8_t)(iValue & 0xFF);
}

int GetCreatureArmorPartCategory(GameObject* Object, int iModelPart) {
	char* ptrAppBlock = creatureApparenceBlock((char*)Object);
	ABCreatureArmor* myCreaArmor = (ABCreatureArmor*)(ptrAppBlock + AmCrtABCreatureArmor);

	if(iModelPart < 0 || iModelPart > 4)
		return 0;

	return myCreaArmor->m_ItemPart[iModelPart].m_ArmorApp[1];
}


void SetCreatureArmoPieceVariation(GameObject* Object, int iModelPiece, int iValue) {
	char* ptrAppBlock = creatureApparenceBlock((char*)Object);
	ABCreatureArmor* myCreaArmor = (ABCreatureArmor*)(ptrAppBlock + AmCrtABCreatureArmor);
	
	if(iModelPiece < ARMOR_MODEL_PIECE_LEFT_SHOULDER || iModelPiece > ARMOR_MODEL_PIECE_RIGHT_ANKLE)
		return;

	int iModelIdx = iModelPiece - ARMOR_MODEL_PIECE_LEFT_SHOULDER;


	myCreaArmor->m_abMainArmor.m_ModelPiece[iModelIdx].iVariation = (uint8_t)(iValue & 0xFF);
}
int GetCreatureArmoPieceVariation(GameObject* Object, int iModelPiece) {
	char* ptrAppBlock = creatureApparenceBlock((char*)Object);
	ABCreatureArmor* myCreaArmor = (ABCreatureArmor*)(ptrAppBlock + AmCrtABCreatureArmor);
	
	if(iModelPiece < ARMOR_MODEL_PIECE_LEFT_SHOULDER || iModelPiece > ARMOR_MODEL_PIECE_RIGHT_ANKLE)
		return 0;

	int iModelIdx = iModelPiece - ARMOR_MODEL_PIECE_LEFT_SHOULDER;


	return myCreaArmor->m_abMainArmor.m_ModelPiece[iModelIdx].iVariation;
}

void SetCreatureArmorPieceUVScroll(GameObject* Object, int iModelPiece, bool bValue) {
	char* ptrAppBlock = creatureApparenceBlock((char*)Object);
	ABCreatureArmor* myCreaArmor = (ABCreatureArmor*)(ptrAppBlock + AmCrtABCreatureArmor);
	
	if(iModelPiece < ARMOR_MODEL_PIECE_LEFT_SHOULDER || iModelPiece > ARMOR_MODEL_PIECE_RIGHT_ANKLE)
		return;

	int iModelIdx = iModelPiece - ARMOR_MODEL_PIECE_LEFT_SHOULDER;


	myCreaArmor->m_abMainArmor.m_ModelPiece[iModelIdx].m_UVScroll.m_bActive = (bValue?1:0);
}

int GetCreatureArmorPieceUVScroll(GameObject* Object, int iModelPiece) {
	char* ptrAppBlock = creatureApparenceBlock((char*)Object);
	ABCreatureArmor* myCreaArmor = (ABCreatureArmor*)(ptrAppBlock + AmCrtABCreatureArmor);
	
	if(iModelPiece < ARMOR_MODEL_PIECE_LEFT_SHOULDER || iModelPiece > ARMOR_MODEL_PIECE_RIGHT_ANKLE)
		return 0;

	int iModelIdx = iModelPiece - ARMOR_MODEL_PIECE_LEFT_SHOULDER;


	int iVal = myCreaArmor->m_abMainArmor.m_ModelPiece[iModelIdx].m_UVScroll.m_bActive;
	
	return ((iVal!=0)?1:0);
}


void SetCreatureArmorPieceUScroll(GameObject* Object, int iModelPiece, float fValue) {
	char* ptrAppBlock = creatureApparenceBlock((char*)Object);
	ABCreatureArmor* myCreaArmor = (ABCreatureArmor*)(ptrAppBlock + AmCrtABCreatureArmor);
	
	if(iModelPiece < ARMOR_MODEL_PIECE_LEFT_SHOULDER || iModelPiece > ARMOR_MODEL_PIECE_RIGHT_ANKLE)
		return;

	int iModelIdx = iModelPiece - ARMOR_MODEL_PIECE_LEFT_SHOULDER;

	myCreaArmor->m_abMainArmor.m_ModelPiece[iModelIdx].m_UVScroll.m_fUScroll = fValue;
}

float GetCreatureArmorPieceUScroll(GameObject* Object, int iModelPiece) {
	char* ptrAppBlock = creatureApparenceBlock((char*)Object);
	ABCreatureArmor* myCreaArmor = (ABCreatureArmor*)(ptrAppBlock + AmCrtABCreatureArmor);
	
	if(iModelPiece < ARMOR_MODEL_PIECE_LEFT_SHOULDER || iModelPiece > ARMOR_MODEL_PIECE_RIGHT_ANKLE)
		return 0;

	int iModelIdx = iModelPiece - ARMOR_MODEL_PIECE_LEFT_SHOULDER;

	return myCreaArmor->m_abMainArmor.m_ModelPiece[iModelIdx].m_UVScroll.m_fUScroll;
}

void SetCreatureArmorPieceVScroll(GameObject* Object, int iModelPiece, float fValue) {
	char* ptrAppBlock = creatureApparenceBlock((char*)Object);
	ABCreatureArmor* myCreaArmor = (ABCreatureArmor*)(ptrAppBlock + AmCrtABCreatureArmor);
	
	if(iModelPiece < ARMOR_MODEL_PIECE_LEFT_SHOULDER || iModelPiece > ARMOR_MODEL_PIECE_RIGHT_ANKLE)
		return;

	int iModelIdx = iModelPiece - ARMOR_MODEL_PIECE_LEFT_SHOULDER;

	myCreaArmor->m_abMainArmor.m_ModelPiece[iModelIdx].m_UVScroll.m_fVScroll = fValue;
}

float GetCreatureArmorPieceVScroll(GameObject* Object, int iModelPiece) {
	char* ptrAppBlock = creatureApparenceBlock((char*)Object);
	ABCreatureArmor* myCreaArmor = (ABCreatureArmor*)(ptrAppBlock + AmCrtABCreatureArmor);
	
	if(iModelPiece < ARMOR_MODEL_PIECE_LEFT_SHOULDER || iModelPiece > ARMOR_MODEL_PIECE_RIGHT_ANKLE)
		return 0;

	int iModelIdx = iModelPiece - ARMOR_MODEL_PIECE_LEFT_SHOULDER;

	return myCreaArmor->m_abMainArmor.m_ModelPiece[iModelIdx].m_UVScroll.m_fVScroll;
}


NWN::D3DXCOLOR* GetCreatureColorPtr(std::string sCommand, GameObject* Object) {
	char* retValue = NULL;

	if(sCommand.find("Color")  == std::string::npos)
		return NULL;

	if (sCommand == "Color1") {
		retValue = ((char*)Object) + AmCrtTint;
	}
	else if (sCommand == "Color1P") {
		char* ptrAppBlock = creatureApparenceBlock((char*)Object);
		retValue          = ptrAppBlock + AmCrtABTint ;
	}
	else if (sCommand == "Color2") {
		retValue = ((char*)Object) + AmCrtTint +0x10;
	}
	else if (sCommand == "Color2P") {
		char* ptrAppBlock = creatureApparenceBlock((char*)Object);
		retValue          = ptrAppBlock + AmCrtABTint+0x10;
	}
	else if (sCommand == "Color3") {
		retValue = ((char*)Object) + AmCrtTint+0x20;
	}
	else if (sCommand == "Color3P") {
		char* ptrAppBlock = creatureApparenceBlock((char*)Object);
		retValue          = ptrAppBlock + AmCrtABTint+0x20;
	}
	else if (sCommand == "ColorHeadSkin")
	{
		retValue = ((char*)Object) + AmCrtHeadTint;
	}
	else if (sCommand == "ColorHeadSkinP") {
		char* ptrAppBlock = creatureApparenceBlock((char*)Object);
		retValue          = ptrAppBlock + AmCrtABHeadTint;
	}
	else if (sCommand == "ColorHeadEye")
	{
		retValue = ((char*)Object) + AmCrtHeadTint+0x10;
	}
	else if (sCommand == "ColorHeadEyeP") {
		char* ptrAppBlock = creatureApparenceBlock((char*)Object);
		retValue          = ptrAppBlock + AmCrtABHeadTint+0x10;
	}
	else if (sCommand == "ColorHeadBrow")
	{
		retValue = ((char*)Object) + AmCrtHeadTint+0x20;
	}
	else if (sCommand == "ColorHeadBrowP") {
		char* ptrAppBlock = creatureApparenceBlock((char*)Object);
		retValue          = ptrAppBlock + AmCrtABHeadTint+0x20;
	}
	else if (sCommand == "ColorHairHacc") {
		retValue = ((char*)Object) + AmCrtHairTint;
	}
	else if (sCommand == "ColorHairHaccP") {
		char* ptrAppBlock = creatureApparenceBlock((char*)Object);
		retValue          = ptrAppBlock + AmCrtABHairTint;
	}
	else if (sCommand == "ColorHair1") {
		retValue = ((char*)Object) + AmCrtHairTint + 0x10;
	}
	else if (sCommand == "ColorHair1P") {
		char* ptrAppBlock = creatureApparenceBlock((char*)Object);
		retValue          = ptrAppBlock + AmCrtABHairTint + 0x10;
	}
	else if (sCommand == "ColorHair2") {
		retValue = ((char*)Object) + AmCrtHairTint + 0x20;
	}
	else if (sCommand == "ColorHair2P") {
		char* ptrAppBlock = creatureApparenceBlock((char*)Object);
		retValue          = ptrAppBlock + AmCrtABHairTint + 0x20;
	}
	else if(sCommand == "ArmorColor1") {
		char* ptrAppBlock = creatureApparenceBlock((char*)Object);
		ABCreatureArmor* myCreaArmor = (ABCreatureArmor*)(ptrAppBlock + AmCrtABCreatureArmor);
		retValue = (char*) &(myCreaArmor->m_abMainArmor.m_Color[0]);
	}
	else if(sCommand == "ArmorColor2") {
		char* ptrAppBlock = creatureApparenceBlock((char*)Object);
		ABCreatureArmor* myCreaArmor = (ABCreatureArmor*)(ptrAppBlock + AmCrtABCreatureArmor);
		retValue = (char*) &(myCreaArmor->m_abMainArmor.m_Color[1]);
	}
	else if(sCommand == "ArmorColor3") {
		char* ptrAppBlock = creatureApparenceBlock((char*)Object);
		ABCreatureArmor* myCreaArmor = (ABCreatureArmor*)(ptrAppBlock + AmCrtABCreatureArmor);
		retValue = (char*) &(myCreaArmor->m_abMainArmor.m_Color[2]);
	}
	else if(sCommand.rfind(cst_ModelPart,0) == 0) {
		sCommand = sCommand.substr(cst_ModelPart.size());
		char* pEnd;
		int iModelPart = strtol(sCommand.c_str(), &pEnd, 10);
		sCommand = pEnd;
		
		char* ptrAppBlock = creatureApparenceBlock((char*)Object);
		ABCreatureArmor* myCreaArmor = (ABCreatureArmor*)(ptrAppBlock + AmCrtABCreatureArmor);
		
		if(iModelPart < 0 || iModelPart > 4)
			return NULL;

		if(sCommand == "Color1")
			retValue = (char*) &(myCreaArmor->m_ItemPart[iModelPart].m_Color[0]);
		else if(sCommand == "Color2")
			retValue = (char*) &(myCreaArmor->m_ItemPart[iModelPart].m_Color[1]);
		else if(sCommand == "Color3")
			retValue = (char*) &(myCreaArmor->m_ItemPart[iModelPart].m_Color[2]);
	}
	else if(sCommand.rfind(cst_ModelPiece, 0) == 0) {
		sCommand = sCommand.substr(cst_ModelPiece.size());
		char* pEnd;
		int iModelPiece = strtol(sCommand.c_str(), &pEnd, 10);
		sCommand = pEnd;

		if(iModelPiece < ARMOR_MODEL_PIECE_LEFT_SHOULDER || iModelPiece > ARMOR_MODEL_PIECE_RIGHT_ANKLE)
			return NULL;

		int iModelIdx = iModelPiece - ARMOR_MODEL_PIECE_LEFT_SHOULDER;

		char* ptrAppBlock = creatureApparenceBlock((char*)Object);
		ABCreatureArmor* myCreaArmor = (ABCreatureArmor*)(ptrAppBlock + AmCrtABCreatureArmor);

		if(sCommand == "Color1")
			retValue = (char*) &(myCreaArmor->m_abMainArmor.m_ModelPiece[iModelIdx].m_Color[0]);
		else if(sCommand == "Color2")
			retValue = (char*) &(myCreaArmor->m_abMainArmor.m_ModelPiece[iModelIdx].m_Color[1]);
		else if(sCommand == "Color3")
			retValue = (char*) &(myCreaArmor->m_abMainArmor.m_ModelPiece[iModelIdx].m_Color[2]);
	}

	return (NWN::D3DXCOLOR*)retValue;
}


//Getter/Setter
int CreatureGetInt(char* cCommand, int iObjectID) {
	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;
	std::string sCommand(cCommand);

	ObjectId = (NWN::OBJECTID) iObjectID;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return 0;

    if(Object->GetObjectType() != NWN::OBJECT_TYPE_CREATURE)
        return 0;


	//CreatureSpecificStuff
	if (sCommand == "Age")
		return GetCreatureAge(Object, false);
	else if (sCommand == "AgePersist")
		return GetCreatureAge(Object, true);
	else if (sCommand == "VisualGender")
		return GetVisualGender(Object);
	else if (sCommand == "HeadVar")
		return GetHeadVariation(Object);
	else if (sCommand == "HeadVarP")
		return GetHeadVariationPersist(Object);
	else if (sCommand == "HairVar")
		return GetHairVariation(Object);
	else if (sCommand == "HairVarP")
		return GetHairVariationPersist(Object);
	else if (sCommand == "FHairVar")
		return GetFHairVariation(Object);
	else if (sCommand == "FHairVarP")
		return GetFHairVariationPersist(Object);
	else if (sCommand == "TailVar")
		return GetTailVariation(Object);
	else if (sCommand == "TailVarP")
		return GetTailVariationPersist(Object);
	else if (sCommand == "WingVar")
		return GetWingVariation(Object);
	else if (sCommand == "WingVarP")
		return GetWingVariationPersist(Object);
	else if (sCommand == "SoundSet")
		return GetCreatureSoundSet(Object);

	else if (sCommand == "WeightTotal")
		return GetCreatureWeightTotal(Object);
	else if (sCommand == "WeightEquipment")
		return GetCreatureWeightEquipment(Object);

	else if (sCommand == "Race")
		return GetCreatureRace(Object);
	else if (sCommand == "SubRace")
		return GetCreatureSubRace(Object);

	else if (sCommand == "Tattoo")
		return GetCreatureTattoo(Object);

	else if (sCommand.rfind("AC", 0) == 0) {
		int iAC = strtol(sCommand.substr(2).c_str(), NULL, 10);
		return GetCreatureAC(Object, iAC);
	}
	//CreatureVisualArmor stuff
	else if (sCommand == "ModelPartMask")
		return GetCreatureModelPartMask(Object);
	else if (sCommand == "Variation")
		return GetCreatureArmorVariation(Object);
	else if (sCommand == "Category")
		return GetCreatureArmorCategory(Object);
	else if (sCommand.rfind(cst_ModelPart, 0) == 0) {
		sCommand = sCommand.substr(cst_ModelPart.size());
		char* pEnd;
		int iModelPart = strtol(sCommand.c_str(), &pEnd, 10);
		sCommand = pEnd;

		if(sCommand == "Variation")
			return GetCreatureArmorPartVariation(Object, iModelPart);
		else if(sCommand == "Category")
			return GetCreatureArmorPartCategory(Object, iModelPart);
	}
	else if (sCommand.rfind(cst_ModelPiece, 0) == 0) {
		sCommand = sCommand.substr(cst_ModelPiece.size());
		char* pEnd;
		int iModelPiece = strtol(sCommand.c_str(), &pEnd, 10);
		sCommand = pEnd;

		if(sCommand == "Variation")
			return GetCreatureArmoPieceVariation(Object, iModelPiece);
		else if(sCommand == "UVScroll")
			return GetCreatureArmorPieceUVScroll(Object, iModelPiece);
	}
	return -1;
}

void CreatureSetInt(char* cCommand, int iObjectID, int iValue) {
	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;
	std::string sCommand(cCommand);

	ObjectId = (NWN::OBJECTID) iObjectID;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return;

    if(Object->GetObjectType() != NWN::OBJECT_TYPE_CREATURE)
        return;

	//CreatureSpecificStuff
	if (sCommand == "Age")
		return SetCreatureAge(Object, iValue, false);
	else if (sCommand == "AgePersist")
		return SetCreatureAge(Object, iValue, true);
	else if (sCommand == "VisualGender")
		return SetVisualGender(Object, iValue);
	else if (sCommand == "HeadVar")
		return SetHeadVariation(Object, iValue);
	else if (sCommand == "HeadVarP")
		return SetHeadVariationPersist(Object, iValue);
	else if (sCommand == "HairVar")
		return SetHairVariation(Object, iValue);
	else if (sCommand == "HairVarP")
		return SetHairVariationPersist(Object, iValue);
	else if (sCommand == "FHairVar")
		return SetFHairVariation(Object, iValue);
	else if (sCommand == "FHairVarP")
		return SetFHairVariationPersist(Object, iValue);
	else if (sCommand == "TailVar")
		return SetTailVariation(Object, iValue);
	else if (sCommand == "TailVarP")
		return SetTailVariationPersist(Object, iValue);
	else if (sCommand == "WingVar")
		return SetWingVariation(Object, iValue);
	else if (sCommand == "WingVarP")
		return SetWingVariationPersist(Object, iValue);
	else if (sCommand == "SoundSet")
		return SetCreatureSoundSet(Object, iValue);

	else if (sCommand == "WeightTotal")
		return SetCreatureWeightTotal(Object, iValue);
	else if (sCommand == "WeightEquipment")
		return SetCreatureWeightEquipment(Object, iValue);

	else if (sCommand == "Race")
		return SetCreatureRace(Object, iValue);
	else if (sCommand == "SubRace")
		return SetCreatureSubRace(Object, iValue);

	else if (sCommand == "Tattoo")
		return SetCreatureTattoo(Object, iValue);

	else if (sCommand.rfind("AC", 0) == 0) {
		int iAC = strtol(sCommand.substr(2).c_str(), NULL, 10);
		return SetCreatureAC(Object, iAC, iValue);
	}
	//CreatureVisualArmor stuff
	else if (sCommand == "ModelPartMask")
		return SetCreatureModelPartMask(Object, iValue);
	else if (sCommand == "Variation")
		return SetCreatureArmorVariation(Object, iValue);
	else if (sCommand == "Category")
		return SetCreatureArmorCategory(Object, iValue);
	else if (sCommand.rfind(cst_ModelPart, 0) == 0) {
		sCommand = sCommand.substr(cst_ModelPart.size());
		char* pEnd;
		int iModelPart = strtol(sCommand.c_str(), &pEnd, 10);
		sCommand = pEnd;

		if(sCommand == "Variation")
			return SetCreatureArmorPartVariation(Object, iModelPart, iValue);
		else if(sCommand == "Category")
			return SetCreatureArmorPartCategory(Object, iModelPart, iValue);
	}
	else if (sCommand.rfind(cst_ModelPiece, 0) == 0) {
		sCommand = sCommand.substr(cst_ModelPiece.size());
		char* pEnd;
		int iModelPiece = strtol(sCommand.c_str(), &pEnd, 10);
		sCommand = pEnd;

		if(sCommand == "Variation")
			return SetCreatureArmoPieceVariation(Object, iModelPiece, iValue);
		else if(sCommand == "UVScroll")
			return SetCreatureArmorPieceUVScroll(Object, iModelPiece, iValue != 0);
	}
}


float CreatureGetFloat(char* cCommand, int iObjectID) {
	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;
	std::string sCommand(cCommand);

	ObjectId = (NWN::OBJECTID) iObjectID;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return 0.0;

    if(Object->GetObjectType() != NWN::OBJECT_TYPE_CREATURE)
        return 0.0;


	if(sCommand == "PersistScaleX")
		return GetCreaturePersistScaleX(Object);
	else if (sCommand == "PersistScaleY")
		return GetCreaturePersistScaleY(Object);
	else if (sCommand == "PersistScaleZ")
		return GetCreaturePersistScaleZ(Object);
	else if (sCommand == cst_ModelPiece) {
		sCommand = sCommand.substr(cst_ModelPiece.size());
		char* pEnd;
		int iModelPiece = strtol(sCommand.c_str(), &pEnd, 10);
		sCommand = pEnd;

		if(sCommand == "fVScroll")
			return GetCreatureArmorPieceVScroll(Object, iModelPiece); 
		else if(sCommand == "fUScroll")
			return GetCreatureArmorPieceUScroll(Object, iModelPiece);
	}

	return 0.0;
}

void CreatureSetFloat(char* cCommand, int iObjectID, float fValue) {
	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;
	std::string sCommand(cCommand);

	ObjectId = (NWN::OBJECTID) iObjectID;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return;

    if(Object->GetObjectType() != NWN::OBJECT_TYPE_CREATURE)
        return;


	if(sCommand == "PersistScaleX")
		return SetCreaturePersistScaleX(Object, fValue);
	else if (sCommand == "PersistScaleY")
		return SetCreaturePersistScaleY(Object, fValue);
	else if (sCommand == "PersistScaleZ")
		return SetCreaturePersistScaleZ(Object, fValue);
	else if (sCommand == cst_ModelPiece) {
		sCommand = sCommand.substr(cst_ModelPiece.size());
		char* pEnd;
		int iModelPiece = strtol(sCommand.c_str(), &pEnd, 10);
		sCommand = pEnd;

		if(sCommand == "fVScroll")
			return SetCreatureArmorPieceVScroll(Object, iModelPiece, fValue); 
		else if(sCommand == "fUScroll")
			return SetCreatureArmorPieceUScroll(Object, iModelPiece, fValue);
	}
}


std::string CreatureGetString(char* sCommand, int iObjectID) {
	std::string sResult = "";
	std::string stCommand(sCommand);

	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;

	ObjectId = (NWN::OBJECTID) iObjectID;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return sResult;

	if(Object->GetObjectType() != NWN::OBJECT_TYPE_CREATURE)
		return sResult;

	NWN::D3DXCOLOR* ObjColor = NULL;
	ObjColor = GetCreatureColorPtr(stCommand, Object);

	if(ObjColor != NULL) {
		sResult = ColorToString(ObjColor);
	} else {
		//OTHER STRING STUFF
	}

	return sResult;
}

void CreatureSetString(char* sCommand, int iObjectID, char* sValue) {
	std::string stCommand(sCommand);

	NWN::OBJECTID       ObjectId;
	GameObject        *Object;
	GameObjectManager m_ObjectManager;

	ObjectId = (NWN::OBJECTID) iObjectID;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	if ((Object = m_ObjectManager.GetGameObject( ObjectId )) == NULL)
		return;

	if(Object->GetObjectType() != NWN::OBJECT_TYPE_CREATURE)
		return;

	NWN::D3DXCOLOR* ObjColor = NULL;
	ObjColor = GetCreatureColorPtr(stCommand, Object);

	if(ObjColor != NULL) {
		NWN::D3DXCOLOR myColor;
		//Color is Ok, we can apply it
		if(StringToColor(sValue, &myColor)) {
			memcpy(ObjColor, &myColor, sizeof(myColor));
		}
	} else {
		//Other String stuff
	}
}

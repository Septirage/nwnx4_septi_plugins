
#include <string>
#include <iostream>
#include <unordered_set>

#include "../../NWN2Lib/NWN2.h"
#include "../../NWN2Lib/NWN2Common.h"
#include "../../misc/Patch.h"
#include "../../hook/scriptManagement.h"

#include "../../misc/log.h"
#include "nwn2heap.h"
#include "Connection.h"
#include "../../septutil/intern2daMgt.h"

#include <detours/detours.h>
extern std::unique_ptr<LogNWNX> logger;

float gfMinScaleX = 0.95f;
float gfMinScaleZ = 0.95f;
float gfMaxScaleX = 1.05f;
float gfMaxScaleZ = 1.05f;
float gMsgServerStopFirstCreation = false;

int g_AnticheatActivated = 0;

namespace NWN
{
	NWN::CAppManager *& g_pAppManager = *(NWN::CAppManager **) OFFS_g_pAppManager;
}

struct smallClass {
	uint8_t classId;
	uint8_t classLvl;
};

struct smallSkill {
	uint8_t skillId;
	uint8_t nbPoint;
	uint8_t maxRank;
	int8_t skillCost;
};

#define FEAT_QUICKT_TO_MASTER	0x102
#define FEAT_ABLE_LEARNER		0x6EE
#define FEAT_SKILLED			0x6ED

//TestIfClassOK 5a3c80 (creaAppBloc, classID);
#define FUNC_TestIfClassOK						0x005a3c80
#define FUNC_CalculateAbility					0x005a24c0
#define FUNC_ComputeAbilityEffectBonuses		0x005f52f0
#define FUNC_GetBaseAttackCalculate				0x007564f0
#define FUNC_TestIfFeatOK						0x0075d770

#define SEPT_ANTICHEAT_STR						0
#define SEPT_ANTICHEAT_DEX						1
#define SEPT_ANTICHEAT_CON						2
#define SEPT_ANTICHEAT_INT						3
#define SEPT_ANTICHEAT_WIS						4
#define SEPT_ANTICHEAT_CHA						5


__declspec(naked) NWN::CNWSPlayer * __fastcall GetClientObjectByPlayerId(__in struct NWN::CServerExoApp * This, __in void * Unused, __in NWN::PLAYERID PlayerId, __in UCHAR ExpansionMask)
{
	__asm
	{
		mov     edx, OFFS_CServerExoApp_GetClientObjectByPlayerId ; set jump target
		jmp     edx ; branch to target procedure
	}
}

std::string ExoStringToString(const NWN::CExoString* myExoString) {
	if(myExoString != NULL) {
		if(myExoString->m_sString != NULL) {
			if(myExoString->m_sString) {
				std::string result(myExoString->m_sString, myExoString->m_nBufferLength);
				if (!result.empty() && result.back() == '\0') {
					result.resize(result.size() - 1);
				}
				return result;
			}
		}
	}
	return "";
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////// Start Of Level Up AntiCheats ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

__declspec(naked) uint32_t __fastcall GetBaseAttackCalculate(__in void* CreatureBlockPtr, __in void* Unused)
{
	__asm
	{
		mov		edx, FUNC_GetBaseAttackCalculate;
		jmp		edx;
	}
}

__declspec(naked) uint32_t __fastcall CalculateAbility(__in void* CreatureBlockPtr, __in void* Unused,  __in int iAbilityID)
{
	__asm
	{
		mov		edx, FUNC_CalculateAbility;
		jmp		edx;
	}
}

__declspec(naked) uint32_t __fastcall ComputeAbilityEffectBonuses_intern(__in void* CreaturePtr, __in void* Unused,  __in int iValue4, __in int i1, __in int i2, __in int i3, __in int i4, __in int i5, __in int i6, __in int iAbilityID, __in int i7)
{
	__asm
	{
		mov		edx, FUNC_ComputeAbilityEffectBonuses;
		jmp		edx;
	}
}

__declspec(naked) uint32_t __fastcall TestIfClassOK(__in void* CreatureBlockPtr, __in void* Unused, __in int idClass)
{
	__asm
	{
		mov		edx, FUNC_TestIfClassOK;
		jmp		edx;
	}
}

__declspec(naked) uint32_t __fastcall TestIfFeatOK_intern(__in void* CreatureStatPtr, __in void* Unused, __in int idFeat, __in int* Unknow)
{
	__asm
	{
		mov			edx, FUNC_TestIfFeatOK;
		jmp			edx;
	}
}

uint32_t ComputeAbilityEffectBonuses(void* CreaturePtr, uint32_t iAbility)
{
	return ComputeAbilityEffectBonuses_intern(CreaturePtr, NULL, 4, 0, 0, 0, 0, 0, 0, iAbility, 0);
}

uint32_t TestIfFeatOk(uint8_t* uPcBlock, uint16_t featToTest)
{
	int Unknow[4] = { 0 };
	return TestIfFeatOK_intern(uPcBlock, NULL, featToTest, Unknow);
}

bool FeatValidAllClasses(uint16_t featToTest) {
	NWN2DA::feats2DA myFeatToTest = GetFeatRow(featToTest);

	if (myFeatToTest != NULL)
	{
		//is "valid" ?
		if (myFeatToTest->m_featValid == 1 && myFeatToTest->m_featRemoved == 0) {
			//AllClasses can Use ?
			if (myFeatToTest->m_allClassCanUse == 1)
			{
				return true;
			}
		}
	}
	return false;
}

int LevelUpViolation(MsgServ* g_msgServ, NWN::OBJECTID ObjectId, std::string sPlayerName, int currentLevel, std::string sCharacterName, std::string errorsList)
{
	//Is that a bad player ?
	logger->Warn("[%s] (perhaps) try something nasty for levelup (%d) of %s : \n\t %s \n", sPlayerName.c_str(), currentLevel+1, sCharacterName.c_str(), errorsList.c_str());
	if(g_msgServ->bCallScriptOnLvlUpError)
	{
		int iRetValue = 0;
		bool bExecuted = true;

		NWScript::ClearScriptParams();
		NWScript::AddScriptParameterString(sPlayerName.c_str());
		NWScript::AddScriptParameterObject(ObjectId);
		NWScript::AddScriptParameterString(errorsList.c_str());
		iRetValue = NWScript::ExecuteScriptEnhanced(g_msgServ->ScriptLvlUpError.c_str(), 0, true, &bExecuted);

		//int iRet = ExecuteEnhancedScript_sp(g_msgServ->ScriptLvlUpError, 0, iRetValue, sPlayerName.c_str(), ObjectId, errorsList.c_str());
		//if (iRet == -1)
		if(!bExecuted)
		{
			logger->Err("Error when calling script \"%s\" with %s, %d, %s ", g_msgServ->ScriptLvlUpError.c_str(), sPlayerName.c_str(), ObjectId, errorsList.c_str());
			return -2;
		}
	}
	return -2;
}

int ReallyBadMsg(MsgServ* g_msgServ, std::string sPlayerName, int currentLevel, std::string sCharacterName, std::string errorsList)
{
	logger->Err("[%s] %s : lvl up (%d) message error. Drop it. Current violations detected :\n %s \n", sPlayerName.c_str(), sCharacterName.c_str(), currentLevel + 1, errorsList.c_str());
	return -1;
}

void PrintMessage(unsigned char* Data, int size, std::string sCharName)
{
	if (static_cast<int>(logger->Level()) >= 4)
	{
		std::string sResult = "";
		char buffer[3];
		for (int i = 0; i < size; ++i) {
			sprintf(buffer, "%02X ", Data[i]);
			sResult += buffer;
		}
		logger->Debug("LevelUpMessage for %s: ", sCharName.c_str());
		logger->Debug("%s", sResult.c_str());
	}
}

int CheckForLevelUp(int playerId, unsigned char* Data, int size, std::string sPlayerName, MsgServ* g_msgServ)
{
	/*
	if (size < 61) {
		//Really bad message, drop it
		return -1;
	}
	*/

	////////////////////////////////////////// Data Preparation //////////////////////////////////////////
	std::string errorsList = "";
	GameObjectManager m_ObjectManager;
	GameObject        *Object;
	NWN::CNWSPlayer * Player = GetClientObjectByPlayerId(NWN::g_pAppManager->m_pServerExoApp, NULL, playerId, 0 );

	//Nothing doable for that. Just quit and drop the message.
	if (Player == NULL) {
		logger->Err("LevelUpMessage: playerId not found.");
		return -1;
	}


	NWN::OBJECTID ObjectId = Player->m_oidPCObject;

	if ((ObjectId & NWN::INVALIDOBJID) != 0)
		ObjectId &= ~(NWN::LISTTYPE_MASK);

	//Nothing doable for that. Just quit and drop the message.
	if ((Object = m_ObjectManager.GetGameObject(ObjectId)) == NULL) {
		logger->Err("LevelUpMessage: PlayerObject not found.");
		return -1;
	}
	
	
	//get character name
	std::string sCharacterName = "";
	{
		const NWN::CExoString * FirstName = (NWN::CExoString*)((**(int**)((int)Object + 0x2d8)) + 4);;
		const NWN::CExoString * LastName = (NWN::CExoString*)((**(int**)((int)Object + 0x2e8)) + 4);;

		if ((FirstName->m_sString == NULL) || (FirstName->m_nBufferLength == 0))
		{
			FirstName = NULL;
		}
		
		sCharacterName = ExoStringToString(FirstName);

		if ((LastName->m_sString == NULL) || (LastName->m_nBufferLength == 0))
		{
			LastName = NULL;
		}

		sCharacterName = sCharacterName + " " + ExoStringToString(LastName);
	}

	PrintMessage(Data, size, sCharacterName);

	uint8_t* uPcBlock = *(uint8_t**)((uint8_t*)Object + 0x1FC4);

	//Nb of class this character already have
	uint8_t nb_OwnedClass = uPcBlock[0x14];


	///////////////////////////////////////// Check for classes /////////////////////////////////////////
	int iPos = 7; // skip MsgType & size (3+4).

	//The classID choosen for this levelup
	uint8_t nChoosenClass = Data[iPos];
	iPos++;
	uint8_t nClassPosition = 0;
	uint8_t lvlChoosenClass = 0;
	uint8_t currentLevel = 0;
	uint8_t previousSchool = 0xFF;
	uint8_t previousDomain1 = 0xFF;
	uint8_t previousDomain2 = 0xFF;


	//Can i take that class ?

	// ****** Build myClasses ***** //
	smallClass myClasses[4];
	{
		uint8_t* classBlock = uPcBlock + 0x10C;

		bool isOwnedClass = false;


		myClasses[0].classId = classBlock[4];
		myClasses[0].classLvl = classBlock[5];
		currentLevel += classBlock[5];
		if (myClasses[0].classId == nChoosenClass)
		{
			isOwnedClass = true;
			lvlChoosenClass = classBlock[5];
			previousSchool = classBlock[7];
			previousDomain1 = classBlock[8];
			previousDomain2 = classBlock[9];
		}


		if (nb_OwnedClass > 1)
		{
			classBlock += 0x124;
			myClasses[1].classId = classBlock[4];
			myClasses[1].classLvl = classBlock[5];
			currentLevel += classBlock[5];
			if (myClasses[1].classId == nChoosenClass)
			{
				nClassPosition = 1;
				isOwnedClass = true;
				lvlChoosenClass = classBlock[5];
				previousSchool = classBlock[7];
				previousDomain1 = classBlock[8];
				previousDomain2 = classBlock[9];
			}
		}


		if (nb_OwnedClass > 2)
		{
			classBlock += 0x124;
			myClasses[2].classId = classBlock[4];
			myClasses[2].classLvl = classBlock[5];
			currentLevel += classBlock[5];
			if (myClasses[2].classId == nChoosenClass)
			{
				nClassPosition = 2;
				isOwnedClass = true;
				lvlChoosenClass = classBlock[5];
				previousSchool = classBlock[7];
				previousDomain1 = classBlock[8];
				previousDomain2 = classBlock[9];
			}
		}


		if (nb_OwnedClass > 3)
		{
			classBlock += 0x124;
			myClasses[3].classId = classBlock[4];
			myClasses[3].classLvl = classBlock[5];
			currentLevel += classBlock[5];
			if (myClasses[3].classId == nChoosenClass)
			{
				nClassPosition = 3;
				isOwnedClass = true;
				lvlChoosenClass = classBlock[5];
				previousSchool = classBlock[7];
				previousDomain1 = classBlock[8];
				previousDomain2 = classBlock[9];
			}
		}



		//Too Many classes ?
		if (!isOwnedClass && nb_OwnedClass > 3)
		{
			nb_OwnedClass++;
			//We have too many Classes, it's not ok.
			errorsList += "#too_many_classes:{" + std::to_string(myClasses[0].classId) + ",";
			errorsList += std::to_string(myClasses[1].classId) + ",";
			errorsList += std::to_string(myClasses[2].classId) + ",";
			errorsList += std::to_string(myClasses[3].classId) + ",";
			errorsList += std::to_string(nChoosenClass) + "}";

			if (g_msgServ->bACLvlUpStopFirstViolation)
			{
				if (LevelUpViolation(g_msgServ, ObjectId, sPlayerName, currentLevel, sCharacterName, errorsList) <= 0)
					return -1;
			}
		}
		else if(isOwnedClass == 0)
		{
			nClassPosition = nb_OwnedClass;
			nb_OwnedClass++;
			myClasses[nClassPosition].classId = nChoosenClass;
			myClasses[nClassPosition].classLvl = 0;
		}
	}

	lvlChoosenClass++;

	// ****** Validate the choice of this Class ***** //

	//Get the "2DA" class ptr
	NWN2DA::classes2DA myClass2daRow = GetClasseRow(nChoosenClass);

	//Invalid class, just quit.
	if (myClass2daRow == NULL) {
		errorsList += "#class_notvalid:{" + std::to_string(nChoosenClass) + "}";

		return LevelUpViolation(g_msgServ, ObjectId, sPlayerName, currentLevel, sCharacterName, errorsList);
	}

	//Is it a playable class ? 
	if(myClass2daRow->m_PlayerClass != 1) {
		errorsList += "#class_notplayer:{" + std::to_string(nChoosenClass) + "}";

		if (g_msgServ->bACLvlUpStopFirstViolation)
		{
			if (LevelUpViolation(g_msgServ, ObjectId, sPlayerName, currentLevel, sCharacterName, errorsList) <= 0)
				return -1;
		}
	}

	//Is LevelMax Of class taken is already reached ?
	uint32_t maxForThisClass = myClass2daRow->m_MaxLevel;
	if(maxForThisClass != 0) {
		if(lvlChoosenClass > maxForThisClass)
		{
			//What are you trying to do ? Go beyond the max level ?
			errorsList += "#class_level_too_high:{" + std::to_string(nChoosenClass) + "," + std::to_string(lvlChoosenClass) + "," + std::to_string(maxForThisClass) + "}";

			if (g_msgServ->bACLvlUpStopFirstViolation)
			{
				if (LevelUpViolation(g_msgServ, ObjectId, sPlayerName, currentLevel, sCharacterName, errorsList) <= 0)
					return -1;
			}
		}
	}

	//Do we meat the Requirements ?

	//Alignment
	uint8_t AlignRestrict = myClass2daRow->m_AlignRestrict;
	uint8_t AlignRstrctType = myClass2daRow->m_AlignRestrictType;
	uint8_t InvertRestrict = myClass2daRow->m_InvertRestrict;

	if(AlignRestrict != 0x0 && AlignRstrctType != 0x0) {
		bool bCompareRes = (InvertRestrict != 0);

		uint8_t iGoodEvil = *(uint8_t *)(uPcBlock + 0x78);
		uint8_t iLawChaos = *(uint8_t *)(uPcBlock + 0x7A);

		bool isGood = false;
		bool isEvil = false;
		bool isGENeutral = false;
		bool isLaw = false;
		bool isChaos = false;
		bool isLCNeutral = false;

		if(iGoodEvil >= 70)
			isGood = true;
		else if(iGoodEvil <= 30)
			isEvil = true;
		else
			isGENeutral = true;

		if(iLawChaos >= 70)
			isLaw = true;
		else if(iLawChaos <= 30)
			isChaos = true;
		else
			isLCNeutral = true;

		bool bResAlign = true;
		//Law/Chaos
		bool LCAlign = true;
		bool GEAlign = true;
		if(AlignRstrctType & 0x1) {
			//Neutral
			if(AlignRestrict & 0x1) {
				LCAlign = LCAlign && (isLCNeutral == bCompareRes);
			}
			//Law
			if(AlignRestrict & 0x2) {
				LCAlign = LCAlign && (isLaw == bCompareRes);
			}
			//Chaos
			if(AlignRestrict & 0x4) {
				LCAlign = LCAlign && (isChaos == bCompareRes);
			}
		}

		//Good/Evil
		if(AlignRstrctType & 0x2) {
			//Neutral
			if(AlignRestrict & 0x1) {
				GEAlign = GEAlign && (isGENeutral == bCompareRes);
			}
			//Good
			if(AlignRestrict & 0x8) {
				GEAlign = GEAlign && (isGood == bCompareRes);
			}
			if(AlignRestrict & 0x10) {
				GEAlign = GEAlign && (isEvil == bCompareRes);
			}			
		}

		bResAlign = LCAlign && GEAlign;

		//We invert stuff. So it became an OR between the two axies
		if (InvertRestrict != 0)
			bResAlign = LCAlign || GEAlign;

		if(!bResAlign) {
			errorsList += "#align_restrict:{class:" + std::to_string(nChoosenClass) + ", GE:" + std::to_string(iGoodEvil) + ", LC : " + std::to_string(iLawChaos) + " }";

			if (g_msgServ->bACLvlUpStopFirstViolation)
			{
				if (LevelUpViolation(g_msgServ, ObjectId, sPlayerName, currentLevel, sCharacterName, errorsList) <= 0)
					return -1;
			}
		}
	}


	//Load list of feats
	uint32_t currentNbOfFeats =  *(uint32_t*)(uPcBlock + 0x1C);
	uint16_t* currentFeatsList = *(uint16_t**)(uPcBlock + 0x18);

	std::unordered_set<uint16_t> myCurrentFeats(currentFeatsList, currentFeatsList + currentNbOfFeats);


	//Call function Verif class
	if (TestIfClassOK(uPcBlock, NULL, nChoosenClass) == 0)
	{
		errorsList += "#class_prereq_error:{" + std::to_string(nChoosenClass) + " }";

		if (g_msgServ->bACLvlUpStopFirstViolation)
		{
			if (LevelUpViolation(g_msgServ, ObjectId, sPlayerName, currentLevel, sCharacterName, errorsList) <= 0)
				return -1;
		}
	}


	///////////////////////////////////////// Ability Point /////////////////////////////////////////
	//No test to do on it but save it for Prereq purpose
	int iAbilityIncreased = -1;
	if ((((currentLevel & 0xff) + 1) & 0x03) == 0) {
		iAbilityIncreased = Data[iPos];
		iPos++;
	}

	///////////////////////////////////////// HitDie(?) /////////////////////////////////////////
	// Don't seem to have any effect. Need further investigation, just skip for now
	iPos++;


	///////////////////////////////////////// Skill Points /////////////////////////////////////////

	uint32_t skillPointForMyClass = myClass2daRow->m_SkillPointBase;

	uint32_t skillBitMask = *(uint32_t*)(Data+iPos); //get the bitmask (be carrefull of indianess)
	iPos+=4;

	//MycurrentSkills
	uint8_t* currentSkillList = *(uint8_t**)(uPcBlock+0x50);
	uint16_t keepedSkillPoints = *(uint16_t*)(uPcBlock + 0x5C);

	std::vector<smallSkill> skillsCurrent;
	int8_t crossClassCost = 2;
	bool bHasSkilled = false;

	if (myCurrentFeats.count(FEAT_ABLE_LEARNER) > 0) {
		crossClassCost = 1;
	}

	if (myCurrentFeats.count(FEAT_SKILLED) > 0) {
		bHasSkilled = true;
	}

	//For each bit to 1 in the bitmask, we have a gain of the skill.
	std::vector<smallSkill> skillGained;	
	int totalSkillPointUsed = 0;


	uint8_t maxSkillId = g_global2da->m_NumberOfSkills;

	for(uint8_t i=0; i < maxSkillId; i++)
	{
		uint32_t iMasked = (i & 0x1F); //Because we have only a 32 bit "vector"

		//We change that skill
		if((skillBitMask & (1 << iMasked)) != 0)
		{
			//	Because the same bit can inform for every i%32 
			//		(ie : bit 0 can be up for Skill0, Skill32, Skill64, ...)
			if(Data[iPos] != 0)
			{
				//We really increase that skill
				smallSkill curSkill;
				curSkill.skillId = i;
				curSkill.nbPoint = Data[iPos];
				skillGained.push_back(curSkill);
			}
			iPos++;
			if ((iPos + 48) > size)
			{
				return ReallyBadMsg(g_msgServ, sPlayerName, currentLevel, sCharacterName, errorsList+"#ErrorForSkill{"+std::to_string(i)+ "}");
			}
		}

		//Create the current skills amount list
		{
			smallSkill curSkill;
			curSkill.skillId = i;
			curSkill.nbPoint = currentSkillList[i];

			uint32_t iAllClassesCanUse = GetSkillRow(i)->m_AllClassesCanUse;
			uint32_t iREMOVEDSkill = GetSkillRow(i)->m_Removed;

			//The game store the oposite of the REMOVED entry in 2DA
			//Here, we don't want to allow skills points to be added
			if(iAllClassesCanUse == 0 || iREMOVEDSkill == 0) {
				curSkill.skillCost = -1;
			} else {
				//By default, set it to crossClassCost
				curSkill.skillCost = crossClassCost;
			}
			curSkill.maxRank = (currentLevel + 4)/2;
			skillsCurrent.push_back(curSkill);
		}
	}


	//Need to check wich skill are "skill class" for maxRank (and currentSkill class for allowing/reduce cost)
	for(int iClass = 0; iClass < nb_OwnedClass; iClass++) {

		//needed for the costs for restricted skills (allowing) and if not ableLearner (reduce cost)
		bool bIsMyClass = (myClasses[iClass].classId == nChoosenClass);

		//Check if this skill is a class skill or not
		NWN2DA::classes2DA classRow = GetClasseRow(myClasses[iClass].classId);

		//Should not happens, but better sure than sorry
		if (classRow == NULL)
			continue;

		//SkillTable for this class
		uint32_t nbClassSkill = classRow->m_sizeSkillsTable;
		NWN2DA::clsSkills2DA* listClassSkill = classRow->m_clsSkillsTable;

		for(int iCS = 0; iCS < nbClassSkill; iCS++) {
			uint32_t cClassSkillID = listClassSkill[iCS].m_SkillIndex;
			uint32_t cClassSkillC = listClassSkill[iCS].m_ClassSkill;
			//It's a class Skill
			if(cClassSkillC == 1) {
				//Allow
				skillsCurrent[cClassSkillID].maxRank = (currentLevel + 4);
				if(bIsMyClass) {
					skillsCurrent[cClassSkillID].skillCost = 1;
				}
			}
		}
	}

	if ((iPos + 48) > size)
	{
		return ReallyBadMsg(g_msgServ, sPlayerName, currentLevel, sCharacterName, errorsList+"#AfterClassCheck{}");
	}

	unsigned int skillPointKept = *(uint16_t*)(Data+iPos);
	iPos+=2;

	uint32_t skillPointUsed = skillPointKept;

	//For my taken Skills :
	std::string skillPointsError = "";
	std::string skillRankToHigh = "";
	for(int i = 0; i < skillGained.size(); i++) {
		//Am i allowed ?
		uint32_t iSkillID = skillGained[i].skillId;

		//Not allowed to take that skill (restricted or removed)
		if(skillsCurrent[iSkillID].skillCost == -1) {
			skillPointsError = " " + std::to_string(iSkillID);
			if (g_msgServ->bACLvlUpStopFirstViolation) {
				std::string sTempErrorList = errorsList + "#skill_restrict:{" + skillPointsError + "}";
				if (LevelUpViolation(g_msgServ, ObjectId, sPlayerName, currentLevel, sCharacterName, sTempErrorList) <= 0)
					return -1;
			}
		} else {
			uint32_t nbPointC = skillGained[i].nbPoint;
			skillPointUsed += (skillsCurrent[iSkillID].skillCost*nbPointC);

			//Too many rank put in this skill
			if (skillsCurrent[iSkillID].maxRank < (skillsCurrent[iSkillID].nbPoint + nbPointC)) {
				skillRankToHigh = " " + std::to_string(iSkillID) + ":" + std::to_string(skillsCurrent[iSkillID].nbPoint + nbPointC);
				if (g_msgServ->bACLvlUpStopFirstViolation) {
					std::string sTempErrorList = errorsList + "#skillrank_toohigh:{" + skillRankToHigh + "}";
					if (LevelUpViolation(g_msgServ, ObjectId, sPlayerName, currentLevel, sCharacterName, sTempErrorList) <= 0)
						return -1;
				}
			}
		}
	}

	//Store the error
	if (skillPointsError != "")
	{
		errorsList += "#skill_restrict:{" + skillPointsError + "}";
	}

	if (skillRankToHigh != "")
	{
		errorsList += "#skillrank_tohigh:{" + skillRankToHigh + "}";
	}


	//Skills, we have test if we can take those skills AND if we don't put too many rank in it.
	// Now, test the total amount of pointZ
	int iIntel = CalculateAbility((void*)uPcBlock, NULL, SEPT_ANTICHEAT_INT);
	iIntel -= ComputeAbilityEffectBonuses((void*)Object, SEPT_ANTICHEAT_INT);
	if (iAbilityIncreased == SEPT_ANTICHEAT_INT)
		iIntel++;

	int iIntelBonus = 0;
	if (iIntel < 10) {
		iIntelBonus = (iIntel - 11) / 2;
	}
	else {
		iIntelBonus = (iIntel - 10) / 2;
	}

	uint16_t skillPointStored = *(uint16_t*)(uPcBlock + 0x5c);

	uint32_t skillPointAllowed = skillPointForMyClass + skillPointStored + (bHasSkilled ? 1 : 0) + iIntelBonus;

	if (skillPointAllowed < skillPointUsed) {
		errorsList += "#toomany_skillPoint:{" + std::to_string(skillPointAllowed) + "," + std::to_string(skillPointUsed) + "}";

		if (g_msgServ->bACLvlUpStopFirstViolation)
		{
			if (LevelUpViolation(g_msgServ, ObjectId, sPlayerName, currentLevel, sCharacterName, errorsList) <= 0)
				return -1;
		}
	}



	////////////////////////////////// Background (in gff, but the autolevel up stuff in fact) //////////////////////////////////
	//We don't care ? Don't seem to have any impact, check 
	iPos+=4;


	///////////////////////////////////////// Feats /////////////////////////////////////////
	uint8_t nbNewFeats = Data[iPos];
	iPos++;

	if( ((iPos+41)+nbNewFeats*2) > size)
	{
		return ReallyBadMsg(g_msgServ, sPlayerName, currentLevel, sCharacterName, errorsList+"#ErrorAfterFeat{}");
	}


	//Ok, start to count the number of given feats.
	//Check for the automatic granted feats. Add the general given feats. Add the bonus class feats
	int iNumberGeneralFeat = 0;
	int iNumberOfDomainsFeats = 0;
	if ((currentLevel+1) < 21) {
		if ((currentLevel + 1) % 3 == 0)
			iNumberGeneralFeat++;
	}
	else if ((currentLevel + 1) % 2 == 1) {
		iNumberGeneralFeat++;
	}

	//Now, check for bonus feats of our class, can be general or class specific one
	int iNumberBonusFeat = myClass2daRow->m_BonusFeat[lvlChoosenClass-1];			//lvl is index 0 in the array
	iNumberGeneralFeat += myClass2daRow->m_NormalBFeat[lvlChoosenClass - 1];

	//check the feats validity
	uint16_t* takenFeatsList = (uint16_t*)(Data + iPos);
	std::unordered_set<uint16_t> myTakenFeats(takenFeatsList, takenFeatsList + nbNewFeats);
	std::unordered_set<uint16_t> myObtainedFeats(takenFeatsList, takenFeatsList + nbNewFeats);
	iPos += (2 * nbNewFeats);


	//Check the list of feat for this class	
	NWN2DA::clsFeat2DA* myClassFeatList = myClass2daRow->m_clsFeatTable;
	uint16_t sizeMyClassFeatList = myClass2daRow->m_sizeFeatTable;

	std::unordered_set<uint16_t> myBonusFeatsOnly;
	std::unordered_set<uint16_t> myRegularOrBonusFeats;
	std::unordered_set<uint16_t> myDomainsFeats;
	std::unordered_set<uint16_t> mySelectableLvlUp;
	std::unordered_set<uint16_t> myAutoGrantedFeats;




	//Add the ranger specials Combat Style feat because... well... Obsidian suck at it
	if (lvlChoosenClass == 2 && nChoosenClass == 7) {
		//As we should get one and only one. Just remove it from taken.
		// Add it to the "autoGranted" to ensure correct Validations

		//In case we don't have CombatStyle feat
		bool bRangerFeatFound = true;
		for (int iCombatStyleFeat : g_msgServ->lRangerCombatFeats)
		{
			uint16_t nCombatStyleFeat = iCombatStyleFeat;

			//"init"
			bRangerFeatFound = false;
			//Test to remove
			if (myTakenFeats.erase(nCombatStyleFeat) != 0)
			{
				//We found it!
				bRangerFeatFound = true;
				//Add it to AutoGranted Feats 
				myAutoGrantedFeats.insert(nCombatStyleFeat);
				//One less to test
				nbNewFeats--;
				break;
			}
		}

		//Ok... In a really specific case where the combatstyle feat was already given.. Test it 
		if (!bRangerFeatFound)
		{
			for (int iCombatStyleFeat : g_msgServ->lRangerCombatFeats)
			{
				uint16_t nCombatStyleFeat = iCombatStyleFeat;

				//Test if i have it
				if (myCurrentFeats.count(nCombatStyleFeat) != 0)
				{
					//We found it!
					bRangerFeatFound = true;
					break;
				}
			}
		}

		if (!bRangerFeatFound)
		{
			errorsList += "#rangercombatstyle_missing:{}";

			if (g_msgServ->bACLvlUpStopFirstViolation)
			{
				if (LevelUpViolation(g_msgServ, ObjectId, sPlayerName, currentLevel, sCharacterName, errorsList) <= 0)
					return -1;
			}
		}
	}

	std::string sAutoFeatNotGranted = "";

	for (uint16_t i = 0; i < sizeMyClassFeatList; i++)
	{
		uint8_t fLType = myClassFeatList[i].m_List;
		int8_t	grantedOn = myClassFeatList[i].m_GrantedOnLevel;

		bool bValidFeatCond = false;

		//Allow to configure to apply PreReqGranted and GrantedOn for every kind of feats
		if (g_msgServ->bGrantedCondForEveryFeats)
		{
			if (grantedOn != -1 && grantedOn <= (int16_t)lvlChoosenClass) {
				//Does it have PreReqGranted?
				int16_t preReqGranted = myClassFeatList[i].m_GrantedPrereq;
				bValidFeatCond = (preReqGranted == -1 || myCurrentFeats.count(preReqGranted) != 0 || myAutoGrantedFeats.count(preReqGranted) != 0);
			}
		}
		else {
			bValidFeatCond = true;
		}


		if (bValidFeatCond) {
			int16_t curFeat = myClassFeatList[i].m_FeatIndex;

			//Automatic granted feat, check and remove !
			if (fLType == 0 && grantedOn == (int16_t)lvlChoosenClass) {
				int16_t preReqGranted = myClassFeatList[i].m_GrantedPrereq;

				if (preReqGranted == -1 || myCurrentFeats.count(preReqGranted) != 0 || myAutoGrantedFeats.count(preReqGranted) != 0)
				{
					//AllPrereqOk 
					if (myTakenFeats.erase(curFeat) == 0) {
						//No ? error only if we not already have it
						if (myCurrentFeats.count(curFeat) == 0 || myAutoGrantedFeats.count(curFeat)) {
							sAutoFeatNotGranted += " " + std::to_string(curFeat);

							if(g_msgServ->bACLvlUpStopFirstViolation)
								break;
						}
					}
					else {
						myAutoGrantedFeats.insert(curFeat);
						nbNewFeats--;
					}
				}
			}
			//Add to bonus feat only !
			else if (fLType == 2) {
				myBonusFeatsOnly.insert(curFeat);
			}
			//Add to regular feat or bonus feat 
			else if (fLType == 3) {
				myRegularOrBonusFeats.insert(curFeat);
			}
			//add to Selectable on lvlup
			else {
				mySelectableLvlUp.insert(curFeat);
			}
		}
	}


	if (sAutoFeatNotGranted != "") {
		errorsList += "#autofeatNotGranted:{" + sAutoFeatNotGranted + "}";

		if (g_msgServ->bACLvlUpStopFirstViolation)
		{
			if (LevelUpViolation(g_msgServ, ObjectId, sPlayerName, currentLevel, sCharacterName, errorsList) <= 0)
				return -1;
		}
	}



	//Cant test the feats right now. Need to wait for the possibility of domains.
	



	///////////////////////////////////////// Spells /////////////////////////////////////////
	// Need to have a way to know if we have a spell choice Panel. And so, spell lists
	// 
	//*10 (spell 0 to 9) : nb spells gained, spell gained list, nb spell removed, spell removed list
	//Check if we can take the spell or not. Check if the number of spell gained/removed is correct (spell removed should be for exchange stuff, should be the more difficult task)
	//Check how the fu**ing double spell class work (if both classes must choose spell at least)
	// For now... just don't check the spells... Need more investigation

	for (int i = 0; i < 10; i++) 
	{
		//NumSpellAdded lvl i
		uint8_t iNumSpell = Data[iPos];
		iPos++;

		//We gain iNumSpell
		//For now, pass them (4 bytes for each spells)
		iPos += 4 * iNumSpell;

		if ((iPos + 21 + (10 - i) * 2 - 1) > size)
		{
			return ReallyBadMsg(g_msgServ, sPlayerName, currentLevel, sCharacterName, errorsList+"#ErrorAfterSpell{lvl:"+std::to_string(i) + "}");
		}

		//NumSpellRemoved lvl i
		iNumSpell = Data[iPos];
		iPos++;

		//We remove iNumSpell
		//For now, pass them (4 bytes for each spells)
		iPos += 4 * iNumSpell;

		if ((iPos + 21 + (10 - i - 1) * 2) > size)
		{
			return ReallyBadMsg(g_msgServ, sPlayerName, currentLevel, sCharacterName, errorsList+"#ErrorAfterRemoved{lvl:"+std::to_string(i) + "}");
		}
	}

	///////////////////////////////////////// God Name /////////////////////////////////////////
	//TODO check if this god is ok with your classes (or just current classes ?)+race+(alignment), ...
	//TODO check if it's still the same. 
	//		if no :
	//			- check if the change is valid. (config  to don't allow the change ? --for example, it must be changed before?)
	//TODO, for now, don't check
	{
		uint32_t iGodNameSize = *(uint32_t*)(Data+iPos);
		iPos += 4;

		//God name now. Just skip it.
		iPos += iGodNameSize;

		if (iPos + 17 > size)
		{
			return ReallyBadMsg(g_msgServ, sPlayerName, currentLevel, sCharacterName, errorsList+"#ErrorAfterGodName{}");
		}
	}


	///////////////////////////////////////// Familiar name /////////////////////////////////////////
	// Need to understand how to know if the familiar is changed
	//If FF FF FF FF 00 00 00 00 => No familiar. Else
	// xx xx xx xx => familiar type
	// uu uu uu uu => familiar name size 
	// ttttttttttt => familiar name
	//TODO check where the "go back to void name" error came from
	{
		uint32_t iFamiliarType = *(uint32_t*)(Data + iPos);
		iPos += 4;
		uint32_t iOldFamiliarType = *(uint32_t*)(uPcBlock + 0xdf4);

		if (iFamiliarType != iOldFamiliarType)
		{
			bool bFamOk = false;


			if (iFamiliarType == 0xFFFFFFFF) {
				// In some case, a character without familiar still have an "old familiar type".
				// Check if we don't have the "Familiar" feat
				if (myCurrentFeats.count(0x12F) == 0) 
				{
					bFamOk = true;
				}
			}
			//New one ?
			else if (myCurrentFeats.count(0x12F) == 0) //Summon familiar feat. 
			{
				//New class need to be a class with familiar
				if (myClass2daRow->m_HasFamiliar != 0 && myObtainedFeats.count(0x12F) != 0) // Has familiar class and familiar feat
				{
					//TODO, familiar validity not checked yet
					bFamOk = true;
				}
			}
			//The game is...  Duh...client will popup the familiar selection as soon as you take a class that have familiar.
			//So if it's a withfamiliar class and lvl 1: it's "ok"
			else
			{
				if (myClass2daRow->m_HasFamiliar != 0 && lvlChoosenClass == 1)
				{
					//TODO, familiar validity not checked yet.
					bFamOk = true;
				}
			}


			if (!bFamOk)
			{
				errorsList += "#familiarType:{oldType:"+std::to_string(iOldFamiliarType)+" newType:"+std::to_string(iFamiliarType)+"}";

				if (g_msgServ->bACLvlUpStopFirstViolation)
				{
					if (LevelUpViolation(g_msgServ, ObjectId, sPlayerName, currentLevel, sCharacterName, errorsList) <= 0)
						return -1;
				}
			}
		}

		//get companion name and pass it (//TODO, need to analyze the name reset error.)
		uint32_t iFamiliarNameSize = *(uint32_t*)(Data + iPos);
		iPos += 4;
		iPos += iFamiliarNameSize;

		if (iPos + 9 > size)
		{
			return ReallyBadMsg(g_msgServ, sPlayerName, currentLevel, sCharacterName, errorsList+"#ErrorAfterFamiliar:{}");
		}
	}


	///////////////////////////////////////// Animal Companion name /////////////////////////////////////////
	{
		uint32_t iCompanionType = *(uint32_t*)(Data + iPos);
		iPos += 4;

		uint32_t iOldCompanionType = *(uint32_t*)(uPcBlock + 0xdf0);

		if (iCompanionType != iOldCompanionType)
		{
			bool bCompOk = false;

			
			if (iCompanionType == 0xFFFFFFFF)
			{
				// In some case, a character without animal companion still have an "old companion type".
				// Check if we don't have the "AnimalCompanion" feat
				if (myCurrentFeats.count(0xC7) == 0) 
				{
					bCompOk = true;
				}
			}
			// New one ? So no "Animal Companion" feat
			else if (myCurrentFeats.count(0xC7) == 0) 
			{
				// New class need to be a class with AnimalCompanion (and we need the feat too)
				if (myClass2daRow->m_HasAnimalCompanion != 0 && myObtainedFeats.count(0xC7) == 0) 
				{
					// TODO, familiar validity not checked yet
					bCompOk = true;
				}
			}
			//As those two feats are client side hardcoded for now, don't need to really put them as customizable. Will do latter
			else if (myObtainedFeats.count(0x878) != 0 || myObtainedFeats.count(0x7D2) != 0)
			{
				//TODO, companion validity not checked yet
				bCompOk = true;
			}


			if (!bCompOk)
			{
				errorsList += "#CompanionType:{oldType:"+std::to_string(iOldCompanionType)+" newType:"+std::to_string(iCompanionType)+"}";

				if(g_msgServ->bACLvlUpStopFirstViolation)
				{
					if (LevelUpViolation(g_msgServ, ObjectId, sPlayerName, currentLevel, sCharacterName, errorsList) <= 0)
						return -1;
				}
			}
		}

		//get companion name and pass it (//TODO, need to analyze the name reset error.)
		uint32_t iCompanionNameSize = *(uint32_t*)(Data + iPos);
		iPos += 4;
		iPos += iCompanionNameSize;

		if (iPos + 1 > size)
		{
			return ReallyBadMsg(g_msgServ, sPlayerName, currentLevel, sCharacterName, errorsList+"#ErrorAfterCompanion:{}");
		}
	}

	///////////////////////////////////////// Domains /////////////////////////////////////////
	//If domain... 
	//TODO Check if the character should have domain (before or now with the new class)
	//	If it should : Grab the 2 domains.
	//		If already have before: should be the same.(can it be legally changed ?) //TODO lets assume that no
	//		If not : check if its ok
	{		
		if (myClass2daRow->m_HasDomains)
		{
			if (iPos + 3 > size)
			{
				return ReallyBadMsg(g_msgServ, sPlayerName, currentLevel, sCharacterName, errorsList+"#DomainError:{}");
			}
			uint8_t uDomain1 = Data[iPos];
			iPos++;
			uint8_t uDomain2 = Data[iPos];
			iPos++;
			//Need to check only if not first level
			if (lvlChoosenClass > 1)
			{
				if ((uDomain1 != previousDomain1 && uDomain1 != previousDomain2) || (uDomain2 != previousDomain1 && uDomain2 != previousDomain2))
				{
					errorsList += "#DomainChanged:{}"; //TODO, fill with explaination

					if (g_msgServ->bACLvlUpStopFirstViolation)
					{
						if (LevelUpViolation(g_msgServ, ObjectId, sPlayerName, currentLevel, sCharacterName, errorsList) <= 0)
							return -1;
					}
				}
			}
			//New domains, add the feats
			else
			{
				uDomain1;
				uDomain2;
				//Domain1
				NWN2DA::domains2da myDomain = GetDomainRow(uDomain1);
				if (myDomain != NULL)
				{
					uint16_t featDomain = myDomain->m_EpithetFeat;
					if (featDomain != 0xFFFF)
					{
						if (myCurrentFeats.count(featDomain) == 0)
						{
							myDomainsFeats.insert(featDomain);
							iNumberOfDomainsFeats++;
						}
					}

					featDomain = myDomain->m_GrantedFeat;
					if (featDomain != 0xFFFF)
					{
						if (myCurrentFeats.count(featDomain) == 0)
						{
							myDomainsFeats.insert(featDomain);
							iNumberOfDomainsFeats++;
						}
					}
				}
				else {
					errorsList += "#DomainDontExist:{"+std::to_string((int)uDomain1)+ "}";

					if(g_msgServ->bACLvlUpStopFirstViolation)
					{
						if (LevelUpViolation(g_msgServ, ObjectId, sPlayerName, currentLevel, sCharacterName, errorsList) <= 0)
							return -1;
					}
				}

				myDomain = GetDomainRow(uDomain2);
				if (myDomain != NULL)
				{
					uint16_t featDomain = myDomain->m_EpithetFeat;
					if (featDomain != 0xFFFF)
					{
						if (myCurrentFeats.count(featDomain) == 0)
						{
							myDomainsFeats.insert(featDomain);
							iNumberOfDomainsFeats++;
						}
					}

					featDomain = myDomain->m_GrantedFeat;
					if (featDomain != 0xFFFF)
					{
						if (myCurrentFeats.count(featDomain) == 0)
						{
							myDomainsFeats.insert(featDomain);
							iNumberOfDomainsFeats++;
						}
					}
				}
				else {
					errorsList += "#DomainDontExist:{"+std::to_string((int)uDomain2)+ "}";

					if(g_msgServ->bACLvlUpStopFirstViolation)
					{
						if (LevelUpViolation(g_msgServ, ObjectId, sPlayerName, currentLevel, sCharacterName, errorsList) <= 0)
							return -1;
					}
				}
				//myDomainsFeats
			}

		}
	}


	//Ok, now we have checked domain. We can test the feats.
	{
		//We have removed all auto granted feat at the point, we can start to see if we don't have taken too much feats
		if (nbNewFeats > (iNumberBonusFeat + iNumberGeneralFeat + iNumberOfDomainsFeats)) {
			errorsList += "#tooManyFeats:{" + std::to_string(nbNewFeats) + ":" + std::to_string(iNumberBonusFeat + iNumberGeneralFeat) + "}";

			if(g_msgServ->bACLvlUpStopFirstViolation)
			{
				if (LevelUpViolation(g_msgServ, ObjectId, sPlayerName, currentLevel, sCharacterName, errorsList) <= 0)
					return -1;
			}
		}

		//Difficult part. Test if we can take something.
		//
		// 
		// Idea to easily test source :
		// Premise : We are sure we can take every feats. Without care about source number for now.
		// Idea:	For each feats, count if it can be taken in classFeat (A); if it can be taken in generic feat. (B)
		//			if it can be taken in both, increment both counter.
		// At the end, just be sure each counter are enough to match the feats to be taken.
		// If all feats can be both taken => no problem. A and B will be greater than the need. 
		// If some feats can only be taken by  A or B.
		// The number of the other will be reduced. So we can't take too many only A, because if we do.. B will not be high enough.
		int iBonusFeatsPossibility = 0;
		int iRegularFeatsPossibility = 0;
		int iDomainsFeatsPossibility = 0;

		for (const auto& testFeat: myTakenFeats) {
			bool bCurFeatPossible = false;
			
			if (myDomainsFeats.count(testFeat) > 0)
			{
				iDomainsFeatsPossibility++;
				bCurFeatPossible = true;
			}

			if (myRegularOrBonusFeats.count(testFeat) > 0 || myBonusFeatsOnly.count(testFeat)) {
				iBonusFeatsPossibility++;
				bCurFeatPossible = true;
			}
			if (myRegularOrBonusFeats.count(testFeat) > 0 || mySelectableLvlUp.count(testFeat) > 0 || FeatValidAllClasses(testFeat)) {
				iRegularFeatsPossibility++;
				bCurFeatPossible = true;
			}

			if (!bCurFeatPossible) {
				iRegularFeatsPossibility = -1;
				iBonusFeatsPossibility = -1;
				break;
			}
		}

		if (iRegularFeatsPossibility < iNumberGeneralFeat || iBonusFeatsPossibility < iNumberBonusFeat || iDomainsFeatsPossibility != iNumberOfDomainsFeats)
		{
			errorsList += "#featSelectionError:{}";

			if(g_msgServ->bACLvlUpStopFirstViolation)
			{
				if (LevelUpViolation(g_msgServ, ObjectId, sPlayerName, currentLevel, sCharacterName, errorsList) <= 0)
					return -1;
			}
		}

		//Now, remove domains feats from taken ones. Add it to autogranted.
		for (const auto& testDomainFeat : myDomainsFeats)
		{
			myTakenFeats.erase(testDomainFeat);
			myAutoGrantedFeats.insert(testDomainFeat);
		}

		//Ok... Here we go. Now we want to test. Soooo patch the creature.
		{
			std::unordered_set<uint16_t> myTakenFeatsCopy(myTakenFeats);

			//Patch Ability
			if (iAbilityIncreased != -1)
				*(uPcBlock + iAbilityIncreased + 4) += 1;

			//Patch classes
			if (nb_OwnedClass <= 4)
			{
				uint8_t* classBlock = uPcBlock + 0x10C;

				//New Class
				if (lvlChoosenClass == 1) {
					uPcBlock[0x14] = nb_OwnedClass;

					classBlock += 0x124 * nClassPosition;
					classBlock[4] = nChoosenClass;
					classBlock[5] = lvlChoosenClass;
				}
				else {
					classBlock += 0x124 * nClassPosition;
					classBlock[5] = lvlChoosenClass;
				}
			}

			uint32_t myRebuildFeatMaxSize = myAutoGrantedFeats.size() + currentNbOfFeats + nbNewFeats;
			uint16_t* RebuildedFeatList = new uint16_t[myRebuildFeatMaxSize];

			memcpy_s((void*)RebuildedFeatList, myRebuildFeatMaxSize*2, currentFeatsList, currentNbOfFeats*2);

			uint32_t myRebuildFeatCurSize = currentNbOfFeats;

			for(uint16_t autoFeat : myAutoGrantedFeats) {
				RebuildedFeatList[myRebuildFeatCurSize] = autoFeat;
				myRebuildFeatCurSize++;
			}

			//Patch feat list
			*(uint16_t**)(uPcBlock + 0x18) = RebuildedFeatList;

			//we loop as long as we have thing 
			while(myTakenFeatsCopy.size() > 0)
			{
				*(uint32_t*)(uPcBlock + 0x1C) = myRebuildFeatCurSize;
				std::sort(RebuildedFeatList, RebuildedFeatList + myRebuildFeatCurSize);

				std::unordered_set<uint16_t> myCurrentValidated;
				for (uint16_t featToTest : myTakenFeatsCopy) {
					if (TestIfFeatOk(uPcBlock, featToTest))
						myCurrentValidated.insert(featToTest);
				}

				if (myCurrentValidated.size() == 0)
				{
					//Just leave, we will not validate anything else
					break;
				}
				else 
				{
					for (uint16_t featToAdd : myCurrentValidated) 
					{
						if (myTakenFeatsCopy.erase(featToAdd) != 0)
						{
							RebuildedFeatList[myRebuildFeatCurSize] = featToAdd;
							myRebuildFeatCurSize++;
						}
					}
				}
			}


			//Depatch Creature

			//Depatch Feats
			*(uint32_t*)(uPcBlock + 0x1C) = currentNbOfFeats;
			*(uint16_t**)(uPcBlock + 0x18) = currentFeatsList;

			//Free Rebuild feats list
			delete[] RebuildedFeatList;
			RebuildedFeatList = nullptr;

			//Depatch Ability
			if (iAbilityIncreased != -1)
				*(uPcBlock + iAbilityIncreased + 4) -= 1;

			//Depatch classes
			if (nb_OwnedClass <= 4)
			{
				uint8_t* classBlock = uPcBlock + 0x10C;

				//New Class
				if (lvlChoosenClass == 1) {
					uPcBlock[0x14] = nb_OwnedClass - 1;

					classBlock += 0x124 * nClassPosition;
					classBlock[4] = 0xFF;
					classBlock[5] = 0;
				}
				else {
					classBlock += 0x124 * nClassPosition;
					classBlock[5] = lvlChoosenClass - 1;
				}
			}

			//Now, test if every feat was testedOk !
			if (myTakenFeatsCopy.size() > 0)
			{
				std::string listOfNotValidFeat = "";
				for (const auto& elem : myTakenFeatsCopy)
				{
					listOfNotValidFeat += " " + std::to_string(elem);
				}

				//We still have "TakenFeats" So.. some are not validated, send error
				errorsList += "#featPrereqNotValid:{" + listOfNotValidFeat +  "}";

				if(g_msgServ->bACLvlUpStopFirstViolation)
				{
					if (LevelUpViolation(g_msgServ, ObjectId, sPlayerName, currentLevel, sCharacterName, errorsList) <= 0)
						return -1;
				}
			}
		}
	}

	///////////////////////////////////////// Spell School /////////////////////////////////////////
	//If must have a spell school =>
	//	If it should, grab the school
	//		If already have before : should be the same (can it be legally changed ?) //TODO, 
	//		If not : check if it's ok	
	{
		if (myClass2daRow->m_HasSchool)
		{
			if (iPos + 2 > size)
			{
				return ReallyBadMsg(g_msgServ, sPlayerName, currentLevel, sCharacterName, errorsList+"#ErrorSpellSchool{}");
			}

			uint8_t uSchool = Data[iPos];
			iPos++;

			if (lvlChoosenClass > 1)
			{
				if (uSchool != previousSchool)
				{
					errorsList += "#SchoolChanged:{}"; //TODO, fill with explaination

					if(g_msgServ->bACLvlUpStopFirstViolation)
					{
						if (LevelUpViolation(g_msgServ, ObjectId, sPlayerName, currentLevel, sCharacterName, errorsList) <= 0)
							return -1;
					}
				}
			}
		}
	}

	//End with a 0x70
	if (Data[iPos] != 0x70 || (iPos+1) != size)
	{
		return ReallyBadMsg(g_msgServ, sPlayerName, currentLevel, sCharacterName, errorsList+"#badMessageEnd{}");
	}

	//No error. That's a good player. Continue.
	if(errorsList == "")
	{
		logger->Debug("[%s] %s : level %d ok.", sPlayerName.c_str(), sCharacterName.c_str(), currentLevel+1);
		return 1;
	}

	if (LevelUpViolation(g_msgServ, ObjectId, sPlayerName, currentLevel, sCharacterName, errorsList) <= 0)
			return -1;
	return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////// Start Of Creation AntiCheats ///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define OFFS_CharacterCreationVerif		0x0058E4AB
#define OFFS_CharacterCreationErrorCode	0x0058E328

unsigned long InitEngineString					= 0x004354D0;
unsigned long AdvancedCharacterCheckError		= 0x0058E29f;
unsigned long ReturnToCharacterCheck			= 0x0058E4B0;
unsigned long ContinueCharacterCreationError	= 0x0058E32D;

int g_iCharCreationReturnCode = 0x23d9;
std::string g_ScriptCreationError = "";
///////////////////////

#define FUNC_GetGFFByte			0x00794930
#define FUNC_GetGFFChar			0x007949A0
#define FUNC_GetGFFWord			0x00794A10
#define FUNC_GetGFFShort		0x00794A80
#define FUNC_GetGFFDWord		0x00794AF0
#define FUNC_GetGFFInt			0x00794B60
#define FUNC_GetGFFFloat		0x00794BD0
#define FUNC_GetGFFStructByName	0x00794850
#define FUNC_GetGFFStruct		0x007947d0
#define FUNC_GetGFFList			0x00794700
#define FUNC_GetGFFListLength	0x00794780

__declspec(naked) uint8_t __fastcall GetGFFByte(void* puVar,  void* Unused, void* gffPtr, const char* cField, void* aiStack110, uint8_t iDefault)
{
	__asm
	{
		mov		edx, FUNC_GetGFFByte;
		jmp		edx;
	}
}

__declspec(naked) uint8_t __fastcall GetGFFChar(void* puVar,  void* Unused, void* gffPtr, const char* cField, void* aiStack110, uint8_t iDefault)
{
	__asm
	{
		mov		edx, FUNC_GetGFFChar;
		jmp		edx;
	}
}

__declspec(naked) uint16_t __fastcall GetGFFWord(void* puVar,  void* Unused, void* gffPtr, const char* cField, void* aiStack110, uint16_t iDefault)
{
	__asm
	{
		mov		edx, FUNC_GetGFFWord;
		jmp		edx;
	}
}

__declspec(naked) uint16_t __fastcall GetGFFShort(void* puVar,  void* Unused, void* gffPtr, const char* cField, void* aiStack110, uint16_t iDefault)
{
	__asm
	{
		mov		edx, FUNC_GetGFFShort;
		jmp		edx;
	}
}

__declspec(naked) uint32_t __fastcall GetGFFDWord(void* puVar,  void* Unused, void* gffPtr, const char* cField, void* aiStack110, uint32_t iDefault)
{
	__asm
	{
		mov		edx, FUNC_GetGFFDWord;
		jmp		edx;
	}
}

__declspec(naked) uint32_t __fastcall GetGFFInt(void* puVar,  void* Unused, void* gffPtr, const char* cField, void* aiStack110, uint32_t iDefault)
{
	__asm
	{
		mov		edx, FUNC_GetGFFInt;
		jmp		edx;
	}
}

__declspec(naked) float __fastcall GetGFFFloat(void* puVar, void* Unused, void* gffPtr, const char* cField, void* aiStack110, float fDefault)
{
	__asm
	{
		mov		edx, FUNC_GetGFFFloat;
		jmp		edx;
	}
}

__declspec(naked) int __fastcall GetGFFStructByName(void* puVar, void* Unused, void* newPtr, void* gffPtr, const char* cName)
{
	__asm
	{
		mov		edx, FUNC_GetGFFStructByName;
		jmp		edx;
	}
}

__declspec(naked) int __fastcall GetGFFStruct(void* puVar, void* Unused, void* newPtr, void* gffPtr, int idx)
{
	__asm
	{
		mov		edx, FUNC_GetGFFStruct;
		jmp		edx;
	}
}

__declspec(naked) int __fastcall GetGFFList(void* puVar, void* Unused, void* newPtr, void* gffPtr, const char* cName)
{
	__asm
	{
		mov		edx, FUNC_GetGFFList;
		jmp		edx;
	}
}

__declspec(naked) int __fastcall GetGFFListLength(void* puVar, void* Unused, void* gffPtr)
{
	__asm
	{
		mov		edx, FUNC_GetGFFListLength;
		jmp		edx;
	}
}

///////////////////////

#define FUNC_Load2DATable				0x0051acb0
#define FUNC_Load2DATablePart2			0x0051b4d0
#define FUNC_2daColumnNameToRowOffset	0x0051a290
#define FUNC_2DAGetString				0x0051a2e0
#define FUNC_2DAGetInt					0x0051a5f0

#define FUNC_GetClientName				0x00571380

__declspec(naked) void __fastcall Load2DATable_intern(void* p2DATable,  void* Unused, char* lowerTableName)
{
	__asm
	{
		mov		edx, FUNC_Load2DATable;
		jmp		edx;
	}
}

__declspec(naked) void __fastcall Load2DATableP2_intern(void* p2DATable)
{
	__asm
	{
		mov		edx, FUNC_Load2DATablePart2;
		jmp		edx;
	}
}

__declspec(naked) int __fastcall Get2DAColumnOffset(uint32_t* p2DATable,  void* Unused, const char* columnName)
{
	__asm
	{
		mov		edx, FUNC_2daColumnNameToRowOffset;
		jmp		edx;
	}
}

__declspec(naked) int __fastcall Get2DAString(uint32_t* p2DATable,  void* Unused, int iRow, int iColumnOffset, NWN::CExoString* result)
{
	__asm
	{
		mov		edx, FUNC_2DAGetString;
		jmp		edx;
	}
}

__declspec(naked) int __fastcall Get2DAInt(uint32_t* p2DATable,  void* Unused, int iRow, int iColumnOffset, int* result)
{
	__asm
	{
		mov		edx, FUNC_2DAGetInt;
		jmp		edx;
	}
}


uint32_t Load2DATable(uint32_t* p2DATable, NWN::CExoString* cTable)
{
	char lowerTableName[32];

	for (int i = 0; i < 32 && i < cTable->m_nBufferLength; i++)
	{
		lowerTableName[i] = (char)tolower(cTable->m_sString[i]);
	}


	Load2DATable_intern(p2DATable, NULL, lowerTableName);
	Load2DATableP2_intern(p2DATable);

	return p2DATable[16];
}

__declspec(naked) NWN::CExoString* __fastcall GetClientName(void* pContext)
{
	__asm
	{
		mov		edx, FUNC_GetClientName;
		jmp		edx;
	}
}


//Septi, time to be smart. Dont do a too complicated function for this please
bool TestIfFeatOk_CharCreation(uint16_t featToTest, NWN2DA::classes2DA myClass,
	const std::unordered_set<uint16_t>& myCurrentFeats,std::vector<smallSkill>& skillsTaken, 
	uint8_t iSubRaceRow, uint32_t iStr, uint32_t iDex, uint32_t iCon, uint32_t iInt, uint32_t iWis, uint32_t iCha, int iGE, int iLC)
{
	NWN2DA::feats2DA myFeatToTest = GetFeatRow(featToTest);

	if (myFeatToTest == NULL)
		return false;

	if (myFeatToTest->m_featRemoved == 1 || myFeatToTest->m_featValid != 1)
	{
		return false;
	}

	//History feat can't be taken (or only with the background and its already done)
	if (myFeatToTest->m_FeatCategory == 8)
	{
		return false;
	}

	if (myFeatToTest->m_PreReqEpic != 0)
	{
		return false;
	}

	if (iStr < myFeatToTest->m_MinStr || (myFeatToTest->m_MaxStr != 0 && iStr > myFeatToTest->m_MaxStr) ||
		iDex < myFeatToTest->m_MinDex || (myFeatToTest->m_MaxDex != 0 && iDex > myFeatToTest->m_MaxDex) ||
		iCon < myFeatToTest->m_MinCon || (myFeatToTest->m_MaxCon != 0 && iCon > myFeatToTest->m_MaxCon) ||
		iInt < myFeatToTest->m_MinInt || (myFeatToTest->m_MaxInt != 0 && iInt > myFeatToTest->m_MaxInt) ||
		iWis < myFeatToTest->m_MinWis || (myFeatToTest->m_MaxWis != 0 && iWis > myFeatToTest->m_MaxWis) ||
		iCha < myFeatToTest->m_MinCha || (myFeatToTest->m_MaxCha != 0 && iCha > myFeatToTest->m_MaxCha))
	{
		return false;
	}

	//PrereqFeat
	{
		uint16_t iPreReqFeat1 = myFeatToTest->m_PrereqFeat1;
		if (iPreReqFeat1 != 0xFFFF)
		{
			if (myCurrentFeats.count(iPreReqFeat1) == 0)
			{
				return false;
			}
		}

		uint16_t iPreReqFeat2 = myFeatToTest->m_PrereqFeat2;
		if (iPreReqFeat2 != 0xFFFF)
		{
			if (myCurrentFeats.count(iPreReqFeat2) == 0)
			{
				return false;
			}
		}
	}

	//PrereqOr
	{
		uint16_t iPreReqOrFeat = myFeatToTest->m_OrReqFeat0;
		bool preReqOr = (iPreReqOrFeat == 0xFFFF);

		preReqOr |= (myCurrentFeats.count(iPreReqOrFeat) > 0);

		iPreReqOrFeat = myFeatToTest->m_OrReqFeat1;
		preReqOr |= (iPreReqOrFeat != 0xFFFF && (myCurrentFeats.count(iPreReqOrFeat)));

		iPreReqOrFeat = myFeatToTest->m_OrReqFeat2;
		preReqOr |= (iPreReqOrFeat != 0xFFFF && (myCurrentFeats.count(iPreReqOrFeat)));

		iPreReqOrFeat = myFeatToTest->m_OrReqFeat3;
		preReqOr |= (iPreReqOrFeat != 0xFFFF && (myCurrentFeats.count(iPreReqOrFeat)));

		iPreReqOrFeat = myFeatToTest->m_OrReqFeat4;
		preReqOr |= (iPreReqOrFeat != 0xFFFF && (myCurrentFeats.count(iPreReqOrFeat)));

		iPreReqOrFeat = myFeatToTest->m_OrReqFeat5;
		preReqOr |= (iPreReqOrFeat != 0xFFFF && (myCurrentFeats.count(iPreReqOrFeat)));

		if (!preReqOr)
			return false;
	}

	//Check if it is really that 
	if (myClass->m_AttackBonus[0] < myFeatToTest->m_MinAttackBonus)
	{
		return false;
	}

	//ReqSkill1
	uint16_t iSkill = myFeatToTest->m_ReqSkill;
	if (iSkill != 0xFFFF)
	{
		if (iSkill < skillsTaken.size())
		{
			uint16_t mySkillRank = skillsTaken[iSkill].nbPoint;
			if (mySkillRank < myFeatToTest->m_ReqSkillMinRanks ||
				(myFeatToTest->m_ReqSkillMaxRanks != 0 && mySkillRank > myFeatToTest->m_ReqSkillMaxRanks))
			{
				return false;
			}
		}
	}

	//ReqSkill2
	iSkill = myFeatToTest->m_ReqSkill2;
	if (iSkill != 0xFFFF)
	{
		if (iSkill < skillsTaken.size())
		{
			uint16_t mySkillRank = skillsTaken[iSkill].nbPoint;
			if (mySkillRank < myFeatToTest->m_ReqSkillMinRanks2 ||
				(myFeatToTest->m_ReqSkillMaxRanks2 != 0 && mySkillRank > myFeatToTest->m_ReqSkillMaxRanks2))
			{
				return false;
			}
		}
	}

	//MinSpellLvl 
	uint8_t iSpellLvl = myFeatToTest->m_MinSpellLvl;
	if (iSpellLvl != 0 && iSpellLvl != 0xFF)
	{
		if (myClass->m_SpellCaster == 0 ||  myClass->m_NumSpellLevels[0] < iSpellLvl)
		{
			return false;
		}
	}
	
	//MinSpellCasterLvl
	iSpellLvl = myFeatToTest->m_MinCasterLvl;
	if (iSpellLvl != 0 && iSpellLvl != 0xFF)
	{
		uint8_t iCasterLvl = 0;
		if (myClass->m_SpellCaster != 0)
		{
			//Already Hardcoded in server, keep it like this for now
			if (myClass->m_ClassRow != 6 && myClass->m_ClassRow != 7)
			{
				iCasterLvl = 1;
			}
		}

		if (iCasterLvl < iSpellLvl)
			return false;
	}

	//MinFort
	uint8_t iFort = myFeatToTest->m_MinFortSave;
	if (myClass->m_FortSave[0] < iFort)
	{
		return false;
	}

	//AlignRestrict 
	uint8_t iAlignRest = myFeatToTest->m_AlignRestrict;
	if (1 < iAlignRest && iAlignRest != 0xFF)
	{
		uint8_t iGETest = 0;
		uint8_t iLCTest = 0;
		//100 - 70 Good, 69 - 31 Neutral,  30 - 0 Evil
		//2 = Non-Law, 3 = Non-Chaos, 4 = Non-Good, 5 = Non-Evil.
		if (iGE < 31)
			iGETest = 5;
		if (iGE >= 70)
			iGETest = 4;

		if (iLC < 31)
			iLCTest = 3;
		if (iLC >= 70)
			iLCTest = 2;

		if (iGETest == iAlignRest)
			return false;

		if (iLCTest == iAlignRest)
			return false;
	}

	//LevelRestrict
	{
		uint8_t uLevel = myFeatToTest->m_MinLevel;
		if (uLevel != 0)
		{
			if (uLevel > 1)
				return false;

			uint8_t uLvlClass = myFeatToTest->m_MinLevelClass;

			if (uLvlClass != 0xFF && uLvlClass != myClass->m_ClassRow)
				return false;
		}

		//No need to test maxLvl
	}

	return true;
}

//uint32_t local_6c[20];
//char	 local_20[32];

///////////////////////

bool CharacterCreationError(std::string sAccountName, std::string sErrorList)
{
	bool bError = true;
	std::string sBaseForError = "CharacterCreationError(" + sAccountName + "):";
	std::string sErrMessage = sBaseForError + sErrorList;
	logger->Err(sErrMessage.c_str());

	if (g_ScriptCreationError != "")
	{
		bool bScriptExec = true;
		NWScript::ClearScriptParams();
		NWScript::AddScriptParameterString(sAccountName.c_str());
		NWScript::AddScriptParameterString(sErrorList.c_str());
		int iRet = NWScript::ExecuteScriptEnhanced(g_ScriptCreationError.c_str(), 0, TRUE, &bScriptExec);
		if (bScriptExec && iRet == 1)
		{
			bError = false;
		}
	}

	if (bError)
	{
		g_iCharCreationReturnCode = 0x384B7;
	}
	//std::string sMessage
	//g_AnticheatLogger->Warn(sMessage.c_str());
	return bError;
}

//571380

int __fastcall AdvancedCharacterCreationCheck(void* puVar, void* gffPtr, void* pContext, int* aiStack110)
{
	if (!g_AnticheatActivated)
		return 0;
	NWN::CExoString* accountName = GetClientName(pContext);
	std::string sAccountName = ExoStringToString(accountName);
	std::string sErrorList = "";
	std::string sBaseForError = "CharacterCreationError(" + sAccountName + "):";

	uint8_t iSubRaceRow = GetGFFByte(puVar, NULL, gffPtr, "Subrace", aiStack110, 0);
	uint8_t iRaceRow = GetGFFByte(puVar, NULL, gffPtr, "Race", aiStack110, 0);
	int iClass;
	NWN2DA::classes2DA myClass;

	NWN2DA::racialSubtypes2DA mySubType = GetSubRacialRow(iSubRaceRow);

	if (mySubType->m_PlayerRace == 0 || mySubType->m_BaseRace != iRaceRow)
	{
		//"BadRace{"+ std::to_string(iRaceRow) + "/" + std::to_string(iSubRaceRow)+ "}"
		std::string sError = "#BadRace:{" + std::to_string(iRaceRow) + " ," + std::to_string(iSubRaceRow) + "}";
		sErrorList += sError;
		if (gMsgServerStopFirstCreation && CharacterCreationError(sAccountName, sErrorList))
		{
			return 1;
		}
	}

	int iStr = GetGFFByte(puVar, NULL, gffPtr, "Str", aiStack110, 0);
	int iDex = GetGFFByte(puVar, NULL, gffPtr, "Dex", aiStack110, 0);
	int iCon = GetGFFByte(puVar, NULL, gffPtr, "Con", aiStack110, 0);
	int iInt = GetGFFByte(puVar, NULL, gffPtr, "Int", aiStack110, 0);
	int iWis = GetGFFByte(puVar, NULL, gffPtr, "Wis", aiStack110, 0);
	int iCha = GetGFFByte(puVar, NULL, gffPtr, "Cha", aiStack110, 0);
	int iGE = GetGFFByte(puVar, NULL, gffPtr, "GoodEvil", aiStack110, 0);
	int iLC = GetGFFByte(puVar, NULL, gffPtr, "LawfulChaotic", aiStack110, 0);
	uint8_t iGender = GetGFFByte(puVar, NULL, gffPtr, "Gender", aiStack110, 0);
	int uDomain1 = 0;
	int uDomain2 = 0;
	int uSchool = 0;

	int iAbilityPoints = 0;

	//Test Ability
	{
		if ((iStr < 8 || iDex < 8 || iCon < 8 || iInt < 8 || iWis < 8 || iCha < 8) ||
			(iStr > 18 || iDex > 18 || iCon > 18 || iInt > 18 || iWis > 18 || iCha > 18))
		{
			std::string sError = "#AbilityError:{"+ std::to_string(iStr) + "," + std::to_string(iDex)+","+std::to_string(iCon)+
				","+std::to_string(iInt)+","+std::to_string(iWis)+","+std::to_string(iCha)+ "}";
			sErrorList += sError;
			if (gMsgServerStopFirstCreation && CharacterCreationError(sAccountName, sErrorList))
			{
				return 1;
			}
		}

		if (iStr > 8)
		{
			iAbilityPoints = iStr - 8;
			if (iStr > 14)
			{
				iAbilityPoints += iStr - 14;
				if (iStr > 16)
				{
					iAbilityPoints += iStr - 16;
				}
			}
		}

		if (iDex > 8)
		{
			iAbilityPoints += iDex - 8;
			if (iDex > 14)
			{
				iAbilityPoints += iDex - 14;
				if (iDex > 16)
				{
					iAbilityPoints += iDex - 16;
				}
			}
		}

		if (iCon > 8)
		{
			iAbilityPoints += iCon - 8;
			if (iCon > 14)
			{
				iAbilityPoints += iCon - 14;
				if (iCon > 16)
				{
					iAbilityPoints += iCon - 16;
				}
			}
		}

		if (iInt > 8)
		{
			iAbilityPoints += iInt - 8;
			if (iInt > 14)
			{
				iAbilityPoints += iInt - 14;
				if (iInt > 16)
				{
					iAbilityPoints += iInt - 16;
				}
			}
		}

		if (iWis > 8)
		{
			iAbilityPoints += iWis - 8;
			if (iWis > 14)
			{
				iAbilityPoints += iWis - 14;
				if (iWis > 16)
				{
					iAbilityPoints += iWis - 16;
				}
			}
		}

		if (iCha > 8)
		{
			iAbilityPoints += iCha - 8;
			if (iCha > 14)
			{
				iAbilityPoints += iCha - 14;
				if (iCha > 16)
				{
					iAbilityPoints += iCha - 16;
				}
			}
		}

		if (iAbilityPoints != 32)
		{
			std::string sError = "#BadAmountAbilityPoints:{"+ std::to_string(iStr) + "," + std::to_string(iDex)+","+std::to_string(iCon)+
				","+std::to_string(iInt)+","+std::to_string(iWis)+","+std::to_string(iCha)+":"+std::to_string(iAbilityPoints) + "}";
			sErrorList += sError;
			if (gMsgServerStopFirstCreation && CharacterCreationError(sAccountName, sErrorList))
			{
				return 1;
			}
		}

		iStr += mySubType->m_StrAdjust;
		iDex += mySubType->m_DexAdjust;
		iCon += mySubType->m_ConAdjust;
		iInt += mySubType->m_IntAdjust;
		iWis += mySubType->m_WisAdjust;
		iCha += mySubType->m_ChaAdjust;
	}




	//Test Align
	{
		if (!(iGE == 15 || iGE == 50 || iGE == 85) || !(iLC == 15 || iLC == 50 || iLC == 85))
		{
			std::string sError = "#AlignError:{"+ std::to_string(iGE) + " , " + std::to_string(iLC)+ "}";
			sErrorList += sError;
			if (gMsgServerStopFirstCreation && CharacterCreationError(sAccountName, sErrorList))
			{
				return 1;
			}
		}
	}

	//Check class
	{

		bool bPreReqErrorFound = false;
		void* newPtr[5];
		void* newPtr2[5];

		int iTempRes = GetGFFList(puVar, NULL, newPtr, gffPtr, "ClassList");
		iTempRes = GetGFFListLength(puVar, NULL, newPtr);
		iTempRes = GetGFFStruct(puVar, NULL, newPtr2, newPtr, 0);

		iClass = GetGFFInt(puVar, NULL, newPtr2, "Class", aiStack110, 0);

		myClass = GetClasseRow(iClass);

		//Need to view the domains and count the feats in it.
		if (myClass->m_HasDomains != 0)
		{
			uDomain1 = GetGFFByte(puVar, NULL, newPtr2, "Domain1", aiStack110, 0);
			uDomain2 = GetGFFByte(puVar, NULL, newPtr2, "Domain2", aiStack110, 0);
		}
		if (myClass->m_HasSchool != 0)
		{
			uSchool = GetGFFByte(puVar, NULL, newPtr2, "School", aiStack110, 0);
		}
		//Test PreReqTable
		{
			uint32_t pTable[20];
			int iRowNumber = Load2DATable(pTable, &(myClass->m_PreReqTable));

			int iReqTypeOffset = Get2DAColumnOffset(pTable, NULL, "ReqType");
			int iReqParam1Offset = Get2DAColumnOffset(pTable, NULL, "ReqParam1");
			int iReqParam2Offset = Get2DAColumnOffset(pTable, NULL, "ReqParam2");

			bool bArcOrDiv = false;
			bool bArcOrDivOk = false;

			for (int i = 0; i < iRowNumber; i++)
			{
				NWN::CExoString restrictType;
				int iRes = Get2DAString(pTable, NULL, i, iReqTypeOffset, &restrictType);
				if (iRes != 0)
				{
					//Need to test
					std::string sRestrictType = ExoStringToString(&restrictType);

					//During creation, you don't have any feat, spells, magic school, or class.
					if (sRestrictType == "FEAT" || sRestrictType == "FEATOR" || sRestrictType == "FEATOR2" ||
						sRestrictType == "CLASSOR" || sRestrictType == "SPELLOR" || sRestrictType == "SPECIALIST")
					{
						//For now, without more details, no real need to call it more than one time
						if (!bPreReqErrorFound) 
						{
							bPreReqErrorFound = true;
							std::string sError = "#ClassPrereqError:{"+ std::to_string(iClass)+"}";
							sErrorList += sError;
							if (gMsgServerStopFirstCreation && CharacterCreationError(sAccountName, sErrorList))
							{
								return 1;
							}
							
							//Already prereq error. No need to search for more.
							break;
						}
					}

					//Just in case... If it's set to 0, no problem
					if (sRestrictType == "BAB" || sRestrictType == "ARCSPELL" || sRestrictType == "DIVSPELL" ||
						sRestrictType == "SPELL" || sRestrictType == "SAD")
					{
						int iValue = 0;
						iRes = Get2DAInt(pTable, NULL, i, iReqParam1Offset, &iValue);

						if (iRes == 0 || iValue > 0)
						{
							//For now, without more details, no real need to call it more than one time
							if (!bPreReqErrorFound)
							{
								bPreReqErrorFound = true;
								std::string sError = "#ClassPrereqError:{" + std::to_string(iClass) + "}";
								sErrorList += sError;
								if (gMsgServerStopFirstCreation && CharacterCreationError(sAccountName, sErrorList))
								{
									return 1;
								}

								//Already prereq error. No need to search for more.
								break;
							}
						}
					}

					//Just in case, if it's set to 0, no problem
					if (sRestrictType == "SKILL" || sRestrictType == "SAVE")
					{
						int iValue = 0;
						iRes = Get2DAInt(pTable, NULL, i, iReqParam2Offset, &iValue);

						if (iRes == 0 || iValue > 0)
						{
							//For now, without more details, no real need to call it more than one time
							if (!bPreReqErrorFound)
							{
								bPreReqErrorFound = true;
								std::string sError = "#ClassPrereqError:{" + std::to_string(iClass) + "}";
								sErrorList += sError;
								if (gMsgServerStopFirstCreation && CharacterCreationError(sAccountName, sErrorList))
								{
									return 1;
								}

								//Already prereq error. No need to search for more.
								break;
							}
						}
					}


					//Tricky here... Will be ok only if arcspellor or divspellor is 0.
					if (sRestrictType == "ARCSPELLOR" || sRestrictType == "DIVSPELLOR")
					{
						int iValue = 0;
						//We have a arc or div test
						bArcOrDiv = true;
						iRes = Get2DAInt(pTable, NULL, i, iReqParam1Offset, &iValue);

						//It's ok, one of them is set to 0
						if (iRes != 0 && iValue == 0)
						{
							bArcOrDivOk = true;
						}
					}


					//Need to be really tested
					if (sRestrictType == "RACE")
					{
						int iValue = 0;
						iRes = Get2DAInt(pTable, NULL, i, iReqParam1Offset, &iValue);

						if (iValue != iRaceRow)
						{
							//For now, without more details, no real need to call it more than one time
							if (!bPreReqErrorFound)
							{
								bPreReqErrorFound = true;
								std::string sError = "#ClassPrereqError:{" + std::to_string(iClass) + "}";
								sErrorList += sError;
								if (gMsgServerStopFirstCreation && CharacterCreationError(sAccountName, sErrorList))
								{
									return 1;
								}

								//Already prereq error. No need to search for more.
								break;
							}
						}
					}

					//Need to be really tested
					if (sRestrictType == "SUBRACE")
					{
						int iValue = 0;
						iRes = Get2DAInt(pTable, NULL, i, iReqParam1Offset, &iValue);

						if (iValue != iSubRaceRow)
						{
							//For now, without more details, no real need to call it more than one time
							if (!bPreReqErrorFound)
							{
								bPreReqErrorFound = true;
								std::string sError = "#ClassPrereqError:{" + std::to_string(iClass) + "};";
								sErrorList += sError;
								if (gMsgServerStopFirstCreation && CharacterCreationError(sAccountName, sErrorList))
								{
									return 1;
								}

								//Already prereq error. No need to search for more.
								break;
							}
						}
					}
				}
			}
			if (bArcOrDiv && !bArcOrDivOk)
			{
				//For now, without more details, no real need to call it more than one time
				if (!bPreReqErrorFound)
				{
					bPreReqErrorFound = true;
					std::string sError = "#ClassPrereqError:{" + std::to_string(iClass) + "}";
					sErrorList += sError;
					if (gMsgServerStopFirstCreation && CharacterCreationError(sAccountName, sErrorList))
					{
						return 1;
					}
				}
			}
		}

		//Test Align restrict
		{
			uint8_t AlignRestrict = myClass->m_AlignRestrict;
			uint8_t AlignRstrctType = myClass->m_AlignRestrictType;
			uint8_t InvertRestrict = myClass->m_InvertRestrict;

			if (AlignRestrict != 0x0 && AlignRstrctType != 0x0)
			{
				bool bCompareRes = (InvertRestrict != 0);
				bool isGood = false;
				bool isEvil = false;
				bool isGENeutral = false;
				bool isLaw = false;
				bool isChaos = false;
				bool isLCNeutral = false;

				if (iGE == 85)
				{
					isGood = true;
				}
				else if (iGE == 15)
				{
					isEvil = true;
				}
				else
				{
					isGENeutral = true;
				}

				if (iLC == 85)
				{
					isLaw = true;
				}
				else if (iLC == 15)
				{
					isChaos = true;
				}
				else
				{
					isLCNeutral = true;
				}

				bool bResAlign = true;

				bool LCAlign = true;
				bool GEAlign = true;
				//Law/Chaos
				if (AlignRstrctType & 0x1)
				{
					//Neutral
					if (AlignRestrict & 0x1)
					{
						LCAlign = LCAlign && (isLCNeutral == bCompareRes);
					}
					//Law
					if (AlignRestrict & 0x2)
					{
						LCAlign = LCAlign && (isLaw == bCompareRes);
					}
					//Chaos
					if (AlignRestrict & 0x4)
					{
						LCAlign = LCAlign && (isChaos == bCompareRes);
					}
				}

				//Good/Evil
				if (AlignRstrctType & 0x2)
				{
					//Neutral
					if (AlignRestrict & 0x1)
					{
						GEAlign = GEAlign && (isGENeutral == bCompareRes);
					}
					//Good
					if (AlignRestrict & 0x8)
					{
						GEAlign = GEAlign && (isGood == bCompareRes);
					}
					if (AlignRestrict & 0x10)
					{
						GEAlign = GEAlign && (isEvil == bCompareRes);
					}
				}

				bResAlign = (LCAlign && GEAlign);

				//We invert stuff. So it became an OR between the two axies
				if (InvertRestrict != 0)
					bResAlign = (LCAlign || GEAlign);

				if (!bResAlign)
				{
					//For now, without more details, no real need to call it more than one time
					if (!bPreReqErrorFound)
					{
						bPreReqErrorFound = true;
						std::string sError = "#ClassPrereqError:{" + std::to_string(iClass) + "}";
						sErrorList += sError;
						if (gMsgServerStopFirstCreation && CharacterCreationError(sAccountName, sErrorList))
						{
							return 1;
						}
					}
				}
			}
		}
	}

	//Prepare the race feats
	std::unordered_set<uint16_t> myCurrentFeats;
	std::unordered_set<uint16_t> myDomainsFeats;
	{
		//Load list of feats
		uint32_t currentNbOfFeats =  mySubType->m_NumberRacialFeats;
		uint16_t* currentFeatsList = mySubType->m_RacialFeatsList;

		myCurrentFeats.insert(currentFeatsList, currentFeatsList + currentNbOfFeats);
	}

	//Prepare the DomainsFeats
	{
		//Create the Domain list
		if (myClass->m_HasDomains != 0)
		{
			//Domain1
			NWN2DA::domains2da myDomain = GetDomainRow(uDomain1);
			uint16_t featDomain = myDomain->m_EpithetFeat;
			if (featDomain != 0xFFFF)
			{
				myDomainsFeats.insert(featDomain);
			}

			featDomain = myDomain->m_GrantedFeat;
			if (featDomain != 0xFFFF)
			{
				myDomainsFeats.insert(featDomain);
			}

			//Domain2
			myDomain = GetDomainRow(uDomain2);
			featDomain = myDomain->m_EpithetFeat;
			if (featDomain != 0xFFFF)
			{
				myDomainsFeats.insert(featDomain);
			}

			featDomain = myDomain->m_GrantedFeat;
			if (featDomain != 0xFFFF)
			{
				myDomainsFeats.insert(featDomain);
			}
		}
	}

	//Test skillspoints
	std::vector<smallSkill> skillsTaken;
	{
		int iIntBonus = 0;

		if (iInt < 10)
		{
			iIntBonus = (iInt - 11) / 2;
		}
		else 
		{
			iIntBonus = (iInt - 10) / 2;
		}

		int iNumberOfSkillPoints = myClass->m_SkillPointBase + iIntBonus;
		iNumberOfSkillPoints *= 4;

		int8_t crossClassCost = 2;
		bool bHasSkilled = false;

		if (myCurrentFeats.count(FEAT_ABLE_LEARNER) > 0)
		{
			crossClassCost = 1;
		}

		if (myCurrentFeats.count(FEAT_SKILLED) > 0)
		{
			iNumberOfSkillPoints += 4;
		}

		int totalSkillPointUsed = 0;

		uint8_t maxSkillId = g_global2da->m_NumberOfSkills;


		//Treat everything as cross class skill for now.
		for (uint8_t i = 0; i < maxSkillId; i++)
		{
			smallSkill curSkill;

			curSkill.skillId = i;

			uint32_t iAllClassesCanUse = GetSkillRow(i)->m_AllClassesCanUse;
			uint32_t iREMOVEDSkill = GetSkillRow(i)->m_Removed;

			//The game store the oposite of the REMOVED entry in 2DA
			//Here, we don't want to allow skills points to be added
			if (iAllClassesCanUse == 0 || iREMOVEDSkill == 0)
			{
				curSkill.skillCost = -1;
			}
			else {
				curSkill.skillCost = crossClassCost;
			}

			curSkill.maxRank = 2;
			curSkill.nbPoint = 0;
			skillsTaken.push_back(curSkill);
		}


		//Now, parse the GFF
		{
			void* newPtr[5];
			void* newPtr2[5];

			GetGFFList(puVar, NULL, newPtr, gffPtr, "SkillList");
			int nbSkillTakens = GetGFFListLength(puVar, NULL, newPtr);

			for (uint8_t i = 0; i < nbSkillTakens; i++)
			{
				GetGFFStruct(puVar, NULL, newPtr2, newPtr, i);

				skillsTaken[i].nbPoint = GetGFFByte(puVar, NULL, newPtr2, "Rank", aiStack110, 0);
			}
		}

		//Ok, now parse the class skill and update them
		{
			uint32_t nbClassSkill = myClass->m_sizeSkillsTable;
			NWN2DA::clsSkills2DA* listClassSkill = myClass->m_clsSkillsTable;

			for (int iCS = 0; iCS < nbClassSkill; iCS++)
			{
				uint32_t cClassSkillID = listClassSkill[iCS].m_SkillIndex;
				uint32_t cClassSkillC = listClassSkill[iCS].m_ClassSkill;

				if (cClassSkillC == 1) {
					//Allow
					skillsTaken[cClassSkillID].maxRank = 4;
					skillsTaken[cClassSkillID].skillCost = 1;
				}
			}
		}

		std::string sListSkillTooHigh = "";
		std::string sListSkillNotAllowed = "";

		//We can now calculate and check the maxRank
		for (uint8_t i = 0; i < maxSkillId; i++)
		{
			if (skillsTaken[i].nbPoint > skillsTaken[i].maxRank)
			{
				std::string sError = " " + std::to_string(skillsTaken[i].skillId) + ":" + std::to_string(skillsTaken[i].nbPoint);
				sListSkillTooHigh += sError;
				/*
				std::string sErrorList2 = sErrorList + "#SkillTooHigh:{" + sListSkillTooHigh +"}";
				if (gMsgServerStopFirstCreation && CharacterCreationError(sAccountName, sErrorList2))
				{
					return 1;
				}
				*/
			}

			if (skillsTaken[i].nbPoint > 0)
			{
				if (skillsTaken[i].skillCost == -1)
				{
					sListSkillNotAllowed += " " + std::to_string(skillsTaken[i].skillId);
//					std::string sError = "SkillNotAllowed{"+ std::to_string(skillsTaken[i].skillId)+"};";
					/*
					std::string sErrorList2 += sErrorList + "#SkillNotAllowed:{" + sListSkillNotAllowed + "}";
					
					if (gMsgServerStopFirstCreation && CharacterCreationError(sAccountName, sErrorList2))
					{
						return 1;
					}
					*/
				}

				totalSkillPointUsed += skillsTaken[i].skillCost * skillsTaken[i].nbPoint;
			}
		}

		//prepare the errors if somes
		{
			if (sListSkillTooHigh != "")
			{
				sErrorList += "#SkillTooHigh:{" + sListSkillTooHigh +"}";
				if (gMsgServerStopFirstCreation && CharacterCreationError(sAccountName, sErrorList))
				{
					return 1;
				}
			}

			if (sListSkillNotAllowed != "")
			{
				sErrorList += "#SkillNotAllowed:{" + sListSkillNotAllowed + "}";
				if (gMsgServerStopFirstCreation && CharacterCreationError(sAccountName, sErrorList))
				{
					return 1;
				}
			}
		}

		if (totalSkillPointUsed > iNumberOfSkillPoints)
		{
			std::string sError = "#TooManySkillPoint:{"+ std::to_string(totalSkillPointUsed)+":"+std::to_string(iNumberOfSkillPoints)+"};";
			sErrorList += sError;
			if (gMsgServerStopFirstCreation && CharacterCreationError(sAccountName, sErrorList))
			{
				return 1;
			}
		}
	}


	//Feats tests
	;
	{
		//First, put in a list every takens feats.
		std::unordered_set<uint16_t> myTakenFeats;

		{
			void* newPtr[5];
			void* newPtr2[5];

			GetGFFList(puVar, NULL, newPtr, gffPtr, "FeatList");
			int nbFeatTaken = GetGFFListLength(puVar, NULL, newPtr);

			for (int i = 0; i < nbFeatTaken; i++)
			{
				GetGFFStruct(puVar, NULL, newPtr2, newPtr, i);

				myTakenFeats.insert(GetGFFWord(puVar, NULL, newPtr2, "Feat", aiStack110, 0));
			}
		}


		std::string sListRacialNotFound = "";
		//Ok, now we must remove the RaceFeats. (and check that we really have them)
		for (const auto& testFeat : myCurrentFeats)
		{
			if (myTakenFeats.count(testFeat) > 0)
			{
				myTakenFeats.erase(testFeat);
			}
			else
			{
				sListRacialNotFound += " " + std::to_string(testFeat);
				/*
				std::string sError = "RacialFeatNotFound{" + std::to_string(testFeat) + "};";
				sErrorList += sError;
				if (CharacterCreationError(sAccountName, sErrorList))
				{
					return 1;
				}
				*/
			}
		}

		if (sListRacialNotFound != "")
		{
			std::string sError = "#RacialFeatNotFound:{" + sListRacialNotFound + "}";
			sErrorList += sError;
			if (gMsgServerStopFirstCreation && CharacterCreationError(sAccountName, sErrorList))
			{
				return 1;
			}

		}

		//Count the number of allowed feats.
		int iNumberBonusFeat = myClass->m_BonusFeat[0];
		int iNumberGeneralFeat = 1 + myClass->m_NormalBFeat[0];

		if (myCurrentFeats.count(FEAT_QUICKT_TO_MASTER) > 0)
		{
			iNumberGeneralFeat++;
		}

		//Prepare the feats type and auto granted ones
		NWN2DA::clsFeat2DA* myClassFeatList = myClass->m_clsFeatTable;
		uint16_t sizeMyClassFeatList = myClass->m_sizeFeatTable;

		std::unordered_set<uint16_t> myBonusFeatsOnly;
		std::unordered_set<uint16_t> myRegularOrBonusFeats;
		std::unordered_set<uint16_t> mySelectableLvlUp;

		std::string sListAutoFeatNotFound = "";
		for (uint16_t i = 0; i < sizeMyClassFeatList; i++)
		{
			uint8_t fLType = myClassFeatList[i].m_List;
			int8_t grantedOn = myClassFeatList[i].m_GrantedOnLevel;

			int16_t curFeat = myClassFeatList[i].m_FeatIndex;

			//Automatic granted feat, check and remove !
			if (fLType == 0 )
			{
				if (grantedOn != -1 && grantedOn <= 1)
				{
					int16_t preReqGranted = myClassFeatList[i].m_GrantedPrereq;
					if (preReqGranted == -1 || myCurrentFeats.count(preReqGranted) != 0)
					{
						//All prereq ok, we must have this feat.
						if (myTakenFeats.erase(curFeat) == 0 && myCurrentFeats.count(curFeat) == 0)
						{
							sListAutoFeatNotFound += " " + std::to_string(curFeat);
							/*
							std::string sError = "AutoFeatNotFound{"+ std::to_string(curFeat)+ "};";
							sErrorList += sError;
							if (CharacterCreationError(sAccountName, sErrorList))
							{
								return 1;
							}
							*/
						}
						else {
							myCurrentFeats.insert(curFeat);
						}
					}
				}
			}
			//Bonus feat only
			else if (fLType == 2) {
				myBonusFeatsOnly.insert(curFeat);
			}
			//Regular feat or bonus feat
			else if (fLType == 3) {
				myRegularOrBonusFeats.insert(curFeat);
			}
			//Add to selectable on level up
			else {
				mySelectableLvlUp.insert(curFeat);
			}
		}

		if (sListAutoFeatNotFound != "")
		{
			std::string sError = "#AutoFeatNotFound:{"+ sListAutoFeatNotFound + "}";
			sErrorList += sError;
			if (gMsgServerStopFirstCreation && CharacterCreationError(sAccountName, sErrorList))
			{
				return 1;
			}
		}

		//Check if we have a background feat. Normaly, only one accepted. Remove it
		for (int i = 0; i < g_global2da->m_NumberOfBackground; i++)
		{
			NWN2DA::background2da myBackGroundToTest = GetBackgroundRow(i);
			if (myBackGroundToTest != NULL)
			{
				if (myBackGroundToTest->m_REMOVED == 0)
				{
					uint16_t iFeat = myBackGroundToTest->m_DisplayFeat;
					if (myTakenFeats.count(iFeat) > 0)
					{
						//Here, we have indead the feat. Remove it from Taken list and add it to currentlist.
						myTakenFeats.erase(iFeat);
						myCurrentFeats.insert(iFeat);

						//Ok, we have this feat. So now, test if we can take it.

						//MinAttackBonus
						if(myBackGroundToTest->m_MinAttackBonus > myClass->m_AttackBonus[0])
						{
							std::string sError = "#BackgroundReqError:{"+ std::to_string(i)+ "}";
							sErrorList += sError;
							if (gMsgServerStopFirstCreation && CharacterCreationError(sAccountName, sErrorList))
							{
								return 1;
							}
							break; //No need to test further
						}

						//Gender
						if (myBackGroundToTest->m_Gender != 2 && myBackGroundToTest->m_Gender != iGender)
						{
							std::string sError = "#BackgroundReqError:{"+ std::to_string(i)+ "}";
							sErrorList += sError;
							if (gMsgServerStopFirstCreation && CharacterCreationError(sAccountName, sErrorList))
							{
								return 1;
							}
							break; //No need to test further
						}

						//Class
						bool iMustTestclass = false;
						uint8_t iCTest1 = myBackGroundToTest->m_OrReqClass[0];
						uint8_t iCTest2 = myBackGroundToTest->m_OrReqClass[0];
						uint8_t iCTest3 = myBackGroundToTest->m_OrReqClass[0];

						if (iCTest1 != 0xFF && iCTest2 != 0xFF && iCTest3 != 0xFF)
							iMustTestclass = true;

						if (iMustTestclass)
						{
							if (iCTest1 != iClass && iCTest2 != iClass && iCTest3 != iClass)
							{
								std::string sError = "#BackgroundReqError:{"+ std::to_string(i)+ "}";
								sErrorList += sError;
								if (gMsgServerStopFirstCreation && CharacterCreationError(sAccountName, sErrorList))
								{
									return 1;
								}
								break; //No need to test further
							}
						}

						//Ability test
						for (int iAb = 0; iAb < 6; iAb++)
						{
							int iMyAb = 0;
							switch (iAb)
							{
								case 0: iMyAb = iStr; break;
								case 1: iMyAb = iDex; break;
								case 2: iMyAb = iCon; break;
								case 3: iMyAb = iInt; break;
								case 4: iMyAb = iWis; break;
								default: iMyAb = iCha;
							}

							int iAbT = myBackGroundToTest->m_MinAbility[iAb];
							if (iAbT != 0 && iMyAb < iAbT)
							{
								std::string sError = "#BackgroundReqError:{"+ std::to_string(i)+ "}";
								sErrorList += sError;
								if (gMsgServerStopFirstCreation && CharacterCreationError(sAccountName, sErrorList))
								{
									return 1;
								}
								break; //No need to test further
							}
							iAbT = myBackGroundToTest->m_MaxAbility[iAb];
							if (iAbT != 0 && iMyAb > iAbT)
							{
								std::string sError = "#BackgroundReqError:{"+ std::to_string(i)+ "}";
								sErrorList += sError;
								if (gMsgServerStopFirstCreation && CharacterCreationError(sAccountName, sErrorList))
								{
									return 1;
								}
								break; //No need to test further
							}
						}

						//Now, break, because we can only have 1 background feat.
						break;
					}
				}
			}
		}

		//Ok. Here we have removed the class and race autogranted feats and add them in myCurrentFeats.
		//Need now to test if the number of feat is ok.


		if (myTakenFeats.size() != (iNumberGeneralFeat + iNumberBonusFeat + myDomainsFeats.size()) )
		{
			//Ok, here are the difficult thing.
			//Feats can be taken and given by the domains. (for example, take luck of heroes and then chance domain)
			//Try to see that

			//for now... Just error if it can't be that (less than every domain feat taken or more than none)
			if (myTakenFeats.size() < (iNumberGeneralFeat + iNumberBonusFeat) ||
				(myTakenFeats.size() > (iNumberGeneralFeat + iNumberBonusFeat + myDomainsFeats.size())))
			{
				std::string sError = "#FeatNumber:{"+ std::to_string(myTakenFeats.size())+ "}";
				sErrorList += sError;
				if (gMsgServerStopFirstCreation && CharacterCreationError(sAccountName, sErrorList))
				{
					return 1;
				}
			}
		}


		//Test if we can select those feats.
		int iBonusFeatsPossibility = 0;
		int iRegularFeatsPossibility = 0;
		int iDomainFeat = 0;

		for (const auto& testFeat: myTakenFeats) 
		{
			bool bCurFeatPossible = false;

			if (myDomainsFeats.count(testFeat) > 0)
			{
				iDomainFeat++;
				bCurFeatPossible = true;
			}

			if (myRegularOrBonusFeats.count(testFeat) > 0 || myBonusFeatsOnly.count(testFeat)) {
				iBonusFeatsPossibility++;
				bCurFeatPossible = true;
			}
			if (myRegularOrBonusFeats.count(testFeat) > 0 || mySelectableLvlUp.count(testFeat) > 0 || FeatValidAllClasses(testFeat)) {
				iRegularFeatsPossibility++;
				bCurFeatPossible = true;
			}

			if (!bCurFeatPossible) {
				iRegularFeatsPossibility = -1;
				iBonusFeatsPossibility = -1;
				break;
			}
		}

		if (iRegularFeatsPossibility < iNumberGeneralFeat || iBonusFeatsPossibility < iNumberBonusFeat || iDomainFeat < myDomainsFeats.size())
		{
			std::string sError = "#FeatChoiceError:{}";
			sErrorList += sError;
			if (gMsgServerStopFirstCreation && CharacterCreationError(sAccountName, sErrorList))
			{
				return 1;
			}
		}

		//Remove the domains feat. Don't care anymore if its also taken as a feat or not
		std::string sListDomainFeatNotFound = "";
		for (const auto& testFeat : myDomainsFeats)
		{
			if (myTakenFeats.erase(testFeat) == 0)
			{
				sListDomainFeatNotFound += " " + std::to_string(testFeat);
				/*
				std::string sError = "#DomainFeatNotFound:{"+std::to_string(testFeat)+"}";
				sErrorList += sError;
				if (CharacterCreationError(sAccountName, sErrorList))
				{
					return 1;
				}
				*/
			}
			else
			{
				myCurrentFeats.insert(testFeat);
			}
		}

		if (sListDomainFeatNotFound != "")
		{
			std::string sError = "#DomainFeatNotFound:{"+sListDomainFeatNotFound+"}";
			sErrorList += sError;
			if (gMsgServerStopFirstCreation && CharacterCreationError(sAccountName, sErrorList))
			{
				return 1;
			}
		}

		//Only one background feat is allowed
		bool haveTakenBackgroundFeat = false;
		for (uint16_t featToTest : myTakenFeats)
		{
			NWN2DA::feats2DA myFeatToTest = GetFeatRow(featToTest);
			if (myFeatToTest != NULL)
			{
				if (myFeatToTest->m_FeatCategory == 9)
				{
					//Already took one
					if (haveTakenBackgroundFeat)
					{
						std::string sError = "#TooManyBackgroundFeat:{}";
						sErrorList += sError;
						if (gMsgServerStopFirstCreation && CharacterCreationError(sAccountName, sErrorList))
						{
							return 1;
						}
						break;//Don't need to test more, already an error
					}
					else
					{
						haveTakenBackgroundFeat = true;
					}
				}
			}
		}

		//Ok, now.. Test each feats prerequisites
		//we loop as long as we have thing 
		while(myTakenFeats.size() > 0)
		{
			std::unordered_set<uint16_t> myCurrentValidated;
			for (uint16_t featToTest : myTakenFeats) {
				if (TestIfFeatOk_CharCreation(featToTest, myClass, myCurrentFeats, skillsTaken, mySubType->m_Index, iStr, iDex, iCon, iInt, iWis, iCha, iGE, iLC))
					myCurrentValidated.insert(featToTest);
			}

			if (myCurrentValidated.size() == 0)
			{
				//Just leave, we will not validate anything else
				break;
			}
			else 
			{
				for (uint16_t featToAdd : myCurrentValidated) 
				{
					if (myTakenFeats.erase(featToAdd) != 0)
					{
						myCurrentFeats.insert(featToAdd);
					}
				}
			}
		}

		if (myTakenFeats.size() > 0)
		{
			std::string sError = "#AtLeastOneFeatNotValidated:{"+std::to_string(*myTakenFeats.begin()) + "};";
			sErrorList += sError;
			if (gMsgServerStopFirstCreation && CharacterCreationError(sAccountName, sErrorList))
			{
				return 1;
			}
		}
	}





	/////////////////////////////////////////////////////////////////////////////
	//////////////////////////////// VISUAL PART ////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////


	//Test scale
	{
		void* newPtr[5];
		if (GetGFFStructByName(puVar, NULL, newPtr, gffPtr, "ModelScale") != 1)
		{
			std::string sError = "#DataError:{};";
			sErrorList += sError;
			if (gMsgServerStopFirstCreation && CharacterCreationError(sAccountName, sErrorList))
			{
				return 1;
			}
		}
		else {
			float scaleX = GetGFFFloat(puVar, NULL, newPtr, "x", aiStack110, 0.0);
			float scaleY = GetGFFFloat(puVar, NULL, newPtr, "y", aiStack110, 0.0);
			float scaleZ = GetGFFFloat(puVar, NULL, newPtr, "z", aiStack110, 0.0);

			float fEspilon = 0.001f;
			//Scale X and Y should be equals
			if (std::abs(scaleX - scaleY) > fEspilon)
			{
				std::string sError = "#ScaleXandYDifferent:{" + std::to_string(scaleX) + " , " + std::to_string(scaleY) + "}";
				sErrorList += sError;
				if (gMsgServerStopFirstCreation && CharacterCreationError(sAccountName, sErrorList))
				{
					return 1;
				}
			}

			//
			if (scaleX < (gfMinScaleX - fEspilon) || scaleX >(gfMaxScaleX + fEspilon) || scaleZ < (gfMinScaleZ - fEspilon) || scaleZ >(gfMaxScaleZ + fEspilon))
			{
				std::string sError = "#ScaleError:{X:" + std::to_string(scaleX) + ",Z:" + std::to_string(scaleZ) + "}";
				sErrorList += sError;
				if (gMsgServerStopFirstCreation && CharacterCreationError(sAccountName, sErrorList))
				{
					return 1;
				}
			}
		}
	}

	return 0;
}

__declspec(naked) void CharacterCreationReturnCode()
{
	//Load the wanted return code and set it back to default one
	__asm
	{
		mov		eax, dword ptr[g_iCharCreationReturnCode]
		mov		g_iCharCreationReturnCode, 0x23D9
		jmp		dword ptr[ContinueCharacterCreationError]
	}
}

__declspec(naked) void CharacterCreationCheck()
{
	__asm
	{
		//Ok, save ECX
		PUSH	ECX
		PUSH	EDX

		//Ok, now we must do all the other tests
		LEA		ECX, [ESP + 0x30]
		PUSH	ECX
		LEA		EDX, [ESP + 0x40]
		MOV		ECX, [EBP +0x8]
		PUSH	ECX
		MOV		ECX, ESI
		CALL	AdvancedCharacterCreationCheck

		//Restore EDX, ECX
		POP		EDX
		POP		ECX

		TEST	EAX, EAX
		JE		FinishCharacterCreationTest	

		JMP		dword ptr[AdvancedCharacterCheckError]
		//Check if race OK (player race) done

		// Check if size ok done

		// Check Color ? (for later)

		//Check if Carac Ok. done

		//Save carac + race bonus. done 

		//Check if Class OK : alignment , (god for latter?) , prereqTable. done (there is something strange wis ability, seems to be more a client side stuff) 

		//Check if CompPointsOK. done

		//Check if feats OK. Done


		//Familiar/AnimalCompanion TODO
		//Spells...TODO

FinishCharacterCreationTest:
		//Resume the original flow
		CALL dword ptr[InitEngineString]
		JMP	 dword ptr[ReturnToCharacterCheck]
	}

}

namespace {
	Patch _CharacterCreationPatches[] =
	{
		Patch((DWORD)OFFS_CharacterCreationVerif, (char*)"\xe9\x00\x00\x00\x00", (int)5),
		Patch(OFFS_CharacterCreationVerif + 1, (relativefunc)CharacterCreationCheck),
		
		Patch((DWORD)OFFS_CharacterCreationErrorCode, (char*)"\xe9\x00\x00\x00\x00", (int)5),
		Patch(OFFS_CharacterCreationErrorCode+1, (relativefunc)CharacterCreationReturnCode),

		Patch()
	};

	Patch *CharacterCreationPatches = _CharacterCreationPatches;

} // namespace

bool ApplyAntiCheatCreationPatch(SimpleIniConfig* config, bool activateIt)
{
	g_ScriptCreationError = "";

	//Script on error (creation) ?
	config->Read("ScriptOnCreationError", &g_ScriptCreationError, std::string(""));
	
	gfMinScaleX = 0.95f;
	gfMinScaleZ = 0.95f;
	gfMaxScaleX = 1.05f;
	gfMaxScaleZ = 1.05f;

	int iCreationStopFirst = 0;
	config->Read("StopLvlUpFirstViolation", &iCreationStopFirst, 1);
	gMsgServerStopFirstCreation = (iCreationStopFirst != 0);

	config->Read("MinScaleX", &gfMinScaleX, 0.95f);
	config->Read("MinScaleZ", &gfMinScaleZ, 0.95f);

	config->Read("MaxScaleX", &gfMaxScaleX, 1.05f);
	config->Read("MaxScaleZ", &gfMaxScaleZ, 1.05f);

	int i = 0;
	while(CharacterCreationPatches[i].Apply()) {
		i++;
	}

	g_AnticheatActivated = activateIt;

	return true;
}

void ChangeCharacterCreationStatus(bool bAntiCheatActivated)
{
	g_AnticheatActivated = bAntiCheatActivated;
}
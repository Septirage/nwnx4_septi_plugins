
#include <unordered_map>
#include <string>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <sstream>
#include <stdexcept>

#include "RuleCreatureFct.h"
#include "../../septutil/NwN2DataPos.h"


__declspec(naked) int8_t __fastcall GetRacialAbilityBonus_Intern(__in void* pGlobalState, __in void* Unused, __in int abilityID, __in uint16_t uRace, __in uint16_t uSubRace)
{
	__asm
	{
		mov             edx, 0x0076c040;
		jmp             edx;
	}
}

int FunctionClassBased(const std::vector<int>& params, int creaBlock, FunctionType tType) 
{
	int iValue = 0;
	// For each, check if in params, if yes, summ or take max
	uint8_t iNbClass   = *(uint8_t*)(creaBlock + 0x14);
	int iBaseClass = 0x10C;

	for (uint8_t i = 0; i < iNbClass; i++) {
		uint8_t iClassID = *(uint8_t*)(creaBlock + iBaseClass + 0x4);

		auto it = std::find(params.begin(), params.end(), iClassID);

		// The current class is one of the parameters
		if (it != params.end()) {
			// Somme
			if (tType == FunctionType::CLASSLVLSUM)
				iValue += *(uint8_t*)(creaBlock + iBaseClass + 0x5);
			else //Max
			{
				uint8_t tVal = *(uint8_t*)(creaBlock + iBaseClass + 0x5);
				if (iValue < tVal)
					iValue = tVal;
			}
		}
		iBaseClass += 0x0124;
	}

	return iValue;
}

int GetBaseSkill(int iSkill, int creaBlock)
{
	int res = 0;
	uint8_t maxSkillId = GetGlobal2DA()->m_NumberOfSkills;
	if (iSkill >= 0 && iSkill < maxSkillId)
	{
		uint8_t* currentSkillList = *(uint8_t**)(creaBlock+AmCrtABCurrentSkillList);
		res = currentSkillList[iSkill];
	}
	return res;
}

int GetBaseAbility(int iAbility, int creaBlock)
{
	int res = 0;
	if (iAbility >= 0 && iAbility < 6)
	{
		res = *(uint8_t *)(iAbility + 4 + creaBlock);
		uint16_t uRace = *(uint16_t *)(creaBlock + AmCrtABRace);
		uint16_t uSubRace = *(uint16_t *)(creaBlock + AmCrtABSubRace);
		int8_t iBonus = GetRacialAbilityBonus_Intern((void*)GetGlobal2DA(), NULL, iAbility, uRace, uSubRace);
		res += iBonus;
	}
	return res;
}

int ClassLevelSum(const std::vector<int>& params, int creaBlock)
{
	return FunctionClassBased(params, creaBlock, FunctionType::CLASSLVLSUM);
}


int ClassLevelMax(const std::vector<int>& params, int creaBlock) 
{ 
	return FunctionClassBased(params, creaBlock, FunctionType::CLASSLVLMAX);
}

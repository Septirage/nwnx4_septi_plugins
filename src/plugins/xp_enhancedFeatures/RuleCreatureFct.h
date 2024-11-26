#if !defined(SEPT_RULECREAFCT)
#define SEPT_RULECREAFCT


#include "../../septutil/intern2daMgt.h"

enum class FunctionType {
	CLASSLVLSUM,
	CLASSLVLMAX,
	SKILL,
	ABILITY
};


int FunctionClassBased(const std::vector<int>& params, int creaBlock, FunctionType tType);
int ClassLevelSum(const std::vector<int>& params, int creaBlock);
int ClassLevelMax(const std::vector<int>& params, int creaBlock);
int GetBaseSkill(int iSkill, int creaBlock);
int GetBaseAbility(int iAbility, int creaBlock);


#endif

#include <misc/log.h>
#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include <misc/Patch.h>
#include <hook/scriptManagement.h>
#include "../../septutil/mini.h"

#include <string>
#include <vector>
#include <stdexcept>
#include <sstream>

#include "RuleParser.h"
#include "BonusParser.h"

extern std::unique_ptr<LogNWNX> logger;

//The 3 engine functions (thiscall: ECX = stats, 1 parameter: 1 = off hand ; ret 4)
//Threat: first roll of the threat range (19 for 19-20). Attack roll + character sheet (both hands)
#define OFFS_CriticalThreat					0x0059eaa0		//sub esp,8 / cmp [esp+0xC],1 (8 bytes)
unsigned long ReturnCriticalThreatBody	= 0x0059eaa8;
//Confirm: bonus to the confirmation roll. Attack roll + TouchAttack
#define OFFS_CriticalConfirm				0x0059ec10		//cmp [esp+4],0 (5 bytes)
unsigned long ReturnCriticalConfirmBody	= 0x0059ec15;
//Multiplier (GetCreatureCritMultilplier). Damages + character sheet (off hand)
#define OFFS_CriticalMultiplier				0x0059eca0		//cmp [esp+4],1 (5 bytes)
unsigned long ReturnCriticalMultiplierBody = 0x0059eca5;

//UpdateCombatInformation has an inline copy of GetCreatureCritMultilplier for the main hand (0x5ac907-0x5ac963,
//result in EBX). Replaced by a call to the function, so that the sheet uses the hooked function too
#define OFFS_InlineMainHandMultiplier		0x005ac907

//TouchAttack(Melee/Ranged) (0x6909c0, one routine for both commands) does not call the threat function: its threat roll
//is hardcoded in [esp+0x18], 20, or 19 with the feat 0x83d (melee) / 0x83e (ranged). Compared to the d20 only when the
//touch hits, attacker and target creatures (the non creature attacker path has its own hardcoded 20, not hooked)
#define OFFS_TouchAttackThreat				0x00690c38		//cmp eax,[esp+0x18] / jl 0x690c84 (6 bytes)
unsigned long ReturnTouchAttackThreat		= 0x00690c3e;	//threat: confirmation roll
unsigned long ReturnTouchAttackNoThreat		= 0x00690c84;	//no threat: hit (1)

//ResolveDamage (0x63f920, thiscall: ESI = attacker creature, EDI = target object ; ret 4): junction after the Epic Dodge
//test. The Epic Dodge branch ends with its own ret: what arrives here was not dodged. 4 jumps to 0x63fd01 itself
//(0x63fbea, 0x63fbf5, 0x63fc1c, 0x63fc38), none inside 0x63fd02-0x63fd07. No x87 in ResolveDamage.
//Damage packet at [esp+0x10]: [esp+0x34] = critical (packet+0x24 = attack result 3, set at 0x63f975), [esp+0x68] = off hand
#define OFFS_ResolveDamageAfterEpicDodge	0x0063fd01		//cmp dword ptr [edi+0x180],1 (7 bytes)
unsigned long ReturnResolveDamageAfterEpicDodge	= 0x0063fd08;	//jne 0x63fd12 (flags of the cmp)
//End of ResolveDamage: lea ecx,[esp+0x4c] / call 0x5820b0 (frees the DR list of the packet) / pop x4 / add esp,0x5c / ret 4
unsigned long ReturnResolveDamageEnd			= 0x0063fdcf;

//GetCreatureCritMultilplier (thiscall: ECX = stats, 1 = off hand ; ret 4). Hooked above when the file is read
#define OFFS_GetCreatureCritMultiplier		0x0059eca0

//Safety bounds: below, the engine gives no critical at all (threat) or no weapon damage on a critical (multiplier)
#define CRITICAL_MIN_THREAT					2
#define CRITICAL_MIN_MULTIPLIER				1

//TouchType of a CriticalTouchThreatRule (mask). The other lists keep TOUCHTYPE_ALL and are never filtered
#define TOUCHTYPE_MELEE						0x01
#define TOUCHTYPE_RANGED					0x02
#define TOUCHTYPE_ALL						(TOUCHTYPE_MELEE | TOUCHTYPE_RANGED)

//Command of TouchAttack, first parameter ([esp+0x30] at the hook): 0x92 TouchAttackMelee, 0x93 TouchAttackRanged
#define TOUCHATTACK_CMD_MELEE				0x92


struct CriticalRuleSet {
	RuleParser::Rule* featRule;
	RuleParser::Rule* areaTypeRule;
	RuleParser::Rule* extraRule;
	MathExpressionParser::Expr* bonusCalculation;
	int index;
	std::string sScript;	//CriticalEventRule only (no Bonus there)
	uint8_t touchMask;		//CriticalTouchThreatRule only (TouchType), TOUCHTYPE_ALL elsewhere


	CriticalRuleSet(MathExpressionParser::Expr* bonus, int idx)
	    : bonusCalculation(bonus)
	    , index(idx)
	    , touchMask(TOUCHTYPE_ALL)
	{
		featRule = nullptr;
		areaTypeRule = nullptr;
		extraRule    = nullptr;
	}

	~CriticalRuleSet()
	{
		delete bonusCalculation;
		delete areaTypeRule;
		delete extraRule;
		delete featRule;
	}
};


std::vector<CriticalRuleSet*> m_criticalThreatRuleSetList;
std::vector<CriticalRuleSet*> m_criticalConfirmRuleSetList;
std::vector<CriticalRuleSet*> m_criticalMultiplierRuleSetList;
std::vector<CriticalRuleSet*> m_criticalTouchThreatRuleSetList;
std::vector<CriticalRuleSet*> m_criticalEventRuleSetList;


static bool ParseCriticalRuleCondition(std::string sRuleX, std::string sValue, RuleParser::RuleType tFeatRule, std::vector<CriticalRuleSet*>& myRuleList)
{
	try {
		std::vector<RuleParser::Token> tokens1 = RuleParser::tokenize(sValue, tFeatRule);
		int pos1                   = 0;
		RuleParser::Rule* root1                = RuleParser::parseExpression(tokens1, pos1);
		if (tFeatRule == RuleParser::RuleType::FEAT)
			myRuleList.back()->featRule = root1;
		else if (tFeatRule == RuleParser::RuleType::AREA)
			myRuleList.back()->areaTypeRule = root1;
		else
			myRuleList.back()->extraRule = root1;

	} catch (const std::exception& e) {
		std::string sRuleType;

		if (tFeatRule == RuleParser::RuleType::FEAT)
			sRuleType = "Feat";
		else if (tFeatRule == RuleParser::RuleType::AREA)
			sRuleType = "Area";
		else
			sRuleType = "Extra";
		logger->Err("Error during parse of [%s] \"%s\" = %s : %s", sRuleX.c_str(),
		            sRuleType.c_str(), sValue.c_str(), e.what());
		logger->Info("Due to the Error, the complete rule will not be applied");
		return false;
	}

	return true;
}


static bool ParseCriticalModifier(int iRuleNumber, std::string sValue, std::vector<CriticalRuleSet*>& myRuleList, std::string sField)
{
	try {
		auto bonusCalc = MathExpressionParser::prepareSimplified(sValue, MathExpressionParser::Mode::INTEGER);
		CriticalRuleSet* rs1 = new CriticalRuleSet(bonusCalc, iRuleNumber);

		myRuleList.push_back(rs1);
	} catch (const std::exception& e) {

		logger->Err("Error during parse of \"%s\" = %s : %s", sField.c_str(), sValue.c_str(), e.what());
		logger->Info("Due to the Error, the complete rule will not be applied");
		return false;
	}

	return true;
}


//Sum of the rules of one list. uTouchMask: only the rules of this touch type (touch attacks), TOUCHTYPE_ALL elsewhere
static int32_t criticalRuleApplication(int pcBlock, std::vector<CriticalRuleSet*>& myRuleList, uint8_t uTouchMask = TOUCHTYPE_ALL)
{
	int32_t iMod = 0;
	int areaType = 0;

	if (myRuleList.empty())
		return 0;

	int pcPtr = *(int*)(pcBlock + 0xA4);

	//GetAreaType once for all
	{
		GameObject* Object;
		GameObjectManager m_ObjectManager;
		NWN::OBJECTID areaID = *(NWN::OBJECTID*)(pcPtr + 0x144);

		if ((areaID & NWN::INVALIDOBJID) != 0)
			areaID &= ~(NWN::LISTTYPE_MASK);

		if ((Object = m_ObjectManager.GetGameObject(areaID)) != NULL) {
			if (Object->GetObjectType() == NWN::OBJECT_TYPE_AREA) {
				char* areaPtr = (char*)Object->AsArea();

				areaType = *(uint32_t*)(areaPtr + 0x8);
			}
		}
	}

	RuleParser::m_activatedRuleSet.clear();

	for (CriticalRuleSet* myRule : myRuleList)
	{
		if ((myRule->touchMask & uTouchMask) == 0)
			continue;

		bool bOk = true;
		if (myRule->featRule != nullptr)
		{
			bOk = RuleParser::evaluateRule(myRule->featRule, RuleParser::RuleType::FEAT, pcBlock);
		}

		if (bOk && myRule->areaTypeRule != nullptr)
		{
			bOk = RuleParser::evaluateRule(myRule->areaTypeRule, RuleParser::RuleType::AREA, areaType);
		}

		if (bOk && myRule->extraRule != nullptr)
		{
			bOk = RuleParser::evaluateRule(myRule->extraRule, RuleParser::RuleType::EXTRA, pcPtr);
		}

		if (bOk)
		{
			RuleParser::m_activatedRuleSet.insert(myRule->index);
			iMod += myRule->bonusCalculation->evaluateInt(pcBlock);
		}
	}

	RuleParser::m_activatedRuleSet.clear();

	return iMod;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Original functions: the overwritten first instructions, then the rest of the engine function.
// Called as the engine does (ECX = stats, the parameter on the stack), they return to the caller with ret 4.
//////////////////////////////////////////////////////////////////////////////////////////////////////////

__declspec(naked) int __fastcall CriticalThreatOriginal(void* pStats, void* Unused, int iOffHand)
{
	__asm
	{
		sub		esp, 8
		cmp		dword ptr [esp + 0xC], 1
		jmp		dword ptr [ReturnCriticalThreatBody]
	}
}

__declspec(naked) int __fastcall CriticalConfirmOriginal(void* pStats, void* Unused, int iOffHand)
{
	__asm
	{
		cmp		dword ptr [esp + 4], 0
		jmp		dword ptr [ReturnCriticalConfirmBody]
	}
}

__declspec(naked) int __fastcall CriticalMultiplierOriginal(void* pStats, void* Unused, int iOffHand)
{
	__asm
	{
		cmp		dword ptr [esp + 4], 1
		jmp		dword ptr [ReturnCriticalMultiplierBody]
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Hooks: jumped to from the start of the engine functions. A __fastcall with an unused EDX and one stack
// parameter has the same convention as the engine function (thiscall, ret 4).
//////////////////////////////////////////////////////////////////////////////////////////////////////////

int __fastcall CriticalThreatHook(void* pStats, void* Unused, int iOffHand)
{
	int iThreat = CriticalThreatOriginal(pStats, NULL, iOffHand);

	//The range grows: its first roll goes down
	iThreat -= criticalRuleApplication((int)pStats, m_criticalThreatRuleSetList);
	if (iThreat < CRITICAL_MIN_THREAT)
		iThreat = CRITICAL_MIN_THREAT;

	return iThreat;
}

int __fastcall CriticalConfirmHook(void* pStats, void* Unused, int iOffHand)
{
	return CriticalConfirmOriginal(pStats, NULL, iOffHand) + criticalRuleApplication((int)pStats, m_criticalConfirmRuleSetList);
}

static int ApplyMultiplierRules(int pcBlock, int iMultiplier)
{
	iMultiplier += criticalRuleApplication(pcBlock, m_criticalMultiplierRuleSetList);
	if (iMultiplier < CRITICAL_MIN_MULTIPLIER)
		iMultiplier = CRITICAL_MIN_MULTIPLIER;

	return iMultiplier;
}

int __fastcall CriticalMultiplierHook(void* pStats, void* Unused, int iOffHand)
{
	return ApplyMultiplierRules((int)pStats, CriticalMultiplierOriginal(pStats, NULL, iOffHand));
}

//Touch attacks: the threat roll of the engine (20 or 19), then the range grows as with CriticalThreatRule.
//Only the rules of the touch type of the command (same test as the engine for the feat 0x83d / 0x83e, 0x690aeb)
int __fastcall TouchAttackThreatRules(void* pStats, int iCommand, int iThreat)
{
	uint8_t uTouchMask = (iCommand == TOUCHATTACK_CMD_MELEE) ? TOUCHTYPE_MELEE : TOUCHTYPE_RANGED;

	iThreat -= criticalRuleApplication((int)pStats, m_criticalTouchThreatRuleSetList, uTouchMask);
	if (iThreat < CRITICAL_MIN_THREAT)
		iThreat = CRITICAL_MIN_THREAT;

	return iThreat;
}

//Jumped to from TouchAttack when the touch hits (attacker and target creatures). ESP = stack of TouchAttack:
//[esp+0x14] = d20 (also in EAX), [esp+0x18] = threat roll of the engine, [esp+0x30] = command (0x92 / 0x93, never
//written by TouchAttack). ESI = attacker (object). EBX, EDI and EBP are read again by the engine (kept by the calls).
//EAX, ECX, EDX and the flags are written again before any read.
__declspec(naked) void TouchAttackThreatHook()
{
	__asm
	{
		mov		ecx, esi
		mov		edx, dword ptr [esi]
		call	dword ptr [edx + 0x68]			//Attacker as creature (not NULL on this path, tested at 0x690ac1)
		mov		ecx, dword ptr [eax + 0x1FC4]	//Attacker stats
		mov		edx, dword ptr [esp + 0x30]		//Command: TouchAttackMelee or TouchAttackRanged
		push	dword ptr [esp + 0x18]			//Threat roll of the engine
		call	TouchAttackThreatRules			//fastcall, ret 4
		mov		dword ptr [esp + 0x18], eax

		//Overwritten instructions (EAX = d20 again)
		mov		eax, dword ptr [esp + 0x14]
		cmp		eax, dword ptr [esp + 0x18]
		jl		NoThreat
		jmp		dword ptr [ReturnTouchAttackThreat]
NoThreat:
		jmp		dword ptr [ReturnTouchAttackNoThreat]
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////
// Critical event: on a critical hit confirmed and not cancelled (immunity, Epic Dodge), run the Script of every
// CriticalEventRule whose conditions are true for the attacker. No return value: the critical is never changed.
//////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef int (__thiscall *GetCreatureCritMultiplier_t)(void* pStats, int iOffHand);
static GetCreatureCritMultiplier_t const GetCreatureCritMultiplier = (GetCreatureCritMultiplier_t)OFFS_GetCreatureCritMultiplier;

//A script started by the event never starts the event again
static bool g_bCriticalEventRunning = false;

//Scripts of the rules true for the attacker, in the order of the rules (all the true rules, not only the first)
static void criticalEventRuleScripts(int pcBlock, std::vector<std::string>& scripts)
{
	int areaType = 0;
	int pcPtr = *(int*)(pcBlock + 0xA4);

	//GetAreaType once for all
	{
		GameObject* Object;
		GameObjectManager m_ObjectManager;
		NWN::OBJECTID areaID = *(NWN::OBJECTID*)(pcPtr + 0x144);

		if ((areaID & NWN::INVALIDOBJID) != 0)
			areaID &= ~(NWN::LISTTYPE_MASK);

		if ((Object = m_ObjectManager.GetGameObject(areaID)) != NULL) {
			if (Object->GetObjectType() == NWN::OBJECT_TYPE_AREA) {
				char* areaPtr = (char*)Object->AsArea();

				areaType = *(uint32_t*)(areaPtr + 0x8);
			}
		}
	}

	RuleParser::m_activatedRuleSet.clear();

	for (CriticalRuleSet* myRule : m_criticalEventRuleSetList)
	{
		bool bOk = true;
		if (myRule->featRule != nullptr)
		{
			bOk = RuleParser::evaluateRule(myRule->featRule, RuleParser::RuleType::FEAT, pcBlock);
		}

		if (bOk && myRule->areaTypeRule != nullptr)
		{
			bOk = RuleParser::evaluateRule(myRule->areaTypeRule, RuleParser::RuleType::AREA, areaType);
		}

		if (bOk && myRule->extraRule != nullptr)
		{
			bOk = RuleParser::evaluateRule(myRule->extraRule, RuleParser::RuleType::EXTRA, pcPtr);
		}

		if (bOk)
		{
			RuleParser::m_activatedRuleSet.insert(myRule->index);
			scripts.push_back(myRule->sScript);
		}
	}

	RuleParser::m_activatedRuleSet.clear();
}

//The object of this id is still the same object. GetGameObject (0x5ba910 -> 0x54d220) gives the CGameObject, at the
//same address as the creature (cast +0x68) and the object (cast +0x3c) for creatures, placeables and doors
static bool IsSameGameObject(NWN::OBJECTID oObject, char* pObject)
{
	GameObjectManager m_ObjectManager;

	return (char*)m_ObjectManager.GetGameObject(oObject) == pObject;
}

//Returns 1: ResolveDamage goes on. 0: the attacker or the target is no longer the same object after a script,
//ResolveDamage ends at once (no damage written in the attack)
int __fastcall CriticalEventCall(char* pAttacker, char* pTarget, int iOffHand)
{
	if (g_bCriticalEventRunning || m_criticalEventRuleSetList.empty())
		return 1;

	int pcBlock = *(int*)(pAttacker + 0x1FC4);

	//All the rules are checked before any script runs (a script can change what they test)
	std::vector<std::string> scripts;
	criticalEventRuleScripts(pcBlock, scripts);

	if (scripts.empty())
		return 1;

	NWN::OBJECTID oAttacker = *(NWN::OBJECTID*)(pAttacker + 0xA0);
	NWN::OBJECTID oTarget	= *(NWN::OBJECTID*)(pTarget + 0xA0);
	int bOffHand			= (iOffHand != 0) ? 1 : 0;
	//Same function and same hand as the damage roll (with the CriticalMultiplierRules)
	int iMultiplier			= GetCreatureCritMultiplier((void*)pcBlock, bOffHand);
	int iContinue			= 1;

	g_bCriticalEventRunning = true;

	for (const std::string& sScript : scripts)
	{
		bool isExecScriptOk = true;

		NWScript::ClearScriptParams();
		NWScript::AddScriptParameterObject(oAttacker);
		NWScript::AddScriptParameterObject(oTarget);
		NWScript::AddScriptParameterInt(bOffHand);
		NWScript::AddScriptParameterInt(iMultiplier);
		NWScript::ExecuteScriptEnhanced(sScript.c_str(), oAttacker, false, &isExecScriptOk, false);

		if (!IsSameGameObject(oAttacker, pAttacker) || !IsSameGameObject(oTarget, pTarget))
		{
			logger->Warn("CriticalEvent: attacker or target no longer valid after %s, damage of this attack skipped", sScript.c_str());
			iContinue = 0;
			break;
		}
	}

	g_bCriticalEventRunning = false;

	return iContinue;
}

//Jumped to from 0x63fd01 (ResolveDamage, after Epic Dodge). ESP = stack of ResolveDamage, ESI = attacker creature,
//EDI = target object. EAX, ECX, EDX and the flags are free: the overwritten cmp is replayed just before going back.
__declspec(naked) void CriticalEventHook()
{
	__asm
	{
		cmp		dword ptr [esp + 0x34], 1		//Critical counted (packet+0x24)
		jne		ReplayCmp

		pushad									//0x20
		pushfd									//0x04: the stack of ResolveDamage is now at +0x24
		push	dword ptr [esp + 0x8C]			//Off hand (packet+0x58, [esp+0x68] of ResolveDamage)
		mov		edx, edi						//Target object
		mov		ecx, esi						//Attacker creature
		call	CriticalEventCall				//fastcall, ret 4
		test	eax, eax
		jz		EndOfResolveDamage
		popfd
		popad

ReplayCmp:
		//Overwritten instruction (its flags are read by the jne at 0x63fd08)
		cmp		dword ptr [edi + 0x180], 1
		jmp		dword ptr [ReturnResolveDamageAfterEpicDodge]

EndOfResolveDamage:
		popfd
		popad
		jmp		dword ptr [ReturnResolveDamageEnd]
	}
}

Patch _CriticalRulePatches[] = {
	Patch((DWORD)OFFS_CriticalThreat, (char*)"\xe9\x00\x00\x00\x00\x90\x90\x90", (int)8),
	Patch(OFFS_CriticalThreat + 1, (relativefunc)CriticalThreatHook),

	Patch((DWORD)OFFS_CriticalConfirm, (char*)"\xe9\x00\x00\x00\x00", (int)5),
	Patch(OFFS_CriticalConfirm + 1, (relativefunc)CriticalConfirmHook),

	Patch((DWORD)OFFS_CriticalMultiplier, (char*)"\xe9\x00\x00\x00\x00", (int)5),
	Patch(OFFS_CriticalMultiplier + 1, (relativefunc)CriticalMultiplierHook),

	//PUSH 0 / MOV ECX,EBP / CALL GetCreatureCritMultilplier (0x59eca0) / MOV EBX,EAX / JMP 0x5ac964
	Patch((DWORD)OFFS_InlineMainHandMultiplier, (char*)"\x6a\x00\x8b\xcd\xe8\x90\x23\xff\xff\x8b\xd8\xeb\x50", (int)13),

	Patch()
};

Patch *CriticalRulePatches = _CriticalRulePatches;

//Only applied when at least one CriticalTouchThreatRule is configured
Patch _CriticalTouchThreatPatches[] = {
	Patch((DWORD)OFFS_TouchAttackThreat, (char*)"\xe9\x00\x00\x00\x00\x90", (int)6),
	Patch(OFFS_TouchAttackThreat + 1, (relativefunc)TouchAttackThreatHook),

	Patch()
};

Patch *CriticalTouchThreatPatches = _CriticalTouchThreatPatches;

//Only applied when at least one CriticalEventRule is configured.
//Original bytes at 0x63fd01: 83 bf 80 01 00 00 01 (cmp dword ptr [edi+0x180],1)
Patch _CriticalEventPatches[] = {
	Patch((DWORD)OFFS_ResolveDamageAfterEpicDodge, (char*)"\xe9\x00\x00\x00\x00\x90\x90", (int)7),
	Patch(OFFS_ResolveDamageAfterEpicDodge + 1, (relativefunc)CriticalEventHook),

	Patch()
};

Patch *CriticalEventPatches = _CriticalEventPatches;

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
bool isCriticalAlreadyPatched = false;
bool isCriticalTouchThreatAlreadyPatched = false;
bool isCriticalEventAlreadyPatched = false;

static void ClearCriticalRuleList(std::vector<CriticalRuleSet*>& myRuleList)
{
	for (auto ruleSet : myRuleList) {
		delete ruleSet;
	}
	myRuleList.clear();
}

void desinitCriticalRule()
{
	ClearCriticalRuleList(m_criticalThreatRuleSetList);
	ClearCriticalRuleList(m_criticalConfirmRuleSetList);
	ClearCriticalRuleList(m_criticalMultiplierRuleSetList);
	ClearCriticalRuleList(m_criticalTouchThreatRuleSetList);
	ClearCriticalRuleList(m_criticalEventRuleSetList);
}

//TouchType field of a CriticalTouchThreatRule: one word, MELEE, RANGED or ALL. 0 if unknown
static uint8_t ParseTouchType(std::string sValue)
{
	std::string sWord, sMore;
	std::istringstream iss(sValue);
	if (!(iss >> sWord) || (iss >> sMore))
		return 0;

	for (char& c : sWord)
		c = (char)toupper((unsigned char)c);

	if (sWord == "MELEE")
		return TOUCHTYPE_MELEE;
	if (sWord == "RANGED")
		return TOUCHTYPE_RANGED;
	if (sWord == "ALL")
		return TOUCHTYPE_ALL;

	return 0;
}

//Read the sections sRuleBase1, sRuleBase2... into myRuleList. bReadTouchType: the TouchType field is read (touch attacks)
static void ParseCriticalRuleSections(mINI::INIStructure& ini, std::string sRuleBase, std::vector<CriticalRuleSet*>& myRuleList, bool bReadTouchType = false)
{
	int iRuleNumber = 1;
	std::string sRuleX = sRuleBase + std::to_string(iRuleNumber);

	while (ini.has(sRuleX))
	{
		auto section = ini.get(sRuleX);

		uint8_t uTouchMask = TOUCHTYPE_ALL;
		if (bReadTouchType && section.has("TouchType"))
			uTouchMask = ParseTouchType(section.get("TouchType"));

		if (!section.has("Bonus"))
		{
			logger->Err("[CriticalFile]: No Bonus field for [%s].", sRuleX.c_str());
		}
		else if (uTouchMask == 0)
		{
			logger->Err("[CriticalFile]: Bad TouchType field for [%s] : %s (MELEE, RANGED or ALL).", sRuleX.c_str(), section.get("TouchType").c_str());
		}
		else if (ParseCriticalModifier(iRuleNumber, section.get("Bonus"), myRuleList, "Bonus"))
		{
			bool bValidRule = true;
			myRuleList.back()->touchMask = uTouchMask;

			if (section.has("Area"))
			{
				bValidRule = ParseCriticalRuleCondition(sRuleX, section.get("Area"), RuleParser::RuleType::AREA, myRuleList);
			}

			if (bValidRule && section.has("Feat"))
			{
				bValidRule = ParseCriticalRuleCondition(sRuleX, section.get("Feat"), RuleParser::RuleType::FEAT, myRuleList);
			}

			if (bValidRule && section.has("Extra"))
			{
				bValidRule = ParseCriticalRuleCondition(sRuleX, section.get("Extra"), RuleParser::RuleType::EXTRA, myRuleList);
			}

			if (!bValidRule)
			{
				CriticalRuleSet* rsToDel = myRuleList.back();
				myRuleList.pop_back();
				delete rsToDel;
			}
		}

		iRuleNumber++;
		sRuleX = sRuleBase + std::to_string(iRuleNumber);
	}
}

//Read the sections CriticalEventRule1, CriticalEventRule2... (Script required, no Bonus)
static void ParseCriticalEventRuleSections(mINI::INIStructure& ini)
{
	int iRuleNumber = 1;
	std::string sRuleBase = "CriticalEventRule";
	std::string sRuleX = sRuleBase + std::to_string(iRuleNumber);

	while (ini.has(sRuleX))
	{
		auto section = ini.get(sRuleX);

		if (!section.has("Script") || section.get("Script") == "")
		{
			logger->Err("[CriticalFile]: No Script field for [%s].", sRuleX.c_str());
		}
		else
		{
			CriticalRuleSet* rs1 = new CriticalRuleSet(nullptr, iRuleNumber);
			rs1->sScript = section.get("Script");
			m_criticalEventRuleSetList.push_back(rs1);

			bool bValidRule = true;

			if (section.has("Area"))
			{
				bValidRule = ParseCriticalRuleCondition(sRuleX, section.get("Area"), RuleParser::RuleType::AREA, m_criticalEventRuleSetList);
			}

			if (bValidRule && section.has("Feat"))
			{
				bValidRule = ParseCriticalRuleCondition(sRuleX, section.get("Feat"), RuleParser::RuleType::FEAT, m_criticalEventRuleSetList);
			}

			if (bValidRule && section.has("Extra"))
			{
				bValidRule = ParseCriticalRuleCondition(sRuleX, section.get("Extra"), RuleParser::RuleType::EXTRA, m_criticalEventRuleSetList);
			}

			if (!bValidRule)
			{
				CriticalRuleSet* rsToDel = m_criticalEventRuleSetList.back();
				m_criticalEventRuleSetList.pop_back();
				delete rsToDel;
			}
			else
			{
				logger->Info("* CriticalEvent: [%s] runs %s", sRuleX.c_str(), rs1->sScript.c_str());
			}
		}

		iRuleNumber++;
		sRuleX = sRuleBase + std::to_string(iRuleNumber);
	}
}

void initCriticalRule(std::string nxhome, std::string sFileName)
{
	std::string inifile(nxhome);
	inifile.append("\\");
	inifile.append(sFileName);

	mINI::INIFile file(inifile);
	mINI::INIStructure ini;

	bool bOkRead = file.read(ini);

	if (!bOkRead)
		logger->Err("Cant find CriticalFile : %s", sFileName.c_str());

	if (bOkRead && ini.has("General"))
	{
		if (ini.get("General").has("DisableHook"))
		{
			std::string sDisableHook = ini.get("General").get("DisableHook");

			bOkRead = !(sDisableHook == "1");
		}
	}

	if (bOkRead)
	{
		ParseCriticalRuleSections(ini, "CriticalThreatRule", m_criticalThreatRuleSetList);
		ParseCriticalRuleSections(ini, "CriticalConfirmRule", m_criticalConfirmRuleSetList);
		ParseCriticalRuleSections(ini, "CriticalMultiplierRule", m_criticalMultiplierRuleSetList);
		ParseCriticalRuleSections(ini, "CriticalTouchThreatRule", m_criticalTouchThreatRuleSetList, true);
		ParseCriticalEventRuleSections(ini);
	}


	if (bOkRead && !isCriticalAlreadyPatched)
	{
		isCriticalAlreadyPatched = true;
		int i = 0;
		while (CriticalRulePatches[i].Apply()) {
			i++;
		}
	}
	else if (!bOkRead && isCriticalAlreadyPatched)
	{
		isCriticalAlreadyPatched = false;

		for (int i = (int)(sizeof(_CriticalRulePatches) / sizeof(Patch)) - 2; i >= 0; i--)
			CriticalRulePatches[i].Remove();
	}

	//Touch attacks: patched only with at least one valid rule (none read when the file is not read)
	bool bTouchThreatRules = !m_criticalTouchThreatRuleSetList.empty();

	if (bTouchThreatRules && !isCriticalTouchThreatAlreadyPatched)
	{
		isCriticalTouchThreatAlreadyPatched = true;
		int i = 0;
		while (CriticalTouchThreatPatches[i].Apply()) {
			i++;
		}
	}
	else if (!bTouchThreatRules && isCriticalTouchThreatAlreadyPatched)
	{
		isCriticalTouchThreatAlreadyPatched = false;

		for (int i = (int)(sizeof(_CriticalTouchThreatPatches) / sizeof(Patch)) - 2; i >= 0; i--)
			CriticalTouchThreatPatches[i].Remove();
	}

	//Critical event: patched only with at least one valid rule (none read when the file is not read)
	bool bCriticalEventRules = !m_criticalEventRuleSetList.empty();

	if (bCriticalEventRules && !isCriticalEventAlreadyPatched)
	{
		isCriticalEventAlreadyPatched = true;
		int i = 0;
		while (CriticalEventPatches[i].Apply()) {
			i++;
		}
	}
	else if (!bCriticalEventRules && isCriticalEventAlreadyPatched)
	{
		isCriticalEventAlreadyPatched = false;

		for (int i = (int)(sizeof(_CriticalEventPatches) / sizeof(Patch)) - 2; i >= 0; i--)
			CriticalEventPatches[i].Remove();
	}
}

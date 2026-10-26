#include <misc/log.h>
#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include <misc/Patch.h>

#include <charconv>
#include <cstdint>
#include <string>
#include <string_view>

#include "SneakAttackConditions.h"

extern std::unique_ptr<LogNWNX> logger;

//////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// GetSneakAttackConditions ///////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//Only one source of truth: the engine. Its sneak attack test (0x63cab0) or its death attack test (0x63cd20) is
//called as is. Only two things are added, both active during our call only:
//	- nFeatTest 0: the test of the attacker's sneak attack feats (call 0x59aa90 in 0x63cab0) answers "yes",
//	- the calls of the immunity test 0x5a0ec0 of both functions are watched, to know if an immunity was seen
//	  (so, when the attack is still possible, the engine bypassed it).
//
//TestIfSneakApply? 0x63cab0 (thiscall attacker, target ; ret 4), called by the engine before the attack roll.
//It finds by itself the current attack data of the attacker (round = creature+0xEDC, index = byte round+0x2370,
//0x642fd0 = round + index * 0xB4, no null test), then:
//	- reads atk+0x3C (ranged attack: 10 m max only if == 1) and the word atk+0x5C (0x363 / 0x364 Whirlwind = no sneak),
//	- calls DoIHaveSneakAttack 0x59aa90 (attacker feats),
//	- tests the 3 ways (attacker not seen, target flat-footed, flanked) and the immunity (0x5a0ec0 with the types
//	  0x1E then 0x1F, bypassed by Epic Precision or FeatIgnoreCritImmunity / Spirits Ruin),
//	- writes only 1 in atk+0x40 (never 0) when the sneak attack applies,
//	- on an immunity not bypassed: adds a CNWCCMessageData (integer 0x86) to the list atk+0xA4 (count atk+0xA8).
//It exits at once if the target is NULL.
//ResolveDeathAttack 0x63cd20 (thiscall attacker, target ; ret 4): the same code (compared by disassembly), except
//its feat test, inline (death attack feats, always active), and the flag written: atk+0x44 instead of atk+0x40.
//
//So, around the call, the fields read or written are saved, forced, then restored: no visible side effect on the
//attack data. Risk: a call while an attack of the same attacker is being resolved (script run from the combat)
//uses the same "current" attack data: it is saved / restored, so the attack in progress keeps its values.

namespace {

	//TestIfSneakApply? (thiscall attacker, target ; ret 4)
	constexpr uintptr_t FUNC_TestIfSneakApply		= 0x0063cab0;
	//ResolveDeathAttack (thiscall attacker, target ; ret 4)
	constexpr uintptr_t FUNC_ResolveDeathAttack		= 0x0063cd20;
	//DoIHaveSneakAttack (thiscall attacker stats, no stack parameter ; ret) : returns 0 / 1 in EAX
	//Its only call in the whole binary is the one of 0x63cab0 below
	unsigned long DoIHaveSneakAttackFunc			= 0x0059aa90;
	//In 0x63cab0: mov ecx,ebx (attacker stats) / call 0x59aa90 (e8 7a df f5 ff) / test eax,eax
	constexpr uintptr_t OFFS_SneakTestFeatCall		= 0x0063cb11;
	//Immunity test (thiscall target stats, immunity type, attacker ; ret 8): result in EAX
	unsigned long IsImmuneFunc						= 0x005a0ec0;
	//Its calls in 0x63cab0 (types 0x1E then 0x1F) and in 0x63cd20 (same), all "e8 rel32" then test eax,eax
	constexpr uintptr_t OFFS_SneakImmunityCall1		= 0x0063cc10;	//e8 ab 42 f6 ff
	constexpr uintptr_t OFFS_SneakImmunityCall2		= 0x0063cc28;	//e8 93 42 f6 ff
	constexpr uintptr_t OFFS_DeathImmunityCall1		= 0x0063d047;	//e8 74 3e f6 ff
	constexpr uintptr_t OFFS_DeathImmunityCall2		= 0x0063d05f;	//e8 5c 3e f6 ff
	//Attack data of the round (thiscall round, index ; ret 4): round + index * 0xB4
	constexpr uintptr_t FUNC_GetAttackData			= 0x00642fd0;
	//CNWCCMessageData destructor body (thiscall, no parameter ; ret), the memory is freed after (as TouchAttack 0x690e81)
	constexpr uintptr_t FUNC_MessageDataDestroy		= 0x0074f2a0;
	//Import of NWN2_Heap::Deallocate (cdecl) used by the engine
	constexpr uintptr_t IAT_NWN2HeapDeallocate		= 0x007e65c4;

	constexpr uint32_t CREATURE_STATS				= 0x1FC4;
	constexpr uint32_t CREATURE_COMBATROUND			= 0xEDC;
	constexpr uint32_t COMBATROUND_CURRENTATTACK	= 0x2370;	//byte
	constexpr uint32_t ATK_RANGED					= 0x3C;		//dword, 1 = ranged
	constexpr uint32_t ATK_SNEAK					= 0x40;		//dword, 1 = sneak attack
	constexpr uint32_t ATK_DEATH					= 0x44;		//dword, 1 = death attack
	constexpr uint32_t ATK_SPECIALATTACK			= 0x5C;		//word
	constexpr uint32_t ATK_MESSAGES					= 0xA4;		//array of CNWCCMessageData*: data, count, allocated
	constexpr uint32_t ATK_MESSAGES_COUNT			= 0xA8;

	//nFeatTest (1 = sneak attack test, feats tested by the engine)
	constexpr int FEATTEST_NONE						= 0;	//sneak attack test, feats not tested
	constexpr int FEATTEST_DEATH					= 2;	//death attack test (its feats are always tested)

	//Result mask
	constexpr int SNEAKCOND_POSSIBLE				= 1;
	constexpr int SNEAKCOND_REFUSED_IMMUNE			= 2;
	constexpr int SNEAKCOND_IMMUNITY_BYPASSED		= 4;

	typedef void (__thiscall *AttackTestFn)(void* pAttacker, void* pTarget);
	typedef uint8_t* (__thiscall *GetAttackDataFn)(void* pCombatRound, uint32_t uIndex);
	typedef void (__thiscall *MessageDataDestroyFn)(void* pMessageData);
	typedef void (__cdecl *HeapDeallocateFn)(void* p);

	bool bSneakConditionsPatched = false;

	//True during our call only
	bool bSneakConditionsProbe = false;
	//While true, the call of DoIHaveSneakAttack in 0x63cab0 returns 1 without calling it
	bool bSneakTestBypassFeat = false;
	//Set by the immunity hook during our call when 0x5a0ec0 returned non zero
	bool bSneakImmunitySeen = false;

	//Replaces the target of the call 0x63cab0 -> 0x59aa90. Same convention: ECX = stats, return in EAX.
	//Flag false: jump to the original function (return address, ECX and stack untouched) = transparent.
	__declspec(naked) void SneakTestFeatCallHook()
	{
		__asm
		{
			cmp		byte ptr [bSneakTestBypassFeat], 0
			jne		BypassFeat

			jmp		dword ptr [DoIHaveSneakAttackFunc]

		BypassFeat:
			mov		eax, 0x1
			ret
		}
	}

	//Replaces the target of the 4 calls of 0x5a0ec0 (thiscall, 2 stack parameters, ret 8).
	//Calls the original with the same ECX and parameters, its result goes back untouched in EAX.
	//Only the flags are changed: the 4 callers do test eax,eax just after.
	__declspec(naked) void ImmunityCallHook()
	{
		__asm
		{
			push	dword ptr [esp + 0x8]		//parameter 2 (attacker)
			push	dword ptr [esp + 0x8]		//parameter 1 (immunity type), +4 after the first push
			call	dword ptr [IsImmuneFunc]

			cmp		byte ptr [bSneakConditionsProbe], 0
			je		ImmunityDone
			test	eax, eax
			je		ImmunityDone
			mov		byte ptr [bSneakImmunitySeen], 1

		ImmunityDone:
			ret		0x8
		}
	}

	Patch _SneakConditionsPatches[] =
	{
		Patch(OFFS_SneakTestFeatCall + 1, (relativefunc)SneakTestFeatCallHook),

		Patch(OFFS_SneakImmunityCall1 + 1, (relativefunc)ImmunityCallHook),
		Patch(OFFS_SneakImmunityCall2 + 1, (relativefunc)ImmunityCallHook),
		Patch(OFFS_DeathImmunityCall1 + 1, (relativefunc)ImmunityCallHook),
		Patch(OFFS_DeathImmunityCall2 + 1, (relativefunc)ImmunityCallHook),

		Patch()
	};
	Patch* SneakConditionsPatches = _SneakConditionsPatches;

	//The flags are removed whatever the way out of the call
	struct SneakConditionsProbeGuard
	{
		SneakConditionsProbeGuard(bool bBypassFeat)
		{
			bSneakImmunitySeen = false;
			bSneakTestBypassFeat = bBypassFeat;
			bSneakConditionsProbe = true;
		}
		~SneakConditionsProbeGuard()
		{
			bSneakConditionsProbe = false;
			bSneakTestBypassFeat = false;
		}
	};

	uint8_t* GetCreatureFromId(int objectID)
	{
		GameObject* Object;
		GameObjectManager m_ObjectManager;

		NWN::OBJECTID ObjectId = (NWN::OBJECTID)objectID;
		if ((ObjectId & NWN::INVALIDOBJID) != 0)
			ObjectId &= ~(NWN::LISTTYPE_MASK);

		if ((Object = m_ObjectManager.GetGameObject(ObjectId)) == NULL || Object->GetObjectType() != NWN::OBJECT_TYPE_CREATURE)
			return NULL;

		if (*(void**)((uint8_t*)Object + CREATURE_STATS) == NULL)
			return NULL;

		return (uint8_t*)Object;
	}

} // namespace


void initSneakAttackConditionsPatch()
{
	if (bSneakConditionsPatched)
		return;

	int i = 0;
	while (SneakConditionsPatches[i].Apply()) {
		i++;
	}
	bSneakConditionsPatched = true;
}


//Script: the sneak (or death) attack conditions of the engine between oAttacker and oTarget.
//bRanged: 1 = as a ranged attack (10 m max). nFeatTest: 0 sneak attack without feat test, 1 sneak attack with the
//feat test, 2 death attack (feat test always done by the engine).
//Mask: 1 possible, 2 refused by an immunity not bypassed, 4 immunity seen and bypassed (with 1; never with 2).
//The current attack data of the attacker is used by the engine: everything read or written is saved and restored.
int GetSneakAttackConditions(int attackerID, int targetID, int bRanged, int nFeatTest)
{
	if (!bSneakConditionsPatched)
	{
		logger->Err("EnhancedFeatures_GetInt: SneakAttackConditions not added, please enable it in the ini File with option AddGetSneakAttackConditionsFct");
		return 0;
	}

	if (nFeatTest < FEATTEST_NONE || nFeatTest > FEATTEST_DEATH)
	{
		logger->Err("EnhancedFeatures_GetInt: SneakAttackConditions invalid nFeatTest %d (0, 1 or 2)", nFeatTest);
		return 0;
	}

	uint8_t* pAttacker = GetCreatureFromId(attackerID);
	uint8_t* pTarget = GetCreatureFromId(targetID);
	if (pAttacker == NULL || pTarget == NULL)
		return 0;

	void* pCombatRound = *(void**)(pAttacker + CREATURE_COMBATROUND);
	if (pCombatRound == NULL)
		return 0;

	bool bDeathAttack = (nFeatTest == FEATTEST_DEATH);
	uintptr_t uTestFunc = bDeathAttack ? FUNC_ResolveDeathAttack : FUNC_TestIfSneakApply;
	uint32_t uFlagOffset = bDeathAttack ? ATK_DEATH : ATK_SNEAK;

	//Same attack data as the one 0x63cab0 / 0x63cd20 will find
	uint8_t uIndex = *((uint8_t*)pCombatRound + COMBATROUND_CURRENTATTACK);
	uint8_t* pAtk = ((GetAttackDataFn)FUNC_GetAttackData)(pCombatRound, uIndex);

	//Save
	uint32_t uOldRanged = *(uint32_t*)(pAtk + ATK_RANGED);
	uint32_t uOldFlag = *(uint32_t*)(pAtk + uFlagOffset);
	uint16_t uOldSpecialAttack = *(uint16_t*)(pAtk + ATK_SPECIALATTACK);
	uint32_t uOldMessageCount = *(uint32_t*)(pAtk + ATK_MESSAGES_COUNT);

	//Force: generic attack (not Whirlwind), melee or ranged as asked, no sneak / death attack yet
	*(uint32_t*)(pAtk + ATK_RANGED) = (bRanged != 0) ? 1 : 0;
	*(uint16_t*)(pAtk + ATK_SPECIALATTACK) = 0;
	*(uint32_t*)(pAtk + uFlagOffset) = 0;

	bool bImmunitySeen;
	{
		SneakConditionsProbeGuard probeGuard(nFeatTest == FEATTEST_NONE);
		((AttackTestFn)uTestFunc)(pAttacker, pTarget);
		bImmunitySeen = bSneakImmunitySeen;
	}

	int iResult = 0;
	if (*(uint32_t*)(pAtk + uFlagOffset) == 1)
	{
		iResult |= SNEAKCOND_POSSIBLE;
		//After an immunity, the engine only goes on when it is bypassed (else message 0x86 and exit)
		if (bImmunitySeen)
			iResult |= SNEAKCOND_IMMUNITY_BYPASSED;
	}

	//The engine only adds a message (0x86) to the list on an immunity not bypassed: destroy the added ones
	//(as TouchAttack does in 0x690e81: destructor body then NWN2_Heap::Deallocate)
	uint32_t uNewMessageCount = *(uint32_t*)(pAtk + ATK_MESSAGES_COUNT);
	if (uNewMessageCount > uOldMessageCount)
	{
		iResult |= SNEAKCOND_REFUSED_IMMUNE;

		//Read after the call: the add (0x57df70) may have moved the array
		void** pMessages = *(void***)(pAtk + ATK_MESSAGES);
		HeapDeallocateFn pDeallocate = *(HeapDeallocateFn*)IAT_NWN2HeapDeallocate;
		for (uint32_t i = uOldMessageCount; i < uNewMessageCount; i++)
		{
			void* pMessage = pMessages[i];
			if (pMessage != NULL)
			{
				((MessageDataDestroyFn)FUNC_MessageDataDestroy)(pMessage);
				pDeallocate(pMessage);
			}
			pMessages[i] = NULL;
		}
	}

	//Restore (the array itself, maybe grown, is kept: only its count goes back)
	*(uint32_t*)(pAtk + ATK_MESSAGES_COUNT) = uOldMessageCount;
	*(uint32_t*)(pAtk + ATK_RANGED) = uOldRanged;
	*(uint16_t*)(pAtk + ATK_SPECIALATTACK) = uOldSpecialAttack;
	*(uint32_t*)(pAtk + uFlagOffset) = uOldFlag;

	return iResult;
}


int GetIntSneakAttackConditions(int attackerID, const char* sParams)
{
	//sParams = "target|bRanged|nFeatTest" (or "target|bRanged": nFeatTest = 0)
	std::string_view sView{ sParams };
	int iValues[3] = { 0, 0, 0 };
	bool bOk = true;
	bool bEnd = false;
	int iNbValues = 0;

	while (bOk && !bEnd && iNbValues < 3)
	{
		size_t uEnd = sView.find('|');
		if (uEnd == std::string_view::npos)
		{
			uEnd = sView.size();
			bEnd = true;
		}

		auto [ptr, ec] = std::from_chars(sView.data(), sView.data() + uEnd, iValues[iNbValues]);
		bOk = (ec == std::errc{}) && (ptr == sView.data() + uEnd);
		iNbValues++;

		if (!bEnd)
			sView.remove_prefix(uEnd + 1);
	}

	//2 or 3 values, nothing after
	if (!bOk || !bEnd || iNbValues < 2)
	{
		logger->Err(("EnhancedFeatures_GetInt: SneakAttackConditions invalide '" + std::string(sParams) + "'").c_str());
		return 0;
	}

	return GetSneakAttackConditions(attackerID, iValues[0], iValues[1], iValues[2]);
}

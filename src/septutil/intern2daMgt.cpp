#include "intern2daMgt.h"
#include "../../NWN2Lib/NWN2.h"
#include "../../NWN2Lib/NWN2Common.h"
#include <cstdint>

namespace NWN2DA
{


}

NWN2DA::access2da* g_global2da = NULL;
NWN2DA::classes2DA g_classes2da = NULL;
NWN2DA::feats2DA g_feats2da = NULL;
NWN2DA::skills2DA g_skills2da = NULL;
NWN2DA::racialtypes2DA g_races2da = NULL;
NWN2DA::racialSubtypes2DA g_subraces2da = NULL;
NWN2DA::background2da g_backgrounds2da = NULL;
NWN2DA::domains2da g_domains2da = NULL;
NWN2DA::spells2da g_spells2da = NULL;

void Init2DAStructs() {
	g_global2da = *(NWN2DA::access2da**)(0x86443C);
	g_classes2da = g_global2da->m_pClasses;
	g_feats2da = g_global2da->m_pFeats;
	g_skills2da = g_global2da->m_pSkills;
	g_races2da = g_global2da->m_pRacialTypes;
	g_subraces2da = g_global2da->m_pRacialSubTypes;
	g_backgrounds2da = g_global2da->m_pBackground;
	g_domains2da = g_global2da->m_pDomains;
	g_spells2da = g_global2da->m_pSpellStrct->m_pSpells;
}

NWN2DA::background2da GetBackgroundRow(uint32_t iBackgroundRow)
{
	if (g_backgrounds2da == NULL)
		Init2DAStructs();

	if (iBackgroundRow < g_global2da->m_NumberOfBackground)
	{
		return (g_backgrounds2da + iBackgroundRow);
	}
	return NULL;
}

NWN2DA::domains2da GetDomainRow(uint32_t iDomainRow)
{
	if (g_domains2da == NULL)
		Init2DAStructs();

	if (iDomainRow < g_global2da->m_NumberOfDomains)
	{
		return (g_domains2da + iDomainRow);
	}

	return NULL;
}

NWN2DA::racialtypes2DA GetRacialRow(uint32_t iRaceRow)
{
	if (g_races2da == NULL)
		Init2DAStructs();

	if (iRaceRow < g_global2da->m_NumberOfRacialTypes)
	{
		if (g_races2da[iRaceRow].m_Index == iRaceRow)
		{
			return (g_races2da + iRaceRow);
		}
		else if (g_races2da[iRaceRow].m_Index > iRaceRow)
		{
			uint32_t iIdx = iRaceRow;
			while (iIdx >= 0)
			{
				if(g_races2da[iIdx].m_Index == iRaceRow)
					return (g_races2da + iIdx);
				iIdx--;
			}
		}
	}
	return NULL;
}

NWN2DA::racialSubtypes2DA GetSubRacialRow(uint32_t iSubRaceRow)
{
	if (g_subraces2da == NULL)
		Init2DAStructs();

	if (iSubRaceRow < g_global2da->m_NumberOfRacialSubTypes)
	{
		if (g_subraces2da[iSubRaceRow].m_Index == iSubRaceRow)
		{
			return (g_subraces2da + iSubRaceRow);
		}
		else if (g_subraces2da[iSubRaceRow].m_Index > iSubRaceRow)
		{
			uint32_t iIdx = iSubRaceRow;
			while (iIdx >= 0)
			{
				if(g_subraces2da[iIdx].m_Index == iSubRaceRow)
					return (g_subraces2da + iIdx);
				iIdx--;
			}
		}
	}
	return NULL;
}

NWN2DA::classes2DA GetClasseRow(uint32_t iClassRow)
{
	if (g_classes2da == NULL)
		Init2DAStructs();

	if (iClassRow < g_global2da->m_NumberOfClasses)
	{
		if (g_classes2da[iClassRow].m_ClassRow == iClassRow)
		{
			return (g_classes2da + iClassRow);
		}
		else if (g_classes2da[iClassRow].m_ClassRow > iClassRow)
		{
			uint32_t iIdx = iClassRow;
			while (iIdx >= 0)
			{
				if(g_classes2da[iIdx].m_ClassRow == iClassRow)
					return (g_classes2da + iIdx);
				iIdx--;
			}
		}
	}
	return NULL;
}

NWN2DA::feats2DA GetFeatRow(uint32_t iFeatRow)
{
	if (g_feats2da == NULL)
		Init2DAStructs();

	if (iFeatRow < g_global2da->m_NumberOfFeats)
	{
		if (g_feats2da[iFeatRow].m_featIdx == iFeatRow)
		{
			return (g_feats2da + iFeatRow);
		}
		else if (g_feats2da[iFeatRow].m_featIdx > iFeatRow)
		{
			uint32_t iIdx = iFeatRow;
			while (iIdx >= 0)
			{
				if(g_feats2da[iIdx].m_featIdx == iFeatRow)
					return (g_feats2da + iIdx);
				iIdx--;
			}
		}
	}
	return NULL;
}

NWN2DA::skills2DA GetSkillRow(uint32_t iSkill)
{
	if (g_skills2da == NULL)
		Init2DAStructs();

	if (iSkill < g_global2da->m_NumberOfSkills)
	{
		return (g_skills2da + iSkill);
	}

	return NULL;
}

NWN2DA::spells2da GetSpellRow(uint32_t iSpell)
{
	if (g_spells2da == NULL)
		Init2DAStructs();

	if (iSpell < g_global2da->m_pSpellStrct->m_NumberOfSpells)
	{
		return (g_spells2da + iSpell);
	}

	return NULL;
}

NWN2DA::access2da* GetGlobal2DA()
{
	if (g_global2da == NULL)
		Init2DAStructs();

	return g_global2da;
}


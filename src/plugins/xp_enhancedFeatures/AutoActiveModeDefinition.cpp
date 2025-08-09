
#include "SmallPatchFunctions.h"

#include <misc/Patch.h>
#include <misc/ini.h>
#include <misc/log.h>
#include <plugins/plugin.h>
#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include "../../septutil/NwN2DataPos.h"
#include <unordered_set>

#define OFFS_UpdateMode1_EFF			0x005712A6
#define OFFS_UpdataMode2_EFF			0x00573321

#define OFFS_PrepareSetDetectMode_EFF	0x005f458F
#define OFFS_TestIfDetectMode_EFF		0x005bdc93


extern std::unique_ptr<LogNWNX> logger;


//TODO... OSEF ? On se branche juste sur les fonctions pour le reste ? Vérifier comment ça se comporte en rajoutant/changeant juste pour les autres


	std::unordered_set<uint32_t> g_listAutoDetect;
	std::unordered_set<uint32_t> g_listAutoTrack;
	//

	bool TestIfFeat(int uPcBlock, uint16_t uFeat)
	{
		uint32_t currentNbOfFeats  = *(uint32_t*)(uPcBlock + 0x1C);
		if (currentNbOfFeats != 0) 
		{
			uint16_t* currentFeatsList = *(uint16_t**)(uPcBlock + 0x18);

			return std::binary_search(currentFeatsList, currentFeatsList + currentNbOfFeats, uFeat);
		}

		return false;
	}

	
	uint8_t __fastcall TestNoFeatAutoDetect_EFF(uint8_t* CreaPtr) 
	{
		uint8_t* creaStat = *(uint8_t**)((int)CreaPtr + AmCrtPtrAppBlock);

		for (const auto& iElement : g_listAutoDetect) {
			if (TestIfFeat(creaStat, iElement)) {
				//If we found, just stop and return 0
				return 0;
			}
		}
		return 1;
	}

	uint8_t __fastcall TestDetectMode_EFF(uint8_t* CreaPtr)
	{
		//Then, test if DetectMode Activated
		if (CreaPtr[AmCrtDetectMode] == 1)
			return 1;


		//Then if feat. (no nofeat = feat)
		if (TestNoFeatAutoDetect_EFF(CreaPtr) == 0)
		{
			return 1;
		}

		if (CreaPtr[AmCrtInCombat] == 0 && CreaPtr[0xF1E] == 0)
		{
			return 2;
		}

		return 0;
	}



	unsigned long ReturnPrepareSetDetectMode_EFF = 0x005F4596;

	__declspec(naked) void FixPrepareSetDetectMode_EFF()
	{
		__asm
		{
			MOV EBX, 0x1
			CMP AL, BL
			JNZ GoBackPrepareDetectEFF

			MOV ECX, ESI
			CALL TestNoFeatAutoDetect_EFF

			CMP AL, BL
			GoBackPrepareDetectEFF:

			JMP dword ptr[ReturnPrepareSetDetectMode_EFF]
		}
	}



	unsigned long ReturnTestDetectMode_EFF = 0x005BDCD0;

	__declspec(naked) void CallTestDetectMode_EFF()
	{
		__asm
		{
			CALL TestDetectMode_EFF
		}
	}


	Patch _PatchAutoDetectList[] =
	{
		Patch(OFFS_UpdateMode1_EFF, (char*)"\xe9\x15\x00\x00\x00\x90", (int)6),
		Patch(OFFS_UpdataMode2_EFF, (char*)"\xe9\x15\x00\x00\x00\x90", (int)6),

		Patch(OFFS_PrepareSetDetectMode_EFF, (char*)"\xe9\x00\x00\x00\x00", (int)5),
		Patch(OFFS_PrepareSetDetectMode_EFF + 1, (relativefunc)FixPrepareSetDetectMode_EFF),

		Patch(OFFS_TestIfDetectMode_EFF, (char*)"\xe9\x00\x00\x00\x00\x90\x90", (int)7),
		Patch(OFFS_TestIfDetectMode_EFF + 1, (relativefunc)CallTestDetectMode_EFF),

		Patch()
	};

	Patch* PatchAutoDetectList = _PatchAutoDetectList;



bool AutoActiveModeDef(SimpleIniConfig* config)
{
	std::string sList = "";
	//AutoDetectFeats
	{
		config->Read("AutoDetectFeats", &sList, std::string(""));
		if (sList == "None")
		{
			logger->Info("* Remove all AutoDetect feats (be carrefull 240 can cause problem in client side)");
			g_listAutoDetect.clear();
		}
		else if(sList != "")
		{
			std::string sNewListAutoDetect = " ";
			std::istringstream iss(sList);
			uint32_t number;
		
			while (iss >> number) {
				g_listAutoDetect.insert(number);
				sNewListAutoDetect += std::to_string(number) + " ";
			}
			logger->Info("* New list of AutoDetectFeats :%s", sNewListAutoDetect.c_str());
		}
		else
		{
			g_listAutoDetect.insert(240);
		}


		i = 0;
		while (PatchAutoDetectList[i].Apply())
		{
			i++;
		}

	}

	//AutoTrackFeats
	{
		config->Read("AutoTrackFeats", &sList, std::string(""));
		if (sList == "None")
		{
			logger->Info("* Remove all AutoTrack feats (be carrefull 1393 can cause problem in client side)");
			g_listAutoTrack.clear();
		}
		else if(sList != "")
		{
			std::string sNewListAutoTrack = " ";
			std::istringstream iss(sList);
			uint32_t number;

			while (iss >> number) {
				g_listAutoTrack.insert(number);
				sNewListAutoTrack += std::to_string(number) + " ";
			}
			logger->Info("* New list of AutoTrack Feats :%s", sNewListAutoTrack.c_str());
		}
		else
		{
			g_listAutoTrack.insert(1393);
		}

		i = 0;
		while (PatchAutoTrackList[i].Apply())
		{
			i++;
		}
	}


	return true;
}

#include "appListManagement.h"
#include "aspectManagerUtils.h"
#include "aspectManager.h"
#include <bit>
#include <cassert>
#include <charconv>
#include <optional>
#include <string_view>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <locale>
#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>
#include "../../septutil/NwN2DataPos.h"

//Remove this sCategory for this Appearance
void AppearanceListManagement::removeFromTenue(std::string sRRG, int iPart, std::string sRCat, unsigned int iVar, std::string sCategory) {
	std::vector<std::string>::iterator position = std::find(outfitMap[sRRG][iPart][sRCat][iVar].begin(), outfitMap[sRRG][iPart][sRCat][iVar].end(), sCategory);
	if (position != outfitMap[sRRG][iPart][sRCat][iVar].end()) // == myVector.end() means the element was not found
		outfitMap[sRRG][iPart][sRCat][iVar].erase(position);
}

//Add this sCategory for this Appearance
void AppearanceListManagement::addToTenue(std::string sRRG, int iPart, std::string sRCat, unsigned int iVar, std::string sCategory) {
	std::vector<std::string>::iterator position = std::find(outfitMap[sRRG][iPart][sRCat][iVar].begin(), outfitMap[sRRG][iPart][sRCat][iVar].end(), sCategory);
	//Add only if not already in it
	if (position == outfitMap[sRRG][iPart][sRCat][iVar].end())
		outfitMap[sRRG][iPart][sRCat][iVar].push_back(sCategory);
}

//Add an appearance to the specific VisualCategory
void AppearanceListManagement::addPresentationArmorList(std::string sRRG, int iPart,std::string sCode, std::string sVisualCat) {
	if (categorizedOutfitsMap.count(sRRG) == 0) {
		categorizedOutfitsMap[sRRG];
	}

	if (categorizedOutfitsMap[sRRG].count(iPart) == 0) {
		categorizedOutfitsMap[sRRG][iPart];
	}

	if (categorizedOutfitsMap[sRRG][iPart].count(sVisualCat) == 0) {
		categorizedOutfitsMap[sRRG][iPart][sVisualCat];
	}

	//If not in it, we add
	if (std::find(categorizedOutfitsMap[sRRG][iPart][sVisualCat].begin(), 
			categorizedOutfitsMap[sRRG][iPart][sVisualCat].end(), sCode) == categorizedOutfitsMap[sRRG][iPart][sVisualCat].end()) {
		categorizedOutfitsMap[sRRG][iPart][sVisualCat].push_back(sCode);
	}
}

std::string AppearanceListManagement::CreateCodeApp(std::string sRCat, std::string sVar) {
	std::string sCode = sRCat;
	sCode.append(SEPARATOR_MODEL);
	if (sVar.size() < 3)
		sCode.append("0");
	if (sVar.size() < 2)
		sCode.append("0");
	sCode.append(sVar);

	return sCode;
}

std::string AppearanceListManagement::CreateCodeApp(int iRCat, int iVar) {
	std::string sCode( indexToPrefix[iRCat]);
	sCode.append(SEPARATOR_MODEL);
	if (iVar < 100)
		sCode.append("0");
	if (iVar < 10)
		sCode.append("0");
	sCode.append(std::to_string(iVar));

	return sCode;
}

bool AppearanceListManagement::DeleteCategorie(std::string sRRG, int iPart, std::string sCatToDelete) {
	if (categorizedOutfitsMap.count(sRRG) != 0) {
		if (categorizedOutfitsMap[sRRG].count(iPart) != 0) {
			if (categorizedOutfitsMap[sRRG][iPart].count(sCatToDelete) != 0) {
				if (categorizedOutfitsMap[sRRG][iPart][sCatToDelete].size() == 0) {
					categorizedOutfitsMap[sRRG][iPart].erase(sCatToDelete);
				} else
					return false;
			}
		}
	}
	return true;
}

bool AppearanceListManagement::CreateNewCategorie(std::string sRRG, int iPart, std::string sNewCat) {
	if(categorizedOutfitsMap.count(sRRG) > 0)
	{
		if(categorizedOutfitsMap[sRRG].count(iPart) > 0)
		{
			//ICI, erreur !
			if(categorizedOutfitsMap[sRRG][iPart].count(sNewCat) > 0)
			{
				return false;
			}
			else
			{
				categorizedOutfitsMap[sRRG][iPart][sNewCat]; //.Add(Param3, new List<string>());
			}
		}
	}
	return true;
}


//Remove from category if it has other category
void AppearanceListManagement::removeFromCategory(std::string sRRG, std::string sCode, int iPart, int iRCat, int iVar, std::string sCategory) {
	//is this RRG exist ?
	if (categorizedOutfitsMap.count(sRRG) != 0)
	{
		//if have this armorPart (Helm/gloves/...)
		if (categorizedOutfitsMap[sRRG].count(iPart) != 0)
		{
			//if this VisualCategory exist
			if (categorizedOutfitsMap[sRRG][iPart].count(sCategory) != 0)
			{
				auto lst = categorizedOutfitsMap[sRRG][iPart][sCategory];
				bool found = (std::find(lst.begin(), lst.end(), sCode) != lst.end());
				//Do we have this Appearance here ?
				if (found)
				{
					//sCode is in sCategory, continue
					if (outfitMap.count(sRRG) != 0)
					{
						if (outfitMap[sRRG].count(iPart) != 0)
						{
							int iIndex = sCode.find_last_of(SEPARATOR_MODEL);
							std::string sRCat = sCode.substr(0, iIndex); //Grab the "Real" Category
							if (outfitMap[sRRG][iPart].count(sRCat) != 0)
							{
								if (outfitMap[sRRG][iPart][sRCat].count(iVar) != 0) //Get the Variation (could have been grabbed with sCode?)
								{
									//Are we on several VisualCategory ?
									if (outfitMap[sRRG][iPart][sRCat][iVar].size() > 1)
									{
										//Remove from sCategory
										removeFromTenue(sRRG, iPart, sRCat, iVar, sCategory);
										categorizedOutfitsMap[sRRG][iPart][sCategory].remove(sCode);
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

//Add to the category if not already in it
void AppearanceListManagement::addToCategory(std::string sRRG, std::string sCode, int iPart, int iRCat, int iVar, std::string sCategory) {

	int iIndex = sCode.find_last_of(SEPARATOR_MODEL);
	std::string sRCat = sCode.substr(0, iIndex);

	addPresentationArmorList(sRRG, iPart, sCode, sCategory);
	outfitMap[sRRG];
	outfitMap[sRRG][iPart];
	outfitMap[sRRG][iPart][sRCat];
	outfitMap[sRRG][iPart][sRCat][iVar];
	addToTenue(sRRG, iPart, sRCat, iVar, sCategory);

}






int AppearanceListManagement::GetNumberOfHairForRRG(char *sRRG) {
	if(m_hairs.count(sRRG) != 0)
		return m_hairs[sRRG].size();
	return 0;
}

int AppearanceListManagement::GetNumberOfHeadForRRG(char *sRRG) {
	if(m_heads.count(sRRG) != 0)
		return m_heads[sRRG].size();
	return 0;
}

int AppearanceListManagement::GetHairForRRG(char *sRRG, int iIdx) {
	int iRet = -1;

	if(iIdx < 0)
		return iRet;
	
	if(m_hairs.count(sRRG) != 0) {
		if(m_hairs[sRRG].size() > iIdx) {
			auto it = m_hairs[sRRG].begin();
			std::advance (it,iIdx);
			iRet = (int)*it;
		}
	}

	return iRet;
}

int AppearanceListManagement::GetHeadForRRG(char *sRRG, int iIdx) {
	int iRet = -1;

	if(iIdx < 0)
		return iRet;

	if(m_heads.count(sRRG) != 0) {
		if(m_heads[sRRG].size() > iIdx) {
			auto it = m_heads[sRRG].begin();
			std::advance (it,iIdx);
			iRet = (int)*it;
		}
	}

	return iRet;
}

int AppearanceListManagement::GetNumberOfVisualCat(char* sRRG, int iArmorPart) {
	if (categorizedOutfitsMap.count(sRRG) != 0)
	{
		if (categorizedOutfitsMap[sRRG].count(iArmorPart) != 0)
		{
			return categorizedOutfitsMap[sRRG][iArmorPart].size();
		}
	}

	return 0;
}

int AppearanceListManagement::GetNumberOfArmorVariation(char* cRRG_Cat, int iArmorPart) {
	std::string sRRG_Cat = cRRG_Cat;
	if(sRRG_Cat.size() < 4)
		return 0;
	
	std::string sRRG = sRRG_Cat.substr(0, 3);
	std::string sVCategory = sRRG_Cat.substr(3);

	std::transform(sRRG.begin(), sRRG.end(), sRRG.begin(), ::toupper);

	if (categorizedOutfitsMap.count(sRRG) != 0)
	{
		if (categorizedOutfitsMap[sRRG].count(iArmorPart) != 0)
		{
			if(categorizedOutfitsMap[sRRG][iArmorPart].count(sVCategory) != 0)
				return categorizedOutfitsMap[sRRG][iArmorPart][sVCategory].size();
		}
	}

	return 0;
}

int AppearanceListManagement::GetNumberOfArmorPieceVariation(char* cRRG, int iModelPiece) {
	std::string sRRG = cRRG;
	if (sRRG.size() != 3)
		return 0;

	std::string sPiece = GetModelPieceStr(iModelPiece);

	if (sPiece == "")
		return 0;

	std::transform(sRRG.begin(), sRRG.end(), sRRG.begin(), ::toupper);
	std::transform(sPiece.begin(), sPiece.end(), sPiece.begin(), ::toupper);

	if (accessoriesMap.count(sRRG) != 0) {
		if (accessoriesMap[sRRG].count(sPiece) != 0) {
			return accessoriesMap[sRRG][sPiece].size();
		}
	}

	return 0;
}

int AppearanceListManagement::GetArmorPiece(char* cRRG, int iIdx_Piece) {
	std::string sRRG = cRRG;
	int iRet = -1;
	if (sRRG.size() != 3)
		return -1;

	int iIdx = iIdx_Piece/100;
	int iModelPiece = iIdx_Piece%100;

	std::string sArmorPiece = GetModelPieceStr(iModelPiece);

	if (sArmorPiece == "")
		return -1;

	std::transform(sRRG.begin(), sRRG.end(), sRRG.begin(), ::toupper);
	std::transform(sArmorPiece.begin(), sArmorPiece.end(), sArmorPiece.begin(), ::toupper);

	if (accessoriesMap.count(sRRG) != 0)
	{
		if (accessoriesMap[sRRG].count(sArmorPiece) != 0)
		{
			if (iIdx < accessoriesMap[sRRG][sArmorPiece].size())
			{
				accessoriesMap[sRRG][sArmorPiece].sort();

				auto it = accessoriesMap[sRRG][sArmorPiece].begin();
				std::advance (it,iIdx);
				iRet = (int) * it;
			}
		}
	}
	return iRet;
}

int AppearanceListManagement::GetNumberOfWeaponSubPart(char* cWeaponType) {
	int iRet = 0;
	std::string sWeaponType = cWeaponType;

	std::transform(sWeaponType.begin(), sWeaponType.end(), sWeaponType.begin(), ::toupper);

	if (weaponsMap.count(sWeaponType) != 0)
		iRet = weaponsMap[sWeaponType].size();

	return iRet;
}

int AppearanceListManagement::GetNumberOfVariationForWeaponSubPart(char* cWeapon, int iWpnPart) {
	int iRet = 0;

	std::string sSubPart = "*";
	if (iWpnPart == 1)
		sSubPart = "_B";
	else if (iWpnPart == 2)
		sSubPart = "_C";

	std::string sWeapon = cWeapon;

	std::transform(sWeapon.begin(), sWeapon.end(), sWeapon.begin(), ::toupper);

	if (weaponsMap.count(sWeapon) != 0)
	{
		if (weaponsMap[sWeapon].count(sSubPart) != 0)
		{
			iRet = weaponsMap[sWeapon][sSubPart].size();
		}
		//Specific case of "only one part" we can have "*" or "_A"
		if (sSubPart.compare("*") == 0) 
		{
			if (weaponsMap[sWeapon].count("_A"))
			{
				iRet += weaponsMap[sWeapon]["_A"].size();
			}
		}
	}

	return iRet;
}

int AppearanceListManagement::GetWeaponPartVariation(char* cWeapon, int iIdx_Part) {
	int iRet = -1;

	std::string sWeapon(cWeapon);
	int iIdx = iIdx_Part / 10;
	int iSubPart = iIdx_Part % 10;

	std::string sSubPart = "*";
	if (iSubPart == 1)
		sSubPart = "_B";
	else if (iSubPart == 2)
		sSubPart = "_C";

	std::transform(sWeapon.begin(), sWeapon.end(), sWeapon.begin(), ::toupper);
	std::transform(sSubPart.begin(), sSubPart.end(), sSubPart.begin(), ::toupper);

	if (weaponsMap.count(sWeapon) != 0)
	{
		if (sSubPart.compare("*") == 0)
		{
			bool bTrouve = false;
			if (weaponsMap[sWeapon].count(sSubPart) != 0)
			{
				//This index is, indead, in the "*" category
				if (iIdx < weaponsMap[sWeapon][sSubPart].size())
				{
					bTrouve = true;
				}
				//Not in this category, so it must be further
				else
				{
					iIdx -= weaponsMap[sWeapon][sSubPart].size();
				}
			}
			//Not in "*" category, try in the "_A" one.
			if (!bTrouve)
			{
				if (weaponsMap[sWeapon].count("_A") != 0)
				{
					//We will, indeed, find it in the "_A" category
					if (iIdx < weaponsMap[sWeapon]["_A"].size())
					{
						sSubPart = "_A";
					}
				}
			}
		}

		if (weaponsMap[sWeapon].count(sSubPart) != 0)
		{
			weaponsMap[sWeapon][sSubPart].sort();
			if (iIdx < weaponsMap[sWeapon][sSubPart].size())
			{
				auto it = weaponsMap[sWeapon][sSubPart].begin();
				std::advance(it, iIdx);
				iRet = (int)*it;
			}
		}
	}

	return iRet;
}


void AppearanceListManagement::SetRestrictedCategory(std::string sCategory, bool isRestricted) {
	restrictedCat[sCategory] = isRestricted;
}

int AppearanceListManagement::GetRestrictedCategory(std::string sCategory) {
	int iRet = 0;

	if (restrictedCat.count(sCategory) != 0)
	{
		if (restrictedCat.at(sCategory))
		{
			iRet = 1;
		}
	}

	return iRet;
}




void AppearanceListManagement::RemoveAppearanceFromCategory(char* cRRG_RCat, int iPart_Var, char* cCategory) {
	std::string sRRG_RCat = cRRG_RCat;
	std::string sCategory = cCategory;

	if(sRRG_RCat.size() < 4)
		return;

	std::string sRRG = sRRG_RCat.substr(0, 3);

	sRRG_RCat = sRRG_RCat.substr(3);

	if(!is_number(sRRG_RCat))
		return;

	int iRCat = std::stoul(sRRG_RCat);
	int iPart = iPart_Var / 1000;
	int iVar = iPart_Var % 1000;

	std::transform(sRRG.begin(), sRRG.end(), sRRG.begin(), ::toupper);

	std::string sCode = CreateCodeApp( iRCat, iVar+1);

	removeFromCategory(sRRG, sCode, iPart, iRCat, iVar+1, sCategory);
}

//Remove by sCode
void AppearanceListManagement::RemoveAppearanceFromCategoryC(char* cRRG_sCode, int iPart, char* cCategory) {
	std::string sRRG_sCode = cRRG_sCode;
	std::string sCategory = cCategory;

	if(sRRG_sCode.size() < 4)
		return;

	std::string sRRG = sRRG_sCode.substr(0, 3);
	std::string sCode = sRRG_sCode.substr(3);

	std::size_t iIndex = sCode.find_last_of(SEPARATOR_MODEL);

	if(iIndex == std::string::npos || (iIndex+1) >= sCode.size() )
		return; 
	
	std::string sPrefixRCat = sCode.substr(0, iIndex);
	std::string sVar = sCode.substr(iIndex+1);

	if(prefixToIndex.count(sPrefixRCat) == 0)
		return;

	if(!is_number(sVar))
		return;

	int iRCat = prefixToIndex[sPrefixRCat];
	int iVar = std::stoul(sVar);

	std::transform(sRRG.begin(), sRRG.end(), sRRG.begin(), ::toupper);

	removeFromCategory(sRRG, sCode, iPart, iRCat, iVar, sCategory);
}

void AppearanceListManagement::AddAppearanceToCategory(char* cRRG_RCat, int iPart_Var, char* cVCategory) {
	std::string sRRG_RCat = cRRG_RCat;
	std::string sVCategory = cVCategory;

	if(sRRG_RCat.size() < 4)
		return;

	std::string sRRG = sRRG_RCat.substr(0, 3);
	std::string sRCat = sRRG_RCat.substr(3);

	if(!is_number(sRCat))
		return;

	int iRCat = std::stoul(sRCat);
	int iPart = iPart_Var / 1000;
	int iVar = iPart_Var % 1000;

	std::transform(sRRG.begin(), sRRG.end(), sRRG.begin(), ::toupper);

	std::string sCode = CreateCodeApp( iRCat, iVar+1);

	addToCategory(sRRG, sCode, iPart, iRCat, iVar+1, sVCategory);
}

//Add by sCode
void AppearanceListManagement::AddAppearanceToCategoryC(char* cRRG_sCode, int iPart, char* cCategory) {
	std::string sRRG_sCode = cRRG_sCode;
	std::string sCategory = cCategory;

	if(sRRG_sCode.size() < 4)
		return;

	std::string sRRG = sRRG_sCode.substr(0,3);
	std::string sCode = sRRG_sCode.substr(3);

	std::size_t iIndex = sCode.find_last_of(SEPARATOR_MODEL);


	if(iIndex == std::string::npos || (iIndex+1) >= sCode.size() )
		return; 

	std::string sPrefixRCat = sCode.substr(0, iIndex);
	std::string sVar = sCode.substr(iIndex+1);

	if(prefixToIndex.count(sPrefixRCat) == 0)
		return;

	if(!is_number(sVar))
		return;

	int iRCat = prefixToIndex[sPrefixRCat];
	int iVar = std::stoul(sVar);

	std::transform(sRRG.begin(), sRRG.end(), sRRG.begin(), ::toupper);
	addToCategory(sRRG, sCode, iPart, iRCat, iVar, sCategory);
}



void AppearanceListManagement::CreateNewVisualCategory(char* cRRG, int iPart, char* cVisualCat) {
	if (categorizedOutfitsMap.count(cRRG) != 0)
	{
		if (categorizedOutfitsMap[cRRG].count(iPart) != 0)
		{
			categorizedOutfitsMap[cRRG][iPart][cVisualCat];
		}
	}
}


//Delete VisualType if empty or if every App in it is also in another VisualType.
void AppearanceListManagement::DeleteVisualCategory(char* cRRG, int iPart, char* cVisualType)
{
	bool bCanDelete = true;
	//rrg exist
	if (categorizedOutfitsMap.count(cRRG) != 0)
	{
		//ModelPart exist
		if (categorizedOutfitsMap[cRRG].count(iPart) != 0)
		{
			//VisualType to Delete exist.
			if (categorizedOutfitsMap[cRRG][iPart].count(cVisualType) != 0)
			{
				int iSize = categorizedOutfitsMap[cRRG][iPart][cVisualType].size();

				for (std::string sCode : categorizedOutfitsMap[cRRG][iPart][cVisualType])
				{
					//The sCode is already in the system, we can trust the format
					int iIndex = sCode.find_last_of(SEPARATOR_MODEL);
					std::string sPrefix = sCode.substr(0, iIndex);
					int iVariante = std::stoul(sCode.substr(iIndex + 1));


					if (outfitMap[cRRG][iPart][sPrefix][iVariante].size() < 2)
					{
						bCanDelete = false;
						break;
					}
				}

				if (bCanDelete)
				{
					//for (int i = 0; i < iSize; i++)
					for (std::string sCode : categorizedOutfitsMap[cRRG][iPart][cVisualType])
					{
						//The sCode is already in the system, we can trust the format
						int iIndex = sCode.find_last_of(SEPARATOR_MODEL);
						std::string sPrefix = sCode.substr(0, iIndex);
						int iVariante = std::stoul(sCode.substr(iIndex + 1));

						removeFromTenue(cRRG, iPart, sPrefix, iVariante, cVisualType);
					}

					//Remove the VisualType
					categorizedOutfitsMap[cRRG][iPart][cVisualType].clear();
				}
			}

			if(bCanDelete)
				categorizedOutfitsMap[cRRG][iPart].erase(cVisualType);
		}
	}
}

//Rename the RRG,iPart, VisualCat to  RRG,iPart,cNewName
void AppearanceListManagement::RenameVisualCategory(char* cRRG_VisualCat, int iPart, char* cNewName) {
	std::string sRRG_VCat = cRRG_VisualCat;

	if(sRRG_VCat.size() < 4)
		return;

	std::string sRRG = sRRG_VCat.substr(0,3);
	std::string sVCat = sRRG_VCat.substr(3);

	std::transform(sRRG.begin(), sRRG.end(), sRRG.begin(), ::toupper);
	if (categorizedOutfitsMap.count(sRRG) != 0)
	{
		if (categorizedOutfitsMap[sRRG].count(iPart) != 0)
		{
			if (categorizedOutfitsMap[sRRG][iPart].count(sVCat) != 0)
			{
				//Be sure the new VCategory exist
				categorizedOutfitsMap[sRRG][iPart][cNewName];
				for(std::string sCode : categorizedOutfitsMap[sRRG][iPart][sVCat])
				{
					categorizedOutfitsMap[sRRG][iPart][cNewName].push_back(sCode);
					//Already in the system, we can trust the string format
					int iIndex = sCode.find_last_of(SEPARATOR_MODEL);
					//Just be sure to have everything
					outfitMap[sRRG];
					outfitMap[sRRG][iPart];
					//Add the sCode to the new VCat
					outfitMap[sRRG][iPart][sCode.substr(0, iIndex)];
					outfitMap[sRRG][iPart][sCode.substr(0, iIndex)][std::stoul(sCode.substr(iIndex + 1))];
					//Remove old VCat for sCode
					removeFromTenue(sRRG, iPart, sCode.substr(0, iIndex), std::stoul(sCode.substr(iIndex + 1)), sVCat);
					//Add the new VCat for sCode
					addToTenue(sRRG, iPart, sCode.substr(0, iIndex), std::stoul(sCode.substr(iIndex + 1)), cNewName);
				}

				//Remove the old VCat
				categorizedOutfitsMap[sRRG][iPart][sVCat].clear();
				categorizedOutfitsMap[sRRG][iPart].erase(sVCat);
				//sort the content of the vcat
				categorizedOutfitsMap[sRRG][iPart][cNewName].sort();
			}
		}
	}
}

void AppearanceListManagement::ChangeVisualCategory(char* cRRG_VisualCat, int iPart, char* cCode_NewVisualCat) {
	std::string sRRG_VisualC = cRRG_VisualCat;

	if(sRRG_VisualC.size() < 4)
		return;

	std::string sRRG = sRRG_VisualC.substr(0,3);
	std::string sOldVisualCat = sRRG_VisualC.substr(3);

	std::string sCode_NewVisualCat = cCode_NewVisualCat;
	std::size_t tPos = sCode_NewVisualCat.find_last_of('#');

	if(tPos == std::string::npos || (tPos+1) >= sCode_NewVisualCat.size())
		return;

	std::string sNewVisualCat = sCode_NewVisualCat.substr(tPos+1);
	std::string sCode = sCode_NewVisualCat.substr(tPos);


	std::transform(sRRG.begin(), sRRG.end(), sRRG.begin(), ::toupper);

	//Test the existance
	if (categorizedOutfitsMap.count(sRRG) != 0)
	{
		if(categorizedOutfitsMap[sRRG].count(iPart) != 0)
		{
			if (categorizedOutfitsMap[sRRG][iPart].count(sOldVisualCat) != 0)
			{
				auto lst = categorizedOutfitsMap[sRRG][iPart][sOldVisualCat];
				bool bFound = (std::find(lst.begin(), lst.end(), sCode) != lst.end());
				//Ok, it exist
				if(bFound)
				{
					//Is the newCat exist too ?
					if (categorizedOutfitsMap[sRRG][iPart].count(sNewVisualCat) != 0)
					{
						//Test if it's already in the newCat 
						auto nlst = categorizedOutfitsMap[sRRG][iPart][sNewVisualCat];
						bFound = (std::find(nlst.begin(), nlst.end(), sCode) != nlst.end());
						
						

						//Add the sCode to newCat if not already here
						if(!bFound)
							categorizedOutfitsMap[sRRG][iPart][sNewVisualCat].push_back(sCode);

						//Remove from oldCat
						categorizedOutfitsMap[sRRG][iPart][sOldVisualCat].remove(sCode);

						//The sCode is already in the system, we can trust the format
						int iIndex = sCode.find_last_of(SEPARATOR_MODEL);
						outfitMap[sRRG];
						outfitMap[sRRG][iPart];
						outfitMap[sRRG][iPart][sCode.substr(0,iIndex)];
						outfitMap[sRRG][iPart][sCode.substr(0,iIndex)][std::stoul(sCode.substr(iIndex+1))];

						removeFromTenue(sRRG, iPart, sCode.substr(0, iIndex), std::stoul(sCode.substr(iIndex + 1)), sOldVisualCat);

						addToTenue(sRRG, iPart, sCode.substr(0, iIndex), std::stoul(sCode.substr(iIndex + 1)), sNewVisualCat);

						//No need to sort if already exist before
						if(bFound)
							categorizedOutfitsMap[sRRG][iPart][sNewVisualCat].sort();
					}
				}
			}
		}
	}
}


std::string AppearanceListManagement::GetVisualCategory(char* cRRG, int iPart_Idx) {
	std::string sVat = "";
	int iPart = iPart_Idx / 1000;
	int iIdx = iPart_Idx % 1000;

	std::string sRRG = cRRG;
	std::transform(sRRG.begin(), sRRG.end(), sRRG.begin(), ::toupper);

	if(categorizedOutfitsMap.count(sRRG) != 0)
	{
		if(categorizedOutfitsMap[sRRG].count(iPart) != 0)
		{
			std::list<std::string> keys;
			for(auto kv : categorizedOutfitsMap[sRRG][iPart]) {
				keys.push_back(kv.first);
			}

			if(iIdx < keys.size())
			{
				keys.sort();

				auto it = keys.begin();
				std::advance(it, iIdx);
				sVat = *it;
			}
		}
	}
	return sVat;
}

std::string AppearanceListManagement::GetVariationCode(char* cRRG_VCat, int iPart_Idx) {
	std::string sResult = "";
	std::string sRRG_VCat = cRRG_VCat;
	
	if(sRRG_VCat.size() < 4)
		return sResult;

	std::string sRRG = sRRG_VCat.substr(0, 3);
	std::string sVisualCat = sRRG_VCat.substr(3);

	int iPart = iPart_Idx / 1000;
	int iIdx = iPart_Idx % 1000;

	std::transform(sRRG.begin(), sRRG.end(), sRRG.begin(), ::toupper);

	if(categorizedOutfitsMap.count(sRRG) != 0)
	{
		if(categorizedOutfitsMap[sRRG].count(iPart) != 0)
		{
			if(categorizedOutfitsMap[sRRG][iPart].count(sVisualCat) != 0)
			{
				auto it = categorizedOutfitsMap[sRRG][iPart][sVisualCat].begin();
				std::advance (it,iIdx);
				sResult = *it;
			}
		}
	}

	return sResult;
}


int AppearanceListManagement::GetNumberOfVisualCategory(char *cRRG_Prefix, int iPart_Idx) {
	std::string sRRG_Prefix = cRRG_Prefix;
	std::string sVisualCat = "";

	if(sRRG_Prefix.size() < 4)
		return 0;

	std::string sRRG = sRRG_Prefix.substr(0,3);
	std::string sPrefixIdx = sRRG_Prefix.substr(3);

	
	if(!is_number(sPrefixIdx))
		return 0;

	int iPart = iPart_Idx / 1000;
	int iIdx  = iPart_Idx % 1000;
	iIdx++; //We sort by name, but in the object file, variation is name -1;

	std::transform(sRRG.begin(), sRRG.end(), sRRG.begin(), ::toupper);

	if(outfitMap.count(sRRG) != 0)
	{
		if(outfitMap[sRRG].count(iPart) != 0)
		{
			int iPrefix = std::stoi(sPrefixIdx);

			//Should not occur, just quit if index not know
			if(indexToPrefix.count(iPrefix) == 0)
				return 0;

			std::string sPrefix = indexToPrefix[iPrefix];

			if(outfitMap[sRRG][iPart].count(sPrefix) != 0)
			{
				if(outfitMap[sRRG][iPart][sPrefix].count(iIdx) != 0)
				{
					return outfitMap[sRRG][iPart][sPrefix][iIdx].size();
				}
			}
		}
	}
	return 0;
}

std::string AppearanceListManagement::GetVisualCategoryOfRealAppearance(char* cRRG_Prefix, int iNum_Part_Idx) {
	std::string sRRG_Prefix = cRRG_Prefix;
	std::string sVisualCat = "";

	if(sRRG_Prefix.size() < 4)
		return sVisualCat;

	std::string sRRG = sRRG_Prefix.substr(0,3);
	std::string sPrefixIdx = sRRG_Prefix.substr(3);

	if(!is_number(sPrefixIdx))
		return sVisualCat;

	int iPart = iNum_Part_Idx / 1000;
	int iNumber  = iPart / 1000;
	iPart = iPart % 1000;

	int iIdx = iNum_Part_Idx % 1000;
	iIdx++; //We sort by name, but in the object file, variation is name -1;

	std::transform(sRRG.begin(), sRRG.end(), sRRG.begin(), ::toupper);

	if(outfitMap.count(sRRG) != 0)
	{
		if(outfitMap[sRRG].count(iPart) != 0)
		{
			int iPrefix = std::stoi(sPrefixIdx);

			//Should not occur, just quit if index not know
			if(indexToPrefix.count(iPrefix) == 0)
				return sVisualCat;

			std::string sPrefix = indexToPrefix[iPrefix];

			if(outfitMap[sRRG][iPart].count(sPrefix) != 0)
			{
				if(outfitMap[sRRG][iPart][sPrefix].count(iIdx) != 0)
				{
					if(iNumber < outfitMap[sRRG][iPart][sPrefix][iIdx].size())
					{
						sVisualCat = outfitMap[sRRG][iPart][sPrefix][iIdx].at(iNumber);
					}
				}
			}
		}
	}
	return sVisualCat;
}

//Get the number of VisualCategory, with possibly some non filled stuff
std::list<std::string> AppearanceListManagement::GetFullListOfVisualCategory(std::string sRace, std::string sGender, int iPart)
{
	std::list<std::string> m_currentList;
	for (auto rrg : categorizedOutfitsMap) {
		bool isOk = false;
		if (sGender != "") {
			std::string sLastPart = "";
			sLastPart += (rrg.first.back());
			isOk = (sLastPart == sGender);
			if (!isOk) { continue; }
		}

		if (sRace != "") {
			isOk = (rrg.first.rfind(sRace, 0) == 0);
			if (!isOk) { continue; }
		}

		for (auto part : categorizedOutfitsMap[rrg.first])
		{
			if (iPart != -1) {
				if (iPart != part.first) { continue; }
			}

			for (auto rangement : categorizedOutfitsMap[rrg.first][part.first]) {
				m_currentList.push_back(rangement.first);
			}
		}
	}

	m_currentList.sort();
	m_currentList.unique();

	return m_currentList;
}

int AppearanceListManagement::GetFullNumberOfVisualCategory(std::string sRace, std::string sGender, int iPart)
{
	return GetFullListOfVisualCategory(sRace, sGender, iPart).size();
}

//Delete VisualType if empty or if every App in it is also in another VisualType.
int AppearanceListManagement::DeleteExtendedVisualCategory(std::string sRace, std::string sGender, int iPart, std::string sVisualType)
{
	for (auto rrg : categorizedOutfitsMap) {
		bool isOk = false;
		if (sGender != "") {
			std::string sLastPart = "";
			sLastPart += (rrg.first.back());
			isOk = (sLastPart == sGender);
			if (!isOk) { continue; }
		}

		if (sRace != "") {
			isOk = (rrg.first.rfind(sRace, 0) == 0);
			if (!isOk) { continue; }
		}

		for (auto part : categorizedOutfitsMap[rrg.first])
		{
			if (iPart != -1) {
				if (iPart != part.first) { continue; }
			}

			if (categorizedOutfitsMap[rrg.first][part.first].count(sVisualType) != 0)
			{
				for (std::string sCode : categorizedOutfitsMap[rrg.first][part.first][sVisualType])
				{
					//The sCode is already in the system, we can trust the format
					int iIndex = sCode.find_last_of(SEPARATOR_MODEL);
					std::string sPrefix = sCode.substr(0, iIndex);
					int iVariante = std::stoul(sCode.substr(iIndex + 1));


					if (outfitMap[rrg.first][part.first][sPrefix][iVariante].size() < 2)
					{
						//Cant delete. So leave ! 
						return 0;
					}
				}
			}
		}
	}

	//Here, we have see that we can delete, so just rescan eveyrthing and delete.
	for (auto rrg : categorizedOutfitsMap) {
		bool isOk = false;
		if (sGender != "") {
			std::string sLastPart = "";
			sLastPart += (rrg.first.back());
			isOk = (sLastPart == sGender);
			if (!isOk) { continue; }
		}

		if (sRace != "") {
			isOk = (rrg.first.rfind(sRace, 0) == 0);
			if (!isOk) { continue; }
		}

		for (auto part : categorizedOutfitsMap[rrg.first])
		{
			if (iPart != -1) {
				if (iPart != part.first) { continue; }
			}

			if (categorizedOutfitsMap[rrg.first][part.first].count(sVisualType) != 0)
			{
				int iSize = categorizedOutfitsMap[rrg.first][part.first][sVisualType].size();

				for (std::string sCode : categorizedOutfitsMap[rrg.first][part.first][sVisualType])
				{
					//The sCode is already in the system, we can trust the format
					int iIndex = sCode.find_last_of(SEPARATOR_MODEL);
					std::string sPrefix = sCode.substr(0, iIndex);
					int iVariante = std::stoul(sCode.substr(iIndex + 1));

					removeFromTenue(rrg.first, part.first, sPrefix, iVariante, sVisualType);
				}

				//Remove the VisualType
				categorizedOutfitsMap[rrg.first][part.first][sVisualType].clear();
			}

			categorizedOutfitsMap[rrg.first][part.first].erase(sVisualType);
		}
	}

	return 1;
}


void AppearanceListManagement::ExtendedCopyVisualToAnother(std::string sRace, std::string sGender, int iPart, std::string sFromVisual, std::string toVisual)
{
	for (auto rrg : categorizedOutfitsMap) {
		bool isOk = false;
		if (sGender != "") {
			std::string sLastPart = "";
			sLastPart += (rrg.first.back());
			isOk = (sLastPart == sGender);
			if (!isOk) { continue; }
		}

		if (sRace != "") {
			isOk = (rrg.first.rfind(sRace, 0) == 0);
			if (!isOk) { continue; }
		}

		for (auto part : categorizedOutfitsMap[rrg.first])
		{
			if (iPart != -1) {
				if (iPart != part.first) { continue; }
			}

			if (categorizedOutfitsMap[rrg.first][part.first].count(sFromVisual) != 0)
			{
				for (std::string sCode : categorizedOutfitsMap[rrg.first][part.first][sFromVisual])
				{
					//The sCode is already in the system, we can trust the format
					int iIndex = sCode.find_last_of(SEPARATOR_MODEL);
					std::string sPrefix = sCode.substr(0, iIndex);
					int iVariante = std::stoul(sCode.substr(iIndex + 1));

					if(prefixToIndex.count(sPrefix) == 0)
						continue;

					addToCategory(rrg.first, sCode, part.first, prefixToIndex[sPrefix], iVariante, toVisual);
				}
			}
		}
	}
}


std::string AppearanceListManagement::GetIndexOfFullListVisualCategory(std::string sRace, std::string sGender, int iPart, int iIdx)
{
	std::list<std::string> m_list = GetFullListOfVisualCategory(sRace, sGender, iPart);
	
	if (iIdx < 0 || iIdx >= m_list.size())
		return "";
	else
	{
		auto it = m_list.begin();
		std::advance(it, iIdx);
		return *it;
	}
}

void AppearanceListManagement::ListSystemSetInt(std::string sCommand, char* sParam1, int nParam2, int nValue) {
	//RestrictedCategory Stuff
	if (sCommand == "SetRestrictedCategory")
		return SetRestrictedCategory(sParam1, nValue != 0);
}

int AppearanceListManagement::ListSystemGetInt(std::string sCommand, char* sParam1, int nParam2) {
	int retValue = -1;
	if(sCommand == "NumHairVarByRRG")
		return GetNumberOfHairForRRG(sParam1);
	else if(sCommand == "NumHeadVarByRRG")
		return GetNumberOfHeadForRRG(sParam1);
	else if(sCommand == "IndexOfHairByRRG")
		return GetHairForRRG(sParam1, nParam2);
	else if(sCommand == "IndexOfHeadByRRG")
		return GetHeadForRRG(sParam1, nParam2);

	else if(sCommand == "NumOfVATByRRGPart")
		return GetNumberOfVisualCat(sParam1, nParam2);
	else if(sCommand == "NumOfVARByRRGPartVAT")
		return GetNumberOfArmorVariation(sParam1, nParam2);

	else if(sCommand == "NumberOfPartByRRGPart")
		return GetNumberOfArmorPieceVariation(sParam1, nParam2);
	else if(sCommand == "PartByRRGPartIdx")
		return GetArmorPiece(sParam1, nParam2);

	else if(sCommand == "NumbWeaponSubPbyW")
		return GetNumberOfWeaponSubPart(sParam1);
	else if (sCommand == "NumOfWPartbyWSP")
		return GetNumberOfVariationForWeaponSubPart(sParam1, nParam2);
	else if (sCommand == "WPartByWSPIdx")
		return GetWeaponPartVariation(sParam1, nParam2);

	else if(sCommand == "NumberOfVisualCat")
		return GetNumberOfVisualCategory(sParam1, nParam2);
	else if (sCommand == "ExtendedNumberOfVisualCat") {
		std::string str(sParam1);
		std::string s1, s2;
		size_t pos = str.find_last_of('#');
		if (pos != std::string::npos) {
			s1 = str.substr(0, pos);
			if((pos+1) > str.size())
				s2 = str.substr(pos + 1);
		} else {
			s1 = str;
		}
		return GetFullNumberOfVisualCategory(s1, s2, nParam2);
	}


	else if (sCommand == "GetRestrictedCategory")
		return GetRestrictedCategory(sParam1);
	

	else if (sCommand == "ScanFinish") {
		return iScanFinish;
	}
	else if (sCommand == "SaveFile") {
		if (generateSaveFile())
			return 1;
		else
			return 0;
	}
	else if (sCommand == "LoadFile") {
		if (isSaveFileExist())
		{
			//Need to clean EVERYTHING
			m_hairs.clear();
			m_heads.clear();
			accessoriesMap.clear();
			weaponsMap.clear();
			outfitMap.clear();
			categorizedOutfitsMap.clear();
			restrictedCat.clear();

			//Reload from savefile
			return processSaveFile();
		}
		return 0;
	}
	return -1;
}

void AppearanceListManagement::ListSystemSetString(std::string sCommand, char* sParam1, int nParam2, char* sValue) {
	
	if(sCommand == "RemoveFromCategory")
		RemoveAppearanceFromCategory(sParam1, nParam2, sValue);
	else if(sCommand == "RemoveFromCategoryC")
		RemoveAppearanceFromCategoryC(sParam1, nParam2, sValue);

	else if(sCommand == "AddToCategory")
		AddAppearanceToCategory(sParam1, nParam2, sValue);
	else if (sCommand == "AddToCategoryC")
		AddAppearanceToCategoryC(sParam1, nParam2, sValue);

	else if (sCommand == "CreateNewCat")
		CreateNewVisualCategory(sParam1, nParam2, sValue);
	else if (sCommand == "DeleteCat")
		DeleteVisualCategory(sParam1, nParam2, sValue);
	else if (sCommand == "RenameCat")
		RenameVisualCategory(sParam1, nParam2, sValue);
	else if (sCommand == "ChangeCat")
		ChangeVisualCategory(sParam1, nParam2, sValue);
	else if(sCommand == "ExtendedDeleteCat") {
		std::string str(sParam1);
		std::string sToDel(sValue);
		std::string s1, s2;
		size_t pos = str.find_last_of('#');
		if (pos != std::string::npos) {
			s1 = str.substr(0, pos);
			s2 = str.substr(pos + 1); //At worst it will be str.size, so empty string
		} else {
			s1 = str;
		}

		DeleteExtendedVisualCategory(s1, s2, nParam2, sToDel);
	}
	else if (sCommand == "ExtendedCopyCat") {
		std::string str(sParam1);
		std::string sDest(sValue);
		std::string sRace, sGender, sOrigin, s2;
		size_t pos = str.find_first_of('#');
		if (pos != std::string::npos) {
			sRace = str.substr(0, pos);
			s2 = str.substr(pos + 1);
			pos = s2.find_first_of('#');
			if (pos != std::string::npos)
			{
				sGender = s2.substr(0, pos);
				sOrigin = s2.substr(pos + 1);
				ExtendedCopyVisualToAnother(sRace, sGender, nParam2, sOrigin, sDest);
			}
		}
	}
}

std::string AppearanceListManagement::ListSystemGetString(std::string sCommand, char* sParam1, int nParam2)
{
	if(sCommand == "VatByRRGPartIdx")
		return GetVisualCategory(sParam1, nParam2);
	else if(sCommand == "VarByRRGPartVatIdx")
		return GetVariationCode(sParam1, nParam2);
	else if(sCommand == "RealToVisualCat")
		return GetVisualCategoryOfRealAppearance(sParam1, nParam2);
	else if (sCommand == "ExtendedGetVisualCategory") {
		std::string str(sParam1);
		std::string s1, s2;
		size_t pos = str.find_last_of('#');
		if (pos != std::string::npos) {
			s1 = str.substr(0, pos);
			s2 = str.substr(pos + 1);
		} else {
			s1 = str;
		}
		int iPart = nParam2 % 10;
		if (iPart == 9)
			iPart = -1;
		int iIdx = nParam2 / 10;
		return GetIndexOfFullListVisualCategory(s1, s2, iPart, iIdx);
	}

	return "";
}





//TODO 
void AppearanceListManagement::analyser2da()
{
	/*
	int __thiscall CVirtualMachine::RunScript(CExoString * ScriptName, unsigned long ObjectSelf, const CExoArrayList<CScriptParameterWrapper> * Parameters, BOOL ObjectSelfValid, CVirtualMachine::PARAMETER_VALIDATION ParameterValidation);
	// int CVirtualMachine_RunScript(CExoString * ScriptName, 0, const CExoArrayList<CScriptParameterWrapper> * Parameters, TRUE, 3);
	int iMax = GetNum2DARows("armorvisualdata");
	std::string value = "";

	for(int i=0; i< iMax; i++)
	{
	value = Get2DAString("armorvisualdata", "Prefix", i);
	if(value != "")
	{
	prefixToIndex.insert(value, i);
	indexToPrefix.insert(i, value);
	}
	}*/
}

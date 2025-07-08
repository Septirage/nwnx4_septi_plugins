#include "appListManagement.h"
#include "aspectManagerUtils.h"
#include "aspectManager.h"
#include "messageManagement.h"
#include "../../NWN2Lib/NWN2.h"
#include "../../NWN2Lib/NWN2Common.h"
#include "..\..\misc\Patch.h"
#include <bit>
#include <cassert>
#include <charconv>
#include <detours/detours.h>
#include <optional>
#include <string_view>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <locale>
#include <string>
#include "../../septutil/NwN2DataPos.h"


unsigned int buffToInteger(unsigned char * buffer)
{

	unsigned int a = buffer[3];
	unsigned int c;
	a = a << 24;

	c = buffer[2];
	c = c << 16;
	a = a + c;

	c = buffer[1];
	c = c << 8;
	a = a + c;

	c = buffer[0];
	a = a + c;

	/*
	unsigned int b = static_cast<unsigned int>((buffer[0]) << 24 |
	(buffer[1]) << 16 | 
	(buffer[2]) << 8 | 
	(buffer[3])); */
	return a;
}


using convert_t = std::codecvt_utf8<wchar_t>;

std::wstring_convert<convert_t, wchar_t> strconverter;

std::string to_string(std::wstring wstr)
{
	return strconverter.to_bytes(wstr);
}

std::wstring to_wstring(std::string str)
{
	return strconverter.from_bytes(str);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////  Functions to Scan and analyse files (hak & zip) //////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



bool AppearanceListManagement::generateSaveFile()
{
	// Delete the file if it exists.	
	std::ofstream fileBuffer;
	fileBuffer.open(strconverter.from_bytes(pathFileSave), std::ios::out | std::ios::trunc);
	if (fileBuffer.is_open())
	{
		fileBuffer << "!" << SAVEFILE_VERSION << std::endl;

		//"Armor" Models
		fileBuffer << "#!" << "ArmorModel" << std::endl;
		for (auto rrg : categorizedOutfitsMap)
		{
			for (auto part : categorizedOutfitsMap[rrg.first])
			{
				for (auto rangement : categorizedOutfitsMap[rrg.first][part.first])
				{
					for (std::string code : categorizedOutfitsMap[rrg.first][part.first][rangement.first])
					{
						fileBuffer << rrg.first + "@" + std::to_string(part.first) + "@" + code + "#" + rangement.first << std::endl;
					}
				}
			}
		}
		for (auto part : restrictedCat)
		{
			if (part.second)
			{
				fileBuffer << "#" + part.first << std::endl;
			}
		}

		//Accessory Models
		fileBuffer << "#!" << "Accessory" << std::endl;

		for (auto rrg : accessoriesMap) {
			for (auto type : accessoriesMap[rrg.first]) {
				fileBuffer << rrg.first + "#" + type.first + "#";
				bool bFirst = true;
				for (auto iVar : accessoriesMap[rrg.first][type.first]) {
					if (bFirst)
						bFirst = false;
					else
						fileBuffer << ",";

					fileBuffer << iVar;
				}

				fileBuffer << std::endl;
			}
		}


		
		// Item Models
		fileBuffer << "#!" << "Item" << std::endl;

		for (auto type : weaponsMap) {
			for (auto part : weaponsMap[type.first]) {
				fileBuffer << type.first + "#" + part.first + "#";
				bool bFirst = true;
				for (auto iVar : weaponsMap[type.first][part.first]) {
					if (bFirst)
						bFirst = false;
					else
						fileBuffer << ",";

					fileBuffer << iVar;
				}

				fileBuffer << std::endl;
			}
		}

		// Hair Models
		fileBuffer << "#!" << "Hair" << std::endl;

		
		for (auto rrg : m_hairs) {
			fileBuffer << rrg.first + "#";
			
			bool bFirst = true;
			for (auto iVar : m_hairs[rrg.first]) {
				if (bFirst)
					bFirst = false;
				else
					fileBuffer << ",";
				fileBuffer << iVar;
			}

			fileBuffer << std::endl;
		}


		// Head Models
		fileBuffer << "#!" << "Head" << std::endl;

		for (auto rrg : m_heads) {
			fileBuffer << rrg.first + "#";
			bool bFirst = true;
			for (auto iVar : m_heads[rrg.first]) {
				if (bFirst)
					bFirst = false;
				else
					fileBuffer << ",";
				fileBuffer << iVar;
			}

			fileBuffer << std::endl;
		}
		

		fileBuffer.close();
	}
	else
	{
		logger->Err("Error to open the save file");
		return false;
	}

	return true;
}

//Just test if we have an already existing savefile
bool AppearanceListManagement::isSaveFileExist()
{
	bool bVal = true;
	if (pathFileSave != "")
	{
		std::ifstream fileBuffer;
		fileBuffer.open(strconverter.from_bytes(pathFileSave), std::ios::in);
		if (!fileBuffer.is_open())
		{
			bVal = false;
		}
		fileBuffer.close();
	}
	return bVal;
}

//Read the ini containing the hak & zip to be scanned
void
AppearanceListManagement::analyseSpecificIni()
{
	std::ifstream fileBuffer;
	fileBuffer.open(strconverter.from_bytes(pathListFile), std::ios::in);
	if (!fileBuffer.is_open())
	{
		std::string log = "Failed to open the file "+pathListFile+" containing the list of files to be scanned.";
		logger->Err(log.c_str());
		return;
	}
	fileBuffer.seekg (0, std::ios::beg);
	std::string line;
	while (getline(fileBuffer, line))
	{
		//line starting by # are commentary
		if(line.size() >= 1 && line[0] != '#')
		{
			//List the files to scan
			std::string extension = line.substr(line.size()-4);
			std::transform(extension.begin(), extension.end(), extension.begin(), ::toupper);
			if(extension == ".HAK")
			{
				listHak.push_back(line);
			}
			else if(extension == ".ZIP")
			{
				listZip.push_back(line);
			}
			else
			{
				std::string log = "Error durring the parse of " + pathListFile + " with the line:"+line;
				logger->Err(log.c_str());
			}
		}
	}
	fileBuffer.close();
}

//Init the whole list system
void AppearanceListManagement::Initialisation()
{
	analyser2da();
	if (bScanFile)
	{
		analyseSpecificIni();
	}

	isNewFile = !isSaveFileExist();

	processSaveFile();
	if (bScanFile)
	{
		processHak();
		processZip();
		if (!isNewFile)
		{
			//Clean & Inform
			//We have some file removed from last time.
			if (bScanFile && armorFileList.size() > 0)
			{
				logger->Info("############# Manage Models Initialisation #############");
				logger->Info("######### Files previously saved but not found #########");
				for (std::string file : armorFileList)
				{
					logger->Info(file.c_str());

					std::vector<std::string> portion;
					tokenize(file, "@", portion);
					if (portion.size() == 3)
					{
						if (categorizedOutfitsMap.count(portion[0]) > 0)
						{
							int iPortion = std::stoi(portion[1]);
							if (categorizedOutfitsMap[portion[0]].count(iPortion) > 0)
							{
								std::string sRangement = "";
								for (auto rangement : categorizedOutfitsMap[portion[0]][iPortion])
								{
									std::list<std::string> listCA = categorizedOutfitsMap[portion[0]][iPortion][rangement.first];
									if (std::find(listCA.begin(), listCA.end(), portion[2]) != listCA.end())
									{
										sRangement = rangement.first;
										listCA.remove(file);
										std::string log = "In : " + rangement.first;
										logger->Info(log.c_str());
										break;
									}
								}
								if (sRangement != "")
								{
									if (categorizedOutfitsMap[portion[0]][iPortion][sRangement].size() == 0)
									{
										categorizedOutfitsMap[portion[0]][iPortion].erase(sRangement);
										if (categorizedOutfitsMap[portion[0]][iPortion].size() == 0)
										{
											categorizedOutfitsMap[portion[0]].erase(iPortion);
											if (categorizedOutfitsMap[portion[0]].size() == 0)
											{
												categorizedOutfitsMap.erase(portion[0]);
											}
										}
									}
								}
							}
						}
					}
				}
				logger->Info("########################################################");
			}
		}

		generateSaveFile();
	}

	{
		//Sort everything !
		for (auto rrgG : categorizedOutfitsMap)
		{
			std::string rrg = rrgG.first;
			for (auto iPartG : categorizedOutfitsMap[rrg])
			{
				int iPart = iPartG.first;
				for (auto rangementG : categorizedOutfitsMap[rrg][iPart])
				{
					std::string rangement = rangementG.first;
					categorizedOutfitsMap[rrg][iPart][rangement].sort();
					if (!isNewFile)
					{
						for (std::string code : categorizedOutfitsMap[rrg][iPart][rangement])
						{
							std::size_t iIndex = code.find_last_of(SEPARATOR_MODEL);
							//outfitMap[rrg][iPart][code.substr(0, iIndex)][std::stoul(code.substr(iIndex + 1))].push_back(rangement) //= rangement; kriagki
							addToTenue(rrg, iPart, code.substr(0, iIndex), std::stoul(code.substr(iIndex + 1)), rangement);

						}
					}
				}
			}
		}
	}
	iScanFinish = 1;
}

//only needed on the function bellow, don't bother to put it int .h
enum ScanPart {
	ARMORMODELS_PART = 1,
	ACCESSORY_PART	= 2,
	ITEM_PART		= 3,
	HEAD_PART		= 4,
	HAIR_PART		= 5,

	INVALID_PART	= -1

};

//Load the data in the saveFile
bool AppearanceListManagement::processSaveFile()
{
	bool bRet = true;
	std::string log;
	if(isNewFile)
		return true;
	bool error = false;
	ScanPart eCurrentPart = INVALID_PART;
	if (pathFileSave == "")
	{
		bRet = bScanFile;
		error = true;
		log = "No save file defined";
	}
	else
	{
		std::ifstream fileBuffer;
		fileBuffer.open(strconverter.from_bytes(pathFileSave), std::ios::in);
		if (fileBuffer.is_open())
		{
			fileBuffer.seekg (0, std::ios::beg);
			std::string line;

			//Check the version (allow me to patch old version)
			bool bFirstLine = true;
			bool bfirstVersion = true;
			int uOriginVersion = 0;

			while (getline(fileBuffer, line) && !error)
			{
				if (bFirstLine)
				{
					bFirstLine = false;
					//For now, don't care about real version.
					if (line.size() > 1 && line.substr(0, 1) == "!")
					{
						bfirstVersion = false;
						std::string versionLine = line.substr(1);

						char *pEnd = NULL;
						uOriginVersion = std::strtol(versionLine.c_str(), &pEnd, 10);
						if (*pEnd) // error was detected
						{
							std::string sVersionError = "Error with version number for (" + line + ") continue as if were version 1.";
							logger->Err(sVersionError.c_str());
							uOriginVersion = 1;
						}
						continue;
					}
					else
					{
						eCurrentPart = ARMORMODELS_PART;
					}
				}

				//Head/Hair/Accessory
				if (!bfirstVersion && line.size() > 2 && line.substr(0, 2) == "#!")
				{
					line = line.substr(2);
					if (line == "ArmorModel")
						eCurrentPart = ARMORMODELS_PART;
					else if (line == "Accessory")
						eCurrentPart = ACCESSORY_PART;
					else if (line == "Item")
						eCurrentPart = ITEM_PART;
					else if (line == "Hair")
						eCurrentPart = HAIR_PART;
					else if (line == "Head")
						eCurrentPart = HEAD_PART;
					else
						eCurrentPart = INVALID_PART;
				} 
				else if (eCurrentPart == ARMORMODELS_PART)
				{
					if (line.size() > 1 && line.substr(0, 1) == "#")
					{
						restrictedCat[line.substr(1)] = true;
					}
					else
					{
						std::vector<std::string> subs;
						tokenize(line, "#", subs);
						if (subs.size() >= 2) {
							std::vector<std::string> portion;
							std::string contenu = subs[0];
							tokenize(contenu, "@", portion);
							if (portion.size() == 3) {

								if (uOriginVersion < 2)
								{
									//We are before version 2 of the file. We store HHM instead of P_HHM. Fix it.
									portion[0] = "P_" + portion[0];
								}

								int iModelPart = std::stoi(portion[1]);

								// This file was a first version file
								// patch the iModelPart
								if (bfirstVersion) {
									iModelPart = convertOldToNewModelPart(iModelPart);
									size_t idxSep = portion[2].find_last_of(SEPARATOR_MODEL);
									std::string sBefore = portion[2].substr(0, idxSep);
									std::string sAfter = portion[2].substr(idxSep + 1);
									if (sAfter.size() < 3)
									{
										sAfter = "0" + sAfter;
									}
									portion[2] = sBefore + SEPARATOR_MODEL + sAfter;
								}
								armorFileList.push_back(portion[0] + "@"
								                        + std::to_string(iModelPart) + "@"
								                        + portion[2]);
								addPresentationArmorList(portion[0], iModelPart, portion[2],
								                         subs[1]);
								size_t idxSep = portion[2].find_last_of(SEPARATOR_MODEL);

								for (int s = 1; s < subs.size(); s++) {
									addToTenue(portion[0], iModelPart, portion[2].substr(0, idxSep),
									           std::stoul(portion[2].substr(idxSep + 1)), subs[s]);
								}
							} else {
								error = true;
								log   = "Error on the line " + line;
							}
						} else {
							error = true;
							log   = "Error on the line " + line;
						}
					}
				}
				else if (eCurrentPart == ITEM_PART || eCurrentPart == ACCESSORY_PART)
				{
					std::vector<std::string> subs;
					tokenize(line, "#", subs);
					if (subs.size() == 3) {

						std::vector<std::string> listVar;
						std::string content = subs[2];
						tokenize(content, ",", listVar);

						for (int s = 0; s < listVar.size(); s++) {
							if (eCurrentPart == ACCESSORY_PART)
							{
								//TODO accessoryFileList
								accessoryPart(subs[0], subs[1], listVar[s]);
							}
							else
							{
								// TODO WeaponFileList
								weapons(subs[0], subs[1], listVar[s]);
							}
						}					
					}
				}
				else if (eCurrentPart == HEAD_PART || eCurrentPart == HAIR_PART)
				{
					std::vector<std::string> subs;
					tokenize(line, "#", subs);
					if (subs.size() == 2) {
						if (uOriginVersion < 2)
						{
							//We are before version 2 of the file. We store HHM instead of P_HHM. Fix it.
							subs[0] = "P_" + subs[0];
						}
						std::vector<std::string> listVar;
						std::string content = subs[1];
						tokenize(content, ",", listVar);

						std::string sType;
						if (eCurrentPart == HEAD_PART)
							sType = "HEAD";
						else
							sType = "HAIR";

						for (int s = 0; s < listVar.size(); s++) {
							bodyPart(subs[0], sType, listVar[s]);
						}
					}
				}
			}
			fileBuffer.close();
		}
		else
		{
			bRet = bScanFile;
			error = true;
			log = "Can't open the save file " + pathFileSave;
		}
	}
	if(error)
	{
		logger->Err(log.c_str());
	}
	return bRet;
}

//Scan every hak listed and put valid model in list
void
AppearanceListManagement::processHak()
{
	for (std::string hakPath : listHak) {

		// try
		//{
		std::string log = "Start the parse of ";
		log             = log + hakPath;
		logger->Trace(log.c_str());

		std::ifstream fileBuffer;
		fileBuffer.open(strconverter.from_bytes(hakPath), std::ios::in | std::ios::binary);
		if (fileBuffer.is_open()) {
			fileBuffer.seekg (0, std::ios::beg);
			unsigned int nombre;
			int offset;
			{
				char tag[8];
				fileBuffer.read(tag, 8);
				// verif tag ?

				fileBuffer.seekg(16);


				unsigned char ccode[4];
				fileBuffer.read((char*)ccode, 4);
				nombre = buffToInteger(ccode);

				fileBuffer.seekg(24);

				fileBuffer.read((char*)ccode, 4);
				offset = buffToInteger(ccode);
			}
			std::string log = "Number of file : " + std::to_string(nombre);
			logger->Trace(log.c_str());

			// loop on the files
			int i = 0;
			for (i = 0; i < nombre; i++) {
				fileBuffer.seekg(offset + (i * 40));

				char elemCour[40];
				fileBuffer.read((char*)elemCour, 40);
				// seulement MDB
				unsigned char* val = (unsigned char*)(elemCour + 36);
				if ( val[0] == 0xA0 && val[1] == 0x0F ) {
					std::string nom(elemCour, 32);
					std::transform(nom.begin(), nom.end(), nom.begin(), ::toupper);
					parseFileName(nom);
				}
			}
			fileBuffer.close();
		} else {
			std::string log = "This file does not exist :" + hakPath;
			logger->Info(log.c_str());
			continue;
		}
		{
			std::string log = "End of the parse of " + hakPath;
			logger->Trace(log.c_str());
		}
		//}
	}
}

//Scan every zip listed and put valid model in list
void
AppearanceListManagement::processZip()
{
	std::string log;
	for (const std::string& zipPath : listZip) {
		log = "Start the parse of " + zipPath;
		logger->Trace(log.c_str());
		analyseZip(zipPath);
		log = "End of the parse of " + zipPath;
		logger->Trace(log.c_str());
	}
}

//Analyse and list valid model in zip
void
AppearanceListManagement::analyseZip(std::string zipPath)
{
	std::list<std::string> listeNom;
	std::ifstream fileBuffer;
	fileBuffer.open(strconverter.from_bytes(zipPath), std::ios::in | std::ios::binary);
	if (fileBuffer.is_open()) {
		fileBuffer.seekg (0, std::ios::beg);
		bool stop = false;
		while (!stop) {
			unsigned char ccode[4];
			fileBuffer.read((char*)ccode, 4);
			unsigned int icode = buffToInteger(ccode);
			if (icode != 0x04034B50) {
				std::string log = "end of file header";
				logger->Trace(log.c_str());
				break;
			}
			// verif 0x504B0304 ou 0x04034B50 ?
			fileBuffer.seekg(14, std::ios_base ::cur);

			fileBuffer.read((char*)ccode, 4);
			unsigned int iTailleFichier = buffToInteger(ccode);

			fileBuffer.seekg(4, std::ios_base ::cur);

			ccode[0] = 0;
			ccode[1] = 0;
			ccode[2] = 0;
			ccode[3] = 0;
			fileBuffer.read((char*)ccode, 2);
			unsigned int iTailleNom = buffToInteger(ccode);

			ccode[0] = 0;
			ccode[1] = 0;
			ccode[2] = 0;
			ccode[3] = 0;
			fileBuffer.read((char*)ccode, 2);
			unsigned int iTailleExtra = buffToInteger(ccode);

			char *leNom = new char[iTailleNom+1];
			fileBuffer.read(leNom, iTailleNom);
			leNom[iTailleNom] = '\0';
			std::string nom(leNom);
			delete[] leNom;
			if (std::find(listeNom.begin(), listeNom.end(), nom) == listeNom.end())
				listeNom.push_back(nom);
			else {
				std::string log = "File duplicate: " + nom;
				logger->Info(log.c_str());
				break;
			}

			fileBuffer.seekg(iTailleExtra + iTailleFichier, std::ios_base ::cur);
		}
		fileBuffer.close();
		// On a notre liste, on traite.
		for (std::string& pathFile : listeNom) {
			if (pathFile.ends_with(".MDB") || pathFile.ends_with(".mdb")) {
				std::string nom    = pathFile;
				nom                = nom.erase(nom.size() - 4, 4);
				std::size_t icoupe = nom.find_last_of('/');
				if (icoupe != std::string::npos)
					nom = nom.erase(0, icoupe + 1);
				icoupe = nom.find_last_of('\\');
				if (icoupe != std::string::npos)
					nom = nom.erase(0, icoupe + 1);

				std::transform(nom.begin(), nom.end(), nom.begin(), ::toupper);
				parseFileName(nom);
			}
		}
	} else {
		std::string log = "This file does not exist :" + zipPath;
		logger->Err(log.c_str());
		return;
	}
}

void
AppearanceListManagement::parseFileName(std::string nom)
{
	// Equipment type
	//std::string pattern = "p_aaf_acme_helm01";
	std::transform(nom.begin(), nom.end(), nom.begin(), ::toupper);

	//Armor Part
	const std::regex regArmor("^(.+)_("+prefixRegexMask+")_(HELM|BODY|BOOTS|GLOVES|CLOAK|BELT)([0-9]+)");
	std::smatch mArmor;
	if (std::regex_search(nom, mArmor, regArmor)) {
		//No prefix, testing more this file is a no-sense
		if (prefixRegexMask == "")
			return;
		/*
		mArmor[1]; //the racial code
		mArmor[2]; //armor type code
		mArmor[3]; //object type
		mArmor[4]; //number
		*/

		//If not a type we want (should already be managed by regex)
		if (prefixToIndex.count(mArmor[2]) == 0)
			return;
		std::string sCode = prefixToVisual[mArmor[2]];

		// Place the invisible part in a specific restricted VisualType, just to make sure it won't be forgotten by Admin
		if (nom == "P_HHM_CL_BODY99" || nom == "P_HHM_CL_BOOTS99" ||
			nom == "P_HHM_CL_GLOVES99") 
		{
			sCode = "_Invisible";
			restrictedCat[sCode] = true;
		}
		int ret = armorPart(mArmor[1], mArmor[2], mArmor[3], mArmor[4]);
		if (ret == 0) {
			//New file, we add.
			if (isNewFile) {
				std::string arPartLst = CreateCodeApp(mArmor[2], mArmor[4]);
				/*
				std::string arPartLst(m[2]);
				arPartLst.append(SEPARATOR_MODEL);
				arPartLst.append(m[4]);
				*/
				addPresentationArmorList(
					mArmor[1], getArmorPartCode(mArmor[3]), arPartLst, sCode);
				addToTenue(mArmor[1], getArmorPartCode(mArmor[3]), mArmor[2], std::stoul(mArmor[4]), sCode);
				//outfitMap[m[1]][getArmorPartCode(m[3])][m[2]][std::stoul(m[4])].push_back(sCode); // = sCode; kriagki
			} else {
				// Else... Juste verify if we already have it.
				std::string sNom = std::string(mArmor[1]) + "@" + std::to_string(getArmorPartCode(mArmor[3])) + "@" + std::string(mArmor[2]) + SEPARATOR_MODEL + std::string(mArmor[4]);
				std::string sNom2 = std::string(mArmor[1]) + "@" + std::to_string(getArmorPartCode(mArmor[3])) + "@" + CreateCodeApp(mArmor[2], mArmor[4]);
				if (std::find(armorFileList.begin(), armorFileList.end(), sNom) !=
					armorFileList.end()) {
					armorFileList.remove(sNom);
					std::string log = "Finded " + sNom;
					logger->Trace(log.c_str());
				} else if (std::find(armorFileList.begin(), armorFileList.end(), sNom2) !=
					armorFileList.end()) {
					armorFileList.remove(sNom);
					std::string log = "Finded " + sNom;
					logger->Trace(log.c_str());
				} else {
					//std::string(m[2]) + SEPARATOR_MODEL + std::string(m[4])
					addPresentationArmorList(
						mArmor[1], getArmorPartCode(mArmor[3]), CreateCodeApp(mArmor[2], mArmor[4]), sCode);
					addToTenue(mArmor[1], getArmorPartCode(mArmor[3]), mArmor[2], std::stoul(mArmor[4]), sCode);
					//outfitMap[m[1]][getArmorPartCode(m[3])][m[2]][std::stoul(m[4])].push_back(sCode); //= sCode; //Kriagki
				}
			}
		}
		if (ret == -2) {
			if (isNewFile)
			{
				std::string log = "File duplication " + nom;
				logger->Info(log.c_str());
			}
			else
			{
				std::string sNom = std::string(mArmor[1]) + "@" + std::to_string(getArmorPartCode(mArmor[3])) + "@" + std::string(mArmor[2]) + SEPARATOR_MODEL + std::string(mArmor[4]);
				std::string sNom2 = std::string(mArmor[1]) + "@" + std::to_string(getArmorPartCode(mArmor[3])) + "@" + CreateCodeApp(mArmor[2], mArmor[4]);

				if (std::find(armorFileList.begin(), armorFileList.end(), sNom) !=
					armorFileList.end()) {
					armorFileList.remove(sNom);
					std::string log = "Finded " + sNom;
					logger->Trace(log.c_str());
				}
				else if (std::find(armorFileList.begin(), armorFileList.end(), sNom2) !=
					armorFileList.end()) {
					armorFileList.remove(sNom2);
					std::string log = "Finded " + sNom2;
					logger->Trace(log.c_str());
				}
			}
		} else if (ret == -1) {
			std::string log = "Problem with the file " + nom;
			logger->Info(log.c_str());
		}
	}
	else {
		//Head/Hair:
		const std::regex regHeadHair("^(.+)_(HEAD|HAIR)([0-9]+)");
		std::smatch mHeadHair;
		if (std::regex_search(nom, mHeadHair, regHeadHair)) {
			/*
				mHeadHair.Groups[1].Value; //racial code
				mHeadHair.Groups[2].Value; //HAIR/HEAD
				mHeadHair.Groups[3].Value; //number
			*/

			bodyPart(mHeadHair[1], mHeadHair[2], mHeadHair[3]);
		}
		else {
			//Accessory
			const std::regex regAccessory("^A_(.+)_(LSHOULDER|RSHOULDER|LBRACER|RBRACER|LELBOW|RELBOW|LUPARM|RUPARM|LHIP|RHIP|FHIP|BHIP|LUPLEG|RUPLEG|LLOWLEG|RLOWLEG|LKNEE|RKNEE|LFOOT|RFOOT|LANKLE|RANKLE)([0-9]+)");
			std::smatch mAccessory;
			if (std::regex_search(nom, mAccessory, regAccessory)) { /*
				mAccessory.Groups[1].Value; //racial code
				mAccessory.Groups[2].Value; //accessory type
				mAccessory.Groups[3].Value; //number */
				accessoryPart(mAccessory[1], mAccessory[2], mAccessory[3]);
			}
			else {
				// weaponsMap
				const std::regex r4("^W_((SHE_)?([^0-9]+))([0-9]+)(_([A,B,C]))?");
				std::smatch m4;
				if (std::regex_search(nom, m4, r4)) {
					std::string sousPartie = "*";
					if (std::string(m4[5]).size() > 0)
						sousPartie = m4[5];

					/*
					m4.Groups[1].Value; //weapon type
					m4.Groups[2].Value; //accessory type
					m4.Groups[3].Value; //number*/
					std::string mPartMixed = "W_";
					mPartMixed += m4[1];

					weapons(mPartMixed, sousPartie, m4[4]);
				}
				else {
					//item 
					const std::regex r5("^I_([^0-9]+)([0-9]+)(_([A,B,C]))?");
					std::smatch m5;
					if (std::regex_search(nom, m5, r5)) {
						std::string sousPartie = "*";
						if (std::string(m5[3]).size() > 0)
							sousPartie = m5[3];

						std::string mPartMixed = "I_";
						mPartMixed += m5[1];
						weapons(mPartMixed, sousPartie, m5[2]);
					}
				}
			}
		}
	}
}

int
AppearanceListManagement::armorPart(std::string race, std::string type, std::string part, std::string number)
{
	int iPart = getArmorPartCode(part);

	std::string log = std::to_string(iPart);

	if (iPart == -1) {
		std::string log = "Error : " + race + "," + type + "," + part + "," + number;
		logger->Err(log.c_str());
		return -1;
	}

	if (outfitMap.count(race) == 0) {
		outfitMap[race];
	}
	if (outfitMap[race].count(iPart) == 0) {
		outfitMap[race][iPart];
	}
	if (outfitMap[race][iPart].count(type) == 0) {
		outfitMap[race][iPart][type];
	}
	unsigned int nombre = std::stoul(number);
	if (outfitMap[race][iPart][type].count(nombre) == 0) {
		outfitMap[race][iPart][type][nombre];// = ""; kriagki
	} else
		return -2;
	return 0;
}

void
AppearanceListManagement::accessoryPart(std::string race, std::string type, std::string number)
{
	if (accessoriesMap.count(race) == 0) {
		accessoriesMap[race];
	}
	if (accessoriesMap[race].count(type) == 0) {
		accessoriesMap[race][type];
	}
	unsigned int nombre = std::stoul(number);
	if (std::find(accessoriesMap[race][type].begin(), accessoriesMap[race][type].end(), nombre) ==
		accessoriesMap[race][type].end())
		accessoriesMap[race][type].push_back(nombre);
}

void
AppearanceListManagement::weapons(std::string type, std::string sousPartie, std::string number)
{
	if (weaponsMap.count(type) == 0) {
		weaponsMap[type];
	}
	if (weaponsMap[type].count(sousPartie) == 0) {
		weaponsMap[type][sousPartie];
	}
	unsigned int nombre = std::stoul(number);
	if (std::find(weaponsMap[type][sousPartie].begin(), weaponsMap[type][sousPartie].end(), nombre) == weaponsMap[type][sousPartie].end())
		weaponsMap[type][sousPartie].push_back(nombre);
}

void
AppearanceListManagement::bodyPart(std::string race, std::string type, std::string number)
{
	if (type == "HAIR") {
		if (m_hairs.count(race) == 0) {
			m_hairs[race];
		}
		unsigned int nombre = std::stoul(number);

		if (std::find(m_hairs[race].begin(), m_hairs[race].end(), nombre) == m_hairs[race].end())
			m_hairs[race].push_back(nombre);
	} else {
		if (m_heads.count(race) == 0) {
			m_heads[race];
		}
		unsigned int nombre = std::stoul(number);
		if (std::find(m_heads[race].begin(), m_heads[race].end(), nombre) == m_heads[race].end())
			m_heads[race].push_back(nombre);
	}
}

int
AppearanceListManagement::getArmorPartCode(std::string part)
{
	int iPart = -1;
	if (part == "HELM")
		iPart = ARMOR_MODEL_PART_HELM;
	if (part == "BODY") // Chest
		iPart = ARMOR_MODEL_PART_ARMOR;
	if (part == "BOOTS") // boot
		iPart = ARMOR_MODEL_PART_BOOTS;
	if (part == "GLOVES") // gloves
		iPart = ARMOR_MODEL_PART_GLOVES;
	if (part == "CLOAK") // cloak
		iPart = ARMOR_MODEL_PART_CLOAK;
	if (part == "BELT") // belt
		iPart = ARMOR_MODEL_PART_BELT;

	return iPart;
}


int
AppearanceListManagement::convertOldToNewModelPart(int iOldModelPartID)
{
	int iPart = -1;
	if (iOldModelPartID == 0)	//Helm
		iPart = ARMOR_MODEL_PART_HELM;
	if (iOldModelPartID == 1) // Chest
		iPart = ARMOR_MODEL_PART_ARMOR;
	if (iOldModelPartID == 2) // boot
		iPart = ARMOR_MODEL_PART_BOOTS;
	if (iOldModelPartID == 3) // gloves
		iPart = ARMOR_MODEL_PART_GLOVES;
	if (iOldModelPartID == 6) // cloak
		iPart = ARMOR_MODEL_PART_CLOAK;
	if (iOldModelPartID == 10) // belt
		iPart = ARMOR_MODEL_PART_BELT;

	return iPart;
}


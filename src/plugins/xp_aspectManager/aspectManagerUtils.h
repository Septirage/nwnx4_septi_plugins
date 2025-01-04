#if !defined(SEPT_ASPECTMANAGERUTILS)
#define SEPT_ASPECTMANAGERUTILS

#include <string>
#include <vector>

#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>


const std::string cst_ModelPiece = "ModelPiece";
const std::string cst_ModelPart = "ModelPart";
const std::string cst_DmgRedct = "DmgRedct";

enum MODEL_PART
{
	ARMOR_MODEL_PART_HELM		= 0x00,
	ARMOR_MODEL_PART_GLOVES		= 0x01,
	ARMOR_MODEL_PART_BOOTS		= 0x02,
	ARMOR_MODEL_PART_BELT		= 0x03,
	ARMOR_MODEL_PART_CLOAK		= 0x04,
	ARMOR_MODEL_PART_ARMOR		= 0x05,
};

enum INVENTORY_SLOT
{
	INVENTORY_SLOT_HEAD			=	0x00,
	INVENTORY_SLOT_CHEST		=	0x01,
	INVENTORY_SLOT_BOOTS		=	0x02,
	INVENTORY_SLOT_ARMS			=	0x03,
	INVENTORY_SLOT_RIGHTHAND	=	0x04,
	INVENTORY_SLOT_LEFTHAND		=	0x05,
	INVENTORY_SLOT_CLOAK		=	0x06,

	INVENTORY_SLOT_BELT			=	0x0A,
};


enum MODEL_PIECE
{
	ARMOR_MODEL_PIECE_LEFT_SHOULDER = 11,
	ARMOR_MODEL_PIECE_RIGHT_SHOULDER = 12,
	ARMOR_MODEL_PIECE_LEFT_BRACER = 13,
	ARMOR_MODEL_PIECE_RIGHT_BRACER = 14,
	ARMOR_MODEL_PIECE_LEFT_ELBOW = 15,
	ARMOR_MODEL_PIECE_RIGHT_ELBOW = 16,
	ARMOR_MODEL_PIECE_LEFT_ARM = 17,
	ARMOR_MODEL_PIECE_RIGHT_ARM = 18,
	ARMOR_MODEL_PIECE_LEFT_HIP = 19,
	ARMOR_MODEL_PIECE_RIGHT_HIP = 20,
	ARMOR_MODEL_PIECE_FRONT_HIP = 21,
	ARMOR_MODEL_PIECE_BACK_HIP = 22,
	ARMOR_MODEL_PIECE_LEFT_LEG = 23,
	ARMOR_MODEL_PIECE_RIGHT_LEG = 24,
	ARMOR_MODEL_PIECE_LEFT_SHIN = 25,
	ARMOR_MODEL_PIECE_RIGHT_SHIN = 26,
	ARMOR_MODEL_PIECE_LEFT_KNEE = 27,
	ARMOR_MODEL_PIECE_RIGHT_KNEE = 28,
	ARMOR_MODEL_PIECE_LEFT_FOOT = 29,
	ARMOR_MODEL_PIECE_RIGHT_FOOT = 30,
	ARMOR_MODEL_PIECE_LEFT_ANKLE = 31,
	ARMOR_MODEL_PIECE_RIGHT_ANKLE = 32,
};

enum DAYNIGHT_CYCLE
{
	DAYNIGHT_SUNRISE = 0,
	DAYNIGHT_DAYTIME = 1,
	DAYNIGHT_SUNSET = 2,
	DAYNIGHT_MOONRISE = 3,
	DAYNIGHT_NIGHTTIME = 4,
	DAYNIGHT_MOONSET = 5,
	DAYNIGHT_TRANSITION = 6,
	DAYNIGHT_DEFAULT = 7,
};




	static void tokenize(std::string const& str, const char* delim,	std::vector<std::string>& out)
	{
		char* token = strtok(const_cast<char*>(str.c_str()), delim);
		while (token != nullptr)
		{
			out.push_back(std::string(token));
			token = strtok(nullptr, delim);
		}
	};

	char* creatureApparenceBlock(char* pCrea);

	std::string ExoStringToString(NWN::CExoString* myExoString);
	void PutNWNExoString(char* pointedExoStr, char* sValue);

	std::string ColorToString(NWN::D3DXCOLOR* myColor);
	bool StringToColor(const char* StringColor, NWN::D3DXCOLOR* myColor);

	bool is_number(std::string s);


	std::string getDescription(char* ptr);
	void setDescription(char* ptr, char* sValue);

	std::string GetModelPieceStr(int iModelPiece);


#endif

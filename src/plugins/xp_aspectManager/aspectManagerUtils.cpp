#include "aspectManagerUtils.h"
#include "../../septutil/NwN2DataPos.h"

#include "nwn2heap.h"
#include <NWN2Lib/NWN2.h>
#include <NWN2Lib/NWN2Common.h>

	typedef void(__cdecl* NWN2Heap_Deallocate_Proc)(void* p);
	extern NWN2Heap_Deallocate_Proc NWN2Heap_Deallocate;

//NWNExoString Utilities

std::string ExoStringToString(NWN::CExoString* myExoString) {
	if(myExoString != NULL) {
		if(myExoString->m_sString != NULL) {
			if(myExoString->m_sString) {
				std::string result(myExoString->m_sString, myExoString->m_nBufferLength);
				return result;
			}
		}
	}
	return "";
}

void PutNWNExoString(char* pointedExoStr, char* sValue)
{
	if (pointedExoStr != NULL)
	{
		NWN::CExoString* OutStr = (NWN::CExoString*)pointedExoStr;
		NWN::CExoString CpyStr;
		CpyStr.m_nBufferLength = OutStr->m_nBufferLength;
		CpyStr.m_sString = OutStr->m_sString;

		unsigned long length = strlen(sValue)+1;
		NWN2_HeapMgr *pHeapMgr = NWN2_HeapMgr::Instance();
		NWN2_Heap *pHeap = pHeapMgr->GetDefaultHeap();
		char* strWanted = (char*)pHeap->Allocate(length+1);
		sprintf_s(strWanted, length, sValue);
		OutStr->m_sString = strWanted;
		OutStr->m_nBufferLength = length;

		if (CpyStr.m_sString != NULL)
		{

			if (NWN2Heap_Deallocate)
				NWN2Heap_Deallocate( CpyStr.m_sString );
			//FreeNwn2Heap( OutStr->m_sString );
			CpyStr.m_sString = NULL;
		}
	}
}



// Color Utilities
std::string ColorToString(NWN::D3DXCOLOR* myColor) {
	unsigned long   ColorData;
	uint8_t Data;
	char StringColor[9];


		ColorData = 0;
		Data = (uint8_t)(myColor->r * 255.0);
		ColorData = Data;
		ColorData = (ColorData << 8) & 0xFF00;
		Data = (uint8_t)(myColor->g * 255.0);
		ColorData |= Data;
		ColorData = (ColorData << 8) & 0xFFFF00;
		Data = (uint8_t)(myColor->b * 255.0);
		ColorData |= Data;
		ColorData = (ColorData << 8) & 0xFFFFFF00;
		Data = (uint8_t)(myColor->a * 255.0);
		ColorData |= Data;

	sprintf_s(
		StringColor,
		9,
		"%08X",
		ColorData );
	
	std::string sTxt(StringColor);
	sTxt = "#" + sTxt;

	return sTxt;
}

bool StringToColor(const char * cValues, NWN::D3DXCOLOR* myColor) {

    std::string sValues = cValues; 
    
	//Just be sure we have an #RGBA string
    if(sValues.size() == 7)
        sValues += "FF";
    
    if(sValues.size() != 9)
        return false;

	uint32_t   ColorData;
	int             Scanned;
	uint8_t Data;

	Scanned = sscanf_s(
		sValues.c_str(),
		"#%08X",
		&ColorData );

	if (Scanned != 1)
		return false;

	Data = (ColorData >> 24) & 0xFF;
	myColor->r = (float) (((float) Data) * (1.0/255.0));
	Data = (ColorData >> 16) & 0xFF;
	myColor->g = (float) (((float) Data) * (1.0/255.0));
	Data = (ColorData >> 8) & 0xFF;
	myColor->b = (float) (((float) Data) * (1.0/255.0));
	Data = (ColorData) & 0xFF;
	myColor->a = (float) (((float) Data) * (1.0/255.0));

	return true;
}



char* creatureApparenceBlock(char* iCrea)
{
	return *((char**)(iCrea + AmCrtPtrAppBlock));
}


bool is_number(std::string s)
{
    if (!s.empty())
    {
        if (s[0] == '-') 
        {
			s = s.substr(1);
        }
    }
	return !s.empty()
	    && std::find_if(s.begin(), s.end(), [](unsigned char c) { return !std::isdigit(c); })
	    == s.end();
}




std::string getDescription(char *ptr)
{
	if (ptr == NULL)
		return "";
	char *retValue = *(char**)ptr;
	std::string strRetValue = "";
	if(retValue != NULL && *(ptr+8) != 0)
	{
		retValue = *(char**)retValue;
		retValue = (retValue + 0x4);

		NWN::CExoString* OutStr = (NWN::CExoString*)retValue;
		if (OutStr->m_sString != NULL)
		{
			if (OutStr->m_sString)
			{
				strRetValue = OutStr->m_sString;
				strRetValue = "s" + strRetValue;
			}
		}
	}
	else
	{
		uint32_t indexVal = *(uint32_t*)(ptr - 4);
		strRetValue = std::to_string(indexVal);
		strRetValue = "i" + strRetValue;
	}

	return strRetValue;
}

void setDescription(char* ptr, char* sValue)
{
	if (ptr == NULL || sValue == NULL)
		return;
	char *retValue = *(char**)ptr;
	if (retValue == NULL)
	{
		NWN2_HeapMgr *pHeapMgr = NWN2_HeapMgr::Instance();
		NWN2_Heap *pHeap = pHeapMgr->GetDefaultHeap();
		char* strExoWanted = (char*)pHeap->Allocate(0xC);
		*(uint32_t*)strExoWanted = 0xFFFFFFFE;
		*(uint32_t*)(strExoWanted + 4) = 0;
		*(uint32_t*)(strExoWanted + 8) = 0;
		char* myPtr = (char*)pHeap->Allocate(0x32);
		*(char**)myPtr = (strExoWanted);
		*(char**)ptr = myPtr;
		*(uint32_t*)(ptr + 4) = 0x10;
		*(uint32_t*)(ptr + 8) = 0x1;
		*(uint32_t*)(ptr - 4) = 0xFFFFFFFF;
		retValue = myPtr;
	}

	{
		retValue = *(char**)retValue;
		retValue = retValue + 0x4;
		PutNWNExoString(retValue, sValue);
	}
}


std::string GetModelPieceStr(int iModelPiece)
{
	std::string sResult = "";
	switch (iModelPiece) 
	{
		case ARMOR_MODEL_PIECE_LEFT_SHOULDER:
			sResult = "LShoulder";
			break;
		case ARMOR_MODEL_PIECE_RIGHT_SHOULDER:
			sResult = "RShoulder";
			break;
		case ARMOR_MODEL_PIECE_LEFT_BRACER:
			sResult = "LBracer";
			break;
		case ARMOR_MODEL_PIECE_RIGHT_BRACER:
			sResult = "RBracer";
			break;
		case ARMOR_MODEL_PIECE_LEFT_ELBOW:
			sResult = "LElbow";
			break;
		case ARMOR_MODEL_PIECE_RIGHT_ELBOW:
			sResult = "RElbow";
			break;
		case ARMOR_MODEL_PIECE_LEFT_ARM:
			sResult = "LUpArm";
			break;
		case ARMOR_MODEL_PIECE_RIGHT_ARM:
			sResult = "RUpArm";
			break;

		case ARMOR_MODEL_PIECE_LEFT_HIP:
			sResult = "LHip";
			break;
		case ARMOR_MODEL_PIECE_RIGHT_HIP:
			sResult = "RHip";
			break;
		case ARMOR_MODEL_PIECE_FRONT_HIP:
			sResult = "FHip";
			break;
		case ARMOR_MODEL_PIECE_BACK_HIP:
			sResult = "BHip";
			break;

		case ARMOR_MODEL_PIECE_LEFT_LEG:
			sResult = "LUpLeg";
			break;
		case ARMOR_MODEL_PIECE_RIGHT_LEG:
			sResult = "RUpLeg";
			break;

		case ARMOR_MODEL_PIECE_LEFT_SHIN:
			sResult = "LLowLeg";
			break;
		case ARMOR_MODEL_PIECE_RIGHT_SHIN:
			sResult = "RLowLeg";
			break;

		case ARMOR_MODEL_PIECE_LEFT_KNEE:
			sResult = "LKnee";
			break;
		case ARMOR_MODEL_PIECE_RIGHT_KNEE:
			sResult = "RKnee";
			break;

		case ARMOR_MODEL_PIECE_LEFT_FOOT:
			sResult = "LFoot";
			break;
		case ARMOR_MODEL_PIECE_RIGHT_FOOT:
			sResult = "RFoot";
			break;

		case ARMOR_MODEL_PIECE_LEFT_ANKLE:
			sResult = "LAnkle";
			break;
		case ARMOR_MODEL_PIECE_RIGHT_ANKLE:
			sResult = "RAnkle";
			break;
	}

	return sResult;
}


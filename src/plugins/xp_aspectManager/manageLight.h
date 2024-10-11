#if !defined(SEPT_MANAGELIGHT)
#define SEPT_MANAGELIGHT

int LightGetInt(char* sCommand, int iObjectID);
void LightSetInt(char* sCommand, int iObjectID, int iValue);

float LightGetFloat(char* sCommand, int iObjectID);
void LightSetFloat(char* sCommand, int iObjectID, float fValue);

std::string LightGetString(char* sCommand, int iObjectID);
void LightSetString(char* sCommand, int iObjectID, char* sValue);



#endif
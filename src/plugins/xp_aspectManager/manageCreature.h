#if !defined(SEPT_MANAGECREATURE)
#define SEPT_MANAGECREATURE

int CreatureGetInt(char* sCommand, int iObjectID);
void CreatureSetInt(char* sCommand, int iObjectID, int iValue);

float CreatureGetFloat(char* sCommand, int iObjectID);
void CreatureSetFloat(char* sCommand, int iObjectID, float fValue);

void CreatureSetString(char* sCommand, int iObjectID, char* sValue);
std::string CreatureGetString(char* sCommand, int iObjectID);


#endif
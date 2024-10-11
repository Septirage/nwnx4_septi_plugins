#if !defined(SEPT_MANAGEOBJECT)
#define SEPT_MANAGEOBJECT

int ObjectGetInt(char* sCommand, int iObjectID);
void ObjectSetInt(char* sCommand, int iObjectID, int iValue);

float ObjectGetFloat(char* sCommand, int iObjectID);
void ObjectSetFloat(char* sCommand, int iObjectID, float fValue);

std::string ObjectGetString(std::string sCommand, char* cParam1, int iObjectID);
void ObjectSetString(char* sCommand, char* cParam1,  int iObjectID, char* sValue);



#endif
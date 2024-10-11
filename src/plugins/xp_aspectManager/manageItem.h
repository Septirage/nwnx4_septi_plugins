#if !defined(SEPT_MANAGEITEM)
#define SEPT_MANAGEITEM

int ItemGetInt(char* sCommand, int iObjectID);
void ItemSetInt(char* sCommand, int iObjectID, int iValue);

float ItemGetFloat(char* sCommand, int iObjectID);
void ItemSetFloat(char* sCommand, int iObjectID, float fValue);

std::string ItemGetString(char* sCommand, int iObjectID);
void ItemSetString(char* sCommand, int iObjectID, char* sValue);



#endif
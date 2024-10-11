#if !defined(SEPT_MANAGEAREA)
#define SEPT_MANAGEAREA


int AreaGetInt(char* cCommand, int iObjectID);
void AreaSetInt(char* cCommand, int iObjectID, int iValue);


float AreaGetFloat(char* cCommand, int iObjectID);
void AreaSetFloat(char* cCommand, int iObjectID, float fValue);

std::string AreaGetString(char* cCommand, int iObjectID);
void AreaSetString(char* cCommand, int iObjectID, char* sValue);




#endif

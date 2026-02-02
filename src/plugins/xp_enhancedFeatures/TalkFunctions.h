#if !defined(SEPT_EFFTALKFCT)
#define SEPT_EFFTALKFCT

void InitTalkFunctions();

void SetStringTalkFunction(char* sCommand, int nParam2, char* sValue);

void SetFloatTalkFunction(char* cCommand, int nParam2, float fValue);
float GetFloatTalkFunction(char* cCommand, int nParam2);

#endif
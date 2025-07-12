#if !defined(ASPECT_MANAGER_SEPT_REFRESHOBJECT)
#define ASPECT_MANAGER_SEPT_REFRESHOBJECT


#define FCT_REMOVEFROMAREA              0x00607390
#define FCT_ADDTOAREA                   0x00606f00

#define FCT_COLISIONANDLOS              0x0064a650
#define FCT_COLISIONANDLOSDOOR			0x00652d00

void ColisionLoSModelName(void* ptrObjet);
void AddToArea(void* ptrArea, void* ptrObject);
void RemoveFromArea(void* ptrArea, void* ptrObject);
void refreshObject(int nParam2, int nValue, int iOption = 0);

#endif
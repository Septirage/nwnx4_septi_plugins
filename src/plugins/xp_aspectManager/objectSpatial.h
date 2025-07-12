#if !defined(ASPECT_MANAGER_SEPT_OBJECTSPATIAL)
#define ASPECT_MANAGER_SEPT_OBJECTSPATIAL

#include <string>
#include <vector>

void SetDynamicColision(int nObject, int nValue);
void ObjectSetPosition(int iObjectId, float fX, float fY, float fZ);
void ObjectSetOrientation(int iObjectId, float fPos);
void ObjectSetPositionAndOrientation(int iObjectId, float fX, float fY, float fZ, float fFacing);
std::string TriggerGetGeometry(int iObjectId);
void TriggerChangeGeometry(int iObjectId, std::vector<float>& vGeometry);


#endif

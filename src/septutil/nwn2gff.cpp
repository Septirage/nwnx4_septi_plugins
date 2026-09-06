// nwn2gff.cpp
//
#include "nwn2gff.h"

#define OFF_FUNC_GetGFFByte          0x00794930
#define OFF_FUNC_GetGFFChar          0x007949A0
#define OFF_FUNC_GetGFFWord          0x00794A10
#define OFF_FUNC_GetGFFShort         0x00794A80
#define OFF_FUNC_GetGFFDWord         0x00794AF0
#define OFF_FUNC_GetGFFInt           0x00794B60
#define OFF_FUNC_GetGFFFloat         0x00794BD0
#define OFF_FUNC_GetGFFStructByName  0x00794850
#define OFF_FUNC_GetGFFStruct        0x007947d0
#define OFF_FUNC_GetGFFList          0x00794700
#define OFF_FUNC_GetGFFListLength    0x00794780

NWNGetGFFByte_t         const NWNGetGFFByte         = (NWNGetGFFByte_t)OFF_FUNC_GetGFFByte;
NWNGetGFFChar_t         const NWNGetGFFChar         = (NWNGetGFFChar_t)OFF_FUNC_GetGFFChar;
NWNGetGFFWord_t         const NWNGetGFFWord         = (NWNGetGFFWord_t)OFF_FUNC_GetGFFWord;
NWNGetGFFShort_t        const NWNGetGFFShort        = (NWNGetGFFShort_t)OFF_FUNC_GetGFFShort;
NWNGetGFFDWord_t        const NWNGetGFFDWord        = (NWNGetGFFDWord_t)OFF_FUNC_GetGFFDWord;
NWNGetGFFInt_t          const NWNGetGFFInt          = (NWNGetGFFInt_t)OFF_FUNC_GetGFFInt;
NWNGetGFFFloat_t        const NWNGetGFFFloat        = (NWNGetGFFFloat_t)OFF_FUNC_GetGFFFloat;
NWNGetGFFStructByName_t const NWNGetGFFStructByName = (NWNGetGFFStructByName_t)OFF_FUNC_GetGFFStructByName;
NWNGetGFFStruct_t       const NWNGetGFFStruct       = (NWNGetGFFStruct_t)OFF_FUNC_GetGFFStruct;
NWNGetGFFList_t         const NWNGetGFFList         = (NWNGetGFFList_t)OFF_FUNC_GetGFFList;
NWNGetGFFListLength_t   const NWNGetGFFListLength   = (NWNGetGFFListLength_t)OFF_FUNC_GetGFFListLength;

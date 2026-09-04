// nwn2gff.h
//
// Acces Wrappers to engine natives functions 
//
#pragma once

#include <cstdint>

// ----------------------------------------------------------------------------
// Value reading
// ----------------------------------------------------------------------------

typedef uint8_t  (__thiscall* NWNGetGFFByte_t) (void* puVar, void* gffPtr, const char* cField, void* aiStack110, uint8_t  iDefault);
typedef uint8_t  (__thiscall* NWNGetGFFChar_t) (void* puVar, void* gffPtr, const char* cField, void* aiStack110, uint8_t  iDefault);
typedef uint16_t (__thiscall* NWNGetGFFWord_t) (void* puVar, void* gffPtr, const char* cField, void* aiStack110, uint16_t iDefault);
typedef uint16_t (__thiscall* NWNGetGFFShort_t)(void* puVar, void* gffPtr, const char* cField, void* aiStack110, uint16_t iDefault);
typedef uint32_t (__thiscall* NWNGetGFFDWord_t)(void* puVar, void* gffPtr, const char* cField, void* aiStack110, uint32_t iDefault);
typedef uint32_t (__thiscall* NWNGetGFFInt_t)  (void* puVar, void* gffPtr, const char* cField, void* aiStack110, uint32_t iDefault);
typedef float    (__thiscall* NWNGetGFFFloat_t)(void* puVar, void* gffPtr, const char* cField, void* aiStack110, float    fDefault);

// ----------------------------------------------------------------------------
// GFF Structures/Lists navigation
// ----------------------------------------------------------------------------

// Get a GFF sub-struct by its name
// return 1 if valid, 0 otherwise
typedef int (__thiscall* NWNGetGFFStructByName_t)(void* puVar, void* newPtr, void* gffPtr, const char* cName);

// Get a GFF sub-struct by its index (in a list)
// return 1 if valid, 0 otherwise
typedef int (__thiscall* NWNGetGFFStruct_t)(void* puVar, void* newPtr, void* gffPtr, int idx);

// Get a GFF list by its name
// return 1 if valid, 0 otherwise
typedef int (__thiscall* NWNGetGFFList_t)(void* puVar, void* newPtr, void* gffPtr, const char* cName);

// Get list length
typedef int (__thiscall* NWNGetGFFListLength_t)(void* puVar, void* gffPtr);

// ----------------------------------------------------------------------------
// Instances (adresses resolved in nwn2gff.cpp)
// ----------------------------------------------------------------------------

extern NWNGetGFFByte_t         const NWNGetGFFByte;
extern NWNGetGFFChar_t         const NWNGetGFFChar;
extern NWNGetGFFWord_t         const NWNGetGFFWord;
extern NWNGetGFFShort_t        const NWNGetGFFShort;
extern NWNGetGFFDWord_t        const NWNGetGFFDWord;
extern NWNGetGFFInt_t          const NWNGetGFFInt;
extern NWNGetGFFFloat_t        const NWNGetGFFFloat;
extern NWNGetGFFStructByName_t const NWNGetGFFStructByName;
extern NWNGetGFFStruct_t       const NWNGetGFFStruct;
extern NWNGetGFFList_t         const NWNGetGFFList;
extern NWNGetGFFListLength_t   const NWNGetGFFListLength;


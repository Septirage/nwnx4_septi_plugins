//////////////////////////////////////////////////////////////////////////////////////////////
// nwnx_enhancedfeatures_spell - spell/spellbook specific functions of the EnhancedFeatures plugin
// Original Scripter:  Septirage
//--------------------------------------------------------------------------------------------
// Last Modified By:	Septirage           2024-09-20
//--------------------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////////////////

// Clear all memorized spell slots of a specific spell id, including metamagic'd ones.
// oCreature - The creature for whom to adjust the spell book.
// nClassType - A CLASS_TYPE_* constant. Must be a MemorizesSpells class.
// a SPELL_* constant.
void ClearMemorizedSpellBySpellId_EFF(object oCreature, int nClassType, int nSpellId);


// Clear a specific memorized spell slot.
// oCreature 	- The creature for whom to adjust the spell book.
// nClassType 	- A CLASS_TYPE_* constant. Must be a MemorizesSpells class.
// nSpellLevel 	- The spell Level (0-9)
// nIndex		- The index of the spell slot. Bounds: 0 <= nIndex < GetMemorizedSpellCountByLevel (slot)
void ClearMemorizedSpell_EFF(object oCreature, int nClassType, int nSpellLevel, int nIndex);

// Set the ready state of a memorized spell slot
// oCreature 	- The creature for whom to adjust the spell book.
// nClassType 	- A CLASS_TYPE_* constant. Must be a MemorizesSpells class.
// nSpellLevel 	- The spell Level (0-9)
// nIndex		- The index of the spell slot. Bounds: 0 <= nIndex < GetMemorizedSpellCountByLevel (slot)
// bReady		- TRUE to mark the slot ready
void SetMemorizedSpellReady_EFF(object oCreature, int nClassType, int nSpellLevel, int nIndex, int bReady);

// Set a memorized spell slot.
// oCreature  	  - The creature for whom to adjust the spell book.
// nClassType 	  - A CLASS_TYPE_* constant. Must be a MemorizesSpells class.
// nSpellLevel 	  - The spell Level (0-9)
// nIndex		  - The index of the spell slot. Bounds: 0 <= nIndex < GetMemorizedSpellCountByLevel (slot)
// nSpellId		  - A SPELL_* constant.
// bReady		  - TRUE to mark the slot ready. (default: TRUE)
// nMetaMagic	  - A METAMAGIC_* constant. (default: METAMAGIC_NONE)
// bIsDomainSpell - TRUE for a domain spell. (default: FALSE)
void SetMemorizedSpell_EFF(object oCreature, int nClassType, int nSpellLevel, int nIndex, int nSpellId, int bReady = TRUE, int nMetaMagic = METAMAGIC_NONE, int bIsDomainSpell = FALSE);



// Set the number of MemorizableSpell slot
// oCreature 	- The creature for whom to adjust the spell book.
// nClassType 	- A CLASS_TYPE_* constant. Must be a MemorizesSpells class.
// nSpellLevel 	- The spell Level (0-9)
// iNumber		- The new number of Slot 
void SetMemorizableSpellNumber_EFF(object oCreature, int nClassType, int nSpellLevel, int iNumber);


// Set the number of MemorizableSpell slot
// oCreature 	- The creature for whom to adjust the spell book.
// nClassType 	- A CLASS_TYPE_* constant. Must be a Sorcerers/Bards type class
// nSpellLevel 	- The spell Level (0-9)
// iNumber		- The new number spell usable
void SetNumberOfSpellUsesLeft_EFF(object oCreature, int nClassType, int nSpellLevel, int nNumber);

// Set the number of MemorizableSpell slot
// oCreature 	- The creature for whom to adjust the spell book.
// nClassType 	- A CLASS_TYPE_* constant.
// nSpellLevel 	- The spell Level (0-9)
// iNumber		- The new number of Bonus Spell
// Note: Will be used after rest for Sorceres/bards type class. For Wizard/cleric type it will only be a reminder
void SetNumberOfBonusSpell_EFF(object oCreature, int nClassType, int nSpellLevel, int nNumber);



// Gets if a spell is in the known spell list.
// oCreature 	- The creature for whom to check the spell book.
// nClassType	- A CLASS_TYPE_* constant. Must be a SpellBookRestricted class.
// nSpellId		- A SPELL_* constant.
int GetIsInKnownSpellList_EFF(object oCreature, int nClassType, int nSpellId);

// Gets the number of known spells for a given spell level.
// oCreature 	- The creature for whom to check the spell book.
// nClassType	- A CLASS_TYPE_* constant. Must be a SpellBookRestricted class.
// nSpellLevel 	- The spell Level (0-9)
int GetKnownSpellCount_EFF(object oCreature, int nClassType, int nSpellLevel);

// Gets the spell id of a known spell. 
// oCreature 	- The creature for whom to check the spell book.
// nClassType	- A CLASS_TYPE_* constant. Must be a SpellBookRestricted class.
// nSpellLevel 	- The spell Level (0-9)
// nIndex		- The index of the spell slot. Bounds: 0 <= nIndex < GetKnownSpellCount
int GetKnownSpellId_EFF(object oCreature, int nClassType, int nSpellLevel, int nIndex);

// Gets if the memorized spell slot has a domain spell. 
// oCreature 	- The creature for whom to check the spell book.
// nClassType	- A CLASS_TYPE_* constant. Must be a MemorizesSpells class.
// nSpellLevel 	- The spell Level (0-9)
// nIndex		- The index of the spell slot. Bounds: 0 <= nIndex < GetMemorizedSpellCountByLevel  (slot)
int GetMemorizedSpellIsDomainSpell_EFF(object oCreature, int nClassType, int nSpellLevel, int nIndex);



// Gets the metamagic of a memorized spell slot.  
// oCreature 	- The creature for whom to check the spell book.
// nClassType	- A CLASS_TYPE_* constant. Must be a MemorizesSpells class.
// nSpellLevel 	- The spell Level (0-9)
// nIndex		- The index of the spell slot. Bounds: 0 <= nIndex < GetMemorizedSpellCountByLevel  (slot)
int GetMemorizedSpellMetaMagic_EFF(object oCreature, int nClassType, int nSpellLevel, int nIndex);


// Gets the spell id of a memorized spell slot. 
// oCreature 	- The creature for whom to check the spell book.
// nClassType	- A CLASS_TYPE_* constant. Must be a MemorizesSpells class.
// nSpellLevel 	- The spell Level (0-9)
// nIndex		- The index of the spell slot. Bounds: 0 <= nIndex < GetMemorizedSpellCountByLevel  (slot)
int GetMemorizedSpellId_EFF(object oCreature, int nClassType, int nSpellLevel, int nIndex);



// Gets the ready state of a memorized spell slot. 
// oCreature 	- The creature for whom to check the spell book.
// nClassType	- A CLASS_TYPE_* constant. Must be a MemorizesSpells class.
// nSpellLevel 	- The spell Level (0-9)
// nIndex		- The index of the spell slot. Bounds: 0 <= nIndex < GetMemorizedSpellCountByLevel  (slot)
int GetMemorizedSpellReady_EFF(object oCreature, int nClassType, int nSpellLevel, int nIndex);



const int GETSPELLNUMBER_BASEONLY_EFF		= 0x1;
const int GETSPELLNUMBER_BONUSABILITY_EFF	= 0x2;
const int GETSPELLNUMBER_BONUSBYFEAT_EFF	= 0x4;
const int GETSPELLNUMBER_BONUSSPELLS_EFF	= 0x8;


// Gets the number of memorized spell slots (or memorized/or ready) for a given spell level. 
// oCreature 		  - The creature
// nClassType		  - A CLASS_TYPE_* constant
// nSpellLevel 		  - The spell Level (0-9)
// iSpellNumberType	  - One or several GETSPELLNUMBER_* Constants
int GetMaxNumberOfSpell_EFF(object oCreature, int nClassType, int nSpellLevel, int iSpellNumberType);


// Gets the number of memorized spell slots (or memorized/or ready) for a given spell level. 
// oCreature 		  - The creature for whom to check the spell book.
// nClassType		  - A CLASS_TYPE_* constant. Must be a MemorizesSpells class.
// nSpellLevel 		  - The spell Level (0-9)
// iSlotMemorizeReady - if 0: count the number of Slot.  If 1: only number of slot with a spell on it. 2: Only slot with ready spell on it
int GetMemorizedSpellCountByLevel_EFF(object oCreature, int nClassType, int nSpellLevel, int iSlotMemorizeReady);


// Gets the number of spell that still can be used
// oCreature 		  - The creature for whom adjust the number of spells.
// nClassType		  - A CLASS_TYPE_* constant. Must be a Sorcerers/Bards type class
// nSpellLevel 		  - The spell Level (0-9)
int GetNumberOfSpellUsesLeft_EFF(object oCreature, int nClassType, int nSpellLevel);



void ClearMemorizedSpellBySpellId_EFF(object oCreature, int nClassType, int nSpellId)
{
	NWNXSetInt("EnhancedFeatures", "Magic|ClearMemorizedSpellBySpellId", IntToString(nClassType), ObjectToInt(oCreature),  nSpellId);
}


void ClearMemorizedSpell_EFF(object oCreature, int nClassType, int nSpellLevel, int nIndex)
{
	NWNXSetInt("EnhancedFeatures", "Magic|ClearMemorizedSpell", IntToString(nClassType)+" "+IntToString(nSpellLevel), ObjectToInt(oCreature),  nIndex);
}

void SetMemorizedSpellReady_EFF(object oCreature, int nClassType, int nSpellLevel, int nIndex, int bReady)
{
	string sParam = IntToString(nClassType)+" "+IntToString(nSpellLevel)+" "+IntToString(nIndex);
	NWNXSetInt("EnhancedFeatures", "Magic|SetMemorizedSpellReady", sParam, ObjectToInt(oCreature),  bReady);
}

void SetMemorizedSpell_EFF(object oCreature, int nClassType, int nSpellLevel, int nIndex, int nSpellId, int bReady, int nMetaMagic, int bIsDomainSpell)
{
	string sParam = IntToString(nClassType)+" "+IntToString(nSpellLevel)+" "+IntToString(nIndex)+" "+IntToString(nSpellId)+" ";
	sParam = sParam +IntToString(bReady)+" "+IntToString(nMetaMagic);
	NWNXSetInt("EnhancedFeatures", "Magic|SetMemorizedSpell", sParam, ObjectToInt(oCreature),  bIsDomainSpell);
}

void SetMemorizableSpellNumber_EFF(object oCreature, int nClassType, int nSpellLevel, int iNumber)
{
	string sParam = IntToString(nClassType)+" "+IntToString(nSpellLevel);
	NWNXSetInt("EnhancedFeatures", "Magic|SetMemorizableSpellNumber", sParam, ObjectToInt(oCreature),  iNumber);
}

void SetNumberOfSpellUsesLeft_EFF(object oCreature, int nClassType, int nSpellLevel, int iNumber)
{
	string sParam = IntToString(nClassType)+" "+IntToString(nSpellLevel);
	NWNXSetInt("EnhancedFeatures", "Magic|SetNumberOfSpellUsesLeft", sParam, ObjectToInt(oCreature),  iNumber);
}


void SetNumberOfBonusSpell_EFF(object oCreature, int nClassType, int nSpellLevel, int nNumber)
{
	string sParam = IntToString(nClassType)+" "+IntToString(nSpellLevel);
	NWNXSetInt("EnhancedFeatures", "Magic|SetNumberOfBonusSpell", sParam, ObjectToInt(oCreature),  nNumber);
}



int GetIsInKnownSpellList_EFF(object oCreature, int nClassType, int nSpellId)
{
	return NWNXGetInt("EnhancedFeatures", "Magic|GetIsInKnownSpellList", IntToString(nClassType)+" "+IntToString(nSpellId), ObjectToInt(oCreature));
}

int GetKnownSpellCount_EFF(object oCreature, int nClassType, int nSpellLevel)
{
	return NWNXGetInt("EnhancedFeatures", "Magic|GetKnownSpellCount", IntToString(nClassType)+" "+IntToString(nSpellLevel), ObjectToInt(oCreature));
}

int GetKnownSpellId_EFF(object oCreature, int nClassType, int nSpellLevel, int nIndex)
{
	string sParam = IntToString(nClassType)+" "+IntToString(nSpellLevel)+" "+IntToString(nIndex);
	return NWNXGetInt("EnhancedFeatures", "Magic|GetKnownSpellId", sParam, ObjectToInt(oCreature));
}


int GetMemorizedSpellIsDomainSpell_EFF(object oCreature, int nClassType, int nSpellLevel, int nIndex)
{
	string sParam = IntToString(nClassType)+" "+IntToString(nSpellLevel)+" "+IntToString(nIndex);
	return NWNXGetInt("EnhancedFeatures", "Magic|GetMemorizedSpellIsDomainSpell", sParam, ObjectToInt(oCreature));
}

int GetMemorizedSpellMetaMagic_EFF(object oCreature, int nClassType, int nSpellLevel, int nIndex)
{
	string sParam = IntToString(nClassType)+" "+IntToString(nSpellLevel)+" "+IntToString(nIndex);
	return NWNXGetInt("EnhancedFeatures", "Magic|GetMemorizedSpellMetaMagic", sParam, ObjectToInt(oCreature));
}

int GetMemorizedSpellId_EFF(object oCreature, int nClassType, int nSpellLevel, int nIndex)
{
	string sParam = IntToString(nClassType)+" "+IntToString(nSpellLevel)+" "+IntToString(nIndex);
	return NWNXGetInt("EnhancedFeatures", "Magic|GetMemorizedSpellId", sParam, ObjectToInt(oCreature));
}

int GetMemorizedSpellReady_EFF(object oCreature, int nClassType, int nSpellLevel, int nIndex)
{
	string sParam = IntToString(nClassType)+" "+IntToString(nSpellLevel)+" "+IntToString(nIndex);
	return NWNXGetInt("EnhancedFeatures", "Magic|GetMemorizedSpellReady", sParam, ObjectToInt(oCreature));
}

int GetMaxNumberOfSpell_EFF(object oCreature, int nClassType, int nSpellLevel, int iFullBonusBase)
{
	string sParam = IntToString(nClassType)+" "+IntToString(nSpellLevel)+" "+IntToString(iFullBonusBase);
	return NWNXGetInt("EnhancedFeatures", "Magic|GetMaxNumberOfSpell", sParam, ObjectToInt(oCreature));
}

int GetMemorizedSpellCountByLevel_EFF(object oCreature, int nClassType, int nSpellLevel, int iSlotMemorizeReady)
{
	string sParam = IntToString(nClassType)+" "+IntToString(nSpellLevel)+" "+IntToString(iSlotMemorizeReady);
	return NWNXGetInt("EnhancedFeatures", "Magic|GetMemorizedSpellCountByLevel", sParam, ObjectToInt(oCreature));
}

int GetNumberOfSpellUsesLeft_EFF(object oCreature, int nClassType, int nSpellLevel)
{
	string sParam = IntToString(nClassType)+" "+IntToString(nSpellLevel);
	return NWNXGetInt("EnhancedFeatures", "Magic|GetNumberOfSpellUsesLeft", sParam, ObjectToInt(oCreature));
}
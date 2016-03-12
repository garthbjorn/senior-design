/*
 *  Copyright 2014 Embedded Artists AB
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "mbed.h"
#include "DMBoard.h"
#include "InternalEEPROM.h"
#include "Registry.h"


/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/

//Registry::Entry::Entry(const char* key, const char* value, bool free) :
//    _key(key),_value(value),_freeKey(free),_freeValue(free)
//{
//}
//
//Registry::Entry::~Entry()
//{
//  if (_freeKey && _key != NULL) {
//    free((void*)_key);
//    _key = NULL;
//  }
//}
//
//void Registry::Entry::modify(const char* newValue)
//{
//  if (_freeValue && _value != NULL) {
//    free((void*)_value);
//    _value = NULL;
//  }
//  _freeValue = true;
//  _value = newValue;
//}

/******************************************************************************
 * Local variables
 *****************************************************************************/

/******************************************************************************
 * Private Class
 *****************************************************************************/

/******************************************************************************
 * Private Functions
 *****************************************************************************/

Registry::Registry() : _numEntries(0), _entries(NULL)
{
}

Registry::~Registry()
{
  if (_data != NULL) {
    free(_data);
    _data = NULL;
  }
  _numEntries = 0;
  _entries = NULL;
}

Registry::RegistryError Registry::fromEEPROM()
{
  // Registry is stored as a header followed by a nuber of key-value
  // pairs. Each key and value is a maximum of 31 characters + null
  // termination. The header looks like this:
  //
  // key = 'e','a','r','e','g','\0'
  // val = X
  // 
  // where X is the number of key-value pairs used (one byte).
  // The list is compacted so that there are no unused entries in
  // the list.
  //
  _entries = (reg_entry_t*)_data;
  _numEntries = _entries[0].val[0];
  for (int i = 0; i < NumEntries; i++) {
    _modified[i] = false;
  }
    
  // Check header entry
  if (strcmp(_entries[0].key, "eareg") == 0) {
    
    // Check num entries
    if (_numEntries < NumEntries) {
        
      // list is valid, but make sure null termination is ok
      // just in case
      for (int i = 1; i <= _numEntries; i++) {
        _entries[i].key[EntryLen-1] = '\0';
        _entries[i].val[EntryLen-1] = '\0';
      }
      
      return Ok;
    }
  }
  
  // The list is either corrupt or doesn't exist, create one
  memset(_data, 0, NumEntries*sizeof(reg_entry_t));
  sprintf(_entries[0].key, "eareg");
  _entries[0].val[0] = _numEntries = 0;
  _modified[0] = true;
  
  return Ok;
}

Registry::RegistryError Registry::toEEPROM()
{
  if (_entries[0].val[0] != _numEntries) {
    _entries[0].val[0] = _numEntries;
    _modified[0] = true;
  }
  return Ok;
}

int Registry::find(const char* key)
{
  for (int i = 1; i <= _numEntries; i++) {
    if (strcmp(key, _entries[i].key) == 0) {
      return i;
    }
  }
  return -1;
}

/******************************************************************************
 * Public Functions
 *****************************************************************************/

Registry::RegistryError Registry::load()
{
  InternalEEPROM eeprom;
  RegistryError err = Ok;

  _mutex.lock();
  if (_data != NULL) {
    free(_data);
    _data = NULL;
  }
  _numEntries = 0;
  _entries = NULL;

  do {
    eeprom.init();

    _data = (uint8_t*)malloc(eeprom.memorySize());
    if (_data == NULL) {
      err = MemoryError;
      break;
    }
    
    uint32_t read = eeprom.read(0,_data,eeprom.memorySize());
    if (read != eeprom.memorySize()) {
      err = EEPROMReadError;
      break;
    }
    
    eeprom.powerDown();
    
    // decode the data
    err = fromEEPROM();
    if (err != Ok) {
      free(_data);
      _data = NULL;
    }
    
  } while(0);
    
  _mutex.unlock();
  return err;
}

Registry::RegistryError Registry::setValue(const char* key, const char* val)
{
  RegistryError err = Ok;
  _mutex.lock();
  int existingIdx = find(key);
  if (existingIdx == -1) {
    // new value
    if (_numEntries < (NumEntries-1)) {
      // room for more
     if (strlen(key) < EntryLen) {
       if (strlen(val) < EntryLen) {
         _numEntries++;
         strncpy(_entries[_numEntries].key, key, EntryLen-1);
         strncpy(_entries[_numEntries].val, val, EntryLen-1);
         _modified[_numEntries] = true;
       } else {
         err = ValueLenError;
       }
     } else {
       err = KeyLenError;
     }
    } else {
      err = RegistryFullError;
    }
  } else {
    // already have value, modify it
    if (strlen(val) < EntryLen) {
      strncpy(_entries[existingIdx].val, val, EntryLen-1);
      _modified[existingIdx] = true;
    } else {
      err = ValueLenError;
    }
  }
  _mutex.unlock();
  return err;
}

Registry::RegistryError Registry::getValue(const char* key, char** pVal)
{
  RegistryError err = Ok;
  _mutex.lock();
  int existingIdx = find(key);
  if (existingIdx == -1) {
    *pVal = NULL;
    err = NoSuchKeyError;
  } else {
    *pVal = (char*)malloc(EntryLen);
    if (*pVal == NULL) {
      err = MemoryError;
    } else {
      strncpy(*pVal, _entries[existingIdx].val, EntryLen);
    }
  }
  _mutex.unlock();
  return err;
}

Registry::RegistryError Registry::entryAt(int pos, char** pKey, char** pVal)
{
  RegistryError err = Ok;
  _mutex.lock();
  if (pos < 0 || pos >= NumEntries) {
    err = InvalidPositionError;
  } else {
    *pKey = (char*)malloc(EntryLen);
    if (*pKey == NULL) {
      err = MemoryError;
    } else {
      strncpy(*pKey, _entries[pos+1].key, EntryLen);
      *pVal = (char*)malloc(EntryLen);
      if (*pVal == NULL) {
        err = MemoryError;
        free(*pKey);
        *pKey = NULL;
      } else {
        strncpy(*pVal, _entries[pos+1].val, EntryLen);
      }
    }
  }
  _mutex.unlock();
  return err;
}


Registry::RegistryError Registry::registerListener()
{
    return Ok;
}

Registry::RegistryError Registry::store()
{
  InternalEEPROM eeprom;
  RegistryError err = Ok;

  _mutex.lock();
    
  do {
    err = toEEPROM();
    if (err != Ok) {
      break;
    }
    
    eeprom.init();  

    for (int i = 0; i <= _numEntries; i++) {
      if (_modified[i]) {
        if (eeprom.write(i*EntrySize, (uint8_t*)&_entries[i], EntrySize) != EntrySize) {
          err = EEPROMWriteError;
          break;
        }
        _modified[i] = false;
      }
    }
    
    eeprom.powerDown();
    
  } while(0);
  
  _mutex.unlock();  
  
  return err;
}

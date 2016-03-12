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

#ifndef REGISTRY_H
#define REGISTRY_H

#include "mbed.h"
#include "rtos.h"
#include "InternalEEPROM.h"

/**
 * Example of using the Registry class:
 *
 * @code
 * #include "mbed.h"
 * #include "DMBoard.h"
 *
 * int main(void) {
 *    DMBoard* board = &DMBoard::instance();
 *    board->init();
 *    ...
 *    board->registry()
 * }
 * @endcode
 */
class Registry {
public:
  
    enum RegistryError {
        Ok                =       0,
        ReadOnlyError,
        MemoryError,
        EEPROMReadError,
        EEPROMWriteError,
        NoSuchKeyError,
        KeyLenError,
        ValueLenError,
        InvalidPositionError,
        RegistryFullError,
    };
    
    /** Get the only instance of the Registry
     *
     *  @returns The Registry
     */
    static Registry& instance()
    {
        static Registry singleton;
        return singleton;
    }
  
    /** Loads all (if any) values from the internal EEPROM
     *
     *  @returns
     *       Ok on success
     *       An error code on failure
     */
    RegistryError load();

    /** Sets (or replaces if existing) the value for the key
     *
     *  The key/value parameters are copied so it is ok to free them
     *  after calling this function.
     *
     *  @param key  the key to create or update
     *  @param val  the value to store for the key
     *
     *  @returns
     *       Ok on success
     *       An error code on failure
     */
    RegistryError setValue(const char* key, const char* val);
    
    /** Gets (if any) the the value for the key
     *
     *  If Ok is returned then pVal will point to allocated memory
     *  that must be deallocated with free() by the caller.
     *
     *  @param key   the key to look for
     *  @param pVal  will hold the value if successful
     *
     *  @returns
     *       Ok on success
     *       NoSuchKeyError if there is no value for the key
     *       An error code on failure
     */
    RegistryError getValue(const char* key, char** pVal);
    
    /** Retrieves the key-value pair at the specified index
     *
     *  If Ok is returned then pKey and pVal will point to allocated 
     *  memory that must be deallocated with free() by the caller.
     *
     *  @param pos   the index to look for
     *  @param pKey  will hold the key if successful
     *  @param pVal  will hold the value if successful
     *
     *  @returns
     *       Ok on success
     *       An error code on failure
     */
    RegistryError entryAt(int pos, char** pKey, char** pVal);
    
    /** Returns the number of key-value pairs in the registry
     *
     *  @returns the number of key-value pairs
     */
    int numEntries() { return _numEntries; }
    
    RegistryError registerListener();

    /** Stores the registry in the internal EEPROM
     *
     *  @returns
     *       Ok on success
     *       An error code on failure
     */
    RegistryError store();
    

private:
    enum Constants {
      NumEntries   = InternalEEPROM::EEPROM_NUM_PAGES,
      EntryLen     = 32,
      EntrySize    = 2*EntryLen,
    };

    typedef struct {
      char key[EntryLen];
      char val[EntryLen];        
    } reg_entry_t;

    uint8_t* _data;
    int _numEntries;
    reg_entry_t* _entries;
    bool _modified[NumEntries];
    Mutex _mutex;
    
    explicit Registry();
    // hide copy constructor
    Registry(const Registry&);
    // hide assign operator
    Registry& operator=(const Registry&);
    ~Registry();
    
    RegistryError fromEEPROM();
    RegistryError toEEPROM();
    int find(const char* key);
};

#endif

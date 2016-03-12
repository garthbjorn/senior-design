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

#ifndef INTERNAL_EEPROM_H
#define INTERNAL_EEPROM_H

#include "mbed.h"

/**
 * Internal EEPROM Driver
 */
class InternalEEPROM {
public:
    
    enum Constants {
        EEPROM_MEMORY_SIZE = 4032,
        EEPROM_PAGE_SIZE   =   64,
        EEPROM_NUM_PAGES   = EEPROM_MEMORY_SIZE/EEPROM_PAGE_SIZE,
    };
  
    InternalEEPROM();
    ~InternalEEPROM();

    /** Initializes the EEPROM
     *
     *  @returns
     *       Ok on success
     *       An error code on failure
     */
    void init();
    
    /** Put the internal EEPROM in a low power state
     */
    void powerDown();
    
    /** Reads size bytes from offset addr
     *
     *  Note that this function will power up the EEPROM so it is
     *  recommended to call powerDown() when finished reading.
     *
     *  @param addr  the offset to read from
     *  @param data  buffer to store the read data in
     *  @param size  number of bytes to read
     *
     *  @returns
     *       The number of bytes read
     */
    int read(uint32_t addr, uint8_t* data, uint32_t size);

    /** Writes size bytes to offset addr
     *
     *  Note that this function will power up the EEPROM so it is
     *  recommended to call powerDown() when finished writing.
     *
     *  @param addr  the offset to write to
     *  @param data  the data to write
     *  @param size  number of bytes to write
     *
     *  @returns
     *       The number of bytes written
     */
    int write(uint32_t addr, const uint8_t* data, uint32_t size);
  
    /** Returns the size (in bytes) of the internal EEPROM
     *
     *  @returns
     *       The size in bytes
     */
    uint32_t memorySize() { return EEPROM_MEMORY_SIZE; }
  
private:

    bool _initialized;
    
    void powerUp();
    void clearInterrupt(uint32_t mask);
    void waitForInterrupt(uint32_t mask);
    void setAddr(uint32_t pageAddr, uint32_t pageOffset);
    void setCmd(uint32_t cmd);
    void readPage(uint32_t pageAddr, uint32_t pageOffset, uint8_t* buf, uint32_t size);
    void writePage(uint32_t pageAddr, uint32_t pageOffset, const uint8_t* buf, uint32_t size);
    void eraseOrProgramPage(uint32_t pageAddr);
};

#endif

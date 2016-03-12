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

#ifndef BIOS_EEPROM_H
#define BIOS_EEPROM_H

#include "mbed.h"

/**
 * Bios EEPROM Driver
 */
class BiosEEPROM {
public:
    
    BiosEEPROM();

    /** Reads
     *
     * @param address   the address to read from
     * @param data      buffer to store read data in
     * @param size      number of bytes to read
     *
     *  @returns
     *       True on success
     *       False on failure
     */
    bool read(uint32_t address, char* data, uint32_t size);
  
private:
    I2C _i2c;
};

#endif

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
#include "BiosEEPROM.h"

/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/

#define BIOS_EEPROM_ADDR      (0xAC)


/******************************************************************************
 * Local variables
 *****************************************************************************/

/******************************************************************************
 * Private Functions
 *****************************************************************************/


/******************************************************************************
 * Public Functions
 *****************************************************************************/

BiosEEPROM::BiosEEPROM() : _i2c(P0_27, P0_28)
{
    _i2c.frequency(400000);
}

bool BiosEEPROM::read(uint32_t address, char* data, uint32_t size)
{
    bool success = false;
    do {
        char buf[2];
        buf[0] = address >> 8;
        buf[1] = address & 0xff;
        
        if (_i2c.write(BIOS_EEPROM_ADDR, buf, 2) == 0) {
            if (_i2c.read(BIOS_EEPROM_ADDR, data, size) == 0) {
                success = true;
                break;
            }
        }
    } while(false);
    return success;
}

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

#ifndef BIOSLOADER_H
#define BIOSLOADER_H

#include "mbed.h"
#include "DMBoard.h"
#include "BiosDisplay.h"
#include "BiosTouch.h"
#include "bios.h"

/**
 * Unpacks and prepares the BIOS code.
 */
class BiosLoader {
public:
    
    /** Get the only instance of the BiosLoader
     *
     *  @returns The BIOS
     */
    static BiosLoader& instance()
    {
        static BiosLoader singleton;
        return singleton;
    }
  

    void getMacAddress(char mac[6]);
    bool isKnownSPIFIMemory(uint8_t mfgr, uint8_t devType, uint8_t devID, uint32_t memSize, uint32_t* eraseBlockSize);
    void handleI2CInterrupt();    

    void getBiosStats(uint8_t& type, uint8_t& major, uint8_t& minor, uint8_t& rev);

    friend class BiosDisplay;
    friend class BiosTouch;
    friend class DMBoard;

private:

    bool _initialized;

    bios_header_t _bios;
    void* _biosData;
    uint8_t* _conf;
    uint32_t _confSize;
    uint32_t _stats;

    explicit BiosLoader();
    // hide copy constructor
    BiosLoader(const BiosLoader&);
    // hide assign operator
    BiosLoader& operator=(const BiosLoader&);
    ~BiosLoader();

    /** Loads, verifies and prepares the BIOS
     *
     *  @returns
     *       Ok on success
     *       An error code on failure
     */
    DMBoard::BoardError init();
    DMBoard::BoardError readBIOS(uint8_t** data, uint32_t* size);
    DMBoard::BoardError params(bios_header_t** header, void** instanceData);
    
    void resetI2C();
};

#endif /* BIOSLOADER_H */

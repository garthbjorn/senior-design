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
#include "BiosLoader.h"
#include "DMBoard.h"
#include "BiosEEPROM.h"
#include "crc.h"
#include "bios.h"
#include "meas.h"

#if defined(DM_BOARD_BIOS_DEVELOPMENT)
  #ifdef __cplusplus
    extern "C" {
  #endif
      void bios_debug_aid(bios_header_t* header, const char** pMsg, uint32_t* paramSize);
  #ifdef __cplusplus
    }
  #endif
#endif

/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/
 
#define MOVE_POINTER(__x, __off) (   ( (uint32_t*)(__x) ) = (uint32_t*)( (uint32_t)(__x) + (__off) ) )

/*
 * Make sure that we reserve at least this amount of RAM for future
 * expansion of the BIOS. This prevents the user from squeezing out
 * the last drop of available RAM in his application.
 */
#define BIOS_RESERVED_CHUNK  0x1000
#define BIOS_MAX_SIZE        0x100000
#ifndef MAX
  #define MAX(__a, __b) (((__a)>(__b))?(__a):(__b))
#endif

/*
 * The BIOS is API compatible as long as the Major component of the
 * version is the same.
 */
#define SUPPORTED_BIOS_VER   0x000000
#define SUPPORTED_BIOS_MASK  0xff0000  // only look at the Major component
#define SUPPORTED_VERSION(__ver) (((__ver)&SUPPORTED_BIOS_MASK) == SUPPORTED_BIOS_VER)

//#define MAC_IN_SDK

/******************************************************************************
 * Local variables
 *****************************************************************************/

/******************************************************************************
 * Global functions
 *****************************************************************************/

#if !defined(MAC_IN_SDK)
/* The LPC4088QSB platform in the MBED SDK have defined the WEAK function
 * mbed_mac_address (ethernet_api.c) to read a unique MAC address from the
 * onboard EEPROM.
 * The LPC4088DM platform in the MBED SDK does not have the WEAK function
 * so it is implemented here instead. This version of the function will ask
 * the bios for a MAC address.
 */
void mbed_mac_address(char *mac) {
  static char cache[6];
  static bool haveIt = false;
  if (!haveIt) {
    BiosLoader::instance().getMacAddress(cache);
    haveIt = true;
  }
  memcpy(mac, cache, 6);
}
#endif

/******************************************************************************
 * Private Functions
 *****************************************************************************/

// Called by the NVIC
static void loader_i2c0_irq_handler()
{
  BiosLoader::instance().handleI2CInterrupt();
}


// Function called from the BIOS
static uint32_t readTimeMs()
{
  return us_ticker_read()/1000;
}


BiosLoader::BiosLoader() : 
    _initialized(false),
    _biosData(NULL),
    _conf(NULL),
    _confSize(0),
    _stats(0)
{
}

BiosLoader::~BiosLoader()
{
  if (_biosData != NULL) {
    free(_biosData);
    _biosData = NULL;
  }
  if (_conf != NULL) {
    free(_conf);
    _conf = NULL;
    _confSize = 0;
  }
}

DMBoard::BoardError BiosLoader::readBIOS(uint8_t** data, uint32_t* size)
{
  DMBoard::BoardError err = DMBoard::Ok;
  BiosEEPROM eeprom;
  file_header_t fh;

  if (_conf != NULL) {
    *data = _conf;
    *size = _confSize;
    return DMBoard::Ok;
  }
    
  do {
    if (!eeprom.read(0, (char*)&fh, sizeof(file_header_t))) {
      resetI2C();
      if (!eeprom.read(0, (char*)&fh, sizeof(file_header_t))) {
        err = DMBoard::BiosStorageError;
        break;
      }
    }
    
    if (fh.magic != BIOS_MAGIC) {
      err = DMBoard::BiosInvalidError;
      break;
    }
    
    if (!SUPPORTED_VERSION(fh.version)) {
      err = DMBoard::BiosVersionError;
      break;
    }
    
    if ((fh.headerSize + fh.size) > BIOS_MAX_SIZE) {
      err = DMBoard::BiosInvalidError;
      break;
    }
    
    _confSize = fh.headerSize + fh.size;
    _conf = (uint8_t*)malloc(MAX(_confSize,BIOS_RESERVED_CHUNK));
    if (_conf == NULL) {
      _confSize = 0;
      err = DMBoard::MemoryError;
      break;
    }
    
    if (!eeprom.read(0, (char*)_conf, _confSize)) {
      err = DMBoard::BiosStorageError;
      break;
    }
    
    uint32_t crc = crc_Buffer((uint32_t*)(&_conf[fh.headerSize]), fh.size/4);
    if (crc != fh.crc) {
      err = DMBoard::BiosInvalidError;
      break;
    }
    
    // Bios header has been verified and seems ok
    *data = _conf;
    *size = _confSize;
    _stats = fh.version;
    err = DMBoard::Ok;
  } while (false);
  
  if (err != DMBoard::Ok) {
    if (_conf != NULL) {
      free(_conf);
      _conf = NULL;
      _confSize = 0;
    }
  }

  return err;
}

DMBoard::BoardError BiosLoader::params(bios_header_t** header, void** instanceData)
{
  if (!_initialized) {
    DMBoard::BoardError err = init();
    if (err != DMBoard::Ok) {
      return err;
    }    
  }
  if (_initialized) {
    *header = &_bios;
    *instanceData = _biosData;
    return DMBoard::Ok;
  } else {
    return DMBoard::BiosInvalidError;
  }
}

DMBoard::BoardError BiosLoader::init()
{
  DMBoard::BoardError err = DMBoard::Ok;
  if (!_initialized) {
    do {
      
      // Get the display bios from the DMBoard. DMBoard will have verified it
      // and will keep it in RAM so there is no need to copy it.
      uint8_t* p = NULL;
      uint32_t size = 0;
      err = readBIOS(&p, &size);
      if (err != DMBoard::Ok) {
        break;
      }
      
#if defined(MAC_IN_SDK)
      // The BIOS has been read so we know that the I2C bus is working. After the
      // BIOS is "started" it will take ownership of the bus and it can cause
      // problems for other peripherals on it. The only other peripheral today
      // is the EEPROM with the MAC address. It is read by mbed_mac_address() in
      // ethernet_api.c in the SDK and it will be cached. By reading it here now
      // we prevent future access to the I2C bus.
      char mac[6] = {0};
      mbed_mac_address(mac);
#endif
      
      // Extract the function pointers so that they can be modified to match the
      // actual location of the code
      file_header_t* file_header = (file_header_t*)p;      
      memcpy(&_bios, &file_header->header, sizeof(bios_header_t));
      
      // Allocate memory for the BIOS instance data
      _biosData = malloc(file_header->paramSize);
      if (_biosData == NULL) {
        err = DMBoard::MemoryError;
        break;
      }
      
      // All offsets must be moved by two factors:
      // 1) The position of the code in RAM (location of "p")
      // 2) The header size (the code/data comes after it)
      uint32_t offset = ((uint32_t)p) + file_header->headerSize;
      uint32_t* functions = (uint32_t*)&_bios;
      for (uint32_t i = 0; i < (sizeof(bios_header_t)/sizeof(uint32_t)); i++) {
        functions[i] += offset;
      }

#if defined(DM_BOARD_BIOS_DEVELOPMENT)
      // This requires that the project contains the source code for the BIOS
      const char* msg;
      uint32_t tmp = 0;
      bios_debug_aid(&_bios, &msg, &tmp);
      if (tmp > file_header->paramSize) {
        free(_biosData);
        _biosData = malloc(tmp);
        if (_biosData == NULL) {
          err = DMBoard::MemoryError;
          break;
        }
      }
      DMBoard::instance().logger()->printf("BIOS info: %s\n", msg);
#endif
      
      // Prepare the BIOS instance data before calling the first function
      BiosError_t e = _bios.initParams(_biosData, SystemCoreClock, PeripheralClock, wait_us, readTimeMs);
      if (e != BiosError_Ok) {
        err = DMBoard::BiosInvalidError;
        break;
      }

      // Setup the mandatory I2C0 interrupt handler after initParams but before all other calls
      NVIC_DisableIRQ(I2C0_IRQn);
      NVIC_SetVector(I2C0_IRQn, (uint32_t)loader_i2c0_irq_handler);      
      NVIC_EnableIRQ(I2C0_IRQn);
      
      _initialized = true;
    } while(0);
  }
  return err;
}

void BiosLoader::resetI2C()
{
    DMBoard::instance().logger()->printf("BiosLoader::resetI2C()\n");
    DigitalOut reset(P0_23);
    reset = 0;
    wait_ms(1);
    reset = 1;
    wait_ms(10);
}


/******************************************************************************
 * Public Functions
 *****************************************************************************/

bool BiosLoader::isKnownSPIFIMemory(uint8_t mfgr, uint8_t devType, uint8_t devID, uint32_t memSize, uint32_t* eraseBlockSize)
{
  if (!_initialized) {
    DMBoard::BoardError err = init();
    if (err != DMBoard::Ok) {
      return false;
    }    
  }
  if (_initialized) {
    bool known = false;
    BiosError_t err = _bios.spifiIsSupported(_biosData, mfgr,devType,devID,memSize,&known,eraseBlockSize);
    if (err == BiosError_Ok) {
      return known;
    }
  }
  return false;
}

void BiosLoader::getMacAddress(char mac[6])
{
  if (!_initialized) {
    init();
  }
  if (_initialized) {
    BiosError_t err = _bios.ethernetMac(_biosData, mac);
    if (err == BiosError_Ok) {
      return;
    }
  }
  
  // We always consider the MAC address to be retrieved even though
  // reading is failed. If it wasn't possible to read then the default
  // address will be used.
  mac[0] = 0x00;
  mac[1] = 0x02;
  mac[2] = 0xF7;
  mac[3] = 0xF0;
  mac[4] = 0x00;
  mac[5] = 0x01;
}

void BiosLoader::handleI2CInterrupt()
{
  _bios.i2cIRQHandler(_biosData);
}

void BiosLoader::getBiosStats(uint8_t& type, uint8_t& major, uint8_t& minor, uint8_t& rev)
{
  type  = (_stats >> 24) & 0xff;
  major = (_stats >> 16) & 0xff;
  minor = (_stats >>  8) & 0xff;
  rev   = (_stats >>  0) & 0xff;
}


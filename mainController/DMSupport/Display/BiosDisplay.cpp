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
#include "BiosDisplay.h"
#include "BiosLoader.h"
#include "DMBoard.h"
#include "bios.h"


/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/
 

/******************************************************************************
 * Local variables
 *****************************************************************************/


/******************************************************************************
 * Private Functions
 *****************************************************************************/

BiosDisplay::BiosDisplay() : 
    _initialized(false),
    _poweredOn(false),
    _bios(NULL),
    _biosData(NULL),
    _width(0),
    _height(0),
    _bpp(0),
    _supportedRes(0),
    _activeRes(Res_16bit_rgb565),
    _landscape(false)
{
}

BiosDisplay::~BiosDisplay()
{
  // _bios and _biosData are deallocated by BiosLoader
}


/******************************************************************************
 * Public Functions
 *****************************************************************************/

BiosDisplay::DisplayError BiosDisplay::init()
{
  DisplayError err = DisplayError_Ok;
  if (!_initialized) {
    do {
      if (BiosLoader::instance().params(&_bios, &_biosData) != DMBoard::Ok) {
        err = DisplayError_ConfigError;
        break;
      }
      
      err = (DisplayError)_bios->displayInit(_biosData);
      if (err != DisplayError_Ok) {
        break;
      }

      err = (DisplayError)_bios->displayInformation(_biosData, &_width, &_height, &_bpp, &_landscape, &_supportedRes, &_activeRes);
      if (err != DisplayError_Ok) {
        break;
      }
      
      _initialized = true;
    } while(0);
  }
  return err;
}

BiosDisplay::DisplayError BiosDisplay::powerUp(void* framebuffer, Display::Resolution res, FrameRate_t rate)
{
  DisplayError err = DisplayError_Ok;
  if (!_poweredOn) {
    err = init();
    if (err == DisplayError_Ok) {
      do {
        err = (DisplayError)_bios->displayPowerUp(_biosData, framebuffer, (Resolution_t)res, rate);
          
        if (err != DisplayError_Ok) {
          break;
        }

        err = (DisplayError)_bios->displayInformation(_biosData, &_width, &_height, &_bpp, &_landscape, &_supportedRes, &_activeRes);     
        if (err != DisplayError_Ok) {
          break;
        }
        
        _poweredOn = true;
      } while(0);
    }
  }
  return err;
}

BiosDisplay::DisplayError BiosDisplay::powerDown()
{
  DisplayError err = DisplayError_Ok;
  if (_poweredOn) {
    err = (DisplayError)_bios->displayPowerDown(_biosData);
    _poweredOn = false;
  }
  return err;
}

BiosDisplay::DisplayError BiosDisplay::backlight(int percent)
{
  DisplayError err = DisplayError_Ok;
  if (!_initialized) {
      err = DisplayError_NoInit;
  } else {
    err = (DisplayError)_bios->displayBacklight(_biosData, percent);
  }      
  return err;
}

uint16_t BiosDisplay::width()
{
  return _width;
}

uint16_t BiosDisplay::height()
{
  return _height;
}

uint16_t BiosDisplay::bytesPerPixel()
{
  return _bpp;
}

uint32_t BiosDisplay::fbSize()
{
  return _width * _height * _bpp;
}

bool BiosDisplay::landscape()
{
  return _landscape;
}

void* BiosDisplay::allocateFramebuffer(Display::Resolution res)
{
  if (_initialized) {
    return malloc(_width*_height*bpp(res));
  }
  return NULL;
}

void* BiosDisplay::allocateFramebuffers(uint32_t num, Display::Resolution res)
{
  if (_initialized && num>0) {
    return malloc(_width*_height*bpp(res)*num);
  }
  return NULL;
}

void BiosDisplay::setFramebuffer(void* buff)
{
  LPC_LCD->UPBASE = (uint32_t)buff;
}

void* BiosDisplay::swapFramebuffer(void* buff)
{
  uint32_t old = LPC_LCD->UPBASE;
  LPC_LCD->UPBASE = (uint32_t)buff;
  return (void*)old;
}

bool BiosDisplay::isSupported(Display::Resolution res)
{
  return (_supportedRes & res);
}

BiosDisplay::Resolution BiosDisplay::currentResolution()
{
  return (Resolution)_activeRes;
}

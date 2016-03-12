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

#if defined(DM_BOARD_USE_DISPLAY)
  #include "BiosDisplay.h"
#endif
#if defined(DM_BOARD_USE_TOUCH)
  #include "BiosTouch.h"
#endif

#if defined(DM_BOARD_ENABLE_MEASSURING_PINS)
  #include "meas.h"
#endif        

/******************************************************************************
 * Configuration Compatibility Control
 *****************************************************************************/

#if defined(DM_BOARD_USE_USB_DEVICE) && defined(DM_BOARD_USE_USB_HOST)
  #error The hardware supports either USB Device or USB Host - not both at the same time
#endif
  
#if defined(DM_BOARD_USE_USBSERIAL_IN_RTOSLOG) && !defined(DM_BOARD_USE_USB_DEVICE)
  #error Cannot use USBSerial in RtosLog without DM_BOARD_USE_USB_DEVICE
#endif

#if defined(DM_BOARD_USE_TOUCH) && !defined(DM_BOARD_USE_DISPLAY)
  #error Cannot have touch controller without a display!
#endif

/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/

#if defined(DM_BOARD_DISABLE_STANDARD_PRINTF)
class DevNull : public Stream {
 
public:
    DevNull(const char *name=NULL) : Stream(name) {}
 
protected:
    virtual int _getc()      {return 0;}
    virtual int _putc(int c) {return c;}
};
 
static DevNull null("null");
#endif


/******************************************************************************
 * Local variables
 *****************************************************************************/

/******************************************************************************
 * Private Functions
 *****************************************************************************/

DMBoard::DMBoard() : 
    _initialized(false),
#if defined(DM_BOARD_USE_MCI_FS)
    _mcifs("mci", P4_16),
#endif
#if defined(DM_BOARD_USE_QSPI_FS)
    _qspifs("qspi"),
#endif
    _buzzer(P1_5),
    _button(P2_10),
    _led1(LED1),
    _led2(LED2),
    _led3(LED3),
    _led4(LED4)
{
}

DMBoard::~DMBoard()
{
}

/******************************************************************************
 * Public Functions
 *****************************************************************************/

DMBoard::BoardError DMBoard::init()
{
  BoardError err = Ok;
  if (!_initialized) {
    do {
      // Turn off the buzzer
      _buzzer.period_ms(1);
      _buzzer = 0;
      
      // Make sure the button is configured correctly
      _button.mode(PullUp);
    
      // Turn off all LEDs
      _led1 = 1;
      _led2 = 1;
      _led3 = 0;
      _led4 = 0;
        
      // Make sure that the logger is always initialized even if
      // other initialization tasks fail
      _logger.init();
        
#if defined(DM_BOARD_ENABLE_MEASSURING_PINS)
      _INTERNAL_INIT_MEAS();
#endif        

#if defined(DM_BOARD_DISABLE_STANDARD_PRINTF)
      // Kill all ouput of calls to printf() so that there is no
      // simultaneous calls into the non-thread-safe standard libraries.
      // User code should use the RtosLogger anyway.
      freopen("/null", "w", stdout);
#endif

#if defined(DM_BOARD_USE_QSPI) || defined(DM_BOARD_USE_QSPI_FS)
      if (SPIFI::instance().init() != SPIFI::Ok) {
        err = SpifiError;
        break;
      }
#endif
      
#if defined(DM_BOARD_USE_DISPLAY)
      if (BiosDisplay::instance().init() != Display::DisplayError_Ok) {
        err = DisplayError;
        break;
      }
#endif
      
#if defined(DM_BOARD_USE_TOUCH)
      if (BiosTouch::instance().init() != TouchPanel::TouchError_Ok) {
        err = TouchError;
        break;
      }
#endif

#if defined(DM_BOARD_USE_REGISTRY)
      if (Registry::instance().load() != Registry::Ok) {
        err = RegistryError;
        break;
      }
#endif
      _initialized = true;
    } while(0);
  }
  return err;
}

void DMBoard::setLED(Leds led, bool on)
{
  switch(led) {
    case Led1:
      _led1 = (on ? 0 : 1);
      break;
    case Led2:
      _led2 = (on ? 0 : 1);
      break;
    case Led3:
      _led3 = (on ? 1 : 0);
      break;
    case Led4:
      _led4 = (on ? 1 : 0);
      break;
  }
}

void DMBoard::buzzer(int frequency, int duration_ms)
{
  if (frequency <= 0) {
    _buzzer = 0;
  } else {
    _buzzer.period_us(1000000/frequency);
    _buzzer = 0.5;
  }
  if (duration_ms > 0) {
    Thread::wait(duration_ms);
    _buzzer = 0;
  }
}

bool DMBoard::buttonPressed()
{ 
  return _button.read() == 0;
}

TouchPanel* DMBoard::touchPanel()
{
#if defined(DM_BOARD_USE_TOUCH)
  if (BiosTouch::instance().isTouchSupported()) {
    return &BiosTouch::instance();
  }
#endif
  return NULL;
}

Display* DMBoard::display()
{
#if defined(DM_BOARD_USE_DISPLAY)
  return &BiosDisplay::instance();
#else
  return NULL;
#endif
}

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
#include "BiosTouch.h"
#include "BiosLoader.h"
#include "DMBoard.h"
#include "bios.h"
#include "meas.h"

/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/

#define NUM_COORDS  5
#define NUM_MAILS   10
typedef struct {
  touch_coordinate_t touch[NUM_COORDS];
  int num;
} touch_mail_t;

#define SIG_NEW_DATA 0x1
class TouchHandler {
    public:
        TouchHandler(bios_header_t* bios, void* biosData, int num) :
           _latest(NULL), _touchIRQ(P2_25), _bios(bios), 
           _biosData(biosData), _points(num),
           _listener(NULL), _lostData(0),
           _dbgAdded(0), _dbgRemoved(0) {}
        void handleTouchInterrupt();
        void handleNewData(touch_coordinate_t* coord, int num);
        void changeTouchInterrupt(bool enable, touch_irq_trigger_t trigger);
        TouchPanel::TouchError read(touch_coordinate_t* coord, int num);
        void run();
        FunctionPointer* setListener(FunctionPointer* listener);
    private:
        Mail<touch_mail_t, NUM_MAILS> _mailbox;
        Mutex _mutex;
        touch_coordinate_t* _latest;
        InterruptIn _touchIRQ;
        bios_header_t* _bios;
        void* _biosData;
        int _points;
        FunctionPointer* _listener;
        uint32_t _lostData;
        uint32_t _dbgAdded;
        uint32_t _dbgRemoved;
};

/******************************************************************************
 * Local variables
 *****************************************************************************/

/******************************************************************************
 * Private Functions
 *****************************************************************************/

BiosTouch::BiosTouch() : 
    _initialized(false),
    _haveInfo(false),
    _poweredOn(false),
    //_touchIRQ(P2_25),
    _bios(NULL),
    _biosData(NULL),
    _handlerThread(NULL),
    _handler(NULL),
    _supportsTouch(false)
{
}

BiosTouch::~BiosTouch()
{
  // _bios and _biosData are deallocated by BiosLoader
    
  if (_handlerThread != NULL) {
    delete _handlerThread;
    _handlerThread = NULL;
  }
  if (_handler != NULL) {
    delete _handler;
    _handler = NULL;
  }
}

// Function called from the BIOS
static void touchIrqEnabler(uint32_t arg, bool enable, touch_irq_trigger_t trigger)
{
  ((TouchHandler*)arg)->changeTouchInterrupt(enable, trigger);
}

// Function called from the BIOS
static void touchNewData(uint32_t arg, touch_coordinate_t* coords, int num)
{
  ((TouchHandler*)arg)->handleNewData(coords, num);
}

static void touchTask(void const* args)
{
  ((TouchHandler*)args)->run();
}


void TouchHandler::run()
{
  RtosLog* log = DMBoard::instance().logger();
      
  _latest = (touch_coordinate_t*)malloc(_points*sizeof(touch_coordinate_t));
  if (_latest == NULL) {
    log->printf("Failed to allocate memory for touch events\n");
    mbed_die();
  }
  memset(_latest, 0, _points*sizeof(touch_coordinate_t));
  if (_points > 1) {
    // multitouch - inject end event if missing
    uint32_t maxDelay = osWaitForever;
    while(true) {
      osEvent evt = _mailbox.get(maxDelay);
      if (evt.status == osEventMail) {
        touch_mail_t* mail = (touch_mail_t*)evt.value.p;
        memcpy(_latest, mail->touch, mail->num * sizeof(touch_coordinate_t));
        _mailbox.free(mail);
        _dbgRemoved++;
        maxDelay = 25; // wait up to 25ms for next event before injecting a "pen up" event
      } else if (evt.status == osEventTimeout) {
        int numNonZero = 0;
        for (int i = 0; i < _points; i++) {
          if (_latest[i].z > 0) {
            numNonZero++;
          }
          _latest[i].z = 0;    
        }
        maxDelay = osWaitForever;
        if (numNonZero == 0) {
          // last event was a pen-up event so no need to inject one
          //log->printf("skip penup\n");
          continue;            
        }
        //log->printf("inject penup\n");
      } else {
        log->printf("got non-mail event: 0x%x\n", evt.status);
        continue;
      }
      _mutex.lock();
      FunctionPointer* fp = _listener;
      _mutex.unlock();
    
      if (fp != NULL) {
        fp->call();
      }
    }
  } else {
    // normal singe-touch
    while(true) {
      osEvent evt = _mailbox.get(osWaitForever);
      if (evt.status == osEventMail) {
        touch_mail_t* mail = (touch_mail_t*)evt.value.p;
        memcpy(_latest, mail->touch, mail->num * sizeof(touch_coordinate_t));
        _mailbox.free(mail);
        _dbgRemoved++;
      } else {
        log->printf("got non-mail event: 0x%x\n", evt.status);
        continue;
      }
      _mutex.lock();
      FunctionPointer* fp = _listener;
      _mutex.unlock();
  
      if (fp != NULL) {
        fp->call();
      }
    }
  }
}

TouchPanel::TouchError TouchHandler::read(touch_coordinate_t* coord, int num)
{
  if (num > _points || num < 1) {
    return TouchPanel::TouchError_InvalidParam;
  }
  _mutex.lock();
  memcpy(coord, _latest, num*sizeof(touch_coordinate_t));
  _mutex.unlock();
  
  return TouchPanel::TouchError_Ok;
}

void TouchHandler::handleTouchInterrupt()
{
  _bios->touchIrqHandler(_biosData);
}

void TouchHandler::handleNewData(touch_coordinate_t* coord, int num)
{
  touch_mail_t* mail = _mailbox.alloc(0);
  if (mail == NULL) {
    //DMBoard::instance().logger()->printf("Lost touch event\n");
    _lostData++;
  } else {
    _dbgAdded++;
    mail->num = (num < NUM_COORDS) ? num : NUM_COORDS;
    memcpy(&mail->touch, coord, mail->num*sizeof(touch_coordinate_t));
    _mailbox.put(mail);
  }
}

void TouchHandler::changeTouchInterrupt(bool enable, touch_irq_trigger_t trigger)
{
  switch (trigger) {
    case TOUCH_IRQ_RISING_EDGE:
      if (enable) {
        _touchIRQ.rise(this, &TouchHandler::handleTouchInterrupt);
      } else {
        _touchIRQ.rise(NULL);
      }
      break;

    case TOUCH_IRQ_FALLING_EDGE:
      if (enable) {
        _touchIRQ.fall(this, &TouchHandler::handleTouchInterrupt);
      } else {
        _touchIRQ.fall(NULL);
      }
      break;
          
    case TOUCH_IRQ_HIGH_LEVEL:
    case TOUCH_IRQ_LOW_LEVEL:
    default:
      DMBoard::instance().logger()->printf("BIOS requests unknown trigger type %d\n", trigger);
      break;
  }
}

FunctionPointer* TouchHandler::setListener(FunctionPointer* listener)
{
  _mutex.lock();
  FunctionPointer* old = _listener;
  _listener = listener;
  _mutex.unlock();
  return old;
}


/******************************************************************************
 * Public Functions
 *****************************************************************************/

BiosTouch::TouchError BiosTouch::init()
{
  TouchError result = TouchError_Ok;
  if (!_initialized) {
    do {
      if (BiosLoader::instance().params(&_bios, &_biosData) != DMBoard::Ok) {
        result = TouchError_ConfigError;
        break;
      }
      
      result = (TouchError)_bios->touchInformation(_biosData, &_supportsTouch, &_supportsTouchCalibration, &_touchNumFingers);
      if (result != TouchError_Ok) {
        break;
      }
      _haveInfo = true;
      
      // is it supported at all?
      if (!_supportsTouch) {
        result = TouchError_TouchNotSupported;
        break;
      }
      
      _handler = new TouchHandler(_bios, _biosData, _touchNumFingers);
          
      result = (TouchError)_bios->touchInit(_biosData, touchIrqEnabler, (uint32_t)_handler, touchNewData, (uint32_t)_handler);
      if (result != TouchError_Ok) {
        break;
      }
      
      result = (TouchError)_bios->touchPowerUp(_biosData);
      if (result != TouchError_Ok) {
        break;
      }

      _handlerThread = new Thread(touchTask, _handler);

      _initialized = true;
    } while(0);
    
    if (!_initialized) {
      if (_handler != NULL) {
        delete _handler;
        _handler = NULL;
      }
    }
  }
  return result;
}

BiosTouch::TouchError BiosTouch::read(touch_coordinate_t &coord)
{
  TouchError err = TouchError_Ok;
  if (!_initialized) {
      err = TouchError_NoInit;
  } else {
    err = _handler->read(&coord, 1);
  }      
  return err;
}

BiosTouch::TouchError BiosTouch::read(touch_coordinate_t* coord, int num)
{
  TouchError err = TouchError_Ok;
  if (!_initialized) {
      err = TouchError_NoInit;
  } else {
    err = _handler->read(coord, num);
  }      
  return err;
}

BiosTouch::TouchError BiosTouch::info(bool* resistive, int* maxPoints, bool* calibrated)
{
  TouchError err = TouchError_Ok;
  if (!_haveInfo) {
      err = TouchError_NoInit;
  } else {
    *maxPoints = _touchNumFingers;
    *calibrated = _supportsTouchCalibration;
  }      
  return err;
}

bool BiosTouch::isTouchSupported()
{
#if defined(DM_BOARD_USE_TOUCH)
  if (_haveInfo) {
    return _supportsTouch;
  }
#endif
  return false;
}

BiosTouch::TouchError BiosTouch::calibrateStart()
{
  TouchError err = TouchError_Ok;
  if (!_initialized) {
      err = TouchError_NoInit;
  } else {
    err = (TouchError)_bios->touchCalibrateStart(_biosData);
  }      
  return err;
}

BiosTouch::TouchError BiosTouch::getNextCalibratePoint(uint16_t* x, uint16_t* y, bool* last)
{
  TouchError err = TouchError_Ok;
  if (!_initialized) {
      err = TouchError_NoInit;
  } else {
    err = (TouchError)_bios->touchGetNextCalibPoint(_biosData, x, y, last);
  }      
  return err;
}

BiosTouch::TouchError BiosTouch::waitForCalibratePoint(bool* morePoints, uint32_t timeout)
{
  TouchError err = TouchError_Ok;
  if (!_initialized) {
      err = TouchError_NoInit;
  } else {
    err = (TouchError)_bios->touchWaitForCalibratePoint(_biosData, morePoints, timeout);
  }      
  return err;
}

FunctionPointer* BiosTouch::setListener(FunctionPointer* listener)
{
  if (_initialized) {
    return _handler->setListener(listener);
  }
  return NULL;
}

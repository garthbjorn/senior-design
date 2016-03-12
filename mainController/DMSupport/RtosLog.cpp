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
#include "RtosLog.h"
#include <cstdarg>

/******************************************************************************
 * Defines and typedefs
 *****************************************************************************/

 
/******************************************************************************
 * Local variables
 *****************************************************************************/

/******************************************************************************
 * Private Functions
 *****************************************************************************/

void RtosLog::logTask(void const* args)
{
    RtosLog* instance = (RtosLog*)args;
    
    while (true) {
        osEvent evt = instance->_queue.get();
        if (evt.status == osEventMessage) {
            message_t *message = (message_t*)evt.value.p;
            if (message->ptr != NULL) {
                instance->_serial.printf(message->ptr);
                free(message->ptr);
            } else {
                instance->_serial.printf(message->msg);
            }
            
            instance->_mpool.free(message);
            
            // Increase the number of available messages in the pool
            instance->_sem.release();
        }
    }    
}

/******************************************************************************
 * Public Functions
 *****************************************************************************/

#if defined(DM_BOARD_USE_USBSERIAL_IN_RTOSLOG)
    RtosLog::RtosLog() :
        _sem(NumMessages), _serial(), _thr(NULL)
    {
    }
#else
RtosLog::RtosLog() :
    _sem(NumMessages), _serial(USBTX, USBRX), _thr(NULL)
{
#if defined(DM_BOARD_USE_FAST_UART)
    // This works because both the default serial (used by printf) and the s instance
    // (used by s.printf) would use the same underlying UART code so setting the baudrate
    // in one affects the other.
    _serial.baud(115200);
#endif
}
#endif

RtosLog::~RtosLog()
{
    if (_thr != NULL) {
        _thr->terminate();
        delete _thr;
        _thr = NULL;
    }
}

void RtosLog::init()
{
    if (_thr == NULL) {
        _thr = new Thread(&RtosLog::logTask, this);
    }
}

int RtosLog::printf(const char* format, ...)
{
    // The pool has no "wait for free message" so we use a Sempahore
    // to keep track of the number of free messages and, if needed,
    // block the caller until a message is free
    _sem.wait();
    
    // Allocate a null terminated message. Will always succeed due to
    // the semaphore above
    message_t *message = _mpool.calloc();
    
    // Write the callers formatted message 
    std::va_list args;
    va_start(args, format);
    int ret = vsnprintf(message->msg, MessageLen, format, args);
    va_end(args);
    
    // If the entire message could not fit in the preallocated buffer
    // then allocate a new one and try again.
    if (ret > MessageLen) {
        message->ptr = (char*)malloc(ret + 1);
        if (message->ptr != NULL) {
            va_start(args, format);
            ret = vsnprintf(message->ptr, ret + 1, format, args);
            va_end(args);
        }
    }
    
    // Send message
    _queue.put(message);
    
    // Note that the Semaphore is not released here, that is done after
    // the message has been processed and released into the pool by
    // logTask()
    //_sem.release();
    
    return ret;
}

int RtosLog::isr_printf(const char* format, ...)
{
    // The pool has no "wait for free message" so we use a Sempahore
    // to keep track of the number of free messages and, if needed,
    // block the caller until a message is free
    int available = _sem.wait(0);
    if (available <= 0) {
      // no free messages and it is not good to wait in an ISR so
      // we discard the message
      return 0;
    }
    
    // Allocate a null terminated message. Will always succeed due to
    // the semaphore above
    message_t *message = _mpool.calloc();
    
    // Write the callers formatted message 
    std::va_list args;
    va_start(args, format);
    int ret = vsnprintf(message->msg, MessageLen, format, args);
    va_end(args);
    
    // If the entire message could not fit in the preallocated buffer
    // then allocate a new one and try again.
    if (ret > MessageLen) {
        message->ptr = (char*)malloc(ret + 1);
        if (message->ptr != NULL) {
            va_start(args, format);
            ret = vsnprintf(message->ptr, ret + 1, format, args);
            va_end(args);
        }
    }
    
    // Send message
    _queue.put(message);
    
    // Note that the Semaphore is not released here, that is done after
    // the message has been processed and released into the pool by
    // logTask()
    //_sem.release();
    
    return ret;
}

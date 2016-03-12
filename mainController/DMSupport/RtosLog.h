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

#ifndef RTOSLOG_H
#define RTOSLOG_H

#include "mbed.h"
#include "rtos.h"
#include "dm_board_config.h"

#if defined(DM_BOARD_USE_USBSERIAL_IN_RTOSLOG)
  #include "USBSerial.h"
#endif

/**
 * All threads can independantly call the printf function in the RtosLog class
 * without risk of getting the output tangled up.
 *
 * The RtosLog class uses a Thread to process the messages one at a time.
 *
 *
 * Example of using the RtosLog class:
 *
 * @code
 * #include "mbed.h"
 * #include "DMBoard.h"
 *
 * void myTask(void const*args) {
 *    RtosLog* log = DMBoard::instance().logger();
 *    ...
 *    log->printf("Hello world!\n");
 *    ...
 * }
 * @endcode
 *
 */
class RtosLog {
public:
    enum Constants {
        MessageLen  = 80,
        NumMessages =  16,
    };
  
    RtosLog();
    ~RtosLog();
    
    /** Starts the logger thread, called from DMBoard::instance().init()
     */
    void init();
    
    /** Printf that works in an RTOS
     *
     *  This function will create a string from the specified format and
     *  optional extra arguments and put it into a message that the RtosLog
     *  thread will write to the log.
     *
     *  Note that if the underlying queue is full then this function
     *  will block until an entry becomes available. This is required to 
     *  make sure that all printf calls actually get printed. If this happens
     *  too often then increase the priority of the RtosLog thread or reduce
     *  the number of printed messages.
     *
     *  @param format  format string
     *  @param ...     optional extra arguments
     */
    int printf(const char* format, ...);
    
    /** Printf that works in an RTOS
     *
     *  This function will create a string from the specified format and
     *  optional extra arguments and put it into a message that the RtosLog
     *  thread will write to the log.
     *
     *  Note that if the underlying queue is full then this function
     *  discards the message and returns immediately.
     *
     *  @param format  format string
     *  @param ...     optional extra arguments
     */
    int isr_printf(const char* format, ...);
  
private:
    
    typedef struct {
        char*  ptr;                 /* Non-NULL if memory is allocated */
        char   msg[MessageLen+1];   /* A counter value                 */
    } message_t;
 
    MemoryPool<message_t, NumMessages> _mpool;
    Queue<message_t, NumMessages> _queue;
    Semaphore _sem;
#if defined(DM_BOARD_USE_USBSERIAL_IN_RTOSLOG)
    USBSerial _serial;
#else
    Serial _serial;
#endif
    Thread* _thr;
    
    static void logTask(void const* args);
};

#endif

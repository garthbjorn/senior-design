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

#ifndef _MEAS_H_
#define _MEAS_H_

/******************************************************************************
 * Includes
 *****************************************************************************/

#include "mbed.h"
#include "dm_board_config.h"

/******************************************************************************
 * Typedefs and defines
 *****************************************************************************/

// Enable extra measuring pins, at the expense of peripheral access.
// For example MEAS pins 5-9 will prevent use of UART4/SPI2 and some timers.
//#define EXTRA_PINS

// Internal macros, don't use directly
#if defined(DM_BOARD_ENABLE_MEASSURING_PINS)
  #define _INTERNAL_SET_MEAS_PIN(__port, __pin)    ((LPC_GPIO ## __port)->SET = (1UL << (__pin)))
  #define _INTERNAL_CLR_MEAS_PIN(__port, __pin)    ((LPC_GPIO ## __port)->CLR = (1UL << (__pin)))
#else
  #define _INTERNAL_SET_MEAS_PIN(__port, __pin)    do {} while(0)
  #define _INTERNAL_CLR_MEAS_PIN(__port, __pin)    do {} while(0)
#endif

// Manipulates measurement pin 1 which is GPIO1[24], available on J10-7
#define SET_MEAS_PIN_1()     _INTERNAL_SET_MEAS_PIN(1, 24)
#define CLR_MEAS_PIN_1()     _INTERNAL_CLR_MEAS_PIN(1, 24)

// Manipulates measurement pin 2 which is GPIO1[23], available on J10-8
#define SET_MEAS_PIN_2()     _INTERNAL_SET_MEAS_PIN(1, 23)
#define CLR_MEAS_PIN_2()     _INTERNAL_CLR_MEAS_PIN(1, 23)

// Manipulates measurement pin 3 which is GPIO1[20], available on J10-9
#define SET_MEAS_PIN_3()     _INTERNAL_SET_MEAS_PIN(1, 20)
#define CLR_MEAS_PIN_3()     _INTERNAL_CLR_MEAS_PIN(1, 20)

// Manipulates measurement pin 4 which is GPIO1[19], available on J10-10
#define SET_MEAS_PIN_4()     _INTERNAL_SET_MEAS_PIN(1, 19)
#define CLR_MEAS_PIN_4()     _INTERNAL_CLR_MEAS_PIN(1, 19)

#ifdef EXTRA_PINS
    // Manipulates measurement pin 5 which is GPIO5[0], available on J10-15
    #define SET_MEAS_PIN_5()     _INTERNAL_SET_MEAS_PIN(5, 0)
    #define CLR_MEAS_PIN_5()     _INTERNAL_CLR_MEAS_PIN(5, 0)
    
    // Manipulates measurement pin 6 which is GPIO5[1], available on J10-16
    #define SET_MEAS_PIN_6()     _INTERNAL_SET_MEAS_PIN(5, 1)
    #define CLR_MEAS_PIN_6()     _INTERNAL_CLR_MEAS_PIN(5, 1)
    
    // Manipulates measurement pin 7 which is GPIO5[2], available on J10-17
    #define SET_MEAS_PIN_7()     _INTERNAL_SET_MEAS_PIN(5, 2)
    #define CLR_MEAS_PIN_7()     _INTERNAL_CLR_MEAS_PIN(5, 2)
    
    // Manipulates measurement pin 8 which is GPIO5[3], available on J10-18
    #define SET_MEAS_PIN_8()     _INTERNAL_SET_MEAS_PIN(5, 3)
    #define CLR_MEAS_PIN_8()     _INTERNAL_CLR_MEAS_PIN(5, 3)
    
    // Manipulates measurement pin 9 which is GPIO5[4], available on J10-19
    #define SET_MEAS_PIN_9()     _INTERNAL_SET_MEAS_PIN(5, 4)
    #define CLR_MEAS_PIN_9()     _INTERNAL_CLR_MEAS_PIN(5, 4)
    
    #define _INTERNAL_EXTRA_INIT_MEAS() do {\
        LPC_IOCON->P5_0 &= ~0x7; \
        LPC_IOCON->P5_1 &= ~0x7; \
        LPC_IOCON->P5_2 &= ~0x7; \
        LPC_IOCON->P5_3 &= ~0x7; \
        LPC_IOCON->P5_4 &= ~0x7; \
        LPC_GPIO5->DIR |= (1<<0) | (1<<1) | (1<<2) | (1<<3) | (1<<4); \
        CLR_MEAS_PIN_5(); \
        CLR_MEAS_PIN_6(); \
        CLR_MEAS_PIN_7(); \
        CLR_MEAS_PIN_8(); \
        CLR_MEAS_PIN_9(); \
      } while(0)
#else
    #define _INTERNAL_EXTRA_INIT_MEAS() do {} while(false)
#endif

#if defined(DM_BOARD_ENABLE_MEASSURING_PINS)
  #define _INTERNAL_INIT_MEAS() do {\
      LPC_IOCON->P1_19 &= ~0x7; \
      LPC_IOCON->P1_20 &= ~0x7; \
      LPC_IOCON->P1_23 &= ~0x7; \
      LPC_IOCON->P1_24 &= ~0x7; \
      LPC_GPIO1->DIR |= (1<<19) | (1<<20) | (1<<23) | (1<<24); \
      CLR_MEAS_PIN_1(); \
      CLR_MEAS_PIN_2(); \
      CLR_MEAS_PIN_3(); \
      CLR_MEAS_PIN_4(); \
      _INTERNAL_EXTRA_INIT_MEAS(); \
    } while(0)
#else
  #define _INTERNAL_INIT_MEAS()                    do {} while(0)
#endif

/******************************************************************************
 * Global Variables
 *****************************************************************************/

#endif /* end _MEAS_H_ */


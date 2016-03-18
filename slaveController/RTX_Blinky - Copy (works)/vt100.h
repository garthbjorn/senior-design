/*
 * vt100.h
 *
 *  Author: Wesley
 */

#ifndef VT100_H_
#define VT100_H_

#include <stdint.h>
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_usart.h"
#include "f0_usart.h"

void VT100_init();
void update_rawADC(double vin);
void update_degF(double vin);
void update_degC(double vin);

#endif /* VT100_H_ */

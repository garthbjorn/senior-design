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

void VT100_init(USART_TypeDef* USARTx);
void update_rawADC(USART_TypeDef* USARTx, uint16_t adc);
void update_degF(USART_TypeDef* USARTx, uint16_t F);
void update_degC(USART_TypeDef* USARTx, int16_t C);

#endif /* VT100_H_ */


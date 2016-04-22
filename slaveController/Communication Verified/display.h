
#ifndef DISPLAY_H_
#define DISPLAY_H_

#include "stm32f0xx.h"

void display_init(USART_TypeDef* USARTx);
void update_rawADC(USART_TypeDef* USARTx, uint16_t adc);
void update_degF(USART_TypeDef* USARTx, uint16_t F);
void update_degC(USART_TypeDef* USARTx, int16_t C);

#endif /* DISPLAY_H_ */

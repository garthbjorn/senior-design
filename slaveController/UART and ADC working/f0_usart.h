
#ifndef F4_USART_H_
#define F4_USART_H_

#include "stm32f0xx.h"

void USART_Output_Configure(void);
void USART_Configure(void);
void usart_f0_init(void);
void USART_PUT_TEMP(USART_TypeDef* USARTx, uint32_t t);
void USART_putnum(USART_TypeDef* USARTx, uint32_t x);
void USART_putchar(USART_TypeDef* USARTx, char c);
void USART_puts(USART_TypeDef* USARTx, volatile char * s);


#endif /* PART_USART_H_ */

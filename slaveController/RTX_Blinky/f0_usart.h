
#ifndef F4_USART_H_
#define F4_USART_H_

#include "stm32f0xx.h"

void NVIC_USART_Configure(void);
void USART1_Configure(void);
void USART2_Configure(void);
void USART_GPIO(void);
void usart_f0_init(void);
char getRx(void);
void USART_PUT_TEMPC(USART_TypeDef* USARTx, uint32_t t);
void USART_PUT_TEMPF(USART_TypeDef* USARTx, uint32_t t);
void USART_putnum(USART_TypeDef* USARTx, uint32_t x);
void USART_putchar(USART_TypeDef* USARTx, char c);
void USART_puts(USART_TypeDef* USARTx, volatile char * s);


#endif /* PART_USART_H_ */

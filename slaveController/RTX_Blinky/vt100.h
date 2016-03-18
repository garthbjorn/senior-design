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


#define VT100_CLEAR						USART_puts(USART1,"\x1b[2J");
#define VT100_HOME						USART_puts(USART1,"\x1b[H");
#define VT100_HIDE_CURSOR			USART_puts(USART1,"\x1b[?25l");
#define VT100_INIT						{VT100_HOME;VT100_CLEAR;VT100_HIDE_CURSOR;}
#define VT100_POS(row,col)		USART_puts(USART1,"\x1b[" #row ";" #col "H")

//#define VT100_UP_RIGHT(row,col)		USART_puts(USART1,"\x1b[%dA\033[%dD",row,col);
//#define VT100_UP_LEFT(row,col)		USART_puts(USART1,"\x1b[%dA\033[%dC",row,col);
//#define VT100_DOWN_RIGHT(row,col)	USART_puts(USART1,"\x1b[%dB\033[%dD",row,col);
//#define VT100_DOWN_LEFT(row,col)	USART_puts(USART1,"\x1b[%dB\033[%dC",row,col);

//#define PRINTS(arg)						uart_prints(arg)
//#define VT100_prints(row,col,str)		printf("\x1b[" #row ";" #col "H" str)
//#define VT100_printf(row,col,fmt,...)	printf("\x1b[" #row ";" #col "H" fmt, __VA_ARGS__)

#endif /* VT100_H_ */


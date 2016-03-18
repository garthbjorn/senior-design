/* ECE433
 * Spring 2016
 * Wesley Butler*/
 
/* VT100
 * for P3 */

#include <stdint.h>
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_usart.h"
//#include <avr/io.h>
#include "vt100.h"
#include "f0_usart.h"

#define VT100_CLEAR					USART_puts(USART1,"\x1b[2J");
#define VT100_HOME					USART_puts(USART1,"\x1b[H");
#define VT100_HIDE_CURSOR			USART_puts(USART1,"\x1b[?25l");
//#define VT100_UP_RIGHT(row,col)		USART_puts(USART1,"\x1b[%dA\033[%dD",row,col);
//#define VT100_UP_LEFT(row,col)		USART_puts(USART1,"\x1b[%dA\033[%dC",row,col);
//#define VT100_DOWN_RIGHT(row,col)	USART_puts(USART1,"\x1b[%dB\033[%dD",row,col);
//#define VT100_DOWN_LEFT(row,col)	USART_puts(USART1,"\x1b[%dB\033[%dC",row,col);

void update_rawADC(USART_TypeDef* USARTx, uint16_t adc){
	//VT100_DOWN_LEFT(3,12);
	USART_puts(USARTx, "\x1b[3B\x1b[12C");
	if(adc<1000)
		USART_puts(USARTx, " ");
	if(adc<100)
		USART_puts(USARTx, " ");
	if(adc<10)
		USART_puts(USARTx, " ");
	USART_putnum(USARTx, adc);
	VT100_HOME;
}

void update_degF(USART_TypeDef* USARTx, uint16_t F){
	//VT100_DOWN_LEFT(5,12);
	USART_puts(USARTx, "\x1b[5B\x1b[12C");
	if(F<1000)
		USART_puts(USARTx, " ");
	if(F<100)
		USART_puts(USARTx, " ");
	if(F<10)
		USART_puts(USARTx, " ");
	USART_putnum(USARTx, F);
	VT100_HOME;
}

void update_degC(USART_TypeDef* USARTx, int16_t C){
	//VT100_DOWN_LEFT(7,12);
	if(C<0) //range normally goes down to -18C, but I can't print that...
		C=0;
	USART_puts(USARTx, "\x1b[7B\x1b[12C");
	//printf("%.2f",vin);
	if(C<1000)
		USART_puts(USARTx, " ");
	if(C<100)
		USART_puts(USARTx, " ");
	if(C<10)
		USART_puts(USARTx, " ");
	USART_putnum(USARTx, C);
	VT100_HOME;
	USART_puts(USARTx, "\x1b[9B\x1b[12C");
	USART_PUT_TEMP(USARTx, C);
	VT100_HOME;
}

void VT100_init(USART_TypeDef* USARTx){
	VT100_CLEAR;
	VT100_HOME;
	VT100_HIDE_CURSOR;
	
	USART_puts(USARTx, "|''''''''''''''''''''''''''''''''''|\n\r");
	USART_puts(USARTx, "|=== ADC_Testing  -  Wes Butler ===|\n\r");
	USART_puts(USARTx, "|                                  |\n\r");
	USART_puts(USARTx, "| Raw ADC =                        |\n\r");
	USART_puts(USARTx, "|                                  |\n\r");
	USART_puts(USARTx, "| degr. F =                        |\n\r");
	USART_puts(USARTx, "|                                  |\n\r");
	USART_puts(USARTx, "| degr. C =                        |\n\r");
	USART_puts(USARTx, "|                                  |\n\r");
	USART_puts(USARTx, "|  USART1 =                        |\n\r");
	USART_puts(USARTx, "|__________________________________|\n\r");
	
	VT100_HOME;
	
	update_rawADC(USARTx, 0);
	update_degF(USARTx, 0);
	update_degC(USARTx, 0);
}

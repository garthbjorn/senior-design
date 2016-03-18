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

void update_rawADC(double vin){
	//VT100_DOWN_LEFT(3,12);
	USART_puts(USART1, "\x1b[3B\x1b[12C");
	USART_putnum(USART1, vin);
	VT100_HOME;
}

void update_degF(double vin){
	//VT100_DOWN_LEFT(5,12);
	USART_puts(USART1, "\x1b[5B\x1b[12C");
	USART_putnum(USART1, vin);
	VT100_HOME;
}

void update_degC(double vin){
	//VT100_DOWN_LEFT(7,12);
	USART_puts(USART1, "\x1b[7B\x1b[12C");
	//printf("%.2f",vin);
	USART_putnum(USART1, vin);
	VT100_HOME;
}

void VT100_init(){
	VT100_CLEAR;
	VT100_HOME;
	VT100_HIDE_CURSOR;
	
	USART_puts(USART1, "|''''''''''''''''''''''''''''''''''|\n\r");
	USART_puts(USART1, "|=== ADC_Testing  -  Wes Butler ===|\n\r");
	USART_puts(USART1, "|                                  |\n\r");
	USART_puts(USART1, "| Raw ADC =                        |\n\r");
	USART_puts(USART1, "|                                  |\n\r");
	USART_puts(USART1, "| degr. F =                        |\n\r");
	USART_puts(USART1, "|                                  |\n\r");
	USART_puts(USART1, "| degr. C =                        |\n\r");
	USART_puts(USART1, "|__________________________________|\n\r");
	
	VT100_HOME;
	
	update_rawADC(0);
	update_degF(0);
	update_degC(0);
}


#include "stm32f0xx_gpio.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_misc.h"
#include <stdio.h>
#include "f0_usart.h"

void usart_f0_init()
{
	NVIC_Configure();
	USART_Configure();
	USART_Output_Configure();
}

void NVIC_Configure(){
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void USART_Configure()
{
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);
  
	USART_Cmd(USART2, ENABLE);
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
}

void USART_Output_Configure()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_0);
  
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void USART_PUT_TEMPC(USART_TypeDef* USARTx, uint32_t t){
		USART_putchar(USARTx, 'c');
		if(t<1000)
			USART_putnum(USARTx, 0);
		if(t<100)
			USART_putnum(USARTx, 0);
		if(t<10)
			USART_putnum(USARTx, 0);
		USART_putnum(USARTx, t);
		USART_putchar(USARTx, '\n');
}	

void USART_PUT_TEMPF(USART_TypeDef* USARTx, uint32_t t){
		USART_putchar(USARTx, 'f');
		if(t<1000)
			USART_putnum(USARTx, 0);
		if(t<100)
			USART_putnum(USARTx, 0);
		if(t<10)
			USART_putnum(USARTx, 0);
		USART_putnum(USARTx, t);
		USART_putchar(USARTx, '\n');
}	

void USART_putnum(USART_TypeDef* USARTx, uint32_t x)
{
	char value[10]; //a temp array to hold results of conversion
	int i = 0; //loop index
  
	do
	{
		value[i++] = (char)(x % 10) + '0'; //convert integer to character
		x /= 10;
	} while (x);
  
	while (i) //send data
	{
		USART_putchar(USARTx, value[--i]); 
	}
}

void USART_putchar(USART_TypeDef* USARTx, char c)
{
	while (!(USARTx->ISR & 0x00000040));
		USART_SendData(USARTx, c);
}

void USART_puts(USART_TypeDef* USARTx, volatile char * s) {
	while (*s) {
		USART_putchar(USARTx, *s++);
	}
}

//char Rx_indx, Rx_data[2], Rx_Buffer[100], Transfer_cplt;
#define MAX_STRLEN 12 // this is the maximum string length of our string in characters
volatile char received_string[MAX_STRLEN+1]; // this will hold the recieved string



//This function handles USART2 global interrupt request.
void USART2_IRQHandler(void)
{	
	USART_putchar(USART2, USART2->RDR);
	
	
	// check if the USART1 receive interrupt flag was set
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){
		//(USART2->ISR & USART_FLAG_RXNE) != (uint16_t)RESET){
		
		//static uint8_t cnt = 0; // this counter is used to determine the string length
		char t = USART2->RDR; // the character from the USART1 data register is saved in t

		USART_putchar(USART2, t);
		
		/* check if the received character is not the LF character (used to determine end of string) 
		 * or the if the maximum string length has been been reached 
		 */
//		if( (t != '\n') && (cnt < MAX_STRLEN) ){ 
//			received_string[cnt] = t;
//			cnt++;
//		}
//		else{ // otherwise reset the character counter and print the received string
//			cnt = 0;
//			USART_puts(USART2, received_string);
//		}
	}
}












////FILE mystdout = FDEV_SETUP_STREAM(USART_putchar, NULL, _FDEV_SETUP_WRITE);
//
//#ifdef __GNUC__
  /////* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     ////set to 'Yes') calls __io_putchar() */
	//#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
//#else
	//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
//#endif /* __GNUC__ */
//
///**
  //* @brief  Retargets the C library printf function to the USART.
  //* @param  None
  //* @retval None
  //*/
//PUTCHAR_PROTOTYPE
//{
  ///* Place your implementation of fputc here */
  ///* e.g. write a character to the USART */
	////USART_SendData(USART1, (uint8_t) ch);
	////while (!(USART1->ISR & 0x00000040));
	//while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
		//;
		//USART_SendData(USART1, ch);
//
	  ///* Loop until transmit data register is empty */
	////while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
		////{}
//
	//return ch;
//}




///* USART2 PA.2 Tx, PA.3 Rx STM32F0-Discovery sourcer32@gmail.com */
 //
//#include "stm32f0xx.h"
//#include <stm32f0xx_gpio.h>
//#include <stm32f0xx_rcc.h>
//#include <stm32f0xx_usart.h>
////#include "stm32f0_discovery.h"
 //
//int main(void)
//{
	//USART_InitTypeDef USART_InitStructure;
	//GPIO_InitTypeDef GPIO_InitStructure;
 //
	//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
 //
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
 //
	//GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
	//GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
 //
    ///* Configure USART2 pins:  Rx and Tx ----------------------------*/
	//GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3;
	//GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	//GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	//GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	//GPIO_Init(GPIOA, &GPIO_InitStructure);
 //
	//USART_InitStructure.USART_BaudRate = 9600;
	//USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	//USART_InitStructure.USART_StopBits = USART_StopBits_1;
	//USART_InitStructure.USART_Parity = USART_Parity_No;
	//USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	//USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	//USART_Init(USART2, &USART_InitStructure);
 //
	//USART_Cmd(USART2, ENABLE);
	//USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
 //
	//while (1)
	//{
		////while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
 //
			//USART_SendData(USART2, 'X');
	//}
//}


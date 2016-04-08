
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_misc.h"
#include <stdio.h>
#include "f0_usart.h"
#include "f0_adc.h"
#include "display.h"

void usart_f0_init()
{
	NVIC_USART_Configure();
	//USART1_Configure();
	USART2_Configure();
	USART_GPIO();
}

void NVIC_USART_Configure(){
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void USART1_Configure()
{
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
  
	USART_Cmd(USART1, ENABLE);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

void USART2_Configure()
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

void USART_GPIO()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
  //USART2
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
	//USART1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
  
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void USART_PUT_TEMPC(USART_TypeDef* USARTx, uint32_t t){
		USART_putchar(USARTx, 'c');
		USART_putnum(USARTx, t);
		USART_putchar(USARTx, 'n');
}	

void USART_PUT_TEMPF(USART_TypeDef* USARTx, uint32_t t){
		USART_putchar(USARTx, 'f');
		USART_putnum(USARTx, t);
		USART_putchar(USARTx, 'n');
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
	//while (!(USARTx->ISR & 0x00000040));
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)!=SET);
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

char received;

char getRx(void){
	return received;
}

uint16_t C = 0;
uint16_t F = 0;

//This function handles USART2 global interrupt request.
void USART2_IRQHandler(void)
{	
	//USART_putchar(USART2, USART2->RDR);
	
	
	// check if the USART1 receive interrupt flag was set
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){
		//(USART2->ISR & USART_FLAG_RXNE) != (uint16_t)RESET){
		
		//static uint8_t cnt = 0; // this counter is used to determine the string length
		//char t = USART2->RDR; // the character from the USART1 data register is saved in t
		
//		received = USART_ReceiveData(USART2);
		
		//USART_putchar(USART2, received);
		
		if(ADCvalue() > 25){
			C = ADCvalue()*0.732-18;
		}else{
			C = 0;
		}
		F = 9/5*C+32;
		
		if(USART_ReceiveData(USART2) == 'f'){
				USART_PUT_TEMPF(USART2, F);	//degF 0-2500F
		}
		if(USART_ReceiveData(USART2) == 'c'){
				USART_PUT_TEMPC(USART2, C);		//degC 0-1370C
		}
		
//		USART_putchar(USART2, USART_ReceiveData(USART2));
		
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

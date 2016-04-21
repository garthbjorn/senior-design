
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_misc.h"
#include <stdio.h>
#include "f0_usart.h"
#include "f0_adc.h"
#include "display.h"
#include "valves.h"

extern uint8_t go;

void usart_f0_init(){
	NVIC_USART1_Configure();
	NVIC_USART2_Configure();
	USART1_Configure();
	USART2_Configure();
	USART_GPIO();
}

void NVIC_USART1_Configure(){
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void NVIC_USART2_Configure(){
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

void USART1_Configure(){
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

void USART2_Configure(){
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

void USART_GPIO(){
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	
  //USART2
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1); //TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1); //RX
	//USART1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1); //TX
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1); //RX
  
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

void USART_putnum(USART_TypeDef* USARTx, uint32_t x){
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

void USART_putchar(USART_TypeDef* USARTx, char c){
	//while (!(USARTx->ISR & 0x00000040));
	while(USART_GetFlagStatus(USARTx,USART_FLAG_TXE)!=SET);
		USART_SendData(USARTx, c);
}

void USART_puts(USART_TypeDef* USARTx, volatile char * s) {
	while (*s) {
		USART_putchar(USARTx, *s++);
	}
}



uint8_t value1[32] = {0};
static uint8_t readChar1 = 0;
static int index1 = 0;
uint8_t ret = '\r';

//This function handles USART2 global interrupt request.
void USART1_IRQHandler(void)
{	
	int num = 0;
	int i = 0;
	// check if the USART1 receive interrupt flag was set
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
		readChar1 = USART_ReceiveData(USART1);
		
		USART_putchar(USART2,	readChar1); //For testing use ONLY
		
		switch(readChar1){
			case 'w': // Step OPEN air
				set_dir_air(0);
				step_air(1);
				break;
			case 's': //Step CLOSE air
				set_dir_air(1);
				step_air(1);
				break;
			case 'e': // Step OPEN gas
				set_dir_gas(0);
				step_gas(1);
				break;
			case 'd': // Step CLOSE gas
				set_dir_gas(1);
				step_gas(1);
				break;
			case 'x':	//open DC valves
				open_air();
				open_gas();
				break;
			case 'c':	//close DC valves
				close_air();
				close_gas();
				break;
			case '\r': //enter
				USART_putchar(USART2,	'\n');
				break;
			default:
				break;
		}
		
		value1[index1++] = readChar1;
		if(readChar1 == '\r'){
			switch(value1[0]){
				case 'a': //Air
					switch(value1[1]){
						case 'p':
							set_dir_air(0);
							num = 0;
							i = 2;
							while(value1[i] != '\r'){
								if(value1[i] == '\b') break;
								num *= 10;
								num += value1[i] - 0x30;
								i++;
							}
							step_air(num);
							break;
						case 'n':
							set_dir_air(1);
							num = 0;
							i = 2;
							while(value1[i] != '\r'){
								if(value1[i] == '\b') break;
								num *= 10;
								num += value1[i] - 0x30;
								i++;
							}
							step_air(num);
							break;
						default:
							break;
					}
					
					//USART_puts(USART2, "you tried to do an air routine");
					break;
				case 'g': //Gas
					switch(value1[1]){
						case 'p':
							set_dir_gas(0);
							num = 0;
							i = 2;
							while(value1[i] != '\r'){
								if(value1[i] == '\b') break;
								num *= 10;
								num += value1[i] - 0x30;
								i++;
							}
							step_gas(num);
							break;
						case 'n':
							set_dir_gas(1);
							num = 0;
							i = 2;
							while(value1[i] != '\r'){
								if(value1[i] == '\b') break;
								num *= 10;
								num += value1[i] - 0x30;
								i++;
							}
							step_gas(num);
							break;
						default:
							break;
					}
					
					//USART_puts(USART2, "you tried to do an gas routine");
					break;
				default:
					break;
			}
			index1 = 0;
		}
		readChar1 = 0;
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

uint8_t value[32] = {0};
static uint8_t readChar = 0;
static int index = 0;
uint16_t levels = 0;
uint32_t steps[10] = {0};
uint32_t times[10] = {0};
uint8_t newline = 'n';

//This function handles USART2 global interrupt request.
void USART2_IRQHandler(void)
{	
	int num = 0;
	int i = 0;
	// check if the USART1 receive interrupt flag was set
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){
		readChar = USART_ReceiveData(USART2);
		
		switch(readChar){
			case 'c':
				if(ADCvalue() > 25){
					C = ADCvalue()*0.732-18;
				}else{
					C = 0;
				}
				USART_PUT_TEMPC(USART2, C);		//degC 0-1370C
				break;
			default:
				break;
		}
		value[index++] = readChar;
		if(readChar == newline){
			if(value[1] == 'z'){
				switch(value[0]){
					case 'p': //Profile pts
						num = 0;
						i = 2;
						while(value[i] != newline){
							num *= 10;
							num += value[i] - 0x30;
							i++;
						}
						levels = num;
						GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_SET);
						break;
					case 's': //Steps
						switch(value[3]){
							case '0':
								num = 0;
								i = 3;
								while(value[i] != newline){
									num *= 10;
									num += value[i] - 0x30;
									i++;
								}
								steps[0] = num;
								GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_SET);
								break;
							case '1':
								num = 0;
								i = 3;
								while(value[i] != newline){
									num *= 10;
									num += value[i] - 0x30;
									i++;
								}
								steps[1] = num;
								GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_SET);
								break;
							case '2':
								num = 0;
								i = 3;
								while(value[i] != newline){
									num *= 10;
									num += value[i] - 0x30;
									i++;
								}
								steps[2] = num;
								GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_SET);
								break;
							case '3':
								num = 0;
								i = 3;
								while(value[i] != newline){
									num *= 10;
									num += value[i] - 0x30;
									i++;
								}
								steps[3] = num;
								GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_SET);
								break;
							case '4':
								num = 0;
								i = 3;
								while(value[i] != newline){
									num *= 10;
									num += value[i] - 0x30;
									i++;
								}
								steps[4] = num;
								GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_SET);
								break;
							case '5':
								num = 0;
								i = 3;
								while(value[i] != newline){
									num *= 10;
									num += value[i] - 0x30;
									i++;
								}
								steps[5] = num;
								GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_SET);
								break;
						}
						break;
					case 't': //Time
						switch(value[3]){
							case '0':
								num = 0;
								i = 3;
								while(value[i] != newline){
									num *= 10;
									num += value[i] - 0x30;
									i++;
								}
								times[0] = num;
								GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_SET);
								break;
							case '1':
								num = 0;
								i = 3;
								while(value[i] != newline){
									num *= 10;
									num += value[i] - 0x30;
									i++;
								}
								times[1] = num;
								GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_SET);
								break;
							case '2':
								num = 0;
								i = 3;
								while(value[i] != newline){
									num *= 10;
									num += value[i] - 0x30;
									i++;
								}
								times[2] = num;
								GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_SET);
								break;
							case '3':
								num = 0;
								i = 3;
								while(value[i] != newline){
									num *= 10;
									num += value[i] - 0x30;
									i++;
								}
								times[3] = num;
								GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_SET);
								break;
							case '4':
								num = 0;
								i = 3;
								while(value[i] != newline){
									num *= 10;
									num += value[i] - 0x30;
									i++;
								}
								times[4] = num;
								GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_SET);
								break;
							case '5':
								num = 0;
								i = 3;
								while(value[i] != newline){
									num *= 10;
									num += value[i] - 0x30;
									i++;
								}
								times[5] = num;
								GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_SET);
								break;
						}
						break;
					case 'g': //Go
						//START SHIT
						go = 1;
						GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_SET);
						break;
					case 'x': //No
						//STAHP SHIT
						go = 0;
						GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_RESET);
						break;
					case 'd': //for debug to reset LED
						GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_RESET);
						break;
				}
			}
			index = 0;
		}
		readChar = 0;
	}
}



////This function handles USART2 global interrupt request.
//void USART1_IRQHandler(void)
//{	
//	// check if the USART1 receive interrupt flag was set
//	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
//		USART_putchar(USART2,	USART_ReceiveData(USART1)); //For testing use ONLY
//		
//		switch(USART_ReceiveData(USART1)){
//			case 'w': // Step OPEN air
//				set_dir_air(0);
//				step_air(1);
//				break;
//			case 's': //Step CLOSE air
//				set_dir_air(1);
//				step_air(1);
//				break;
//			case 'e': // Step OPEN gas
//				set_dir_gas(0);
//				step_gas(1);
//				break;
//			case 'd': // Step CLOSE gas
//				set_dir_gas(1);
//				step_gas(1);
//				break;
//			case 'o':	//open DC valves
//				open_air();
//				open_gas();
//				break;
//			case 'p':	//close DC valves
//				close_air();
//				close_gas();
//				break;
//			case '\r': //enter
//				
//				break;
//			case '\b': //backspace
//				
//				break;
//			default:
//				break;
//		}
//	}
//}

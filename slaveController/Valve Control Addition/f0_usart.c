
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_misc.h"
#include <stdio.h>
#include "f0_usart.h"
#include "f0_adc.h"
#include "valves.h"

extern uint8_t go;
extern uint8_t U1;
extern uint8_t U2;

uint8_t value1[32] = {0};
static uint8_t readChar1 = 0;
static int index1 = 0;
uint8_t ret = '\r';

uint16_t C = 0;
uint16_t F = 0;

uint8_t value[32] = {0};
static uint8_t readChar = 0;
static int index = 0;
uint16_t levels = 0;
extern int air_c[6];
extern int gas_c[6];
extern int time_c[6];
extern int temp[7];
uint8_t profile;
uint8_t newline = '\n';

uint8_t run(void){
	return go;
}

uint8_t currProfile(void){
	return profile;
}

//char Rx_indx, Rx_data[2], Rx_Buffer[100], Transfer_cplt;
#define MAX_STRLEN 12 // this is the maximum string length of our string in characters
volatile char received_string[MAX_STRLEN+1]; // this will hold the recieved string

char received;

char getRx(void){
	return received;
}

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

void USART1_comm(){
	int num = 0;
	int num2 = 0;
	int i = 0;
	// check if the USART1 receive interrupt flag was set
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
		readChar1 = USART_ReceiveData(USART1);
		
		USART_putchar(USART2,	readChar1); //For testing use ONLY
		
		switch(readChar1){
			case 'x':	//open DC valves
				go = 1;
				open_air();
				open_gas();
				break;
			case 'c':	//close DC valves
				go = 0;
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
					break;
				case 'b':
					switch(value1[1]){
						case 'p':
							set_dir_air(0);
							switch(value1[2]){
								case 'p':
									set_dir_gas(0);
									num = 0;
									i = 3;
									while(value1[i] != '.'){
										if(value1[i] == '\b') break;
										num *= 10;
										num += value1[i] - 0x30;
										i++;
									}
									i++;
									while(value1[i] != '\r'){
										if(value1[i] == '\b') break;
										num2 *= 10;
										num2 += value1[i] - 0x30;
										i++;
									}
									step_both(num,num2);
									break;
								case 'n':
									set_dir_gas(1);
									num = 0;
									i = 3;
									while(value1[i] != '.'){
										if(value1[i] == '\b') break;
										num *= 10;
										num += value1[i] - 0x30;
										i++;
									}
									i++;
									while(value1[i] != '\r'){
										if(value1[i] == '\b') break;
										num2 *= 10;
										num2 += value1[i] - 0x30;
										i++;
									}
									step_both(num,num2);
									break;
							}
							break;
						case 'n':
							set_dir_air(1);
							switch(value1[2]){
								case 'p':
									set_dir_gas(0);
									num = 0;
									i = 3;
									while(value1[i] != '.'){
										if(value1[i] == '\b') break;
										num *= 10;
										num += value1[i] - 0x30;
										i++;
									}
									i++;
									while(value1[i] != '\r'){
										if(value1[i] == '\b') break;
										num2 *= 10;
										num2 += value1[i] - 0x30;
										i++;
									}
									step_both(num,num2);
									break;
								case 'n':
									set_dir_gas(1);
									num = 0;
									i = 3;
									while(value1[i] != '.'){
										if(value1[i] == '\b') break;
										num *= 10;
										num += value1[i] - 0x30;
										i++;
									}
									i++;
									while(value1[i] != '\r'){
										if(value1[i] == '\b') break;
										num2 *= 10;
										num2 += value1[i] - 0x30;
										i++;
									}
									step_both(num,num2);
									break;
							}
							break;
						default:
							break;
					}
					break;
				case 'p':
					switch(value1[1]){
						case '0':
							profile_custom();
							break;
						case '1':
							profile1();
							break;
						case '2':
							profile2();
							break;
						case '3':
							profile3();
							break;
					}
					break;
				default:
					break;
			}
			index1 = 0;
		}
		readChar1 = 0;
	}
}

//This function handles USART2 global interrupt request.
void USART1_IRQHandler(void){	
	USART1_comm();
//	U1++;
}

void USART2_comm(){
	uint32_t num = 0;
	uint32_t i = 0;
	// check if the USART2 receive interrupt flag was set
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){
		readChar = USART_ReceiveData(USART2);
		USART_putchar(USART2, readChar);
		
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
					case 'p': //Profile
						switch(value[2]){
							case '0':
								profile = 0;
								break;
							case '1':
								profile = 1;
								break;
							case '2':
								profile = 2;
								break;
							case '3':
								profile = 3;
								break;
							default:
								close_air();
								close_gas();
								profile = 4;
								break;
						}
						break;
					case 'a': //Air
						switch(value[2]){
							case '0':
								num = 0;
								i = 3;
								while(value[i] != newline){
									num *= 10;
									num += value[i] - 0x30;
									i++;
								}
								air_c[0] = num;
								break;
							case '1':
								num = 0;
								i = 3;
								while(value[i] != newline){
									num *= 10;
									num += value[i] - 0x30;
									i++;
								}
								air_c[1] = num;
								break;
							case '2':
								num = 0;
								i = 3;
								while(value[i] != newline){
									num *= 10;
									num += value[i] - 0x30;
									i++;
								}
								air_c[2] = num;
								break;
							case '3':
								num = 0;
								i = 3;
								while(value[i] != newline){
									num *= 10;
									num += value[i] - 0x30;
									i++;
								}
								air_c[3] = num;
								break;
							case '4':
								num = 0;
								i = 3;
								while(value[i] != newline){
									num *= 10;
									num += value[i] - 0x30;
									i++;
								}
								air_c[4] = num;
								break;
							case '5':
								num = 0;
								i = 3;
								while(value[i] != newline){
									num *= 10;
									num += value[i] - 0x30;
									i++;
								}
								air_c[5] = num;
								break;
						}
						break;
					case 'g': //Gas
						switch(value[2]){
							case '0':
								num = 0;
								i = 3;
								while(value[i] != newline){
									num *= 10;
									num += value[i] - 0x30;
									i++;
								}
								gas_c[0] = num;
								break;
							case '1':
								num = 0;
								i = 3;
								while(value[i] != newline){
									num *= 10;
									num += value[i] - 0x30;
									i++;
								}
								gas_c[1] = num;
								break;
							case '2':
								num = 0;
								i = 3;
								while(value[i] != newline){
									num *= 10;
									num += value[i] - 0x30;
									i++;
								}
								gas_c[2] = num;
								break;
							case '3':
								num = 0;
								i = 3;
								while(value[i] != newline){
									num *= 10;
									num += value[i] - 0x30;
									i++;
								}
								gas_c[3] = num;
								break;
							case '4':
								num = 0;
								i = 3;
								while(value[i] != newline){
									num *= 10;
									num += value[i] - 0x30;
									i++;
								}
								gas_c[4] = num;
								break;
							case '5':
								num = 0;
								i = 3;
								while(value[i] != newline){
									num *= 10;
									num += value[i] - 0x30;
									i++;
								}
								gas_c[5] = num;
								break;
						}
						break;
					case 't': //Time
						switch(value[2]){
							case '0':
								num = 0;
								i = 3;
								while(value[i] != newline){
									num *= 10;
									num += value[i] - 0x30;
									i++;
								}
								time_c[0] = num;
								break;
							case '1':
								num = 0;
								i = 3;
								while(value[i] != newline){
									num *= 10;
									num += value[i] - 0x30;
									i++;
								}
								time_c[1] = num;
								break;
							case '2':
								num = 0;
								i = 3;
								while(value[i] != newline){
									num *= 10;
									num += value[i] - 0x30;
									i++;
								}
								time_c[2] = num;
								break;
							case '3':
								num = 0;
								i = 3;
								while(value[i] != newline){
									num *= 10;
									num += value[i] - 0x30;
									i++;
								}
								time_c[3] = num;
								break;
							case '4':
								num = 0;
								i = 3;
								while(value[i] != newline){
									num *= 10;
									num += value[i] - 0x30;
									i++;
								}
								time_c[4] = num;
								break;
							case '5':
								num = 0;
								i = 3;
								while(value[i] != newline){
									num *= 10;
									num += value[i] - 0x30;
									i++;
								}
								time_c[5] = num;
								break;
						}
						break;
					case 's': //Go
						go = 1;
						open_air();
						open_gas();
						break;
					case 'x': //No
						close_air();
						close_gas();
						set_time_zero(profile);
						break;
				}
			}
			index = 0;
		}
		readChar = 0;
	}
}

//This function handles USART2 global interrupt request.
void USART2_IRQHandler(void){	
	USART2_comm();
//	U2++;
}




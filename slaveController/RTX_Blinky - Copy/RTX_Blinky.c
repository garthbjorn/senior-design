#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_adc.h"
#include "stm32f0xx_usart.h"
#include "stm32f0xx.h"
//#include "f0_usart.h"
//#include "vt100.h"

//void Delay()
//{
//	int i;
//	for (i = 0; i < 1000000; i++)
//		asm("nop");
//}

void ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOC, ENABLE);
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;     // PA.1 ADC IN1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
 
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void ADC1_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;
 
    /* ADC1 DeInit */
	ADC_DeInit(ADC1);
 
    /* ADC1 Periph clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
 
    /* Initialize ADC structure */
	ADC_StructInit(&ADC_InitStructure);
 
    /* Configure the ADC1 in continous mode withe a resolutuion equal to 12 bits  */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Upward;
	ADC_Init(ADC1, &ADC_InitStructure);
 
    /* Convert the ADC1 input with 55.5 Cycles as sampling time */
	ADC_ChannelConfig(ADC1, ADC_Channel_1, ADC_SampleTime_1_5Cycles);
	//ADC_ChannelConfig(ADC1, 1, ADC_SampleTime_55_5Cycles); // PA.1 - IN1
 
    /* ADC Calibration */
	ADC_GetCalibrationFactor(ADC1);
 
    /* Enable ADC1 */
	ADC_Cmd(ADC1, ENABLE);
 
    /* Wait the ADCEN falg */
	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN));
 
    /* ADC1 regular Software Start Conv */
	ADC_StartOfConversion(ADC1);
}

void adc_f0_init(void)
{
	ADC_GPIO_Config();
	ADC1_Config();
}


///////////////////////


void m_USART_putchar(USART_TypeDef* USARTx, char c)
{
	//while (!(USARTx->ISR & 0x00000040))
		USART_SendData(USARTx, c);
}

void m_USART_puts(USART_TypeDef* USARTx, volatile char * s) {
	while (*s) {
		m_USART_putchar(USARTx, *s++);
	}
}

void m_USART_Configure()
{
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	USART_InitStructure.USART_BaudRate = 115200;//9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
  //USART_Init(USART2, &USART_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
	//USART_Cmd(USART2, ENABLE);
	
}

void m_USART_Output_Configure()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
	//GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_1);
	//GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_1);
  
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9 | GPIO_Pin_10;
	//GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//void USART_PUT_TEMP(USART_TypeDef* USARTx, uint32_t t){
//		USART_putchar(USARTx, 't');
//		USART_putnum(USARTx, t);
//		USART_putchar(USARTx, '\n');
//}	

void m_USART_putnum(USART_TypeDef* USARTx, uint32_t x)
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
		m_USART_putchar(USARTx, value[--i]); 
	}
}

void m_usart_f0_init()
{
	m_USART_Output_Configure();
	m_USART_Configure();
}

int main()
{
	m_usart_f0_init();
//	VT100_init();
 
	while (1)
	{
		//while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
			m_USART_puts(USART2,"\x1b[H");
			USART_SendData(USART1, 'X');
		m_USART_puts(USART2,"\x1b[H");
			m_USART_putchar(USART1, 'P');
		m_USART_puts(USART2,"\x1b[H");
			m_USART_puts(USART1, "string");
		m_USART_puts(USART2,"\x1b[H");
			m_USART_putnum(USART1, 1);
	}

	
//	adc_f0_init();
//	usart_f0_init();
//	VT100_init(USART2);
// 
//	while (1)
//	{
//		uint16_t ADC1ConvertedValue;
// 
//	    /* Test EOC flag */
//		while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
//			;
// 
//	    /* Get ADC1 converted data */
//		ADC1ConvertedValue = ADC_GetConversionValue(ADC1);
//		update_rawADC(USART2, ADC1ConvertedValue);						//ADC value 0-4096
//		//update_degF(USART2, ADC1ConvertedValue*100/164);			//degF     0-2500F
//		//update_degC(USART2, (ADC1ConvertedValue*100/295)-18);	//degC (-)18-1370C
//		
//		
//		if (ADC1ConvertedValue > 4000)
//		{
//			GPIOC->BSRR = GPIO_Pin_8;
//			GPIOC->BSRR = GPIO_Pin_9;
//		}
//		else if (ADC1ConvertedValue > 2000)
//		{
//			GPIOC->BSRR = GPIO_Pin_8;
//			GPIOC->BRR = GPIO_Pin_9;
//		}
//		else
//		{
//			GPIOC->BRR = GPIO_Pin_8;
//			GPIOC->BRR = GPIO_Pin_9;
//		}
//	}
}


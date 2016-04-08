#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_adc.h"
#include "stm32f0xx_usart.h"
#include "f0_usart.h"
#include "vt100.h"

void Delay(__IO uint32_t nCount) {
  while(nCount--) {
  }
}

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


int main()
{
	uint16_t C = 0;
	uint16_t F = 0;
	uint16_t ADC1Value = 0;
	//uint16_t i = 0;
	
	adc_f0_init();
	usart_f0_init();
	VT100_init(USART1);
 
	while (1)
	{
	    /* Test EOC flag */
		while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
			;
 
	    /* Get ADC1 converted data */
		ADC1Value = ADC_GetConversionValue(ADC1);
		update_rawADC(USART1, ADC1Value);		//ADC value 0-4096
		//update_degF(USART1, ADC1ConvertedValue/0.78);	//degF 0-2500F //1.64
		//update_degC(USART1, ADC1ConvertedValue/1.43);		//degC 0-1370C	//3
		if(ADC1Value > 25){
			C = ADC1Value*0.732-18;
		}else{
			C = 0;
		}
		F = 9/5*C+32;
		
		update_degC(USART1, C);		//degC 0-1370C	//3
		update_degF(USART1, F);	//degF 0-2500F //1.64
		
		if (ADC1Value > 4000)
		{
			GPIOC->BSRR = GPIO_Pin_8;
			GPIOC->BSRR = GPIO_Pin_9;
		}
		else if (ADC1Value > 2000)
		{
			GPIOC->BSRR = GPIO_Pin_8;
			GPIOC->BRR = GPIO_Pin_9;
		}
		else
		{
			GPIOC->BRR = GPIO_Pin_8;
			GPIOC->BRR = GPIO_Pin_9;
		}
		Delay(2500000);
	}
}

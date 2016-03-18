
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_adc.h"
#include "stm32f0xx_usart.h"
#include "f0_usart.h"
#include "f0_adc.h"
#include "vt100.h"

//void Delay()
//{
//	int i;
//	for (i = 0; i < 1000000; i++)
//		asm("nop");
//}

int main()
{
	adc_f0_init();
	usart_f0_init();
	VT100_init(USART1);
 
	while (1)
	{
		uint16_t ADC1ConvertedValue;
 
	    /* Test EOC flag */
		while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET)
			;
 
	    /* Get ADC1 converted data */
		ADC1ConvertedValue = ADC_GetConversionValue(ADC1);
		update_rawADC(USART1, ADC1ConvertedValue);		//ADC value 0-4096
		update_degF(USART1, ADC1ConvertedValue/1.64);	//degF 0-2500F
		update_degC(USART1, ADC1ConvertedValue/3);		//degC 0-1370C
		
		if (ADC1ConvertedValue > 4000)
		{
			GPIOC->BSRR = GPIO_Pin_8;
			GPIOC->BSRR = GPIO_Pin_9;
		}
		else if (ADC1ConvertedValue > 2000)
		{
			GPIOC->BSRR = GPIO_Pin_8;
			GPIOC->BRR = GPIO_Pin_9;
		}
		else
		{
			GPIOC->BRR = GPIO_Pin_8;
			GPIOC->BRR = GPIO_Pin_9;
		}
	}
}

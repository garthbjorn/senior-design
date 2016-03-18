
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_adc.h"
#include "stm32f0xx_usart.h"
#include "f0_usart.h"
#include "f0_adc.h"
#include "vt100.h"
#include "display.h"

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
	VT100_INIT;
	display_init(USART1);
 
	while (1)
	{
		adc_convert();
	}
}

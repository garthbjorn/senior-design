
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_adc.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_usart.h"
#include "f0_usart.h"
#include "f0_adc.h"
#include "vt100.h"
#include "display.h"
#include "valves.h"

uint8_t go = 0;

//uint16_t profile[0] = {};  //A bunch of temperature data
		//Data will most likely include a start temp, a ramp, a polish temp, a down ramp, and a final temp
	//Need to make a systick and define a specific frequency in which this array 
	// will be parsed to match up with real time.

int main()
{
	adc_f0_init();
	usart_f0_init();
	valve_init();
	
	//systick_sync
	while (1)
	{
		//if(go){
			//if(systick_edge()){
				
				//increment some kind of counter
			//}
		//}
		
		adc_convert();
	}
}

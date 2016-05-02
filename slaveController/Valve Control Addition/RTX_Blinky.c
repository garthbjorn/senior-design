/*
*	RTX_Blinky.c (MAIN)
* 
* The following program keeps track of which profile, if
* any, is currently in operation as well as performs
* periodic ADC temperature reads.
*
*	Pending functionality: 
*			Include ADC interrupt so that temperatures may be
*			updated during profile execution
* 			
*
* Author: Wesley Butler/Garth Cline
*/

#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_adc.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_usart.h"
#include "f0_usart.h"
#include "f0_adc.h"
#include "valves.h"

/*
* Local variables
*/
uint8_t go = 0;
uint8_t prof = 4;
uint8_t test = 0;
uint8_t i = 0;

int main()
{
	adc_f0_init();
	usart_f0_init();
	valve_init();
	
	while (1)
	{
		go = run();
		if(go){
			prof = currProfile();
			switch(prof){
				case 0:
					profile_custom();
					break;
				case 1:
					profile1();
					break;
				case 2:
					profile2();
					break;
				case 3:
					profile3();
					break;
				default:
					close_air();
					close_gas();
					break;
			}
			fix_time(prof);
			while(go); //Wait until STOP is pressed on Main Control
		}else{
			close_air();
			close_gas();
		}
			
		adc_convert();
	}
}


#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_adc.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_usart.h"
#include "f0_usart.h"
#include "f0_adc.h"
#include "valves.h"

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
			while(go); //Wait until someone presses STOP
		}else{
			close_air();
			close_gas();
		}
			
		adc_convert();
	}
}




//		if(U1 > 0){
//			USART1_comm();
//			U1--;
//		}
//		if(U2 > 0){
//			USART2_comm();
//			U2--;
//		}

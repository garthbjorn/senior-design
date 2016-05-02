/*
*	valves.c
* 
* This module facilitates valve operation for both shutoff and
*	 stepper valves.
*
*	Pending functionality: 
*			
*
* Author: Wesley Butler
*/

#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "f0_usart.h"
#include "valves.h"

uint32_t delay_factor = 1920000;
int air_c[6] = {0};
int gas_c[6] = {0};
int time_c[6] = {0};
int air_1[8] = {950, 700, 600, 0, 400, 400, 1000, 450};
int gas_1[8] = {1500, 3000, 0, 2000, 500, 500, 700, 800};
int time_1[7] = {3, 2, 780, 30, 30, 30, 30};
int air_2[7] = {950, 700, 600, 0, 800, 1000, 450};
int gas_2[7] = {1500, 3000, 0, 2000, 1000, 700, 800};
int time_2[6] = {3, 2, 780, 45, 45, 30};
int air_3[6] = {950, 700, 600, 0, 1800, 450};
int gas_3[6] = {1500, 3000, 0, 2500, 1000, 1000};
int time_3[5] = {3, 2, 780, 60, 30};
int temp[7] = {0};

/*
*	Delay. 1 second = 1920000 counts
* 
* @param counts
*/
void Delay(__IO uint32_t nCount) {
  while(nCount-- && run()) {
  }
}

/*
*	Sets all times to zero for the current profile. Executed under a STOP condition.
* 
* @param current profile
*/
void set_time_zero(uint8_t profile){
	uint32_t i = 0;
	switch(profile){
		case 0:
			for(i=0;i<6;i++){
				temp[i] = time_c[i];
				time_c[i] = 0;
			}
			break;
		case 1:
			for(i=0;i<6;i++){
				temp[i] = time_1[i];
				time_1[i] = 0;
			}
			break;
		case 2:
			for(i=0;i<6;i++){
				temp[i] = time_2[i];
				time_2[i] = 0;
			}
			break;
		case 3:
			for(i=0;i<6;i++){
				temp[i] = time_3[i];
				time_3[i] = 0;
			}
			break;
		default:
			break;
	}
}

/*
*	Fixes time values if set_time_zero is used.
* 
* @param current profile
*/
void fix_time(uint8_t profile){
	uint32_t i = 0;
	switch(profile){
		case 0:
			for(i=0; i<6; i++){
				time_c[i] = temp[i];
			}
			break;
		case 1:
			for(i=0; i<6; i++){
				time_1[i] = temp[i];
			}
			break;
		case 2:
			for(i=0; i<6; i++){
				time_2[i] = temp[i];
			}
			break;
		case 3:
			for(i=0; i<6; i++){
				time_3[i] = temp[i];
			}
			break;
		default:
			break;
	}
}

/* 
* Runs Custom Profile.
* It is the responsibility of the user to ensure that 
* 	a profile begins and ends with air at 4 lpm and gas
* 	at 2 ft3/hr 
*/
void profile_custom(void){
	int i;
	for(i=0; i<6; i++){
		temp[i] = time_c[i];
	}
	open_air();
	open_gas();
	/* warm up */
	set_dir_air(0);
	set_dir_gas(0);
	Delay(20*delay_factor); //20 seconds. This is where you light the gas
	step_both(air_c[0],gas_c[0]);
	Delay(time_c[0]*delay_factor);
	step_both(air_c[1],gas_c[1]);
	Delay(time_c[1]*delay_factor);
	/* cool down */
	set_dir_air(1);
	set_dir_gas(1);
	step_both(air_c[2],gas_c[2]);
	Delay(time_c[2]*delay_factor);
	step_both(air_c[3],gas_c[3]);
	Delay(time_c[3]*delay_factor);
	step_both(air_c[4],gas_c[4]);
	Delay(time_c[4]*delay_factor);
	step_both(air_c[5],gas_c[5]);
	Delay(time_c[5]*delay_factor);
}

/* 
* Runs Profile 1 
*/
void profile1(void){
	int i;
	for(i=0; i<7; i++){
		temp[i] = time_1[i];
	}
	open_air();
	open_gas();
	set_dir_air(0);
	set_dir_gas(0);
	//step_both(350,800);
	Delay(20*delay_factor); //20 seconds
	step_both(air_1[0],gas_1[0]);
	Delay(time_1[0]*delay_factor); //3 seconds
	step_both(air_1[1],gas_1[1]);
	Delay(time_1[1]*delay_factor); //2 seconds
	step_both(air_1[2],gas_1[2]);
	Delay(time_1[2]*delay_factor); //13 minutes
	set_dir_air(1);
	set_dir_gas(1);
	step_both(air_1[3],gas_1[3]);
	Delay(time_1[3]*delay_factor); //30 seconds
	step_both(air_1[4],gas_1[4]);
	Delay(time_1[4]*delay_factor); //30 seconds
	step_both(air_1[5],gas_1[5]);
	Delay(time_1[5]*delay_factor); //30 seconds
	step_both(air_1[6],gas_1[6]);
	Delay(time_1[6]*delay_factor); //30 seconds
	step_both(air_1[7],gas_1[7]);
}

/* 
* Runs Profile 2
*/
void profile2(void){
	int i;
	for(i=0; i<6; i++){
		temp[i] = time_2[i];
	}
	open_air();
	open_gas();
	set_dir_air(0);
	set_dir_gas(0);
	//step_both(350,800);
	Delay(20*delay_factor); //20 seconds
	step_both(air_2[0],gas_2[0]);
	Delay(time_2[0]*delay_factor); //3 seconds
	step_both(air_2[1],gas_2[1]);
	Delay(time_2[1]*delay_factor); //2 seconds
	step_both(air_2[2],gas_2[2]);
	Delay(time_2[2]*delay_factor); //13 minutes
	set_dir_air(1);
	set_dir_gas(1);
	step_both(air_2[3],gas_2[3]);
	Delay(time_2[3]*delay_factor);	//45 seconds
	step_both(air_2[4],gas_2[4]);
	Delay(time_2[4]*delay_factor);	//45 seconds
	step_both(air_2[5],gas_2[5]);
	Delay(time_2[5]*delay_factor); //30 seconds
	step_both(air_2[6],gas_2[6]);
}

/* 
* Runs Profile 3 
*/
void profile3(void){
	int i;
	for(i=0; i<5; i++){
		temp[i] = time_3[i];
	}
	open_air();
	open_gas();
	set_dir_air(0);
	set_dir_gas(0);
	//step_both(350,800);
	Delay(20*delay_factor);  //20 seconds
	step_both(air_3[0],gas_3[0]);
	Delay(time_3[0]*delay_factor); //3 seconds
	step_both(air_3[1],gas_3[1]);
	Delay(time_3[1]*delay_factor); //2 seconds
	step_both(air_3[2],gas_3[2]);
	Delay(time_3[2]*delay_factor); //13 minutes
	set_dir_air(1);
	set_dir_gas(1);
	step_both(air_3[3],gas_3[3]);
	Delay(time_3[3]*delay_factor);	//60 seconds
	step_both(air_3[4],gas_3[4]);
	Delay(time_3[4]*delay_factor);	//30 seconds
	step_both(air_3[5],gas_3[5]);
}


void open_air(){
		GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_RESET);
}

void close_air(){
		GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_SET);
}

void open_gas(){
		GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_RESET);
}

void close_gas(){
		GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_SET);
}

static uint8_t stepper_res = 2;//16;

/* 
* Configure DIR pin for air.
*
* @param direction (1 = close, 0 = open)
*/
void set_dir_air(uint8_t dir){
		dir = dir&0x1;
		GPIO_WriteBit(GPIOB, GPIO_Pin_6, dir);	// Configure DIR pin
}

/* 
* Program steps for air.
*
* @param number of steps
*/
void step_air(uint16_t num){
		uint16_t i=0;
		for(i=0; i<num; i++){
			/* Send pulses to STEP */
			GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_SET);
			Delay(50);
			GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_RESET);
			Delay(3600);	
		}
}

/* 
* Configure DIR pin for gas.
*
* @param direction (1 = close, 0 = open)
*/
void set_dir_gas(uint8_t dir){
		dir = dir&0x1;
		GPIO_WriteBit(GPIOB, GPIO_Pin_4, dir);	
}

/* 
* Program steps for gas.
*
* @param number of steps
*/
void step_gas(uint16_t num){	
		uint16_t i=0;
		for(i=0; i<num; i++){
			/* Send pulses to STEP */
			GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);
			Delay(50);
			GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET);
			Delay(3600);
		}
}

/* 
* Program steps for air and gas.
*
* @param number of steps for air
* @param number of steps for gas
*/
void step_both(uint16_t air, uint16_t gas){
		uint16_t i = 0;
		uint16_t max = 0;
		if(air > gas) max = gas;
		else max = air;
	
		for(i = 0; i < max; i++){
			step_air(1);
			step_gas(1);
		}
		
		if(air > gas) step_air(air-max);
		else step_gas(gas-max);
}

/* NOT USED
* Reset stepper valves.
*/
void stepper_reset(void){
		uint16_t i = 0;
		close_air();
		close_gas();
		set_dir_air(0);
		set_dir_gas(0);
		for(i = 0; i < 400*stepper_res; i++){
			//Do these a bunch until sure that valve is closed (400*resolution = max)
			step_air(1);
			step_gas(1);
			Delay(3600);
		}
}

/* 
* Initialize valve GPIO
*/
void Valve_GPIO_Init(void){
		GPIO_InitTypeDef GPIO_InitStructure1;
		GPIO_InitTypeDef GPIO_InitStructure2;
		
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
		
		//STEPPER VALVE GPIO:	     		 4: DIR_GAS,  5: STEP_GAS, 6: DIR_AIR,  7: STEP_AIR
		GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7; 
		GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure1.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure1.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOB, &GPIO_InitStructure1);
		
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
		
		//DIGITAL VALVE GPIO:						8: DC_GAS,   9: DC_AIR
		GPIO_InitStructure2.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9;
		GPIO_InitStructure2.GPIO_Speed = GPIO_Speed_2MHz;
		GPIO_InitStructure2.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure2.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure2.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(GPIOC, &GPIO_InitStructure2);
}

/* 
* Setup valves
*/
void valve_init(){
		Valve_GPIO_Init();
		//stepper_reset();
		close_air();
		close_gas();
		set_dir_air(1);
		set_dir_gas(1);
}


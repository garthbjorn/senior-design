
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "valves.h"

void Delay(__IO uint32_t nCount) {
  while(nCount--) {
  }
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

/* Configure DIR pin. 0 = open, 1 = close */
void set_dir_air(uint8_t dir){
		dir = dir&0x1;
		GPIO_WriteBit(GPIOB, GPIO_Pin_6, dir);	// Configure DIR pin
}

/* 1 step = 1/16 of a step (400 full steps total) */
void step_air(uint16_t num){
		uint16_t i=0;
		for(i=0; i<num; i++){
			/* Send pulses to STEP */
			GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_SET);
			Delay(50); //5 = apx. 2 us
			GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_RESET);
			Delay(8500);	//8000 = apx. 588Hz ... 10000 = apx 472Hz
		}
}

/* Configure DIR pin. 0 = open, 1 = close */
void set_dir_gas(uint8_t dir){
		dir = dir&0x1;
		GPIO_WriteBit(GPIOB, GPIO_Pin_4, dir);	
}

/* 1 step = 1/16 of a step (400 full steps total) */
void step_gas(uint16_t num){	
		uint16_t i=0;
		for(i=0; i<num; i++){
			/* Send pulses to STEP */
			GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_SET);
			Delay(50);			//5 = apx. 2 us
			GPIO_WriteBit(GPIOB, GPIO_Pin_5, Bit_RESET);
			Delay(8500);	//8000 = apx. 588Hz ... 10000 = apx 472Hz
		}
}

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
			Delay(8000); //8000 = apx. 588Hz ... 10000 = apx 472Hz
		}
}

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

void valve_init(){
		Valve_GPIO_Init();
		//stepper_reset();
		//open_air();
		//open_gas();
		close_air();
		close_gas();
		set_dir_air(1);
		set_dir_gas(1);
}


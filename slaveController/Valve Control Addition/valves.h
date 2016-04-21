
#ifndef VALVES_H_
#define VALVES_H_

#include "stm32f0xx.h"

void Delay(__IO uint32_t nCount);
void open_air(void);
void close_air(void);
void open_gas(void);
void close_gas(void);
void step_air(uint16_t num);
void set_dir_air(uint8_t dir);
void step_gas(uint16_t num);
void set_dir_gas(uint8_t dir);
void stepper_reset(void);
void Valve_GPIO_Init(void);
void valve_init(void);

#endif /* VALVES_H_ */

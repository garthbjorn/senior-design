
#ifndef VALVES_H_
#define VALVES_H_

#include "stm32f0xx.h"

void Delay(__IO uint32_t nCount);
void set_time_zero(uint8_t profile);
void fix_time(uint8_t profile);
void profile_custom(void);
void profile1(void);
void profile2(void);
void profile3(void);
void open_air(void);
void close_air(void);
void open_gas(void);
void close_gas(void);
void step_air(uint16_t num);
void set_dir_air(uint8_t dir);
void step_gas(uint16_t num);
void set_dir_gas(uint8_t dir);
void step_both(uint16_t air, uint16_t gas);
void stepper_reset(void);
void Valve_GPIO_Init(void);
void valve_init(void);

#endif /* VALVES_H_ */


#ifndef F0_ADC_H_
#define F0_ADC_H_

#include "stm32f0xx.h"

void ADC_GPIO_Config(void);
void ADC1_Config(void);
void adc_f0_init(void);
void adc_convert(void);
uint16_t ADCvalue(void);

#endif /* PART_ADC_H_ */

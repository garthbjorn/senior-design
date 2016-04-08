
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"
#include "stm32f0xx_adc.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_usart.h"
#include "f0_usart.h"
#include "f0_adc.h"
#include "vt100.h"
#include "display.h"

void Delay(__IO uint32_t nCount) {
  while(nCount--) {
  }
}

uint16_t TimerPeriod = 0;
uint16_t Channel1Pulse = 0, Channel2Pulse = 0, Channel3Pulse = 0, Channel4Pulse = 0;

void PWM_Config(void)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
 
  /* GPIOA Clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  
  /* GPIOA Configuration: Channel 1, 2 as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_1);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_1);
  
  TimerPeriod = (SystemCoreClock / 10000 ) - 1;
	Channel1Pulse = (uint16_t) (((uint32_t) 5 * (TimerPeriod - 1)) / 10);
	Channel2Pulse = (uint16_t) (((uint32_t) 375 * (TimerPeriod - 1)) / 1000);
 
  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
  
  /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
 
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
 
  /* Channel 1, 2 Configuration in PWM mode */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_Pulse = Channel1Pulse; //0;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
  TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
  TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
 
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	 TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
 
  TIM_OCInitStructure.TIM_Pulse = Channel2Pulse;//0;
  TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	 TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
 
  //TIM_OCInitStructure.TIM_Pulse = 0;
  //TIM_OC3Init(TIM3, &TIM_OCInitStructure);
 
  /* TIM3 counter enable */
  TIM_Cmd(TIM3, ENABLE);
 
  /* TIM3 Main Output Enable */
  TIM_CtrlPWMOutputs(TIM3, ENABLE);
}

int main()
{
	adc_f0_init();
	usart_f0_init();
//	VT100_INIT;
//	display_init(USART2);
//	PWM_Config();
 
	while (1)
	{
		//Delay(5000000); //Magic speed 10000+
		
		adc_convert();
		//USART_putchar(USART2, USART_ReceiveData(USART2));
		//USART_SendData(USART2, USART_ReceiveData(USART2));
//		USART_putchar(USART2, 'f');
		//Delay(5000000);
//		USART_putchar(USART2, getRx());
		//USART_PUT_TEMPF(USART2, ADCvalue());
//		adc_convert();
//		USART_putchar(USART1, 'c');
//		Delay(5000000);
		//USART_PUT_TEMPC(USART2, ADCvalue());
//		USART_putchar(USART2, getRx());
		
		//if(USART_GetITStatus(USART2, USART_IT_RXNE)) {
		//	char t = USART2->RDR;
		//	USART_putchar(USART2, t);
		//}
		
		//Delay(5000000);
		//USART_puts(USART2, "string");
		//USART_SendData(USART2, USART2->RDR);
	}
}

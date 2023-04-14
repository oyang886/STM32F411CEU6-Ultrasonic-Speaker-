#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#include "stm32f4xx.h"
#include "light.h"

// GPIOB
// GPIO_Pin_1
// RCC_AHB1Periph_GPIOB
// ADC1
// RCC_APB2Periph_ADC1
// ADC_Channel_9
// ADC_IRQn
// ADC_IRQHandler
void init_pwm(void);
void ADC_GPIO_Config(void);
void DEBUG_ADC_Mode_Config(void);
void ADC_NVIC_Config(void);

#endif /* __BSP_ADC_H */

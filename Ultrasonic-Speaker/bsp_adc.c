#include "bsp_adc.h"
#define OVERFLOW 2470
#define PSR 1

u16 ADC_ConvertedValue;
float music;
u16 duty;

int map(int x, int in_min, int in_max, int out_min, int out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void init_pwm()
{
	GPIO_LED_Config(OVERFLOW - 1, PSR - 1);
}

void ADC_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}

void DEBUG_ADC_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	ADC_StructInit(&ADC_InitStructure);
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	
	// 这里用480个时钟来采样ADC
	ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 1, ADC_SampleTime_3Cycles);
	
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
	
	ADC_Cmd(ADC1, ENABLE);
}

void ADC_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	
	NVIC_InitStructure.NVIC_IRQChannel = ADC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&NVIC_InitStructure);
}

void ADC_IRQHandler(void)
{
	if(ADC_GetITStatus(ADC1, ADC_IT_EOC) == SET)
	{
		ADC_ConvertedValue = ADC_GetConversionValue(ADC1);
		
		// duty = (u16)map(ADC_ConvertedValue, 0, 4095, 0, OVERFLOW/2 - 1);
		// TIM_SetCompare1(TIM2, OVERFLOW/2 + duty);
		// music = (float)ADC_ConvertedValue * 0.000244200244f;
		// duty = (OVERFLOW/2 - 2) * music;
		// TIM_SetCompare1(TIM2, OVERFLOW/2 + duty);
		TIM_SetCompare1(TIM5, (OVERFLOW - 2) * ADC_ConvertedValue/4096 + 1);
	}
	ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
}

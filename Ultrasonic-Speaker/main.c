#include "bsp_adc.h"

int main(void)
{
	init_pwm();
	ADC_GPIO_Config();
	DEBUG_ADC_Mode_Config();
	ADC_NVIC_Config();
	
	ADC_SoftwareStartConv(ADC1);
	
	while(1)
	{
		
	}
}

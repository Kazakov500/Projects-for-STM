#include "stm32f4xx.h"

#include <stdlib.h>
#include <math.h>
#include <time.h>

void DAC_ini(void)
{
	GPIO_InitTypeDef GPIO_Init_DAC;
	DAC_InitTypeDef		DAC_Init_user;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	
	GPIO_Init_DAC.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init_DAC.GPIO_Mode = GPIO_Mode_AF;
	GPIO_Init_DAC.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init_DAC.GPIO_OType = GPIO_OType_PP;
	GPIO_Init_DAC.GPIO_PuPd = GPIO_PuPd_UP;
	
	GPIO_Init(GPIOA, &GPIO_Init_DAC);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	
	DAC_Init_user.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
	DAC_Init_user.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
	DAC_Init_user.DAC_Trigger = DAC_Trigger_None;
	DAC_Init_user.DAC_WaveGeneration = DAC_WaveGeneration_None;
	
	DAC_Init(DAC_Channel_1, &DAC_Init_user);
	
	DAC_Cmd(DAC_Channel_1, ENABLE);
}

void ADC_ini()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//Включить порт А
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//Включить ЦАП
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);//включить таймер 6
	
	GPIO_InitTypeDef GPIO_Init_ADC;//Структура инициализации АЦП
	
	/* Настраиваем ногу АЦП */
	GPIO_Init_ADC.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init_ADC.GPIO_Mode = GPIO_Mode_AN;
	GPIO_Init_ADC.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init_ADC.GPIO_OType = GPIO_OType_PP;
	GPIO_Init_ADC.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_Init(GPIOA, &GPIO_Init_ADC);//инициализация АЦП
	
	/* Настраиваем ногу ЦАПа */
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		

	/* Настраиваем таймер так чтоб он тикал почаще */

	TIM6->PSC = 0;
	TIM6->ARR = 500;
	TIM6->DIER |= TIM_DIER_UIE; //разрешаем прерывание от таймера
	TIM6->CR1 |= TIM_CR1_CEN; // Начать отсчёт!
	NVIC_EnableIRQ(TIM6_DAC_IRQn); //Разрешение TIM6_DAC_IRQn прерывания
	/* Включить DAC1 */
	DAC->CR |= DAC_CR_EN1;
		
		
		
	ADC_InitTypeDef ADC_Init_user; 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC, ENABLE);
	
  ADC_Init_user.ADC_Resolution = ADC_Resolution_12b;
  ADC_Init_user.ADC_ScanConvMode = DISABLE;
  ADC_Init_user.ADC_ContinuousConvMode = DISABLE;
  ADC_Init_user.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_Init_user.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_Init_user.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_Init_user.ADC_NbrOfConversion = 1;
	
	ADC_Init(ADC1, &ADC_Init_user);
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_56Cycles);

	
	NVIC_EnableIRQ(ADC_IRQn);
	ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);
	ADC_Cmd(ADC1, ENABLE);
}

void LED_ini()
{
//LOG("> RCC init");
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIODEN;
//LOG("> GPIO init start...");

	GPIOD->MODER = 0x55000000;
	GPIOD->OTYPER = 0;
	GPIOD->OSPEEDR = 0;
}


int main(void)
{
	LED_ini();
	ADC_ini();
	//DAC_ini();
	SysTick_Config(SystemCoreClock/1000);//1 ms
	
	while(1)
	{
		ADC_SoftwareStartConv(ADC1);
		//DAC_SetChannel1Data(DAC_Align_12b_R, 100);
		//GPIOD->ODR = 0x4000;

	}
}


uint16_t data_adc;//туды пхаеть будем значения
void ADC_IRQHandler()//эта функция сама вызывается при каждом такце ацп
{
	if(ADC_GetITStatus(ADC1, ADC_IT_EOC) == SET)
	{
		ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
		
		data_adc = ADC_GetConversionValue(ADC1);//Хапаем значение с ацп
		//TIM6->SR &= ~TIM_SR_UIF; //Сбрасываем флаг UIF
		//DAC_SetChannel1Data(DAC_Align_12b_R, data_adc);
	}
}

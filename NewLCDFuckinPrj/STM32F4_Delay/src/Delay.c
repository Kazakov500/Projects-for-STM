/*
+-------------------------------------+
| File     : Delay_config.h           |
| Author   : Butskikh D.S.            |
| Site     : www.mcustep.ru           |
| Mail     : admin@mcustep.ru         |
| Data     : 10/12/2013               |
| Device   : STM32F4xx                |
| Compiler : MDK-ARM (Keil)           |
| Project  : Delays                   |
| Version  : 1.0                      |
+-------------------------------------+
*/


#include "Delay.h"



#if (USE_TIMER_FOR_DELAY==1)

void _delay_ms (uint16_t delay)
{
	TIMX->PSC = F_TIMER/TIM_DIVIDER_MS-1;
	TIMX->ARR = delay;
	TIMX->EGR = TIM_EGR_UG;
	TIMX->CR1 = TIM_CR1_CEN|TIM_CR1_OPM;
	while ((TIMX->CR1&TIM_CR1_CEN)!=0);
}

void _delay_us (uint16_t delay)
{
	TIMX->PSC = F_TIMER/1000000-1;
	TIMX->ARR = delay;
	TIMX->EGR = TIM_EGR_UG;
	TIMX->CR1 = TIM_CR1_CEN|TIM_CR1_OPM;
	while ((TIMX->CR1&TIM_CR1_CEN)!=0);
}


#else

volatile void _delay_ (uint32_t delay)
{
    while(delay--);
}





#endif /*USE_TIMER_FOR_DELAY*/















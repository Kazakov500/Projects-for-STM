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


#ifndef DELAY_CONFIG_H_
#define DELAY_CONFIG_H_


#define USE_TIMER_FOR_DELAY         0                   // 0 - approximate
                                                        // 1 - precision

#define SYSTEM_CLOCK_FREQUENCY      168000000UL
#define F_TIM_APB1                  84000000UL
#define F_TIM_APB2                  168000000UL

//------ For precision -----

#define NUMBER_DELAY_TIMER          6                   // 1 - TIM1       6 - TIM6      11 - TIM11
                                                        // 2 - TIM2       7 - TIM7      12 - TIM12
                                                        // 3 - TIM3       8 - TIM8      13 - TIM13
                                                        // 4 - TIM4       9 - TIM9      14 - TIM14
                                                        // 5 - TIM5      10 - TIM10
//----- For approximate -----

#define NUM_CYCLES_US               ((22 * SYSTEM_CLOCK_FREQUENCY)/168000000UL)
#define NUM_CYCLES_MS               (NUM_CYCLES_US * 1000)



#endif /*DELAY_CONFIG_H_*/

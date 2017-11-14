/*
+-------------------------------------+
| File     : Delay.h                  |
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


#ifndef DELAY_H_
#define DELAY_H_

#include "stm32f4xx.h"
#include "Delay_config.h"

#if (USE_TIMER_FOR_DELAY==1)

#if (NUMBER_DELAY_TIMER==1)
#define TIMX                    TIM1
#define F_TIMER                 F_TIM_APB2
#elif (NUMBER_DELAY_TIMER==2)
#define TIMX                    TIM2
#define F_TIMER                 F_TIM_APB1
#elif (NUMBER_DELAY_TIMER==3)
#define TIMX                    TIM3
#define F_TIMER                 F_TIM_APB1
#elif (NUMBER_DELAY_TIMER==4)
#define TIMX                    TIM4
#define F_TIMER                 F_TIM_APB1
#elif (NUMBER_DELAY_TIMER==5)
#define TIMX                    TIM5
#define F_TIMER                 F_TIM_APB1
#elif (NUMBER_DELAY_TIMER==6)
#define TIMX                    TIM6
#define F_TIMER                 F_TIM_APB1
#elif (NUMBER_DELAY_TIMER==7)
#define TIMX                    TIM7
#define F_TIMER                 F_TIM_APB1
#elif (NUMBER_DELAY_TIMER==8)
#define TIMX                    TIM8
#define F_TIMER                 F_TIM_APB2
#elif (NUMBER_DELAY_TIMER==9)
#define TIMX                    TIM9
#define F_TIMER                 F_TIM_APB2
#elif (NUMBER_DELAY_TIMER==10)
#define TIMX                    TIM10
#define F_TIMER                 F_TIM_APB2
#elif (NUMBER_DELAY_TIMER==11)
#define TIMX                    TIM11
#define F_TIMER                 F_TIM_APB2
#elif (NUMBER_DELAY_TIMER==12)
#define TIMX                    TIM12
#define F_TIMER                 F_TIM_APB1
#elif (NUMBER_DELAY_TIMER==13)
#define TIMX                    TIM13
#define F_TIMER                 F_TIM_APB1
#elif (NUMBER_DELAY_TIMER==14)
#define TIMX                    TIM14
#define F_TIMER                 F_TIM_APB1
#else
#error Uncorrect number timer
#endif /*NUMBER_DELAY_TIMER*/


#if (F_TIMER>0xFFFF)
 #define TIM_DIVIDER_MS          3000
 #define delay_ms(ms)            _delay_ms(((ms)*3))
#else
 #define TIM_DIVIDER             1000
 #define delay_ms(ms)            _delay_ms((ms))
#endif /**/
#define delay_us(us)            _delay_us(us)
extern void _delay_ms (uint16_t delay);
extern void _delay_us (uint16_t delay);
#else
#define delay_ms(ms)            _delay_(((ms)*(NUM_CYCLES_MS)))
#define delay_us(us)            _delay_(((us)*(NUM_CYCLES_US)))
extern volatile void _delay_ (uint32_t delay);
#endif /*USE_TIMER_FOR_DELAY*/





#endif /*DELAY_H_*/

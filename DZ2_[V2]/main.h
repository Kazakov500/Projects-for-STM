#ifndef __MAIN_H
#define	__MAIN_H

#include "stm32f10x.h" 
#include "stdbool.h"
#include "string.h"

// Раскомментировать #define,
// если нужно использовать DMA при передаче по USART
//#define USE_DMA

/* Размеры буферов приёма и передачи */
#define	RX_BUFF_SIZE	256
#define TX_BUFF_SIZE	256

/* Управление светодиодами */
#define	LED1_ON()	GPIOD->BSRR |= GPIO_BSRR_BS7
#define	LED1_OFF()	GPIOD->BSRR |= GPIO_BSRR_BR7


/* Прототипы функций */
void delay(uint32_t takts);

#endif

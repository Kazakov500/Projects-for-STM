/*----------------------------------------------------------------------------
 * Name:    Blinky.c
 * Purpose: LED Flasher for STM32F4-Discovery
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2004-2015 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include "stm32f4xx.h"              // Keil::Device:STM32Cube HAL:Common
//#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
//#include "Board_LED.h"                  // ::Board Support:LED
//#include "Board_Buttons.h"              // ::Board Support:Buttons
#include "RTE_Components.h"             // Component selection

//#include "LCD\lcd12864.h"

/*
void LED_Init(void)
{
	GPIO_InitTypeDef LED_Init;
	
	__HAL_RCC_GPIOD_CLK_ENABLE();
	
	LED_Init.Pin = GPIO_PIN_12;
	LED_Init.Mode = GPIO_MODE_OUTPUT_PP;
	LED_Init.Pull = GPIO_NOPULL;
	LED_Init.Speed = GPIO_SPEED_FREQ_LOW;
	
}*/
/**
  * Main function
  */
int main (void) 
{
	//HAL_Init();
	//LED_Initialize();
	
	while (1)
	{
		//GPIO_TogglePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_14);
		//osDelay(500); 
		//HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
		//HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
		//osDelay(500);  
		//HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13 | GPIO_PIN_15);		
		//osDelay(500); 
		//HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
		//HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
		//osDelay(500);  
	}
	
	return 0;
}

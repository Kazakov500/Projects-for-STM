#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "cmsis_os.h"                   // ARM::CMSIS:RTOS:Keil RTX
#include "Board_LED.h"                  // ::Board Support:LED
#include "Board_Buttons.h"              // ::Board Support:Buttons
#include "RTE_Components.h"             // Component selection

#include "string.h"

/*#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"*/

#define CS1 GPIO_PIN_0
#define CS2 GPIO_PIN_1
#define DI GPIO_PIN_2
#define RW GPIO_PIN_3
#define E GPIO_PIN_4

void setDataToLCDPort(char* DB)
{
	int i = 0;
	for(i=0; i<strlen(DB); i++)
		if(DB[i] == '1') GPIOD->ODR |= (1<<i);			
		else GPIOD->ODR |= (0<<i);
}

/*
void lcd_write_str(char* str) 
{
	do {
		lcd_write_str(*str);
	}while(*++str);
}*/

void setDataToPin(uint16_t PIN, int data)
{
	if(data == 1) HAL_GPIO_TogglePin(GPIOE, PIN);
	else HAL_GPIO_DeInit(GPIOE, PIN);
}

void delay(unsigned int s)
{
	while(--s > 0) {
		__NOP();
	}
}

int main (void) 
{
	HAL_Init();
	//LED_Initialize();
	
		GPIO_InitTypeDef  GPIO_InitStruct;
		GPIO_InitStruct.Pin   = GPIO_PIN_All;
		GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;//GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
		GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
		
		//HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
		//HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		//HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		//HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);
	
	while(1)
	{
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, GPIO_PIN_SET);
		osDelay(100);
	}
	
	/*HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_0);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_2);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_3);
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_4);*/
		//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
	/*
		HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_0);
			HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_1);
			HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_2);*/
	
	/*GPIO_InitStruct.Pin   = GPIO_PIN_All;
		GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
		GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
		
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);*/
	/*
	int del = 1;//9999 nops
	setDataToPin(DI, 0);
	setDataToPin(RW, 0);
	//delay(del);
	osDelay(del); 
	setDataToLCDPort("11111100");
	setDataToPin(E, 1);
	//delay(del);
	osDelay(del); 
	setDataToPin(E, 0);
	
	//delay(del);
	osDelay(del); 
	setDataToLCDPort("00000010");
	setDataToPin(E, 1);
	//delay(del);
	osDelay(del); 
	setDataToPin(E, 0);
	
	//delay(del);
	osDelay(del); 
	setDataToLCDPort("00011101");
	setDataToPin(E, 1);
	//delay(del);
	osDelay(del); 
	setDataToPin(E, 0);
	
	//delay(del);
	osDelay(del); 
	setDataToLCDPort("00000011");
	setDataToPin(E, 1);
	//delay(del);
	osDelay(del); 
	setDataToPin(E, 0);
	
	
	
	
	//work
	setDataToPin(DI, 1);
	setDataToPin(CS1, 1);
	setDataToPin(CS2, 0);
	
	
	//delay(del);
	osDelay(del); 
	setDataToLCDPort("11111111");
	setDataToPin(E, 1);
	//delay(del);
	osDelay(del); 
	setDataToPin(E, 0);
	*/
	
		/**/
	/*
		delay(del);
	setDataToLCDPort("00000001");
	setDataToPin(E, 1);
	delay(del);
	setDataToPin(E, 0);
	
	
		delay(del);
	setDataToLCDPort("00000110");
	setDataToPin(E, 1);
	delay(del);
	setDataToPin(E, 0);	


		delay(del);
	setDataToLCDPort("00000010");
	setDataToPin(E, 1);
	delay(del);
	setDataToPin(E, 0);	

	setDataToPin(DI, 1);

		delay(del);
	setDataToLCDPort("11111111");
	setDataToPin(E, 1);
	delay(del);
	setDataToPin(E, 0);	
*/


			// ??????????? ?????
		//GPIO_InitTypeDef  GPIO_InitStruct;  // ??????? ?????????

  /* Configure GPIO pins: PD12 PD13 PD14 PD15 */
		/*GPIO_InitStruct.Pin   = GPIO_PIN_All;
		GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull  = GPIO_PULLDOWN;
		GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
		
		HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_0);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_3);
		osDelay(100); 
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_3);
		
		
		//GPIOD->ODR = 0x111111;
		
		
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_0);
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5);
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_3);
		osDelay(100); 
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_3);
*/

		//GPIOD->ODR = 0x0001U;
		while (1)
		{
			//111111111111111
			//000000000000000
			//setData_("1111111111111111");
			//HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12 | GPIO_PIN_14);
			//GPIOD->ODR = 0x11111111;//Green led 0x00001000;
			
			//0b00000000000000000000000100000000
			//GPIOD->ODR = (1111<<12) ;
			
			/*
			GPIOD->ODR = (1<<12) ;
			osDelay(1); 
			GPIOD->ODR = (0<<12) ;
			osDelay(20); 
			*/

			//osDelay(1); 
			//GPIOD->ODR = (0<<15) ;
			//osDelay(1); 
			//GPIOD->
			//HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13);
			//HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_15);
			//setDataToLCDPort("0000000000000101");
			//GPIOD->ODR |= (1<<12) ;
			//GPIOD->ODR |= (1<<15) ;
			//osDelay(500); 
			
			
			
			/*
				//work
			setDataToPin(DI, 1);
			setDataToPin(CS1, 1);
			setDataToPin(CS2, 0);
			
			
			//delay(del);
			osDelay(del); 
			setDataToLCDPort("11111111");
			setDataToPin(E, 1);
			//delay(del);
			osDelay(del); 
			setDataToPin(E, 0);
			*/
			
			
			//HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
			//HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);
			//HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13 | GPIO_PIN_15);		
			//osDelay(500); 
			//HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, GPIO_PIN_RESET);
			//HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);
		}
}

#include "stm32f4xx.h"
#include "binaryConst.h"
#include "lcd.h"
#include "Delay.h"


int main(void)
{
	int del = 1000000;//999999;
	int del_led = 15;
	uint32_t i;
	
	
	
	
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIODEN;
	
	GPIOD->MODER = 0x55000000;
	GPIOD->OTYPER = 0;
	GPIOD->OSPEEDR = 0;
	
	lcd_init();
	//lcd_write_str("Fuckin LCD!!!");
	
	while(1)
	{
		

		GPIOD->ODR = 0x1000;
		delay_ms(del_led);
		//GPIOD->ODR = 0x0000;
		//delay_ms(del_led);
		
		GPIOD->ODR = 0x2000;
		delay_ms(del_led);
		//GPIOD->ODR = 0x0000;
		//delay_ms(del_led);
		
		GPIOD->ODR = 0x4000;
		delay_ms(del_led);
		//GPIOD->ODR = 0x0000;
		//delay_ms(del_led);
		
		GPIOD->ODR = 0x8000;
		delay_ms(del_led);
		//GPIOD->ODR = 0x0000;
		//delay_ms(del_led);

		//lcd_write_data2(0,1,0,1,0,1,0,1);
		
		

	/*
		GPIO_SetBits(LCD_PORT,LCD_CS1_Pin);//cs1 = 1
		GPIO_ResetBits(LCD_PORT,LCD_CS2_Pin);//cs2 = 0
			
	
		GPIO_SetBits(LCD_PORT,GPIO_Pin_13);//cs2 = 0

			
		GPIO_SetBits(LCD_PORT,LCD_RS_Pin);  //rs = 1
		GPIO_ResetBits(LCD_PORT,LCD_RW_Pin);  //rw = 0
	
		GPIO_SetBits(LCD_PORT,LCD_CS2_Pin);//cs2 = 1
		GPIO_ResetBits(LCD_PORT,LCD_CS1_Pin);//cs1 = 0
		*/
		//delay(del);
		
		
		//for(int i = 0; i<10; i++) 
		//{
		//	lcd_write_data2(1,1,1,1,1,1,1,1);
			
			 //delay(del);
		//}
	}
	
	
	
}


#include "stm32f4xx.h"
#include "lcd.h"
#include "Delay.h"
#include "SYM.h"

#include <stdlib.h>
#include <math.h>
#include <time.h>


int p = 0;

	
void init_audio_dac(void);

void LOG(char* msg)
{
	LCD_clear_page(p);
	lcd_draw_str(msg, 0, p);
	p++;
	if (p>3) p = 0;
	delay_ms(200);
}


int main(void)
{
	lcd_init();
	Clear_LCD();
	
	//LOG("> Kernel Loading...");
	
	//lcd_draw_int(0021, 0, 3);
	//lcd_draw_int(15, 0, 2);
	//lcd_draw_float(-123.456, 3, 10, 1);
	//lcd_draw_float(567.432, 3, 20, 2);
	//lcd_draw_int(15, 20, 3);
	//lcd_draw_int(12, 40, 0);

	/*for(int i=0; i<=120; i++) 
	{
		lcd_set_Y(i, 1);
		lcd_data("00000001");
		
		lcd_set_Y(i, 3);
		lcd_data("10000000");
	}
	lcd_set_Y(0, 2);
		lcd_data("11111111");
	lcd_set_Y(120, 2);
		lcd_data("11111111");
	
	for(int i=2; i<120-1; i++) 
	{
		lcd_set_Y(i, 2);
		lcd_data("01111110");
		delay_ms(10);
	}
	Clear_LCD();
	p = 0;
	
LOG("> LCD init...");
LOG("> start...");
LOG("> initialization...");

*/
	int del_led = 15;
	uint32_t i;
	
	
	

//LOG("> RCC init");
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIODEN;
//LOG("> GPIO init start...");

	GPIOD->MODER = 0x55000000;
	GPIOD->OTYPER = 0;
	GPIOD->OSPEEDR = 0;
//LOG("> GPIO init end");	
	

	
	//lcd_draw_str("à", 10, 1);
//LOG("ÝÒÀ ÔÈÃÍß ÇÀÏÀÕÀËÀ!!!");

	
	
//LOG("> Main loop start...");
	

	
	int k = 0;
	while(1)
	{
		/*
		int x = 16;
		for(int i=0; i<=120; i++)
			lcd_draw_vert_line(i, x+15*sin((i+k)/8.0));
		k++;*/
		
		/*
		//rand lines
		for(int i=0; i<=120-5; i+=5)
		{
			int pos_ = rand() % 32;
			lcd_draw_vert_line(i, pos_);
			lcd_draw_vert_line(i+1, pos_);
			lcd_draw_vert_line(i+2, pos_);
			lcd_draw_vert_line(i+3, pos_);
			lcd_draw_vert_line(i+4, pos_);
		}
		delay_us(5000);*/
	
	

		GPIOD->ODR |= 0x1000;
//LOG(">Green LED on...");
		//delay_ms(2);
		
		GPIOD->ODR |= 0x2000;
//LOG(">Orange LED on...");	
		//delay_ms(2);

		GPIOD->ODR |= 0x4000;
//LOG(">Read LED on...");
		//delay_ms(2);

		GPIOD->ODR |= 0x8000;
//LOG(">Blue LED on...");	
		//delay_ms(2);
		
		
	}
	
	
	
}


void rammstain()
{
	Clear_LCD();
	p = 0;
	LOG("~ $ guitars");
	
	Clear_LCD();
	p = 0;
	
	LOG("~ $ DO");
	LOG("~ $ DO class");
	LOG("~ $ DO class if");
	
	Clear_LCD();
	p = 0;
	
	LOG("~ $ DO");
	LOG("~ $ DO class");
	LOG("~ $ DO class if");
	
	Clear_LCD();
	p = 0;
	
	LOG("~ $ DO");
	LOG("~ $ DO class");
	LOG("~ $ DO class if");
	
	Clear_LCD();
	p = 0;
	
	LOG("~ $ guitars");
	
	Clear_LCD();
	p = 0;
	
	LOG("~ $ DO");
	LOG("~ $ DO class");
	LOG("~ $ DO class if");
	LOG("~ $ DO class if");
	LOG("~ $ DO class if inline");
	LOG("~ $ DO class if inline");
	LOG("~ $ DO class if inline");
	LOG("~ $ bool this delete define!");
	
	Clear_LCD();
	p = 0;
	
	LOG("~ $ ERROR...");
	LOG("~ $ ERROR...");
	LOG("~ $ ERROR...");
}	

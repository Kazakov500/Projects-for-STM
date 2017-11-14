/*
 * lcd.c
 *
 *  Created on: 23.05.2012
 *      Author: lamazavr
 */

#include "lcd.h"
#include "binaryConst.h"
#include "Delay.h"

int del_us = 1000;


void lcd_init_gpio() {
	RCC_AHB1PeriphClockCmd(LCD_RCC_GPIO,ENABLE);

	GPIO_InitTypeDef init;
	init.GPIO_Mode = GPIO_Mode_OUT;
	init.GPIO_Pin = GPIO_Pin_All;
	init.GPIO_OType = GPIO_OType_PP;
	//init.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(LCD_PORT,&init);
}
void lcd_write_data(u16 data) 
{
	GPIO_SetBits(LCD_PORT, LCD_E_Pin);
	GPIO_SetBits(LCD_PORT, data);
	delay_us(10);
	GPIO_ResetBits(LCD_PORT, LCD_E_Pin);
	//GPIO_ResetBits(LCD_PORT,LCD_E_Pin | data);
}

void lcd_write_data2(u16 _DB7, u16 _DB6, u16 _DB5, u16 _DB4, u16 _DB3, u16 _DB2, u16 _DB1, u16 _DB0 ) 
{
	
	
	if(_DB0 == 1) GPIO_SetBits(LCD_PORT, DB0);
	else GPIO_ResetBits(LCD_PORT, DB0);
	if(_DB1 == 1) GPIO_SetBits(LCD_PORT, DB1);
	else GPIO_ResetBits(LCD_PORT, DB1);
	if(_DB2 == 1) GPIO_SetBits(LCD_PORT, DB2);
	else GPIO_ResetBits(LCD_PORT, DB2);
	if(_DB3 == 1) GPIO_SetBits(LCD_PORT, DB3);
	else GPIO_ResetBits(LCD_PORT, DB3);
	if(_DB4 == 1) GPIO_SetBits(LCD_PORT, DB4);
	else GPIO_ResetBits(LCD_PORT, DB4);
	if(_DB5 == 1) GPIO_SetBits(LCD_PORT, DB5);
	else GPIO_ResetBits(LCD_PORT, DB5);
	if(_DB6 == 1) GPIO_SetBits(LCD_PORT, DB6);
	else GPIO_ResetBits(LCD_PORT, DB6);
	if(_DB7 == 1) GPIO_SetBits(LCD_PORT, DB7);
	else GPIO_ResetBits(LCD_PORT, DB7);
	
	//GPIO_SetBits(LCD_PORT, LCD_RS_Pin);
	//delay_us(del_us);
	
	GPIO_SetBits(LCD_PORT, LCD_E_Pin);
	delay_us(del_us);
	GPIO_ResetBits(LCD_PORT, LCD_E_Pin);
	delay_us(del_us);
	//GPIO_ResetBits(LCD_PORT, LCD_RS_Pin);
	//delay_us(del_us);
	//GPIO_SetBits(LCD_PORT, LCD_E_Pin);
	//delay_us(1);
	//GPIO_ResetBits(LCD_PORT, LCD_E_Pin);
	//GPIO_ResetBits(LCD_PORT,LCD_E_Pin | data);
}

void lcd_init() 
{
	lcd_init_gpio();

	//GPIO_ResetBits(LCD_PORT, LCD_RS_Pin);  // rs = 0
	GPIO_ResetBits(LCD_PORT, LCD_RS_Pin);
	GPIO_ResetBits(LCD_PORT, LCD_RW_Pin);
	GPIO_SetBits(LCD_PORT,LCD_CS2_Pin);//cs2 = 1
	GPIO_SetBits(LCD_PORT,LCD_CS1_Pin);//cs1 = 0
	
	//GPIO_SetBits(LCD_PORT, LCD_E_Pin);
	

	
	GPIO_SetBits(LCD_PORT,RST);
	delay_us(del_us);


	lcd_write_data2(0,0,1,1,1,1,1,1); //turn on
	delay_us(del_us*10);
	//lcd_write_data2(1,1,1,1,1,1,0,0); //turn on
	lcd_write_data2(0,1,0,0,0,0,0,0); //Y 0
	//lcd_write_data2(0,0,0,0,0,0,1,0); //Y 0
	lcd_write_data2(1,0,1,1,1,0,0,0); //X 0
	//lcd_write_data2(0,0,0,1,1,1,0,1); //X 0
	lcd_write_data2(1,1,0,0,0,0,0,0); // start pos
	//lcd_write_data2(0,0,0,0,0,0,1,1); // start pos
	
	/*
	lcd_write_data(_0b00111111); //turn on
	delay_us(del_us*10);
	lcd_write_data(_0b00111111); //turn on
	delay_us(del_us*10);
	lcd_write_data(_0b00111111); //turn on
	delay_us(del_us*10);
	lcd_write_data(_0b01000000); //Y 0
	lcd_write_data(_0b10111000); //X 0
	lcd_write_data(_0b11000000); // start pos
	*/
	
	GPIO_ResetBits(LCD_PORT,LCD_RW_Pin); 
	GPIO_SetBits(LCD_PORT,LCD_RS_Pin);
	
	
	
	for(int i = 0; i<100; i++) 
	{

	//GPIO_SetBits(LCD_PORT,LCD_CS2_Pin);//cs2 = 0
	//GPIO_SetBits(LCD_PORT,LCD_CS1_Pin);//cs1 = 0
	
		GPIO_SetBits(LCD_PORT,RST);
	
	//GPIO_SetBits(LCD_PORT,RST);
		delay_us(del_us);
	
	
		//lcd_write_data2(1,1,1,1,1,1,1,1);
		//lcd_write_data2(0,0,0,0,0,0,0,0);
		lcd_write_data(_0b11111111);
    lcd_write_data(_0b00000000);
	}
	
}


void lcd_init__() 
{
	lcd_init_gpio();
	int del = 999999;
	GPIO_ResetBits(LCD_PORT, LCD_RS_Pin);  // rs = 0
	GPIO_ResetBits(LCD_PORT, LCD_RW_Pin);  //rw = 0
	delay_us(10);

	lcd_write_data(_0b00111111); //turn on
	delay_us(10);

	lcd_write_data(_0b01000000); //Y 0
	delay_us(10);

	lcd_write_data(_0b10111000); //X 0
	delay_us(10);

	lcd_write_data(_0b11000000); // start
	delay_us(10);

	GPIO_SetBits(LCD_PORT,LCD_RS_Pin);  //rs = 1
	GPIO_ResetBits(LCD_PORT,LCD_RW_Pin);  //rw = 0
	
	GPIO_SetBits(LCD_PORT,LCD_CS1_Pin);
	GPIO_ResetBits(LCD_PORT,LCD_CS2_Pin);
	
	for(int i = 0; i<10; i++) 
	{
		lcd_write_data(_0b11111111);
		lcd_write_data(_0b00000000);
	}
	
}



void lcd_init_() 
{
	lcd_init_gpio();
	int del = 99999;
	GPIO_ResetBits(LCD_PORT, LCD_RS_Pin);  // rs = 0
	delay_us(10);

	lcd_write_data(_0b00110000); //init1
	delay_us(10);

	lcd_write_data(_0b00110000); //init2
	delay_us(10);

	lcd_write_data(_0b00110000); //init3
	delay_us(10);

	lcd_write_data(_0b00111000); // function set  8bit 2line 5x8 dots
	delay_us(10);

	lcd_write_data(_0b00001111); // display on + cursor underline + blinking
	delay_us(10);

	lcd_write_data(_0b00000001); //clear
	delay_us(10);

	lcd_write_data(_0b00000110); //entry mode set
	delay_us(10);

	lcd_write_data(_0b00000010); // return to home
	delay_us(10);


	GPIO_SetBits(LCD_PORT,LCD_RS_Pin);  //rs = 1
	
		for(int i = 0; i<10; i++) 
	{
		lcd_write_data(_0b11111111);
		lcd_write_data(_0b00000000);
	}
}
	
	
	
	

void lcd_write_str(char*str) {
	do {
		lcd_write_data(*str);
	}while(*++str);
}
void lcd_write_cmd(u16 cmd) {
	GPIO_ResetBits(LCD_PORT,LCD_RS_Pin);
	lcd_write_data(cmd);
	GPIO_SetBits(LCD_PORT,LCD_RS_Pin);
}
void lcd_set_cursor(int line,int pos) {
	pos |= _0b10000000;
	if (line == 1) {
		pos += 0x40;
	}
	lcd_write_cmd(pos);
}

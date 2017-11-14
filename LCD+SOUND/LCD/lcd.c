/*
 * lcd.c
 *
 *  Created on: 23.05.2012
 *      Author: lamazavr
 */

#include "lcd.h"
#include "Delay.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "Table.h"

int del_us = 1;
int del_us_init = 30;

void lcd_init_gpio() {
	RCC_AHB1PeriphClockCmd(LCD_RCC_GPIO,ENABLE);

	GPIO_InitTypeDef init;
	init.GPIO_Mode = GPIO_Mode_OUT;
	init.GPIO_Pin = GPIO_Pin_All;
	init.GPIO_OType = GPIO_OType_PP;
	init.GPIO_Speed = GPIO_Speed_100MHz;

	GPIO_Init(LCD_PORT,&init);
}

void lcd_data(char* data) 
{
	GPIO_SetBits(LCD_PORT, A0);
	delay_us(del_us);
	GPIO_SetBits(LCD_PORT, E);
	delay_us(del_us);
	
	LCD_PORT->BSRRH = 0xFF;
	for (int i = 0; i<8; i++)
		LCD_PORT->BSRRL |= ((data[i] - '0')<<i);
	
	GPIO_ResetBits(LCD_PORT, E);
	delay_us(del_us);
	GPIO_SetBits(LCD_PORT, E);
	delay_us(del_us);
}

void lcd_data_int(uint16_t data) 
{
	GPIO_SetBits(LCD_PORT, A0);
	delay_us(del_us);
	//GPIO_SetBits(LCD_PORT, E);
	//delay_us(del_us);
	
	//LCD_PORT->BSRRH = 0xFF;
	//for (int i = 0; i<8; i++)
	//GPIO_ResetBits(LCD_PORT, 0xFF);
	GPIO_ResetBits(LCD_PORT, E);
	
	
	GPIO_ResetBits(LCD_PORT, 0x00FF);
	GPIO_SetBits(LCD_PORT, data);
	
	GPIO_SetBits(LCD_PORT, E);
	
	
	
	//LCD_PORT->BSRRL = data;
	//delay_us(del_us);
	//GPIO_SetBits(LCD_PORT, E);
	//delay_us(del_us);	
	//GPIO_SetBits(LCD_PORT, E);
	//delay_us(del_us);
}

void lcd_command(char* data) 
{
	GPIO_ResetBits(LCD_PORT, A0);
	delay_us(del_us);
	GPIO_SetBits(LCD_PORT, E);
	delay_us(del_us);
	
	LCD_PORT->BSRRH = 0xFF;
	for (int i = 0; i<8; i++)
		LCD_PORT->BSRRL |= ((data[i] - '0')<<(7-i));
	
	GPIO_ResetBits(LCD_PORT, E);
	delay_us(del_us);	
	GPIO_SetBits(LCD_PORT, E);
	delay_us(del_us);
}

void lcd_command_int(uint16_t data) 
{
	GPIO_ResetBits(LCD_PORT, A0);
	delay_us(del_us);
	GPIO_SetBits(LCD_PORT, E);
	delay_us(del_us);
	
	LCD_PORT->BSRRH = 0xFF;
	//for (int i = 0; i<8; i++)
		LCD_PORT->BSRRL = data;
	
	GPIO_ResetBits(LCD_PORT, E);
	delay_us(del_us);	
	GPIO_SetBits(LCD_PORT, E);
	delay_us(del_us);
}



void lcd_init() 
{
	lcd_init_gpio();
	
	GPIO_ResetBits(LCD_PORT, RES);
	
	GPIO_ResetBits(LCD_PORT, A0);
	GPIO_ResetBits(LCD_PORT, RD_WR);
	
	
	delay_us(del_us_init);
	GPIO_SetBits(LCD_PORT, RES);
	delay_us(del_us_init);
	//CS 0
	GPIO_ResetBits(LCD_PORT, CS);
	
	lcd_command("11101110");//END
	lcd_command("10100100");//Static drive ON
	lcd_command("10101001");//Duty Select
	lcd_command("10101111");//Display ON
	lcd_command("10111000");//Page 0
	lcd_command("00000000");//Y 0
	
	
	//CS 1
	GPIO_SetBits(LCD_PORT, CS);
	
	lcd_command("11101110");//END
	lcd_command("10100100");//Static drive ON
	lcd_command("10101001");//Duty Select
	lcd_command("10101111");//Display ON
	lcd_command("10111000");//Page 0
	lcd_command("00000000");//Y 0	
}

void lcd_set_page(int page)
{
	switch(page)
	{
			case 1: lcd_command("10111001");
		break;
			case 2: lcd_command("10111010");
		break;
			case 3: lcd_command("10111011");
		break;	
			default: lcd_command("10111000");		
		break;
	}
}

void lcd_set_chip(int chip)
{
	switch(chip)
	{
			case 1: GPIO_ResetBits(LCD_PORT, CS);
		break;
			default: GPIO_SetBits(LCD_PORT, CS);
		break;
	}
}

void Clear_LCD()
{	
	for(int k = 0; k<2; k++)
	{
		lcd_set_chip(k);
		for(int j = 0; j<4; j++)
		{
			lcd_set_page(j);
			lcd_command("00000000");
			for(int i = 0; i<=60; i++)
			{
					lcd_data("00000000");
				//delay_us(200);
			}
		}
	}
}

void LCD_clear_page(uint16_t page)
{	
	for(int k = 0; k<2; k++)
	{
		lcd_set_chip(k);
			lcd_set_page(page);
			lcd_command("00000000");
			for(int i = 0; i<=60; i++)
					lcd_data("00000000");
	}
}


void lcd_set_Y(uint16_t Y, uint16_t page)
{
	if (Y<=60)
	{
		lcd_set_chip(0);
		lcd_set_page(page);
		lcd_command_int(60-Y);
	}
	else
	{
		lcd_set_chip(1);
		lcd_set_page(page);
		lcd_command_int(Y-61);
	}
}


void lcd_draw_vert_line(uint16_t Y, uint16_t X)
{
	uint16_t page = X / 8;
	for(int i = 0; i < page; i++)
	{
		lcd_set_Y(Y, 3-i);
		lcd_data("11111111");
	}
	uint16_t last_X = X % 8;
	if (last_X != 0)
	{
		lcd_set_Y(Y, 3-page);
		lcd_draw_small_line(last_X, 1);
	}
	for(int i = page+1; i < 4; i++)
	{
		lcd_set_Y(Y, 3-i);
		lcd_data("00000000");
	}
	
	
	
	//char* bin = "00000000";
	/*char* bin = (char*)malloc(2);
	//bin = "00000000";
	lcd_set_Y(Y, 3);
	
	for(int i=0; i<8; i++)
		bin[i] = '0';
	if (X != 0)
		for(int i=0; i<X; i++)
			bin[7-i] = '1';*/
	
	//uint16_t swap_X = ((last_X & 0xff) << 4) | (last_X >> 4);
	//uint16_t X_bin = pow(2, X) - 1;
	//char swap_X = X_bin;//(X_bin * 0x0202020202 & 0x010884422010) % 1023;//Reverse bit sequence 00000111 -> 11100000 
	//lcd_data_int(X_bin);
	//lcd_data(bin);
}
	
/*uint32_t reverse(uint32_t x)
{
    x = ((x >> 1) & 0x55555555u) | ((x & 0x55555555u) << 1);
    x = ((x >> 2) & 0x33333333u) | ((x & 0x33333333u) << 2);
    x = ((x >> 4) & 0x0f0f0f0fu) | ((x & 0x0f0f0f0fu) << 4);
    x = ((x >> 8) & 0x00ff00ffu) | ((x & 0x00ff00ffu) << 8);
    x = ((x >> 16) & 0xffffu) | ((x & 0xffffu) << 16);
    return x;
}*/

void lcd_draw_small_line(uint16_t X, char down)
{
	if(down)
	{
		switch (X)
		{
				case 1: lcd_data("00000001");
			break;
				case 2: lcd_data("00000011");
			break;
				case 3: lcd_data("00000111");
			break;
				case 4: lcd_data("00001111");
			break;
				case 5: lcd_data("00011111");
			break;
				case 6: lcd_data("00111111");
			break;
				case 7: lcd_data("01111111");
			break;
				case 8: lcd_data("11111111");
			break;		
			default: lcd_data("00000000");
			break;
		}
	} else
	{
		switch (X)
		{
				case 1: lcd_data("10000000");
			break;
				case 2: lcd_data("11000000");
			break;
				case 3: lcd_data("11100000");
			break;
				case 4: lcd_data("11110000");
			break;
				case 5: lcd_data("11111000");
			break;
				case 6: lcd_data("11111100");
			break;
				case 7: lcd_data("11111110");
			break;
				case 8: lcd_data("11111111");
			break;		
			default: lcd_data("00000000");
			break;
		}	
	}
	
}


void lcd_write_str(char* str) 
{
	do 
	{
		lcd_data_int(*str);
	}while(*++str);
}

void lcd_draw_char(char** ch) 
{
	int count = atoi( ch[0] );
	for(int i = 1; i < count + 1; i++)
		lcd_data(ch[i]);
}
	
/*void lcd_draw_num_pos(int num, uint16_t Y, uint16_t page) 
{
	int count = atoi( nums[num][0] );
	for(int i = 1; i < count + 1; i++)
	{
		lcd_set_Y((Y + count-1)-(i-1), page);
		lcd_data((nums[num])[i]);
	}
}*/

void lcd_draw_char_pos(char** ch, uint16_t Y, uint16_t page) 
{
	int count = atoi( ch[0] );
	for(int i = 0; i < count; i++)
	{
		lcd_set_Y((Y + count)-i-1, page);
		lcd_data(ch[i+1]);
	}
}

/*void lcd_draw_char_pos_(char ch, uint16_t Y, uint16_t page) 
{
	int ch_pos = ch - 'А';
	int count = atoi( big_rus_sym[ch_pos][0] );
	for(int i = 0; i < count; i++)
	{
		lcd_set_Y((Y + count)-i-1, page);
		lcd_data(big_rus_sym[ch_pos][i+1]);
	}
}*/

void lcd_draw_char_fromTable(uint16_t posT, uint16_t Y, uint16_t page) 
{
	if(posT >= (uint16_t)'а') 
		posT = posT - 'а' + 96 + 0x20 + 0x20;
	else if(posT >= (uint16_t)'А') 
		posT = posT - 'А' + 96 + 0x20;

	for(int i = 0; i < 5; i++)
	{
		lcd_set_Y((Y + 4)-i, page);
		lcd_data_int(table[posT-0x20][4-i]);
	}
}

void lcd_draw_str(char* str, uint16_t Y, uint16_t page) 
{
	int i=0;
	do
	{
		lcd_draw_char_fromTable(*str, Y+i, page );
		delay_us(10000);
		i+=5;
	} while (*++str);
}

void strrev(char *p)
{
  char *q = p;
  while(q && *q) ++q;
  for(--q; p < q; ++p, --q)
    *p = *p ^ *q,
    *q = *p ^ *q,
    *p = *p ^ *q;
}

int lcd_draw_int(int N, uint16_t Y, uint16_t page) 
{
	uint16_t i=0;
	uint16_t str[25];
	uint16_t r;
	int n_ = N;
	if(n_<0)
	{
		n_ = -n_;

    while(n_ != 0)
    {
			r=n_%10;
      n_=n_/10;
			str[i] = '0' + r;
			i++;
    }
		lcd_draw_str("-", Y, page);
		Y += 5;
		for(int j=i-1; j>=0; j--) 
			lcd_draw_char_fromTable(str[j], Y + (((i-1)-j)*5), page);
		return i+1;
	} else
	{
    while(n_ != 0)
    {
			r=n_%10;
      n_=n_/10;
			str[i] = '0' + r;
			i++;
    }
		for(uint16_t j=i-1; j>=0; j--) 
			lcd_draw_char_fromTable(str[j], Y + (((i-1)-j)*5), page);
		return i;
	}
}




void lcd_draw_float(float n_, int col, uint16_t Y, uint16_t page) 
{
	if(n_<0)
	{
		n_ = -n_;
		int H = (int)n_;
		float M = fmod(n_, 1);
		int L = (int)(M * pow(10.0, col));
		Y += 5 * lcd_draw_int(-H, Y, page);
		lcd_draw_str(".", Y, page);
		Y += 5;
		lcd_draw_int(L, Y, page);
	} else
	{
		//Чет не робит
		int H = (int)n_;
		float M = fmod(n_, 1);
		int L = (int)(M * pow(10.0, col));
		Y += 5 * lcd_draw_int(H, Y, page);
		lcd_draw_str(".", Y, page);
		Y += 5;
		lcd_draw_int(L, Y, page);
	}
}
	
	



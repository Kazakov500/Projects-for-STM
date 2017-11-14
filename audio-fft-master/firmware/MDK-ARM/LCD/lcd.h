 /*
 * lcd.h
 *
 *  Created on: 23.05.2012
 *      Author: lamazavr
 */

#ifndef LCD_H_
#define LCD_H_

#include <stm32f4xx_hal_rcc.h>
#include <stm32f4xx_hal_gpio.h>




//#define LCD_CS1_Pin GPIO_Pin_14//1
//#define LCD_CS2_Pin GPIO_Pin_15//2

//3 GND
//4 +5V
//5 Vo Contrast

//#define LCD_RS_Pin GPIO_Pin_10 //6 DI

// EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE<<<<<<<<<<<<<<<<<<<<<

#define LCD_PORT GPIOE
#define LCD_RCC_GPIO RCC_AHB1Periph_GPIOE


#define DB4 GPIO_Pin_4//1 желт//4 not
#define DB5 GPIO_Pin_5//2
#define DB6 GPIO_Pin_6//3
#define DB7 GPIO_Pin_7//4

#define A0 GPIO_Pin_10 //5 DI
#define RD_WR GPIO_Pin_11 //6
#define E GPIO_Pin_12  //7

#define DB3 GPIO_Pin_3//8  (занят SPI mic)
#define DB2 GPIO_Pin_2//9
#define DB1 GPIO_Pin_1//10
#define DB0 GPIO_Pin_0//11

//GND 12
//13 NC not used!
//14 Vcc
//15 +
//16 -


#define RES GPIO_Pin_14//17

#define CS GPIO_Pin_15//18















void delay(unsigned int s);
void lcd_init_gpio();

void lcd_data(char* data);
void lcd_data_int(uint16_t data);
void lcd_command(char* data);
void lcd_command_int(uint16_t data);

void lcd_set_page(int page);
void lcd_set_chip(int chip);

void lcd_set_Y(uint16_t Y, uint16_t page);

void lcd_draw_vert_line(uint16_t Y, uint16_t X);
void lcd_write_str(char* str);
void lcd_draw_small_line(uint16_t X, char down);
	
void lcd_draw_char(char** ch);
void lcd_draw_num_pos(int num, uint16_t Y, uint16_t page);
void lcd_draw_char_pos(char** ch, uint16_t Y, uint16_t page);
void lcd_draw_char_pos_(char ch, uint16_t Y, uint16_t page);
void lcd_draw_char_fromTable(uint16_t posT, uint16_t Y, uint16_t page);
void lcd_draw_str(char* str, uint16_t Y, uint16_t page);

int lcd_draw_int(int N, uint16_t Y, uint16_t page);
void lcd_draw_float(float n_, int col, uint16_t Y, uint16_t page);

void lcd_init();
void Clear_LCD();
void LCD_clear_page(uint16_t page);

#endif /* LCD_H_ */

 /*
 * lcd.h
 *
 *  Created on: 23.05.2012
 *      Author: lamazavr
 */

#ifndef LCD_H_
#define LCD_H_

#include <stm32f4xx_rcc.h>
#include <stm32f4xx_gpio.h>

#define LCD_PORT GPIOC

#define LCD_CS1_Pin GPIO_Pin_14//1
#define LCD_CS2_Pin GPIO_Pin_15//2

//3 GND
//4 +5V
//5 Vo Contrast

#define LCD_RS_Pin GPIO_Pin_10 //6 DI
#define LCD_RW_Pin GPIO_Pin_11 //7
#define LCD_E_Pin GPIO_Pin_12  //8

#define DB0 GPIO_Pin_0//9
#define DB1 GPIO_Pin_1//10
#define DB2 GPIO_Pin_2//11
#define DB3 GPIO_Pin_3//12
#define DB4 GPIO_Pin_4//13
#define DB5 GPIO_Pin_5//14
#define DB6 GPIO_Pin_6//15
#define DB7 GPIO_Pin_7//16

#define RST GPIO_Pin_8//17

//18 -5V

//19 LED +
//20 LED -

#define LCD_RCC_GPIO RCC_AHB1Periph_GPIOC












void delay(unsigned int s);
void lcd_init_gpio();
void lcd_write_data(u16 data);
void lcd_write_data2(u16 _DB7, u16 _DB6, u16 _DB5, u16 _DB4, u16 _DB3, u16 _DB2, u16 _DB1, u16 _DB0 );

void lcd_init();
void lcd_write_str(char*str);
void lcd_write_cmd(u16 cmd);
void lcd_set_cursor(int line,int pos);
void lcd_write_dec_xxx(uint16_t data);
void lcd_write_dec_xxx(uint16_t data);
void lcd_write_dec_xx(uint8_t data);
void lcd_write_dec_x(uint8_t data);
#endif /* LCD_H_ */

/**
 * @file lcd12864.h
 * ���������� ��� ������ � ����������� (128x64) LCD �� �����������, ����������� � KS0108.
 */

#ifndef LCD12864_H
#define	LCD12864_H

//#include <stm32f10x.h>
//#include <stm32f4xx_hal.h>
#include "errors.h"
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "defs.h"

#include "stm32f407xx.h" 

//! ������ ������.
//! ������.
#define LCD12864_WIDTH   128
//! ������.
#define LCD12864_HEIGHT  64
//! ������ � ���������.
#define LCD12864_PAGES_HEIGHT 8
//! ������ � ������.
#define LCD12864_SIZE_BYTES (128 * 64 / 8)

//! ������������ �������� ���������.
//! ������������ ����� ��������.
#define LCD12864_PAGE_MAX        7
//! ������������ ����� ��������.
#define LCD12864_ADDRESS_MAX     127

/**
 * ��������� LCD.
 */
typedef struct _Lcd12864 {
    GPIO_TypeDef* gpio_reset;  //!< ���� ���� RESET.
    uint16_t gpio_pin_reset;   //!< ��� RESET.
    GPIO_TypeDef* gpio_cs1;    //!< ���� ���� CS1.
    uint16_t gpio_pin_cs1;     //!< ��� CS1.
    GPIO_TypeDef* gpio_cs2;    //!< ���� ���� CS2.
    uint16_t gpio_pin_cs2;     //!< ��� CS2.
    GPIO_TypeDef* gpio_e;      //!< ���� ���� E.
    uint16_t gpio_pin_e;       //!< ��� E.
    GPIO_TypeDef* gpio_rw;     //!< ���� ���� RW.
    uint16_t gpio_pin_rw;      //!< ��� RW.
    GPIO_TypeDef* gpio_dc;     //!< ���� ���� DC.
    uint16_t gpio_pin_dc;      //!< ��� DC.
    GPIO_TypeDef* gpio_data;   //!< ���� ������.
    uint32_t gpio_data_offset; //!< �������� ������ ���� ������ � �����.
    uint32_t gpio_data_mask;   //!< ����� � ����� ���� ������.
    uint32_t gpio_data_crl_mask_inv;//!< ��������� ����� � CRL ����� ���� ������.
    uint32_t gpio_data_crl_in;  //!< �������� ����� � CRL ���� ������ ��� �����.
    uint32_t gpio_data_crl_out; //!< �������� ����� � CRL ���� ������ ��� ������.
    uint32_t gpio_data_crh_mask_inv;//!< ��������� ����� � CRH ����� ���� ������.
    uint32_t gpio_data_crh_in;  //!< �������� ����� � CRH ���� ������ ��� �����.
    uint32_t gpio_data_crh_out; //!< �������� ����� � CRH ���� ������ ��� ������.
} lcd12864_t;

/**
 * ��������� ������������� LCD.
 */
typedef struct _Lcd12864_Init {
    GPIO_TypeDef* gpio_reset;  //!< ���� ���� RESET.
    uint16_t gpio_pin_reset;   //!< ��� RESET.
    GPIO_TypeDef* gpio_cs1;    //!< ���� ���� CS1.
    uint16_t gpio_pin_cs1;     //!< ��� CS1.
    GPIO_TypeDef* gpio_cs2;    //!< ���� ���� CS2.
    uint16_t gpio_pin_cs2;     //!< ��� CS2.
    GPIO_TypeDef* gpio_e;      //!< ���� ���� E.
    uint16_t gpio_pin_e;       //!< ��� E.
    GPIO_TypeDef* gpio_rw;     //!< ���� ���� RW.
    uint16_t gpio_pin_rw;      //!< ��� RW.
    GPIO_TypeDef* gpio_dc;     //!< ���� ���� DC.
    uint16_t gpio_pin_dc;      //!< ��� DC.
    GPIO_TypeDef* gpio_data;   //!< ���� ������.
    uint32_t gpio_data_offset; //!< �������� ������ ���� ������ � �����.
} lcd12864_init_t;


/**
 * �������������� LCD � ��� ������ ����.
 * @param lcd LCD.
 * @param lcd_init ��������� ������������� LCD.
 * @return ��� ������.
 */
EXTERN err_t lcd12864_init(lcd12864_t* lcd, lcd12864_init_t* lcd_init);

/**
 * ���������� LCD.
 * @param lcd LCD.
 * @return ��� ������.
 */
EXTERN err_t lcd12864_reset(lcd12864_t* lcd);

/**
 * ��������� ��������� LCD.
 * @param lcd LCD.
 * @return ���� ��������� LCD.
 */
EXTERN bool lcd12864_busy(lcd12864_t* lcd);

/**
 * ��� ������ ����� ��������� LCD.
 * @param lcd LCD.
 * @return ���� ������������ LCD.
 */
EXTERN bool lcd12864_wait(lcd12864_t* lcd);

/**
 * ������������� ������������ �������, ������� � ������� �������.
 * @param lcd LCD.
 * @param display_on ���� ��������� �������.
 */
EXTERN err_t lcd12864_control(lcd12864_t* lcd, bool display_on);

/**
 * ������������� ��������� ����� LCD.
 * @param lcd LCD.
 * @param start_line ��������� �����.
 * @return ��� ������.
 */
EXTERN err_t lcd12864_set_start_line(lcd12864_t* lcd, uint8_t start_line);

/**
 * ������� LCD.
 * @param lcd LCD.
 * @return ��� ������.
 */
EXTERN err_t lcd12864_clear(lcd12864_t* lcd);

/**
 * ���������� ������ � LCD.
 * @param lcd LCD.
 * @param data ������.
 * @param count ������ ������.
 * @return ��� ������.
 */
EXTERN err_t lcd12864_write(lcd12864_t* lcd, const uint8_t* data, size_t count);

#endif	/* LCD12864_H */
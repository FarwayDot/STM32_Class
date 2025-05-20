/*
 * lcd.h
 *
 *  Created on: Mar 15, 2024
 *      Author: jeffr
 */

#ifndef LCD_LCD_H_
#define LCD_LCD_H_

#ifdef __cplusplus
extern "C"{
#endif

/*Includes ------------------------------------------------------*/
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <lwprintf.h>


#include <stdarg.h>


#define LCD_USE_I2C_INTERFACE	0

#define LCD_ASSERT(x)		x == NULL
/*Typedefs ------------------------------------------------------*/
#if LCD_USE_I2C_INTERFACE == 0
typedef void(*lcd_write_nb_fc)(uint8_t data, uint8_t control);
#else
typedef uint8_t(*lcd_i2c_send_cmd_fc)(uint8_t cmd);
typedef uint8_t(*lcd_i2c_send_data_fc)(uint8_t data);
#define SLAVE_ADDRESS_LCD 		0x20<<1             //verificar la direccion i2c del LCD
#endif
typedef void(*lcd_delay_fc)(uint32_t d);


typedef struct {
	uint8_t DisplayControl;
	uint8_t DisplayFunction;
	uint8_t DisplayMode;
	uint8_t Rows;
	uint8_t Cols;
	uint8_t currentX;
	uint8_t currentY;
#if LCD_USE_I2C_INTERFACE == 0
	lcd_write_nb_fc writeNb;
#else
	lcd_i2c_send_cmd_fc writeCmd;
	lcd_i2c_send_data_fc writeData;
#endif
	lcd_delay_fc delay;
}LCD_t;

/*Function prototype --------------------------------------------*/

void lcd_init(LCD_t *d,
#if LCD_USE_I2C_INTERFACE == 0
		lcd_write_nb_fc w,
#else
		lcd_i2c_send_cmd_fc cmd,
		lcd_i2c_send_data_fc data,
#endif
		lcd_delay_fc del, uint8_t cols, uint8_t rows);

void lcd_displayOn(LCD_t *d);

void lcd_displayOff(LCD_t *d);

void lcd_clear(LCD_t *d);

void lcd_puts(LCD_t *d, uint8_t x, uint8_t y, char *str);

void lcd_blinkOn(LCD_t *d);

void lcd_blinkOff(LCD_t *d);

void lcd_cursorOn(LCD_t *d);

void lcd_cursorOff(LCD_t *d);

void lcd_scrollRight(LCD_t *d);

void lcd_scrollLeft(LCD_t *d);

void lcd_createChar(LCD_t *d, uint8_t location, uint8_t *data);

void lcd_putCustom(LCD_t *d, uint8_t x, uint8_t y, uint8_t location);

void lcd_printf(LCD_t *d, uint8_t x, uint8_t y, char *format, ...);

void lcd_clearLine(LCD_t *d, uint8_t col1, uint8_t col2, uint8_t row);

#ifdef __cplusplus
}
#endif

#endif /* LCD_LCD_H_ */

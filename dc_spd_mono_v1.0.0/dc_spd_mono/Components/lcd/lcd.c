/*
 * lcd.c
 *
 *  Created on: Mar 15, 2024
 *      Author: jeffr
 */
#include "lcd.h"

/*Defines -------------------------------------------*/
/* Commands*/
#define HD44780_CLEARDISPLAY        0x01
#define HD44780_RETURNHOME          0x02
#define HD44780_ENTRYMODESET        0x04
#define HD44780_DISPLAYCONTROL      0x08
#define HD44780_CURSORSHIFT         0x10
#define HD44780_FUNCTIONSET         0x20
#define HD44780_SETCGRAMADDR        0x40
#define HD44780_SETDDRAMADDR        0x80

/* Flags for display entry mode */
#define HD44780_ENTRYRIGHT          0x00
#define HD44780_ENTRYLEFT           0x02
#define HD44780_ENTRYSHIFTINCREMENT 0x01
#define HD44780_ENTRYSHIFTDECREMENT 0x00

/* Flags for display on/off control */
#define HD44780_DISPLAYON           0x04
#define HD44780_CURSORON            0x02
#define HD44780_BLINKON             0x01

/* Flags for display/cursor shift */
#define HD44780_DISPLAYMOVE         0x08
#define HD44780_CURSORMOVE          0x00
#define HD44780_MOVERIGHT           0x04
#define HD44780_MOVELEFT            0x00

/* Flags for function set */
#define HD44780_8BITMODE            0x10
#define HD44780_4BITMODE            0x00
#define HD44780_2LINE               0x08
#define HD44780_1LINE               0x00
#define HD44780_5x10DOTS            0x04
#define HD44780_5x8DOTS             0x00
/*Private function -----------------------------------*/
static void lcd_cmd(LCD_t *d, uint8_t cmd);
static void lcd_data(LCD_t *d, uint8_t data);
static void lcd_cursorSet(LCD_t *d, uint8_t col, uint8_t row);
/*Function definition --------------------------------*/
void lcd_init(LCD_t *d,
#if LCD_USE_I2C_INTERFACE == 0
		lcd_write_nb_fc w,
#else
		lcd_i2c_send_cmd_fc cmd,
		lcd_i2c_send_data_fc data,
#endif
		lcd_delay_fc del, uint8_t cols, uint8_t rows){

	if(LCD_ASSERT(d) || LCD_ASSERT(del)){
		return;
	}
#if LCD_USE_I2C_INTERFACE
	if(LCD_ASSERT(cmd) || LCD_ASSERT(data)){
		return;
	}
#else
	if(LCD_ASSERT(w)){
		return;
	}
#endif
#if LCD_USE_I2C_INTERFACE
	d->writeCmd = cmd;
	d->writeData = data;
#else
	d->writeNb = w;
#endif

	d->delay = del;
	d->Rows = rows;
	d->Cols = cols;
	/**
	 * init
	 */
	d->currentX = 0;
	d->currentY = 0;

	d->DisplayFunction = HD44780_4BITMODE | HD44780_5x8DOTS | HD44780_1LINE;
	if(d->Rows>1){
		d->DisplayFunction |=  HD44780_2LINE;
	}

	/*Init 4 bit mode*/
#if LCD_USE_I2C_INTERFACE == 0
	d->writeNb(0x03, 0);
	d->delay(45);		//45ms

	d->writeNb(0x03, 0);
	d->delay(45);		//45ms

	d->writeNb(0x03, 0);
	d->delay(45);		//45ms

	d->writeNb(0x02, 0);
	d->delay(1);		//1ms

	lcd_cmd(d, HD44780_FUNCTIONSET | d->DisplayFunction);

	d->DisplayControl = HD44780_DISPLAYON;
	lcd_displayOn(d);

	lcd_clear(d);

	d->DisplayMode = HD44780_ENTRYLEFT | HD44780_ENTRYSHIFTDECREMENT;

	lcd_cmd(d, HD44780_ENTRYMODESET | d->DisplayMode);


	d->delay(45);
#else
	d->delay(50);

	d->writeCmd(0x30);

	d->delay(5);

	d->writeCmd(0x30);

	d->delay(1);

	d->writeCmd(0x30);

	d->delay(10);

	d->writeCmd(0x20);

	lcd_cmd(d, HD44780_FUNCTIONSET | d->DisplayFunction);

	d->DisplayControl = HD44780_DISPLAYON;
	lcd_displayOn(d);

	lcd_clear(d);

	d->DisplayMode = HD44780_ENTRYLEFT | HD44780_ENTRYSHIFTDECREMENT;

	lcd_cmd(d, HD44780_ENTRYMODESET | d->DisplayMode);


	d->delay(45);
#endif
	return;
}

void lcd_clear(LCD_t *d){
	lcd_cmd(d, HD44780_CLEARDISPLAY);
	d->delay(3);
}

void lcd_puts(LCD_t *d, uint8_t x, uint8_t y, char *str){
	lcd_cursorSet(d, x, y);
	while(*str){
		if(d->currentX >= d->Cols){
			d->currentX = 0;
			d->currentY += 1;
			lcd_cursorSet(d, d->currentX, d->currentY);
		}
		if(*str == '\n'){
			d->currentY += 1;
			lcd_cursorSet(d, d->currentX, d->currentY);
		}else if(*str == '\r'){
			lcd_cursorSet(d, 0, d->currentY);
		}else{
			lcd_data(d, *str);
			d->currentX +=1;
		}
		str += 1;
	}
}

void lcd_displayOn(LCD_t *d){
	d->DisplayControl |=  HD44780_DISPLAYON;
	lcd_cmd(d, HD44780_DISPLAYCONTROL | d->DisplayControl);
}

void lcd_displayOff(LCD_t *d){
	d->DisplayControl &=~  HD44780_DISPLAYON;
	lcd_cmd(d, HD44780_DISPLAYCONTROL | d->DisplayControl);
}

void lcd_blinkOn(LCD_t *d){
	d->DisplayControl |= HD44780_BLINKON;
	lcd_cmd(d, HD44780_DISPLAYCONTROL | d->DisplayControl);
}

void lcd_blinkOff(LCD_t *d){
	d->DisplayControl &=~ HD44780_BLINKON;
	lcd_cmd(d, HD44780_DISPLAYCONTROL | d->DisplayControl);
}

void lcd_cursorOn(LCD_t *d){
	d->DisplayControl |= HD44780_CURSORON;
	lcd_cmd(d, HD44780_DISPLAYCONTROL | d->DisplayControl);
}

void lcd_cursorOff(LCD_t *d){
	d->DisplayControl &=~ HD44780_CURSORON;
	lcd_cmd(d, HD44780_DISPLAYCONTROL | d->DisplayControl);
}

void lcd_scrollLeft(LCD_t *d){
	lcd_cmd(d, HD44780_CURSORSHIFT | HD44780_DISPLAYMOVE | HD44780_MOVELEFT);
}
void lcd_scrollRight(LCD_t *d){
	lcd_cmd(d, HD44780_CURSORSHIFT | HD44780_DISPLAYMOVE | HD44780_MOVERIGHT);
}

void lcd_createChar(LCD_t *d, uint8_t location, uint8_t *data){
	location &= 0x07;
	lcd_cmd(d, HD44780_SETCGRAMADDR | (location << 3));
	for(uint8_t i = 0; i <8; i++){
		lcd_data(d, *data);
	}

}

void lcd_putCustom(LCD_t *d, uint8_t x, uint8_t y, uint8_t location){
	lcd_cursorSet(d, x, y);
	lcd_data(d, location);
}

void lcd_printf(LCD_t *d, uint8_t x, uint8_t y, char *format, ...){
	char str[80];
	/**
	 * printf("var:%.1f temp", T);
	 */
	va_list args;
	va_start(args, format);
	lwprintf_vsnprintf(str, 80, format, args);
#if 0
	vsprintf(str, format, args);
#endif
	lcd_puts(d, x, y, str);
	va_end(args);
	return;
}


void lcd_clearLine(LCD_t *d, uint8_t col1, uint8_t col2, uint8_t row){
	if(row >= d->Rows){
		return;
	}
	lcd_cursorSet(d, col1, row);
	for(uint8_t c = col1; c<= col2; c++){
		lcd_data(d, ' ');
	}
	lcd_cursorSet(d, col1, row);
	return;
}
/*Private function ------------------------------------------*/
void lcd_cmd(LCD_t *d, uint8_t cmd){

#if LCD_USE_I2C_INTERFACE == 0
	d->writeNb((cmd>>4) & 0x0F,0);
	d->writeNb(cmd & 0x0F,0);
#else
	d->writeCmd(cmd);
#endif
	if(cmd < 4){
		d->delay(4);
	}else{
		d->delay(3);
	}
	return;
}

static void lcd_data(LCD_t *d, uint8_t data){
#if LCD_USE_I2C_INTERFACE == 0
	uint32_t delay = 256;
	d->writeNb((data>>4) & 0x0F,1);
	d->writeNb(data & 0x0F,1);
	//d->delay(1);
	while(delay-->0);
#else
	d->writeData(data);
#endif
}

static void lcd_cursorSet(LCD_t *d, uint8_t col, uint8_t row){

	//uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
	uint8_t row_offsets[] = {0x00, 0x40, 0x10, 0x50};
	if(row>= d->Rows){
		row = 0;
	}
	d->currentX = col;
	d->currentY = row;

	lcd_cmd(d, HD44780_SETDDRAMADDR | (col + row_offsets[row]));
	return;
}

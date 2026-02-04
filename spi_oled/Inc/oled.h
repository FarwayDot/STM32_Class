/*
 * oled.h
 *
 *  Created on: Jan 7, 2026
 *      Author: User
 */


#include "stm32f4xx.h"
#include "Config.h"
#include "GPIO_Config.h"
#include <stdlib.h>
#include <string.h>
#include "Delay.h"

/*Oled display*/

#define PIN_OLED_CS		4		//CS
#define GPIO_OLED_CS 	GPIOA
#define OLED_CS			A, PIN_OLED_CS

#define PIN_OLED_DC		2		//DC
#define GPIO_OLED_DC	GPIOA
#define OLED_DC			A, PIN_OLED_DC

#define PIN_OLED_RST	3		//RST
#define GPIO_OLED_RST	GPIOA
#define OLED_RST		A, PIN_OLED_RST

#define OLED_SPI		SPI1

/*Command*/

#define PAGE_SIZE    8
#define XLevelL		0x00
#define XLevelH		0x10
#define YLevel       0xB0
#define	Brightness	 0xFF
#define WIDTH 	     128
#define HEIGHT 	     64

#define u8			uint8_t
#define u16			uint16_t
#define u32			uint32_t

/*Macro Functions*/

#define OLED_CMD	0
#define OLED_DATA	1

#define OLED_CS_Clr()  GPIOX_ODR(OLED_CS) = 0
#define OLED_CS_Set()  GPIOX_ODR(OLED_CS) = 1

#define OLED_DC_Clr()  GPIOX_ODR(OLED_DC) = 0
#define OLED_DC_Set()  GPIOX_ODR(OLED_DC) = 1

#define OLED_RST_Clr()  GPIOX_ODR(OLED_RST) = 0
#define OLED_RST_Set()  GPIOX_ODR(OLED_RST) = 1


/*Functions*/

void OLED_WR_Byte(unsigned dat,unsigned cmd);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_Reset(void);
void OLED_Init_GPIO(void);
void OLED_Init(void);
void OLED_Set_Pixel(unsigned char x, unsigned char y,unsigned char color);
void OLED_Display(void);
void OLED_Clear(unsigned dat);


/*
 * DISPLAY_7_SEGM.h
 *
 *  Created on: Dec 19, 2024
 *      Author: jeanf
 */

#ifndef DISPLAY_7_SEGM_H_
#define DISPLAY_7_SEGM_H_

#include "stm32f4xx.h"
#include <stdint.h>
#include <math.h>
#include "Delay.h"
#include "Config.h"


/* Define Segment Pin */

#define SEG_A_PORT	GPIOB
#define SEG_A_PIN	8

#define SEG_B_PORT	GPIOB
#define SEG_B_PIN	9

#define SEG_C_PORT	GPIOC
#define SEG_C_PIN	9

#define SEG_D_PORT	GPIOA
#define SEG_D_PIN	6

#define SEG_E_PORT	GPIOA
#define SEG_E_PIN	7

#define SEG_F_PORT	GPIOB
#define SEG_F_PIN	6

#define SEG_G_PORT	GPIOC
#define SEG_G_PIN	7

#define SEG_P_PORT	GPIOA
#define SEG_P_PIN	9

/* Define Control Pin*/

#define SEG_DISP1_PORT	GPIOB
#define SEG_DISP1_PIN	10
#define SEG_DISP1		B, SEG_DISP1_PIN

#define SEG_DISP2_PORT	GPIOA
#define SEG_DISP2_PIN	0
#define SEG_DISP2		A, SEG_DISP2_PIN

#define SEG_DISP3_PORT	GPIOB
#define SEG_DISP3_PIN	5
#define SEG_DISP3		B, SEG_DISP3_PIN

#define SEG_DISP4_PORT	GPIOA
#define SEG_DISP4_PIN	1
#define SEG_DISP4		A, SEG_DISP4_PIN

#define TOTAL			1

// Macro para habilitar el reloj del puerto
#define ENABLE_GPIO_CLOCK(port) \
    do { \
        if (port == GPIOA) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; \
        if (port == GPIOB) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; \
        if (port == GPIOC) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; \
    } while (0)

/* Definición de funciones */


// Define las conexiones del display (pines GPIO)
typedef struct {
    GPIO_TypeDef* port; //Para configuración del puerto
    uint16_t pin;		//Para el pin
} SevenSegment_Pin;

// Configuración para el display (7 segmentos + punto decimal)
typedef struct {
    SevenSegment_Pin segmentPins[8]; // A, B, C, D, E, F, G, DP //Se crea un arreglo que contiene la configuración de
    															// los pines de cada segmento
} SevenSegment_Display;

// Prototipos de funciones
void InitPinAsOutput(GPIO_TypeDef* port, uint16_t pin);
void SevenSegment_Init(void);
void SevenSegment_DisplayDigit(uint8_t digit);
void SevenSegment_Clear(void);
void SevenSegment_SelectDisplay(uint8_t display_chosen);
void SevenSegment_Integer(uint16_t number, uint16_t num_repet);



#endif /* DISPLAY_7_SEGM_H_ */

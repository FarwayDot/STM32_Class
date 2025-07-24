/*
 * timer_capture.h
 *
 *  Created on: May 26, 2025
 *      Author: User
 */

#ifndef TIMER_CAPTURE_H_
#define TIMER_CAPTURE_H_

/*Librerías*/
#include <stm32f4xx.h>
#include <stdint.h>

/* Definiciones */


/* Variables globlaes, enumeraciones, listas */
typedef enum
{
	ISR_OFF = 0,
	ISR_ON
}ISR_e;

/* Prototipo de funciones */

void timer3_count_config(void);
void timer3_count_start(void);

void timer2_count_config(uint16_t psc, uint32_t arr);
void timer2_count_start(void);
void timer2_ch3_gpio_config(void);
void timer2_ch3_input_capture_config(uint16_t psc, uint32_t arr);
void timer2_ch3_input_capture_start(uint8_t isr);

void timer2_count_start_IT(void);
#endif /* TIMER_CAPTURE_H_ */

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


/* Prototipo de funciones */
void timer2_count_config(void);
void timer2_count_start(void);
void timer2_ch3_gpio_config(void);
void timer2_ch3_input_capture_config(void);
void timer2_ch3_input_capture_start(void);
void timer_input_calculation(void);

#endif /* TIMER_CAPTURE_H_ */

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
void timer_config(void);
void timer_input_capture_conf(void);
void timer_input_capture_start(void);


#endif /* TIMER_CAPTURE_H_ */

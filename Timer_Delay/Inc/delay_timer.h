/*
 * delay_timer.h
 *
 *  Created on: May 5, 2025
 *      Author: User
 */

#ifndef DELAY_TIMER_H_
#define DELAY_TIMER_H_

/*Librerías*/
#include <stm32f4xx.h>
#include <stdint.h>

/* Definiciones */


/* Variables globlaes, enumeraciones, listas */


/* Prototipo de funciones */
void delay_tim4_init(uint32_t ck_psc);
void delay_ms_tim4(uint32_t delay_ms);


#endif /* DELAY_TIMER_H_ */

/*
 * Timer2.h
 *
 *  Created on: Aug 12, 2025
 *      Author: User
 */

#ifndef TIMER2_H_
#define TIMER2_H_

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
void tim2_ch1_pa0_ic_gpio_config(void);
void tim2_count_config(uint16_t psc, uint32_t arr);
void tim2_count_start(void);
void tim2_PWM_InputCapture_Start(void);
void tim2_PWM_InputCapture_Config(uint8_t interr);



#endif /* TIMER2_H_ */

/*
 * SysTick.h
 *
 *  Created on: Dec 1, 2024
 *      Author: jeanf
 */

#ifndef SYSTICK_H_
#define SYSTICK_H_


#define MODE_DELAY	1

#if MODE_DELAY == 1

void delay_init(void);
void delay_us(uint32_t delay);
void delay_ms(uint32_t delay);

#elif MODE_DELAY == 2

void delay_init(void);
void delay_ms(uint32_t delay);

#elif MODE_DELAY == 3

void delay_init(void);
void delay_prueba(void);

#endif

#endif /* SYSTICK_H_ */

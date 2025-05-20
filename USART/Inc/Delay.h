/*
 * Delay.h
 *
 *  Created on: Dec 17, 2024
 *      Author: jeanf
 */

#ifndef DELAY_H_
#define DELAY_H_

#include "stm32f4xx.h"
#include <stdint.h>

#define USE_DELAY_US  	0

void Delay_Init(uint32_t ticks);

#if USE_DELAY_US == 1
void delay_us(uint32_t delay);
#endif

void delay_ms(uint32_t delay);

#endif /* DELAY_H_ */

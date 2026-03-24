/*
 * RCC.h
 *
 *  Created on: Nov 19, 2024
 *      Author: jeanf
 */

#ifndef RCC_H_
#define RCC_H_

#include "stm32f4xx.h"

#define PLL_ON

#ifdef PLL_OFF

/*
 * Selecciona el HSE como sysclk
 */
void HSE_Config(void);

/*
 * Selecciona el HSI como sysclk
 */
void HSI_Config(void);

#endif

#ifdef PLL_ON

/*
 * Selecciona el HSE como sysclk
 */
void HSE_Config_PLL(void);

/*
 * Selecciona el HSI como sysclk
 */
void HSI_Config_PLL(void);

/*
 * Configurar PLL a 84Mhz
 */
void PLL_Config(uint8_t Source);

typedef enum
{
	HSI_SOURCE,
	HSE_SOURCE
}PLL_Source_t;

#endif

/*
 * Configuración de la flash
 */
void flash_config(void);

#endif /* RCC_H_ */

/*
 * max6675.h
 *
 *  Created on: Jan 10, 2026
 *      Author: User
 */

#ifndef MAX6675_H_
#define MAX6675_H_

#ifdef A

/*Inclusiónd de librerías*/

#include "Config.h"

/*Definiciones*/

#define PIN_MAX6675_CS		5
#define GPIO_MAX6675_CS		GPIOA
#define MAX6675_CS			A, 5

/*Macros*/
#define MAX6675_CS_ON()		GPIOX_ODR(MAX6675_CS) = 0
#define MAX6675_CS_OFF()	GPIOX_ODR(MAX6675_CS) = 1

/*Prototipo de funciones*/

void MAX6675_Init(void);
float MAX6675_Read(SPI_TypeDef *SPIx);

#endif

#endif /* MAX6675_H_ */

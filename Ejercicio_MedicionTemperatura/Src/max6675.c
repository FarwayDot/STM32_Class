/*
 * max6675.c
 *
 *  Created on: Jan 10, 2026
 *      Author: User
 */

/*Inclusiónd de librerías*/

#include "max6675.h"
#include "stdint.h"
#include "spi.h"
#include "GPIO_Config.h"
#include "Delay.h"

#ifdef A
/*Definición de funciones*/

void MAX6675_Init(void)
{
	GPIO_Output_Config(GPIO_MAX6675_CS, PIN_MAX6675_CS, PUPDR_NONE, OSPEEDR_VERYHIGH, OTYPER_PP);
	MAX6675_CS_OFF();

	return;
}


float MAX6675_Read(SPI_TypeDef *SPIx)
{
	float Temp=0;

	MAX6675_CS_OFF();
	MAX6675_CS_OFF();
	delay_ms(250);
	return Temp;
}
#endif

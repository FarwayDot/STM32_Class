/*
 * SysTick.c
 *
 *  Created on: Dec 1, 2024
 *      Author: jeanf
 */

#include <stdint.h>
#include "stm32f4xx.h"
#include "SysTick.h"


#if MODE_DELAY == 1

void delay_init(void)
{
	SysTick -> LOAD = (SystemCoreClock)/1000000;					//Notar que no restamos ya que sería (1/tiempo)																//lo que lo hace insignificante
	SysTick -> CTRL |= 1U<<2; 										//Elegir fuente de reloj
	SysTick -> CTRL &= ~SysTick_CTRL_ENABLE_Msk; 					//Deshabilitamos el SysTick
}

void delay_us(uint32_t delay)
{
	uint32_t i=0;

	SysTick -> VAL = 0;
	SysTick -> CTRL |= SysTick_CTRL_ENABLE_Msk;					 	//Habilitar el conteo

	for(i=0; i<delay; i++)
	{
		while(!(SysTick -> CTRL & (1U<<16)));						//Retardo de 1 us aprox
	}
	SysTick -> CTRL &= ~SysTick_CTRL_ENABLE_Msk; 					//Deshabilitamos el SysTick

	return ;
}

void delay_ms(uint32_t delay)
{
	uint32_t i=0;

	for(i=0;i<delay;i++)
	{
		delay_us(1000);
	}
}

#elif MODE_DELAY == 2

void delay_init(void)
{
	SysTick -> CTRL |= 1U<<2;
}

void delay_ms(uint32_t delay)
{
	//uint32_t i=0;
	SysTick -> CTRL &= ~SysTick_CTRL_ENABLE_Msk; 				//Deshabilitamos el SysTick
	SysTick -> LOAD = (uint32_t)((SystemCoreClock)*(((float)delay)/1000.0) - 1);
	SysTick -> VAL = 0;
	SysTick -> CTRL |= SysTick_CTRL_ENABLE_Msk;					 //Habilitar el conteo
	while(!(SysTick -> CTRL & (1U<<16)));						//Retardo de 1 ms
	SysTick -> CTRL &= ~SysTick_CTRL_ENABLE_Msk; 				//Deshabilitamos el SysTick
	return ;

}

//void delay_ms(uint32_t delay)
//{
//	SysTick -> CTRL &= ~SysTick_CTRL_ENABLE_Msk; 					//Deshabilitamos el SysTick
//	SysTick -> LOAD = SystemCoreClock * ((delay)/1000) - 1; 		//Forma 1 Cargamos el LOAD
//	SysTick -> VAL = 0;
//
//	SysTick -> CTRL |= 1U<<2; 										//Elegir fuente de reloj AHB
//	SysTick -> CTRL |= SysTick_CTRL_ENABLE_Msk;					 	//Habilitar el conteo
//
//	while(!(SysTick -> CTRL & (1U<<16)));							//Revisamos el valor de la bandera
//
//	return ;
//}

#elif MODE_DELAY == 3

void delay_init(void)
{
	SysTick -> LOAD = 0x00FFFFFF; //Cargamos el valor de donde queremos que empieza
	SysTick -> CTRL |= 1U<<2; //Fuente de reloj
	SysTick -> CTRL &= ~SysTick_CTRL_ENABLE_Msk;	//Deshabilitamos el SysTick
}

void delay_prueba()
{
	uint32_t i=0;
	SysTick -> VAL = 0;
	SysTick -> CTRL |= SysTick_CTRL_ENABLE_Msk;		//Habilitar el conteo
	while(!(SysTick -> CTRL & (1U<<16)))
	{
		i++;
	}
	SysTick -> CTRL &= ~SysTick_CTRL_ENABLE_Msk; 	//Deshabilitamos el SysTick
}

#endif

/*
 * Delay.c
 *
 *  Created on: Dec 17, 2024
 *      Author: jeanf
 */

#include "Delay.h"

uint32_t uwTick = 0;

static uint32_t GetTick(void)
{
	return uwTick;
}

// Inicializamos Systick e interrupción
void Delay_Init(uint32_t ticks)
{
	SysTick -> CTRL &= ~SysTick_CTRL_ENABLE_Msk;	//Dehabilitamos systick
	SysTick -> LOAD = ticks - 1;				 	//Cargamos valor LOAD
	SysTick -> CTRL |= 1U<<2; 						//Escogemos fuente de reloj
	NVIC_SetPriority(SysTick_IRQn, 7);				//Asignamos prioridad de 7
	SysTick -> CTRL |= 1<<1;						//Activamos interrupción por conteo
	SysTick -> CTRL |= SysTick_CTRL_ENABLE_Msk;		//Habilitamos conteo
	return;
}

#if USE_DELAY_US == 1 //Si utilizamos delay_us

void delay_us(uint32_t delay)
{
	int32_t tickstart = GetTick();
	uint32_t wait = delay;
	while((GetTick() - tickstart) < wait);
	return;
}

#endif

void delay_ms(uint32_t delay)
{

#if USE_DELAY_US == 1 //Si utilizamos delay_us
	uint32_t i=0;
	for(i=0; i<delay; i++)
	{
		delay_us(1000);
	}
#else

	int32_t tickstart = GetTick();
	uint32_t wait = delay;
	while((GetTick() - tickstart) < wait);

#endif

}





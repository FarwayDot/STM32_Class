/*
 * delay_timer.c
 *
 *  Created on: May 5, 2025
 *      Author: User
 */

#include "delay_timer.h"


volatile uint32_t uwTick_tim4;

/*
 * @brief Retorna el valor de la variable ticks
 * @params none
 * @return ticks
 */
static uint32_t GetTick(void)
{
	return uwTick_tim4;
}

/*
 * @brief Función para inicializar el TIM4 como base de tiempo para retardos
 * @params [ck_psc]: frecuencia de entrada al timer
 */
void delay_tim4_init(uint32_t ck_psc)
{
	uint32_t temp;

	/*Activamos RCC del TIM4*/
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	/*Calculo del prescaler*/
	TIM4->CR1 &= ~TIM_CR1_CEN; //Deshabilitamos
	temp = ck_psc/1000000; //Obtenemos 1us
	TIM4->PSC = temp - 1;
	/*Calculo del auto-reload*/
	TIM4->ARR = 1000 - 1; //Obtenemos 1ms
	/*IT*/
	TIM4->DIER |= TIM_DIER_UIE; //Local
	NVIC_EnableIRQ(TIM4_IRQn);
	/*Activamos conteo*/
	TIM4->CR1 |= TIM_CR1_CEN;
}


/*
 * @brief generar retardos con TIM4
 * @params [ms]
 */
void delay_ms_tim4(uint32_t delay_ms)
{
	int32_t tickstart = GetTick();
	uint32_t wait = delay_ms;
	while((GetTick() - tickstart) < wait);
}

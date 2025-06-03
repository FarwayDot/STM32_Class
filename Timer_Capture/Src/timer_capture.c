/*
 * timer_capture.c
 *
 *  Created on: May 26, 2025
 *      Author: User
 */


#include "timer_capture.h"

void timer_capture_conf(void)
{
	/*Enlazar registro CCR a TI1*/
	TIM2 -> CCMR1 &= ~TIM_CCMR1_CC1S_Msk;
	TIM2 -> CCMR1 |= (0b01<<TIM_CCMR1_CC1S_Pos);
	/*Programar filtro*/
	TIM2 -> CCMR1 &= ~TIM_CCMR1_IC1F_Msk; //Limpiamos
	TIM2 -> CCMR1 |= (0b0011<<TIM_CCMR1_IC1F_Pos); //Sampling 8
	/*Flanco de detección*/
	TIM2 -> CCER  &= ~(TIM_CCER_CC1P_Msk | TIM_CCER_CC1NP_Msk); // noninverted/rising edge
	/*Prescaler*/
	TIM2 -> CCMR1 &= ~TIM_CCMR1_IC1PSC_Msk; //No prescaler
	/*Habilitar captura*/

	/*Deshabilitar interrupción*/
	TIM2 -> DIER &= ~TIM_DIER_CC1IE_Msk;

	return;
}

void timer_capture_start(void)
{
	/*Habilitar interrupción*/
	TIM2 -> DIER |= (1<<TIM_DIER_CC1IE_Pos);
	/*Habilitar comparación*/
	TIM2 -> CCER |= (1<<TIM_CCER_CC1E_Pos);

	return;
}

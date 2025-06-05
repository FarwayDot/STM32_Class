/*
 * timer_capture.c
 *
 *  Created on: May 26, 2025
 *      Author: User
 */


#include "timer_capture.h"

void timer_config(void)
{
	/*Pin Config TIM2_CH3 PA2*/
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN; //RCC

	GPIOA -> MODER &= ~GPIO_MODER_MOD2_0; //Modo AF
	GPIOA -> MODER |= GPIO_MODER_MODE2_1;

	GPIOA -> OSPEEDR |= GPIO_OSPEEDR_OSPEED2; //Very High Speed

	GPIOA -> PUPDR &= ~GPIO_PUPDR_PUPD2_Msk; //No pull up/down

	GPIOA -> AFR[0] &= ~GPIO_AFRL_AFRL2; //Configurar AF
	GPIOA -> AFR[0] |= GPIO_AFRL_AFRL2_0;

	

	/*APB1 TIM2*/
	RCC -> APB1ENR |= RCC_APB1ENR_TIM2EN;
}

void timer_input_capture_conf(void)
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

void timer_input_capture_start(void)
{
	/*Habilitar interrupción*/
	TIM2 -> DIER |= (1<<TIM_DIER_CC1IE_Pos);
	/*Habilitar comparación*/
	TIM2 -> CCER |= (1<<TIM_CCER_CC1E_Pos);

	return;
}

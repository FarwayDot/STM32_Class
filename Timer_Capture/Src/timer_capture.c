/*
 * timer_capture.c
 *
 *  Created on: May 26, 2025
 *      Author: User
 */


#include "timer_capture.h"

/*
 * @brief Timer2 configuration as counter
 *
 */

void timer2_count_config(void)
{
	/*Activamos RCC del TIM4*/
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->CR1 &= ~TIM_CR1_CEN; //Deshabilitamos
	/*Calculo del prescaler*/
	TIM2->PSC = 1599 - 1;
	/*Calculo del auto-reload*/
	TIM2->ARR = 10 - 1; //Obtenemos 1ms

	return;
}


/*
 * @brief Timer2 start count
 *
 */

void timer2_count_start(void)
{
	/*Habilitar conteo */
	TIM2 -> CR1 &= (0b1<<TIM_CR1_CEN_Pos); //TIM2 conteo EN

	return;
}


/*
 * @brief Timer2 CH3 GPIO Init
 *
 */

void timer2_ch3_gpio_config(void)
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


/*
 * @brief Timer2 CH3 Input Capture Config
 *
 */


void timer2_ch3_input_capture_config(void)
{

	/*Enlazar registro CCR a TI1*/
	TIM2 -> CCMR2 &= ~TIM_CCMR2_CC3S_Msk;
	TIM2 -> CCMR2 |= (0b01<<TIM_CCMR2_CC3S_Pos); //Enlazado al TI3
	/*Programar filtro*/
	TIM2 -> CCMR2 &= ~TIM_CCMR2_IC3F_Msk; //Limpiamos
	TIM2 -> CCMR2 |= (0b0011<<TIM_CCMR2_IC3F_Pos); //Sampling 8
	/*Flanco de detección*/
	TIM2 -> CCER  &= ~(TIM_CCER_CC3P_Msk | TIM_CCER_CC3NP_Msk); //rising edge
	/*Prescaler*/
	TIM2 -> CCMR2 &= ~TIM_CCMR2_IC3PSC_Msk; //No prescaler
	/*Deshabilitar interrupción*/
	TIM2 -> DIER &= ~TIM_DIER_CC1IE_Msk;

	return;
}


/*
 * @brief Timer2 CH3 Input Capture Start
 *
 */

void timer2_ch3_input_capture_start(void)
{
	/*Habilitar interrupción*/
	//TIM2 -> DIER |= (1<<TIM_DIER_CC1IE_Pos);
	/*Habilitar captura*/
	TIM2 -> CCER &= ~TIM_CCER_CC3E_Msk;
	TIM2 -> CCER |= (0b1<<TIM_CCER_CC3E_Pos); // TIM2_CH3 captura EN

	return;
}

void timer_input_calculation(void)
{

}

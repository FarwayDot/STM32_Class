/*
 * timer_capture.c
 *
 *  Created on: May 26, 2025
 *      Author: User
 */


#include "timer_capture.h"

void timer3_count_config(void)
{
	/*Activamos RCC del TIM3*/
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->CCER &= ~(TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E);
	TIM3->CR1 &= ~(1<<TIM_CR1_CEN_Pos); //Deshabilitamos conteo
	/*Calculo del prescaler*/
	TIM3->PSC = 16-1;
	/*Calculo del auto-reload*/
	TIM3->ARR = 50 -1; //Obtenemos 1ms o 1kHz
	/*Habilitamos interrupción*/
	TIM3->SR &= ~TIM_SR_UIF; //Apagamos bandera
	//TIM3->DIER &= ~TIM_DIER_UIE_Msk;
	//TIM3->DIER |= (1<<TIM_DIER_UIE_Pos);
	//NVIC_EnableIRQ(TIM3_IRQn);
	/*Activamos conteo*/
	TIM3->CR1 |= (1<<TIM_CR1_CEN_Pos);
	return;
}

void timer3_ch1_gpio_config(void)
{
	//PB4

	return;
}

/*
 * @brief Timer2 configuration as counter
 *
 */

void timer2_count_config(void)
{
	/*Activamos RCC del TIM2*/
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->CR1 &= ~TIM_CR1_CEN; //Deshabilitamos conteo
	/*Calculo del prescaler*/
	TIM2->PSC = 16-1;
	/*Calculo del auto-reload*/
	//TIM2->ARR = 10-1; //Obtenemos 100kHz
	return;
}


/*
 * @brief Timer2 start count
 *
 */

void timer2_count_start(void)
{
	/*Habilitar conteo */
	TIM2 -> CR1 |= (0b1<<TIM_CR1_CEN_Pos); //TIM2 conteo EN
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
	/*Mode AF*/
	GPIOA -> MODER &= ~GPIO_MODER_MODE2_0; //Modo AF
	GPIOA -> MODER |= GPIO_MODER_MODE2_1;
	/*VHS*/
	GPIOA -> OSPEEDR |= GPIO_OSPEEDR_OSPEED2; //Very High Speed
	/*Pull up down*/
	GPIOA -> PUPDR &= ~GPIO_PUPDR_PUPD2_Msk; //No pull up/down
	/*Alternative*/
	GPIOA -> AFR[0] &= ~GPIO_AFRL_AFRL2; 	//Limpiar
	GPIOA -> AFR[0] |= GPIO_AFRL_AFRL2_0;	//Configurar AF1
	return;
}


/*
 * @brief Timer2 CH3 Input Capture Config
 *
 */


void timer2_ch3_input_capture_config(void)
{
	/*Deshabilitar captura antes de configurar*/
	TIM2 -> CCER &= ~TIM_CCER_CC3E_Msk;
	/*Deshabilitar interrupción*/
	TIM2 -> DIER &= ~TIM_DIER_CC3IE;
	/*Enlazar registro CCR a TI3*/
	TIM2 -> CCMR2 &= ~TIM_CCMR2_CC3S_Msk;
	TIM2 -> CCMR2 |= (0b01<<TIM_CCMR2_CC3S_Pos); //Enlazado al TI3
	/*Programar filtro*/
	TIM2 -> CCMR2 &= ~TIM_CCMR2_IC3F_Msk; //Limpiamos
	TIM2 -> CCMR2 |= (0b0011<<TIM_CCMR2_IC3F_Pos); //Sampling 8
	/*Flanco de detección*/
	TIM2 -> CCER  &= ~(TIM_CCER_CC3P_Msk | TIM_CCER_CC3NP_Msk); //rising edge
	/*Prescaler*/
	TIM2 -> CCMR2 &= ~TIM_CCMR2_IC3PSC_Msk; //No prescaler
	return;
}


/*
 * @brief Timer2 CH3 Input Capture Start
 *
 */

void timer2_ch3_input_capture_start(void)
{
	/*Habilitar interrupción*/
	TIM2 -> SR &= ~TIM_SR_CC3IF_Msk;
	TIM2 -> DIER |= TIM_DIER_CC3IE;
	NVIC_EnableIRQ(TIM2_IRQn);
	/*Habilitar captura*/
	TIM2 -> CCER |= TIM_CCER_CC3E; // TIM2_CH3 captura EN
	return;
}


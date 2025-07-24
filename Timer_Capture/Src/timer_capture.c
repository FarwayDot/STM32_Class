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

	return;
}

void timer3_count_start(void)
{
	//Iniciamos conteo en 0
	TIM3 -> CNT = 0;

	//Activamos conte
	TIM3->CR1 |= TIM_CR1_CEN;

	return;
}

/*
 * @brief Timer2 configuration as counter, but it still wouldn't start
 *
 */

void timer2_count_config(uint16_t psc, uint32_t arr)
{
	//Activamos RCC del TIM2
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	//Deshabilitamos conteo
	TIM2->CR1 &= ~TIM_CR1_CEN;

	//Deshabilitamos interrupciónes
	TIM2->SR &= ~(TIM_SR_UIF || TIM_SR_CC1IF || TIM_SR_CC2IF || TIM_SR_CC3IF || TIM_SR_CC4IF);

	//Calculo del prescaler
	TIM2->PSC = psc;

	//Calculo del auto-reload
	TIM2->ARR = arr;

	return;
}


/*
 * @brief Timer2 start count
 *
 */

void timer2_count_start(void)
{
	//Iniciamos conteo en 0
	TIM2->CNT = 0;

	// Habilitamos conteo
	TIM2 -> CR1 |= TIM_CR1_CEN; //TIM2 conteo EN

	return;
}


/*
 * @brief Timer2 CH3 GPIO Init
 *
 */

void timer2_ch3_gpio_config(void)
{
	//Pin Config TIM2_CH3 PB10
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	//Mode AF
	GPIOB -> MODER &= ~GPIO_MODER_MODE10_Msk;
	GPIOB -> MODER |= GPIO_MODER_MODE10_1;

	//Pull up down
	GPIOB -> PUPDR &= ~GPIO_PUPDR_PUPD10_Msk;

	//Alternative
	GPIOB -> AFR[1] &= ~GPIO_AFRH_AFSEL10_Msk;
	GPIOB -> AFR[1] |= GPIO_AFRH_AFSEL10_0;

	return;
}


/*
 * @brief Timer2 CH3 Input Capture Config
 *
 */


void timer2_ch3_input_capture_config(uint16_t psc, uint32_t arr)
{
	//Deshabilitar captura antes de configurar
	TIM2 -> CCER &= ~TIM_CCER_CC3E_Msk;

	//Deshabilitar interrupción por captura
	TIM2 -> DIER &= ~TIM_DIER_CC3IE;

	//Configuramos pin TIM2_CH3 modo AF
	timer2_ch3_gpio_config();

	//Configuramos timer2
	timer2_count_config(psc, arr);

	//Programar filtro
	TIM2 -> CCMR2 &= ~TIM_CCMR2_IC3F_Msk;

	//Prescaler
	TIM2 -> CCMR2 &= ~TIM_CCMR2_IC3PSC_Msk;

	//Flanco de detección
	TIM2 -> CCER  &= ~TIM_CCER_CC3P_Msk;

	//Enlazar registro CCR a TI3, esto hace que el pin TI3 sea tipo entrada
	TIM2 -> CCMR2 &= ~TIM_CCMR2_CC3S_Msk;
	TIM2 -> CCMR2 |= 0b01 << TIM_CCMR2_CC3S_Pos;

	return;
}


/*
 * @brief Timer2 CH3 Input Capture Start with/without interrupt
 *
 */

void timer2_ch3_input_capture_start(uint8_t isr)
{
	//Apagamos bandera
	TIM2 -> SR &= ~TIM_SR_CC3IF_Msk;

	//Activamos interrupción
	if(isr == 1)
	{

		TIM2 -> DIER |= TIM_DIER_CC3IE;
		NVIC_EnableIRQ(TIM2_IRQn);
	}

	//Habilitar captura
	TIM2 -> CCER |= TIM_CCER_CC3E;

	//Comenzamos conteo
	timer2_count_start();

	return;
}

/*
 * @brief Timer2 CH3 Input Capture Start
 *
 */

void timer2_count_start_IT(void)
{
	//Apagamos bandera
	TIM2 -> SR &= ~TIM_SR_UIF;

	//Activamos interrupción
	TIM2 -> DIER |= TIM_DIER_UIE;
	NVIC_EnableIRQ(TIM2_IRQn);

	//Comenzamos conteo
	timer2_count_start();

	return;
}

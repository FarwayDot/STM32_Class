/*
 * Timer2.c
 *
 *  Created on: Aug 12, 2025
 *      Author: User
 */

#include "Timer2.h"

/*
 * @brief	Configurar PA0 TIM2
 */
void tim2_ch1_pa0_ic_gpio_config(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	GPIOA->MODER &= ~GPIO_MODER_MODER0_Msk;
	GPIOA->MODER |= GPIO_MODER_MODER0_1;

	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0_Msk;

	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED0_Msk;

	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL0_Msk;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL0_0;
}

/*
 * @brief	Configurar prescaler y autorreload de TIM2
 */
void tim2_count_config(uint16_t psc, uint32_t arr)
{
	//Activamos RCC del TIM2
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	//Deshabilitamos conteo
	TIM2->CR1 &= ~TIM_CR1_CEN;

	//Deshabilitamos banderas
	TIM2->CCER &= ~(TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E | TIM_CCER_CC4E);

	//Calculo del prescaler
	TIM2->PSC = psc; //21-1

	//Calculo del auto-reload
	TIM2->ARR = arr; //0xFFFFFFFF

	//Actualizamos valores
	TIM2->EGR |= TIM_EGR_UG;
}

/*
 * @brief Configurar Captura de Entrada PWM
 */

void tim2_PWM_InputCapture_Config(uint8_t interr)
{
	/*Deshabilitar captura e interrupciones*/

	TIM2->CR1 &= ~TIM_CR1_CEN_Msk;

	TIM2->CCER &= ~(TIM_CCER_CC1E_Msk | TIM_CCER_CC2E_Msk);

	TIM2->DIER &= ~(TIM_DIER_CC1IE | TIM_DIER_CC2IE | TIM_DIER_UIE);

	/*Configurar Entrada TIM2_CH1 PA0*/

	tim2_ch1_pa0_ic_gpio_config();

	/*Configurar Timer*/
	tim2_count_config( 20, 0xFFFFFFFF);

	/*Configurar Canal*/

	//Configuramos TI1FP1

	TIM2->CCMR1 &= ~TIM_CCMR1_CC1S_Msk;
	TIM2->CCMR1 |= (0b01<<TIM_CCMR1_CC1S_Pos);

	TIM2->CCER &= ~(TIM_CCER_CC1NP_Msk | TIM_CCER_CC1P_Msk); //Rising

	//Configuramos TI1FP2

	TIM2->CCMR1 &= ~TIM_CCMR1_CC2S_Msk;
	TIM2->CCMR1 |= (0b10<<TIM_CCMR1_CC2S_Pos);

	TIM2->CCER &= ~(TIM_CCER_CC2NP_Msk | TIM_CCER_CC2P_Msk);
	TIM2->CCER |= TIM_CCER_CC2P; //Falling

	//Configuramos trigger

	TIM2->SMCR &= ~TIM_SMCR_TS_Msk;
	TIM2->SMCR |= (TIM_SMCR_TS_0 | TIM_SMCR_TS_2);//Trigger con TI1FP1

	//Configuramos modo slave

	TIM2->SMCR &= ~TIM_SMCR_SMS_Msk;
	TIM2->SMCR |= TIM_SMCR_SMS_2;//Reset mode

	//Activamos interrupción
	if(interr != ISR_OFF)
	{
		//Activa interrupción en los dos canales CC
		TIM2 -> DIER |= TIM_DIER_CC1IE | TIM_DIER_CC2IE;
		NVIC_EnableIRQ(TIM2_IRQn);
	}

	return;
}

/*
 * @brief	Empezar Conteo Timer2
 */
void tim2_count_start(void)
{
	//Contador en 0
	TIM2 -> CNT = 0;

	//Habilitar conteo
	TIM2 -> CR1 |= TIM_CR1_CEN;
}

/*
 * @brief	Empezar Captura de Entrada PWM
 */
void tim2_PWM_InputCapture_Start(void)
{
	//Habilitar captura en las dos entradas CC
	TIM2 -> CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E;

	//Empezar conteo
	tim2_count_start();
}

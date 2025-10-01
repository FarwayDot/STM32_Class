/*
 * Timer3.c
 *
 *  Created on: Aug 12, 2025
 *      Author: User
 */

#include "Timer3.h"

/*
 * @brief
 */
void tim3_ch2_pb5_PWM_gpio_config(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	GPIOB->MODER &= ~GPIO_MODER_MODER5_Msk;
	GPIOB->MODER |= GPIO_MODER_MODER5_1;

	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD5_Msk;

	GPIOB->OSPEEDR |= GPIO_OSPEEDR_OSPEED5_Msk;

	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL5_Msk;
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL5_1;
}

/*
 * @brief
 */
void tim3_count_config(uint16_t psc, uint32_t arr)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

	TIM3->PSC = psc; //1us ticks 84-1

	TIM3->ARR = arr; //Periodo 10kHz 100-1

	TIM3->EGR |= TIM_EGR_UG; //Update Generation
}

/*
 * @brief
 */
void tim3_count_start(void)
{
	//Contador en 0
	TIM3 -> CNT = 0;

	//Habilitar conteo
	TIM3 -> CR1 |= TIM_CR1_CEN;
}

/*
 * @brief PWM Configuration
 *
 * PWM_tim3_t *pwm_tim: Structure with all parameters used in PWM Config:
 * 						PSC
 * 						ARR
 * 						ISR
 * 						Mode 1 and 2
 * 						Up and Down
 * 						Edge or Center
 * 						Polarity
 */
void tim3_PWM_Config(PWM_tim3_t *pwm_tim)
{
	/*Deshabilitar captura e interrupciones*/

	TIM3->CR1 &= ~TIM_CR1_CEN_Msk;

	TIM3->CCER &= ~TIM_CCER_CC2E_Msk;

	TIM3->DIER &= ~TIM_DIER_CC2IE_Msk;

	/*tim3 ch2 pb5*/

	tim3_ch2_pb5_PWM_gpio_config();

	/*Configurar Timer para PWM*/

	tim3_count_config(pwm_tim->psc, pwm_tim->arr);

	/*Configurar PWM*/
	TIM3->CCMR1 &= ~TIM_CCMR1_CC2S_Msk; //CC2 Output

	//Mode of PWM
	TIM3->CCMR1 &= ~TIM_CCMR1_OC2M_Msk;
	switch(pwm_tim->output_mode)
	{
		case PWM_Output_1:
			TIM3->CCMR1 |= (0b110<<TIM_CCMR1_OC2M_Pos); //PWM Mode 1: Active while TIMx_CNT < TIMx_CCRx
			break;
		case PWM_Output_2:
			TIM3->CCMR1 |= (0b111<<TIM_CCMR1_OC2M_Pos); //PWM Mode 2: Active while TIMx_CNT > TIMx_CCRx
			break;
	}

	//Type of counting
	switch(pwm_tim->counting_mode)
	{
		case Upcounting:
			TIM3->CCR1 &= ~ TIM_CR1_DIR; //Upcounting
			break;
		case Downcounting:
			TIM3->CCR1 |=  TIM_CR1_DIR; //Downcounting
			break;
	}

	//Polarity
	TIM3->CCER &= ~TIM_CCER_CC2NP_Msk;
	switch(pwm_tim->polarity_mode)
	{
		case Active_High:
			TIM3->CCER &= ~TIM_CCER_CC2P; //Active in High
			break;
		case Active_Low:
			TIM3->CCER |= TIM_CCER_CC2P; //Active in Low
			break;
	}

	TIM3->CCMR1 |= TIM_CCMR1_OC2PE; //Active preloaded CCR para evitar problemas
	TIM3->CR1 |= TIM_CR1_ARPE; //Active preloaded ARR para evitar problemas

	//PWM Aligned Mode
	TIM3->CR1 &= ~TIM_CR1_CMS_Msk; //Mask
	switch(pwm_tim->aligned_mode)
	{
		case Edge_Aligned:
			TIM3->CR1 &= ~TIM_CR1_CMS;
			break;
		case Center_Aligned_1:
			TIM3->CR1 |= TIM_CR1_CMS_0; //Set when it counts down
			break;
		case Center_Aligned_2:
			TIM3->CR1 |= TIM_CR1_CMS_1; //Set when it counts up
			break;
		case Center_Aligned_3:
			TIM3->CR1 |= TIM_CR1_CMS_1 | TIM_CR1_CMS_0; //Set when it counts up/down
			break;
	}

	//ISR
	switch(pwm_tim->isr)
	{
		case PWM_ISR_ON:
			TIM3->DIER |= TIM_DIER_CC2IE;
			NVIC_EnableIRQ(TIM3_IRQn);
			break;
		case PWM_ISR_OFF:
			TIM3->DIER &= ~TIM_DIER_CC2IE;
			break;
	}

	return;
}

/*
 * @brief PWM Start
 *
 * Start Count and Enable Output
 *
 * uint32_t num: Value putted into CCR to compare with CNT
 *
 */

void tim3_PWM_Start(uint32_t num)
{
	TIM3->CCR2 &= ~TIM_CCR2_CCR2_Msk; //Registro a comparar
	TIM3->CCR2 = num-1; // Valor de comparación inicial

	TIM3->CCER |= TIM_CCER_CC2E; //Habilitamos Output Compare

	tim3_count_start();
}

/*
 * @brief PWM Set in Edge-Aligned
 *
 * uint32_t duty: Duty cycle in percentage
 */
void tim3_PWM_Set(uint32_t duty)
{
	uint32_t period = TIM3->ARR + 1;

	if(duty>=100)
	{
		TIM3->CCR2 = period;
	}
	else
	{
		TIM3->CCR2 = (uint32_t)(((duty)/100.0f)*period);
	}
}

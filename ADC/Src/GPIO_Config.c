/*
 * EXTI_Config.c
 *
 *  Created on: Jan 7, 2025
 *  Author: jeanf
 */


#include <GPIO_Config.h>

/*
 * @brief GPIO Input Configuration
 *
 * This function sets up pin as an input. Moreover, configure pin as EXTI only if it's needed.
 * Configuration involves clock, moder register, pupdr register and EXTI.
 *
 * Input: GPIOx, Pin of GPIOx, Pull Up/Down, EXTI structure
 *
 * Examples:
 *
 * Global: EXTI structure: EXTI_Config_t exti13 = {.en = 1, .exti_pin = 13, .trigger = FALLING_IT, .priority = 8};
 * In main(): GPIO_Input_Config(GPIOB, 13, PULL_NONE , &exti13);
 *
 */

void GPIO_Input_Config(GPIO_TypeDef  *GPIOx, uint8_t Pin, Input_Trigger_t Pull, EXTI_Config_t *exti_t)
{
	//Clock
	ENABLE_GPIO_CLOCK(GPIOx);

	//Mode
	GPIOx->MODER &= ~(MODER_MASK<<(2*Pin));
	GPIOx->MODER |= (MODER_INPUT<<(2*Pin));

	//PuLL
	GPIOx->PUPDR &= ~(PUPDR_MASK<<(2*Pin));

	switch(Pull)
	{
		case PULL_NONE:
			//nothing
			break;
		case PULL_PU:
			GPIOx->PUPDR &= ~(PUPDR_PU<<(2*Pin));
			break;
		case PULL_PD:
			GPIOx->PUPDR &= ~(PUPDR_PD<<(2*Pin));
			break;
		default: break;
	}

	//EXTI configuration

	if (exti_t->en)
	{
		//Exti_Config
		exti_t->exti_pin = Pin;
		EXTI_Config(GPIOx , exti_t);
	}
	else
	{
		//Nothing
	}

}

/*
 * @brief GPIO Read
 *
 * This function read the status of an input pin
 * Input: GPIOx, Pin of GPIOx, Pin
 *
 * Examples:
 *
 * status = GPIO_ReadPin(GPIOA, 13);
 */

uint8_t GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	uint8_t bitstatus = GPIO_PIN_RESET;

	/* Check the parameters */
	uint8_t param = CHECK_INPUT(GPIOx, GPIO_Pin);
	//uint8_t param = GPIOx->MODER & (MODER_MASK<<(2*GPIO_Pin));

	if(param)
	{
		if((GPIOx->IDR & (1<<GPIO_Pin)) != GPIO_PIN_RESET)
		{
		bitstatus = GPIO_PIN_SET;
		}
		else
		{
		bitstatus = GPIO_PIN_RESET;
		}

	}
	else
	{
		//Nothing
	}

	return bitstatus;
}

/*
 * @brief GPIO Output Configuration
 *
 * This function sets up pin as an output.
 * Configuration involves clock, moder register, pupdr register, ospeedr register, otyper register.
 *
 * Input: GPIOx, Pin of GPIOx, Pull Up/Down, Speed of Pin, PushPull / OpenDrain Output
 *
 * Examples:
 *
 * In main(): GPIO_Output_Config(GPIOA, 8, PUPDR_NONE, OSPEEDR_VERYHIGH, OTYPER_PP);
 */

void GPIO_Output_Config(GPIO_TypeDef  *GPIOx, uint8_t Pin, uint8_t Pull, uint8_t Speed, uint8_t output_type)
{
	//Clock
	ENABLE_GPIO_CLOCK(GPIOx);

	//Mode
	GPIOx->MODER &= ~(MODER_MASK<<(2*Pin));
	GPIOx->MODER |= (MODER_OUTPUT<<(2*Pin));

	//Pull
	GPIOx->PUPDR &= ~(PUPDR_MASK<<(2*Pin));
	GPIOx->PUPDR |= (Pull<<(2*Pin));

	//Speed
	GPIOx->OSPEEDR &= ~(OSPEEDR_MASK<<(2*Pin));
	GPIOx->OSPEEDR |= (Speed<<(2*Pin));

	//Type of output
	GPIOx->OTYPER &= ~(OTYPER_MASK<<Pin);
	GPIOx->OTYPER |= (output_type<<Pin);
}

/*
 * @brief GPIO Write
 *
 * This function write in the output of a PORT
 * Input: GPIOx, Pin of GPIOx
 *
 * Examples:
 *
 * GPIO_Write(GPIOA, 6, HIGH);
 */

void GPIO_Write(GPIO_TypeDef* GPIOx, uint8_t GPIO_Pin, uint8_t Level)
{
	uint8_t param = CHECK_OUTPUT(GPIOx, GPIO_Pin);

	if(param)
	{
		if(Level != GPIO_PIN_RESET)
		{
			GPIOx->BSRR |= (1<<GPIO_Pin); //SET
		}
		else
		{
			GPIOx->BSRR |= (1 << (GPIO_Pin + 16U)); //RESET
		}
	}
	else
	{
		//Nothing
	}

}

/*
 * @brief GPIO Write Toggle
 *
 * This function toggles the status output of a pin in a PORT
 *
 * Examples:
 *
 * GPIO_Write_Toggle(GPIOA, 6);
 */

void GPIO_Write_Toggle(GPIO_TypeDef* GPIOx, uint8_t GPIO_Pin)
{
	uint8_t param = CHECK_OUTPUT(GPIOx, GPIO_Pin);
	uint8_t status_read = ((GPIOx->IDR) & (1<<GPIO_Pin));

	if(param)
	{
		if(status_read != GPIO_PIN_RESET)
		{
			GPIOx->BSRR |= (1 << (GPIO_Pin + 16U)); //RESET

		}
		else
		{
			GPIOx->BSRR |= (1<<GPIO_Pin); //SET
		}
	}
	else
	{
		//Nothing
	}
}

/*
 *  @brief EXTI Configuration
 *
 *  This function sets up EXTI pin configuration, this involves clock of EXTI,
 *  interrupt mask register, exti control register, trigger and NVIC configuration.
 *
 *  Input: EXTI_Config_t (Dirección of exti structure configuration)
 *
 *	WARNING: It's needed to redefine EXTIx_IRQHandler in "stm32fxx_it.c"
 *
 *	EXTI_Config is in GPIO_Input_Config();
 */

void EXTI_Config(GPIO_TypeDef *GPIOx ,EXTI_Config_t *exti_t)
{
	uint8_t exti_index_r = (uint8_t)(exti_t->exti_pin/4.0);
	uint8_t exti_index_p = (uint8_t)((exti_t->exti_pin%4)*4);

	//Enable SYSCFG
	RCC -> APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	/*
	 * Link GPIO with EXTI
	 * EXTICR[0] 0	1 	2 	3
	 * EXTICR[1] 4 	5 	6 	7
	 * EXTICR[2] 8 	9 	10 	11
	 * EXTICR[3] 12 13 	14 	15
	 *
	 */

	//Enable EXTI
	EXTI->IMR |= (1<<exti_t->exti_pin);
	SYSCFG -> EXTICR[exti_index_r] &= ~(EXTI_PMASK<<(2*exti_t->exti_pin));

	//Conect Pin to Exti Pin

	if (GPIOx == GPIOA)
	{
		SYSCFG -> EXTICR[exti_index_r] |= (EXTI_PA<<exti_index_p);
	}
	else if(GPIOx == GPIOB)
	{
		SYSCFG -> EXTICR[exti_index_r] |= (EXTI_PB<<exti_index_p);
	}
	else if(GPIOx == GPIOC)
	{
		SYSCFG -> EXTICR[exti_index_r] |= (EXTI_PC<<exti_index_p);
	}
	else if(GPIOx == GPIOD)
	{
		SYSCFG -> EXTICR[exti_index_r] |= (EXTI_PD<<exti_index_p);
	}
	else if(GPIOx == GPIOE)
	{
		SYSCFG -> EXTICR[exti_index_r] |= (EXTI_PE<<exti_index_p);
	}
	else if(GPIOx == GPIOH)
	{
		SYSCFG -> EXTICR[exti_index_r] |= (EXTI_PH<<exti_index_p);
	}

	//Configure Trigger
	switch(exti_t->trigger)
	{
	case FALLING_IT:
		EXTI -> FTSR |= (1<<exti_t->exti_pin);
		break;
	case RISING_IT:
		EXTI -> FTSR |= (1<<exti_t->exti_pin);
		break;
	case FALLING_RISING_IT:
		EXTI -> FTSR |= (1<<exti_t->exti_pin);
		EXTI -> RTSR |= (1<<exti_t->exti_pin);
		break;
	default: break;
	}

	//NVIC
	if(exti_t->exti_pin == 0)
	{
		NVIC_SetPriority(EXTI0_IRQn, exti_t->priority);
		NVIC_EnableIRQ(EXTI0_IRQn);
		EXTI -> PR |= (1<<exti_t->exti_pin);
		NVIC_ClearPendingIRQ(EXTI0_IRQn);
	}
	else if(exti_t->exti_pin == 1)
	{
		NVIC_SetPriority(EXTI1_IRQn, exti_t->priority);
		NVIC_EnableIRQ(EXTI1_IRQn);
		EXTI -> PR |= (1<<exti_t->exti_pin);
		NVIC_ClearPendingIRQ(EXTI1_IRQn);
	}
	else if(exti_t->exti_pin == 2)
	{
		NVIC_SetPriority(EXTI2_IRQn, exti_t->priority);
		NVIC_EnableIRQ(EXTI2_IRQn);
		EXTI -> PR |= (1<<exti_t->exti_pin);
		NVIC_ClearPendingIRQ(EXTI2_IRQn);
	}
	else if(exti_t->exti_pin == 3)
	{
		NVIC_SetPriority(EXTI3_IRQn, exti_t->priority);
		NVIC_EnableIRQ(EXTI3_IRQn);
		EXTI -> PR |= (1<<exti_t->exti_pin);
		NVIC_ClearPendingIRQ(EXTI3_IRQn);
	}
	else if(exti_t->exti_pin == 4)
	{
		NVIC_SetPriority(EXTI4_IRQn,exti_t->priority);
		NVIC_EnableIRQ(EXTI4_IRQn);
		EXTI -> PR |= (1<<exti_t->exti_pin);
		NVIC_ClearPendingIRQ(EXTI4_IRQn);
	}
	else if((exti_t->exti_pin>=5) && (exti_t->exti_pin<=9))
	{
		NVIC_SetPriority(EXTI9_5_IRQn, exti_t->priority);
		NVIC_EnableIRQ(EXTI9_5_IRQn);
		EXTI -> PR |= (1<<exti_t->exti_pin);
		NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
	}
	else if((exti_t->exti_pin>=10) && (exti_t->exti_pin<=15))
	{
		NVIC_SetPriority(EXTI15_10_IRQn, exti_t->priority);
		NVIC_EnableIRQ(EXTI15_10_IRQn);
		EXTI -> PR |= (1<<exti_t->exti_pin);
		NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
	}

}

void GPIO_Analog_Config(GPIO_TypeDef *GPIOx, uint8_t Pin, ADC1_Sample_Ch_e sample_ch)
{
	//Enable Clock
	ENABLE_GPIO_CLOCK(GPIOx);

	//Habilitamos ADC para configurar
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

	//Analog Mode
	GPIOx->MODER &= ~(MODER_MASK<<(2*Pin));
	GPIOx->MODER |= (MODER_ANALOG<<(2*Pin));

	//No pull-down
	GPIOx->PUPDR &= ~(PUPDR_MASK<<(2*Pin));

	//Ch Sampling Time
	if(Pin>=0 && Pin<=9)
	{
		ADC1->SMPR2 &= ~(ADC1_SMPR_Msk<<(3*Pin));
		ADC1->SMPR2 |= (sample_ch<<(3*Pin));
	}
	else if(Pin>=10 && Pin<=18)
	{
		ADC1->SMPR1 &= ~(ADC1_SMPR_Msk<<(3*Pin));
		ADC1->SMPR1 |= (sample_ch<<(3*Pin));
	}
	else
	{
		//Nothing
	}

}




/*
 * EXTI_Config.h
 *
 *  Created on: Jan 7, 2025
 *  Author: jeanf
 */

#ifndef GPIO_CONFIG_H_
#define GPIO_CONFIG_H_

#include "stm32f4xx.h"
#include <stdint.h>

/* Definiciones */


/* Variables globlaes, enumeraciones, listas */

/*	GPIO Defines	*/

/*
 * GPIO Read Status Pin
 */
#define GPIO_PIN_SET	1
#define GPIO_PIN_RESET	0

//Function to check input and output configuration
#define CHECK_INPUT(a, b) 	(((a->MODER >> (2 * (b))) & MODER_MASK) == MODER_INPUT)
#define CHECK_OUTPUT(a, b) 	(((a->MODER >> (2 * (b))) & MODER_MASK) == MODER_OUTPUT)

/*
 * MODER Defines
 */
#define MODER_INPUT			0b00U
#define MODER_OUTPUT		0b01U
#define MODER_ALTERNATE		0b10U
#define MODER_ANALOG		0b11U
#define MODER_MASK			0b11U

/*
 * PUPDR Defines
 */
#define PUPDR_NONE		0b00U
#define PUPDR_PU		0b01U
#define PUPDR_PD		0b10U
#define PUPDR_MASK		0b11U

/*	EXTI Defines	*/

/*
 * EXTI PORT
 */
#define EXTI_PA			0b0000
#define EXTI_PB			0b0001
#define EXTI_PC			0b0010
#define EXTI_PD			0b0011
#define EXTI_PE			0b0100
#define	EXTI_PH			0b0111
#define EXTI_PMASK		0b1111

/* ADC Defines*/
#define ADC1_SMPR_Msk	0b111

typedef enum
{
	PULL_NONE = 0,
	PULL_PU,
	PULL_PD,
}Input_Trigger_t;

typedef enum
{
	FALLING_IT = 0,
	RISING_IT,
	FALLING_RISING_IT,
}EXTI_Trigger_t;

typedef struct
{
	uint8_t en;
	uint8_t exti_pin;
	uint8_t trigger;
	uint8_t priority;
}EXTI_Config_t;

typedef enum
{
	ADC1_Cycles_3= 0,
	ADC1_Cycles_15,
	ADC1_Cycles_28,
	ADC1_Cycles_56,
	ADC1_Cycles_84,
	ADC1_Cycles_112,
	ADC1_Cycles_144,
	ADC1_Cycles_480
}ADC1_Sample_Ch_e;


/*
 * OSPEEDR	Defines
 */
#define OSPEEDR_LOW			0b00U
#define OSPEEDR_MEDIUM		0b01U
#define OSPEEDR_HIGH		0b10U
#define OSPEEDR_VERYHIGH	0b11U
#define OSPEEDR_MASK		0b11U

/*
 * OTYPER Defines
 */
#define OTYPER_PP			0b00U
#define OTYPER_OD			0b01U
#define OTYPER_MASK			0b11U

/*
 * ODR Defines
 */
#define LOW					0U
#define HIGH				1U



/* Enable Clock */
#define ENABLE_GPIO_CLOCK(port) \
    do { \
        if (port == GPIOA) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; \
        else if (port == GPIOB) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; \
        else if (port == GPIOC) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; \
        else if (port == GPIOD) RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; \
        else if (port == GPIOE) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN; \
        else if (port == GPIOH) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN; \
    } while (0)


/* Prototipo de funciones */

void GPIO_Input_Config(GPIO_TypeDef  *GPIOx, uint8_t Pin, Input_Trigger_t Pull, EXTI_Config_t *exti_t);
uint8_t GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_Output_Config(GPIO_TypeDef  *GPIOx, uint8_t Pin, uint8_t Pull, uint8_t Speed, uint8_t output_type);
void GPIO_Write(GPIO_TypeDef* GPIOx, uint8_t GPIO_Pin, uint8_t Level);
void GPIO_Write_Toggle(GPIO_TypeDef* GPIOx, uint8_t GPIO_Pin);
void EXTI_Config(GPIO_TypeDef *GPIOx ,EXTI_Config_t *exti_t);
void GPIO_Analog_Config(GPIO_TypeDef *GPIOx, uint8_t Pin,  ADC1_Sample_Ch_e sample_ch);

#endif /* GPIO_CONFIG_H_ */

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"

/* Private includes ----------------------------------------------------------*/


/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/


/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
extern void GPIO_EXTI_Callback(uint8_t pin);
extern void adc_user_handler(void);
extern void DMA_TransmitCpltCallback(void);
extern void DMA_HalfTransmitCpltCallback(void);
/* Private user code ---------------------------------------------------------*/


/* External variables --------------------------------------------------------*/

extern uint32_t uwTick;

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  while (1)
  {
  }
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{

  while (1)
  {

  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  while (1)
  {

  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  while (1)
  {

  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  while (1)
  {

  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{

}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{

}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{

}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
	uwTick++;
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

void ADC_IRQHandler(void)
{
	adc_user_handler();
	return;
}

void DMA2_Stream4_IRQHandler()
{
	//Transfer Complete
	if(DMA2->HISR & (1<<DMA_HISR_TCIF4_Pos))
	{
		//Clear flag
		DMA2->HIFCR |= DMA_HIFCR_CTCIF4;
		//Evaluar
		DMA_TransmitCpltCallback();
	}
	//Half Complete
	if(DMA2->HISR & (1<<DMA_HISR_HTIF4_Pos))
	{
		//Clear flag
		DMA2->HIFCR |= DMA_HIFCR_CHTIF4;
		//Evaluar
		DMA_HalfTransmitCpltCallback();
	}
	//Error Complete
	if(DMA2->HISR & (1<<DMA_HISR_TEIF4_Pos))
	{
		//Clear flag
		DMA2->HIFCR |= DMA_HIFCR_CTEIF4;
		//Evaluar

	}
	//Direct Mode Error
	if(DMA2->HISR & (1<<DMA_HISR_DMEIF4))
	{
		//Clear flag
		DMA2->HIFCR |= DMA_HIFCR_CDMEIF4;
		//Evaluar

	}

	return;
}

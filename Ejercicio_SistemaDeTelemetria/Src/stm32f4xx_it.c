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
extern void DMA_TransmitCpltCallback(void);
extern void DMA_HalfTransmitCpltCallback(void);
extern void TIM2_Callback(void);
extern void USART2_Callback(void);
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

void USART2_IRQHandler()
{
	//Al detectar recepción
	if(USART2->SR & USART_SR_RXNE)
	{
		USART2_Callback();
	}

	return;
}

void TIM2_IRQHandler()
{
	if(TIM2->SR & TIM_SR_UIF)
	{
		//Limpiamos bandera
		TIM2->SR &= ~(1U<<TIM_SR_UIF_Pos);

		//Ejecutamos función externa
		TIM2_Callback();
	}

	return;
}

void DMA2_Stream0_IRQHandler()
{
	//Transfer Complete
	if(DMA2->LISR & (1<<DMA_LISR_TCIF0_Pos))
	{
		//Clear flag
		DMA2->LIFCR |= DMA_LIFCR_CTCIF0;
		//Evaluar
		DMA_TransmitCpltCallback();
	}
	//Half Complete
	if(DMA2->LISR & (1<<DMA_LISR_HTIF0_Pos))
	{
		//Clear flag
		DMA2->LIFCR |= DMA_LIFCR_CHTIF0;
		//Evaluar
		DMA_HalfTransmitCpltCallback();
	}
	//Error Complete
	if(DMA2->LISR & (1<<DMA_LISR_TEIF0_Pos))
	{
		//Clear flag
		DMA2->LIFCR |= DMA_LIFCR_CTEIF0;
		//Evaluar

	}
	//Direct Mode Error
	if(DMA2->LISR & (1<<DMA_LISR_DMEIF0))
	{
		//Clear flag
		DMA2->LIFCR |= DMA_LIFCR_CDMEIF0;
		//Evaluar

	}

	return;
}

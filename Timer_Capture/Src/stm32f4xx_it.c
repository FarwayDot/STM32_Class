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
#define TIM2_CH3_POLARITY	1
#define TIM2_CH3_IC3PSC		1

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

uint32_t CNT[2];
uint32_t Capture;
uint8_t Is_First_Capture = 0;

/* Private function prototypes -----------------------------------------------*/
/*extern void GPIO_Toggle(void);*/
void frequency_calculator(void);
float frequency = 0.0;

/* Private user code ---------------------------------------------------------*/


/* External variables --------------------------------------------------------*/

extern uint32_t uwTick;
extern uint32_t uwTick_tim4;

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

/*Private functions*/
/*
void TIM3_IRQHandler(void)
{
	uint32_t status = TIM3->SR;

	if (status & TIM_SR_UIF)
	{
		TIM3->SR = 0;
		GPIO_Toggle(); // Acción de 1 ms
	}
	return;
}*/


void TIM2_IRQHandler(void)
{
	if(TIM2->SR & TIM_SR_CC3IF)
	{
		TIM2->SR &= ~TIM_SR_CC3IF;
		frequency_calculator();
	}
}

void frequency_calculator(void)
{
	uint32_t TIM_CLK = SystemCoreClock;

	if(Is_First_Capture == 0) //Si es la primera captura
	{
		CNT[0] = TIM2 -> CCR3;
		Is_First_Capture = 1;
	}
	else //Si ya se capturó una vez
	{
		CNT[1] = TIM2 -> CCR3;

		if(CNT[1]>=CNT[0])
		{
			Capture = CNT[1] - CNT[0];
		}
		else
		{
			Capture = TIM2->ARR - CNT[0] + CNT[1]  ;
		}

		frequency = (float)(TIM_CLK/((TIM2->PSC + 1)*TIM2_CH3_POLARITY)/Capture) * TIM2_CH3_IC3PSC;
		//freq = (float)(TIM_CLK/((TIM2->PSC + 1))/Capture);
		TIM2 -> CNT = 0;
		Is_First_Capture = 0;
	}
}




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
#include "GPIO_Config.h"

/* Private typedef -----------------------------------------------------------*/


/* Private define ------------------------------------------------------------*/
#define TIM2_CH3_POLARITY	1 //Si se escoje solo un flanco o los dos
#define TIM2_CH3_IC3PSC		1 //Si se escoje un prescalesr adicional en TIMx -> IC3PSC
#define TIM_CLK				16000000U

//#define PRUEBA

/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

//uint32_t CNT[2];
uint32_t IC_Val1 = 0;
uint32_t IC_Val2 = 0;
uint32_t Difference = 0;
uint8_t Is_First_Captured = 0;
float frequency = 0.0;

/* Private function prototypes -----------------------------------------------*/

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

void TIM2_IRQHandler(void)
{
#ifdef PRUEBA
	if(TIM2->SR & TIM_SR_UIF)
	{
		TIM2->SR &= ~TIM_SR_UIF;
		GPIO_Write_Toggle(GPIOA, 5);
	}


#else
	if(TIM2->SR & TIM_SR_CC3IF)
	{
		TIM2->SR &= ~TIM_SR_CC3IF;

		if(Is_First_Captured == 0) //Si es la primera captura
		{
			GPIO_Write(GPIOA, 5, GPIO_PIN_SET);
			IC_Val1 = TIM2 -> CCR3;
			Is_First_Captured = 1;
		}
		else //Si ya se capturó una vez
		{
			GPIO_Write(GPIOA, 5, GPIO_PIN_RESET);
			IC_Val2 = TIM2 -> CCR3;

			if(IC_Val2 > IC_Val1)
			{
				Difference = IC_Val2 - IC_Val1;
			}
			else
			{
				Difference = (TIM2->ARR  - IC_Val1) + IC_Val2;
			}

			//frequency = (float)(TIM_CLK/((TIM2->PSC + 1)*TIM2_CH3_POLARITY)/Difference) * TIM2_CH3_IC3PSC;
			//frequency = 1000000.0f/Difference;
			frequency = (float)(TIM_CLK/((0 + 1)*TIM2_CH3_POLARITY)/Difference) * TIM2_CH3_IC3PSC;
			TIM2 -> CNT = 0;
			Is_First_Captured = 0;
		}
	}
#endif



}




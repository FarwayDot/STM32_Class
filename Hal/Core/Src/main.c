/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;

/* USER CODE BEGIN PV */


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM2_Init(void);
/* USER CODE BEGIN PFP */
#define TIMCLOCK 	16000000.0f
#define PRESCALER	16.0f
#define REFCLOCK 	(TIMCLOCK/PRESCALER)

uint32_t IC_Val1 = 0;
uint32_t IC_Val2 = 0;
uint32_t Difference = 0;
uint8_t Is_First_Captured= 0;
float frequency = 0.0;

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(htim -> Channel == HAL_TIM_ACTIVE_CHANNEL_3)
	{

		if(Is_First_Captured == 0)
		{
			HAL_GPIO_WritePin(LED_SYNCH_GPIO_Port, LED_SYNCH_Pin, GPIO_PIN_SET);
			IC_Val1 = TIM2 -> CCR3;//HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_3);
			Is_First_Captured = 1;
		}
		else
		{
			HAL_GPIO_WritePin(LED_SYNCH_GPIO_Port, LED_SYNCH_Pin, GPIO_PIN_RESET);
			IC_Val2 = TIM2 -> CCR3;

			if(IC_Val2 > IC_Val1)
			{
				Difference = IC_Val2 - IC_Val1;
			}
			else
			{
				Difference = (TIM2->ARR - IC_Val1) + IC_Val2;
			}

			frequency = (float)((TIMCLOCK/(TIM2->PSC + 1))/Difference);
			__HAL_TIM_SET_COUNTER(htim, 0);
			Is_First_Captured = 0;
		}
	}
}

void tim2_ch3_config_ia(void);

void timer2_count_config(void);
void timer2_ch3_input_capture_config(void);
void timer2_count_start(void);
void timer2_ch3_gpio_config(void);
void timer2_ch3_input_capture_start(void);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */
  TIM3 -> CR1 |= TIM_CR1_CEN;


  //timer2_ch3_gpio_config();
  /*timer2_count_config(); //Configuramos tim2 counter
  timer2_ch3_input_capture_config(); //Configuramos tim2 input capture
  timer2_count_start(); //Comenzamos la cuenta en tim2
  timer2_ch3_input_capture_start();*/
  //tim2_ch3_config_ia();
  HAL_TIM_IC_Start_IT(&htim2, TIM_CHANNEL_3);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	if(TIM3->SR & TIM_SR_UIF)
	{
		TIM3->SR &= ~TIM_SR_UIF;
		HAL_GPIO_TogglePin(LED_SIGN_GPIO_Port, LED_SIGN_Pin);
	}
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 16-1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim2, &sConfigIC, TIM_CHANNEL_3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 16-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 50-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */
  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LED_USER_Pin|LED_SYNCH_Pin|LED_SIGN_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LED_USER_Pin LED_SYNCH_Pin LED_SIGN_Pin */
  GPIO_InitStruct.Pin = LED_USER_Pin|LED_SYNCH_Pin|LED_SIGN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */
  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void tim2_ch3_config_ia(void)
{
    // 1. Habilitar reloj a TIM2
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // 2. Detener el contador antes de configurar
    TIM2->CR1 &= ~TIM_CR1_CEN;

    // 3. Configurar prescaler: 16 MHz / 16 = 1 MHz
    TIM2->PSC = 16 - 1;

    // 4. Auto-reload: máximo para contar periodos largos
    TIM2->ARR = 0xFFFFFFFF;

    // 5. Reiniciar contador
    TIM2->CNT = 0;

    // 6. Configurar canal 3 en modo Input Capture
    // CC3S = 01: TI3 input
    TIM2->CCMR2 &= ~TIM_CCMR2_CC3S_Msk;
    TIM2->CCMR2 |= (0b01 << TIM_CCMR2_CC3S_Pos);

    // 7. Prescaler de entrada: ninguno (IC3PSC = 00)
    TIM2->CCMR2 &= ~TIM_CCMR2_IC3PSC_Msk;

    // 8. Filtro digital: opcional, aquí desactivado (IC3F = 0000)
    TIM2->CCMR2 &= ~TIM_CCMR2_IC3F_Msk;

    // 9. Flanco de captura: ascendente (CC3P = 0, CC3NP = 0)
    TIM2->CCER &= ~(TIM_CCER_CC3P | TIM_CCER_CC3NP);

    // 10. Habilitar captura en el canal 3 (CC3E = 1)
    TIM2->CCER |= TIM_CCER_CC3E;

    // 11. Habilitar interrupción para canal 3
    TIM2->DIER |= TIM_DIER_CC3IE;

    // 12. Habilitar interrupción TIM2 en NVIC
    //NVIC_EnableIRQ(TIM2_IRQn);

    // 13. Habilitar el timer
    //TIM2->CR1 |= TIM_CR1_CEN;
}

void timer2_count_config(void)
{
	/*Activamos RCC del TIM2*/
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	TIM2->CR1 &= ~TIM_CR1_CEN; //Deshabilitamos conteo
	/*Calculo del prescaler*/
	TIM2->PSC = 16-1;
	/*Calculo del auto-reload*/
	TIM2->ARR = 0xFFFFFFFF;
	return;
}
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
	//TIM2 -> CCMR2 |= (0b0011<<TIM_CCMR2_IC3F_Pos); //Sampling 8
	/*Flanco de detección*/
	TIM2 -> CCER  &= ~(TIM_CCER_CC3P | TIM_CCER_CC3NP); //rising edge
	/*Prescaler*/
	TIM2 -> CCMR2 &= ~TIM_CCMR2_IC3PSC_Msk; //No prescaler
	return;
}
void timer2_count_start(void)
{
	TIM2->CNT = 0;
	/*Habilitar conteo */
	TIM2 -> CR1 |= TIM_CR1_CEN; //TIM2 conteo EN
	return;
}

void timer2_ch3_gpio_config(void)
{
	/*Pin Config TIM2_CH3 PB10*/
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOBEN; //RCC
	/*Mode AF*/
	GPIOB -> MODER &= ~GPIO_MODER_MODE10_Msk; //Modo AF
	GPIOB -> MODER |= GPIO_MODER_MODE10_1;
	/*Pull up down*/
	GPIOB -> PUPDR &= ~GPIO_PUPDR_PUPD10_Msk; //No pull up/down
	/*Alternative*/
	GPIOB -> AFR[1] &= ~GPIO_AFRH_AFSEL10_Msk; 	//Limpiar
	GPIOB -> AFR[1] |= GPIO_AFRH_AFSEL10_0;	//Configurar AF1
	return;
}

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
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//#define DHT_PIN_READ()  HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin)
#define DHT_PIN_READ()		((GPIOA->IDR & GPIO_IDR_8)>>8)
#define USE_SWV			1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */

typedef enum
{
	State_Start = 0,
	State_Integral_RH,
	State_Decimal_RH,
	State_Integral_Te,
	State_Decimal_T,
	State_Check
}states_e;


uint8_t Data[5] = {0};
float press = 0.0;
float temp = 0.0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_TIM1_Init(void);
/* USER CODE BEGIN PFP */
void PA8_Output_Config(void);
void PA8_Input_Config(void);

void user_delay_us(uint32_t delay);
uint8_t wait_for_low(uint32_t wait_time);
uint8_t wait_for_high(uint32_t wait_time);
uint32_t pulseIn(uint8_t Pin_State, uint32_t timeout);

uint8_t dht11_start(void);
uint8_t dht11_read(void);
void dht11_process(void);

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
  MX_USART1_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */

  setbuf(stdout, NULL);

  TIM1->CR1 |= TIM_CR1_CEN; //Empieza conteo
  //dht11_start();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  //dht11_read();
	  //while(!dht11_read());
	  //dht11_process();
	  printf("Hola Mundo\n");
	  HAL_Delay(1100);
	  //dht11_start();

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  RCC_OscInitStruct.PLL.PREDIV = RCC_PREDIV_DIV1;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART1;
  PeriphClkInit.Usart1ClockSelection = RCC_USART1CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 48-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 38400;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

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
  HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(USER_LED_GPIO_Port, USER_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : DHT11_Pin */
  GPIO_InitStruct.Pin = DHT11_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : USER_LED_Pin */
  GPIO_InitStruct.Pin = USER_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(USER_LED_GPIO_Port, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void PA8_Output_Config(void)
{
	//pa8 gpio output
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN; //Enable gpioa clock

  GPIOA->MODER &= ~GPIO_MODER_MODER8_Msk; //Clean function
  GPIOA->MODER |= GPIO_MODER_MODER8_0; //Output function

  GPIOA->OTYPER &= ~GPIO_OTYPER_OT_8; //Clean outpyt type

  GPIOA->OSPEEDR &= ~GPIO_OSPEEDR_OSPEEDR8_Msk; //Clean speed
  GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEEDR8_0; //Medium

  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR8_Msk; //Clean pull

}

void PA8_Input_Config(void)
{
	//pa8 gpio input
  RCC->AHBENR |= RCC_AHBENR_GPIOAEN; //Enable gpioa clock

  GPIOA->MODER &= ~GPIO_MODER_MODER8_Msk; //Clean function, input mode

  GPIOA->PUPDR &= ~GPIO_PUPDR_PUPDR8_Msk; // No pull

}

//Delay en us
void user_delay_us(uint32_t delay)
{
	uint32_t tickstart = TIM1->CNT;

	while(((uint32_t)(TIM1->CNT - tickstart))< delay);
	return;
}

//Esperamos en us
uint8_t wait_for_low(uint32_t wait_time) //Esperamos desde 1
{
	uint32_t tickstart =  TIM1->CNT;

	while(DHT_PIN_READ())
	{
		if((uint32_t)(TIM1->CNT - tickstart)>=wait_time)
		{
			return 0;
		}

	}
	return 1;
}

//Esperamos en us
uint8_t wait_for_high(uint32_t wait_time)
{
	uint32_t tickstart =  TIM1->CNT;

	while(!DHT_PIN_READ())
	{
		if((uint32_t)(TIM1->CNT - tickstart)>=wait_time)
		{
			return 0;
		}

	}
	return 1;

}

/*
 * @brief Función que mide el tiempo en el que cambia el estado del pin a Pin_State
 *
 * uint8_t Pin: Pin de entrada (Por ahora no se escoje el puerto)
 * uint8_t Pin_State: Estado del fin final.
 * uint16_t timeout: tiempo que se toma el pin en cambiar de estado a Pin_State
 *
 */
uint32_t pulseIn(uint8_t Pin_State, uint32_t timeout)
{
	uint32_t start_ticks = TIM1->CNT;

	/*Verificamos que el pin cambie de estado*/
	while(DHT_PIN_READ() != Pin_State) //Mientras no cambie de estado
	{
		if(TIM1->CNT - start_ticks > timeout) //Si se pasa del timeout, es error
		{
			return 0; //Error
		}
	}

	uint32_t pulse_ticks = TIM1->CNT; //Guardamos Ticks cuando cambia el pulso

	/*Verificamos que el pin no pase el timeout en el nuevo estado*/
	while(DHT_PIN_READ() == Pin_State)
	{
		if(TIM1->CNT - start_ticks > timeout) //Si pasa de timeout, es error
		{
			return 0;
		}
	}

	return (TIM1->CNT - pulse_ticks);
}

int __io_putchar(int ch){

	uint8_t c = ch & 0xFF;
	while(!(USART1->ISR & USART_ISR_TXE));  //espera hasta que usart este listo para transmitir otro byte
	USART1->TDR = c;
	return ch;
}

uint8_t dht11_start(void)
{
	PA8_Output_Config();
	GPIOA->ODR &= ~GPIO_ODR_8; //Write 0
	HAL_Delay(20);
	GPIOA->ODR |= GPIO_ODR_8; //Write 1
	user_delay_us(40);
	PA8_Input_Config();
	while(DHT_PIN_READ());

	if(pulseIn(GPIO_PIN_RESET, 100) == 0) return 0;
	if(pulseIn(GPIO_PIN_SET, 100) == 0) return 0;

	return 1;
}

uint8_t dht11_read(void)
{
	uint8_t group_data = 0;
	uint8_t bit_data = 0;
	//uint32_t start_tick = 0;

	/*
	 * Data[0] = int RH data
	 * Data[1] = dec RH data
	 * Data[2] = int TEMP data
	 * Data[3] = dec TEMP data
	 * Data[4] = checksum
	 */

	for(group_data = 0; group_data<=4; group_data++)
	{
		for(bit_data = 0; bit_data<=7; bit_data++)
		{
			while(!DHT_PIN_READ()); //Esperamos a que cambie de estado a 1V 50us



			if(wait_for_low(40) && (DHT_PIN_READ()==0)) //Empezamos la espera por nivel de voltaje 0V
			{
				//Bit = 0
				Data[group_data] &= ~(1<<(7-bit_data));
			}
			else if(wait_for_low(50) && (DHT_PIN_READ()==0)) //Ya pasaron 30us
			{
				//Bit = 1
				Data[group_data] |= (1<<(8-bit_data));
			}
		}
	}

	if(Data[4] == Data[0] + Data[1] + Data[2] + Data[3])
	{
		return 1; //Conversión completa
	}
	else
	{
		return 0; //Conversión fallida
	}

}

void dht11_process(void)
{
	press = (float)(Data[0]) + (Data[0]/10.0);
	temp = (float)(Data[0]) + (Data[0]/10.0);

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

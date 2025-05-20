/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define INT_AC1_Pin GPIO_PIN_13
#define INT_AC1_GPIO_Port GPIOC
#define INT_AC1_EXTI_IRQn EXTI15_10_IRQn
#define INT_AC2_Pin GPIO_PIN_14
#define INT_AC2_GPIO_Port GPIOC
#define INT_AC2_EXTI_IRQn EXTI15_10_IRQn
#define Shot_1_Pin GPIO_PIN_0
#define Shot_1_GPIO_Port GPIOA
#define Shot_2_Pin GPIO_PIN_1
#define Shot_2_GPIO_Port GPIOA
#define T_START_ADC_Current_Pin GPIO_PIN_3
#define T_START_ADC_Current_GPIO_Port GPIOA
#define ADC_Voltage_Pin GPIO_PIN_4
#define ADC_Voltage_GPIO_Port GPIOA
#define ADC_Tachometer_Pin GPIO_PIN_5
#define ADC_Tachometer_GPIO_Port GPIOA
#define ADC_Pot_Pin GPIO_PIN_6
#define ADC_Pot_GPIO_Port GPIOA
#define RELAY_Pin GPIO_PIN_12
#define RELAY_GPIO_Port GPIOB
#define T_DOWN_Pin GPIO_PIN_13
#define T_DOWN_GPIO_Port GPIOB
#define T_UP_Pin GPIO_PIN_14
#define T_UP_GPIO_Port GPIOB
#define T_START_Pin GPIO_PIN_15
#define T_START_GPIO_Port GPIOB
#define P_ENTER_Pin GPIO_PIN_9
#define P_ENTER_GPIO_Port GPIOA
#define P_UP_Pin GPIO_PIN_10
#define P_UP_GPIO_Port GPIOA
#define P_DOWN_Pin GPIO_PIN_11
#define P_DOWN_GPIO_Port GPIOA
#define P_MENU_Pin GPIO_PIN_12
#define P_MENU_GPIO_Port GPIOA
#define LED_Pin GPIO_PIN_15
#define LED_GPIO_Port GPIOA
#define LCD_D7_Pin GPIO_PIN_4
#define LCD_D7_GPIO_Port GPIOB
#define LCD_D6_Pin GPIO_PIN_5
#define LCD_D6_GPIO_Port GPIOB
#define LCD_D5_Pin GPIO_PIN_6
#define LCD_D5_GPIO_Port GPIOB
#define LCD_D4_Pin GPIO_PIN_7
#define LCD_D4_GPIO_Port GPIOB
#define LCD_E_Pin GPIO_PIN_8
#define LCD_E_GPIO_Port GPIOB
#define LCD_RS_Pin GPIO_PIN_9
#define LCD_RS_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

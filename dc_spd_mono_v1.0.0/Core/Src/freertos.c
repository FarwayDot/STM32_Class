/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "dc_spd_mono.h"
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
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId mainTaskHandle;
osThreadId digInputTaskHandle;
osThreadId controlTaskHandle;
osThreadId lcdTaskHandle;
osSemaphoreId dc_spd_adc_flagHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void dc_spd_main_task_entry(void const * argument);
void dc_spd_dig_read_task_entry(void const * argument);
void dc_spd_control_task_entry(void const * argument);
void dc_spd_lcd_task_entry(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
	dc_spd_events = xEventGroupCreate();
	configASSERT(dc_spd_events != NULL);

	dc_spd_button_events = xEventGroupCreate();
	configASSERT( dc_spd_button_events != NULL);
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of dc_spd_adc_flag */
  osSemaphoreDef(dc_spd_adc_flag);
  dc_spd_adc_flagHandle = osSemaphoreCreate(osSemaphore(dc_spd_adc_flag), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of mainTask */
  osThreadDef(mainTask, dc_spd_main_task_entry, osPriorityNormal, 0, 256);
  mainTaskHandle = osThreadCreate(osThread(mainTask), NULL);

  /* definition and creation of digInputTask */
  osThreadDef(digInputTask, dc_spd_dig_read_task_entry, osPriorityIdle, 0, 128);
  digInputTaskHandle = osThreadCreate(osThread(digInputTask), NULL);

  /* definition and creation of controlTask */
  osThreadDef(controlTask, dc_spd_control_task_entry, osPriorityIdle, 0, 128);
  controlTaskHandle = osThreadCreate(osThread(controlTask), NULL);

  /* definition and creation of lcdTask */
  osThreadDef(lcdTask, dc_spd_lcd_task_entry, osPriorityBelowNormal, 0, 256);
  lcdTaskHandle = osThreadCreate(osThread(lcdTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_dc_spd_main_task_entry */
/**
  * @brief  Function implementing the mainTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_dc_spd_main_task_entry */
__weak void dc_spd_main_task_entry(void const * argument)
{
  /* USER CODE BEGIN dc_spd_main_task_entry */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END dc_spd_main_task_entry */
}

/* USER CODE BEGIN Header_dc_spd_dig_read_task_entry */
/**
* @brief Function implementing the digInputTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_dc_spd_dig_read_task_entry */
__weak void dc_spd_dig_read_task_entry(void const * argument)
{
  /* USER CODE BEGIN dc_spd_dig_read_task_entry */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END dc_spd_dig_read_task_entry */
}

/* USER CODE BEGIN Header_dc_spd_control_task_entry */
/**
* @brief Function implementing the controlTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_dc_spd_control_task_entry */
__weak void dc_spd_control_task_entry(void const * argument)
{
  /* USER CODE BEGIN dc_spd_control_task_entry */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END dc_spd_control_task_entry */
}

/* USER CODE BEGIN Header_dc_spd_lcd_task_entry */
/**
* @brief Function implementing the lcdTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_dc_spd_lcd_task_entry */
__weak void dc_spd_lcd_task_entry(void const * argument)
{
  /* USER CODE BEGIN dc_spd_lcd_task_entry */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END dc_spd_lcd_task_entry */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */


/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "freertos_config_defines.h"
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
app_button_t button = {	.port = B1_GPIO_Port,
						.pin = B1_Pin,
						.state = BUTTON_FREE};

/* USER CODE END Variables */
/* Definitions for task01 */
osThreadId_t task01Handle;
const osThreadAttr_t task01_attributes = {
  .name = "task01",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for task02 */
osThreadId_t task02Handle;
const osThreadAttr_t task02_attributes = {
  .name = "task02",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for task03 */
osThreadId_t task03Handle;
const osThreadAttr_t task03_attributes = {
  .name = "task03",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Event01 */
osEventFlagsId_t Event01Handle;
const osEventFlagsAttr_t Event01_attributes = {
  .name = "Event01"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Start_Task01(void *argument);
void Start_Task02(void *argument);
void button_task_read_entry(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

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
  /* creation of task01 */
  task01Handle = osThreadNew(Start_Task01, NULL, &task01_attributes);

  /* creation of task02 */
  task02Handle = osThreadNew(Start_Task02, NULL, &task02_attributes);

  /* creation of task03 */
  task03Handle = osThreadNew(button_task_read_entry, (void*)&button, &task03_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Create the event(s) */
  /* creation of Event01 */
  Event01Handle = osEventFlagsNew(&Event01_attributes);

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_Start_Task01 */
/**
  * @brief  Function implementing the task01 thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Start_Task01 */
__weak void Start_Task01(void *argument)
{
  /* USER CODE BEGIN Start_Task01 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Start_Task01 */
}

/* USER CODE BEGIN Header_Start_Task02 */
/**
* @brief Function implementing the task02 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Start_Task02 */
__weak void Start_Task02(void *argument)
{
  /* USER CODE BEGIN Start_Task02 */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Start_Task02 */
}

/* USER CODE BEGIN Header_button_task_read_entry */
/**
* @brief Function implementing the task03 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_button_task_read_entry */
__weak void button_task_read_entry(void *argument)
{
  /* USER CODE BEGIN button_task_read_entry */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END button_task_read_entry */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */


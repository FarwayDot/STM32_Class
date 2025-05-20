/*
 * task_normal_functions.c
 *
 *  Created on: Mar 4, 2025
 *      Author: User
 */

#include "freertos_config_defines.h"


void Start_Task01(void *argument)
{
	osEventFlagsSet(Event01Handle, SYS_READY);

	for(;;)
	{
		if(osEventFlagsGet(Event01Handle) & SYS_TASK_A)
		{
			HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
			osDelay(250);
		}

	}
}

void Start_Task02(void *argument)
{
	osEventFlagsSet(Event01Handle, SYS_READY);

	for(;;)
	{
		if(osEventFlagsGet(Event01Handle) & SYS_TASK_B)
		{
			HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
			osDelay(1000);
		}
	}
}

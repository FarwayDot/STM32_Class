/*
 * button_task_entry.c
 *
 *  Created on: Mar 3, 2025
 *      Author: User
 */

#include "freertos_config_defines.h"

void button_task_read_entry(void *params)
{
	app_button_t *obj;
	obj = (app_button_t *)params;

	//uint8_t cont = 0;
	uint8_t state;
	uint8_t pin_read = 0;
	uint32_t ticks = 0;

	osEventFlagsWait(Event01Handle, SYS_READY, osFlagsWaitAny, osWaitForever);
	osEventFlagsSet(Event01Handle,SYS_TASK_A);

	for(;;)
	{
		pin_read = HAL_GPIO_ReadPin(obj->port, obj->pin);
		state = obj->state;
		switch(state)
		{
			case BUTTON_FREE:
				/*Action*/

				/*Si detecta que se presiona (En 0)*/
				if(pin_read == GPIO_PIN_RESET)
				{
					vTaskDelay(pdMS_TO_TICKS(30));
					obj->state = BUTTON_DEBOUNCE;
				}
				break;

			case BUTTON_DEBOUNCE:
				/*Action*/

				/*Si se mantiene presionado*/
				if(pin_read == GPIO_PIN_RESET)
				{
					ticks = xTaskGetTickCount();
					obj->state = BUTTON_ONPRESS;
				}
				else /*Si se suelta rápidamente*/
				{
					//Event
					obj->state = BUTTON_ONRELEASE;
				}
				break;

			case BUTTON_ONPRESS:
				/*Action*/

				/*Confirmando que sigue presionado*/
				if(pin_read == GPIO_PIN_RESET)
				{
					/*Esperamos un tiempo*/
					if((xTaskGetTickCount() - ticks) >= pdMS_TO_TICKS(2000))
					{
						obj->state = BUTTON_KEEPALIVE;
					}

				}
				else /*Si se soltó el botón*/
				{
					obj->state = BUTTON_ONRELEASE;
				}
				break;

			case BUTTON_KEEPALIVE:
				/*Action*/

				/*Si soltamos el botón*/
				if(pin_read == GPIO_PIN_SET)
				{
					/*Cambiamos a on_release*/
					obj->state = BUTTON_ONRELEASE;
				}
				break;

			case BUTTON_ONRELEASE:
				/*Action*/
				if(!(osEventFlagsGet(Event01Handle)& SYS_TASK_A))
				{
					osEventFlagsClear(Event01Handle, SYS_TASK_B);
					osEventFlagsSet(Event01Handle, SYS_TASK_A);
				}
				else
				{
					osEventFlagsClear(Event01Handle, SYS_TASK_A);
					osEventFlagsSet(Event01Handle, SYS_TASK_B);
				}
				/*Volvemos a FREE*/
				obj->state = BUTTON_FREE;
				break;
			default:
				break;
		}
		vTaskDelay(pdMS_TO_TICKS(20));
	}
}





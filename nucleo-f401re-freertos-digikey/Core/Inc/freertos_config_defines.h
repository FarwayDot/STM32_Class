/*
 * freertos_config_defines.h
 *
 *  Created on: Mar 6, 2025
 *      Author: User
 */

#ifndef INC_FREERTOS_CONFIG_DEFINES_H_
#define INC_FREERTOS_CONFIG_DEFINES_H_


/*User libraries*/
#include "main.h"
#include "cmsis_os.h"
#include <stdio.h>
#include <stdint.h>

/*Defines*/

/*Structurs and enums*/
typedef enum
{
	SYS_READY 	= 1<<0,
	SYS_ERROR 	= 1<<1,
	SYS_TASK_A 	= 1<<2,
	SYS_TASK_B 	= 1<<3,
	SYS_TASK_BUTTON = 1<<4
}event_flags_e;

typedef enum{
    BUTTON_FREE = 0,
    BUTTON_DEBOUNCE,
    BUTTON_ONPRESS,
    BUTTON_KEEPALIVE,
    BUTTON_ONRELEASE,
}app_button_state_n;

typedef struct  app_button
{
	GPIO_TypeDef *port;
    uint16_t pin;
    uint8_t state;
    //EventGroupHandle_t event;
}app_button_t;

/*Global Variables*/
extern osEventFlagsId_t Event01Handle;
/*Prototype functiones*/

#endif /* INC_FREERTOS_CONFIG_DEFINES_H_ */

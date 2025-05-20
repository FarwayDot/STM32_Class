/*
 * dc_spd_dig_read_task_entry.c
 *
 *  Created on: Dec 8, 2024
 *      Author: USER
 */


/*Includes ----------------------------------------------------------------------------------------------------------------*/
#include "dc_spd_mono.h"
#include "lwbtn.h"
/*Defines -----------------------------------------------------------------------------------------------------------------*/

#ifndef DC_SPD_LONG_MIN_PRESS
#define DC_SPD_LONG_MIN_PRESS	500
#endif
/*Typedefs ----------------------------------------------------------------------------------------------------------------*/


/*Global variables --------------------------------------------------------------------------------------------------------*/
static lwbtn_btn_t dc_spd_pannel_btns[] = {
		{.arg = (void *)DC_SPD_PANEL_UP },
		{.arg = (void *)DC_SPD_PANEL_DOWN },
		{.arg = (void *)DC_SPD_PANEL_RUN },
};

static lwbtn_btn_t dc_spd_key_btns[] = {
		{.arg = (void *)DC_SPD_KEY_UP},
		{.arg = (void *)DC_SPD_KEY_DOWN},
		{.arg = (void *)DC_SPD_KEY_ENTER},
		{.arg = (void *)DC_SPD_KEY_MENU},
};
lwbtn_t dc_spd_ext;
lwbtn_t dc_spd_pannel;

EventGroupHandle_t dc_spd_button_events;
/*Function prototype ------------------------------------------------------------------------------------------------------*/
uint8_t dc_spd_panel_btn_get_state(struct lwbtn* lw, struct lwbtn_btn* btn);
uint8_t dc_spd_key_btn_get_state(struct lwbtn* lw, struct lwbtn_btn* btn);
void dc_spd_panel_btn_event(struct lwbtn* lw, struct lwbtn_btn* btn, lwbtn_evt_t evt);
void dc_spd_key_btn_event(struct lwbtn* lw, struct lwbtn_btn* btn, lwbtn_evt_t evt);



/*Task definition ---------------------------------------------------------------------------------------------------------*/
void dc_spd_dig_read_task_entry(void const * argument)
{
	/*Local variables -----------------------------------------------------------------------------------------------------*/

	/*Init ----------------------------------------------------------------------------------------------------------------*/
	xEventGroupWaitBits(dc_spd_events, DC_SPD_CONF_READY, pdFALSE, pdTRUE, portMAX_DELAY);

	lwbtn_init_ex(&dc_spd_pannel, dc_spd_pannel_btns, 3, dc_spd_panel_btn_get_state, dc_spd_panel_btn_event);
	lwbtn_init_ex(&dc_spd_ext, dc_spd_key_btns, 4, dc_spd_key_btn_get_state, dc_spd_key_btn_event);
	/* Infinite loop */
	for(;;)
	{
		lwbtn_process_ex(&dc_spd_pannel, HAL_GetTick());
		lwbtn_process_ex(&dc_spd_ext, HAL_GetTick());
		osDelay(20);
	}
	/* USER CODE END dc_spd_dig_read_task_entry */
}
/*Function prototype ------------------------------------------------------------------------------------------------------*/
uint8_t dc_spd_panel_btn_get_state(struct lwbtn* lw, struct lwbtn_btn* btn){

	static uint8_t ticks = 0;
	uint8_t ret = 0;
	uint32_t button = (uint32_t)(btn->arg);

	if((HAL_GetTick() - ticks)>= 2000){
		switch(button){
		case DC_SPD_PANEL_DOWN:
			ret = (T_DOWN_GPIO_Port->IDR & T_DOWN_Pin) ? 0 : 1;
			break;
		case DC_SPD_PANEL_UP:
			ret = (T_UP_GPIO_Port->IDR & T_UP_Pin) ? 0 : 1;
			break;
		case DC_SPD_PANEL_RUN:
			ret = (T_START_GPIO_Port->IDR & T_START_Pin) ? 0 : 1;
			break;
		}
	}
	return ret;
}

uint8_t dc_spd_key_btn_get_state(struct lwbtn* lw, struct lwbtn_btn* btn){

	static uint8_t ticks = 0;
	uint8_t ret = 0;
	uint32_t button = (uint32_t)(btn->arg);

	if((HAL_GetTick() - ticks)>= 2000){
		switch(button){
		case DC_SPD_KEY_UP:
			ret = (P_UP_GPIO_Port->IDR & P_UP_Pin) ? 0 : 1;
			break;
		case DC_SPD_KEY_DOWN:
			ret = (P_DOWN_GPIO_Port->IDR & P_DOWN_Pin) ? 0 : 1;
			break;
		case DC_SPD_KEY_ENTER:
			ret = (P_ENTER_GPIO_Port->IDR & P_ENTER_Pin) ? 0 : 1;
			break;
		case DC_SPD_KEY_MENU:
			ret = (P_MENU_GPIO_Port->IDR & P_MENU_Pin) ? 0 : 1;
			break;
		}
	}

	return ret;
}

/*Events -----------------------------------------------------------------------------------------------------------------*/
void dc_spd_button_process_evnt(uint8_t button, uint32_t *ticks, uint8_t evt){

	switch(evt){
	case LWBTN_EVT_ONPRESS:
		//set flag press
		xEventGroupSetBits(dc_spd_button_events, 1<<button);
		//init ticks
		if(ticks != NULL){
			*ticks = HAL_GetTick();
		}
		break;
	case LWBTN_EVT_KEEPALIVE:
		if(ticks != NULL){
			if((HAL_GetTick() - *ticks)>= DC_SPD_LONG_MIN_PRESS){
				//set flags
				xEventGroupSetBits(dc_spd_button_events, 1<<(DC_SPD_MAX_BTN + button));
			}
		}
		break;
	case LWBTN_EVT_ONRELEASE:
		//clear flag
		xEventGroupClearBits(dc_spd_button_events, (1<<button) | (1<<(button + DC_SPD_MAX_BTN)));
		//if run button is set
		if(button == DC_SPD_PANEL_RUN){
			xEventGroupSetBits(dc_spd_button_events, DC_SPD_PANEL_STOP);
		}
		break;
	}
}


void dc_spd_panel_btn_event(struct lwbtn* lw, struct lwbtn_btn* btn, lwbtn_evt_t evt){
	uint32_t button = (uint32_t)(btn->arg);
	static uint32_t ticks_down, ticks_up;
	switch(button){
	case DC_SPD_PANEL_DOWN:
		dc_spd_button_process_evnt(button, &ticks_down, evt);
		break;
	case DC_SPD_PANEL_UP:
		dc_spd_button_process_evnt(button, &ticks_up, evt);
		break;
	case DC_SPD_PANEL_RUN:
		dc_spd_button_process_evnt(button, NULL, evt);
		break;
	}

}
void dc_spd_key_btn_event(struct lwbtn* lw, struct lwbtn_btn* btn, lwbtn_evt_t evt){

	uint32_t button = (uint32_t)(btn->arg);
	static uint32_t ticks_down, ticks_up;
	switch(button){
	case DC_SPD_KEY_DOWN:
		dc_spd_button_process_evnt(button, &ticks_down, evt);
		break;
	case DC_SPD_KEY_UP:
		dc_spd_button_process_evnt(button, &ticks_up, evt);
		break;
	case DC_SPD_KEY_ENTER:
		dc_spd_button_process_evnt(button, NULL, evt);
		break;
	case DC_SPD_KEY_MENU:
		dc_spd_button_process_evnt(button, NULL, evt);
		break;
	}
}

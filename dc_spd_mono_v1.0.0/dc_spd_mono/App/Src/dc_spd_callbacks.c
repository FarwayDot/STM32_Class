/*
 * dc_spd_callbacks.c
 *
 *  Created on: Dec 9, 2024
 *      Author: USER
 */


/*Includes -----------------------------------------------------------------------------------------------------------*/
#include "main.h"
#include "dc_spd_mono.h"
#include <stdbool.h>
/*Defines -----------------------------------------------------------------------------------------------------------*/


/*Typedefs ----------------------------------------------------------------------------------------------------------*/
typedef struct DC_SPD_Pulse{
	bool flag;
	uint32_t count;
	uint8_t state;
}DC_SPD_Pulse_t;

/*Global variables --------------------------------------------------------------------------------------------------*/
DC_SPD_Pulse_t dc_spd_positive;
DC_SPD_Pulse_t dc_spd_negative;
/*Function prototype ------------------------------------------------------------------------------------------------*/

/*Callback definition -----------------------------------------------------------------------------------------------*/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc){
	/**
	 * adc conversion finish
	 */
	osSemaphoreRelease(dc_spd_adc_flagHandle);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin){
	case INT_AC1_Pin: //positive half cycle (rising flag)
		if(INT_AC1_GPIO_Port->IDR & INT_AC1_Pin){
			dc_spd_positive.flag = true;
			dc_spd_positive.count = 0;
		}
		break;
	case INT_AC2_Pin: //negative half cycle flag (rising flag)
		if(INT_AC2_GPIO_Port->IDR & INT_AC2_Pin){
			dc_spd_negative.flag = true;
			dc_spd_negative.count = 0;
		}
		break;
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//Increment count
	if(dc_spd_positive.flag == true){
		dc_spd_positive.count += 1;
		//verify state
		switch(dc_spd_positive.state){
		case 0:
			if(dc_spd_positive.count >= (ZERO_CROSS_FIRST_DELAY + dc_spd.alpha)){

				dc_spd_positive.state = 1;
			}
			break;
		case 1:
			if(dc_spd_positive.count >= (ZERO_CROSS_MAX_COUNT)){

				dc_spd_positive.state = 0;
				dc_spd_positive.flag = false;
			}
			break;
		}
	}
	if(dc_spd_negative.flag == true){
		dc_spd_negative.count += 1;
		//verify state
		switch(dc_spd_negative.state){
		case 0:
			if(dc_spd_negative.count >= (ZERO_CROSS_FIRST_DELAY + dc_spd.alpha)){

				dc_spd_negative.state = 1;
			}
			break;
		case 1:
			if(dc_spd_negative.count >= (ZERO_CROSS_MAX_COUNT)){

				dc_spd_negative.state = 0;
				dc_spd_negative.flag = false;
			}
			break;
		}
	}



}
/*Function definition -----------------------------------------------------------------------------------------------*/


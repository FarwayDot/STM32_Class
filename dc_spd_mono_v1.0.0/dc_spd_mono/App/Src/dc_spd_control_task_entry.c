/*
 * dc_spd_control_task_entry.c
 *
 *  Created on: Dec 8, 2024
 *      Author: USER
 */

/*Includes ----------------------------------------------------------------------------------------------------------------*/
#include "dc_spd_mono.h"
#include "PID.h"
#include "RC_LowPassFilter.h"
/*Defines -----------------------------------------------------------------------------------------------------------------*/
#ifndef DC_SPD_ADC_TS
#define DC_SPD_ADC_TS	(float)5E-3
#endif

#ifndef KP
#define KP	 						0.0526
#endif

#ifndef KI
#define KI	 						0.239
#endif

#ifndef KD
#define KD							0
#endif

#ifndef UMAX
#define UMAX 						415
#endif

#ifndef UMIN
#define UMIN 						10
#endif

#ifndef DC_SPD_ADC_WAIT
#define DC_SPD_ADC_WAIT				500
#endif

#define ADC_OFFSET		2015 //YAN: Se cambia de 2005 a 2015

#define	START_TIME					10000 //YAN: De  6000 a 10000
#define STOP_TIME					3000

/*Typedefs ----------------------------------------------------------------------------------------------------------------*/


/*Global variables --------------------------------------------------------------------------------------------------------*/
static RC_LowPassFilter_t F1;
static RC_LowPassFilter_t F2;
static RC_LowPassFilter_t F3;
static RC_LowPassFilter_t F4;

PID_Def_t pid;

static uint16_t dc_spd_adc_buff[4];
/*Function prototype ------------------------------------------------------------------------------------------------------*/

uint32_t dc_spd_start_time(float ref);
uint16_t dc_spd_open_loop(uint16_t ref);
/*Task definition ---------------------------------------------------------------------------------------------------------*/
void dc_spd_control_task_entry(void const * argument)
{
	/*Local variables -----------------------------------------------------------------------------------------------------*/
	uint32_t ticks ;
	float adc1, adc2, adc3, adc4;
	float progress;
	float alphaMin = 0;
	/*Init ----------------------------------------------------------------------------------------------------------------*/

	/*RC Filter init*/
	RC_LowPassFilter_Init(&F1, 1, DC_SPD_ADC_TS);
	RC_LowPassFilter_Init(&F2, 1, DC_SPD_ADC_TS);
	RC_LowPassFilter_Init(&F3, 1, DC_SPD_ADC_TS);
	RC_LowPassFilter_Init(&F4, 1, DC_SPD_ADC_TS);

	/*PID Init -----------------------------------------------------------------------------------------------------------*/
	PID_Init(&pid, KP, KI, KD, DC_SPD_ADC_TS, UMIN, UMAX);

	/*Wait for init */
	xEventGroupWaitBits(dc_spd_events, DC_SPD_CONF_READY, pdFALSE, pdTRUE, portMAX_DELAY);

	/*Init adc conversion*/
	MX_ADC1_Init();
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)dc_spd_adc_buff, 4);
	/*Init adc filters*/
	ticks = HAL_GetTick();
	do{
		/*Wait for adc flag*/
		osSemaphoreWait(dc_spd_adc_flagHandle, osWaitForever);
		/*Filter apply*/
		adc1 = RC_LowPassFilter_Appy(&F1, dc_spd_adc_buff[0]);
		adc2 = RC_LowPassFilter_Appy(&F2, dc_spd_adc_buff[1]);
		adc3 = RC_LowPassFilter_Appy(&F3, dc_spd_adc_buff[2]);
		adc4 = RC_LowPassFilter_Appy(&F4, dc_spd_adc_buff[3]);

	}while((HAL_GetTick() - ticks) <= DC_SPD_ADC_WAIT);
	/* Infinite loop */
	for(;;)
	{
		/*Wait for adc flag*/
		osSemaphoreWait(dc_spd_adc_flagHandle, osWaitForever);
		/*Apply filter*/
		/*Current*/
		adc1 = RC_LowPassFilter_Appy(&F1, dc_spd_adc_buff[0]);
		/*Voltage*/
		adc2 = RC_LowPassFilter_Appy(&F2, dc_spd_adc_buff[1]);
		/*Tachometer*/
		adc3 = RC_LowPassFilter_Appy(&F3, dc_spd_adc_buff[2]);
		/*Pot*/
		adc4 = RC_LowPassFilter_Appy(&F4, dc_spd_adc_buff[3]);
		//procces data
		if(dc_spd.conf.feedback == DC_SPD_FEEDBACK_TACHO){
			dc_spd.pot = (0.5412) * adc4 * 2;//(0.7326) * adc * 2; //YAN: Para valor máximo del pot
			if(dc_spd.conf.control == DC_SPD_CONTROL_POT){
				dc_spd.conf.ref = dc_spd.pot;
			}
		}else{
			dc_spd.pot = (0.5412) * adc4;//(0.7326) * adc * 2; //YAN: Para valor máximo del pot
			if(dc_spd.conf.control == DC_SPD_CONTROL_POT){
				dc_spd.conf.ref = dc_spd.pot;
			}
			//cal pot
			dc_spd.pot = dc_spd_open_loop(dc_spd.conf.ref); //calc open loop alpha
			dc_spd.pot = dc_spd.pot < ALPHAMAX ? ALPHAMAX : dc_spd.pot;
			//calc percentage
			dc_spd.percentage = (100 * dc_spd.pot /(ALPHAMAX - UMAX)) - ((float)100 * UMAX / (ALPHAMAX - UMAX));
			dc_spd.percentage = dc_spd.percentage > 100  ? 100: dc_spd.percentage;

		}
		/*State machine control ------------------------------------------------------------------------------------------*/
		switch(dc_spd.controlState){
		case DC_SPD_CONTROL_START:
			RELAY_GPIO_Port->BSRR |= RELAY_Pin;
			if(dc_spd.conf.feedback == DC_SPD_FEEDBACK_TACHO){

				if((HAL_GetTick() - dc_spd.startTicks)>= dc_spd_start_time(dc_spd.conf.ref)){
					dc_spd.refCurrent = dc_spd.conf.ref;
					dc_spd.controlState = DC_SPD_CONTROL_LOOP;
				}else{
					progress = (float)(HAL_GetTick() - dc_spd.startTicks) / dc_spd_start_time(dc_spd.conf.ref);
					dc_spd.refCurrent = dc_spd.refStart + (int)(dc_spd.delta * progress);
				}
				dc_spd.Uk =UMAX - PID_Compute(&pid, dc_spd.refCurrent, dc_spd.rpm);
				if(dc_spd.Uk >= ALPHAMAX){
					dc_spd.alpha =(uint16_t)(dc_spd.Uk);
				}else{
					dc_spd.alpha = ALPHAMAX;
				}
			}else{
				//increment alpha
				if((HAL_GetTick() - dc_spd.startTicks)>= OPEN_LOOP_START_TIME_INC){
					dc_spd.startTicks = HAL_GetTick();
					//increment
					dc_spd.alpha -= OPEN_LOOP_STEP;

					//verify
					if(dc_spd.alpha < dc_spd.pot){
						dc_spd.alpha = dc_spd.pot;
						dc_spd.controlState = DC_SPD_CONTROL_LOOP;
					}
				}
			}
			break;
		case DC_SPD_CONTROL_LOOP:
			if(dc_spd.conf.feedback == DC_SPD_FEEDBACK_TACHO){
				dc_spd.refCurrent = dc_spd.conf.ref;
				dc_spd.Uk = UMAX - PID_Compute(&pid, dc_spd.refCurrent, dc_spd.rpm);

				/*Calc voltage limiter*/
				alphaMin = (-0.5059 * dc_spd.conf.vmax+ 406.15 );
				if((dc_spd.Uk < alphaMin) && (dc_spd.V >= dc_spd.conf.vmax)){
					dc_spd.alpha = (uint16_t)alphaMin;
					PID_Reset(&pid);
				}
				else{
					if(dc_spd.Uk >= ALPHAMAX){
						dc_spd.alpha =(uint16_t)(dc_spd.Uk);
					}else{
						dc_spd.alpha = ALPHAMAX;
					}
				}
			}else{
				/*calc alpha*/
				dc_spd.Uk = dc_spd.pot;
				/*Calc voltage max*/
				alphaMin = (-0.5059 * dc_spd.conf.vmax+ 406.15 );
				if((dc_spd.Uk < alphaMin) && (dc_spd.V >= dc_spd.conf.vmax)){
					dc_spd.alpha = (uint16_t)alphaMin;
				}
				else{
					if(dc_spd.Uk >= ALPHAMAX){
						dc_spd.alpha =(uint16_t)(dc_spd.Uk);
					}else{
						dc_spd.alpha = ALPHAMAX;
					}
				}
			}
			break;
		case DC_SPD_CONTROL_STOP:
			if(dc_spd.conf.feedback == DC_SPD_FEEDBACK_TACHO){
				if((HAL_GetTick() - dc_spd.startTicks)>= STOP_TIME){
					if(dc_spd.rpm <5){
						RELAY_GPIO_Port->BSRR |= RELAY_Pin<<16;
						dc_spd.refCurrent = dc_spd.conf.ref;
						dc_spd.controlState = DC_SPD_CONTROL_IDLE;
					}
					/**
					 * stop all
					 */

					HAL_TIM_Base_Stop_IT(&htim2);
					/**
					 * reset pid
					 */
					PID_Reset(&pid);
					dc_spd.alpha = UMAX;
				}else{
					progress = (float)(HAL_GetTick() - dc_spd.startTicks) / STOP_TIME;
					dc_spd.refCurrent = dc_spd.refStart + (int)(dc_spd.delta * progress);
					dc_spd.Uk =UMAX - PID_Compute(&pid, dc_spd.refCurrent, dc_spd.rpm);
					if(dc_spd.Uk >= ALPHAMAX){
						dc_spd.alpha =(uint16_t)(dc_spd.Uk);
					}else{
						dc_spd.alpha = ALPHAMAX;
					}

				}
			}else{
				//increment alpha
				if((HAL_GetTick() - dc_spd.startTicks)>= OPEN_LOOP_STOP_TIME_INC){
					dc_spd.startTicks = HAL_GetTick();
					//increment
					dc_spd.alpha += OPEN_LOOP_STEP;

					//verify
					if(dc_spd.alpha >= UMAX){
						dc_spd.alpha = UMAX;
						dc_spd.controlState = DC_SPD_CONTROL_IDLE;
						//stop all

						HAL_TIM_Base_Stop_IT(&htim2);
						//relay set
						RELAY_GPIO_Port->BSRR |= RELAY_Pin<<16;
					}
				}
			}
			break;
		default:
			break;
		}
	}
	/* USER CODE END dc_spd_control_task_entry */
}

/*Function definition -----------------------------------------------------------------------------------------------------*/
void HAL_SYSTICK_Callback(void)
{
	static uint32_t ticksAdc = 0;
	ticksAdc += 1;
	//adc convertions flags (200hz)
	if(HAL_ADC_GetState(&hadc1) != HAL_ADC_STATE_RESET){
		if(ticksAdc >= (DC_SPD_ADC_TS * 1000)){
			ticksAdc = 0;
			HAL_ADC_Start_DMA(&hadc1,(uint32_t *) dc_spd_adc_buff, 5);
		}

	}
}

uint32_t dc_spd_start_time(float ref){
	return (2.1053 * ref + 789.47);
}

uint16_t dc_spd_open_loop(uint16_t ref){
	//y = -0.0725x + 380 //se requiere mayor rango
	return (uint16_t)(-0.1309 * ref + UMAX);
}


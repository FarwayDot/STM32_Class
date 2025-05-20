/*
 * dc_spd_mono_typedefs.h
 *
 *  Created on: Dec 4, 2024
 *      Author: USER
 */

#ifndef APP_INC_DC_SPD_MONO_TYPEDEFS_H_
#define APP_INC_DC_SPD_MONO_TYPEDEFS_H_

/*includes ----------------------------------------------------------------------------------------------------------------*/
#include "dc_spd_mono_config.h"
#include <stdint.h>
#include <cmsis_os.h>


/*Typedefs-----------------------------------------------------------------------------------------------------------------*/
typedef enum DC_SPD_Buttons{
	DC_SPD_KEY_UP = 0,
	DC_SPD_KEY_DOWN,
	DC_SPD_KEY_ENTER,
	DC_SPD_KEY_MENU,
	DC_SPD_PANEL_UP,
	DC_SPD_PANEL_DOWN,
	DC_SPD_PANEL_RUN,
	DC_SPD_PANEL_STOP,
	DC_SPD_MAX_BTN,
}DC_SPD_Buttons_t;


typedef enum DC_SPD_Feedback{
	DC_SPD_FEEDBACK_NONE,
	DC_SPD_FEEDBACK_TACHO
}DC_SPD_Feedback_t;

typedef enum DC_SPD_ControlType{
	DC_SPD_CONTROL_POT,
	DC_SPD_CONTROL_KEYPAD,
}DC_SPD_ControlType_t;

typedef struct __attribute__((packed)) DC_SPD_Conf {
	uint16_t ref;		//ref control (only on button control)
	uint16_t imax;
	uint16_t vmax;
	float tacometer;	//tachometer resolution
	uint8_t feedback;	//feedback type
	uint8_t control;	//control type (button / pot)
}DC_SPD_Conf_t;


typedef struct DC_SPD_Display{
	SemaphoreHandle_t updateFlag;
	int8_t select;
	uint8_t screen;
	int32_t input;
}DC_SPD_Display_t;

typedef enum DC_SPD_Control{
	DC_SPD_CONTROL_START,
	DC_SPD_CONTROL_LOOP,
	DC_SPD_CONTROL_STOP,
	DC_SPD_CONTROL_IDLE,
}DC_SPD_Control_t;


typedef struct{
	DC_SPD_Conf_t conf;
	uint8_t control;			//control state (run / stop)
	/**
	 * electrical parameters
	 */
	float I;					//current measurement
	float V;					//voltage measurement
	/**
	 * potentiometer value
	 */
	float pot;					//potentiometer
	/*Tachometer read*/
	float taco;					//tachometer
	float Uk;
	/**
	 * RPM use for PID control
	 */
	float rpm;
	/**
	 * rpm show on LCD
	 */
	uint16_t rpmShow;
	/*
	 * data access protection
	 */
	SemaphoreHandle_t mutex;

	/**
	 * control parameters
	 */
	uint8_t controlState;
	int16_t delta;
	uint32_t startTicks;
	uint16_t refCurrent;
	uint16_t refStart;
	uint16_t alpha;

	/*open loop*/
	uint16_t alpha_pot;
	uint8_t percentage;
}DC_SPD_Data_t;


typedef enum DC_SPD_Events{
	DC_SPD_CONF_READY 		= 1,				//ready conf
}DC_SPD_Events_t;
#endif /* APP_INC_DC_SPD_MONO_TYPEDEFS_H_ */

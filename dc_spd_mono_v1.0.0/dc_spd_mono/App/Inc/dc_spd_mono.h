/*
 * dc_spd_mono.h
 *
 *  Created on: Dec 4, 2024
 *      Author: USER
 */

#ifndef APP_INC_DC_SPD_MONO_H_
#define APP_INC_DC_SPD_MONO_H_

/*Includes --------------------------------------------------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "gpio.h"
#include "i2c.h"
#include "PID.h"
#include "cmsis_os.h"
#include "lcd.h"
#include "driver_at24cxx_interface.h"
#include "driver_at24cxx.h"
#include "dc_spd_mono_typedefs.h"
#include "event_groups.h"
#include "adc.h"
/*Defines ---------------------------------------------------------------------------------------------------------------*/
#ifndef DC_SPD_EEPROM_FIRST_CHAR
#define DC_SPD_EEPROM_FIRST_CHAR 	0
#endif

#ifndef DC_SPD_EEPROM_FIRST_CHAR_ADDR
#define DC_SPD_EEPROM_FIRST_CHAR_ADDR		0x0
#endif

#ifndef DC_SPD_EEPROM_CONF_ADDR
#define DC_SPD_EEPROM_CONF_ADDR			0x1
#endif
/*Typedefs --------------------------------------------------------------------------------------------------------------*/

/*Extern definition -----------------------------------------------------------------------------------------------------*/
extern EventGroupHandle_t dc_spd_events;
extern osSemaphoreId dc_spd_adc_flagHandle;
extern EventGroupHandle_t dc_spd_button_events;
extern at24cxx_handle_t eeprom;
extern DC_SPD_Data_t dc_spd;
extern PID_Def_t pid;
/*Function prototype ----------------------------------------------------------------------------------------------------*/

uint8_t dc_spd_button_press(uint8_t button);
uint8_t dc_spd_button_press_long(uint8_t button);


#endif /* APP_INC_DC_SPD_MONO_H_ */

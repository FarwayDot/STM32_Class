/*
 * dc_spd_main_task_entry.c
 *
 *  Created on: Dec 4, 2024
 *      Author: USER
 */

/*Includes ---------------------------------------------------------------------------------------------------------------*/
#include "dc_spd_mono.h"

/*Defines ----------------------------------------------------------------------------------------------------------------*/

/*Typedefs ---------------------------------------------------------------------------------------------------------------*/

/*Global variables -------------------------------------------------------------------------------------------------------*/
at24cxx_handle_t eeprom;
const at24cxx_address_t addr_eeprom = AT24CXX_ADDRESS_A000;
const at24cxx_t chip_type = AT24C256;

DC_SPD_Data_t dc_spd;

EventGroupHandle_t dc_spd_events;
/*Function prototype -----------------------------------------------------------------------------------------------------*/



/*Task definition --------------------------------------------------------------------------------------------------------*/
void dc_spd_main_task_entry(void const * argument)
{
	/*Local variables ----------------------------------------------------------------------------------------------------*/
	uint8_t res;
	/*Eeprom init --------------------------------------------------------------------------------------------------------*/
	DRIVER_AT24CXX_LINK_INIT(&eeprom, at24cxx_handle_t);
	DRIVER_AT24CXX_LINK_IIC_INIT(&eeprom, at24cxx_interface_iic_init);
	DRIVER_AT24CXX_LINK_IIC_DEINIT(&eeprom, at24cxx_interface_iic_deinit);
	DRIVER_AT24CXX_LINK_IIC_READ(&eeprom, at24cxx_interface_iic_read);
	DRIVER_AT24CXX_LINK_IIC_WRITE(&eeprom, at24cxx_interface_iic_write);
	DRIVER_AT24CXX_LINK_IIC_READ_ADDRESS16(&eeprom, at24cxx_interface_iic_read_address16);
	DRIVER_AT24CXX_LINK_IIC_WRITE_ADDRESS16(&eeprom, at24cxx_interface_iic_write_address16);
	DRIVER_AT24CXX_LINK_DELAY_MS(&eeprom, at24cxx_interface_delay_ms);
	DRIVER_AT24CXX_LINK_DEBUG_PRINT(&eeprom, at24cxx_interface_debug_print);
	at24cxx_set_type(&eeprom, chip_type);

	/* set addr pin */
	at24cxx_set_addr_pin(&eeprom, addr_eeprom);
	res = at24cxx_init(&eeprom);
	configASSERT(res == 0);
	/*Wait for power voltaje ---------------------------------------------------------------------------------------------*/
	osDelay(1500);
	/*Restores variables -------------------------------------------------------------------------------------------------*/
	at24cxx_read(&eeprom, DC_SPD_EEPROM_FIRST_CHAR_ADDR, &res, 1);
	if(res != DC_SPD_EEPROM_FIRST_CHAR){
		/*Default config ----------------------------------------------------------*/
		dc_spd.conf.control = DC_SPD_CONTROL_KEYPAD;
		dc_spd.conf.feedback = DC_SPD_FEEDBACK_TACHO;
		dc_spd.conf.imax = 30;
		dc_spd.conf.vmax = 380;
		dc_spd.conf.ref = 100;
		dc_spd.conf.tacometer = 0.06;
		/*Write first*/
		at24cxx_write(&eeprom, DC_SPD_EEPROM_FIRST_CHAR_ADDR,  &res, 1);
		/*Write default data*/
		at24cxx_write(&eeprom, DC_SPD_EEPROM_CONF_ADDR, (uint8_t *)&dc_spd.conf, sizeof(DC_SPD_Conf_t));
	}else{
		res = at24cxx_read(&eeprom, DC_SPD_EEPROM_CONF_ADDR, (uint8_t *)&dc_spd.conf, sizeof(DC_SPD_Conf_t));
		configASSERT( res == 0);
	}

	/*Set ready flags*/
	xEventGroupSetBits(dc_spd_events, DC_SPD_CONF_READY);
	/* Infinite loop */
	for(;;)
	{
		if(dc_spd_button_press(DC_SPD_PANEL_RUN) == 1){
			//init control
			xSemaphoreTake(dc_spd.mutex, portMAX_DELAY);
			dc_spd.control = 1;
			xSemaphoreGive(dc_spd.mutex);
			//change state control
			PID_Reset(&pid);
			dc_spd.alpha = ZERO_CROSS_MAX_COUNT - UMIN;
			dc_spd.refStart = 0;
			dc_spd.delta = dc_spd.conf.ref;
			dc_spd.startTicks = HAL_GetTick();
			dc_spd.controlState = DC_SPD_CONTROL_START;
			PID_Reset(&pid);
			HAL_TIM_Base_Start_IT(&htim2);
		}else if(dc_spd_button_press(DC_SPD_PANEL_STOP) == 1){
			//stop control
			xSemaphoreTake(dc_spd.mutex, portMAX_DELAY);
			dc_spd.control = 1;
			xSemaphoreGive(dc_spd.mutex);

			if(dc_spd.controlState != DC_SPD_CONTROL_IDLE){
				dc_spd.refCurrent = dc_spd.conf.ref;
				dc_spd.delta = -dc_spd.conf.ref;
				dc_spd.refStart = dc_spd.refCurrent;
				dc_spd.startTicks = HAL_GetTick();
				dc_spd.controlState = DC_SPD_CONTROL_STOP;
			}
		}
		//led toogle
		osDelay(20);
	}
}

/*Function definition ----------------------------------------------------------------------------------------------------*/



/*
 * dc_spd_lcd_task_entry.c
 *
 *  Created on: Dec 8, 2024
 *      Author: USER
 */
/*Includes ----------------------------------------------------------------------------------------------------------------*/
#include "dc_spd_mono.h"
#include "lcd.h"

/*Defines -----------------------------------------------------------------------------------------------------------------*/
#ifndef DC_SPD_KEY_INPUT_TIME
#define DC_SPD_KEY_INPUT_TIME	2000
#endif

#ifndef DC_SPD_SP_MIN_VAL
#define DC_SPD_SP_MIN_VAL					100
#endif

#ifndef DC_SPD_SP_MAX_VAL
#define DC_SPD_SP_MAX_VAL					6000
#endif


#ifndef DISPLAY_MENU_MAX_SIZE
#define DISPLAY_MENU_MAX_SIZE	4
#endif
/*Typedefs ----------------------------------------------------------------------------------------------------------------*/
typedef enum DC_SPD_DisplayMenu{
	DC_SPD_LCD_HOME,
	DC_SPD_LCD_MAIN_MENU,
	DC_SPD_LCD_VOLT,
	DC_SPD_LCD_CURR,
	DC_SPD_LCD_FEEDBACK,
	DC_SPD_LCD_CONTROL,
	DC_SPD_LCD_FEEDACK_CONF,
}DC_SPD_DisplayMenu_t;
/*Global variables --------------------------------------------------------------------------------------------------------*/
static LCD_t lcd;
static DC_SPD_Display_t display;

char displayMenu[DISPLAY_MENU_MAX_SIZE][17] = {
		"Max. Voltage",
		"Max. Current",
		"Feedback",
		"Ref. Control",
};
/*Function prototype ------------------------------------------------------------------------------------------------------*/

static void dc_spd_lcd_write_nb(uint8_t data, uint8_t control);
static void dc_spd_lcd_udpate_measurement(void);
static void dc_spd_lcd_main_menu(void);
/*Task definition ---------------------------------------------------------------------------------------------------------*/
void dc_spd_lcd_task_entry(void const * argument)
{
	/*Local variables -----------------------------------------------------------------------------------------------------*/
	uint8_t dc_spd_last_state = 0;
	uint32_t ticks = 0;
	uint32_t ticksUpdate ;
	uint8_t dc_spd_toggle_display = 0;
	/*Init ----------------------------------------------------------------------------------------------------------------*/
	/*Wait for flag ready*/
	xEventGroupWaitBits(dc_spd_events, DC_SPD_CONF_READY, pdFALSE, pdTRUE, portMAX_DELAY);
	/*LCD Init -----------------------------------------------------------------------------------------------------------*/
	lcd_init(&lcd, dc_spd_lcd_write_nb, HAL_Delay, 16, 4);
	lcd_printf(&lcd, 0, 0,(char *) "--->DC SPEED<---");
	lcd_printf(&lcd, 0, 1, (char *)"FW.%s\r\n%s", DC_SPD_FW_VERSION, __DATE__);
	for(uint8_t i = 0; i<16; i++){
		lcd_printf(&lcd, i, 3,(char *) ".");
		vTaskDelay(pdMS_TO_TICKS(150));
	}
	/*Init ---------------------------------------------------------------------*/
	display.screen = DC_SPD_LCD_HOME;
	/*Set ready events ---------------------------------------------------------*/

	lcd_clear(&lcd);
	lcd_printf(&lcd, 0, 1,(char *)"sp:%d ", dc_spd.conf.ref);
	lcd_printf(&lcd, 0, 0,(char *) (dc_spd.control  ? "------>RUN<-----":"----->STOP<-----"));
	lcd_printf(&lcd, 0, 2, (char *)"Imax:%d", (int)dc_spd.conf.imax);
	lcd_printf(&lcd, 0, 3, (char *)"Vmax:%d", (int)dc_spd.conf.vmax);

	dc_spd_last_state = dc_spd.control;
	/* Infinite loop */
	for(;;)
	{
		switch(display.screen){
		case DC_SPD_LCD_HOME:
			//update state dc speed
			if(dc_spd_last_state != dc_spd.control){
				dc_spd_last_state = dc_spd.control;
				/*clear line*/
				lcd_clearLine(&lcd, 0, 15, 0);
				lcd_printf(&lcd, 0, 0,(char *) (dc_spd.control  ? "------>RUN<-----":"----->STOP<-----"));
			}
			/*Update measurement*/
			if((HAL_GetTick() - ticks)>= 400){
				ticks = HAL_GetTick();
				/*RPM*/
				dc_spd_lcd_udpate_measurement();
			}
			/*verify control method and Read buttons up and down*/
			if(dc_spd.conf.control == DC_SPD_CONTROL_KEYPAD){
				if(dc_spd_button_press(DC_SPD_KEY_UP) || dc_spd_button_press(DC_SPD_PANEL_UP)){
					//update ticks
					ticks = HAL_GetTick();
					ticksUpdate = HAL_GetTick();
					//first input
					xSemaphoreTake(dc_spd.mutex, portMAX_DELAY);
					dc_spd.conf.ref += 1;
					dc_spd.conf.ref = (dc_spd.conf.ref > DC_SPD_SP_MAX_VAL) ? DC_SPD_SP_MIN_VAL : dc_spd.conf.ref;
					xSemaphoreGive(dc_spd.mutex);
					dc_spd_lcd_udpate_measurement();
					//enter loop
					do{
						if(dc_spd_button_press(DC_SPD_KEY_UP) || dc_spd_button_press(DC_SPD_PANEL_UP) ||
								dc_spd_button_press_long(DC_SPD_KEY_UP) || dc_spd_button_press_long(DC_SPD_PANEL_UP)){
							ticks = HAL_GetTick();
							ticksUpdate = HAL_GetTick();
							//first input
							xSemaphoreTake(dc_spd.mutex, portMAX_DELAY);
							dc_spd.conf.ref += 1;
							dc_spd.conf.ref = (dc_spd.conf.ref > DC_SPD_SP_MAX_VAL) ? DC_SPD_SP_MIN_VAL : dc_spd.conf.ref;
							xSemaphoreGive(dc_spd.mutex);
							//update
							dc_spd_lcd_udpate_measurement();
						}
						//update and toggle
						if((HAL_GetTick() - ticks)>= 300){
							ticks = HAL_GetTick();
							dc_spd_toggle_display ^= 1;
							/*Update*/
							dc_spd_lcd_udpate_measurement();
							/*Toggle is need?*/
							if(dc_spd_toggle_display & 1){
								lcd_clearLine(&lcd, 3, 7, 1);
							}
						}
					}while((HAL_GetTick() - ticks)< DC_SPD_KEY_INPUT_TIME);
					//save data
					//save data
					at24cxx_write(&eeprom, DC_SPD_EEPROM_CONF_ADDR, (uint8_t *)&dc_spd.conf, sizeof(DC_SPD_Conf_t));
				}
				else if(dc_spd_button_press(DC_SPD_KEY_DOWN) || dc_spd_button_press(DC_SPD_PANEL_DOWN)){
					//update ticks
					ticks = HAL_GetTick();
					ticksUpdate = HAL_GetTick();
					/*first input*/
					xSemaphoreTake(dc_spd.mutex, portMAX_DELAY);
					dc_spd.conf.ref -= 1;
					dc_spd.conf.ref = (dc_spd.conf.ref < DC_SPD_SP_MIN_VAL) ? DC_SPD_SP_MAX_VAL : dc_spd.conf.ref;
					xSemaphoreGive(dc_spd.mutex);
					//enter loop
					do{
						if(dc_spd_button_press(DC_SPD_KEY_DOWN) || dc_spd_button_press(DC_SPD_PANEL_DOWN) ||
								dc_spd_button_press_long(DC_SPD_KEY_DOWN) || dc_spd_button_press_long(DC_SPD_PANEL_DOWN)){
							ticks = HAL_GetTick();
							ticksUpdate = HAL_GetTick();
							//first input
							xSemaphoreTake(dc_spd.mutex, portMAX_DELAY);
							dc_spd.conf.ref -= 1;
							dc_spd.conf.ref = (dc_spd.conf.ref < DC_SPD_SP_MIN_VAL) ? DC_SPD_SP_MAX_VAL : dc_spd.conf.ref;
							xSemaphoreGive(dc_spd.mutex);
							//update
							dc_spd_lcd_udpate_measurement();
						}
						//update and toggle
						if((HAL_GetTick() - ticks)>= 300){
							ticks = HAL_GetTick();
							dc_spd_toggle_display ^= 1;
							/*Update*/
							dc_spd_lcd_udpate_measurement();
							/*Toggle is need?*/
							if(dc_spd_toggle_display & 1){
								lcd_clearLine(&lcd, 3, 7, 1);
							}
						}
					}while((HAL_GetTick() - ticks)< DC_SPD_KEY_INPUT_TIME);
					//save data
					at24cxx_write(&eeprom, DC_SPD_EEPROM_CONF_ADDR, (uint8_t *)&dc_spd.conf, sizeof(DC_SPD_Conf_t));
				}
			}
			/*Read button menu*/
			if(dc_spd_button_press(DC_SPD_KEY_MENU) == 1){
				memset(&display, 0, sizeof(DC_SPD_Display_t));
				display.screen = DC_SPD_LCD_MAIN_MENU;
				//change to main menu
				dc_spd_lcd_main_menu();
			}
			break;
		case DC_SPD_LCD_MAIN_MENU:
			//verify up
			if(dc_spd_button_press(DC_SPD_KEY_UP)){
				display.select += 1;
				display.select = (display.select >= DISPLAY_MENU_MAX_SIZE) ? 0: display.select;
				//update display
				lcd_clearLine(&lcd, 0, 15, 2);
				lcd_printf(&lcd, 0, 2,(char *) "->%s", displayMenu[display.select]);
			}
			//verify down
			else if(dc_spd_button_press(DC_SPD_KEY_DOWN)){
				display.select -= 1;
				display.select = (display.select < 0) ? DISPLAY_MENU_MAX_SIZE - 1: display.select;
				//update display
				lcd_clearLine(&lcd, 0, 15, 2);
				lcd_printf(&lcd, 0, 2,(char *) "->%s", displayMenu[display.select]);
			}
			//verify menu
			else if(dc_spd_button_press(DC_SPD_KEY_MENU)){
				lcd_clear(&lcd);
				lcd_printf(&lcd, 0, 1,(char *)"sp:%d ", dc_spd.conf.ref);
				lcd_printf(&lcd, 0, 0,(char *) (dc_spd.control  ? "------>RUN<-----":"----->STOP<-----"));
				lcd_printf(&lcd, 0, 2, (char *)"Imax:%d", (int)dc_spd.conf.imax);
				lcd_printf(&lcd, 0, 3, (char *)"Vmax:%d", (int)dc_spd.conf.vmax);
			}
			//verify enter
			else if(dc_spd_button_press(DC_SPD_KEY_ENTER)){
				display.select += DC_SPD_LCD_VOLT;
				lcd_clear(&lcd);
				/*Select menu*/
				switch(display.select){
				case DC_SPD_LCD_VOLT:
					display.input = dc_spd.conf.vmax;
					lcd_printf(&lcd, 0, 0, (char *)"--->DC SPEED<---SETTINGS\r\n->Max. Voltage\r\n  Input:%d", (int)display.input);
					break;
				case DC_SPD_LCD_CURR:
					display.input = dc_spd.conf.imax;
					lcd_printf(&lcd, 0, 0,(char *) "--->DC SPEED<---SETTINGS\r\n->Max. Current\r\n  Input:%d", (int)display.input);
					break;
				case DC_SPD_LCD_FEEDBACK:
					lcd_printf(&lcd, 0, 0,(char *) "--->DC SPEED<---FB:%s\r\n  Tachometer\r\n  No Tachometer", dc_spd.conf.feedback  == DC_SPD_FEEDBACK_TACHO ? "Tachometer" : "NO");
					lcd_printf(&lcd, 0, dc_spd.conf.feedback + 2,(char *) "->");
					break;
				case DC_SPD_LCD_CONTROL:
					lcd_printf(&lcd, 0, 0, (char *) "--->DC SPEED<---ctrl:%s\r\n  Potentiometer\r\n  Buttons/ModBus", (dc_spd.conf.control == DC_SPD_CONTROL_POT) ? "Pot" : "Buttons");
					lcd_printf(&lcd, 0, dc_spd.conf.control + 2,(char *) "->");
					break;
				}
			}
			break;
		case DC_SPD_LCD_VOLT:
			if(dc_spd_button_press(DC_SPD_KEY_MENU)){
				display.screen = DC_SPD_LCD_MAIN_MENU;
				display.select = 0;
				dc_spd_lcd_main_menu();
			}
			else if(dc_spd_button_press(DC_SPD_KEY_UP) || dc_spd_button_press_long(DC_SPD_KEY_UP)){
				display.input += VMAX_STEP;
				display.input = (display.input > VMAX_MAXVAL) ? VMAX_MINVAL: display.input;
				lcd_clearLine(&lcd, 8, 15, 3);
				lcd_printf(&lcd, 8, 3, (char *)"%d", (int)display.input);

			}else if(dc_spd_button_press(DC_SPD_KEY_DOWN) || dc_spd_button_press_long(DC_SPD_KEY_DOWN)){
				display.input -= VMAX_STEP;
				display.input = (display.input < VMAX_MINVAL) ? VMAX_MAXVAL: display.input;
				lcd_clearLine(&lcd, 8, 15, 3);
				lcd_printf(&lcd, 8, 3, (char *)"%d", (int)display.input);
			}else if(dc_spd_button_press(DC_SPD_KEY_ENTER)){
				display.screen = DC_SPD_LCD_MAIN_MENU;
				display.select = 0;
				dc_spd.conf.vmax = display.input;

				dc_spd_lcd_main_menu();
				//todo: Confirm and save data
				at24cxx_write(&eeprom, DC_SPD_EEPROM_CONF_ADDR, (uint8_t *)&dc_spd.conf, sizeof(DC_SPD_Conf_t));
			}
			break;
		case DC_SPD_LCD_CURR:
			if(dc_spd_button_press(DC_SPD_KEY_MENU)){
				display.screen = DC_SPD_LCD_MAIN_MENU;
				display.select = 0;
				dc_spd_lcd_main_menu();
			}
			else if(dc_spd_button_press(DC_SPD_KEY_UP) || dc_spd_button_press_long(DC_SPD_KEY_UP)){
				display.input += 1;
				display.input = (display.input > IMAX_MAXVAL) ? IMAX_MINVAL: display.input;
				lcd_clearLine(&lcd, 8, 15, 3);
				lcd_printf(&lcd, 8, 3, (char *)"%d", (int)display.input);

			}else if(dc_spd_button_press(DC_SPD_KEY_DOWN) || dc_spd_button_press_long(DC_SPD_KEY_DOWN)){
				display.input -= 1;
				display.input = (display.input < IMAX_MINVAL) ? IMAX_MAXVAL: display.input;
				lcd_clearLine(&lcd, 8, 15, 3);
				lcd_printf(&lcd, 8, 3, (char *)"%d", (int)display.input);
			}else if(dc_spd_button_press(DC_SPD_KEY_ENTER)){
				display.screen = DC_SPD_LCD_MAIN_MENU;
				dc_spd.conf.imax = display.input;

				display.select = 0;
				dc_spd_lcd_main_menu();
				//todo: Confirm and save data
				at24cxx_write(&eeprom, DC_SPD_EEPROM_CONF_ADDR, (uint8_t *)&dc_spd.conf, sizeof(DC_SPD_Conf_t));
			}
			break;
		case DC_SPD_LCD_FEEDBACK:
			if(dc_spd_button_press(DC_SPD_KEY_MENU)){
				display.screen = DC_SPD_LCD_MAIN_MENU;
				display.select = 0;

				dc_spd_lcd_main_menu();

			}else if(dc_spd_button_press(DC_SPD_KEY_UP)){
				display.select += 1;
				display.select = (display.select >= 2) ? 0 : display.select;
				lcd_clearLine(&lcd, 0, 1, 2);
				lcd_clearLine(&lcd, 0, 1, 3);
				lcd_printf(&lcd, 0, display.select + 2,(char *) "->");

			}else if(dc_spd_button_press(DC_SPD_KEY_DOWN)){
				display.select -= 1;
				display.select = (display.select < 0) ? 1 : display.select;
				lcd_clearLine(&lcd, 0, 1, 2);
				lcd_clearLine(&lcd, 0, 1, 3);
				lcd_printf(&lcd, 0, display.select + 2, (char *)"->");

			}else if(dc_spd_button_press(DC_SPD_KEY_ENTER)){
				switch(display.select){
				case 0:
					dc_spd.conf.feedback = DC_SPD_FEEDBACK_TACHO;
					display.select = 0;
					display.input = dc_spd.conf.tacometer * 1000;
					display.screen = DC_SPD_LCD_FEEDACK_CONF;
					lcd_clear(&lcd);
					lcd_printf(&lcd, 0, 0, (char *)"--->DC SPEED<---    SETTING\r\nTachometer Conf\r\n  input:%.3f", (float)display.input / 1000);
					break;
				case 1:
					display.select = 0;
					dc_spd.conf.feedback = DC_SPD_FEEDBACK_NONE;
					//save data
					at24cxx_write(&eeprom, DC_SPD_EEPROM_CONF_ADDR, (uint8_t *)&dc_spd.conf, sizeof(DC_SPD_Conf_t));

					display.screen = DC_SPD_LCD_MAIN_MENU;
					dc_spd_lcd_main_menu();
					break;
				}
			}
			break;
		case DC_SPD_LCD_CONTROL:
			if(dc_spd_button_press(DC_SPD_KEY_MENU)){
				display.screen = DC_SPD_LCD_MAIN_MENU;
				display.select = 0;
				display.input = 0;
				dc_spd_lcd_main_menu();

			}else if(dc_spd_button_press(DC_SPD_KEY_UP)){
				display.select += 1;
				display.select = (display.select > 1) ? 0: display.select;
				lcd_clearLine(&lcd, 0, 1, 2);
				lcd_clearLine(&lcd, 0, 1, 3);
				lcd_printf(&lcd, 0,  display.select + 2,(char *) "->");

			}else if(dc_spd_button_press(DC_SPD_KEY_DOWN)){
				display.select -= 1;
				display.select = (display.select < 0) ? 1: display.select;
				lcd_clearLine(&lcd, 0, 1, 2);
				lcd_clearLine(&lcd, 0, 1, 3);
				lcd_printf(&lcd, 0, display.select + 2,(char *) "->");

			}else if(dc_spd_button_press(DC_SPD_KEY_ENTER)){
				display.screen = DC_SPD_LCD_MAIN_MENU;

				dc_spd.conf.control = display.select == 1 ? DC_SPD_CONTROL_KEYPAD : DC_SPD_CONTROL_POT;
				display.select = 0;

				dc_spd_lcd_main_menu();
				//todo save data
				at24cxx_write(&eeprom, DC_SPD_EEPROM_CONF_ADDR, (uint8_t *)&dc_spd.conf, sizeof(DC_SPD_Conf_t));
			}
			break;
		case DC_SPD_LCD_FEEDACK_CONF:
			if(dc_spd_button_press(DC_SPD_KEY_MENU) == 1){
				lcd_clear(&lcd);
				display.screen = DC_SPD_LCD_FEEDBACK;
				display.select = 0;
				display.input = 0;
				lcd_printf(&lcd, 0, 0,(char *) "--->DC SPEED<---FB:%s\r\n  Tachometer\r\n  No Tachometer", dc_spd.conf.feedback  == DC_SPD_FEEDBACK_TACHO ? "Tachometer" : "NO");
				lcd_printf(&lcd, 0, dc_spd.conf.feedback + 2,(char *) "->");

			}else if(dc_spd_button_press(DC_SPD_KEY_UP) || dc_spd_button_press_long(DC_SPD_KEY_UP)){
				display.input += 1;
				display.input = (display.input >= 1000) ? 0 : display.input;
				lcd_clearLine(&lcd, 8, 15, 3);
				lcd_printf(&lcd, 8, 3, (char *)"%.3f", (float )display.input /1000);
			}else if(dc_spd_button_press(DC_SPD_KEY_DOWN) || dc_spd_button_press_long(DC_SPD_KEY_DOWN)){
				display.input -= 1;
				display.input= (display.input < 0) ? 1000 : display.input;
				lcd_clearLine(&lcd, 8, 15, 3);
				lcd_printf(&lcd, 8, 3, (char *)"%.3f", (float )display.input /1000);
			}else if(dc_spd_button_press(DC_SPD_KEY_ENTER)){
				lcd_clear(&lcd);
				display.screen = DC_SPD_LCD_FEEDBACK;
				display.select = 0;
				dc_spd.conf.tacometer = (float) display.input / 1000;
				lcd_printf(&lcd, 0, 0,(char *) "--->DC SPEED<---FB:%s\r\n  Tachometer\r\n  No Tachometer", dc_spd.conf.feedback  == DC_SPD_FEEDBACK_TACHO ? "Tachometer" : "NO");
				lcd_printf(&lcd, 0, dc_spd.conf.feedback + 2,(char *) "->");
				//todo: confirm save data
				at24cxx_write(&eeprom, DC_SPD_EEPROM_CONF_ADDR, (uint8_t *)&dc_spd.conf, sizeof(DC_SPD_Conf_t));
			}
			break;
		}
		osDelay(20);
	}
	/* USER CODE END dc_spd_lcd_task_entry */
}

/*Function definition -----------------------------------------------------------------------------------------------------*/
static void dc_spd_lcd_main_menu(void){
	lcd_clear(&lcd);
	lcd_printf(&lcd, 0, 0,(char *)"--->DC SPEED<---");
	lcd_printf(&lcd, 0, 1, (char *) "    SETTING");
	lcd_printf(&lcd, 0, 2,(char *) "->%s", displayMenu[display.select]);
}

static void dc_spd_lcd_udpate_measurement(void){
	lcd_clearLine(&lcd, 12, 15, 1);
	lcd_printf(&lcd, 8, 1, (char *)"RPM:%d", dc_spd.rpmShow);
	/*Current and voltaje*/
	lcd_clearLine(&lcd, 11, 15, 2);
	lcd_printf(&lcd, 9, 2, (char *)"I:%.1f", dc_spd.I);
	lcd_clearLine(&lcd, 11,  15, 3);
	lcd_printf(&lcd, 9, 3, (char *)"V:%.1f", dc_spd.V);
	/*Reference*/
	lcd_clearLine(&lcd, 3, 6, 1);
	lcd_printf(&lcd, 3, 1, (char *)"%d", dc_spd.conf.feedback == DC_SPD_FEEDBACK_TACHO ?  (int)dc_spd.conf.ref : dc_spd.percentage);
}

#if LCD_USE_I2C_INTERFACE == 0
static void dc_spd_lcd_write_nb(uint8_t data, uint8_t control){
	uint32_t delay = 32;
	LCD_D7_GPIO_Port->BSRR |= ((data>>3) & 0x1) ? LCD_D7_Pin : LCD_D7_Pin<<16;
	LCD_D6_GPIO_Port->BSRR |= ((data>>2) & 0x1) ? LCD_D6_Pin : LCD_D6_Pin<<16;
	LCD_D5_GPIO_Port->BSRR |= ((data>>1) & 0x1) ? LCD_D5_Pin : LCD_D5_Pin<<16;
	LCD_D4_GPIO_Port->BSRR |= ((data) & 0x1) ? LCD_D4_Pin : LCD_D4_Pin<<16;

	LCD_E_GPIO_Port->BSRR |= LCD_E_Pin<<16;

	if(control){
		LCD_RS_GPIO_Port->BSRR |= LCD_RS_Pin;
	}else{
		LCD_RS_GPIO_Port->BSRR |= LCD_RS_Pin<<16;
	}

	LCD_E_GPIO_Port->BSRR |= LCD_E_Pin;
	while(delay-->0);
	LCD_E_GPIO_Port->BSRR |= LCD_E_Pin<<16;
	return;
}
#endif


uint8_t dc_spd_button_press(uint8_t button){
	if(xEventGroupWaitBits(dc_spd_button_events, 1<<button, pdTRUE, pdTRUE, 10) == pdPASS){
		return 1;
	}
	return 0;
}
uint8_t dc_spd_button_press_long(uint8_t button){
	if(xEventGroupWaitBits(dc_spd_button_events, 1<<(button + DC_SPD_MAX_BTN), pdTRUE, pdTRUE, 10) == pdPASS){
		return 1;
	}
	return 0;
}

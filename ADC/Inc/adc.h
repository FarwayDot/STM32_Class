/*
 * adc.h
 *
 *  Created on: Sep 2, 2025
 *      Author: User
 */

#ifndef ADC_H_
#define ADC_H_

/*Libraries*/
#include "stm32f4xx.h"
#include <stdint.h>

/*Defines*/
#define SINGLE_NO_SCAN			1
#define CONTINUOUS_NO_SCAN		2
#define SINGLE_SCAN				3
#define CONTINUOUS_SCAN			4
#define MODE					SINGLE_SCAN

/*Globa variables*/

//Alineación
typedef enum
{
	ADC1_Right_Align = 0,
	ADC1_Left_Align
}ADC1_Align_e;

//Modo de conversión
typedef enum
{
	ADC1_Single_Mode = 0,
	ADC1_Continuous_Mode
}ADC1_Conversion_Mode_e;

//Scan
typedef enum
{
	ADC1_Scan_Off = 0,
	ADC1_Scan_On
}ADC1_Scan_Mode_e;

//ADCCLK
typedef enum
{
	ADC1_PCLK_DIV2 = 0,
	ADC1_PCLK_DIV4,
	ADC1_PCLK_DIV6,
	ADC1_PCLK_DIV8
}ADC1_CLK_e;

typedef enum
{
	ADC1_Interrupt_Off = 0,
	ADC1_Interrupt_On
}ADC1_Interrupt_e;

typedef enum
{
	ADC1_At_Sequence = 0,
	ADC1_At_Regular
}ADC1_EOCS_e;

typedef enum
{
	ADC1_Res_12bit = 0,
	ADC1_Res_10bit,
	ADC1_Res_8bit,
	ADC1_Res_6bit
}ADC1_Resolution_e;

//Configuración Total
typedef struct
{
	ADC1_Resolution_e resolution;
	ADC1_Align_e align_mode;
	ADC1_CLK_e clk_div;
	ADC1_Conversion_Mode_e conversion_mode;
	ADC1_Scan_Mode_e scan_mode;
	ADC1_EOCS_e eocs_var;
	ADC1_Interrupt_e adc_interrupt;
}ADC1_Params_t;

/*Prototype Function*/
void adc1_config(ADC1_Params_t *adc1_params);
void adc1_start_conv(void);
uint16_t adc1_read_data_Polling(void);

#endif /* ADC_H_ */

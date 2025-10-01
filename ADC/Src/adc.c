/*
 * adc.c
 *
 *  Created on: Sep 2, 2025
 *      Author: User
 */

#include "adc.h"

/*
 * @brief Configuración del canal ADC1 STM32F401
 *
 * Esta función habilita el clock del adc y configura el ADCCLK, Modo Single-Continuo,
 * interrupcion, alineación, EOCS y habilita ADC.
 *
 * @param	*adc1_params	Puntero a una estructura de datos que contiene las directrices
 * 							para la configuración del ADC1
 *
 */
void adc1_config(ADC1_Params_t *adc1_params)
{
	//Configuración Incial
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; //Habilitamos el periférico
	ADC1->CR2 &= ~ADC_CR2_ADON_Msk; //Apagamos ADC
	ADC1->CR1 &= ~ADC_CR1_EOCIE_Msk; //Desactivamos interrupcion
	ADC1->SR = 0; //Limpiamos flags

	//Resolution
	ADC1->CR1 &= ~ADC_CR1_RES_Msk;
	ADC1->CR1 |= (adc1_params->resolution<<ADC_CR1_RES_Pos);

	//Alignment
	ADC1->CR2 &= ~ADC_CR2_ALIGN_Msk; //Clear
	ADC1->CR2 |= (adc1_params->align_mode<<ADC_CR2_ALIGN_Pos);

	//Configurar ADCCLK
	ADC1_COMMON->CCR &= ~ADC_CCR_ADCPRE_Msk; //Clear
	ADC1_COMMON->CCR |= (adc1_params->clk_div<<ADC_CCR_ADCPRE_Pos);

	//Modo de conversión
	ADC1->CR2 &= ~ADC_CR2_CONT_Msk; //Clear
	ADC1->CR2 |= (adc1_params->conversion_mode<<ADC_CR2_CONT_Pos);

	//Scan
	ADC1->CR1 &= ~ADC_CR1_SCAN_Msk; //Clear
	ADC1->CR1 |= (adc1_params->scan_mode<<ADC_CR1_SCAN_Pos);

	//EOC se activa al final de cada secuencia regular finalizada.
	ADC1->CR2 &= ~ADC_CR2_EOCS_Msk; //Clear
	ADC1->CR2 |= (adc1_params->eocs_var<<ADC_CR2_EOCS_Pos);

	//Encendemos ADC
	ADC1->CR2 |= ADC_CR2_ADON;
}

void adc1_start_conversion(void)
{
	//Start regular channel
	ADC1->CR2 |= ADC_CR2_SWSTART;
}

uint16_t adc1_read_data_Polling(void)
{
	//Iniciamos conversión
	adc1_start_conversion();

	//Esperemos a que el flag se active
	while(!(ADC1->SR & ADC_SR_EOC));

	//Retornamos valor
	return ((uint16_t)(ADC1->DR & 0x0000FFFF));
}



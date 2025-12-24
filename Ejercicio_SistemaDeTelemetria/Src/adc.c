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
	uint8_t interr = adc1_params->adc_interrupt;
	uint8_t dma_en = adc1_params->adc_dma;
	uint8_t scan_on = adc1_params->scan_mode;

	//Configuración Incial
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; //Habilitamos el periférico
	ADC1->CR2 &= ~ADC_CR2_ADON_Msk; //Apagamos ADC
	ADC1->CR1 &= ~ADC_CR1_EOCIE_Msk; //Desactivamos interrupcion
	ADC1->SR = 0; //Limpiamos flags

	//Resolution
	ADC1->CR1 &= ~ADC_CR1_RES_Msk;
	ADC1->CR1 |= ((uint32_t)adc1_params->resolution<<ADC_CR1_RES_Pos);

	//Alignment
	ADC1->CR2 &= ~ADC_CR2_ALIGN_Msk; //Clear
	ADC1->CR2 |= ((uint32_t)adc1_params->align_mode<<ADC_CR2_ALIGN_Pos);

	//Configurar ADCCLK
	ADC1_COMMON->CCR &= ~ADC_CCR_ADCPRE_Msk; //Clear
	ADC1_COMMON->CCR |= ((uint32_t)adc1_params->clk_div<<ADC_CCR_ADCPRE_Pos);

	//Modo de conversión
	ADC1->CR2 &= ~ADC_CR2_CONT_Msk; //Clear
	ADC1->CR2 |= ((uint32_t)adc1_params->conversion_mode<<ADC_CR2_CONT_Pos);

	//Scan
	ADC1->CR1 &= ~ADC_CR1_SCAN_Msk; //Clear
	ADC1->CR1 |= (1U<<ADC_CR1_SCAN_Pos);

	if(scan_on)
	{
		Sequence_config(adc1_params);
	}

	//EOC se activa al final de cada secuencia regular finalizada.
	ADC1->CR2 &= ~ADC_CR2_EOCS_Msk; //Clear
	ADC1->CR2 |= ((uint32_t)adc1_params->eocs_var<<ADC_CR2_EOCS_Pos);

	//Interrupt and DMA
	if(interr & !dma_en) //No se activa Interrup con DMA activado
	{
		ADC1->CR1 |= ((uint32_t)adc1_params->adc_interrupt<<ADC_CR1_EOCIE_Pos);
		NVIC_EnableIRQ(ADC_IRQn);
	}
	else if(dma_en)
	{
		ADC1->CR2 |= (1U<<ADC_CR2_DMA_Pos); //DMA request for hardware
		ADC1->CR2 |= (1U<<ADC_CR2_DDS_Pos); //DMA request vuelve luego de la conversión
	}

	//Encendemos ADC
	ADC1->CR2 |= ADC_CR2_ADON;

}

/*
 * @brief Iniciar conversión regular del ADC1
 *
 */
void adc1_start_regular_conversion(void)
{
	//Start regular channel
	ADC1->CR2 |= ADC_CR2_SWSTART;
}

/*
 * @brief Leer datos cada vez que se termina la conversión del ADC
 *
 */
uint16_t adc1_read_regular(void)
{
	//Iniciamos conversión
	adc1_start_regular_conversion();

	//Esperemos a que el flag se active
	while(!(ADC1->SR & ADC_SR_EOC));

	//Retornamos valor
	return ((uint16_t)(ADC1->DR & 0x0000FFFF));
}

/*
 * @brief Se configura los registros asociados a la secuencia
 */

void Sequence_config(ADC1_Params_t *adc1_params)
{
	uint8_t lenght = adc1_params->seq_config.seq_lenght;
	uint8_t idx = 0;


	ADC1->SQR1 = 0;
	ADC1->SQR2 = 0;
	ADC1->SQR3 = 0;

	//Configuramos longitud
	if(lenght>0 && lenght<=16)
	{
		ADC1->SQR1 |= (uint32_t)(lenght-1)<<ADC_SQR1_L_Pos;
	}

	//Configuramos secuencia


	//Primeros 6 elementos
	for(uint8_t i=0; (i<30) & (idx<lenght); i+=5) //Condición para evitar desbordamiento en array
	{
		idx = (uint8_t)(i/5.0);
		ADC1->SQR3 |= (uint32_t)adc1_params->seq_config.seq[idx]<<i;
	}

	idx = 0; //reinicio idx
	for(uint8_t i=0; (i<30) & (idx<lenght); i+=5)
	{
		idx = (uint8_t)(i/5.0) + 6; //Debe empezar en 6 (7to elemento) hasta el 11 (12vo elemento)
		ADC1->SQR2 |= (uint32_t)adc1_params->seq_config.seq[idx]<<i;
	}

	idx = 0; //reinicio idx
	for(uint8_t i=0; (i<20) & (idx<lenght); i+=5)
	{
		idx = (uint8_t)(i/5.0) + 12; //Debe empezar en 6 (7to elemento)
		ADC1->SQR1 |= (uint32_t)adc1_params->seq_config.seq[idx]<<i;
	}


}

void adc1_enable(void)
{
	//Habilitamos ADC
	ADC1->CR2 |= ADC_CR2_ADON;
	return;
}

/*
 * @brief Disable ADC by ADON bit
 */
void adc1_disable(void)
{
	//Deshabilitamos ADC
	ADC1->CR2 &= ~ADC_CR2_ADON_Msk;
	return;
}

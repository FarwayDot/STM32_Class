/*
 * spi.c
 *
 *  Created on: Jan 10, 2026
 *      Author: User
 */

/*Inclusiónd de librerías*/

#include "spi.h"
#include "GPIO_Config.h"

/*Definición de funciones*/

/*
 * @brief GPIO Config
 *
 * COnfiguración de los pines GPIO para funcionar como SPI1
 */
void SPI_GPIO_Config(void)
{

	//SOLO SPI1
	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIOAEN; //Clock

	//GPIO_AF_Config necesita aparte la configuración del registro AFR

	//GPIOA PIN5 SCK 5
	GPIO_AF_Config(GPIO_SPI1_SCK, PIN_SPI1_SCK, PUPDR_NONE, OSPEEDR_VERYHIGH, OTYPER_PP);
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL5_Msk;
	GPIOA->AFR[0] |= 5U<<GPIO_AFRL_AFSEL5_Pos; //AF5 LOW

	//GPIOA PIN6 MISO 6
	GPIO_AF_Config(GPIO_SPI1_MISO, PIN_SPI1_MISO, PUPDR_NONE, OSPEEDR_VERYHIGH, OTYPER_PP);
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL6_Msk;
	GPIOA->AFR[0] |= 5U<<GPIO_AFRL_AFSEL6_Pos; //AF5 LOW

	//GPIOA PIN7 MOSI 7
	GPIO_AF_Config(GPIO_SPI1_MOSI, PIN_SPI1_MOSI, PUPDR_NONE, OSPEEDR_VERYHIGH, OTYPER_PP);
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL7_Msk;
	GPIOA->AFR[0] |= 5U<<GPIO_AFRL_AFSEL7_Pos; //AF5 LOW

	return;
}

/*
 * @brief Inicialización del periférico SPI
 */
void SPI_Init(SPI_TypeDef *SPIx)
{

	//RCC
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	//Config GPIO
	SPI_GPIO_Config();

	//BaudRate
	SPIx->CR1 &= ~SPI_CR1_BR_Msk;
	SPIx->CR1 |= 7U<<SPI_CR1_BR_Pos; //fpclk/32 , fpclk = 84Mhz

	//CPOL y CPHA
	SPIx->CR1 &= ~(SPI_CR1_CPOL_Msk | SPI_CR1_CPHA_Msk); //Modo 0
	//SPIx->CR1 |= 1U<<SPI_CR1_CPOL_Pos;
	//SPIx->CR1 |= 1U<<SPI_CR1_CPHA_Pos;

	//Seleccionar la longitud de la trama
	SPIx->CR1 &= ~SPI_CR1_DFF_Msk; //8bits

	//Configurar MSB o LSB first
	SPIx->CR1 &= ~SPI_CR1_LSBFIRST_Msk;
	//SPIx->CR1 |= 1U<<SPI_CR1_LSBFIRST_Pos;

	//Configurar el manejo del pin NSS
	SPIx->CR1 |= 1U<<SPI_CR1_SSM_Pos; //Manejado por software
	SPIx->CR1 |= 1U<<SPI_CR1_SSI_Pos; //Cuando SSM es 1 este es ahora el nuevo NSS inter no para nuestro SPI

	//Configurar el modo TI si es que fuera necesario

	//Habilitar el modo maestro
	SPIx->CR1 |= 1U<<SPI_CR1_MSTR_Pos;

	//Habilita SPI par Tx y Rx
	SPIx->CR1 |= 1U<<SPI_CR1_SPE_Pos;

	return;
}

/*
 * @brief Envío de datos por SPI No Interrupt
 *
 * SPI_TypeDef *SPIx: Puntero al SPI que queremos usar
 * uint8_t *tx_buffer: Data
 * uint8_t len: longitud de la data
 */
void SPI_Send_Data(SPI_TypeDef *SPIx, uint8_t *pTx_buffer, uint32_t len)
{

	while(len>0)
	{
		//Verificar si el registro está vacío
		while(!(SPIx->SR & SPI_SR_TXE));

		if(SPIx->CR1 & SPI_CR1_DFF) // 16bits
		{
			SPIx->DR = *((uint16_t *)pTx_buffer); //Primero se castea a puntero de 16bits, luego se obtiene el contenido
			pTx_buffer+=2;							 //con el operador indirección '*'
			len -= 2;
		}
		else //8bits
		{
			//Cargar data
			*((__IO uint8_t *)&SPIx->DR) = (*pTx_buffer);
			pTx_buffer++;
			len--;
		}
	}

	//Verificar TX
	while(!(SPIx->SR & SPI_SR_TXE));

	//Verificar que el SPI esté libre
	while(SPIx->SR & SPI_SR_BSY);

	return;
}

/*
 * @brief Transmisión y Recepción de datos por SPI No Interrupt
 *
 * SPI_TypeDef *SPIx: Puntero al SPI que queremos usar
 * uint8_t *pTx_buffer: Data
 * uint8_t *pRx_buffer: Data
 * uint8_t len: longitud de la data
 */
void SPI_Send_Receive_Data(SPI_TypeDef *SPIx, uint8_t *pTx_buffer, uint8_t *pRx_buffer, uint32_t len)
{
	while(len>0)
	{
		//Verificar si el registro está vacío
		while(!(SPIx->SR & SPI_SR_TXE));

		if(SPIx->CR1 & SPI_CR1_DFF) // 16bits
		{
			SPIx->DR = *((uint16_t *)pTx_buffer); //Primero se castea a puntero de 16bits, luego se obtiene el contenido
												 //con el operador indirección '*'

			while(!(SPIx->SR & SPI_SR_RXNE)); //Se espera a recibir
			*((uint16_t *)pRx_buffer) = SPIx->DR; //Se guarda el dato recibido

			len -= 2;
			(uint16_t *)pTx_buffer++;
			(uint16_t *)pRx_buffer++;
		}
		else //8bits
		{
			//Cargar data
			SPIx->DR = (*pTx_buffer) & 0xFF;

			while(!(SPIx->SR & SPI_SR_RXNE)); //Se espera a recibir
			*pRx_buffer = SPIx->DR & 0xFF; //Se guarda el dato recibido

			pTx_buffer++;
			pRx_buffer++;
			len--;
		}

	}

	//Verificar que el SPI esté libre
	while(SPIx->SR & SPI_SR_BSY);

	return;
}

/*
 * @brief Recepción de datos por SPI No Interrupt enEsclavo
 *
 * SPI_TypeDef *SPIx: Puntero al SPI que queremos usar
 * uint8_t *pRx_buffer: Data
 * uint8_t len: longitud de la data
 */
void SPI_Receive_Data(SPI_TypeDef *SPIx, uint8_t *pRx_buffer, uint32_t len)
{
	while(len>0)
	{
		//Esperamos a que la bandera de recepción está apagada
		while(!(SPIx->SR & SPI_SR_RXNE));

		if(SPIx->CR1 & SPI_CR1_DFF) // 16bits
		{
			*((uint16_t *)pRx_buffer) = SPIx->DR; //Se guarda el dato recibido

			len -= 2;
			(uint16_t *)pRx_buffer++;
		}
		else //8bits
		{
			*pRx_buffer = SPIx->DR & 0xFF; //Se guarda el dato recibido
			pRx_buffer++;
			len--;
		}
	}

	//Verificar que el SPI esté libre
	while(SPIx->SR & SPI_SR_BSY);

	return;
}

/*
 * @brief Recepción de datos por SPI No Interrupt en Maestro ya que aquí generamos la señal de reloj
 *
 * SPI_TypeDef *SPIx: Puntero al SPI que queremos usar
 * uint8_t *pRx_buffer: Data
 * uint8_t len: longitud de la data
 */
void SPI_Master_Receive_Data(SPI_TypeDef *SPIx, uint8_t *pRx_buffer, uint32_t len)
{
	while(len>0)
	{
		//Verificar si el registro Tx está vacío
		while(!(SPIx->SR & SPI_SR_TXE));

		if(SPIx->CR1 & SPI_CR1_DFF) // 16bits
		{
			//Ya que el master proporciona el CLOCK, este debe generarlo enviando un byte
			SPIx->DR = (uint16_t)0xFFFF;

			while(!(SPIx->SR & SPI_SR_RXNE)); //Se espera a recibir
			*((uint16_t *)pRx_buffer) = SPIx->DR; //Se guarda el dato recibido

			len -= 2;
			(uint16_t *)pRx_buffer++;
		}
		else //8bits
		{
			//Ya que el master proporciona el CLOCK, este debe generarlo enviando un byte
			SPIx->DR = (uint8_t)0xFF;
			while(!(SPIx->SR & SPI_SR_RXNE)); //Se espera a recibir
			*pRx_buffer = SPIx->DR & 0xFF; //Se guarda el dato recibido
			pRx_buffer++;
			len--;
		}
	}

	//Verificar que el SPI esté libre
	while(SPIx->SR & SPI_SR_BSY);

	return;

}

/*
 * @brief Enviar un byte de datos para pruebas
 *
 * SPI_TypeDef *SPIx: Puntero al SPI que queremos usar
 * uint8_t hex_data: Byte de datos a enviar
 *
 */
void SPI_Send_Byte(SPI_TypeDef *SPIx, uint8_t hex_data)
{

	//Verificar si el registro está vacío
	while(!(SPIx->SR & SPI_SR_TXE));

	//Cargar data
	*((__IO uint8_t *)&SPIx->DR) = hex_data;

	//Verificar TX
	while(!(SPIx->SR & SPI_SR_TXE));

	//Verificar que el SPI esté libre
	while(SPIx->SR & SPI_SR_BSY);

	return;
}


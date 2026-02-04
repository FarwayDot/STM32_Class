/*
 * spi.h
 *
 *  Created on: Jan 10, 2026
 *      Author: User
 */

#ifndef SPI_H_
#define SPI_H_

/*Inclusiónd de librerías*/

#include "stdint.h"
#include "stm32f4xx.h"

/*Definiciones*/

#define PIN_SPI1_SCK	5		//SCK
#define GPIO_SPI1_SCK	GPIOA
#define PIN_SPI1_MISO	6		//MISO
#define GPIO_SPI1_MISO	GPIOA
#define PIN_SPI1_MOSI	7		//MOSI
#define GPIO_SPI1_MOSI	GPIOA

/*Prototipo de funciones*/

void SPI_GPIO_Config(void);
void SPI_Init(SPI_TypeDef *SPIx);
void SPI_Send_Data(SPI_TypeDef *SPIx, uint8_t *pTx_buffer, uint32_t len);
void SPI_Send_Receive_Data(SPI_TypeDef *SPIx, uint8_t *pTx_buffer, uint8_t *pRx_buffer, uint32_t len);
void SPI_Receive_Data(SPI_TypeDef *SPIx, uint8_t *pRx_buffer, uint32_t len);
void SPI_Master_Receive_Data(SPI_TypeDef *SPIx, uint8_t *pRx_buffer, uint32_t len);
void SPI_Send_Byte(SPI_TypeDef *SPIx, uint8_t hex_data);

#endif /* SPI_H_ */

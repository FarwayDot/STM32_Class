/*
 * USART.h
 *
 *  Created on: Jan 24, 2025
 *      Author: User
 */

#ifndef USART_H_
#define USART_H_

/*
 * Librerías
 */
#include "stm32f4xx.h"
#include "Config.h"
#include <stdint.h>
#include <stdlib.h>


/*
 * Definiciones
 */
#define USART2_Pin_Tx	A, 2
#define USART2_Pin_Rx	A, 3

#define M_8BITS 	0
#define M_9BITS		1

#define OVER_16		0
#define OVER_8		1

#define STOP_ONE	0
#define STOP_HALF	1
#define STOP_TWO	2
#define STOP_ONE_HALF 3

#define PARITY_DIS	0
#define PARITY_EN	1

#define PAR_EVEN	0
#define PAR_ODD		1

/*
 * State
 */

#define USART_BUSY_IN_RX 			1
#define USART_BUSY_IN_TX 			2
#define USART_READY 				0
#define USART_EVENT_TX_CMPLT   		0
#define	USART_EVENT_RX_CMPLT   		1
#define	USART_EVENT_IDLE      		2
#define	USART_EVENT_CTS       		3
#define	USART_EVENT_PE        		4
#define	USART_ERR_FE     			5
#define	USART_ERR_NE    	 		6
#define	USART_ERR_ORE    			7

/*
 * Estructuras y enumeraciones
 */

typedef struct
{
	USART_TypeDef*	USARTx;
	uint32_t 		baudrate;
	uint8_t 		m_bit;
	uint8_t			over_bit;
	uint8_t			stop_bit;
	uint8_t 		enable_parity;
	uint8_t 		parity_bit;
	uint8_t			priority;
}USARTx_Config_t;

typedef struct
{
	USART_TypeDef *pUSARTx;
	uint8_t *pTxBuffer;
	uint8_t *pRxBuffer;
	uint32_t TxLen;
	uint32_t RxLen;
	uint8_t TxBusyState;
	uint8_t RxBusyState;
}USARTx_Handle_t;

/*
 * Variables
 */
//const uint8_t APBPrescTable[8]  = {0, 0, 0, 0, 1, 2, 3, 4};


/*
 * DEFINIR EL OVERSAMPLING
 */
#define OVERSAMPLING16 		0
#define OVERSAMPLING8  		1

/**
 * Calculo de BRR
 */
#define UART_DIV_SAMPLING16(_PCLK_, _BAUD_)            ((uint32_t)((((uint64_t)(_PCLK_))*25U)/(4U*((uint64_t)(_BAUD_)))))
#define UART_DIVMANT_SAMPLING16(_PCLK_, _BAUD_)        (UART_DIV_SAMPLING16((_PCLK_), (_BAUD_))/100U)
#define UART_DIVFRAQ_SAMPLING16(_PCLK_, _BAUD_)        ((((UART_DIV_SAMPLING16((_PCLK_), (_BAUD_)) - (UART_DIVMANT_SAMPLING16((_PCLK_), (_BAUD_)) * 100U)) * 16U)\
                                                         + 50U) / 100U)
/* UART BRR = mantissa + overflow + fraction
            = (UART DIVMANT << 4) + (UART DIVFRAQ & 0xF0) + (UART DIVFRAQ & 0x0FU) */
#define UART_BRR_SAMPLING16(_PCLK_, _BAUD_)            ((UART_DIVMANT_SAMPLING16((_PCLK_), (_BAUD_)) << 4U) + \
                                                        (UART_DIVFRAQ_SAMPLING16((_PCLK_), (_BAUD_)) & 0xF0U) + \
                                                        (UART_DIVFRAQ_SAMPLING16((_PCLK_), (_BAUD_)) & 0x0FU))

#define UART_DIV_SAMPLING8(_PCLK_, _BAUD_)             ((uint32_t)((((uint64_t)(_PCLK_))*25U)/(2U*((uint64_t)(_BAUD_)))))
#define UART_DIVMANT_SAMPLING8(_PCLK_, _BAUD_)         (UART_DIV_SAMPLING8((_PCLK_), (_BAUD_))/100U)
#define UART_DIVFRAQ_SAMPLING8(_PCLK_, _BAUD_)         ((((UART_DIV_SAMPLING8((_PCLK_), (_BAUD_)) - (UART_DIVMANT_SAMPLING8((_PCLK_), (_BAUD_)) * 100U)) * 8U)\
                                                         + 50U) / 100U)
/* UART BRR = mantissa + overflow + fraction
            = (UART DIVMANT << 4) + ((UART DIVFRAQ & 0xF8) << 1) + (UART DIVFRAQ & 0x07U) */
#define UART_BRR_SAMPLING8(_PCLK_, _BAUD_)             ((UART_DIVMANT_SAMPLING8((_PCLK_), (_BAUD_)) << 4U) + \
                                                        ((UART_DIVFRAQ_SAMPLING8((_PCLK_), (_BAUD_)) & 0xF8U) << 1U) + \
                                                        (UART_DIVFRAQ_SAMPLING8((_PCLK_), (_BAUD_)) & 0x07U))

/*Prototipo de funciones*/

void USART_Init(USART_TypeDef *USARTx, uint32_t baud); //1bit start, stop, sin paridad
void USART_Send_Byte(USART_TypeDef *USARTx, uint8_t Byte);
void USART_Send_Data(USART_TypeDef *USARTx, uint8_t *Data, uint32_t size);
void USART_Receive_Data(USART_TypeDef *USARTx, uint8_t *Data, uint32_t size);

/*New Functions*/
uint32_t HAL_RCC_GetPCLK1Freq(void);
uint32_t HAL_RCC_GetPCLK2Freq(void);
void USARTx_Init(USARTx_Config_t* USARTx_Config);
void USARTx_Init_IT(USARTx_Config_t* USARTx_Config);
void USARTx_Send_Data(USART_TypeDef *USARTx, uint8_t *pTxSend, uint32_t size);
void USARTx_Receive_Data(USART_TypeDef *USARTx, uint8_t *pRxReceive, uint32_t size);
uint8_t USARTx_SendDataIT(USARTx_Handle_t *pUSARTHandle,uint8_t *pTxBuffer, uint32_t size);
uint8_t USARTx_ReceiveDataIT(USARTx_Handle_t *pUSARTHandle,uint8_t *pRxBuffer, uint32_t size);
void USARTx_IRQHandling(USARTx_Handle_t *pUSARTHandle);


#endif /* USART_H_ */

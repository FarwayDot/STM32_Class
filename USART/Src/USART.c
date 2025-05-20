/*
 * USART.c
 *
 *  Created on: Jan 24, 2025
 *      Author: User
 */


#include "USART.h"

/*
 * @brief: Configuración de pines USART1
 */
static void USART1_Pin_Configuration(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; //Clock

	GPIOA -> MODER &= ~(GPIO_MODER_MODE10_Msk | GPIO_MODER_MODE9_Msk); //Clean GPIO Config
	GPIOA -> MODER |= (GPIO_MODER_MODE10_1 | GPIO_MODER_MODE9_1); //Alternate GPIO Config

	GPIOA -> OSPEEDR  &= ~(GPIO_OSPEEDR_OSPEED10_Msk | GPIO_OSPEEDR_OSPEED9_Msk); //Clean GPIO Config OSPEED
	GPIOA -> OSPEEDR |= ((0x03<<GPIO_OSPEEDR_OSPEED10_Pos) | (0x03<<GPIO_OSPEEDR_OSPEED9_Pos)); //GPIO Config OSPEED

	GPIOA -> PUPDR  &= ~(GPIO_PUPDR_PUPD10_Msk | GPIO_PUPDR_PUPD9_Msk); //Clean GPIO Config OSPEED
	GPIOA -> PUPDR |= ((0x01<<GPIO_PUPDR_PUPD10_Pos) | (0x01<<GPIO_PUPDR_PUPD9_Pos)); //GPIO Config OSPEED

	GPIOA -> AFR[1] &= ~(GPIO_AFRH_AFSEL10_Msk | GPIO_AFRH_AFSEL9_Msk); //Clean GPIO AF
	GPIOA -> AFR[1] |= ((0x07<<GPIO_AFRH_AFSEL10_Pos) | (0x07<<GPIO_AFRH_AFSEL9_Pos)); //Config GPIO AF

}

/*
 * @brief: Configuración de pines USART2
 */
static void USART2_Pin_Configuration(void)
{
	RCC-> AHB1ENR |= GPIOX_CLOCK(USART2_Pin_Tx);
	RCC-> AHB1ENR |= GPIOX_CLOCK(USART2_Pin_Rx);

	GPIOX_MODER(MODE_ALTER, USART2_Pin_Tx);
	GPIOX_MODER(MODE_ALTER, USART2_Pin_Rx);

	GPIOX_AFR(7U,USART2_Pin_Tx);
	GPIOX_AFR(7U,USART2_Pin_Rx);

	GPIOX_OSPEEDR(MODE_SPD_VHIGH, USART2_Pin_Tx);
	GPIOX_OSPEEDR(MODE_SPD_VHIGH, USART2_Pin_Rx);

	GPIOX_PUPDR(MODE_PU_UP, USART2_Pin_Tx);
	GPIOX_PUPDR(MODE_PU_UP, USART2_Pin_Rx);
}



/*
 * @brief: Configuración básica del módulo USART
 *
 * 1 bit STOP
 * Sin paridad
 * 8 bits de datos
 *
 */
void USART_Init(USART_TypeDef *USARTx, uint32_t baud) //1bit start, stop, sin paridad
{
	/*Habilitar Relojs*/
	if(USARTx == USART1)
	{
		RCC->APB2ENR |=  RCC_APB2RSTR_USART1RST; //(1<<4)
		/*Configurar Pines*/
		USART1_Pin_Configuration();
	}
	else if(USARTx == USART2)
	{
		RCC->APB1ENR |=  RCC_APB1RSTR_USART2RST; //(1<<17)
		/*Configurar Pines*/
		USART2_Pin_Configuration();
	}

	/* Configurar los parámetros de comunicación*/

	USARTx -> CR1 &= ~(USART_CR1_M); //Configurar bits de transmisión

	USARTx -> CR1 &= ~(USART_CR1_OVER8_Msk); //Oversampling16

	USARTx -> BRR = UART_BRR_SAMPLING16(SystemCoreClock, baud);//Configurar baudios (Tener cuidado con la frecuencia de reloj ya que USART1 y USART2 trabajan a frecuencias máximas diferentes)

	USARTx -> CR1 |= USART_CR1_TE | USART_CR1_RE; //Habilita Transmisión y recepción

	USARTx -> CR2 &= ~(USART_CR2_STOP_Msk); //1 bits de stop

	USARTx -> CR1 &= ~(USART_CR1_PCE); //Sin paridad

	/*Configuración de interrupciones*/

	/*Habilitar USART*/
	USARTx -> CR1 |= USART_CR1_UE;
}


/*
 * @brief: Enviar 1 byte a través del USART
 */
void USART_Send_Byte(USART_TypeDef *USARTx, uint8_t Byte)
{
	/*Verify Buffer Empty*/
	while(!(USARTx->SR & USART_SR_TXE));

	/*Place Data into Buffer register*/
	USARTx -> DR = Byte;

	/*Verify transmision complete*/
	while(!(USARTx->SR & USART_SR_TC));
}

/*
 * @brief: Enviar N bytes a través del USART
 */
void USART_Send_Data(USART_TypeDef *USARTx, uint8_t *Data, uint32_t size)
{
	uint16_t *pdata;

	for(uint32_t i=0; i<size; i++)
	{
		while(!(USARTx->SR & USART_SR_TXE)); //Verificar el flag TXE

		//Verificar si la transmisión de 8 bits o 9 bits
		if(USARTx->CR1 & USART_CR1_M) //9 bits
		{
			//Enviar dato
			pdata = (uint16_t*)Data; //Esto para utilizar pdata, por eso ahora apunta a la dirección base de Data;
			USARTx -> DR = (*pdata & (uint16_t)0x1FF); //Se escriben los 9bits

			//Revisar paridad
			if(!(USARTx->CR1 & USART_CR1_PCE)) //Si no hay paridad
			{
				Data += 2;
			}
			else //Si hay paridad
			{
				Data++;
			}
		}
		else //8 bits
		{
			USARTx -> DR = (*Data & (uint8_t)0xFF);
			Data++;
		}
	}

	//Todo se va transmitiendo y llegara aca cuando SIZE se acabe, luego, entra al while y automáticamente indica
	//que se ha terminado la transmisión de data.
	/*Esperamos a que la tranmisión de complete*/
	while(!(USARTx->SR & USART_SR_TC));

	return;
}

/*
 * @brief: Recivir N bytes a través del USART
 */
void USART_Receive_Data(USART_TypeDef *USARTx, uint8_t *Data, uint32_t size)
{
	for(uint32_t i=0; i<size; i++)
	{
		while(!(USARTx->SR & USART_SR_RXNE)); //Verificar el flag RXE

		//Verificar si la transmisión de 8 bits o 9 bits
		if(USARTx->CR1 & USART_CR1_M) //9 bits
		{

			//Revisar paridad
			if(!(USARTx->CR1 & USART_CR1_PCE)) //Si no hay paridad
			{
				*((uint16_t*)Data) = USARTx->DR & (uint16_t)0x1FF;
				Data+=2;
			}
			else //Si hay paridad
			{
				*Data= USARTx->DR & (uint8_t)0xFF;
				Data++;
			}
		}
		else
		{
			//Revisar paridad
			if(!(USARTx->CR1 & USART_CR1_PCE)) //Si no hay paridad
			{
				*Data= USARTx->DR & (uint8_t)0xFF;
			}
			else //Si hay paridad
			{
				*Data= USARTx->DR & (uint8_t)0x7F;
			}

			Data++;
		}

	}
}

/*
 * NEW FUNCTIONS TO USART
 */

uint32_t HAL_RCC_GetPCLK1Freq(void)
{
	return (SystemCoreClock >> APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE1) >> RCC_CFGR_PPRE1_Pos]);
}

uint32_t HAL_RCC_GetPCLK2Freq(void)
{
	return (SystemCoreClock >> APBPrescTable[(RCC->CFGR & RCC_CFGR_PPRE2) >> RCC_CFGR_PPRE2_Pos]);
}

void USARTx_Init(USARTx_Config_t* USARTx_Config)
{
	uint32_t baud = USARTx_Config -> baudrate;
	uint32_t pclk;
	/*Habilitar Relojs*/
	if(USARTx_Config -> USARTx == USART1)
	{
		RCC->APB2ENR |=  RCC_APB2RSTR_USART1RST;
		/*Configurar Pines*/
		USART1_Pin_Configuration();
		pclk = HAL_RCC_GetPCLK2Freq();
	}
	else if(USARTx_Config -> USARTx == USART2)
	{
		RCC->APB1ENR |=  RCC_APB1RSTR_USART2RST;
		/*Configurar Pines*/
		USART2_Pin_Configuration();
		pclk = HAL_RCC_GetPCLK1Freq();
	}

	/*M bits*/
	if(USARTx_Config -> m_bit != M_8BITS)
	{
		USARTx_Config -> USARTx -> CR1 |= USART_CR1_M;
	}

	/* Over Sampling and Baud Rate*/
	if(USARTx_Config -> over_bit != OVER_16)
	{
		USARTx_Config -> USARTx-> CR1 |= USART_CR1_OVER8;
		USARTx_Config -> USARTx -> BRR = UART_BRR_SAMPLING8(pclk, baud);
	}
	else
	{
		USARTx_Config -> USARTx-> CR1 &= ~USART_CR1_OVER8_Msk;
		USARTx_Config -> USARTx -> BRR = UART_BRR_SAMPLING16(pclk, baud);
	}

	/* Stop bits*/
	if((USARTx_Config -> stop_bit) ==  STOP_ONE)
	{
		//Nothing
		USARTx_Config -> USARTx -> CR2 |= USART_CR2_STOP_Msk;

	}
	else if((USARTx_Config -> stop_bit) ==  STOP_HALF)
	{
		USARTx_Config -> USARTx -> CR2 |= USART_CR2_STOP_0;
	}
	else
	{
		USARTx_Config -> USARTx -> CR2 |= USART_CR2_STOP_1;
	}

	/* Paridad */
	if(USARTx_Config -> enable_parity != PARITY_DIS)
	{
		USARTx_Config -> USARTx -> CR2 |= USART_CR1_PCE;

		if(USARTx_Config -> parity_bit != PAR_EVEN )
		{
			USARTx_Config -> USARTx -> CR1 |= USART_CR1_PS;
		}
	}

	/* Enable transmisión y recepción */
	USARTx_Config -> USARTx -> CR1 |= USART_CR1_TE | USART_CR1_RE;

	/*Configuración de interrupciones*/

	/*Habilitar USART*/
	USARTx_Config -> USARTx -> CR1 |= USART_CR1_UE;
}

void USARTx_Init_IT(USARTx_Config_t* USARTx_Config)
{
	uint8_t prior =  USARTx_Config -> priority;

	if(USARTx_Config -> USARTx == USART1)
	{
		NVIC_EnableIRQ(USART1_IRQn);
		NVIC_SetPriority(USART1_IRQn, prior);
	}
	else if(USARTx_Config -> USARTx == USART2)
	{
		NVIC_EnableIRQ(USART2_IRQn);
		NVIC_SetPriority(USART2_IRQn, prior);
	}
}

void USARTx_Send_Data(USART_TypeDef *USARTx, uint8_t *pTxSend, uint32_t size)
{

	uint16_t *pTxBuffer;

	/*Durante el size de la data*/
	for(uint8_t i=0; i<size; i++)
	{
		/*Mientras no está vacío, se queda acá*/
		while(!(USARTx -> SR &  USART_SR_TXE));

		/*9 bits*/
		if(USARTx->CR1 & USART_CR1_M)
		{
			pTxBuffer = (uint16_t *) pTxSend;
			USARTx -> DR = (*pTxBuffer & (uint16_t)0x1FF);
			/*Si no se configura la paridad*/
			if(!(USARTx->CR1 & USART_CR1_PCE))
			{
				/*Pasamos dos espacios de pTxSend*/
				pTxSend += 2;
			}
			else /*Si se configura la paridad*/
			{
				/*Pasamos solo un espacio de pTxSend*/
				pTxSend++;
			}
		}
		else /*8 bits*/
		{
			/*Solo para la versión de imprimibles*/
			USARTx -> DR = (*pTxSend & (uint8_t)0xFF);
			pTxSend++;
		}

	}

	/*Revisar si la transmisión se completó*/
	while(!(USARTx->SR & USART_SR_TC));

	return;
}

void USARTx_Receive_Data(USART_TypeDef *USARTx, uint8_t *pRxReceive, uint32_t size)
{

	for(uint32_t i=0; i<size; i++)
	{
		/*Verificar si hay data lista a ser leída*/
		while(!(USARTx->SR & USART_SR_RXNE));

		/*9 bits*/
		if(USARTx->CR1 & USART_CR1_M)
		{
			/*Paridad*/
			if(!(USARTx->CR1 & USART_CR1_PCE)) /*Si no hay paridad*/
			{
				*((uint16_t*)pRxReceive) = USARTx->DR & (uint16_t)0x1FF;
				pRxReceive += 2;
			}
			else /* Si hay paridad*/
			{
				*pRxReceive = USARTx->DR & (uint8_t)0xFF;
				pRxReceive++;
			}
		}
		else /*8 bits*/
		{
			//Revisar paridad
			if(!(USARTx->CR1 & USART_CR1_PCE)) //Si no hay paridad
			{
				*pRxReceive= USARTx->DR & (uint8_t)0xFF;
			}
			else //Si hay paridad
			{
				*pRxReceive= USARTx->DR & (uint8_t)0x7F;
			}

			pRxReceive++;
		}
	}

	return;
}

/*
 * @brief
 * Se encarga de almacenar los datos de envío dentro de un Handler, que luego se utilizará
 * en una función.
 * Dicha función evaluará si se envía o recibe e interrupciones.
 */

uint8_t USARTx_SendDataIT(USARTx_Handle_t *pUSARTHandle,uint8_t *pTxBuffer, uint32_t size)
{
	uint8_t txState = pUSARTHandle -> TxBusyState;

	if(txState != USART_BUSY_IN_TX)
	{
		pUSARTHandle -> TxLen = size;
		pUSARTHandle -> pTxBuffer = pTxBuffer; //Aquí se guarda la dirección de pRxBuffer, recordar que es un arreglo y funciona como tal
		pUSARTHandle -> TxBusyState = USART_BUSY_IN_TX;
		pUSARTHandle -> pUSARTx -> CR1 |= USART_CR1_TXEIE | USART_CR1_TCIE;
	}
	return txState;
}

/*
 * @brief
 * Se encarga de almacenar los datos necesarios para la recepción
 * Dicha función evaluará si se envía o recibe datos e interrupciones.
 */

uint8_t USARTx_ReceiveDataIT(USARTx_Handle_t *pUSARTHandle,uint8_t *pRxBuffer, uint32_t size)
{
	uint8_t rxState = pUSARTHandle -> RxBusyState;

	if(rxState != USART_BUSY_IN_RX)
	{
		pUSARTHandle -> RxLen = size;
		pUSARTHandle -> pRxBuffer = pRxBuffer; //Aquí se guarda la dirección de pRxBuffer, recordar que es un arreglo y funciona como tal
		pUSARTHandle -> RxBusyState = USART_BUSY_IN_RX;
		(void)pUSARTHandle -> pUSARTx -> DR;
		pUSARTHandle -> pUSARTx -> CR1 |= USART_CR1_RXNEIE;
	}
	return rxState;
}

void USARTx_IRQHandling(USARTx_Handle_t *pUSARTHandle)
{
	uint8_t temp1, temp2, temp3;
	uint16_t *pdata;

	/*TC flag: Aquí se llega al final de toda transmisión de datos*/

	/*Si llegamos al final de transmisión*/
	temp1 = pUSARTHandle -> pUSARTx -> SR & USART_SR_TC;
	/*Evaluar si la interrupción por TC está activa*/
	temp2 = pUSARTHandle -> pUSARTx -> CR1 & USART_CR1_TCIE;

	if(temp1 && temp2)
	{
		/*Si estamos en transmisión IT*/
		if(pUSARTHandle -> TxBusyState == USART_BUSY_IN_TX)
		{
			/*Si la data que se envía es vacío*/
			if(!(pUSARTHandle -> TxLen))
			{
				pUSARTHandle -> pUSARTx-> SR &=~ USART_SR_TC; //Se limpia la bancera TC
				pUSARTHandle -> pUSARTx-> CR1 &=~ USART_CR1_TCIE; //Se desactiva la interrupción por TC
				pUSARTHandle -> TxBusyState = USART_READY; //Se indica que está listo para otro estaod
				pUSARTHandle -> pTxBuffer = NULL; //Se limpia el buffer
				pUSARTHandle -> TxLen = 0; //Restablecer la longitud a cero
				//Aquí va una función para indicar por el terminal
			}
		}
	}

	/*TXE flag: Para la transmisión continua*/

	/*Si el buffer está vacío luego de guardar en el registro de desplazamiento el contenido de DR*/
	temp1 = pUSARTHandle -> pUSARTx -> SR & USART_SR_TXE;
	/*Si está activa a la interupción por TX*/
	temp2 = pUSARTHandle -> pUSARTx -> SR & USART_CR1_TXEIE;

	if(temp1 && temp2)
	{
		/*Si estamos en transmisión IT*/
		if(pUSARTHandle -> TxBusyState == USART_BUSY_IN_TX)
		{
			/*Si la data que se envía tiene almenos un caracter*/
			if(pUSARTHandle -> TxLen > 0)
			{
				/*9 bits*/
				if(pUSARTHandle -> pUSARTx -> CR1 & USART_CR1_M)
				{
					pdata = (uint16_t *) pUSARTHandle -> pTxBuffer;
					pUSARTHandle -> pUSARTx -> DR = (*pdata & (uint16_t)0x1FF);
					/*Si no se configura la paridad*/
					if(!(pUSARTHandle -> pUSARTx -> CR1 & USART_CR1_PCE))
					{
						/*Pasamos dos espacios*/
						pUSARTHandle -> pTxBuffer += 2;
						/**/
						pUSARTHandle -> TxLen -= 2;
					}
					else /*Si se configura la paridad*/
					{
						/*Pasamos solo un espacio*/
						pUSARTHandle -> pTxBuffer += 1;
						/**/
						pUSARTHandle -> TxLen -= 1;
					}
				}
				else /*8 bits*/
				{
					/*Solo para la versión de imprimibles*/
					pUSARTHandle -> pUSARTx -> DR = (*pUSARTHandle -> pTxBuffer & (uint8_t)0xFF);
					pUSARTHandle -> pTxBuffer++;
					pUSARTHandle -> TxLen -=1;
				}

				/*Terminamos de transmitir cuando se termine la data*/
				if(pUSARTHandle->TxLen == 0)
				{
					pUSARTHandle->pUSARTx->CR1 &=~ USART_CR1_TXEIE;
				}
			}
		}
	}

	/*RXNE flag: Para la recepción continua*/
	temp1 = pUSARTHandle -> pUSARTx -> SR & USART_SR_RXNE;
	temp2 = pUSARTHandle -> pUSARTx -> CR1 & USART_CR1_RXNEIE;

	if(temp1 && temp2)
	{
		if(pUSARTHandle->RxBusyState == USART_BUSY_IN_RX){

			if(pUSARTHandle -> RxLen > 0)
			{
				/*9 bits*/
				if(pUSARTHandle -> pUSARTx -> CR1 & USART_CR1_M)
				{

					/*Si no se configura la paridad*/
					if(!(pUSARTHandle -> pUSARTx -> CR1 & USART_CR1_PCE))
					{

						*((uint16_t*)pUSARTHandle-> pRxBuffer) = ((pUSARTHandle -> pUSARTx -> DR) & (uint16_t)0x1FF);
						pUSARTHandle -> pRxBuffer ++;
						pUSARTHandle -> pRxBuffer ++;
						pUSARTHandle -> RxLen -= 2;
					}
					else /*Si se configura la paridad*/
					{
						*pUSARTHandle -> pRxBuffer = ((pUSARTHandle -> pUSARTx -> DR) & (uint8_t)0xFF);
						pUSARTHandle -> pRxBuffer++;
						pUSARTHandle -> RxLen -= 1;
					}
				}
				else /*8 bits*/
				{
					if(!(pUSARTHandle -> pUSARTx -> CR1 & USART_CR1_PCE))
					{
						*pUSARTHandle -> pRxBuffer = (uint8_t) (pUSARTHandle -> pUSARTx -> DR & (uint8_t) 0xFF);
					}
					else /*Solo leemos 7 bits por la paridad*/
					{
						*pUSARTHandle -> pRxBuffer = (uint8_t)(pUSARTHandle -> pUSARTx -> DR & (uint8_t)0x7F);
					}

					pUSARTHandle -> pRxBuffer++;
					pUSARTHandle -> RxLen -= 1;

				}
			}
			/*Limpiamos la interrupción cuando se lea toda la data*/
			if(!(pUSARTHandle -> RxLen)){
				pUSARTHandle -> pUSARTx -> CR1 &=~ USART_CR1_RXNEIE; //Desactivamos la interrupción
				pUSARTHandle -> RxBusyState = USART_READY; //Indicamos que está listo para recibir
				//Aquí va una función para indicar por el terminal
			}
		}
	}

	/*CTS flag: para el control*/

	temp1 = pUSARTHandle -> pUSARTx -> SR & USART_SR_CTS;
	temp2 = pUSARTHandle -> pUSARTx -> CR3 & USART_CR3_CTSE;
	temp3 = pUSARTHandle -> pUSARTx -> CR3 & USART_CR3_CTSIE;

	if(temp1 && temp2 && temp3)
	{
			pUSARTHandle -> pUSARTx -> SR &=~ USART_SR_CTS;
			//Aquí va una función para indicar por el terminal
	}


}




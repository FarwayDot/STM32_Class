/*
 * eeprom.c
 *
 *  Created on: Feb 1, 2026
 *      Author: User
 */

#include "eeprom.h"
#include "i2c.h"


/*
 * @brief EEPROM_Write_byte
 */
uint8_t EEPROM_Write_N_byte(I2C_TypeDef *I2Cx, uint8_t EEPROM_Address, uint16_t Byte_Address, uint8_t *pTxbuffer, uint32_t Len)
{
	volatile int tmp; //Para borrar

	if(Len <= 0 || pTxbuffer == (void *)0) return 1;

	while(I2Cx->SR2 & (1U<<I2C_SR2_BUSY_Pos));

	I2Cx->CR1 |= (1U<<I2C_CR1_START_Pos);

	while(!(I2Cx->SR1 & (1U<<I2C_SR1_SB_Pos)));


	//Enviar EEPROM Address
	EEPROM_Address = ((EEPROM_Address & 0x07) | 0x50)<<1; //0 1 0 1 0 A2 A1 A0
	EEPROM_Address &= ~(1U); //1 0 1 0 A2 A1 A0 0
	I2Cx->DR = EEPROM_Address;

	while(!(I2Cx->SR1 & (1U<<I2C_SR1_ADDR_Pos)))
	{
		if(I2Cx->SR1 & (1U<<I2C_SR1_AF_Pos))
		{
			I2Cx->CR1 |= (1U<<I2C_CR1_STOP_Pos);
			I2Cx->SR1 &= ~(1U<<I2C_SR1_AF_Pos);

			return 1;
		}
	}

	tmp = I2Cx->SR1;
	tmp = I2Cx->SR2;
	(void)tmp;

	//Enviar Byte Address
	while(!(I2Cx->SR1 & (1U<<I2C_SR1_TXE_Pos)));
	I2Cx->DR = (Byte_Address>>8) & 0xFF; //High Byte
	while(!(I2Cx->SR1 & (1U<<I2C_SR1_TXE_Pos)));
	I2Cx->DR = Byte_Address & 0xFF; //Low Byte

	//Enviar data
	while(Len>0U)
	{
		while(!(I2Cx->SR1 & (1U<<I2C_SR1_TXE_Pos)));
		I2Cx->DR = *pTxbuffer;
		pTxbuffer++;
		Len--;
	}

	while(!(I2Cx->SR1 & (1U<<I2C_SR1_TXE_Pos)));
	while(!(I2Cx->SR1 & (1U<<I2C_SR1_BTF_Pos)));

	I2Cx->CR1 |= (1U<<I2C_CR1_STOP_Pos);

	return 0;
}

uint8_t EEPROM_Read_Current_byte(I2C_TypeDef *I2Cx, uint8_t EEPROM_Address, uint8_t *pRxbuffer)
{

	volatile int tmp; //Para borrar

	while(I2Cx->SR2 & (1U<<I2C_SR2_BUSY_Pos));

	I2Cx->CR1 |= (1U<<I2C_CR1_START_Pos);

	while(!(I2Cx->SR1 & (1U<<I2C_SR1_SB_Pos)));

	EEPROM_Address = ((EEPROM_Address & 0x07) | 0x50)<<1; //0 1 0 1 0 A2 A1 A0
	EEPROM_Address |= (1U); //1 0 1 0 A2 A1 A0 1
	I2Cx->DR = EEPROM_Address;

	while(!(I2Cx->SR1 & (1U<<I2C_SR1_ADDR_Pos)))
	{
		if(I2Cx->SR1 & (1U<<I2C_SR1_AF_Pos))
		{
			I2Cx->CR1 |= (1U<<I2C_CR1_STOP_Pos);
			I2Cx->SR1 &= ~(1U<<I2C_SR1_AF_Pos);

			return 1;
		}
	}

	tmp = I2Cx->SR1;
	tmp = I2Cx->SR2;
	(void)tmp;

	while(!(I2Cx->SR1 & (1U<<I2C_SR1_RXNE_Pos)));
	*pRxbuffer = I2Cx->DR;

	//ACK on
	I2Cx->CR1 |= (1U<<I2C_CR1_ACK_Pos);

	//ACK off
	I2Cx->CR1 &= ~(1U<<I2C_CR1_ACK_Pos);


	while(I2Cx->SR1 & (1U<<I2C_SR1_RXNE_Pos));

	I2Cx->CR1 |= (1U<<I2C_CR1_STOP_Pos);

	return 0;
}

uint8_t EEPROM_Read_N_Bytes(I2C_TypeDef *I2Cx, uint8_t EEPROM_Address, uint16_t Byte_Address, uint8_t *pRxbuffer, uint8_t Len)
{
	volatile int tmp; //Para borrar

	while(I2Cx->SR2 & (1U<<I2C_SR2_BUSY_Pos));

	/*** 1.Iniciar comunicación ***/

	//Start
	I2Cx->CR1 |= (1U<<I2C_CR1_START_Pos);
	while(!(I2Cx->SR1 & (1U<<I2C_SR1_SB_Pos)));

	//Control byte y EEPROM Address Write
	EEPROM_Address = ((EEPROM_Address & 0x07) | 0x50)<<1; //0 1 0 1 0 A2 A1 A0
	EEPROM_Address &= ~(1U); //1 0 1 0 A2 A1 A0 0 : Write
	I2Cx->DR = EEPROM_Address;

	//Verificar esclavo
	while(!(I2Cx->SR1 & (1U<<I2C_SR1_ADDR_Pos)));
	tmp = I2Cx->SR1;
	tmp = I2Cx->SR2;
	(void)tmp;

	/*** 2.Enviar dirección de 16bits que pertenece a la memoria interna ***/

	//Enviar Byte Address
	while(!(I2Cx->SR1 & (1U<<I2C_SR1_TXE_Pos)));
	I2Cx->DR = (Byte_Address>>8) & 0xFF; //High Byte
	while(!(I2Cx->SR1 & (1U<<I2C_SR1_TXE_Pos)));
	I2Cx->DR = Byte_Address & 0xFF; //Low Byte

	//La transferencia se completó (salió del Shift Register)
	while(!(I2Cx->SR1 & (1U<<I2C_SR1_BTF_Pos)));

	/*** 3.Re Start ***/

	//Enviar Star y verificar
	I2Cx->CR1 |= (1U<<I2C_CR1_START_Pos);
	while(!(I2Cx->SR1 & (1U<<I2C_SR1_SB_Pos)));

	//Enviar EEPROM Address Read
	EEPROM_Address |= (1U); //1 0 1 0 A2 A1 A0 0 : Read
	I2Cx->DR = EEPROM_Address;

	if(Len == 1U)
	{
		//Según ref manual, se debe apagar el ACK si solo se lee un byte antes de limpiar ADDR
		I2Cx->CR1 &= ~(1U<<I2C_CR1_ACK_Pos);

		//Verificar y limpiar ADDR
		while(!(I2Cx->SR1 & (1U<<I2C_SR1_ADDR_Pos)));
		tmp = I2Cx->SR1;
		tmp = I2Cx->SR2;
		(void)tmp;

		//Generar condición de parada
		I2Cx->CR1 |= (1U<<I2C_CR1_STOP_Pos);

		//Esperar RxNE se establezca
		while(!(I2Cx->SR1 & (1U<<I2C_SR1_RXNE_Pos)));
		//Leer datos
		*pRxbuffer = I2Cx->DR;
	}
	else
	{

		//Activar ACK (antes para evitar que no llegue a tiempo luego de limpiar ADDR)
		I2Cx->CR1 |= (1U<<I2C_CR1_ACK_Pos);

		//Verificar y limpiar ADDR, inmediatamente después empieza a recibir
		while(!(I2Cx->SR1 & (1U<<I2C_SR1_ADDR_Pos)));
		tmp = I2Cx->SR1;
		tmp = I2Cx->SR2;
		(void)tmp;

		while(Len>0U)
		{
			//Se debe limpiar ACK y establecer STOP
			if(Len == 1U)
			{
				//Deshabilitar ACK
				I2Cx->CR1 &= ~(1U<<I2C_CR1_ACK_Pos);
				//Generar condición de parada
				I2Cx->CR1 |= (1U<<I2C_CR1_STOP_Pos);
				//Esperar RxNE se establezca
				while(!(I2Cx->SR1 & (1U<<I2C_SR1_RXNE_Pos)));
				*pRxbuffer = I2Cx->DR;
				Len--;
			}
			else
			{
				//Esperar RxNE se establezca
				while(!(I2Cx->SR1 & (1U<<I2C_SR1_RXNE_Pos)));
				*pRxbuffer = I2Cx->DR;
				pRxbuffer++;
				Len--;

			}
		}
	}

	return 0;

}


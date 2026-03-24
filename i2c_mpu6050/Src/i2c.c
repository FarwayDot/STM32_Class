/*
 * i2c.c
 *
 *  Created on: Feb 1, 2026
 *      Author: User
 */

#include "GPIO_Config.h"
#include "i2c.h"


void I2C_GPIO_Config(void)
{

	//PIN 8 AF4 I2C1
	//PIN 9 AF4 I2C1

	GPIO_AF_Config(GPIO_I2C_SCL, PIN_I2C_SCL, PUPDR_NONE, OSPEEDR_VERYHIGH, OTYPER_OD);
	GPIO_I2C_SCL->AFR[1] &= ~(0xF<<0);
	GPIO_I2C_SCL->AFR[1] |= (4U<<0);

	GPIO_AF_Config(GPIO_I2C_SDA, PIN_I2C_SDA, PUPDR_NONE, OSPEEDR_VERYHIGH, OTYPER_OD);
	GPIO_I2C_SDA->AFR[1] &= ~(0xF<<4);
	GPIO_I2C_SDA->AFR[1] |= (4U<<4);

	return;
}

void I2C_Config(I2C_TypeDef *I2Cx)
{
	uint32_t freq_pclk1 = SystemCoreClock/2; //APB1 en Hz
	uint16_t freq = freq_pclk1/1000000; //MHz

	//GPIO Config
	I2C_GPIO_Config();

	//Clock
	RCC->APB1ENR |= (1U<<RCC_APB1ENR_I2C1EN_Pos);

	//Ingresar a modo reset y salir de este (recomend)
	I2Cx->CR1 |= (1U<<I2C_CR1_SWRST_Pos);
	I2Cx->CR1 &= ~(1U<<I2C_CR1_SWRST_Pos);

	//Configuramos el campo FREQ del registro CR2 (Debe colocarse el valor en Mhz)
	I2Cx->CR2 &= ~0x3F;
	I2Cx->CR2 |= freq; //plck1 / 1000000

	//Programar dirección del Esclavo en caso el STM32 trabaje en modo Esclavo
	I2Cx->OAR1 &= ~(1U<<I2C_OAR1_ADDMODE_Pos); //Modo 7 bits address
	I2Cx->OAR1 |= (1U<<14); //Según ref manual debe estar en 1 por software
	I2Cx->OAR1 |= 0xF<<I2C_OAR1_ADD1_Pos;

	//Standard Mode
	I2Cx->CCR |= 210U; //Para los 100kHz (revisar notas)

	//Config TRISE
	I2Cx->TRISE |= (43U & 0x3F);

	//Deshabilitar START
	I2Cx->CR1 &= ~(1U<<I2C_CR1_START_Pos);

	//Habilitar I2C
	I2Cx->CR1 |= (1U<<I2C_CR1_PE_Pos);

	return;
}

/*
 * @brief Transmisión de datos del maestro
 *
 */

uint8_t I2C_Master_Send_Data(I2C_TypeDef *I2Cx, uint16_t SlaveAddr, uint8_t *pTxbuffer, uint32_t Len)
{
	volatile int tmp; //Para borrar

	//Verificar datos
	if(Len <= 0 || pTxbuffer == (void *)0) return 1;

	//Esperar de que el bus esté libre
	while(I2Cx->SR2 & (1U<<I2C_SR2_BUSY_Pos));

	//Generar Start
	I2Cx->CR1 |= (1U<<I2C_CR1_START_Pos);

	//Confirmar que start se genere
	while(!(I2Cx->SR1 & (1U<<I2C_SR1_SB_Pos)));

	//Enviar la dirección del esclavo al que nos comunicaremos
	SlaveAddr = SlaveAddr<<1; //A7 - A1
	SlaveAddr &= ~(1U); //A0 : Write
	I2Cx->DR = SlaveAddr;

	//I2C reconoce al slave
	while(!(I2Cx->SR1 & (1U<<I2C_SR1_ADDR_Pos)))
	{
		// Si recibimos NACK (Nadie contestó)
		if(I2Cx->SR1 & (1U<<I2C_SR1_AF_Pos))
		{
			// Generar STOP para liberar bus
			I2Cx->CR1 |= (1U<<I2C_CR1_STOP_Pos);
			// Limpiar bandera AF
			I2Cx->SR1 &= ~(1U<<I2C_SR1_AF_Pos);
			return 1; // Salir de la función indicando error
		}
	}

	//Limpiar flag ADDR
	tmp = I2Cx->SR1;
	tmp = I2Cx->SR2;
	(void)tmp;

	//Enviar datos
	while(Len>0U)
	{
		//Registro Tx vacío
		while(!(I2Cx->SR1 & (1U<<I2C_SR1_TXE_Pos)));
		//Asignamos puntero al registro
		I2Cx->DR = *pTxbuffer;
		pTxbuffer++;
		Len--;
	}

	//Esperar a que la transmisión se complete
	while(!(I2Cx->SR1 & (1U<<I2C_SR1_TXE_Pos))); //Registro Tx vacío
	while(!(I2Cx->SR1 & (1U<<I2C_SR1_BTF_Pos))); //Este se coloca en 1 cuando no se escribe nada dentro de DR y se espera enviar algo

	//Generar condición de stop
	I2Cx->CR1 |= (1U<<I2C_CR1_STOP_Pos);

	return 0;
}

/*
 * @brief Recibir datos de un esclavo
 *
 */

void I2C_Master_Receive_Data(I2C_TypeDef *I2Cx, uint16_t SlaveAddr, uint8_t *pRxbuffer, uint32_t Len)
{
	volatile int tmp; //Para borrar

	//Verificar datos
	if(Len <= 0) return;

	//Esperar de que el bus esté libre
	while(I2Cx->SR2 & (1U<<I2C_SR2_BUSY_Pos));

	//Start y verificamos
	I2Cx->CR1 |= (1U<<I2C_CR1_START_Pos);
	while(I2Cx->SR1 & (1U<<I2C_SR1_SB_Pos));

	//Enviamos address e instrucción read|1
	SlaveAddr = SlaveAddr<<1; //A7 - A1
	SlaveAddr &= ~(1U); //A0 : Write
	I2Cx->DR = SlaveAddr;

/********************************/
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

	return;
}

/*
 * Read Bytes from a slave who has internal registers (16BITS OR 8BITS)
 */

uint8_t I2C_Master_Read_Register_Bytes(I2C_TypeDef *I2Cx, uint8_t Device_Addres, uint16_t Byte_Address, uint8_t RegSize,  uint8_t *pRxbuffer, uint8_t Len)
{
	volatile int tmp; //Para borrar

	while(I2Cx->SR2 & (1U<<I2C_SR2_BUSY_Pos));

	/*** 1.Iniciar comunicación ***/

	//Start
	I2Cx->CR1 |= (1U<<I2C_CR1_START_Pos);
	while(!(I2Cx->SR1 & (1U<<I2C_SR1_SB_Pos)));

	//Control byte y EEPROM Address Write
	Device_Addres = (Device_Addres)<<1;
	Device_Addres &= ~(1U); //Write
	I2Cx->DR = Device_Addres;

	//Verificar esclavo
	while(!(I2Cx->SR1 & (1U<<I2C_SR1_ADDR_Pos)));
	tmp = I2Cx->SR1;
	tmp = I2Cx->SR2;
	(void)tmp;

	/*** 2.Enviar dirección de 16bits que pertenece a la memoria interna ***/

	//Enviar Byte Address
	if(RegSize == Size_8bits)
	{
		while(!(I2Cx->SR1 & (1U<<I2C_SR1_TXE_Pos)));
		I2Cx->DR = Byte_Address & 0xFF;
	}
	else if(RegSize == Size_16bits)
	{
		while(!(I2Cx->SR1 & (1U<<I2C_SR1_TXE_Pos)));
		I2Cx->DR = (Byte_Address>>8) & 0xFF; //High Byte
		while(!(I2Cx->SR1 & (1U<<I2C_SR1_TXE_Pos)));
		I2Cx->DR = Byte_Address & 0xFF; //Low Byte
	}
	else
	{
		return 1;
	}


	//La transferencia se completó (salió del Shift Register)
	while(!(I2Cx->SR1 & (1U<<I2C_SR1_BTF_Pos)));

	/*** 3.Re Start ***/

	//Enviar Star y verificar
	I2Cx->CR1 |= (1U<<I2C_CR1_START_Pos);
	while(!(I2Cx->SR1 & (1U<<I2C_SR1_SB_Pos)));

	//Enviar  Address Read
	Device_Addres |= (1U); //Read
	I2Cx->DR = Device_Addres;

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


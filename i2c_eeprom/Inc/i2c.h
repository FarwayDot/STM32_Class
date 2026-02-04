/*
 * i2c.h
 *
 *  Created on: Feb 1, 2026
 *      Author: User
 */

#ifndef I2C_H_
#define I2C_H_

#include "stm32f4xx.h"
#include "stdint.h"

//I2C
#define GPIO_I2C_SCL	GPIOB
#define PIN_I2C_SCL		8

#define GPIO_I2C_SDA	GPIOB
#define PIN_I2C_SDA		9

void I2C_GPIO_Config(void);
void I2C_Config(I2C_TypeDef *I2Cx);
void I2C_Master_Send_Data(I2C_TypeDef *I2Cx, uint16_t SlaveAddr, uint8_t *pTxbuffer, uint32_t Len);
void I2C_Master_Receive_Data(I2C_TypeDef *I2Cx, uint16_t SlaveAddr, uint8_t *pRxbuffer, uint32_t Len);

#endif /* I2C_H_ */

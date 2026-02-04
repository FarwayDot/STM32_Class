/*
 * eeprom.h
 *
 *  Created on: Feb 1, 2026
 *      Author: User
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include "stm32f4xx.h"
#include "stdint.h"

uint8_t EEPROM_Write_N_byte(I2C_TypeDef *I2Cx, uint8_t EEPROM_Address, uint16_t Byte_Address, uint8_t *pTxbuffer, uint32_t Len);
uint8_t EEPROM_Read_Current_byte(I2C_TypeDef *I2Cx, uint8_t EEPROM_Address, uint8_t *pRxbuffer);
uint8_t EEPROM_Read_N_Bytes(I2C_TypeDef *I2Cx, uint8_t EEPROM_Address, uint16_t Byte_Address, uint8_t *pRxbuffer, uint8_t Len);
#endif /* EEPROM_H_ */

/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_at24cxx_interface_template.c
 * @brief     driver at24cxx interface template source file
 * @version   2.0.0
 * @author    Shifeng Li
 * @date      2021-02-17
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2021/02/17  <td>2.0      <td>Shifeng Li  <td>format the code
 * <tr><td>2020/10/15  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_at24cxx_interface.h"
#include "main.h"
#include "i2c.h"
#include <stdio.h>
#include <stdarg.h>

/**
 * @brief  interface iic bus init
 * @return status code
 *         - 0 success
 *         - 1 iic init failed
 * @note   none
 */
uint8_t at24cxx_interface_iic_init(void)
{
    MX_I2C2_Init();
    return 0;
}

/**
 * @brief  interface iic bus deinit
 * @return status code
 *         - 0 success
 *         - 1 iic deinit failed
 * @note   none
 */
uint8_t at24cxx_interface_iic_deinit(void)
{
	HAL_I2C_DeInit(&hi2c2);
    return 0;
}

/**
 * @brief      interface iic bus read
 * @param[in]  addr is the iic device write address
 * @param[in]  reg is the iic register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t at24cxx_interface_iic_read(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len)
{
	return HAL_I2C_Mem_Read(&hi2c2, addr, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 100);
}

/**
 * @brief     interface iic bus write
 * @param[in] addr is the iic device write address
 * @param[in] reg is the iic register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t at24cxx_interface_iic_write(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len)
{
    return HAL_I2C_Mem_Write(&hi2c2, addr, reg, I2C_MEMADD_SIZE_8BIT, buf, len, 100);
}

/**
 * @brief      interface iic bus read with 16 bits register address
 * @param[in]  addr is the iic device write address
 * @param[in]  reg is the iic register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t at24cxx_interface_iic_read_address16(uint8_t addr, uint16_t reg, uint8_t *buf, uint16_t len)
{
    return HAL_I2C_Mem_Read(&hi2c2, addr, reg, I2C_MEMADD_SIZE_16BIT, buf, len, 10);
}

/**
 * @brief     interface iic bus write with 16 bits register address
 * @param[in] addr is the iic device write address
 * @param[in] reg is the iic register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t at24cxx_interface_iic_write_address16(uint8_t addr, uint16_t reg, uint8_t *buf, uint16_t len)
{
    return HAL_I2C_Mem_Write(&hi2c2, addr, reg, I2C_MEMADD_SIZE_16BIT, buf, len, 100);
}

/**
 * @brief     interface delay ms
 * @param[in] ms
 * @note      none
 */
void at24cxx_interface_delay_ms(uint32_t ms)
{
#if defined(FREERTOS_H)
	vTaskDelay(pdm_)
#else
	HAL_Delay(ms);
#endif
}

/**
 * @brief     interface print format data
 * @param[in] fmt is the format data
 * @note      none
 */
void at24cxx_interface_debug_print(const char *const fmt, ...)
{

	char str[256];
	uint16_t len;
	va_list args;

	memset((char *)str, 0, sizeof(char) * 256);
	va_start(args, fmt);
	vsnprintf((char *)str, 255, (char const *)fmt, args);
	va_end(args);

	len = strlen((char *)str);

	for(uint16_t i = 0; i<len; i++){
		ITM_SendChar(str[i]);
	}
}

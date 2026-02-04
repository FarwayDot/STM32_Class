/*
 * bmp.h
 *
 *  Created on: Jan 8, 2026
 *      Author: User
 */

#ifndef BMP_H_
#define BMP_H_

#include "stdint.h"


typedef struct
{
     const uint8_t *data;
     uint16_t width;
     uint16_t height;
     uint8_t dataSize;
} tImage;

extern const tImage megusta_bmp;


#endif /* BMP_H_ */

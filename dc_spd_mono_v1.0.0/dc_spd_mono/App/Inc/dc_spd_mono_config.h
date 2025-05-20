/*
 * dc_spd_config.h
 *
 *  Created on: Dec 4, 2024
 *      Author: USER
 */

#ifndef APP_INC_DC_SPD_MONO_CONFIG_H_
#define APP_INC_DC_SPD_MONO_CONFIG_H_

#define DC_SPD_FW_VERSION				"1.0.0"

#define IMAX_MINVAL					2
#define IMAX_MAXVAL					30

#define VMAX_MINVAL					10
#define VMAX_MAXVAL					500
#define VMAX_STEP					2

#define TACOMAX_MINVAL				0.01
#define TACOMAX_MAXVAL				1
#define TACOMAX_STEP				0.001

#define ZERO_CROSS_FIRST_DELAY		15
#define ZERO_CROSS_MAX_COUNT		415



#define KP	 						0.0526
#define KI	 						0.239
#define KD							0
#define TS  						(float)2E-3
#define UMAX 						ZERO_CROSS_MAX_COUNT
#define UMIN 						10


#define ALPHAMAX					10

/*Configure for open loop*/
#define OPEN_LOOP_START_TIME_INC	100
#define OPEN_LOOP_STEP				1
#define OPEN_LOOP_STOP_TIME_INC		200

#endif /* APP_INC_DC_SPD_MONO_CONFIG_H_ */

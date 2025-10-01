/*
 * Timer3.h
 *
 *  Created on: Aug 12, 2025
 *      Author: User
 */

#ifndef TIMER3_H_
#define TIMER3_H_

/*Librerías*/
#include <stm32f4xx.h>
#include <stdint.h>

/* Definiciones */

/* Variables globlaes, enumeraciones, listas */
typedef enum
{
	PWM_ISR_OFF = 0,
	PWM_ISR_ON
}PWM_ISR_e;

typedef enum
{
	PWM_Output_1= 1,
	PWM_Output_2
}PWM_Output_Mode_e;

typedef enum
{
	Upcounting= 1,
	Downcounting
}PWM_Counting_Mode_e;

typedef enum
{
	Edge_Aligned = 0,
	Center_Aligned_1,
	Center_Aligned_2,
	Center_Aligned_3
}PWM_Aligned_Mode_e;

typedef enum
{
	Active_High = 0,
	Active_Low
}PWM_Polarity_e;

typedef struct
{
	uint16_t psc;
	uint32_t arr;
	PWM_Output_Mode_e output_mode;
	PWM_Counting_Mode_e counting_mode;
	PWM_Aligned_Mode_e aligned_mode;
	PWM_Polarity_e polarity_mode;
	PWM_ISR_e isr;
}PWM_tim3_t;

/* Prototipo de funciones */
void tim3_ch2_pb5_PWM_gpio_config(void);
void tim3_count_config(uint16_t psc, uint32_t arr);
void tim3_count_start(void);
void tim3_PWM_Config(PWM_tim3_t *pwm_tim);
void tim3_PWM_Start(uint32_t num);
void tim3_PWM_Set(uint32_t duty);


#endif /* TIMER3_H_ */

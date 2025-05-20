/*
 * RCC.c
 *
 *  Created on: Nov 19, 2024
 *      Author: jeanf
 */

#include "RCC.h"

#ifdef PLL_OFF

void HSI_Config(void)
{
	/*Activamos el HSE*/
	RCC->CR |= RCC_CR_HSION;
	/*Esperamos a que esté listo el oscilador*/
	while(!(RCC->CR & RCC_CR_HSIRDY));
	/*Limpiamos la anterior fuente de reloj*/
	RCC->CFGR &=~ RCC_CFGR_SW_Msk;
	RCC->CFGR |= RCC_CFGR_SW_HSI; //No es necesario activarlo si vas a utilizar PLL
	/*Esperamos a que esté listo el cambio de oscilador*/
	while(RCC->CFGR & RCC_CFGR_SWS_HSI); //Revisar antes de usar
	/*Actualiza SystemCoreClock*/
	SystemCoreClockUpdate();
}

void HSE_Config(void)
{
	/*Activamos el HSE*/
	RCC->CR |= RCC_CR_HSEON | RCC_CR_HSEBYP;
	/*Esperamos a que esté listo el oscilador*/
	while(!(RCC->CR & RCC_CR_HSERDY));
	/*Limpiamos la anterior fuente de reloj*/
	RCC->CFGR &=~ RCC_CFGR_SW_Msk;
	RCC->CFGR = RCC_CFGR_SW_HSE; //No es necesario activarlo si vas a utilizar PLL
	/*Esperamos a que esté listo el cambio de oscilador*/
	while(!(RCC->CFGR & RCC_CFGR_SWS_HSE));
	/*Actualiza SystemCoreClock*/
	SystemCoreClockUpdate();
}


#endif

#ifdef PLL_ON

void HSI_Config_PLL(void)
{
	/*Activamos el HSE*/
	RCC->CR |= RCC_CR_HSION;
	/*Esperamos a que esté listo el oscilador*/
	while(!(RCC->CR & RCC_CR_HSIRDY));
	/*Limpiamos la anterior fuente de reloj*/
	RCC->CFGR &=~ RCC_CFGR_SW_Msk;
	/*Actualiza SystemCoreClock*/
	SystemCoreClockUpdate();
}

void HSE_Config_PLL(void)
{
	/*Activamos el HSE*/
	RCC->CR |= RCC_CR_HSEON | RCC_CR_HSEBYP;
	/*Esperamos a que esté listo el oscilador*/
	while(!(RCC->CR & RCC_CR_HSERDY));
	/*Limpiamos la anterior fuente de reloj*/
	RCC->CFGR &=~ RCC_CFGR_SW_Msk;
	/*Actualiza SystemCoreClock*/
	SystemCoreClockUpdate();
}

void PLL_Config(uint8_t Source)
{
	/*Apagamos el PLL  por las dudas*/
	RCC -> CR &= ~RCC_CR_PLLON_Msk;

	/*Escojemos fuente de reloj para PLL*/

	switch(Source)
	{
		case HSE_SOURCE:

			HSE_Config_PLL(); //FOSC= 8Mhz, M=/4, N=*168, P=1(/4)
			/*Configurar la fuente de reloj de PLL*/
			RCC -> PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;

			/*Configurar M,N,P,Q*/

			// 8Mhz / 4 = 2Mhz
			RCC -> PLLCFGR &= ~RCC_PLLCFGR_PLLM;
			RCC -> PLLCFGR |= (4<<0); //M 1-2Mhz

			//2Mhz * 84 = 168Mhz
			RCC -> PLLCFGR &= ~RCC_PLLCFGR_PLLN;
			RCC -> PLLCFGR |= (168<<6);//N 192-472Mhz

			//336Mhz / 2 = 84Mhz
			RCC -> PLLCFGR &= ~RCC_PLLCFGR_PLLP;
			RCC -> PLLCFGR |= (1<<16);//P <=84Mhz

			//Q <= 48Mhz (Opcional Configurar
			//RCC -> PLLCFGR &= ~RCC_PLLCFGR_PLLQ;
			//RCC -> PLLCFGR |= (4<<24);

			break;

		case HSI_SOURCE:

			HSI_Config_PLL(); //FOSC= 16Mhz, M=/8, N=*168, P=1(/4)
			/*Configurar la fuente de reloj de PLL*/
			RCC -> PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSI;

			/*Configurar M,N,P,Q*/

			// 8Mhz / 4 = 2Mhz
			RCC -> PLLCFGR &= ~RCC_PLLCFGR_PLLM;
			RCC -> PLLCFGR |= (8<<0); //M 1-2Mhz

			//2Mhz * 84 = 168Mhz
			RCC -> PLLCFGR &= ~RCC_PLLCFGR_PLLN;
			RCC -> PLLCFGR |= (168<<6);//N 192-472Mhz

			//336Mhz / 2 = 84Mhz
			RCC -> PLLCFGR &= ~RCC_PLLCFGR_PLLP;
			RCC -> PLLCFGR |= (1<<16);//P <=84Mhz

			//Q <= 48Mhz (Opcional Configurar
			//RCC -> PLLCFGR &= ~RCC_PLLCFGR_PLLQ;
			//RCC -> PLLCFGR |= (4<<24);

			break;
		default: break;
	}

	/*Habilitar el PLL*/
	RCC -> CR |= RCC_CR_PLLON;

	/*Esperamos a que esté listo el cambio de oscilador*/
	while(!(RCC->CR & RCC_CR_PLLRDY));

	/*Configurar Prescalers*/

	RCC -> CFGR &= ~RCC_CFGR_PPRE2_Msk;
	RCC -> CFGR |= RCC_CFGR_PPRE2_DIV1; //APB2 <=84Mhz

	RCC -> CFGR &= ~RCC_CFGR_PPRE1_Msk;
	RCC -> CFGR |= RCC_CFGR_PPRE1_DIV2; //APB1 <=42Mhz

	/*Configurar la latencia de la flash (Revisar Pag. 46 ref. manual STM32F401X)*/
	FLASH -> ACR &= ~FLASH_ACR_LATENCY;
	FLASH -> ACR |= FLASH_ACR_LATENCY_2WS;

	/*Seleccionar la fuente dek sysclk*/
	RCC->CFGR 	&= ~RCC_CFGR_SW_Msk;
	RCC -> CFGR |= RCC_CFGR_SW_PLL;

	/*Esperamos a que esté listo el cambio de oscilador*/
	while(!(RCC->CFGR & RCC_CFGR_SWS_PLL));

	/*Actualiza SystemCoreClock*/
	SystemCoreClockUpdate();
}

#endif


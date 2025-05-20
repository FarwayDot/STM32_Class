/*
 * DISPLAY_7_SEGM.c
 *
 *  Created on: Dec 19, 2024
 *      Author: jeanf
 */

#include "DISPLAY_7_SEGM.h"

//static uint8_t Numbers_Integers[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

static const uint8_t Numbers_In_Binary[10] = {	//ABCDEFGP
										0b11111100, //0
										0b01100000, //1
										0b11011010, //2
										0b11110010, //3
										0b01100110, //4
										0b10110110, //5
										0b10111110,	//6
										0b11100000,	//7
										0b11111110,	//8
										0b11110110};//9
static SevenSegment_Display display = {
    .segmentPins = {
        {SEG_A_PORT, SEG_A_PIN}, // A
        {SEG_B_PORT, SEG_B_PIN}, // B
        {SEG_C_PORT, SEG_C_PIN}, // C
        {SEG_D_PORT, SEG_D_PIN}, // D
        {SEG_E_PORT, SEG_E_PIN}, // E
        {SEG_F_PORT, SEG_F_PIN}, // F
        {SEG_G_PORT, SEG_G_PIN}, // G
        {SEG_P_PORT, SEG_P_PIN}  // DP
    }
};


void InitPinAsOutput(GPIO_TypeDef* port, uint16_t pin)
{
    ENABLE_GPIO_CLOCK(port);

    // Configura el pin como entrada (modo general de propósito)
    port->MODER |= (0b01 << (pin * 2)); //
    port->PUPDR &= ~(0b11 << (pin * 2)); // Sin pull-up ni pull-down
    port->OSPEEDR |= (0b11 << (pin * 2));
}

void SevenSegment_Init(void)
{
	InitPinAsOutput(SEG_A_PORT, SEG_A_PIN);
	InitPinAsOutput(SEG_B_PORT, SEG_B_PIN);
	InitPinAsOutput(SEG_C_PORT, SEG_C_PIN);
	InitPinAsOutput(SEG_D_PORT, SEG_D_PIN);
	InitPinAsOutput(SEG_E_PORT, SEG_E_PIN);
	InitPinAsOutput(SEG_F_PORT, SEG_F_PIN);
	InitPinAsOutput(SEG_G_PORT, SEG_G_PIN);
	InitPinAsOutput(SEG_P_PORT, SEG_P_PIN);

	InitPinAsOutput(SEG_DISP1_PORT, SEG_DISP1_PIN);
	InitPinAsOutput(SEG_DISP2_PORT, SEG_DISP2_PIN);
	InitPinAsOutput(SEG_DISP3_PORT, SEG_DISP3_PIN);
	InitPinAsOutput(SEG_DISP4_PORT, SEG_DISP4_PIN);
}

void SevenSegment_DisplayDigit(uint8_t digit)
{
	if(digit > 9) return;

	uint8_t segment = Numbers_In_Binary[digit];

	for(uint8_t i=0; i<=8; i++)
	{
		uint16_t Pos_pin = display.segmentPins[i].pin; //Empezamos con A en la estructura

		if(segment & (0x80>>i))
		{
			display.segmentPins[i].port->BSRR |= (1 << Pos_pin);
		}
		else
		{
			display.segmentPins[i].port->BSRR |= (1 << (Pos_pin+16)) ;
		}

	}
}
void SevenSegment_Clear(void)
{
    for (int i = 0; i < 8; i++) {
        uint16_t pinPos = display.segmentPins[i].pin;
        display.segmentPins[i].port->BSRR = (1 << (pinPos + 16)); // Apagar segmento
    }
}

void SevenSegment_SelectDisplay(uint8_t display_chosen)
{
	 GPIOX_ODR(SEG_DISP1) = 0;
	 GPIOX_ODR(SEG_DISP2) = 0;
	 GPIOX_ODR(SEG_DISP3) = 0;
	 GPIOX_ODR(SEG_DISP4) = 0;

	switch(display_chosen)
	{
		case 1: GPIOX_ODR(SEG_DISP1) = 1; break;
		case 2: GPIOX_ODR(SEG_DISP2) = 1; break;
		case 3: GPIOX_ODR(SEG_DISP3) = 1; break;
		case 4: GPIOX_ODR(SEG_DISP4) = 1; break;
		default: break;
	}
}

void SevenSegment_Integer(uint16_t number, uint16_t num_repet)
{

#if TOTAL == 0
	uint8_t num_digits = log10(number);
	uint8_t res = 0;
	num_digits++;

		for(uint8_t i = num_digits; i >= 1; i--)
		{
			res = number%10;
			number = number/10;
			SevenSegment_Clear();
			SevenSegment_DisplayDigit((uint8_t) res);
			SevenSegment_SelectDisplay(i);
			delay_ms(5);
		}
#else

	for(uint8_t rep = 1; rep <= num_repet; rep++)
	{
		uint8_t res = 0;
		uint16_t div = number;
		for(uint8_t i = 4; i >= 1; i--)
		{
			res = div%10;
			div = div/10;
			SevenSegment_Clear();
			SevenSegment_DisplayDigit((uint8_t) res);
			SevenSegment_SelectDisplay(i);
			delay_ms(1);
		}
		delay_ms(1);
	}

#endif
}

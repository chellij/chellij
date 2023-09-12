#ifndef _ADCDAC_H
#define _ADCDAC_H
#include <stm32f4xx.h>
#include "GAMEPLAY.h"
#include "USART.h"
#include "LED.h"
#include "LCD.h"

//DEFINITIONS ADC
#define POT_input_pin	0				//Potentiometer input pin = PA0
#define LDR_input_pin	0				//LDR input pin = PC0
#define POT_Channel		0				
#define LDR_Channel		10

//DEFINITIONS DAC
#define DAC_pin		5						//DAC output 1 pin = PA5

//FUNCTIONS ADC
void init_POT(void);
void init_LDR(void);
unsigned short read_ADC(void);
void adclevel(void);

//FUNCTIONS DAC
void init_DAC(void);
void output_dac(unsigned short d);

#endif

#include "ADCDAC.h"

//VARIABLES
unsigned short ADC_DATA;				//current value of read_ADC
char ADCstr[7];									//ADC_DATA value converted into string to transmit

int lev[10] = {num_one,num_two,num_three,num_four,num_five,num_six,num_seven,num_eight,num_nine,num_zero};

//FUNCTIONS
void init_DAC(void)
{
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;			//DAC port clock enable
	GPIOA->MODER|=(0x03<<(2*DAC_pin));			//DAC output pin set as anaglogue
	
	RCC->APB1ENR|=RCC_APB1ENR_DACEN;				//DAC clock enable
	DAC->CR|=DAC_CR_EN2;										//DAC 2 enabled
}

void output_dac(unsigned short d)
{
	DAC->DHR12R2=d;			//write data byte to DAC 2 output register
}

void init_POT ()
{
	//ENABLE PORT(S)
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 					//ONLY GPIO A clock enable
	GPIOA->MODER |= (0x03<<(2*POT_input_pin)); 		//ADC input pin is analogue mode
	
	RCC->APB2ENR|=RCC_APB2ENR_ADC1EN; 						//ADC clock enable
	
	ADC1->SQR1&=~ADC_SQR1_L; 											//set number of conversions per sequence to 1
	ADC1->SQR3&=~ADC_SQR3_SQ1; 										//clear channel select bits
	ADC1->SQR3|=POT_Channel; 											//set channel as channel 0
	ADC1->CR2|=ADC_CR2_ADON;											//start conversion part 1
}

void init_LDR ()
{
	//ENABLE PORT(S)
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; 					//ONLY GPIO C clock enable
	GPIOC->MODER |= (0x03<<(2*LDR_input_pin)); 		//ADC input pin is analogue mode
	
	RCC->APB2ENR|=RCC_APB2ENR_ADC1EN; 						//ADC clock enable
	
	ADC1->SQR1&=~ADC_SQR1_L; 											//set number of conversions per sequence to 1
	ADC1->SQR3&=~ADC_SQR3_SQ1; 										//clear channel select bits
	ADC1->SQR3|=LDR_Channel; 											//set channel as channel 10
	ADC1->CR2|=ADC_CR2_ADON;											//start conversion part 1
}

unsigned short read_ADC()
{
	ADC1->CR2|=ADC_CR2_SWSTART; 									//start ADC conversion part 2
	while((ADC1->SR&ADC_SR_EOC)==0)								//monitor the EOC flag in the ADC1 SR register
	{
		__NOP();																		//wait for conversion complete
	}
	return ADC1->DR; 															//return converted value
}
void adclevel(void)
{		
		ADC_DATA=read_ADC();												//ADC_Data=current converted value 
		
		ADC_DATA=ADC_DATA*1000/1220;								//1.22mV per step (4096 steps) to convert into Volts
	
		ADCstr[0]='0' + (((ADC_DATA)/1000)%10);			//convert and write value into array to be printed.
		ADCstr[1]=0x2E;															//decimal point
		ADCstr[2]='0' + (((ADC_DATA)/100)%10);
		ADCstr[3]='0' + (((ADC_DATA)/10)%10);
		ADCstr[4]='0' + (((ADC_DATA)/1)%10);
		ADCstr[5]='V';															//Volts										
		ADCstr[6]='0';															//string complete
}	

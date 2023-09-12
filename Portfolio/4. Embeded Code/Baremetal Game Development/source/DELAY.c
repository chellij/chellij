#include "DELAY.h" 
#include <stdio.h>

//VARIABLES
int ADCtype=1;
int m=1;
int red=0;
int i=0;
int nextnum=0;
int gametime=0;

//EXTERNAL VARIABLES
extern char ADCstr[];								//from ADCDAC.c
extern unsigned short ADC_DATA;			//from ADCDAC.c
extern char score[]; 								//from LCD.c
extern int Death;										//from LCD.c
extern int diffi;										//from LCD.c
extern int start;										//from SWITCH.c
extern int morse;										//from LED.c
extern int morsepos;								//from LED.c
extern int whitePWM;								//from LED.c
extern int speedtimer;							//from LCD.c
extern int rundelay;								//from LCD.c
extern int flash;										//from GAMEPLAY.c

extern int gamestart;								//from GAMEPLAY.c

//FUNCTIONS

void delay_s (unsigned int time) 	//x second delay with user inputing how many x seconds when they call the function
{
	int i;
	for (i=0; i<(time*(SystemCoreClock/18)); i++);
}

void delay_ms (unsigned int time) //x milisecond delay with user inputing how many x miliseconds when they call the function
{
	int i;
	for (i=0; i<(time*(SystemCoreClock/18000)); i++);
}
void delay_us (unsigned int time) //x microsecond delay with user inputing how many x microseconds when they call the function
{
	int i;
	for (i=0; i<(time*(SystemCoreClock/18000000)); i++);
}
void Init_Timer2(int flash)				//Set up each timer to run at specific speeds
{
	//if flash = 351562, 1 second timer			
	RCC->APB1ENR|=RCC_APB1ENR_TIM2EN;		//timer 2 clock enabled
	TIM2->DIER|=TIM_DIER_UIE;						//timer uptdate interrupt enabled
																			//APB clock is Fcy/2 = 180MHz/2 = 90MHz
	TIM2->PSC=256-1;										//divide APB clock by 256 = 90MHz/256 = 351kHz
	TIM2->ARR=flash;										//counter reload value, gives a timer period of 100ms when F_APB = 90MHz and PSC = 256
	TIM2->CNT=0;												//zero timer counter
	NVIC->ISER[0]|=(0x01<<28);					//timer 2 global interrupt enabled
	TIM2->CR1|=TIM_CR1_CEN;							//start timer counter
}

void Init_Timer3(int pitch)
{
	RCC->APB1ENR |=RCC_APB1ENR_TIM3EN;	//timer 3 clock enabled
	TIM3->DIER|=TIM_DIER_UIE;						//timer uptdate interrupt enabled
																			//APB clock is Fcy/2 = 180MHz/2 = 90MHz
	TIM3->PSC=128-1;										//divide APB clock by 128 = 90MHz/128 = 703kHz
	TIM3->ARR=pitch;										//counter reload value depends on the pitch/frequency of note required
	TIM3->CNT=0;												//zero timer counter
	NVIC->ISER[0]=(0x01<<29);						//timer 3 global interrupt enabled
	TIM3->CR1|=TIM_CR1_CEN;							//start timer counter
}
void Init_Timer4(void)
{

	RCC->APB1ENR|=RCC_APB1ENR_TIM4EN;		//timer 4 clock enabled
	TIM4->DIER|=TIM_DIER_UIE;						//timer uptdate interrupt enabled
																			//APB clock is Fcy/2 = 180MHz/2 = 90MHz
	TIM4->PSC=512;											//divide APB clock by 512 = 90MHz/512 = 17.5781kHz
	TIM4->ARR=18000;										//counter reload value
	TIM4->CNT=0;												//zero timer counter
	NVIC->ISER[0]|=(0x01<<30);					//timer 4 global interrupt enabled
	TIM4->CR1|=TIM_CR1_CEN;							//start timer counter
}

void Init_Timer4_off(void)
{
	//same as Init_Timer4 except ARR=0 so that it doesnt run.
	RCC->APB1ENR|=RCC_APB1ENR_TIM4EN;		
	TIM4->DIER|=TIM_DIER_UIE;						
																			
	TIM4->PSC=512;											
	TIM4->ARR=0;										
	TIM4->CNT=0;												
	NVIC->ISER[0]|=(0x01<<30);					
	TIM4->CR1|=TIM_CR1_CEN;							
}

void Init_Timer5(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;	//timer 5 clock enabled
	TIM5->DIER|=TIM_DIER_UIE;						//timer uptdate interrupt enabled
																			//APB clock is Fcy/2 = 180MHz/2 = 90MHz
	TIM5->PSC=256-1;										//divide APB clock by 256 = 90MHz/512 = 35kHz
	TIM5->ARR=351562/10;								//counter reload value
	TIM5->CNT=0; 												//zero timer counter
	NVIC->ISER[1] |= 1<<18;							//timer 4 global interrupt enabled - uses ISER[1] as timer5 is on bit 50 and each ISE Register only has 32 bits
	TIM5->CR1 |= TIM_CR1_CEN; 					//start timer counter
}

void Init_Timer5_off(void)
{
	//same as Init_Timer5except ARR=0 so that it doesnt run.
	RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
	TIM5->DIER|=TIM_DIER_UIE;
	
	TIM5->PSC=256-1;	
	TIM5->ARR=0;
	TIM5->CNT=0; 
	NVIC->ISER[1] |= 1<<18;
	TIM5->CR1 |= TIM_CR1_CEN; // Enable Counting
}

void TIM2_IRQHandler(void)					//WHITE LED
{
	TIM2->SR&=~TIM_SR_UIF;						// clear IRQ flag in TIM2			
	whiteodLED_Tog();									//Call white led toggle function
}

void TIM3_IRQHandler(void)					//BUZZER
{
	TIM3->SR&=~TIM_SR_UIF;						// clear IRQ flag in TIM3
	BuzzTog();												//Call buzzer toggle function
}

void TIM4_IRQHandler(void)					//Green 5 Hz, Red sampling, ADC, POT, random number and score to PuTTY
{
	TIM4->SR&=~TIM_SR_UIF;						// clear IRQ flag in TIM4

	if (start==0)											//before game start, only need POT and difficulty to show on USART
	{
		init_POT();											//to ensure POT is being read and use the correct channel, must call init_pot()
		printPOT();											//prints POT level in volts to USART
		difficultystart();							//starts difficulty level according to potentiometer
		difficultylevel();							//prints difficulty level to 7Seg
		printdiffi();										//prints difficulty level to USART
	}
	
	if (start==1 && Death==0)					//once game has started, everything gets written to USART
	{	
		if(ADCtype==1)
		{
			init_POT();										//to ensure POT is being read and use the correct channel, must call init_pot()
			printPOT();										//prints POT level in volts to USART
			printdiffi();									//prints difficulty level to USART
		}
		if(ADCtype==0)
		{	
			init_LDR();										//to ensure LDR is being read and use the correct channel, must call init_ldr()
			printLDR();										//prints LDR level in volts to USART
			roundscore();									//prints current score to USART
			printduration();							//prints game duration to USART
			printdice();									//prints random number between 1 and 6 to USART
		}	
		ADCtype=!ADCtype;								//so this doesn't happen to fast, the LDR and POT are read on alternate callings of the interrupt handler so after it has run , ADCtype must be flipped.
	}
	if (red==0 || red==2 || red==4 || red==6 || red==8 || red==10) {greenflash();}		//flash green LED every other calling (200ms / 5hZ)
	if (red==5 || red==10){	redLED_sample(); }																				//flash red LED every 5 callings (500ms) 0n+off=1sec
	if (red==10) {if (gamestart==1)	{ gametime++; } }																	//every 10 callings (1second) gametime (duration) increments
	red++;
	if (red==11) {red=0;}																															//if red is 11, red goes back to 0
}
void TIM5_IRQHandler(void)																		//Morsecode
{
	TIM5->SR&=~TIM_SR_UIF;																			// clear IRQ flag in TIM5
	
	if(m==1){morsecode();}																			//if m = 1 , run morsecode()
	
	if(m==0 && morsepos<6){morseoff(); m=!m;}										//if m = 0 and morsepos is under 6, turn blue LED off and swap m to 1
	if(m==0 && morsepos==6){morseoff(); nextnum++;							//if m = 0 and morsepos is 6, turn blue LED off and increment nextnum++ - this will happen three times so the gap between digits is 3x the gap between flashes.
			if(nextnum==3){nextnum=0;m=!m;}}												//once nextnum reaches 3, it is reset and m swaps to 1.
	if (morsepos==6){morsepos=0; i++; 													//if morsepos = 6, i is incremented
		if(i>3){i=0;}}																						//if i is 3, i goes back to 0
}

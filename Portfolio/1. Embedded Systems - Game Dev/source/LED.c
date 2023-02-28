#include "LED.h" 
extern int i;							//from DELAY.c
extern char mscore[]; 		//from LCD.c
extern int m;							//from DELAY.c
extern int speedtimer;		//from LCD.c

//VARIABLES
int whitePWM=0;						//variable to help toggle White OD LED
int redSample=0;					//variable to help toggle Red Nucleo LED
int greenon=0;						//variable to help toggle Green Nucleo LED
int morsepos=0;						//variable to move along Morse Code posisiton
int dash=0;								//variable to repeat morse dot x3 (dash length = 3x dot length)

//-------------all initiate functions-------------//
void odLED_init (void) 			
{
	RCC->AHB1ENR = RCC_AHB1ENR_GPIOCEN; 																						//Enable Port C (RED,BLUE,GREEN OPEN DRAIN)
	GPIOC->MODER |= (0x01<<(2*odred)) | (0x1<<(2*odyellow)) | (0x1<<(2*odgreen));		//Set Port:C 		pins:7,8,9
	GPIOC->MODER &= ~((2<<(2*odred)) | (2<<(2*odyellow)) | (2<<(odgreen*2))); 			//Clear Port:C 	pins:7,8,9
	GPIOC->OTYPER |= (1<<(odred) | (1<<(odyellow)) | (1<<odgreen)); 								//Set Port:C 		pins:7,8,9 to open drain (OType 0-Push Pull, 1-Open Drain)
	GPIOC->BSRR = (1<<(odred) | (1<<(odyellow)) | (1<<(odgreen))); 									//Set Port:C 		pins 7,8,9 to off/low
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN;																						//Enable Port F (WHITE OPEN DRAIN)
	GPIOF->MODER |= (0x01<<(2*odwhite));																						//Set Port:F 		pin:10
	GPIOF->MODER &= ~(2<<(2*odwhite));																							//Clear Port:F 	pin:10
	GPIOF->OTYPER |= (1<<(odwhite));																								//Set Port:F 		pin:10 to open drain (OType 0-Push Pull, 1-Open Drain)
	GPIOF->BSRR = (1<<(odwhite));																										//Set Port:F 		pin:10 to off/low
}
void ppLED_init(void) 			
{	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;																						//Enable Port C (RED,BLUE,GREEN PUSH PULL)
	GPIOC->MODER |= (0x01<<(2*ppred)) | (0x1<<(2*ppyellow)) | (0x1<<(2*ppgreen));		//Set Port:C 		pins:2,3,6
	GPIOC->MODER &= ~((2<<(2*ppred)) | (2<<(2*ppyellow)) | (2<<(2*ppgreen)));				//Clear Port:B 	pins:2,3,6
}
void nucleoLED_init(void) 	
{	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;																						//Enable Port B (RED,BLUE,GREEN)
	GPIOB->MODER |= (0x01<<(2*nucleogreen)) | (0x1<<(2*nucleoblue)) | (0x1<<(2*nucleored));		//Set Port:B 		pins:0,7,14
	GPIOB->MODER &= ~((2<<(2*nucleogreen)) | (2<<(2*nucleoblue)) | (2<<(nucleored*2)));				//Clear Port:B 	pins:0,7,14
}
void ssLED_init(void)			
{
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;																																																						//Enable Port E (SEVEN SEG)
	
  GPIOE->MODER |= (0x01<<(2*seg_A) | 1<<(2*seg_B) | 1<<(2*seg_C) | 1<<(2*seg_D) | 1<<(2*seg_E) | 1<<(2*seg_F) | 1<<(2*seg_G) | 1<<(2*seg_DP)); 		//Set Port E: 	pins E2-E9 (A-G) to output mode 0x01
  GPIOE->MODER &= ~(2<<(2*seg_A) | 2<<(2*seg_B) | 2<<(2*seg_C) | 2<<(2*seg_D) | 2<<(2*seg_E) | 2<<(2*seg_F) | 2<<(2*seg_G) | 2<<(2*seg_DP));			//Clear Port E: pins E2-E29 (A-G)
  
  GPIOE->MODER |= (1<<(2*seg_LE1) | 1<<(2*seg_LE2)); 																																															//Set Port E: 	pins 10,11 as latches
  GPIOE->MODER &= ~(2<<(2*seg_LE1) | 2<<(2*seg_LE2));																																															//Clear Port E: pins 10,11 as latches
 
	GPIOE->MODER |= 0x01<<(2*seg_OE); 																																																							//Set Port E: 	pin 15 (7SEG !OE) to output mode 0x01
  GPIOE->BSRR = 0x00<<(2*seg_OE); 																																																								//Turn Port E: 	pin 15 (7SEG !OE) on (active low)		
}
void rgbLED_init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;																																					//Enable Port E (SEVEN SEG)
  GPIOE->MODER |= 0x01<<(2*rgbOE); 																																							//Set Port E: 	pins 0 (RGB !OE) to output mode 0x01
  GPIOE->BSRR = 0x00<<(2*rgbOE); 																																								//Turn Port E: 	pin 0 (RGB !OE) on (active low)
	
  GPIOE->MODER |= (1<<(2*rgbRedLatch) | 1<<(2*rgbGreenLatch) | 1<<(2*rgbBlueLatch)); 														//Set Port E: 	pins 12,13,14 as latches
  GPIOE->MODER &= ~(2<<(2*rgbRedLatch) | 2<<(2*rgbGreenLatch) | 2<<(2*rgbBlueLatch));														//Clear Port E: pins 12,13,14 as latches

	GPIOE->MODER |= (1<<(2*2) | 1<<(2*3) | 1<<(2*4) | 1<<(2*5) | 1<<(2*6) | 1<<(2*7)  | 1<<(2*8) | 1<<(2*9));  		//Set Port E: 	pins E2-E9 (D0-D7) to output mode 0x01
  GPIOE->MODER &= ~(2<<(2*2) | 2<<(2*3) | 2<<(2*4) | 2<<(2*5) | 2<<(2*6) | 2<<(2*7)  | 2<<(2*8) | 2<<(2*9));		//Clear Port E:	pins E2-E9 (D0-D7)  
}
void Led_init(void) 			
{	
	/* Call each function individually to initialise them all */
	odLED_init();	
	ppLED_init();
	nucleoLED_init();
	ssLED_init();	
	//rgbLED_init(); init later so it doesnt light up immediately with 7seg
} 

//-------------nucleo board-------------//
void redLED_sample(void)			/*Function to toggle red LED (PB14) each time it is called, the LED turns on/off*/
{
	if(redSample==0)			{GPIOB->BSRR=(1<<nucleored);}
	else if(redSample==1)	{GPIOB->BSRR=(1<<(nucleored+16));}
	redSample=!redSample;
}

void greenflash(void)					/*Function to toggle green LED (PB0) each time it is called, the LED turns on/off*/
{
	if(greenon==0)				{GPIOB->BSRR=(1<<nucleogreen);}
	else if(greenon==1)		{GPIOB->BSRR=(1<<(nucleogreen+16));}
	greenon=!greenon;
}

void morseon(void)			{GPIOB->BSRR=1<<(7);}					/*Function to turn green LED (PB7) on - used within morsecode function*/
void morseoff(void)			{GPIOB->BSRR=(1<<(7+16));}		/*Function to turn green LED (PB7) off - used within morsecode function*/

/* A description of morsecode() as it is quite complicated. 
	 1) morsecode() is called by an interrupt timer which runs every 100ms as this is the length of a "dot".
	 2) the interrupt timer will either call morsecode() which will turn the blue led on or will enter a statement inside the interrupt to turn the blue led off.
	 3) each time morsecode() is called, only one part of a digit will be called this is where morsepos comes in, if morsepos is in position 0, the first part will be called etc.
	 4) variable i increments between 0 and 2 to indicate which digit of the score is being displayed.
	 5) e.g. if msocre[] is 001, .---- .---- ..--- will be flashed eventually, however while morsepos=0 and i=2, only one dot will happen, when morsepos increases to 1, i will still be 2 and a dash will be called.
	 6) if a dot is required, the code simply turns on the blue led, increments morsepos and switches m from 1 to 0 so the next occurance will be the interrupt turning the led off.
	 7) if a dash is required, the code also turns the blue led on, however this time it instead increments a variable "dash". This means the next occurance of the interrupt won't tun off
			the led but will instead bring it back into the same place off the code and "dash" will increment again, this happens three times so that the dash length becomes 3x that of a dot which
			is the standard length.
	 8) once "dash" = 3, dash is reset, morsepos is incremented and m is switched again just like when dot is finished.
	 9) although it is a long chunk of code, only one line will be called every 100ms
	10) after morsepos = 5, it will be reset in the interrupt handler and i will be increased so that the next value is displayed

A KEY OF MORSE CODE:	zero 	.----		one 	..---		two 	...--		three ....-		four 	.....		five 	-....		six 	--...		seven ---..		eight ----.		nine	-----		*/

void morsecode(void)
{	
	if (morsepos==0 && m==1)  		//.....-----										
	{
		if (mscore[i]==0) {morseon(); morsepos++; m=!m;}		//dot			
		if (mscore[i]==1) {morseon(); morsepos++; m=!m;}		//dot			
		if (mscore[i]==2) {morseon(); morsepos++; m=!m;}		//dot			
		if (mscore[i]==3) {morseon(); morsepos++; m=!m;}		//dot		
		if (mscore[i]==4) {morseon(); morsepos++; m=!m;}		//dot
		if (mscore[i]==5) {morseon(); dash++;								//dash
				if (dash==3){ dash=0;morsepos++;m=!m;}}
		if (mscore[i]==6) {morseon(); dash++;								//dash
				if (dash==3){ dash=0;morsepos++;m=!m;}}
		if (mscore[i]==7) {morseon(); dash++;								//dash
				if (dash==3){ dash=0;morsepos++;m=!m;}}
		if (mscore[i]==8) {morseon(); dash++;								//dash
				if (dash==3){ dash=0;morsepos++;m=!m;}}
		if (mscore[i]==9) {morseon(); dash++;								//dash
				if (dash==3){ dash=0;morsepos++;m=!m;}}
	}
	if (morsepos==1 && m==1) 	//-.....----
	{
		if (mscore[i]==0) {morseon(); dash++;								//dash
				if (dash==3){ dash=0;morsepos++;m=!m;}}
		if (mscore[i]==1) {morseon(); morsepos++; m=!m;}		//dot
		if (mscore[i]==2) {morseon(); morsepos++; m=!m;}		//dot
		if (mscore[i]==3) {morseon(); morsepos++; m=!m;}		//dot
		if (mscore[i]==4) {morseon(); morsepos++; m=!m;}		//dot
		if (mscore[i]==5) {morseon(); morsepos++; m=!m;}		//dot
		if (mscore[i]==6) {morseon(); dash++;								//dash
				if (dash==3){ dash=0;morsepos++;m=!m;}}
		if (mscore[i]==7) {morseon(); dash++;								//dash
				if (dash==3){ dash=0;morsepos++;m=!m;}}
		if (mscore[i]==8) {morseon(); dash++;								//dash
				if (dash==3){ dash=0;morsepos++;m=!m;}}
		if (mscore[i]==9) {morseon(); dash++;								//dash
				if (dash==3){ dash=0;morsepos++;m=!m;}}
	}
	if (morsepos==2 && m==1) 	//--.....---
	{
		if (mscore[i]==0) {morseon(); dash++;								//dash
				if (dash==3){ dash=0;morsepos++;m=!m;}}
		if (mscore[i]==1) {morseon(); dash++;								//dash
				if (dash==3){ dash=0;morsepos++;m=!m;}}
		if (mscore[i]==2) {morseon(); morsepos++; m=!m;}		//dot
		if (mscore[i]==3) {morseon(); morsepos++; m=!m;}		//dot
		if (mscore[i]==4) {morseon(); morsepos++; m=!m;}		//dot
		if (mscore[i]==5) {morseon(); morsepos++; m=!m;}		//dot
		if (mscore[i]==6) {morseon(); morsepos++; m=!m;}		//dot
		if (mscore[i]==7) {morseon(); dash++;								//dash
				if (dash==3){ dash=0;morsepos++;m=!m;}}
		if (mscore[i]==8) {morseon(); dash++;								//dash
				if (dash==3){ dash=0;morsepos++;m=!m;}}
		if (mscore[i]==9) {morseon(); dash++;								//dash
				if (dash==3){ dash=0;morsepos++;m=!m;}}
	}
	if (morsepos==3 && m==1) 	//---.....--
	{
		if (mscore[i]==0) {morseon(); dash++;								//dash
				if (dash==3){ dash=0;morsepos++;m=!m;}}
		if (mscore[i]==1) {morseon(); dash++;								//dash
				if (dash==3){ dash=0;morsepos++;m=!m;}}
		if (mscore[i]==2) {morseon(); dash++;								//dash
				if (dash==3){ dash=0;morsepos++;m=!m;}}
		if (mscore[i]==3) {morseon(); morsepos++; m=!m;}		//dot
		if (mscore[i]==4) {morseon(); morsepos++; m=!m;}		//dot
		if (mscore[i]==5) {morseon(); morsepos++; m=!m;}		//dot
		if (mscore[i]==6) {morseon(); morsepos++; m=!m;}		//dot
		if (mscore[i]==7) {morseon(); morsepos++; m=!m;}		//dot
		if (mscore[i]==8) {morseon(); dash++;								//dash
				if (dash==3){ dash=0;morsepos++;m=!m;}}
		if (mscore[i]==9) {morseon(); dash++;								//dash
				if (dash==3){ dash=0;morsepos++;m=!m;}}
	}
	if (morsepos==4 && m==1) 	//----.....-
	{
		if (mscore[i]==0) {morseon(); dash++;								//dash
				if (dash==3){ dash=0;morsepos++;m=!m;}}	
		if (mscore[i]==1) {morseon(); dash++;								//dash
				if (dash==3){ dash=0;morsepos++;m=!m;}}
		if (mscore[i]==2) {morseon(); dash++;								//dash
				if (dash==3){ dash=0;morsepos++;m=!m;}}	
		if (mscore[i]==3) {morseon(); dash++;								//dash
				if (dash==3){ dash=0;morsepos++;m=!m;}}
		if (mscore[i]==4) {morseon(); morsepos++; m=!m;}		//dot
		if (mscore[i]==5) {morseon(); morsepos++; m=!m;}		//dot
		if (mscore[i]==6) {morseon(); morsepos++; m=!m;}		//dot
		if (mscore[i]==7) {morseon(); morsepos++; m=!m;}		//dot
		if (mscore[i]==8) {morseon(); morsepos++; m=!m;}		//dot
		if (mscore[i]==9) {morseon(); dash++;								//dash
				if (dash==3){ dash=0;morsepos++;m=!m;}}
	}
	if (morsepos==5 && m==1) 	//-----.....
	{
		if (mscore[i]==0) {morseon(); dash++;								//dash
				if (dash==3){ dash=0;morsepos++;m=!m;}}
		if (mscore[i]==1) {morseon(); dash++;								//dash
				if (dash==3){ dash=0;morsepos++;m=!m;}}
		if (mscore[i]==2){morseon(); dash++;								//dash
				if (dash==3){ dash=0;morsepos++;m=!m;}}
		if (mscore[i]==3) {morseon(); dash++;								//dash
				if (dash==3){ dash=0;morsepos++;m=!m;}}
		if (mscore[i]==4) {morseon(); dash++;								//dash
				if (dash==3){ dash=0;morsepos++;m=!m;}}
		if (mscore[i]==5) {morseon(); morsepos++; m=!m;}		//dot
		if (mscore[i]==6) {morseon(); morsepos++; m=!m;}		//dot
		if (mscore[i]==7) {morseon(); morsepos++; m=!m;}		//dot
		if (mscore[i]==8) {morseon(); morsepos++; m=!m;}		//dot
		if (mscore[i]==9) {morseon(); morsepos++; m=!m;}		//dot
	}
}

//-----------open drain---------//

/*Individual functions for different stages of traffic light system*/
void odLED_Ready (void) 		{GPIOC->BSRR = (1<<(odred+16));}												//Set port:B pin 7 (RedLED) to high
void odLED_ReadyOff (void)	{GPIOC->BSRR = (1<<(odred));}														//Set port:B pin 7 to low
void odLED_Set (void)				{GPIOC->BSRR = (1<<(odred+16) | (1<<(odyellow+16)));}		//Set port:B pin 7,8 (Red+AmberLED) to high
void odLED_SetOff (void)		{GPIOC->BSRR = (1<<(odred) | (1<<(odyellow)));}					//Set port:B pins 7,8 to low
void odLED_Go (void)				{GPIOC->BSRR = (1<<(odgreen+16));}											//Set port:B pin 9 (GreenLED) to high
void odLED_GoOff (void)			{GPIOC->BSRR = (1<<(odgreen));}													//Set port:B pin 9 to low


void trafficlights(int delay)		/*function to do all traffic light functions one after another with a small delay decided as function is called */							
{
	odLED_Ready();
	delay_ms(delay);
	odLED_ReadyOff();
	delay_ms(delay);
	odLED_Set();
	delay_ms(delay);
	odLED_SetOff();
	delay_ms(delay);
	odLED_Go();
	delay_ms(delay);
	odLED_GoOff();
}
void whiteodLED_Tog(void)				/*Function to toggle white LED (PF10) each time it is called, the LED turns on/off*/
{
	if(whitePWM==0)	{GPIOF->BSRR=(1<<(odwhite));}
	if(whitePWM==1)	{GPIOF->BSRR=(1<<(odwhite+16));}
	whitePWM=!whitePWM;								//Toggle variable so when whiteodLED_PWM is next entered it runs other if statement
}

//----------seven segment-------//

void ss_tens(int digit)					//Function to write required tens digit to 7seg - if double digit number should be used inside a repeating loop with ss_ones to flash fast enough to see
{
  ss_strobeL();									//Strobes the tens(left hand)digit
  GPIOE->BSRR = digit;         	//writes digit on 7seg (tens pos)
  ss_strobeL();									//Strobes the tens(left hand)digit to lock the LEDs in
	delay_ms(8);									//delay to make sure the human eye can register the value
	ss_clrL();										//clear the ones digit.
}

void ss_ones(int digit)					//Function to write required ones digit to 7seg - if double digit number should be used inside a repeating loop with ss_tens to flash fast enough to see
{
	ss_strobeR();									//Strobes the ones(right hand)digit
  GPIOE->BSRR = digit;         	//Writes digit on 7seg	(ones pos)
  ss_strobeR();									//Strobes the ones(right hand)digit to lock the LEDs in
	delay_ms(8);									//delay to make sure the human eye can register the value
	ss_clrR();										//clear the ones digit.
}	

void ss_clrL(void)							//Clear 7seg tens digit from data bus
{
  ss_strobeL();									//Strobes the tens(left hand)digit
	GPIOE->BSRR = clear_ss;				//Clears all LEDs on 7seg	(tens pos)
  ss_strobeL();									//Strobes the tens(left hand)digit
}

void ss_clrR(void)							//Clear 7segment ones digit from data bus
{
  ss_strobeR();									//Strobes the ones(right hand)digit
	GPIOE->BSRR = clear_ss;				//Clears all LEDs on 7seg	(ones pos)
  ss_strobeR();									//Strobes the ones(right hand)digit
}

void	ss_strobeL(void)					//Strobe 7seg tens digit to latch
{
  GPIOE->BSRR = 0x01<<27;  			//Resets pin11 (LE2)(0x01 << 27)
  GPIOE->BSRR = 0x01<<10;   		//Sets pin10 (LE1)  (0x01 << 10)
  GPIOE->BSRR = 0x00<<10;				//Clears pin10 (LE1)(0x00 << 10)
}

void ss_strobeR(void) 					//Strobe 7seg ones digit to latch
{
  GPIOE->BSRR = 0x01<<26;  			//Resets pin10 (LE1)
	GPIOE->BSRR = 0x01<<11;  			//Sets pin 11 (LE2)
	GPIOE->BSRR = 0x00<<11; 			//Clears pin11 (LE2)
}

void ss_difficulty(int digit)		//Function to display current difficulty level on 7seg (levels 0-9)
{
	ss_strobeR();									//Strobes the ones(right hand)digit
  GPIOE->BSRR = digit;          //Writes variable "digit" on seg7 (ones pos)
	ss_strobeR();									//Strobes the ones(right hand)digit
}

//------------rgb bar-----------//

void rgbLED_clr(void)
{
	rstrobe();										//Strobe RGB bar RED LEDs to latch
	gstrobe();										//Strobe RGB bar GREEN LEDs to latch
	bstrobe();										//Strobe RGB bar BLUE LEDs to latch
	GPIOE->BSRR = clrRGB;					//Set ALL rgb leds to OFF
}

void rgbLED_all(void)
{
	rstrobe();										//Strobe RGB bar RED LEDs to latch
	gstrobe();										//Strobe RGB bar GREEN LEDs to latch
	bstrobe();										//Strobe RGB bar BLUE LEDs to latch
  GPIOE->BSRR = allRGB;					//Set ALL rgb leds to ON
}

void rstrobe(void)
{
	GPIOE->BSRR = 0x01<<(rgbGreenLatch+16);  	//Resets pin13 (GreenLatch)	(0x01 << 29)
	GPIOE->BSRR = 0x01<<(rgbBlueLatch+16); 		//Resets pin14 (BlueLatch)	(0x01 << 30)
  GPIOE->BSRR = 0x01<<rgbRedLatch;   				//Sets pin12 (RedLatch)  		(0x01 << 10)
  GPIOE->BSRR = 0x00<<rgbRedLatch;					//Clears pin12 (RedLatch)		(0x00 << 10)
}
void r_on (void)
{
	rstrobe();										//Strobe RGB bar RED LEDs to latch
	GPIOE->BSRR = allRGB; 				//Set ALL RED rgb leds to ON
}

void r_off(void)
{
	rstrobe();										//Strobe RGB bar RED LEDs to latch
	GPIOE->BSRR = clrRGB; 				//Set ALL RED rgb leds to OFF
}

void gstrobe(void)
{
	GPIOE->BSRR = 0x01<<(rgbRedLatch+16);   	//Resets pin12 (RedLatch)		(0x01 << 28)
	GPIOE->BSRR = 0x01<<(rgbBlueLatch+16);		//Resets pin14 (BlueLatch)	(0x01 << 30)
  GPIOE->BSRR = 0x01<<rgbGreenLatch;  			//Sets pin13 (GreenLatch)  	(0x01 << 10)
  GPIOE->BSRR = 0x00<<rgbGreenLatch;				//Clears pin13 (GreenLatch)	(0x00 << 10)
}
void g_on(void)
{	
	gstrobe();										//Strobe RGB bar GREEN LEDs to latch
	GPIOE->BSRR = allRGB;  				//Set ALL GREEN rgb leds to ON
}

void g_off(void)
{
	gstrobe();										//Strobe RGB bar GREEN LEDs to latch
	GPIOE->BSRR = clrRGB;  				//Set ALL GREEN rgb leds to OFF
}

void bstrobe(void)
{
	GPIOE->BSRR = 0x01<<(rgbRedLatch+16);  		//Resets pin12 (RedLatch)		(0x01 << 28)
	GPIOE->BSRR = 0x01<<(rgbGreenLatch+16); 	//Resets pin13 (GreenLatch)	(0x01 << 29)
  GPIOE->BSRR = 0x01<<rgbBlueLatch;   			//Sets pin13 (BlueLatch)  	(0x01 << 10)
  GPIOE->BSRR = 0x00<<rgbBlueLatch;					//Clears pin13 (BlueLatch)	(0x00 << 10)
}
void b_on(void)
{
	bstrobe();										//Strobe RGB bar BLUE LEDs to latch
	GPIOE->BSRR = allRGB;  				//Set ALL BLUE rgb leds to ON
}
void b_off(void)
{
	bstrobe();										//Strobe RGB bar BLUE LEDs to latch
	GPIOE->BSRR = clrRGB;  				//Set ALL BLUE rgb leds to OFF
}

#include "BUZZER.h"

void Buzzer_init(void)
{	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;	//Enable Port B for Buzzer (PB13)
	
	GPIOB->MODER |=(0x01<<13*2);					//Clear pin and Set PB13 as output
	GPIOB->MODER &= ~(2<<13*2);
}
void BuzzTog(void)
{
	GPIOB->ODR ^= (0x01<<13);							//Toggle the buzzer on and off - used in interrupt.
}
void BuzzStartTune(void)								//Changing frequencies with small pauses makes a tune.
{
	Init_Timer3(NOTE_C5);
	delay_ms(120);
	Init_Timer3(0);
	delay_ms(120);
	Init_Timer3(0);
	delay_ms(120);
	Init_Timer3(NOTE_G4);
	delay_ms(120);
	Init_Timer3(0);
	delay_ms(120);
	Init_Timer3(0);
	delay_ms(120);
	Init_Timer3(NOTE_E4);
	delay_ms(120);
	Init_Timer3(0);
	delay_ms(120);
	Init_Timer3(0);
	delay_ms(120);
	Init_Timer3(NOTE_A4);
	delay_ms(120);
	Init_Timer3(0);
	delay_ms(120);
	Init_Timer3(NOTE_B4);
	delay_ms(120);
	Init_Timer3(0);
	delay_ms(120);
	Init_Timer3(NOTE_AS4);
	delay_ms(120);
	Init_Timer3(NOTE_A4);
	delay_ms(120);
	Init_Timer3(0);
	delay_ms(120);
	
	Init_Timer3(NOTE_G4);
	delay_ms(90);
	Init_Timer3(NOTE_E5);
	delay_ms(90);
	Init_Timer3(NOTE_G5);
	delay_ms(90);
	Init_Timer3(NOTE_A5);
	delay_ms(120);
	Init_Timer3(0);
	delay_ms(120);
	Init_Timer3(NOTE_F5);
	delay_ms(120);
	Init_Timer3(NOTE_G5);
	delay_ms(120);
	Init_Timer3(0);
	delay_ms(120);
	Init_Timer3(NOTE_E5);
	delay_ms(120);
	Init_Timer3(0);
	delay_ms(120);
	Init_Timer3(NOTE_C5);
	delay_ms(120);
	Init_Timer3(NOTE_D5);
	delay_ms(120);
	Init_Timer3(NOTE_B4);
	delay_ms(120);
	Init_Timer3(0);
	delay_ms(120);
	Init_Timer3(0);
	delay_ms(120);
}

void BuzzStartShort(void)								//Changing frequencies with small pauses makes a longer tune.
{
	Init_Timer3(NOTE_E5);
	delay_ms(120);
	Init_Timer3(NOTE_E5);
	delay_ms(120);
	Init_Timer3(0);
	delay_ms(120);
	Init_Timer3(NOTE_E5);
	delay_ms(120);
	Init_Timer3(0);
	delay_ms(120);
	Init_Timer3(NOTE_C5);
	delay_ms(120);
	Init_Timer3(NOTE_E5);
	delay_ms(120);
	Init_Timer3(0);
	delay_ms(120);
	Init_Timer3(NOTE_G5);
	delay_ms(120);
	Init_Timer3(0);
	delay_ms(120);
	Init_Timer3(0);
	delay_ms(120);
	Init_Timer3(0);
	delay_ms(120);
	Init_Timer3(NOTE_G5);
	delay_ms(120);
	Init_Timer3(0);
	delay_ms(120);
	Init_Timer3(0);
	delay_ms(120);
	Init_Timer3(0);
	delay_ms(120);
}

void BuzzDeathTune(void)								//Changing frequencies with small pauses makes a death tune.
{
	Init_Timer3(NOTE_E5);
	delay_ms(300);
	Init_Timer3(0);
	delay_ms(120);
	Init_Timer3(NOTE_D5);
	delay_ms(300);
	Init_Timer3(0);
	delay_ms(120);
	Init_Timer3(NOTE_A6);
	delay_ms(300);
	Init_Timer3(0);
	delay_ms(120);
}
void ready321go(void)										//Function to beep buzzer, flash leds and print on lcd.
{		
	rgbLED_init();							//initiates RGB bar LEDs
	rgbLED_all();								//turn on all leds of RGB bar (they will reflect the score on 7seg as the game plays)
	printLCD1("3");							//print 3 to LCD
	Init_Timer3(NOTE_C5);				//start buzzer interrupt at pitch middle C
	delay_ms(300);							//interrupt continues over delay for 300s
	Init_Timer3(0);							//interrupt ends (pitch 0)
	delay_ms(300);							//pause
	
	odLED_Ready();							//turn on Open Drain Red LED
	printLCD1("2");							//print 2 to LCD
	Init_Timer3(NOTE_C5);				//start buzzer interrupt at pitch middle C
	delay_ms(300);							//interrupt continues over delay for 300s
	Init_Timer3(0);							//interrupt ends (pitch 0)
	delay_ms(300);							//pause
	odLED_ReadyOff();						//turn off Open Drain Red LED
	
	odLED_Set();								//turn on Open Drain Red+Amber LED
	printLCD1("1");							//print 1 to LCD
	Init_Timer3(NOTE_C5);				//start buzzer interrupt at pitch middle C
	delay_ms(300);							//interrupt continues over delay for 300s
	Init_Timer3(0);							//interrupt ends (pitch 0)
	delay_ms(300);							//pause
	odLED_SetOff();							//turn off Open Drain Red+Amber LED
	
	odLED_Go();									//turn on Open Drain Green LED
	printLCD1("Go");						//print GO to LCD
	Init_Timer3(NOTE_C4);				//start buzzer interrupt an octave higher than middle C
	delay_ms(300);							//interrupt continues over delay for 300s
	Init_Timer3(0);							//interrupt ends (pitch 0)
	delay_ms(300);							//pause
	odLED_GoOff();							//turn off Open Drain Greeb LED
}

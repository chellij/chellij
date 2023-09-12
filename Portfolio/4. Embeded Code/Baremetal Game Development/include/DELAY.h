#ifndef __DELAY_H
#define __DELAY_H
#include <stm32f4xx.h>
#include "ADCDAC.h"
#include "USART.h"
#include "BUZZER.h"
#include "SWITCH.h"
#include "LED.h"

void delay_s (unsigned int time);  	//x second delay with user inputing how many x seconds when they call the function
void delay_ms(unsigned int time);  	//x milisecond delay with user inputing how many x miliseconds when they call the function
void delay_us (unsigned int time);  //x microsecond delay with user inputing how many x microseconds when they call the function

void Init_Timer2(int flash); //Set up each timer to run at specific speeds
void Init_Timer3(int pitch);		
void Init_Timer4(void);		
void Init_Timer4_off(void);		
void Init_Timer5(void);	
void Init_Timer5_off(void);
	
void TIM2_IRQHandler(void);	//white flash speed
void TIM3_IRQHandler(void);	//buzzer
void TIM4_IRQHandler(void);	//green 5HZ, info to PuTTy
void TIM5_IRQHandler(void);	//morse code

#endif //DELAY_Header

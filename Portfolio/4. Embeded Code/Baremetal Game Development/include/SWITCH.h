#ifndef __SWITCH_H
#define __SWITCH_H
#include <stm32f4xx.h>
#include "DELAY.h"
#include "LED.h"

//Defines pins for each button for ease of reading
#define ButtonA 0					//(PG0)
#define ButtonB 1					//(PG1)
#define ButtonC (2)				//(PG2)
#define ButtonD (3)				//(PG3)
#define UserButton (13)		//(PC13)

//Defines the value required to read each button as an imput so can be read quickly.
#define gameA (GPIOG->IDR & (1<<ButtonA))		//if button press is registered, gameA=1 
#define gameB (GPIOG->IDR & (1<<ButtonB))		//if button press is registered, gameB=2 
#define gameC (GPIOG->IDR & (1<<ButtonC))		//if button press is registered, gameC=4 
#define gameD (GPIOG->IDR & (1<<ButtonD))		//if button press is registered, gameD=8 

//GAME CONTROLS ABCD
void Gamecontr_init(void);
void initEXTI_gameA(void);
void EXTI0_IRQHandler(void);
void initEXTI_gameB(void);
void EXTI2_IRQHandler(void);
void initEXTI_gameC(void);
void EXTI2_IRQHandler(void);
void initEXTI_gameD(void);
void EXTI3_IRQHandler(void);

//BLUE USER BUTTON
void BlueUserButton(void);
void initEXTI_Blue(void);
void EXTI15_10_IRQHandler(void);

#endif //SWITCH_Header

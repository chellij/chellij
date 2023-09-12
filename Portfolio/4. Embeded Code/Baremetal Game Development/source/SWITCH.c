#include "SWITCH.h" 
#include <stdio.h>

//VARIABLES
int start=0;																//Variable used for seperating game sections
int press;																	//Variable for blue button press registered
int loop=1;																	//Variable to create loop until start happens
int readA;																	//Variable to read value of buttonA input
int readB;																	//Variable to read value of buttonB input
int readC;																	//Variable to read value of buttonC input
int readD;																	//Variable to read value of buttonD input
int go=0;																		//Variable to create loop
int Blue=0;																	//Variable used to not let blue button work until correct point in game

//EXTERNAL VARIABLES
extern int man; 														//from LCD.c
extern int space; 													//from LCD.c
extern int Death;														//from LCD.c


void Gamecontr_init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN; 			//Enable Port G
	GPIOG->PUPDR |= ((0x01<<5) | (0x01<<7));	//Pull up C and D
}

void initEXTI_gameA(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;		//Enable Port G for GamebuttonA (PG0)
	RCC->APB2ENR |= 0x4000;									//Enable SYSCFG clock - bit 14 of APB2ENR = SYSCFG Enable
	
	GPIOG->MODER &=~(0x1<<0);								//Set PG0 to input mode (~ inverts 1 to 0)
	
	SYSCFG->EXTICR[0] |= 0x6;								//Set EXTI 0 as port G (for EXTIx[3:0] PG[0] = 0b0110 = 0x6 )
	
	EXTI->IMR |= 0x01;											//Unmask EXTI 0 (bit 0, not masked=1 .... 0x01)
	EXTI->RTSR |= 0x01;											//Rising edge trigger
	
	NVIC_EnableIRQ(EXTI0_IRQn);							//Enable EXTI 0 interrupt in NVIC
}

void EXTI0_IRQHandler(void)								//IF BUTTON A PRESSED
{
	if (start==0) { readA=gameA; }					//Before game start - read input level to navigate options
	if (start==1 && Death==0)								//During gameplay
	{	
		man=0;																//Update variable
		locate(man,space);										//locate LCD in new place
		putLCD(2);														//print standing man
		locate(1,space);											//locate previous position
		putLCD(20);														//print empty space
	}
	if (start==1 && Death==1){readA=gameA;} //After game over - read input level to navigate options
	EXTI->PR = 0x1;													//Clear interrupt pending flag
}

void initEXTI_gameB(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;		//Enable Port G for GamebuttonB (PG1)
	RCC->APB2ENR |= 0x4000;									//Enable SYSCFG clock - bit 14 of APB2ENR = SYSCFG Enable
	
	GPIOG->MODER &=~(0x1<<1);								//Set PG1 to input mode (~ inverts 1 to 0)
	
	SYSCFG->EXTICR[0] |= 0x60;							//Set EXTI 1 as port G (for EXTIx[3:0] PG[1]= 0b 0110 0000 = 0x6 0)
	
	EXTI->IMR |= 0x1<<1;										//Unmask EXTI 1 (bit 1, not masked=1 .... 0x2)
	EXTI->RTSR |= 0x1<<1;										//Rising edge trigger
	
	NVIC_EnableIRQ(EXTI1_IRQn);							//Enable EXTI 1 interrupt in NVIC
}

void EXTI1_IRQHandler(void)
{
	if (start==0) { readB=gameB; }					//Before game start - read input level to navigate options
	if (start==1 && Death==0)								//During gameplay
	{	
		space=2;															//Update variable
		locate(man,space);										//locate LCD in new place
		putLCD(2);														//print standing man
		locate(man,3);												//locate previous position
		putLCD(20);														//print empty space
	}
	if(start==1 && Death==1){readB=gameB;}  //After game over - read input level to navigate options
	EXTI->PR = 0x1<<1;											//Clear interrupt pending flag
}

void initEXTI_gameC(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;		//Enable Port G for GamebuttonB (PG2)
	RCC->APB2ENR |= 0x4000;									//Enable SYSCFG clock - bit 14 of APB2ENR = SYSCFG Enable
	
	GPIOG->MODER &=~(0x1<<2);								//Set PG2 to input mode (~ inverts 1 to 0)
	GPIOG->PUPDR |= (0x01<<5);
	
	SYSCFG->EXTICR[0] |= 0x600;							//Set EXTI 2 as port G (for EXTIx[3:0] PG[2]= 0b 0110 0000 0000 = 0x6 00)
	
	EXTI->IMR |= 0x1<<2;										//Unmask EXTI 2 (bit 2, not masked=1 .... 0x3)
	EXTI->RTSR |= 0x1<<2;										//Rising edge trigger
	
	NVIC_EnableIRQ(EXTI2_IRQn);							//Enable EXTI 2 interrupt in NVIC
}

void EXTI2_IRQHandler(void)
{
	if (start==0) { readC=gameC; }					//Before game start - read input level to navigate options
	if (start==1 && Death==0)								//During gameplay

	{	
		man=1;																//Update variable
		locate(man,space);										//locate LCD in new place
		putLCD(2);														//print standing man
		locate(0,space);											//locate previous position
		putLCD(20);														//print empty space
	}
	if(start==1 && Death==1){readC=gameC;}	//After game over - read input level to navigate options
	EXTI->PR = 0x1<<2;											//Clear interrupt pending flag
}
void initEXTI_gameD(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;		//Enable Port G for GamebuttonB (PG3)
	RCC->APB2ENR |= 0x4000;									//Enable SYSCFG clock - bit 14 of APB2ENR = SYSCFG Enable
	
	GPIOG->MODER &=~(0x1<<3);								//Set PG1 to input mode (~ inverts 1 to 0)
	GPIOG->PUPDR |= (0x01<<7);
	
	SYSCFG->EXTICR[0] |= 0x6000;						//Set EXTI 3 as port G (for EXTIx[3:0] PG[3]= 0b 0110 0000 0000 0000 = 0x6 000)

	EXTI->IMR |= 0x1<<3;										//Unmask EXTI 3 (bit 3, not masked=1 .... 0x4)
	EXTI->RTSR |= 0x1<<3;										//Rising edge trigger
	
	NVIC_EnableIRQ(EXTI3_IRQn);							//Enable EXTI 3 interrupt in NVIC
}

void EXTI3_IRQHandler(void)
{
	if (start==0) {readD=gameD;}						//Before game start - read input level to navigate options
	if (start==1 && Death==0)								//During gameplay
	{	
		space=3;															//Update variable
		locate(man,space);										//locate LCD in new place
		putLCD(2);														//print standing man
		locate(man,2);												//locate previous position
		putLCD(20);														//print empty space
	}
	if(start==1&&Death==1)	{readD=gameD;}	//After game over - read input level to navigate options
	EXTI->PR = 0x1<<3;											//Clear interrupt pending flag
}


void initEXTI_Blue(void)
{
	RCC->AHB1ENR = RCC_AHB1ENR_GPIOCEN;			//Enable Port C for User Button (PC13)
	RCC->APB2ENR |= 0x4000;									//Enable SYSCFG clock
	
	GPIOC->MODER &= ~(1<<13);								//Set PC13 to input mode (~ inverts)
	
	SYSCFG->EXTICR[3] &= ~0x00F0;						//Clear EXTI 13 port selection0
	SYSCFG->EXTICR[3] |=0x0020;							//Set EXTI 13 as port C
	
	EXTI->IMR |= 0x2000;										//Unmask EXTI 13
	EXTI->RTSR |= 0x2000;										//Rising edge trigger
	
	NVIC_EnableIRQ(EXTI15_10_IRQn);					//Enable EXTI 13 interrupt in NVIC
}

void EXTI15_10_IRQHandler(void)
{
	BlueUserButton();												//Run function
	EXTI->PR = 0x2000;											//Clear interrupt pending flag
}

void BlueUserButton(void)
{	
	if (Blue==1)	
	{
		while(loop==1)																//while var loop = 1
		{	
			press = GPIOC->IDR & (1<<UserButton);				//Variable to watch in debug
			GPIOC->BSRR = (0x01<<(ppred));
			delay_ms(500);
			if(press==0x2000)														//if user button is pressed
			{
				press = GPIOC->IDR & (1<<UserButton);				//Variable to watch in debug
				GPIOC->BSRR = (0x01<<(ppyellow));
				delay_ms(500);
				if(press==0x2000)														//if user button is pressed
				{
					press = GPIOC->IDR & (1<<UserButton);				//Variable to watch in debug
					GPIOC->BSRR = (0x01<<(ppgreen));
					delay_ms(500);
					if(press==0x2000)
					{	
						start=1;															//update variable start to start game
						Blue=2;																//update variable blue so blue button doesnt work again
						loop++;																//increase loop to leave for loop
						Ready321();														//run Ready321 code from GAMEPLAY.c
						break;
					}		
					else	
					GPIOC->BSRR = (0x01<<(ppred+16) | 1<<(ppyellow+16) | 1<<(ppgreen+16));
					break;
				}
				else 
				GPIOC->BSRR = (0x01<<(ppred+16) | 1<<(ppyellow+16) | 1<<(ppgreen+16));
				break;
			}
			else
			GPIOC->BSRR = (0x01<<(ppred+16) | 1<<(ppyellow+16) | 1<<(ppgreen+16));
		}
	}
	else __NOP();
}

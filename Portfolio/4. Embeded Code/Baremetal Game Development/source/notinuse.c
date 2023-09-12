#include "notinuse.h"

void gamecontr_A (void) //GAME CONTROLS - BUTTON A - GPIO PORT:G PIN:0
{	
	if(start==1)
	{
		GPIOC->BSRR	= (1 <<(ppred));							//Turn on LED (PC2)
		delay_s(1);	
		
		while(1)
		{	
			int watchA=GPIOG->IDR & (1<<ButtonA);		//Variable to watch on debug
			if(GPIOG->IDR & (1<<ButtonA))						//If ButtonA is pressed
			{
				GPIOC->BSRR = (1<<(ppred));					//Keep LED on (PC2)
			}
			else																		//Else if ButtonA isn't pressed
			{
				GPIOC->BSRR = (1<<(ppred+16));				//Turn off LED (PC2)
			}
		}
	}
	else
	{
		delay_ms(500);
	}
}

void gamecontr_B (void) //GAME CONTROLS - BUTTON B - GPIO PORT:G PIN:1
{
	if(start==1)
	{
		GPIOC->BSRR	 =	(1 <<(ppyellow));				//Turn on LED (PC2)
		delay_s(1);	
		
		while(1)
		{	
			if(GPIOG->IDR & (1<<ButtonB))						//If ButtonA is pressed
			{
				GPIOC->BSRR = (1<<(ppyellow));				//Keep LED on (PC2)
			}
			else																		//Else if ButtonA isn't pressed
			{
				GPIOC->BSRR = (1<<(ppyellow+16));		//Turn off LED (PC2)
			}
		}
	}
	else
	{
		delay_ms(500);
	}
}

void gamecontr_C (void) //GAME CONTROLS - BUTTON C - GPIO PORT:G PIN:2
{	
	if(start==1)
	{
		GPIOC->BSRR	= (1<<(ppgreen));						//Turn on LED (PC6)
		delay_s(1);	
		
		while(1)
		{	
			if(GPIOG->IDR & (1<<ButtonC))						//If ButtonC is pressed
			{
				GPIOC->BSRR = (1<<(ppgreen));				//Keep LED on (PC6)
			}
			else																		//Else if ButtonC isn't pressed
			{
				GPIOC->BSRR = (1<<(ppgreen+16));			//Turn off LED (PC6)
			}
		}
	}
	else
	{
		delay_ms(500);
	}
}

void gamecontr_D (void)	//GAME CONTROLS - BUTTON D - GPIO PORT:G PIN:3
{
	if(start==1)
	{
		GPIOB->BSRR =	(1 <<(nucleoblue));						//Turn on LED (PC7)
		delay_s(1);	
		
		while(1)
		{	
			if(GPIOG->IDR & (1<<ButtonD))						//If ButtonD is pressed
			{
				GPIOB->BSRR = (1<<(nucleoblue));					//Keep LED on (PC7)
			}
			else																		//Else if ButtonD isn't pressed
			{
				GPIOB->BSRR = (1<<(nucleoblue+16));			//Turn off LED (PC7)
			}
		}
	}
	else
	{
		delay_ms(500);
	}
}

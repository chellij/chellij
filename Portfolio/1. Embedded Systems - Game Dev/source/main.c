/*ELEC240 - Student 10611816 - DEC 17 2020 - using default compiler 5 

This game is a simple run and jump game designed to test your reaction speed.
Using the potentiometer you can select your initial level (0-4) and then as the 
game progresses, it will speed up periodically until you reach level 9.

Before the game begins, instructions will appear on the terminal (PuTTy) and you 
will be asked to enter your name using the keyboard and then confirm it is correctly
displayed on the LCD, if you have made a mistake just redo it.

As the game begins, objects will move towards you, do your best to jump over them, if 
you hit an object the game will end however you will be given a chance to have another go. 
If you choose not to play again, your highest score achieved will be displayed.

*/

//Include the given libraries
#include <stm32f429xx.h> 
#include <stdio.h> //(include stdio for printf)

//Include my local libraries for each element
#include "ADCDAC.h"
#include "BUZZER.h"
#include "DELAY.h" 
#include "GAMEPLAY.h"
#include "LCD.h"
#include "LED.h" 
#include "SWITCH.h" 
#include "USART.h"

//setup for printf
#pragma import(__use_no_semihosting)
struct __FILE {int handle;};
FILE __stdout;
FILE __stdin;
void _sys_exit(int x)
{
	x=x;
}

//VARIABLES
int loop2 = 0;							//variable loop2 is used to identify the phase of the game

//EXTERNAL VARIABLES
extern int start; 					//from SWITCH.c
extern char name[]; 				//from LCD.c
extern char high[];					//from LCD.c

int main(void)
{	
	__enable_irq();						//enable global interrupts
	initEXTI_Blue();						//initiate Blue Button exernal interrupt		(PC13)
	initEXTI_gameA();						//initiate Game Button A exernal interrupt  (PG0)
	initEXTI_gameB();						//initiate Game Button B exernal interrupt	(PG1)
	initEXTI_gameC();						//initiate Game Button C exernal interrupt	(PG2)
	initEXTI_gameD();						//initiate Game Button D exernal interrupt	(PG3)
	init_all();								//calls function to initialise all aspects of game play
	HowToPlay();							//calls function to show instructions for the game on PuTTy
	PressStart();							//calls function to set up and then wait for Blue Button for game to start
		
	while(1)									//until the while loop breaks, these functions will repeat
	{
		if(loop2==0)						//loop2=0 shows the game is waiting to start
		{				
			if(start==1)					//if the blue button is pressed, variable start becomes 1 and the next functions can run
			{	
				GamePlay();					//calls function to run the game
				DeathSequence();		//calls function to run death sequence
				highscorefunc();		//calls function to update high score at end of each round
				restart();					//calls function to check if the user wants to replay
			}
			else									//if variable 1 isnt 1
			{
				delay_us(100);			//there is a delay of 100us and the loop restarts
			}
		}
		if(loop2==1)						//if user doesnt want to replay loop2 will have changed to 2
		{
			break;								//in this case, the while(1) loop is broken
		}
	}
	
	/*turn off timers controlling USART, blue LED, 
	white LED, green LED, red LED */
	
	Init_Timer4_off();				
	Init_Timer5_off();				
	Init_Timer2(0);						
	
	/*move USART cursor two lines down, prints "Your High Score: XXX 
	where XXX is current high score on USART*/	
	
	printf("\x1b[2E");				
	USART_SendStr("Your High Score: ");
	USART3_write(high[0]);	
	USART3_write(high[1]);
	USART3_write(high[2]);
	USART3_write(high[3]);

	/*clear the LCD screen, on LCD line 0 print "High Score", 
	on LCD line 1 print the name recieved in HowToPlay function and high score*/
	
	clrscreen();			
	BusyWait();
	printLCD1("High Score");	
	printLCD2(name);					
	highscoreprint();
	rgbLED_clr();

}	//end main

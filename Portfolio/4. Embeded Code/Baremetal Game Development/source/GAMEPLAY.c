#include "PLL_config.c"
#include "GAMEPLAY.h"
#include <stdio.h>

//VARIABLES
int correct=0;									//variable to check the accuracy of the user imput name
int flash=351562;								//ARR value for one second delay (TIM2)
int gamestart=0;

//EXTERNAL VARIABLES
extern int loop2;								//from main.c
extern int readA;								//from SWITCH.c
extern int readB;								//from SWITCH.c
extern int readC;								//from SWITCH.c
extern int readD;								//from SWITCH.c
extern int Death;								//from LCD.c
extern char name[]; 						//from LCD.c
extern int man;		 							//from LCD.c
extern int space; 							//from LCD.c
extern int pos1; 								//from LCD.c
extern int pos2; 								//from LCD.c
extern int pos3; 								//from LCD.c
extern int pos4; 								//from LCD.c
extern int bounce; 							//from LCD.c
extern int point;								//from LCD.c
extern int speedtimer;					//from LCD.c
extern int difficulty[];				//from LCD.c
extern int diffi;								//from LCD.c
extern int Blue;								//from SWITCH.c
extern int gametime;						//from DELAY.c
extern int start;								//from SWITCH.c

//FUNCTIONS
void init_all(void)
{
	//Run PLL_Config and SystemCoreClock so timers and interrupts can work
	PLL_Config();  
	SystemCoreClockUpdate();
	
	//Initialisation for each aspect of the system
	Led_init();									//All OpenDrain, Push Pull, 7 Seg and Nucleo Board LEDs	
	Lcd_init();									//LCD
	Gamecontr_init();						//Game Buttons ABCD
	CustomLCD_init();						//Sends Custom Letters to Memory
	Buzzer_init();							//Buzzer
	init_USART();								//USART
}

void HowToPlay(void)
{
	//INSTRUCTIONS ON PUTTY
	BuzzStartShort();																													//Play short tune
	printLCD1("Read instructions");																						//Print to PuTTy
 	printLCD2("on PuTTY");
	printf("\x1b[4m");																												//Turn on underline
	printf("\x1b[2J");																												//Erase display
	printf("\x1b[2;0f");																											//Moves cursor to row 2, column 0
	USART_SendStr("______________________________________");									//Creates line
	printf("\x1b[2E");																												//Moves cursor 2 lines down							
	printf("\x1b[1;37;46m");																									//Cyan Background, White Writing
	USART_SendStr("Instructions for the game: ");
	printf("\x1b[0;0;0m");																										//Reset colours
	printf("\x1b[1E");																												//Moves cursor 1 line down
	USART_SendStr("Press button A when you have read each line. ");
	printf("\x1b[1E");																												//Moves cursor 1 line down									
	while (readA==0) {__NOP();}																								//Nothing happens until ButtonA is read as input.
	readA = 0;																																//readA is reset
	delay_ms(300);
	USART_SendStr("Use buttons A and C to jump up and down respectively. ");
	printf("\x1b[1E");																												//Moves cursor 1 line down									
	while (readA==0) {__NOP();}																								//Nothing happens until ButtonA is read as input.
	readA = 0;																																//readA is reset
	delay_ms(300);
	USART_SendStr("Use buttons B and D to move left and right. ");
	printf("\x1b[1E");																												//Moves cursor 1 line down									
	while (readA==0) {__NOP();}																								//Nothing happens until ButtonA is read as input.
	readA = 0;																																//readA is reset
	delay_ms(300);
	USART_SendStr("To change the difficulty, adjust the potentiometer. ");
	printf("\x1b[1E");																												//Moves cursor 1 line down									
	while (readA==0) {__NOP();}																								//Nothing happens until ButtonA is read as input.
	readA = 0;																																//readA is reset
	delay_ms(300);
	USART_SendStr("During gameplay the terminal will display readings of the POT and the LDR. ");
	printf("\x1b[1E");																												//Moves cursor 1 line down									
	while (readA==0) {__NOP();}																								//Nothing happens until ButtonA is read as input.
	readA = 0;																																//readA is reset
	delay_ms(300);
	USART_SendStr("The LDR will also generate a random number 1-6 and show the score and duration of the game. ");
	printf("\x1b[1E");																												//Moves cursor 1 line down									
	while (readA==0) {__NOP();}																								//Nothing happens until ButtonA is read as input.
	readA = 0;																																//readA is reset
	delay_ms(300);
	
	while(correct==0)
	{
		USART_SendStr("________");
		printf("\x1b[3E");																												//Moves cursor 2 lines down									
		//INPUT PLAYER NAME.....
		printf("\x1b[1;31m");																											//Text is red
		USART_SendStr("Write your name here and then press enter. ");
		printf("\x1b[1E");																												//Moves cursor 1 line down									
		clrscreen();																															//Clear LCD
		BusyWait();
		printname();																															//Call function printname()
		readA=0; 																																	//readA is reset
		readC=0;																																	//readC is reset
		printf("\x1b[0;0;0m");																										//Reset all colour rules
		USART_SendStr("Does your name look correct on the LCD? A=Yes, C=No. ");
		while(readA==0 & readC==0) {__NOP();}																			//Nothing happens until ButtonA OR ButtonC is read as input.
		if(readC==4 && readA==0){delay_ms(100);}																	//If ButtonA is pressed, delay and move on
		else if(readA==1 && readC==0){correct=1;}																	//If ButtonC is pressed, repeat while loop.
		readA=0; 																																	//readA is reset
		readC=0; 																																	//readC is reset
	}
	printf("\x1b[1E");																												//Moves cursor 1 line down									
}

void PressStart(void)
{
	USART_SendStr("________");
	printf("\x1b[2E");																												//Moves cursor 2 lines down									
	USART_SendStr("Spin the potentiometer to choose your starting level (0-4).");
	printf("\x1b[2E");																												//Moves cursor 2 lines down									
	USART_SendStr("When you're ready to play, press the blue button and look at the LCD. ");
	printf("\x1b[1E");																												//Moves cursor 1 line down									
	USART_SendStr("________");
	printf("\x1b[2E");																												//Moves cursor 2 lines down									
	printf("\x1b[s");
	//READY TO START
	Init_Timer4();								//Initiate Timer4 to start reading POT and difficulty level for adjustment before gameplay
	BuzzStartTune();							//Play start tune
	clrscreen();									//Clear LCD
	BusyWait();
	Blue=1;												//With variable blue being 1, BlueButton function will now run when userbutton is pressed
	printLCD1("Press Start");			//Print to LCD
	printLCD2("(blue button)");
}

void Ready321(void)
{	
	//START GAME
	GPIOC->BSRR = (0x01<<(ppred+16) | 1<<(ppyellow+16) | 1<<(ppgreen+16));		//turn off all push pull leds
	delay_ms(100);								
	clrscreen();									//Clear LCD 
	BusyWait();										//Wait for LCD to finish
	ready321go();									//Call function ready321go from Buzzer.c
}

void GamePlay(void)
{	
	clrscreen();									//Clear LCD 
	BusyWait();										//Wait for LCD to finish
	Init_Timer5();								//Initiate Timer5 to start flashing LED in morse code
	gamestart=1;									//variable GameStart becomes 1 so that the game duration counter in Tim4 Handler can increment
	while(Death==0)
	{
		difficultylevel();					//Call difficulty level function from GAMEPLAY.c
		printscore();								//Call print score function from LCD.c
		run();											//Call run function from LCD.c
		scrollline();								//Call scrolling function from LCD.c
		speedtimer++;								//Call speed timer function from LCD.c
	}	
	printscore();									//Call printscore function from LCD.c again
}

void DeathSequence(void)
{
	ss_clrR();										//Clears difficulty from 7Seg so all LEDS off
	BuzzDeathTune();							//Plays tune to signify gameover
	clrscreen();									//Clear LCD 
	BusyWait();										//Wait for LCD to finish
	printLCD1("Game Over!");			
	printLCD2(name);							//prints to LCD line 2 the name recieved from user earlier
	printscore();									//prints to LCD the score for this round	
	delay_s(2);										//wait 2 seconds to read score
	gamestart=0;									//reset the gamestart variable so gametimer stops counting
}

void restart(void)
{
	clrscreen();									//Clear LCD 
	BusyWait();										//Wait for LCD to finish
	readA=0;											//resets readA
	readB=0;											//resets readB
	readC=0;											//resets readC
	readD=0;											//resets readD
	printLCD1("Play Again?");
	printLCD2("Yes=B, No=D");
	
	BusyWait();
	while(readB==0 & readD==0){__NOP();}	//Nothing happens until ButtonB OR ButtonD is read as input.			
	if(readB==2) 									//if ButtonB is pressed, game restarts and all variables that change during game are reset
	{
		delay_ms(300); 
		Death=0; 
		man=1; 
		space=2; 
		pos1=0; 
		pos2=0; 
		pos3=0; 
		pos4=0; 
		bounce=0;
		point=0;
		gametime=0;
		readB=0;
		difficultystart();
	}
	if(readD==8) {readD=0;loop2++;}	//if ButtonD is pressed, loop2 is increased which means while loop in main.c is broken
}
void difficultystart(void)				//Function to decide the initial speed/level of the game
{	
	int startspeed=0;								//each time function is called, startspeed needs to be reset so local variable is ok
	init_POT();											//make sure POT is being sampled not LDR
	startspeed=read_ADC();					//read current ADC value
	
	//Depending on ADC value and therefore startspeed, speedtimer will be different at the beginning of each game.
	if(startspeed>0  & startspeed<=399)  speedtimer=0;
	if(startspeed>400& startspeed<=799)  speedtimer=5;
	if(startspeed>800& startspeed<=1199) speedtimer=15;
	if(startspeed>1200&startspeed<=1599) speedtimer=25;
	if(startspeed>1600&startspeed<=1999) speedtimer=35;
	if(startspeed>2000&startspeed<=2399) speedtimer=45;
	if(startspeed>2400&startspeed<=2799) speedtimer=55;
	if(startspeed>2800&startspeed<=3199) speedtimer=65;
	if(startspeed>3200&startspeed<=3599) speedtimer=75;
	if(startspeed>3600&startspeed<=3999) speedtimer=85;
	if(startspeed>4000) 								 speedtimer=95;
}
void difficultylevel(void) 			//Function to display the difficulty level of the game on the 7Seg and flash white OD LED
{	
	rgbLED_all();									//Initiate RGB bar to reflect the 7seg display as they share pins
	
	/*For each inequality if-statement the following things happen 
	1) Change the difficulty level to reflect current game speed
	2) Clear 7Seg display and display new difficulty level using LEDs
	3) if the game has started, Start Timer2 at a rate that corresponds with current level/time.	*/
	
	if(speedtimer<=20 && speedtimer>=0){
		diffi=0;
		ss_clrR();
		ss_difficulty(num_zero);
		if (start==0){Init_Timer2(0);}
		else {Init_Timer2(351562);}
	}	
	if(speedtimer<=40 && speedtimer>=21){
		diffi=1;
		ss_clrR();
		ss_difficulty(num_one);
		if (start==0){Init_Timer2(0);}
		else{Init_Timer2(316405);}
	}
	if(speedtimer<=60 && speedtimer>=41){
		diffi=2;
		ss_clrR();
		ss_difficulty(num_two);
		if (start==0){Init_Timer2(0);}
		else{Init_Timer2(281249);}
	}		
	if(speedtimer <=80 && speedtimer>=61){
		diffi=3;
		ss_clrR();
		ss_difficulty(num_three);
		if (start==0){Init_Timer2(0);}
		else{Init_Timer2(210937);}
	}		
	if(speedtimer <=100 && speedtimer>=81){
		diffi=4;
		ss_clrR();
		ss_difficulty(num_four);
		if (start==0){Init_Timer2(0);}
		else{Init_Timer2(175781);}
	}		
	if(speedtimer <=120 && speedtimer>=101){
		diffi=5;
		ss_clrR();
		ss_difficulty(num_five);
		if (start==0){Init_Timer2(0);}
		if (start==1){Init_Timer2(140625);}
	}
	if(speedtimer <=150 && speedtimer>=121){
		diffi=6;
		ss_clrR();
		ss_difficulty(num_six);
		if (start==0){Init_Timer2(0);}
		if (start==1){Init_Timer2(105468);}
	}		
	if(speedtimer <=180 && speedtimer>=151){
		diffi=7;
		ss_clrR();
		ss_difficulty(num_seven);
		if (start==0){Init_Timer2(0);}
		if (start==1){Init_Timer2(70312);}
	}
	if(speedtimer <=210 && speedtimer>=181){
		diffi=8;
		ss_clrR();
		ss_difficulty(num_eight);
		if (start==0){Init_Timer2(0);}
		if (start==1){Init_Timer2(35156);}
	}
	if(speedtimer>=211){
		diffi=9;
		ss_clrR();
		ss_difficulty(num_nine);
		if (start==0){Init_Timer2(0);}
		if (start==1){Init_Timer2(17578);}
	}		
}

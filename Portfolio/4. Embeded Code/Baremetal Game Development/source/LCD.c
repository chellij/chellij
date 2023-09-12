#include "LCD.h"
#include <stdlib.h>
#include <stdio.h>

//VARIABLES
int pos1;										//Variable to set the position of block1 on LCD
int pos2;										//Variable to set the position of block2 on LCD
int pos3;										//Variable to set the position of block3 on LCD
int pos4;										//Variable to set the position of block4 on LCD
int var;										//Random Variable to set the line of block1 on LCD
int var2;										//Random Variable to set the line of block2 on LCD
int var3;										//Random Variable to set the line of block3 on LCD
int var4;										//Random Variable to set the line of block4 on LCD
int scroll=0;								//Counts the scroll to move entire game along
int bounce;									//Variable to stagger blocks 1-4 at different points along game
int space=2;								//Variable to set Column position of running figure
int man=1;									//Variable to set Row position of running figure

char name[10];
int point=0;								//Variable to increase score as game plays
char score[4];							//Array to make "point" printable on LCD
char mscore[3];							//Array to make "point" flashable on LEDs.
int highscore=0;						//Variable to hold highest score achieved as game plays
char high[4];								//Array to make "highscore" printable on LCD

//SPEED/DIFFICULTY
int difficulty[]={600,550,500,450,300,250,200,150,100,50};		//Array holding the delay between scroll increasing at different levels
int diffi=0;								//Variable to pick which value of difficulty[] to use
int delay;									//Variable delay is used to update the difficulty level as the game runs
int speedtimer=0;						//Variable required to increase diffi to increase speed of game
int Death=0;								//When the running man and blocks collide, variable Death is changed to 1 and the for loop is broken

//EXTERNAL VARIABLES
extern int readA;											//from SWITCH.c
extern int readB;											//from SWITCH.c
extern int readC;											//from SWITCH.c
extern int readD;											//from SWITCH.c
extern unsigned short ADC_DATA;				//from ADCDAC.c

//SETUP
void Lcd_init(void)
{
	SystemCoreClockUpdate();
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIODEN;    //Enable LCD port clock

	//CONFIG LCD GPIO PINS
	GPIOD->MODER&=~((0x03<<(2*RS)) |(0x03<<(2*RW)) |(0x03<<(2*EN)) | (0xffff<<(2*D0)));		//Clear pin direction settings
	GPIOD->MODER|=((0x01<<(2*RS)) | (0x01<<(2*RW)) | (0x01<<(2*EN)) | (0x5555<<(2*D0))); 	//Reset pin direction settings to digital outputs

	//LCD INIT COMMANDS
	clrRS(); 															//All lines default low
	clrRW();
	clrEN();
	
	delay_us(40000);
	
	//turns on pins 4 and 5, turn off pins 6 and 7 according to flowchart
	GPIOD->BSRR=((1<<4)|(1<<5)|(1<<(6+16))|(1<<(7+16))); 
	LCD_strobe();
	delay_us(40);
	
	//assembly instruction for 4 bit mode
	GPIOD->BSRR=0x00FF0028;
	LCD_strobe();
	delay_us(40);
	
	//assembly instruction for 4 bit mode
	GPIOD->BSRR=0x00FF0028;
	LCD_strobe();
	delay_us(40);			
	
	cmdLCD(0x28);												//set 4Bit data, 2 line, 5x7 font
	cmdLCD(0x0C);												//move cursor right (0x0F if cursor wanted)
	cmdLCD(0x01);												//clear screen, move cursor to home
	delay_us(2000);
	cmdLCD(0x06);												//turn on display, cursor blinking
	cmdLCD(LCD_LINE1);
}

void BusyWait(void)	
{
	int port=1;
	set_LCD_bus_input();
	setRW();
	clrRS();
	
	while(port==1)
	{
		setEN();
		delay_us(40);
		port=GPIOD->IDR&(1<<7);
		port=port>>7;
		clrEN();
		delay_us(40);	
	}
	set_LCD_bus_output();
}

void set_LCD_data(unsigned char d)
{
	GPIOD->BSRR=(0xff<<(D0+16));	//clear data lines
	GPIOD->BSRR=(d<<D0);					//put data on lines
}

void LCD_strobe(void)		//10us high pulse on LCD enable line
{
	delay_us(10);
	setEN();
	delay_us(10);
	clrEN();
}

void CustomLCD_init(void)
{
	int i;
	unsigned char character[] ={	
																0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,		//top block 0
																0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,		//bottom block 1
																0x0E,0x0E,0x04,0x1F,0x04,0x04,0x0A,0x0A,		//stand 2
																0x0E,0x0E,0x05,0x1F,0x14,0x04,0x0A,0x09,		//walk 3
														 }; 
	
	for (i=0; i<32; i++)
	{
		BusyWait();													//Wait for LCD
		cmdLCD((0x40|i)); 									//Command to cgram
		delay_us(40);													
		putLCD(character[i]);								//Write custom character to data bus
		delay_us(40);
	}
}


//PRINT TEXT
void putLCD(unsigned char put)	//sends a char to the LCD display
{																				
	int nibble1 = (put & 0xF0);
	int nibble2 = ((put << 4) & 0xF0);
	
	BusyWait();														//wait for LCD to be not busy
	setRS();															//text command
	clrRW();															//write command
	
	set_LCD_data(nibble1);								//set data on bus
	LCD_strobe();													//apply command
	set_LCD_data(nibble2);								//set data on bus
	LCD_strobe();													//apply command
}

void printLCD1(char *strChars) 				//Prints whatever string of characters ya want to line 0
{
	unsigned int n=0;
	locate(0,0);
	
	while(*(strChars + n)>0)
	{
		putLCD(*(strChars + n));
		n++;
	}
}

void printLCD2(char *strChars) 				//Prints whatever string of characters ya want to line 1
{
	unsigned int n=0;
	locate(1,0);
	
	while(*(strChars + n)>0)
	{
		putLCD(*(strChars + n));
		n++;
	}
}

void printscore(void)									//Prints score
{ 
	int i;
	
	score[0]='0' + (((point)/100)%10);	//Converts score from int to array to print to LCD
	score[1]='0' + (((point)/10)%10);
	score[2]='0' + ((point)%10);
	score[3]=0;

	mscore[0]=(((point)/100)%10);				//Converts score from int to array to flash on blue LED
	mscore[1]=(((point)/10)%10);
	mscore[2]=((point)%10);
	
	locate(1,13);												//locates bottom right of LCD
	for(i=0; i<3; i++)									//prints each digit of score array
	{
		putLCD(score[i]);
	}	
}
void clrscreen(void)									//Clears screen by printing nothing to both lines
{
	printLCD1("                ");
	printLCD2("                ");
}
void locate(char row, char column)		//Locates place on lcd, user enters two values
{
	if (row==0) 												//first value dictates row
	{
		cmdLCD(0x80+column);
	}
	else if (row==1) 										//second value dictates column
	{
		cmdLCD(0xC0+column);
	}
}
void cmdLCD(unsigned char cmd)				//sends a byte to the LCD control register
{
	int nibble1 = (cmd & 0xF0);
	int nibble2 = ((cmd << 4) & 0xF0);
	
	BusyWait();													//wait for LCD to be not busy
	clrRS();														//control command
	clrRW();														//write command
	
	set_LCD_data(nibble1);							//set data on bus
	LCD_strobe();												//apply command
	set_LCD_data(nibble2);							//set data on bus	
	LCD_strobe();												//apply command
}

//SCROLLING/GAME
void block1(void)											//Code for scrolling blocks, repeats with adjusted variables for blocks2-4.
{
	if(pos1>=0)													//if object1 is bigger than 0
	{
		locate(var, pos1);								//random line 0 or 1, position1
		putLCD(var);											//write block 0 or 1
		locate(var, pos1+1);							//same line position+1 (to the right)
		putLCD(20);												//write blank space
		if (pos1==0)											//if object1 is 0
		{
			locate(var,0);									//same line, place 0
			putLCD(20);											//put line
			pos1=15;
			var=rand()%2;										//generate var 0 or 1	
		}
		pos1--;
	}
}
void block2(void)
{
	if(pos2>=0 & bounce>=4)
	{	
		locate(var2, pos2);
		putLCD(var2);		
		locate(var2, pos2+1);
		putLCD(20);	
		if (pos2==0 & bounce>=4 )
		{	
			locate(var2,0);
			putLCD(20);	
			pos2=15;
			var2 = rand() % 2;
		}
		pos2--;
	}
}
void block3(void)
{
	if(pos3>=0 & bounce>=8)
	{	
		locate(var3, pos3);
		putLCD(var3);
		locate(var3, pos3+1);
		putLCD(20);
		if (pos3==0 & bounce>=8 )
		{	
			locate(var3,0);
			putLCD(20);	
			pos3=15;
			var3 = rand() % 2;
		}
		pos3--;
	}
}
void block4(void)
{
	if(pos4>=0 & bounce>=12)
	{	
		locate(var4, pos4);
		putLCD(var4);
		locate(var4, pos4+1);
		putLCD(20);
		if (pos4==0 & bounce>=12 )
		{	
			locate(var4,0);
			putLCD(20);	
			pos4=15;
			var4 = rand() % 2;
		}
		pos4--;
	}
}
void scrollline(void)									//Function to scroll LCD
{
	if(scroll<2)
	{
		if(scroll==0)											//When scroll = 0
		{
			pos1=0;													//Set all pos variables to 0 so when called they enter their zero if statement
			pos2=0;
			pos3=0;
			pos4=0;
			scroll++;												//increase scroll so not entered again
		}
		if(scroll==1)											//When scroll = 0
		{
			block1();												//Call all block functions in a row
			block2();
			block3();
			block4();	
			bounce++;												//increase bounce
		}
	}
	if (pos4+1==space && var4==man) {Death=1;} //if a block and the man are in the same space, game over
	if (pos4+1==space && var4!=man) {point++;} //if a block and the man are in the same column but not row, add a point
	if (pos3+1==space && var3==man) {Death=1;}
	if (pos3+1==space && var3!=man) {point++;}
	if (pos2+1==space && var2==man) {Death=1;}
	if (pos2+1==space && var2!=man) {point++;}
	if (pos1+1==space && var==man)	{Death=1;}
	if (pos1+1==space && var!=man) {point++;}
}

void run(void)
{	
	delay=difficulty[diffi];										//update varibale delay to current state
	locate(man,space);													//locate on LCD current position of man
	putLCD(2);																	//print standing man
	delay_ms(delay);														

	locate(man,space);													//locate on LCD current position of man
	putLCD(3);																	//print running man
	delay_ms(delay);
}

void printname(void)	
{
	char c;																			//local variable c
	int x=0;																		//local variable x is reset each time function is entered
	BusyWait();

	printLCD1("Your Name:");											
	BusyWait();
	while(1)																		//while name is being recieved
	{
		c = USART3_read();												//recieve byte from USART
		USART3_write(c);													//write byte back to USART
		if(c==0x0D)																//if enter is pressed, name is over
		{
			break;
		}
		else																			//if anything other than enter is pressed,
		{
			locate(1,x);														//print on LCD starting at bottom line, place 0
			putLCD(c);															//byte recieved from USART
			name[x]=c;															//store byte in array name[]for later use
			x++;																		//repeat
		}
	}
	printf("\x1b[1E");													//Move cursor down a line on USART when finished
}

void highscorefunc(void)											
{
	if (point>highscore)												//if variable point is higher than current highscore
	{ 
		highscore=point;													//update highscore to new high
		high[2]='0' + ((highscore)%10);						//convert to array to print
		high[1]='0' + (((highscore)/10)%10);
		high[0]='0' + (((highscore)/100)%10);
		high[3]=0;
	}
	else 																				//otherwise convert current high score to array to print
	{	
		high[2]='0' + ((highscore)%10);
		high[1]='0' + (((highscore)/10)%10);
		high[0]='0' + (((highscore)/100)%10);
		high[3]=0;
	}
}

void highscoreprint(void)										
{		
	int	i;
	locate(1,12);																//in bottom corner of screen
	for(i=0; i<3; i++)
	{
		putLCD(high[i]);													//print current high score value
	}
}

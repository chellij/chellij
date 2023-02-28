#ifndef __LCDPAUL_H_
#define __LCDPAUL_H_
#include <stm32f4xx.h>
#include "DELAY.h"
#include "USART.h"
#include "ADCDAC.h"
#include "SWITCH.h"

//Define pins on the LCD for easy coding 
#define RS	11 	//(PD11)
#define RW	12	//(PD12)
#define EN	13	//(PD13)
#define D0	0		//(PD0)
#define D1	1		//(PD1)
#define D2	2		//(PD2)
#define D3	3		//(PD3)
#define D4	4		//(PD4)
#define D5	5		//(PD5)
#define D6	6		//(PD6)
#define D7	7		//(PD7)

//Define the hex code for specific places on LCD for more readable code
#define LCD_LINE1		0x80		
#define LCD_LINE2		0xc0

#define setRS()	GPIOD->BSRR=(0x01<<RS)
#define clrRS()	GPIOD->BSRR=(0x01<<(RS+16))
#define setRW()	GPIOD->BSRR=(0x01<<RW)
#define clrRW()	GPIOD->BSRR=(0x01<<(RW+16))
#define setEN()	GPIOD->BSRR=(0x01<<EN)
#define clrEN()	GPIOD->BSRR=(0x01<<(EN+16))

#define set_LCD_bus_input()		GPIOD->MODER&=~(0xffff<<(2*D0))
#define set_LCD_bus_output()	GPIOD->MODER|=(0x5555<<(2*D0))

//SETUP
void Lcd_init(void);
void BusyWait(void);
void set_LCD_data(unsigned char d);
void LCD_strobe(void);
void CustomLCD_init(void);
void clrscreen(void);

//PRINT TEXT
void printLCD1(char *strChars);
void printLCD2(char *strChars);
void putLCD(unsigned char put);
void printscore(void);
void locate(char row, char column);
void cmdLCD(unsigned char cmd);

//GAME
void HowToPlay(void);

void block1(void);
void block2(void);
void block3(void);
void block4(void);
void run(void);
void scrollline(void);
void gamebuttons(void);

void printname(void);

//Death/GameOver
void GameOver(void);
void highscorefunc(void);
void highscoreprint(void);
#endif

#include "USART.h"
#include "LCD.h"
#include "ADCDAC.h"

//VARIABLES
unsigned int x;
char dienumber[3];
int randomnum;
char duration[4];

//EXTERNAL VARIABLES
extern unsigned short ADC_DATA;			//from ADCDAC.c
extern char ADCstr[];								//from ADCDAC.c
extern char score[];								//from LCD.c
extern int diffi;										//from LCD.c
struct FILE;
extern int gametime;								//from DELAY.c

void init_USART(void)
{
	unsigned char i=USART_TX_pin/8;											//used to select AFR[reg] for TX_pin ... pins 0-7 AFR[0] pins 8-15 AFR[1]
	unsigned char j=USART_RX_pin>>3;										//same as above for RX_pin (uses shift to divide)
	RCC->AHB1ENR|=RCC_AHB1ENR_GPIODEN;									//usart port clock enable
	
	//clear pin function bits
	GPIOD->MODER&=~((0x03<<(2*USART_TX_pin))|(0x03<<(2*USART_RX_pin)));
	
	//reset pin function bits (alternate function)
	GPIOD->MODER|=((0x02<<(2*USART_TX_pin))|(0x02<<(2*USART_RX_pin)));							

	// ALTERNATE FUNCTION SELECT BITS
	GPIOD->AFR[i]&=~(0x0f<<(4*(USART_TX_pin-(i*8))));   //clear pin function
	GPIOD->AFR[i]|=(0x07<<(4*(USART_TX_pin-(i*8))));		//set usart as alternate function for TX_pin
	GPIOD->AFR[j]&=~(0x0f<<(4*(USART_RX_pin-(j*8))));		//clear pin function
	GPIOD->AFR[j]|=(0x07<<(4*(USART_RX_pin-(j*8))));		//set usart as alternate function for RX_pin
	
	RCC->APB1ENR|=RCC_APB1ENR_USART3EN;									//usart clock enable
	CLEAR_BIT(USART3->CR1, 0<<9); 											//Odd Parity
	CLEAR_BIT(USART3->CR1, 1<<12);											//1 Start bit 8 Data Bits
	
	//USART CONFIG	//transmit enable		//receive enable		//usart main enable bit 	//usart enable RXNE
	USART3->CR1|=(USART_CR1_TE|USART_CR1_RE|USART_CR1_UE|USART_CR1_RXNEIE);
	
	USART3->BRR=SystemCoreClock/4/(BAUDRATE);						//set baud rate
}

void USART_SendData(const char Data) 
{ // send char
	while(!(USART3->SR & USART_SR_TC) ); 								//wait for transmission complete
	USART3->DR=(Data & 0x01FF); 												//write byte to usart data register
}


void USART_SendStr(const char *text) 
{ 
	while(*text !='\0')																	//while string continues
	{ 
		USART_SendData(*text);														//write individual bytes of string to USART
		text++;																						//move to next byte
	}
	USART3->DR = (*text & 0x01FF); 											//write byte to usart data register
}

char USART3_read(void)														
{
	while(!(USART3->SR & 0x0020)) {__NOP();}						//while USART doesn't read user input, wait
	return USART3->DR;																	//when input is read, return data
}

void USART3_write(int ch)
{
	while(!(USART3->SR & USART_SR_TC)){__NOP();}				//while there is no data, wait
	USART3->DR=(ch & 0xFF);															//when there is data, write it to USART
}

int fputc(int ch, struct FILE *f)								
{
	USART_SendData((unsigned int) ch);
	return ch;
}

int fgetc(struct FILE *f)
{
	int ch;
	while(!(USART3->SR & USART_SR_RXNE));
	ch = USART3->DR ;
	return ch;
}

#include <stdio.h>

void printPOT(void)											
{
	printf("\x1b[u");											//Move cursor to saved position
	adclevel();														//call function to read ADC level and make into array
	USART_SendStr("POT= ");								//print string on USART
	USART_SendStr(ADCstr);								//print ADC Level array on USART
}

void printdiffi(void)										
{	
	printf("\x1b[5C");										//Move cursor 5 columns right
	USART_SendStr("Difficulty Level= ");	//print string on USART
	USART3_write('0'+ diffi);							//print difficulty level on USART
}	

void printLDR(void)				
{			
	printf("\x1b[5C");										//Move cursor 5 columns right		
	adclevel();														//call function to read ADC level and make into array	
	USART_SendStr("LDR= ");								//print string on USART
	USART_SendStr(ADCstr);								//print ADC Level array on USART
}

void roundscore(void)
{
	printf("\x1b[5C");										//Move cursor 5 columns right		
	USART_SendStr("Score= ");							//print string on USART
	USART3_write(score[0]);								//print each value of score array on USART
	USART3_write(score[1]);
	USART3_write(score[2]);
	USART3_write(score[3]);
}

void printduration(void)
{	
	printf("\x1b[5C");										//Move cursor 5 columns right		
	USART_SendStr("Time: ");							//print string on USART
	
	duration[0]='0'+(gametime/100)%10;		//convert gametime to printable array	
	duration[1]='0'+(gametime/10)%10;
	duration[2]='0'+(gametime%10);
	duration[3]='0';
	
	USART_SendStr(duration);							//print each value of duraction array on USART
	USART_SendStr("s ");									//print string on USART
}

void printdice(void)
{	
	printf("\x1b[5C");										//Move cursor 5 columns right		
	randomnum=((ADC_DATA)%6);							//create a random number between 1 and 6 like a dice
	USART_SendStr("Die= ");								//print string on USART
	USART3_write('0'+randomnum);					//print random number to USART
	printf("\x1b[0G");										//return to beginning of line
}

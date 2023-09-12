#ifndef __USART_H_
#define __USART_H_
#include <stm32f4xx.h>

//Defines the pins used in the USART as well as baudrate for easy reading of init code
#define USART_TX_pin	8				//PD8
#define USART_RX_pin	9				//PD9
#define BAUDRATE			115200	//Baudrate = 115200, PLL Clock Speed = 180Mhz

void init_USART(void);								//Function to set up USART
void USART_SendData(char Data);				//Function to send one character of data
void USART_SendStr(const char *text);	//Function to send a string of data

char USART3_read(void);								//Function to send a byte
void USART3_write(int ch);						//Function to read a byte

void printPOT(void);									//Function to print POT level in V to USART
void printLDR(void);									//Function to print LDR level in V to USART
void printdice(void);									//Function to print random dice value to USART
void roundscore(void);								//Function to print current score to USART
void printduration(void);							//Function to print game duration to USART
void printdiffi(void);								//Function to print difficulty level to USART

#endif

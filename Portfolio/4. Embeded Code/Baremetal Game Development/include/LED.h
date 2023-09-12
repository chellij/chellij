#ifndef __LED_H
#define __LED_H
#include <stm32f4xx.h>
#include "DELAY.h"

//NUCLEO LEDS - PORT B - PIN X
#define nucleogreen (0)
#define nucleoblue (7)
#define nucleored (14)

//PUSH PULL LEDS - PORT C - PIN X
#define ppred (2)
#define ppyellow (3)
#define ppgreen (6)

//OPEN DRAIN LEDS - PORT C - PIN X
#define odred (7)
#define odyellow (8)
#define odgreen (9)

//OPEN DRAIN WHITE LED - PORT F - PIN X
#define odwhite (10)

//SEVEN SEG and RGB LEDS - PORT E - PIN X
#define rgbOE 0
#define seg_F 2
#define seg_E 3
#define seg_G 4
#define seg_D 5
#define seg_A 6
#define seg_B 7
#define seg_C 8
#define seg_DP 9
#define seg_LE1 10
#define seg_LE2 11
#define rgbRedLatch 12
#define rgbGreenLatch 13
#define rgbBlueLatch 14
#define seg_OE 15

//BSRR CODES FOR NUMBERS 0-9 FOR SEVEN SEG
#define num_one (1<<(seg_B) | 1<<(seg_C) | 1<<(seg_DP+16))
#define num_two (1<<(seg_A) | 1<<(seg_B) | 1<<(seg_D) | 1<<(seg_E) | 1<<(seg_G) | 1<<(seg_DP+16))
#define num_three (1<<(seg_A) | 1<<(seg_B) | 1<<(seg_C) | 1<<(seg_D) | 1<<(seg_G) | 1<<(seg_DP+16))
#define num_four	(1<<(seg_B) | 1<<(seg_C) | 1<<(seg_F) | 1<<(seg_G) | 1<<(seg_DP+16))
#define num_five	(1<<(seg_A) | 1<<(seg_C) | 1<<(seg_D) | 1<<(seg_F) | 1<<(seg_G) | 1<<(seg_DP+16))
#define num_six (1<<(seg_A) | 1<<(seg_C) | 1<<(seg_D) | 1<<(seg_E) | 1<<(seg_F) | 1<<(seg_G) | 1<<(seg_DP+16))
#define num_seven (1<<(seg_A) | 1<<(seg_B) | 1<<(seg_C) | 1<<(seg_DP+16))
#define num_eight (1<<(seg_A) | 1<<(seg_B) | 1<<(seg_C) | 1<<(seg_D) | 1<<(seg_E) | 1<<(seg_F) | 1<<(seg_G) | 1<<(seg_DP+16))
#define num_nine	(1<<(seg_A) | 1<<(seg_B) | 1<<(seg_C) | 1<<(seg_D) | 1<<(seg_F) | 1<<(seg_G) | 1<<(seg_DP+16))
#define num_zero (1<<(seg_A) | 1<<(seg_B) | 1<<(seg_C) | 1<<(seg_D) | 1<<(seg_E) | 1<<(seg_F) | 1<<(seg_DP+16))
#define let_g (1<<(seg_A) | 1<<(seg_C) | 1<<(seg_D) | 1<<(seg_E) | 1<<(seg_F) | 1<<(seg_DP+16))
#define clear_ss (1<<(16+seg_A) | 1<<(16+seg_B) | 1<<(16+seg_C) | 1<<(16+seg_D) | 1<<(16+seg_E) | 1<<(16+seg_F) | 1<<(16+seg_G) | 1<<(seg_DP+16))

//BSRR CODE FOR ALL RGB LEDS (D0-D7 = PE2-PE9)
#define allRGB (1<<(2) | 1<<(3) | 1<<(4) | 1<<(5) | 1<<(6) | 1<<(7) | 1<<(8) | 1<<(9))
#define clrRGB (1<<(16+2) | 1<<(16+3) | 1<<(16+4) | 1<<(16+5) | 1<<(16+6) | 1<<(16+7) | 1<<(16+8) | 1<<(16+9))

//OPEN DRAIN LEDS
void odLED_init(void);
void whiteodLED_Tog(void);
void trafficlights(int delay);
void odLED_Ready (void);
void odLED_ReadyOff (void);
void odLED_Set (void);
void odLED_SetOff (void);
void odLED_Go (void);
void odLED_GoOff (void);

//PUSH PULL LEDS
void ppLED_init(void);

//NUCLEO BOARD LEDS
void nucleoLED_init(void);
void GreenLED5Hz(void);
void redLED_sample(void);
void greenflash(void);
void morseon(void);
void morseoff(void);
void morsecode(void);

//SEVEN SEG LEDs
void ssLED_init(void);
void ss_clrL(void);
void ss_clrR(void);
void ss_ones(int digit);
void ss_tens(int digit);
void ss_strobeL(void);
void ss_strobeR(void);
void ss_difficulty(int digit);

//RGB BAR
void rgbLED_init(void);
void rgbLED_clr(void);
void rgbLED_all(void);

void rstrobe(void);
void r_on(void);
void r_off(void);

void gstrobe(void);
void g_on(void);
void g_off(void);

void bstrobe(void);
void b_on(void);
void b_off(void);

//INITIATE ALL LEDS
void Led_init(void);

#endif //LED_Header

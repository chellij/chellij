#ifndef __GAMEPLAY_H_
#define __GAMEPLAY_H_
#include <stm32f4xx.h>
#include "ADCDAC.h"
#include "BUZZER.h"
#include "DELAY.h" 
#include "LCD.h"
#include "LED.h" 
#include "SWITCH.h" 
#include "USART.h"

//Define different 7seg configurations for ease of reading in difficultylevel() function.
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

void init_all(void);
void Ready321(void);
void PressStart(void);
void GamePlay(void);
void DeathSequence(void);
void restart(void);

void difficultystart(void);
void difficultylevel(void);
#endif

#ifndef _PWM_TONE_H_
#define _PWM_TONE_H_
 
/* Includes ------------------------------------------------------------------*/
#include "mbed.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/*
#define Do3  131 //C octave3
#define Do3s 139 //C#
#define Re3  147 //D
#define Re3s 156//D#
#define Mi3  165 //E
#define Fa3  175 //F
#define Fa3s 185 //F#
#define So3  196 //G
#define So3s 208 //G#
#define La3  220 //A
#define La3s 233 //A#
#define Ti3  247 //B
#define Do4  262 //C octave4
#define Do4s 277 //C#
#define Re4  294 //D
#define Re4s 311//D#
#define Mi4  330 //E
#define Fa4  349 //F
#define Fa4s 370 //F#
#define So4  392 //G
#define So4s 415 //G#
#define La4  440 //A
#define La4s 466 //A#
#define Ti4  494 //B
#define Do5  523 //C octave5
#define Do5s 554 //C#
#define Re5  587 //D
#define Re5s 622//D#
#define Mi5  659 //E
#define Fa5  699 //F
#define Fa5s 740 //F#
#define So5  784 //G
#define So5s 831 //G#
#define La5  880 //A
#define La5s 932 //A#
#define Ti5  988 //B
#define Do6  1047 //C octave6
#define Do6s 1109 //C#
#define Re6  1175 //D
#define Re6s 1245 //D#
#define Mi6  1319 //E
#define Fa6  1397 //F
#define Fa6s 1480 //F#
#define So6  1568 //G
#define So6s 1661 //G#
#define La6  1760 //A
#define La6s 1865 //A#
#define Ti6  1976 //B
*/
#define C3   131 //C octave3
#define C3s  139 //C#
#define Dd3  147 //D
#define D3s  156//D#
#define E3   165 //E
#define F3   175 //F
#define F3s  185 //F#
#define G3   196 //G
#define G3s  208 //G#
#define Aa3  220 //A
#define A3s  233 //A#
#define B3   247 //B
#define C4   262 //C octave4
#define C4s  277 //C#
#define Dd4   294 //D
#define D4s  311//D#
#define E4   330 //E
#define F4   349 //F
#define F4s  370 //F#
#define G4   392 //G
#define G4s  415 //G#
#define Aa4   440 //A
#define A4s  466 //A#
#define B4   494 //B
#define C5   523 //C octave5
#define C5s  554 //C#
#define Dd5   587 //D
#define D5s  622 //D#
#define E5   659 //E
#define F5   699 //F
#define F5s  740 //F#
#define G5   784 //G
#define G5s  831 //G#
#define Aa5   880 //A
#define A5s  932 //A#
#define B5   988 //B
#define C6   1047 //C octave6
#define C6s  1109 //C#
#define Dd6   1175 //D
#define D6s  1245 //D#
#define E6   1319 //E
#define F6   1397 //F
#define F6s  1480 //F#
#define G6   1568 //G
#define G6s  1661 //G#
#define Aa6   1760 //A
#define A6s  1865 //A#
#define B6   1976 //B
 
 
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Tune(PwmOut name, int period, int beat);
void Auto_tunes(PwmOut name, int period, int beat);
void Stop_tunes(PwmOut name);
        
/* Private functions ---------------------------------------------------------*/
 
#endif
 
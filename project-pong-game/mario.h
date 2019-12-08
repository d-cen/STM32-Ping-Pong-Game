#ifndef _MARIO_MUSIC
#define _MARIO_MUSIC

#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include <stdio.h>
#include <stdlib.h>

#define NOTE_3DO 1046
#define NOTE_3RE 1174
#define NOTE_3MI 1318
#define NOTE_3FA 1396
#define NOTE_3SO 1568
#define NOTE_3LY 1720
#define NOTE_3SI 1975

// 4th octave
#define NOTE_4DO 2093
#define NOTE_4RE 2349
#define NOTE_4MI 2637
#define NOTE_4FA 2793
#define NOTE_4SO 3136
#define NOTE_4LY 3440
#define NOTE_4SI 3951

#define WAIT 30

// Function declaration
void setup_gpio(void);
void playFreq(unsigned int freq);
void delay(unsigned int ms);
int check_condition(void);
void mario(void);

#endif

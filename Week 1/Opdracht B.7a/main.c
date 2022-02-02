/*
 * main.c
 *
 * Created: 2/2/2022 1:49:58 PM
 *  Author: raoul
 */ 

#include <xc.h>
#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>

void wait( int ms )
{
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );        // library function (max 30 ms at 8MHz)
	}
}

void s1(void){
	PORTD = 0x40;
}

void s2(void){
	PORTD = 0x20;
}

void s3(void){
	PORTD = 0x10;
}

void start(void){
	PORTD = 0x80;
}

void end(void){
	PORTD = 0x08;
}


typedef enum { D7, D6, D5 } ENUM_EVENTS;
typedef enum { START, S1, S2, S3, END } ENUM_STATES;
typedef struct {
	void (*func)(void);
	ENUM_STATES nextState;
} PATTERN_STRUCT;

PATTERN_STRUCT pattern[5][3] = {
	//   D7				D6				D5
	{ {start, START}, {s1, S1  },	{s2, S2}   },// start
	{ {start, START}, {s1, S1  },	{s2, S2}   },// s1
	{ {start, START}, {s1, S1  }, 	{s3, S3}   },// s2
	{ {start, START}, {end, END}, 	{end, END} },// s3
	{ {start, START}, {end, END}, 	{end, END} } // end
};

ENUM_STATES state = START;
void handleEvent(ENUM_EVENTS event){
	if( pattern[state][event].func != NULL) {
		pattern[state][event].func();
	}
	state = pattern[state][event].nextState;
}

int main(void)
{
	DDRD = 0b11111111;
	int isPressed = 0;
	int isPressed2 = 0;
	int isPressed3 = 0;
	start();
    while(1)
    {
		
		wait(10);
		
		if (PINC & 0x20)
		{
			if (isPressed == 0)
			{
				isPressed = 1;
				handleEvent(D5);
			}
		} else {
			isPressed = 0;
		}
		if (PINC & 0x40)
		{
			if (isPressed2 == 0)
			{
				isPressed2 = 1;
				handleEvent(D6);
			}
		} else {
			isPressed2 = 0;
		}
		if (PINC & 0x80)
		{
			if (isPressed3 == 0)
			{
				isPressed3 = 1;
				handleEvent(D7);
			}
		} else {
			isPressed3 = 0;
		}
	}
}

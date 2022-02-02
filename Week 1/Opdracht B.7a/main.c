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

void wait( int ms )
{
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );        // library function (max 30 ms at 8MHz)
	}
}


//
// State S1
//
void s1(void){
	PORTD = 0x20;
}

//
// State S2
//
void s2(void){
	PORTD = 0x40;
}

//
// State S3
//
void s3(void){
	PORTD = 0x10;
}

//
// State Start
//
void start(void){
	PORTD = 0x80;
}

//
// State Stop
//
void end(void){
	PORTD = 0x08;
}


typedef enum { START, S1, S2, S3, END } ENUM_STATES;
typedef struct {
	void (*state)(void);
	ENUM_STATES nextState;
} PATTERN_STRUCT;

PATTERN_STRUCT pattern[5] = {
	{start, S2}, {s1, S3}, {s2, S1}, {s3, END}, {end, START}
};


int main(void)
{
	DDRD = 0b11111111;
	ENUM_STATES state = START;
    while(1)
    {
		wait(500);
		pattern[state].state();
		state = pattern[state].nextState;
    }
}

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

typedef enum { START, S1, S2, S3, END } ENUM_STATES;

int main(void)
{
	DDRD = 0b11111111;
	ENUM_STATES state = START;
    while(1)
    {
		wait(500);
		switch (state)
		{
			case START:
				PORTD = 0x80;
				state = S2;
				break;
			case S1:
				PORTD = 0x20;
				state = S3;
				break;
			case S2:
				PORTD = 0x40;
				state = S1;
				break;
			case S3:
				PORTD = 0x10;
				state = END;
				break;
			case END:
				PORTD = 0x08;
				state = START;
				break;
		}
    }
}
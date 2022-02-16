/*
 * main.c
 *
 * Created: 2/16/2022 10:31:40 AM
 *  Author: raoul
 */ 

#define F_CPU 8e6
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd.h"

void wait( int ms ) {
	for (int tms=0; tms<ms; tms++) {
		_delay_ms( 1 );			// library function (max 30 ms at 8MHz)
	}
}

int main(void)
{	
	init();
	
    DDRD &= ~(1 << 7);		// PD7 op input: DDRD=xxxx xxx0
    DDRB = 0xFF;			// PORTB is output
    TCCR2 = 0b00000111;		// counting via PD7, rising edge

    while (1) {
	    PORTB = TCNT2;		// Toon waarde TCCR2
		char str[16];
		sprintf(str, "%d       ", TCNT2);
		set_cursor(0);
	    display_text(str);
	    wait(200);
    }
}
/*
 * main.c
 *
 * Created: 2/16/2022 11:25:23 AM
 *  Author: raoul
 */ 


#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BIT(x)			(1 << (x))

// wait(): busy waiting for 'ms' millisecond
// Used library: util/delay.h
void wait( int ms ) {
	for (int tms=0; tms<ms; tms++) {
		_delay_ms( 1 );			// library function (max 30 ms at 8MHz)
	}
}

volatile int isHigh = 1;

void timer2Init( void ) {
	OCR2 = 117;				// Compare value of counter 2
	TIMSK |= BIT(7);		// T2 compare match interrupt enable
	sei();					// turn_on intr all
	TCCR2 = 0b00001101;		// Initialize T2: timer, prescaler=1024, compare output disconnected,CTC,RUN
}

ISR( TIMER2_COMP_vect ) {
	PORTC ^= BIT(0);
	if ( isHigh == 1 ) {
		OCR2 = 117;
		isHigh = 0;
	} else {
		OCR2 = 195;
		isHigh = 1;
	}
}

int main( void ) {
	DDRC = 0xFF;				// set PORTC for output (toggle PC0)
	timer2Init();

	while (1) {
		// do something else
		wait(10);			// every 10 ms (busy waiting
	}
}



#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int i = 0;

void wait( int ms ) {
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}

/*****************************************************************
short:			ISR INT0
inputs:
outputs:
notes:			Set PORTD.5
Version :    	DMK, Initial code
*******************************************************************/
ISR( INT1_vect ) {
	i--;
	i = (i < 0 ? 7 : i);
	PORTB = (1<<i);
}

/******************************************************************
short:			ISR INT1
inputs:
outputs:
notes:			Clear PORTD.5
Version :    	DMK, Initial code
*******************************************************************/
ISR( INT2_vect ) {
	i++;
	i = (i > 7 ? 0 : i);
	PORTB = (1<<i);		
}

/******************************************************************
short:			main() loop, entry point of executable
inputs:
outputs:
notes:			Slow background task after init ISR
Version :    	DMK, Initial code
*******************************************************************/
int main( void ) {
	// Init I/O
	DDRD = 0xF0;			// PORTD(7:4) output, PORTD(3:0) input	
	DDRB = 0xFF;

	// Init Interrupt hardware
	EICRA |= 0x3C;			// INT1 falling edge, INT0 rising edge
	EIMSK |= 0x06;			// Enable INT1 & INT0
	
	// Enable global interrupt system
	//SREG = 0x80;			// Of direct via SREG of via wrapper
	sei();				

	while (1) {
		wait( 500 );								
	}

	return 1;
}

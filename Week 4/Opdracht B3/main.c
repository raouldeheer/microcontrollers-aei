/*
 * main.c
 *
 * Created: 2/23/2022 11:09:03 AM
 *  Author: raoul
 */ 

#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BIT(x)	(1 << (x))

void wait( int ms )
{
	for (int tms=0; tms<ms; tms++)
	{
		_delay_ms( 1 );			// library function (max 30 ms at 8MHz)
	}
}


int main( void )
{
	DDRF = 0x00;				// set PORTF for input (ADC)
	DDRA = 0xFF;				// set PORTA for output
	
	ADMUX = 0b11100011;			// AREF=2.56v, ADLAR=1, ADC3
	ADCSRA = 0b10000110;		// ADC-enable, not started, not free running, division by 64

	while (1)
	{
		ADCSRA |= 0b01000000;			// Start ADC
		while ( ADCSRA & 0b01000000 ) ;	// Wait for completion
		PORTA = ADCH;					// Show MSB of ADC
		wait(500);						// every 50 ms (busy waiting)
	}
}

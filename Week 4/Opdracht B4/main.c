/*
 * main.c
 *
 * Created: 2/23/2022 11:38:46 AM
 *  Author: raoul
 */ 

#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lcd.h"
#include <stdio.h>

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
	
	// Init LCD
	init();

	DDRF = 0x00;				// set PORTF for input (ADC)
	DDRA = 0xFF;				// set PORTA for output
	
	ADMUX = 0b11100001;			// AREF=2.56v, ADLAR=1, ADC1
	ADCSRA = 0b10000110;		// ADC-enable, not started, not free running, division by 64

	while (1)
	{
		ADCSRA |= 0b01000000;			// Start ADC
		while ( ADCSRA & 0b01000000 ) ;	// Wait for completion
		
		char str[20];
		int temp = ADCH;
		sprintf(str, "Temp: %d", temp);
		set_cursor(0);
		display_text(str);
		PORTA = temp;					// Show MSB of ADC
		wait(500);						// every 50 ms (busy waiting)
	}
}

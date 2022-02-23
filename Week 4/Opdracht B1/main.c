/*
 * main.c
 *
 * Created: 2/23/2022 10:19:16 AM
 *  Author: raoul
 */ 

#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

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
	DDRB = 0xFF;				// set PORTB for output
	
	ADMUX = 0b01100010;			// AREF=VCC, ADLAR=1, ADC2
	ADCSRA = 0b11100110;		// ADC-enable, start, free running, division by 64

}

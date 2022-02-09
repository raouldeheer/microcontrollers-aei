/*
 * main.c
 *
 * Created: 2/9/2022 12:25:55 PM
 *  Author: raoul
 */ 
#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

unsigned int i = 0;
int int1 = 0;
int int2 = 0;

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
	int1 = 1;
}

/******************************************************************
short:			ISR INT1
inputs:
outputs:
notes:			Clear PORTD.5
Version :    	DMK, Initial code
*******************************************************************/
ISR( INT2_vect ) {
	int2 = 1;
}

const unsigned char Numbers [16] = {
	//Pgfedcba
	0b00111111,		// 0
	0b00000110,		// 1
	0b01011011,		// 2
	0b01001111,		// 3
	0b01100110,		// 4
	0b01101101,		// 5
	0b01111101,		// 6
	0b00000111,		// 7
	0b01111111,		// 8
	0b01101111,		// 9
	0b01110111,		// A
	0b01111100,		// b
	0b00111001,		// C
	0b01011110,		// d
	0b01111001,		// E
	0b01110001,		// F
};


int main(void)
{
	// Init I/O
	DDRD = 0xF0;			// PORTD(7:4) output, PORTD(3:0) input
	DDRB = 0xFF;
	
	// Init Interrupt hardware
	EICRA |= 0x3C;			// INT1 falling edge, INT0 rising edge
	EIMSK |= 0x06;			// Enable INT1 & INT0
	
	// Enable global interrupt system
	//SREG = 0x80;			// Of direct via SREG of via wrapper
	sei();
	
    while(1)
    {
		if (int1 + int2 == 2)
		{
			i = 0;
		} else {
			if (int1 == 1)
			{
				i++;
			}
			if (int2 == 1)
			{
				i--;
			}
		}
		int1 = 0;
		int2 = 0;
		if (i > 15)
		{
			PORTB = Numbers[14];
		} else {
			if (i < 0) {
				PORTB = Numbers[14];
			} else {
				PORTB = Numbers[i];
			}
		}
		wait( 100 );
        //TODO:: Please write your application code 
    }
}
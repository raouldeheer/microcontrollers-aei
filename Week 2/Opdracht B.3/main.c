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

void wait( int ms ) {
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
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
	int i = 0;
	DDRD = 0xFF;
	DDRC = 0x00;
	
	int isPressed = 0;
	int isPressed1 = 0;
	int isPressed2 = 0;
    while(1)
    {
		if (PINC & 0x01)
		{
			if (isPressed == 0)
			{
				isPressed = 1;
				i++;
			}
			} else {
			isPressed = 0;
		}
		if (PINC & 0x02)
		{
			if (isPressed1 == 0)
			{
				isPressed1 = 1;
				i--;
			}
			} else {
			isPressed1 = 0;
		}
		if (PINC & 0x03)
		{
			if (isPressed2 == 0)
			{
				isPressed2 = 1;
				i = 0;
			}
			} else {
			isPressed2 = 0;
		}
		PORTD = Numbers[i];
		wait( 50 );
        //TODO:: Please write your application code 
    }
}
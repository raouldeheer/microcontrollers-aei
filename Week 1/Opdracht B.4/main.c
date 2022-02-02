/*
 * main.c
 *
 * Created: 2/2/2022 11:47:13 AM
 *  Author: imre
 */ 

#define F_CPU 8e6

#include <avr/io.h>
#include <util/delay.h>

void wait( int ms )
{
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );        // library function (max 30 ms at 8MHz)
	}
}


int main(void)
{
	DDRD = 0b11111111;
    while(1)
    {
		for(int i = 0; i < 8; i++){
			PORTD = 0x01 << i;
			wait(50);
		}
		for(int i = 0; i < 8; i++){
			PORTD = 0x80 >> i;
			wait(50);
		}
    }
	return 1;
}
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
	int i = 1000;
    while(1)
    {
		PORTD = 0x80;
		wait(i);
		PORTD = 0x00;
		wait(i);
		if(PINC & 0x01){
			if(i == 1000){
				i = 4000;
			}else{
				i = 1000;
			}
				
		}
    }
	return 1;
}
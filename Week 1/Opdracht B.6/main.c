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
	
	int counter = 0;
	int isFast = 0;
	int isOn = 0;
	int isPressed = 0;
	
    while(1)
    {
		wait(10);
		counter += 10;
		
		if (PINC & 0x01)
		{
			if (isPressed == 0)
			{
				isPressed = 1;
				if (isFast)
				{
					isFast = 0;
				} else {
					isFast = 1;
				}
			}
		} else {
			isPressed = 0;
		}
		
		if (isFast)
		{
			if (counter >= 250)
			{
				if (isOn)
				{
					isOn = 0;
					PORTD = 0x00;
				} else {
					isOn = 1;
					PORTD = 0x80;
				}
				counter -= 250;
			}
		} else {
			if (counter >= 1000)
			{
				if (isOn)
				{
					isOn = 0;
					PORTD = 0x00;
				} else {
					isOn = 1;
					PORTD = 0x80;
				}
				counter -= 1000;
			}
		}
    }
	return 1;
}
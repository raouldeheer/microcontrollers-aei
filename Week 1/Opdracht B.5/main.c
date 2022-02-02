/*
 * main.c
 *
 * Created: 2/2/2022 11:47:13 AM
 *  Author: imre
 */ 

#define F_CPU 8e6

#include <avr/io.h>
#include <util/delay.h>

typedef struct {
	unsigned char data;
	unsigned int delay ;
} PATTERN_STRUCT;

PATTERN_STRUCT pattern[] = {
	{0x81, 50}, {0x42, 50}, {0x24, 50}, {0x18, 50}, {0x24, 50}, {0x42, 50}, {0x81, 50}, {0x00, 0x00}
};

void wait( int ms )
{
	for (int i=0; i<ms; i++) {
		_delay_ms( 1 );        // library function (max 30 ms at 8MHz)
	}
}


int main( void )
{
	DDRD = 0b11111111;					// PORTD all output 
	
	while (1) {
		// Set index to begin of pattern array
		int index = 0;
		// as long as delay has meaningful content
		while( pattern[index].delay != 0 ) {
			// Write data to PORTD	
			PORTD = pattern[index].data;
			// wait
			wait(pattern[index].delay);
			// increment for next round
			index++;
		}
	}

	return 1;
}
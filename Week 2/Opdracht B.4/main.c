
#define F_CPU 8e6

#include <avr/io.h>
#include <util/delay.h>

typedef struct {
	unsigned char data;
	unsigned int delay ;
} PATTERN_STRUCT;

PATTERN_STRUCT pattern[] = {
	//dPgfedcba
	{0b00000001, 50},
	{0b00100011, 50},
	{0b00100010, 50},
	{0b01100010, 50},
	{0b01010100, 50},
	{0b00010100, 50},
	{0b00011100, 50},
	{0b00001000, 50},//
	{0b00011100, 50},
	{0b00010100, 50},
	{0b01010100, 50},
	{0b01100010, 50},
	{0b00100010, 50},
	{0b00100011, 50},
	{0b00000001, 50},
	{0x00, 0x00}
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
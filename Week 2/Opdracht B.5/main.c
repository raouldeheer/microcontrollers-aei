#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "lcd.h"

int main( void ) {
	// Init LCD
	init();

	// Write sample string
	char str[16] = "Yedi you are";
	display_text(str);

	set_cursor(2);
	display_text(str);

	// Loop forever
	while (1) {
		_delay_ms( 250 );
	}

	return 1;
}


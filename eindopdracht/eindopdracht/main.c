/*
 * main.c
 *
 * Created: 3/16/2022 11:04:54 AM
 *  Author: raoul
 */
//  #define F_CPU 8e6
//  #include <avr/io.h>
//  #include <util/delay.h>
//  #include <avr/interrupt.h>
//  #include <stdio.h>
 
 
//  #define BIT(x)	(1 << (x))

//  void wait( int ms )
//  {
// 	 for (int tms=0; tms<ms; tms++)
// 	 {
// 		 _delay_ms( 1 );			// library function (max 30 ms at 8MHz)
// 	 }
//  }


// int main(void)
// {
		
// 	DDRF = 0x00;				// set PORTF for input (ADC)
// 	DDRA = 0xFF;				// set PORTA for output
// 	DDRB = 0xFF;				// set PORTB for output
		
// 	ADMUX = 0b11100001;			// AREF=2.56v, ADLAR=1, ADC1
// 	ADCSRA = 0b10000110;		// ADC-enable, not started, not free running, division by 64

// 	while (1)
// 	{
// 		ADMUX = 0b11100000;				// AREF=2.56v, ADLAR=1, ADC0
// 		ADCSRA |= 0b01000000;			// Start ADC
// 		while ( ADCSRA & 0b01000000 ) ;	// Wait for completion
			
// 		int temp = ADCH;
// 		PORTA = temp;					// Show MSB of ADC
			
// 		ADMUX = 0b11100001;				// AREF=2.56v, ADLAR=1, ADC1
// 		ADCSRA |= 0b01000000;			// Start ADC
// 		while ( ADCSRA & 0b01000000 ) ;	// Wait for completion
			
// 		int temp2 = ADCH;
// 		PORTB = temp2;					// Show MSB of ADC
			
// 		wait(500);						// every 50 ms (busy waiting)
// 	}
    
// }

#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>

void twi_init(void)
{
	TWSR = 0;
	TWBR = 32;	 // TWI clock set to 100kHz, prescaler = 0
}

void twi_start(void)
{
	TWCR = (0x80 | 0x20 | 0x04);
	while( 0x00 == (TWCR & 0x80) );
}

void twi_stop(void)
{
	TWCR = (0x80 | 0x10 | 0x04);
}

void twi_tx(unsigned char data)
{
	TWDR = data;
	TWCR = (0x80 | 0x04);
	while( 0 == (TWCR & 0x80) );
}

#define BIT(x)			(1 << (x))

void wait( int ms )
{
	for (int i=0; i<ms; i++)
	{
		_delay_ms( 1 );		// library function (max 30 ms at 8MHz)
	}
}

void send_byte(int addr, int data) {
	twi_start();
	twi_tx(0xE0);	// Display I2C addres + R/W bit
	twi_tx(addr);	// Address
	twi_tx(data);	// data
	twi_stop();
}

int main( void )
{
	
	twi_init();		// Init TWI interface

	// Init HT16K22. Page 32 datasheet
	twi_start();
	twi_tx(0xE0);	// Display I2C addres + R/W bit
	twi_tx(0x21);	// Internal osc on (page 10 HT16K33)
	twi_stop();

	twi_start();
	twi_tx(0xE0);	// Display I2C address + R/W bit
	twi_tx(0xA0);	// HT16K33 pins all output
	twi_stop();

	twi_start();
	twi_tx(0xE0);	// Display I2C address + R/W bit
	twi_tx(0xE3);	// Display Dimming 4/16 duty cycle
	twi_stop();

	twi_start();
	twi_tx(0xE0);	// Display I2C address + R/W bit
	twi_tx(0x81);	// Display OFF - Blink On
	twi_stop();
	
	send_byte(0x00, 0x00);
	send_byte(0x02, 0x00);
	send_byte(0x04, 0x00);
	send_byte(0x06, 0x00);
	send_byte(0x08, 0x00);
	send_byte(0x0a, 0x00);
	send_byte(0x0c, 0x00);
	send_byte(0x0e, 0x00);

	int bytecode = 0x01;
	TCCR1B = 0b00000101;		// Initialize T1: timer, prescaler=1024, CTC,RUN
	
	while (1)
	{
		// 0x3D
		if (TCNT1 >= 15625) {
			TCNT1 = 0;
			if (bytecode == 0x40)
			{
				bytecode = 0x01;
			}
			else
			{
				bytecode = bytecode << 1;
			}
		}
		
		send_byte(0x00, bytecode);
		
		
		
		
		wait(50);
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		/*
		send_byte(0x00, 0x00);
		send_byte(0x02, 0x00);
		send_byte(0x04, 0x00);
		send_byte(0x06, 0x00);
		send_byte(0x08, 0x00);
		send_byte(0x0a, 0x00);
		send_byte(0x0c, 0x00);
		send_byte(0x0e, 0x00);
		//send_byte(0x02, 0x00);
		//send_byte(0x03, 0x00);
		//send_byte(0x04, 0x00);
		//send_byte(0x05, 0x00);
		wait(500);	


		send_byte(0x00, 0x80);
		send_byte(0x02, 0xff);
		send_byte(0x04, 0xff);
		send_byte(0x06, 0xff);
		send_byte(0x08, 0xff);
		send_byte(10, 0xff);
		send_byte(0x0c, 0xff);
		send_byte(0x0e, 0xff);
		//send_byte(0x02, 0xff);
		//send_byte(0x03, 0xff);
		//send_byte(0x04, 0xff);
		//send_byte(0x05, 0xff);

		wait(500);
		*/
	}

	return 1;
}
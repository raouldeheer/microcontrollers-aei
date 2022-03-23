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

void setValueInValues(int values[64], int x, int y, int v)
{
	values[(x-1) + ((y-1)*8)] = v;
}

int getValueInValues(int values[64], int x, int y)
{
	return values[(x-1) + ((y-1)*8)];
}

void value_to_display(int values[64]) 
{
	for (int i = 0; i<8; i++)
	{
		int byteToPrint = 0;
		for (int j = 0; j<8; j++)
		{
			if (j == 0)
			{
			byteToPrint |= (values[j + (i*8)] << 7);
			} 
			else 
			{
			byteToPrint |= (values[j + (i*8)] << (j-1));
			}
		}
		send_byte(i*2, byteToPrint);
	}
}

int checkNextMove(int values[64], int x, int y)
{
	if (getValueInValues(values, x, y+1) || y>=8)
	{
		return 1;
	}
	return 0;
}

int checkFullLine(int values[64], int y)
{
	return getValueInValues(values, 1, y) 
		&& getValueInValues(values, 2, y)
		&& getValueInValues(values, 3, y)
		&& getValueInValues(values, 4, y)
		&& getValueInValues(values, 5, y)
		&& getValueInValues(values, 6, y)
		&& getValueInValues(values, 7, y)
		&& getValueInValues(values, 8, y);
}

void clearLine(int values[64], int y) 
{
	for (int i = 1; i<9; i++)
	{
		setValueInValues(values, i, y, 0);
	}
}

void clearAndMoveLine(int values[64], int y) 
{
	for (int i = y; i>1; i--)
	{
		clearLine(values, i);
		for (int j = 1; j<9; j++)
		{
			setValueInValues(values, j, i, getValueInValues(values, j, i-1));
		}
	}
	clearLine(values, 1);
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

	//int bytecode = 0x01;
	DDRF = 0x00;				// set PORTF for input (ADC)
	
	ADMUX = 0b11100001;			// AREF=2.56v, ADLAR=1, ADC1
	ADCSRA = 0b10000110;		// ADC-enable, not started, not free running, division by 64
	TCCR1B = 0b00000101;		// Initialize T1: timer, prescaler=1024, CTC,RUN
	
	int values[64] = {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		//1, 1, 0, 1, 1, 1, 1, 1,
		//1, 1, 0, 1, 1, 1, 1, 1,
	};
	int liveValues[64] =  {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
	};
	int blockx = 3;
	int blocky = 0;
	int isSinking = 1;
	
	int control1;
	int upDownControl;
		
	while (1)
	{
		/*
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
		*/
		
		ADMUX = 0b11100000;				// AREF=2.56v, ADLAR=1, ADC0
		ADCSRA |= 0b01000000;			// Start ADC
		while ( ADCSRA & 0b01000000 ) ;	// Wait for completion
		
		control1 = ADCH;
		
		ADMUX = 0b11100001;				// AREF=2.56v, ADLAR=1, ADC1
		ADCSRA |= 0b01000000;			// Start ADC
		while ( ADCSRA & 0b01000000 ) ;	// Wait for completion
		
		upDownControl = ADCH;
		
		for (int i = 0; i < 64; i++) {
			liveValues[i] = values[i];
		}
		
		if (control1 > 190)
		{
			// Right
			//setValueInValues(liveValues, 8, 2, 1);
			if (blockx < 8)
			{
				if (getValueInValues(values, blockx+1, blocky)==0) 
				{
					blockx++;
				}
			}
		} 
		else if (control1 < 60)
		{
			// Left
			//setValueInValues(liveValues, 6, 2, 1);
			if (blockx > 1)
			{
				if (getValueInValues(values, blockx-1, blocky)==0)
				{
					blockx--;
				}
			}
		}
		
		if (upDownControl > 190)
		{
			// Up
			//setValueInValues(liveValues, 7, 1, 1);
		}
		else if (upDownControl < 60)
		{
			// Down
			//setValueInValues(liveValues, 7, 3, 1);
		}
		
		if (isSinking)
		{
			if (checkNextMove(values, blockx, blocky))
			{
				isSinking=0;
				setValueInValues(liveValues, blockx, blocky, 1);
				setValueInValues(values, blockx, blocky, 1);
				if (blocky == 0)
				{
					//setValueInValues(values, 6, 1, 1);
				}
			}
			else
			{
				blocky++;
				setValueInValues(liveValues, blockx, blocky, 1);
			}
		}
		else
		{
			//setValueInValues(values, 1, 1, 1);
		}
		if (checkFullLine(values, 8)) {
			clearAndMoveLine(values, 8);
			//setValueInValues(values, 2, 2, 1);
		}
		
		if (isSinking == 0)
		{
			blockx = 3;
			blocky = 0;
			isSinking = 1;
		}
		value_to_display(liveValues);
		
	
		
		
		wait(500);
	}

	return 1;
}
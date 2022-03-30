/*
 * main.c
 *
 * Created: 3/16/2022 11:04:54 AM
 *  Author: raoul
 */
#define F_CPU 8e6
#include <avr/io.h>
#include <util/delay.h>
#include <time.h>
#include "shapes.h"

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

int checkSingleBlock(int values[64], int x, int y, POS checkOffset)
{
	if (getValueInValues(values, x + checkOffset.x, y+checkOffset.y) || y>8)
	{
		return 1;
	}
	return 0;
}

int checkNextMove(int values[64], POS block[4], POS checkOffset)
{
	int result = checkSingleBlock(values, block[0].x, block[0].y, checkOffset);
	for (int i = 1; i<4; i++)
	{
		result += checkSingleBlock(values, block[0].x + block[i].x, block[0].y + block[i].y, checkOffset);
	}
	return result;
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

int checkEmptyLine(int values[64], int y)
{
	return getValueInValues(values, 1, y)
	|| getValueInValues(values, 2, y)
	|| getValueInValues(values, 3, y)
	|| getValueInValues(values, 4, y)
	|| getValueInValues(values, 5, y)
	|| getValueInValues(values, 6, y)
	|| getValueInValues(values, 7, y)
	|| getValueInValues(values, 8, y);
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

void drawShape(int values[64], POS block[4])
{
	setValueInValues(values, block[0].x, block[0].y, 1);
	for (int i = 1; i<4; i++)
	{
		setValueInValues(values, block[0].x + block[i].x, block[0].y + block[i].y, 1);
	}
}

int wallCheckRight(POS block[4])
{
	return block[0].x < 8 &&
	block[0].x + block[1].x < 8 &&
	block[0].x + block[2].x < 8 &&
	block[0].x + block[3].x < 8;
}

int wallCheckLeft(POS block[4])
{
	return block[0].x > 1 &&
	block[0].x + block[1].x > 1 &&
	block[0].x + block[2].x > 1 &&
	block[0].x + block[3].x > 1;
}

int wallCheckRightTurn(POS block[4])
{
	return block[0].x <= 8 &&
	block[0].x + block[1].x <= 8 &&
	block[0].x + block[2].x <= 8 &&
	block[0].x + block[3].x <= 8;
}

int wallCheckLeftTurn(POS block[4])
{
	return block[0].x >= 1 &&
	block[0].x + block[1].x >= 1 &&
	block[0].x + block[2].x >= 1 &&
	block[0].x + block[3].x >= 1;
}

int values[64] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
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
POS block[4];
int rotation;



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

	block[0].x = 4;
	block[0].y = 1;
	rotation = 0;
	
	void (*ptr[4]) (POS block[4]);
	
	void (*ptrO[4]) (POS block[4]) = {makeShapeO, makeShapeO, makeShapeO, makeShapeO};
	void (*ptrI[4]) (POS block[4]) = {makeShapeI1, makeShapeI2, makeShapeI3, makeShapeI4};
	void (*ptrT[4]) (POS block[4]) = {makeShapeT1, makeShapeT2, makeShapeT3, makeShapeT4};
	void (*ptrS[4]) (POS block[4]) = {makeShapeS1, makeShapeS2, makeShapeS3, makeShapeS4};
	void (*ptrZ[4]) (POS block[4]) = {makeShapeZ1, makeShapeZ2, makeShapeZ3, makeShapeZ4};
	void (*ptrJ[4]) (POS block[4]) = {makeShapeJ1, makeShapeJ2, makeShapeJ3, makeShapeJ4};
	void (*ptrL[4]) (POS block[4]) = {makeShapeL1, makeShapeL2, makeShapeL3, makeShapeL4};
		
	void (**ptrptr[7]) (POS block[4]) = {ptrO,ptrI,ptrT,ptrS,ptrZ,ptrJ,ptrL};
	

	ADMUX = 0b11100000;				// AREF=2.56v, ADLAR=1, ADC0
	ADCSRA |= 0b01000000;			// Start ADC
	while ( ADCSRA & 0b01000000 ) ;	// Wait for completion
	int randData = ADCH;
	
	ADMUX = 0b11100001;				// AREF=2.56v, ADLAR=1, ADC1
	ADCSRA |= 0b01000000;			// Start ADC
	while ( ADCSRA & 0b01000000 ) ;	// Wait for completion
	int randData2 = ADCH;
	
	srand(randData*randData*randData2*randData2);
	int number = ((rand() % (7 - 1 + 1)) + 1) - 1;
	ptr[0] = ptrptr[number][0];
	ptr[1] = ptrptr[number][1];
	ptr[2] = ptrptr[number][2];
	ptr[3] = ptrptr[number][3];
	int isSinking = 1;
	
	int leftRightControl;
	int upDownControl;
			
	while (1)
	{
		ADMUX = 0b11100000;				// AREF=2.56v, ADLAR=1, ADC0
		ADCSRA |= 0b01000000;			// Start ADC
		while ( ADCSRA & 0b01000000 ) ;	// Wait for completion
		
		leftRightControl = ADCH;
		
		ADMUX = 0b11100001;				// AREF=2.56v, ADLAR=1, ADC1
		ADCSRA |= 0b01000000;			// Start ADC
		while ( ADCSRA & 0b01000000 ) ;	// Wait for completion
		
		upDownControl = ADCH;
		
		for (int i = 0; i < 64; i++) {
			liveValues[i] = values[i];
		}
		ptr[rotation](block);
		
		if (leftRightControl > 190)
		{
			// Right
			if (wallCheckRight(block))
			{
				POS checkOffset;
				checkOffset.x = 1;
				checkOffset.y = 0;
				if (checkNextMove(values, block, checkOffset) == 0)
				{
					block[0].x++;
				}
			}
		} 
		else if (leftRightControl < 60)
		{
			// Left
			if (wallCheckLeft(block))
			{
				POS checkOffset;
				checkOffset.x = -1;
				checkOffset.y = 0;
				if (checkNextMove(values, block, checkOffset) == 0)
				{
					block[0].x--;
				}
			}
		}
		
		if (upDownControl > 190)
		{
			// Up
			int oldRotation = rotation;
			rotation++;
			if (rotation >= 4) {
				rotation = 0;
			}
			ptr[rotation](block);
			POS checkOffset;
			checkOffset.x = 0;
			checkOffset.y = 0;
			if (checkNextMove(values, block, checkOffset) > 0 ||
				!(wallCheckLeftTurn(block) && wallCheckRightTurn(block)))
			{
				rotation = oldRotation;
				ptr[rotation](block);
			}
		}
		else if (upDownControl < 60)
		{
			// Down
			int oldRotation = rotation;
			rotation--;
			if (rotation <= -1) {
				rotation = 3;
			}
			ptr[rotation](block);
			POS checkOffset;
			checkOffset.x = 0;
			checkOffset.y = 0;
			if (checkNextMove(values, block, checkOffset) > 0 ||
				!(wallCheckLeftTurn(block) && wallCheckRightTurn(block)))
			{
				rotation = oldRotation;
				ptr[rotation](block);
			}
		}
		
		// 0x3D == 15625
		if (TCNT1 >= 12625) {
			TCNT1 = 0;
			if (isSinking)
			{
				POS checkOffset;
				checkOffset.x = 0;
				checkOffset.y = 1;
				if (checkNextMove(values, block, checkOffset))
				{
					isSinking=0;
					drawShape(liveValues, block);
					drawShape(values, block);
				}
				else
				{
					block[0].y++;
					drawShape(liveValues, block);
				}
			}
		
			for (int i = 0; i<7; i++)
			{
				if (checkFullLine(values, 8-i)) {
					clearAndMoveLine(values, 8-i);
				}
			}
		
			if (isSinking == 0)
			{
				if (checkEmptyLine(values, 1))
				{	
					int gameoverValues[64] = {
						0, 0, 0, 0, 0, 0, 0, 0,
						0, 1, 1, 1, 1, 1, 1, 0,
						0, 1, 0, 0, 0, 0, 1, 0,
						0, 1, 0, 1, 1, 0, 1, 0,
						0, 1, 0, 1, 1, 0, 1, 0,
						0, 1, 0, 0, 0, 0, 1, 0,
						0, 1, 1, 1, 1, 1, 1, 0,
						0, 0, 0, 0, 0, 0, 0, 0,
					};
					int gameoverValues2[64] = {
						1, 1, 1, 1, 1, 1, 1, 1,
						1, 0, 0, 0, 0, 0, 0, 1,
						1, 0, 1, 1, 1, 1, 0, 1,
						1, 0, 1, 0, 0, 1, 0, 1,
						1, 0, 1, 0, 0, 1, 0, 1,
						1, 0, 1, 1, 1, 1, 0, 1,
						1, 0, 0, 0, 0, 0, 0, 1,
						1, 1, 1, 1, 1, 1, 1, 1,
					};
					while(1){
						value_to_display(gameoverValues);
						wait(500);
						value_to_display(gameoverValues2);
						wait(500);
					}
				}
				block[0].x = 4;
				block[0].y = 1;
				rotation = 0;
				isSinking = 1;
				int number = ((rand() % (7 - 1 + 1)) + 1) - 1;
				ptr[0] = ptrptr[number][0];
				ptr[1] = ptrptr[number][1];
				ptr[2] = ptrptr[number][2];
				ptr[3] = ptrptr[number][3];
			}
		} else {
			drawShape(liveValues, block);		
		}

		value_to_display(liveValues);
	
		wait(100);
	}

	return 1;
}
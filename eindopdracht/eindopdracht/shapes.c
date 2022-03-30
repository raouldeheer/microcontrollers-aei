/*
 * shapes.c
 *
 * Created: 30-3-2022 14:49:53
 *  Author: raoul
 */ 
#include "shapes.h"
void makeShapeO(POS block[4])
{
	block[1].x = 0;
	block[1].y = -1;
	block[2].x = 1;
	block[2].y = -1;
	block[3].x = 1;
	block[3].y = 0;
}

void makeShapeI2(POS block[4])
{
	block[1].x = 0;
	block[1].y = 1;
	block[2].x = 0;
	block[2].y = 2;
	block[3].x = 0;
	block[3].y = -1;
}

void makeShapeI1(POS block[4])
{
	block[1].x = 1;
	block[1].y = 0;
	block[2].x = 2;
	block[2].y = 0;
	block[3].x = -1;
	block[3].y = 0;
}

void makeShapeI4(POS block[4])
{
	block[1].x = 0;
	block[1].y = -1;
	block[2].x = 0;
	block[2].y = -2;
	block[3].x = 0;
	block[3].y = 1;
}

void makeShapeI3(POS block[4])
{
	block[1].x = -1;
	block[1].y = 0;
	block[2].x = -2;
	block[2].y = 0;
	block[3].x = 1;
	block[3].y = 0;
}

void makeShapeT1(POS block[4])
{
	block[1].x = 1;
	block[1].y = 0;
	block[2].x = 0;
	block[2].y = -1;
	block[3].x = -1;
	block[3].y = 0;
}

void makeShapeT2(POS block[4])
{
	block[1].x = 0;
	block[1].y = -1;
	block[2].x = 0;
	block[2].y = 1;
	block[3].x = 1;
	block[3].y = 0;
}

void makeShapeT3(POS block[4])
{
	block[1].x = 1;
	block[1].y = 0;
	block[2].x = -1;
	block[2].y = 0;
	block[3].x = 0;
	block[3].y = 1;
}

void makeShapeT4(POS block[4])
{
	block[1].x = 0;
	block[1].y = 1;
	block[2].x = 0;
	block[2].y = -1;
	block[3].x = -1;
	block[3].y = 0;
}

void makeShapeS1(POS block[4])
{
	block[1].x = 1;
	block[1].y = -1;
	block[2].x = 0;
	block[2].y = -1;
	block[3].x = -1;
	block[3].y = 0;
}

void makeShapeS2(POS block[4])
{
	block[1].x = 0;
	block[1].y = -1;
	block[2].x = 1;
	block[2].y = 0;
	block[3].x = 1;
	block[3].y = 1;
}

void makeShapeS3(POS block[4])
{
	block[1].x = 1;
	block[1].y = 0;
	block[2].x = 0;
	block[2].y = 1;
	block[3].x = -1;
	block[3].y = 1;
}

void makeShapeS4(POS block[4])
{
	block[1].x = -1;
	block[1].y = 0;
	block[2].x = 0;
	block[2].y = 1;
	block[3].x = -1;
	block[3].y = -1;
}

void makeShapeZ1(POS block[4])
{
	block[1].x = 1;
	block[1].y = 0;
	block[2].x = 0;
	block[2].y = -1;
	block[3].x = -1;
	block[3].y = -1;
}

void makeShapeZ2(POS block[4])
{
	block[1].x = 1;
	block[1].y = 0;
	block[2].x = 0;
	block[2].y = 1;
	block[3].x = 1;
	block[3].y = -1;
}

void makeShapeZ3(POS block[4])
{
	block[1].x = -1;
	block[1].y = 0;
	block[2].x = 0;
	block[2].y = 1;
	block[3].x = 1;
	block[3].y = 1;
}

void makeShapeZ4(POS block[4])
{
	block[1].x = -1;
	block[1].y = 0;
	block[2].x = 0;
	block[2].y = -1;
	block[3].x = -1;
	block[3].y = 1;
}

void makeShapeJ1(POS block[4])
{
	block[1].x = -1;
	block[1].y = 0;
	block[2].x = 1;
	block[2].y = 0;
	block[3].x = -1;
	block[3].y = -1;
}

void makeShapeJ2(POS block[4])
{
	block[1].x = 0;
	block[1].y = 1;
	block[2].x = 0;
	block[2].y = -1;
	block[3].x = 1;
	block[3].y = -1;
}

void makeShapeJ3(POS block[4])
{
	block[1].x = 1;
	block[1].y = 1;
	block[2].x = 1;
	block[2].y = 0;
	block[3].x = -1;
	block[3].y = 0;
}

void makeShapeJ4(POS block[4])
{
	block[1].x = 0;
	block[1].y = 1;
	block[2].x = 0;
	block[2].y = -1;
	block[3].x = -1;
	block[3].y = 1;
}

void makeShapeL1(POS block[4])
{
	
	block[1].x = -1;
	block[1].y = 0;
	block[2].x = 1;
	block[2].y = 0;
	block[3].x = 1;
	block[3].y = -1;
}

void makeShapeL2(POS block[4])
{
	block[1].x = 0;
	block[1].y = -1;
	block[2].x = 0;
	block[2].y = 1;
	block[3].x = 1;
	block[3].y = 1;
}

void makeShapeL3(POS block[4])
{
	block[1].x = -1;
	block[1].y = 1;
	block[2].x = 1;
	block[2].y = 0;
	block[3].x = -1;
	block[3].y = 0;
}
void makeShapeL4(POS block[4])
{
	block[1].x = -1;
	block[1].y = -1;
	block[2].x = 0;
	block[2].y = -1;
	block[3].x = 0;
	block[3].y = 1;
}
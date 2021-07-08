/***************************************************************************************
* File Name          : shiftreg.c
* Author             : D.Gotic
* Version            : V0.1
* Date               : 03/12/2010
* Description        : Test program for the 74HC595 lib. Four 7 seg displays are hooked
*					 : on outputs of four 74HC595 shift registers. 
****************************************************************************************/

/* AVR-GCC includes */
#include <avr/io.h>
#include <util/delay.h>

/* 74HC595 library */
#include "shiftreg.h"

/* 7 Segment Codes */
#define ZERO	0x3F
#define	ONE		0x06
#define TWO		0x5B
#define THREE	0x4F
#define FOUR	0x66
#define FIVE	0x6D
#define SIX		0x7D
#define SEVEN	0x07
#define EIGHT	0x7F
#define NINE	0x6F

int main(void)
{
	/* an array which holds segments for digits from 0...9 */
	uint8_t digits[] = {ZERO,ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE};
	uint8_t i;

	/* 4 SHIFT_register variables declared */
	SHIFTREG_register reg1,reg2,reg3,reg4;	

	/* Initialization of registers */
	reg1 = SHIFTREG_InitRegister(&DDRD,&PORTD,PD0,PD1,PD2);
	reg2 = SHIFTREG_InitRegister(&DDRC,&PORTC,PC0,PC1,PC2);
	reg3 = SHIFTREG_InitRegister(&DDRC,&PORTC,PC3,PC4,PC5);	
	reg4 = SHIFTREG_InitRegister(&DDRD,&PORTD,PC3,PC4,PC5);	

	while(1)
	{
		/* a simple counter with a 500ms delay 
		   counting from 0 to 9 and displaying
		   those digits on all four 7 segment 
		   displays */
		for (i=0; i< 10; i++)
		{
			SHIFTREG_OutputValue(&reg1,digits[i]);
			SHIFTREG_OutputValue(&reg2,digits[i]);
			SHIFTREG_OutputValue(&reg3,digits[i]);
			SHIFTREG_OutputValue(&reg4,digits[i]);
			_delay_ms(500);
		}
	}

	return 0;
}

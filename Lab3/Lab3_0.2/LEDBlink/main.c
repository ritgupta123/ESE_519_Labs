/*
 * LEDBlink.c
 *
 * Created: 06-09-2018 19:01:13
 * Author : rgjus
 */ 

#define F_CPU 16000000L

#include <avr/io.h>
//#include <util/delay.h>

int main(void)
{
	DDRB = 0x20;
	PORTB = 0;
	
    /* Replace with your application code */
    while (1) 
    {
		if ((PINB & 0x01) == 0x00)
			PORTB = PORTB | 0x20;
		else
			PORTB = PORTB & 0xDF;
    }
}


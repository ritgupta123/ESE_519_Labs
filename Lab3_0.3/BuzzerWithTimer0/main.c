/*
 * BuzzerWithTimer0.c
 *
 * Created: 12-09-2018 09:26:01
 * Author : rgjus
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>


// initialize timer, interrupt and variable
void timer0_init()
{
	// set up timer with prescaler = 1024
	TCCR0B |= (1 << CS02) /*| (1 << CS00)*/;
	
	// initialize counter
	TCNT0 = 0;
}

int main(void)
{
	// connect buzzer to pin PD6
	DDRD |= (1 << 6);
	
	// initialize timer
	timer0_init();
	
	// loop forever
	while(1)
	{
			// check if the timer count reaches 250
			if (TCNT0 >= 62)
			{
	
				PORTD ^= (1 << 6);	   // Play the buzzer (At frequency = 1008Hz, Time period = 992us)
				TCNT0 = 0;            // reset counter
			}
	}
	
}


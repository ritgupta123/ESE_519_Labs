/*
 * LedBlinkUsingInputCapture.c
 *
 * Created: 13-09-2018 09:30:22
 * Author : rgjus
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

void timer1_init()
{
	TCNT1 = 0;	// Initializes the timer count value
		
	TCCR1B &= 0xBF; // Sets mode of operation of timer(Normal by default), and edge for trigger(Falling edge on 6th bit - 0)
	
	 TIFR1 = (1 << ICF1); // Clears Input Capture flag
	 
	 TIMSK1 |= (1 <<ICIE1); //Enables Input Capture Interrupt
	 
	 sei();	// Enables global interrupts
}

ISR(TIMER1_CAPT_vect)
{	
	// Toggle the LED
	PORTB ^= (1 << 5);
	
	// Toggle the edge trigger 
	TCCR1B ^= (1 << 6);
}

int main(void)
{
	DDRB = 0x20;
	PORTB = 0;;
	
	timer1_init();
	
    /* Replace with your application code */
    while (1) 
    {
		
    }
}


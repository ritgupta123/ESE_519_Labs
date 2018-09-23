#define F_CPU = 16000000L

#include <avr/io.h>
#include <avr/interrupt.h>

// Initialize the timer and interrupts
void timer0_init()
{
	TCNT0 = 0;	// Initialize the timer count value to 0
	
	//TCCR0A |= (1 << COM0A0);	// Sets the toggle OC0A on Compare Match mode
	
	TCCR0A |= (1 << WGM01);		// Set the mode of operation of timer to CTC
	
	OCR0A = 239;					// Initialize the output compare value
	
	TIMSK0 = (1 << OCIE0A);		// Enable output compare A match interrupt enable
	
	TCCR0B |= (1 << CS00) | (1 << CS01);      // Enable the timer with prescaler(64) specification

	sei();						// Enable global interrupts
}

ISR(TIMER0_COMPA_vect)
{
	// Toggle PD6
	// PORTD ^= (1 << 6);
	
	TCNT0 = 0;
	
	
}

int main(void)
{
	DDRD = 0x40;
	PORTD = 0;
	
	timer0_init();
	
	/* Replace with your application code */
	while (1)
	{
	}
}
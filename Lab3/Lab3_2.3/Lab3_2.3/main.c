/*
 * Lab3_2.2.c
 *
 * Created: 18-09-2018 17:12:19
 * Author : rgjus
 */ 

#define F_CPU 16000000UL
#define BAUD 9600
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "uart.h"

#define NUM_TOTAL_TICKS		65535
#define CLOCK_TIME_PERIOD_US	0.0625
#define MAX_BUZZER_CYCLE_COUNT	239
#define MIN_BUZZER_CYCLE_COUNT	119


long int rising_edge_time, falling_edge_time;
volatile bool edges_detected = 0;
int num_overflows = 0;

void timer1_init()
{
	TCCR1B |= (1 << CS10);      // Enable the timer with no prescaling
	
	TCNT1 = 0;	// Initialize the timer count value to 0
	
	TCCR1A |= (1 << COM1A0);	// COmpare output mode - Toggle OC1A on Compare Match
	
	TIMSK1 = (1 << OCIE1A);		// Enable output compare A match interrupt enable
	
	OCR1A = 80;					// Initialize the output compare value

	sei();						// Enable global interrupts
}

ISR(TIMER1_COMPA_vect)
{
	TCNT1 = 0;		// Initialize the timer count value to 0

	//TCCR1A &= ~(1 << COM1A0);	// Disable the output compare on timer1
	
	TIMSK1 &= ~(1 << OCIE1A);	// Disable the output compare match interrupt
					
	TCCR1B |= (1 << ICES1);	// Enable the input capture trigger on rising edge
	 	
	TIMSK1 = (1 << ICIE1);	// Enable the input capture interrupt	
	
	TIMSK1 |= (1 < TOIE1);	// Enable the overflow interrupt
	
	sei();						// Enable global interrupts
}

ISR(TIMER0_COMPA_vect)
{
	
}

ISR(TIMER1_OVF_vect)
{
	num_overflows++;
}

ISR(TIMER1_CAPT_vect)
{
	// Check if trigger is set on rising edge
	if (TCCR1B & (1 << ICES1))
	{
		rising_edge_time = ICR1;	// Get the time at which rising edge is received
		TCCR1B &= ~(1 << ICES1);		// Enable the input capture trigger on falling edge
	}
	else
	{
		falling_edge_time = ICR1;	// Get the time at which falling edge is received
		TCCR1B |= (1 << ICES1);		// Enable the input capture trigger on rising edge
		edges_detected = 1;
	}
}

// Initialize the timer and interrupts
void timer0_init()
{
	TCNT0 = 0;	// Initialize the timer count value to 0
	
	TCCR0A |= (1 << COM0A0);	// Sets the toggle OC0A on Compare Match mode
	
	TCCR0A |= (1 << WGM01);		// Set the mode of operation of timer to CTC
	
	OCR0A = (int)(MIN_BUZZER_CYCLE_COUNT / 2);					// Initialize the output compare value
	
	TIMSK0 = (1 << OCIE0A);		// Enable output compare A match interrupt enable
	
	TCCR0B |= (1 << CS00) | (1 << CS01);      // Enable the timer with prescaler(64) specification

	//sei();						// Enable global interrupts
}

int get_cycle_count(int pulse_width)
{
	if (pulse_width < 150)
		return 119;		
	else if (pulse_width < 444)
		return 127;
	else if (pulse_width < 738)
		return 142;
	else if (pulse_width < 1032)
		return 159;
	else if (pulse_width < 1326)
		return 179;
	else if (pulse_width < 1620)
		return 190;
	else if (pulse_width < 1914)
		return 213;
	else
		return 239;
}

int main(void)
{
	uart_init();

	//printf("Initializing system\n");
		
	DDRB = 0x02;		// Set PB1 to be output pin and PB0 as input pin
	DDRD = 0x40;		// For buzzer
	
    PORTB = (1 << 1);	// Set output pin to high	
	
	// Initialize the timer
	timer0_init();
	timer1_init();
	
	while(1)
	{
		//printf("edges\n");
		if (edges_detected) 
		{
			long int pulse_width, num_clock_ticks;
			
			// Get the pulse width
			if (num_overflows == 0)
			{
				num_clock_ticks = falling_edge_time - rising_edge_time;
			}
			else
			{
				num_clock_ticks = (NUM_TOTAL_TICKS - rising_edge_time) + (num_overflows * NUM_TOTAL_TICKS) + falling_edge_time;
			}
			pulse_width = num_clock_ticks * CLOCK_TIME_PERIOD_US;
			
			// Print the width on terminal
			char result[64];
			snprintf(result, 64, "Pulse width is %ld\n", pulse_width);
			puts(result);
		
			// Update the OCR0A value depending on pulse width
			int cycle_count = (0.092 * pulse_width) + 102.38;
			
			if (cycle_count > MAX_BUZZER_CYCLE_COUNT)
				cycle_count = MAX_BUZZER_CYCLE_COUNT;
			if (cycle_count < MIN_BUZZER_CYCLE_COUNT)
				cycle_count = MIN_BUZZER_CYCLE_COUNT;
				
			OCR0A = (int)(cycle_count / 2);
			printf("%d \n", OCR0A);
			
			num_overflows = 0;
			edges_detected = 0;
		}
	}
}
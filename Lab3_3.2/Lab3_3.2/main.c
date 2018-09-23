#define F_CPU 16000000UL
#define BAUD 9600

#include <avr/io.h>
#include "uart.h"
#include <stdio.h>
#include <util/delay.h>

uint16_t adc_out;

void adc_init()
{
	ADMUX |= (1 << REFS0);   // Set AVcc as voltage reference and by default, ADC0(PC0) is selected in ADMUX

	ADCSRA |= (1 << ADEN);   // Enable ADC
	
	ADCSRA |= (1 << ADIE);	// Enable interrupt
	
	//SREG |= (1 << I);		// Enable global interrupt to enable interrupt for ADC
	
	ADCSRA |= (1 << ADATE);   // Set ADC to auto trigger mode (free running for ADTS = '000' in ADCSRB register)
	
	ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2);  // Set prescaler to 128(To achieve frequency 125KHz) for '111'
	
	ADCSRA |= (1 << ADSC);	// Start the conversion
}

ISR(ADC_vect)
{
	adc_out = ADC
}

int main(void)
{
	ADMUX |= 0x00;   //set ADC0 as input
	DDRB = 0x1C;   //set PB2,PB3,PB4 to output
	
	uart_init();
	adc_init();
	
	while (1)
	{		
		if (adc_out <= 118)
		{
			PORTB = 0x00;   //set PB[4:2] to '000'
		}
		else if (adc_out <= 301)
		{
			PORTB = 0x04;   //set PB[4:2] to '001'
		}
		else if (adc_out <= 420)
		{
			PORTB = 0x08;   //set PB[4:2] to '010'
		}
		else if (adc_out <= 540)
		{
			PORTB = 0x0C;   //set PB[4:2] to '011'
		}
		else if (adc_out <= 659)
		{
			PORTB = 0x10;   //set PB[4:2] to '100'
		}
		else if (adc_out <= 778)
		{
			PORTB = 0x14;   //set PB[4:2] to '101'
		}
		else if (adc_out <= 898)
		{
			PORTB = 0x18;   //set PB[4:2] to '110'
		}
		else
		{
			PORTB = 0x1C;   //set PB[4:2] to '111'
		}
		
		char result[64];
		snprintf(result, 64, "ADC output is %d\n", adc_out);
		printf(result);
		//_delay_ms(100);
	}
}
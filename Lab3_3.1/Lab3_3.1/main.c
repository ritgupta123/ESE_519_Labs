+#define F_CPU 16000000UL
+#define BAUD 9600

#include <avr/io.h>
#include "uart.h"
#include <stdio.h>

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
	ADMUX |= 0x00;   //set AD0 as input
	
	char result[64];
	
	uart_init();
	adc_init();
	
	while (1)
	{
		snprintf(result, 64, "ADC output is %d\n", adc_out);
		printf(result);
	}
}
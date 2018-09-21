#define F_CPU 16000000UL
#define BAUD 9600

#include <avr/io.h>
#include "uart.h"
#include <stdio.h>
#include <util/delay.h>


void adc_init()
{
	ADMUX |= (1 << REFS0);   //set AVcc as voltage reference
	ADCSRA |= 0x07;  //prescaler is 128 for '111'
	ADCSRA |= (1 << ADEN);   //turn ADC on
	//ADCSRA |= (1 << ADATE);   //set ADC to auto trigger mode (free running for ADTS = '000')
}


uint16_t ReadADC(uint8_t ADCchannel)
{
	//select ADC channel with safety mask
	ADMUX = (ADMUX & 0xF0) | (ADCchannel & 0x0F);
	//single conversion mode
	ADCSRA |= (1<<ADSC);
	// wait until ADC conversion is complete
	while( ADCSRA & (1<<ADSC) );
	return ADC;
}


int main(void)
{
	ADMUX |= 0x00;   //set AD0 as input
	
	uint16_t adc_out;
	
	adc_init();
	uart_init();
	
	//ADCSRA |= (1 << ADSC);   //start first conversion
	
	//printf(adc_out);
	//printf("\n");
	
	while (1)
	{
		//printf("start");
		
		adc_out = ReadADC(0);
		
		char result[64];
		snprintf(result, 64, "ADC output is %d\n", adc_out);
		printf(result);
		//printf(adc_out);
		_delay_ms(100);
	}
}
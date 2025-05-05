#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#include "adc.h"

void initADC(){
    ADMUX |= (1<<ADC1D); //reference voltge instelle 
    ADMUX = (1 << REFS0); //instelle van de voltage
    ADCSRA  |= (1<<ADEN); //enablen van ADC
    ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2); //setten van de clockfrequentie
}
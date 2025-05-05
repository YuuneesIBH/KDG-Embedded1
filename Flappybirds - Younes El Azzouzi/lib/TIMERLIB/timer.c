#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "led.h"
#include "button.h"
#include "usart.h"
#include <stdlib.h>
#include "display.h"

void initTimer2() {
    TCCR2A |= (1 << WGM21); //timer2 op CTC modus
    TCCR2B |= (1 << CS22) | (1 << CS21);
    OCR2A = 249; //compare match value 
    TIMSK2 |= (1 << OCIE2A); //compare match aandoen
}
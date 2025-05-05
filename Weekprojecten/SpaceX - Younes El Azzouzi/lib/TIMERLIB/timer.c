#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "led.h"
#include "button.h"
#include "usart.h"
#include <stdlib.h>
#include "display.h"

void initTimer() {
    TCCR2A |= (1<<WGM21) | (1<<WGM20);
    TIMSK2 |= (1<<OCIE2A);
    sei();
}

void startTimer() {
    TCCR2B |= (1<<CS22); //prescaler is 256 elke 16 microseconden
    OCR2A = 249; //na 250 tellen zitten we op 250 x 16 microseconden
}

void stopTimer() {
    //prescaler op 0 zetten
    TCCR2B = 0;
}
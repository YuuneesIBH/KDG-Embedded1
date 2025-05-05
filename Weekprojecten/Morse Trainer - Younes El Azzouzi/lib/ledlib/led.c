#include "led.h"
#include <avr/io.h>
#include <util/delay.h>

//enable methods voor het activeren van de ledjes (pins)
//lightUp methods voor het aanzetten van de ledjes 
//lightDown methods voor het uitzetten van de ledjes

//&= voert een logical  AND uit, dus als er iets aan is dan blijft dat ook staan
//&= ~ voert een  logical NOT uit en daarna &=, dus als er niets aan is dan blijft niets staan
//|= voert een logical OR uit , dus als er iets aan is of er niets aan is dan blijft wat er al was staan

void enableLed(int ledje){
    DDRB  |= (PB2 + ledje);
}

void enableLeds(uint8_t leds){
    DDRB |= (leds << PB2);
}

void enableAllLeds(){
   enableLeds(0b00001111); //method hergebruiken voor duidelijkere code
}

void lightUpLed(int ledje){
    PORTB &= ~(PB2 + ledje); //PB2 gebruiken als de offset voor de juiste ledje aan de hand van de int
}

void lightUpLeds(uint8_t leds) {
    PORTB &= (~leds << PB2);
}

void lightUpAllLeds(){
    lightUpLeds(0b00001111);
}

void lightDownLed(int ledje){
    PORTB |= (PB2 + ledje);
}

void lightDownLeds(uint8_t leds){
    PORTB |= (leds << PB2);
}

void lightDownAllLeds(){ 
    PORTB |= 0xFF; //binaire waarde allemaal op nul geen inputs dus
}

void emoteStartUpAnimatie(){
    _delay_ms(1000);
    lightDownLed(0b00100001);
    _delay_ms(1000);
    lightDownLed(0b00010001);
    _delay_ms(1000);
    lightDownLed(0b00001001);
    _delay_ms(1000);
    lightDownLed(0b00000101);
    _delay_ms(1000);
    lightUpAllLeds();
    _delay_ms(500);
    lightDownAllLeds();
}
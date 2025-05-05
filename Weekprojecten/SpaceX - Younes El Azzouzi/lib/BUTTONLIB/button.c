#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#include "button.h"

void enableButton(int button){
    DDRC &= ~_BV(PC1 + button); //pin van knop als input activeren
    PORTC |= _BV(PC1 + button); //pull-up resistor on
}

void enableAllButtons(){
    enableButton(0);
    enableButton(1);
    enableButton(2);
}

int buttonPushed(int button){
    if (bit_is_clear(PINC, (PC1 + button))) { //is het 0
        return 1; //knop is ingedrukt
    }
    return 0;
}

int buttonReleased(int button) {
    return bit_is_set(PINC, PC1 + button); //is het 1
}

void enableButtonInterrupt(int button){
    PCICR |= (1<<PCIE1);
    PCMSK1 |= (1<<(PC1 + button));
    sei();
}

void enableAllButtonInterrupts(){ //zelfde werkwijze als enableButtonInterrupt(int button)
    PCICR |= (1 << PCIE1); //enable interrupts op poort C (cursus)
    PCMSK1 |= (1 << PC1) | (1 << PC2) | (1 << PC3); //bits voor PC.. toevoegen voor change interrupts in te schakelen
    sei(); //global interrupt add zoals main.c
}

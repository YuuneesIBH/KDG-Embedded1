#include <avr/io.h>

void enableButton (int a){
    if (a < 0 || a > 2) return;
    DDRC &= ~(PC1 + a);  // Make the selected pin an input
    PORTC |= 1<<(PC1+a); //pull-up resistor
}

void enableAllButtons(){
    DDRC &=~0b00001110;
    PORTC |=0b00001110;
}

int buttonPushed(int button){
    if (bit_is_clear(PINC(PC1+button))) {
        return 1;
    } else { return 0; }
}

int buttonReleased(int button){
    !buttonPushed(button);
}
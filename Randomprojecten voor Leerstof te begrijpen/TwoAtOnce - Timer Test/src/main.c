#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
//eigen libraries
#include <display.h>
#include <led.h>
#include <usart.h>
#include <timer.h>
#include <button.h>
#include <buzzer.h>
#include <adc.h>

#define KORT_DELAY 250
#define MAX_LEVENS 3
#define DELAY_BIRD_MOVE 1000
#define DELAY_BUTTON_PRESS 100

volatile uint16_t milliseconden = 0;
volatile bool updateFlag = false;

volatile bool buttonPressed = false;
volatile bool goUpButton = false;
volatile bool goDownButton = false;
volatile bool startSpel = false;
volatile char birdPosition = 'B';
char displayChars[3] = {'A', 'B', 'C'};
int gameSnelheid = 0;

ISR(PCINT1_vect){
    // Debounce delay
    _delay_ms(50);
    
    // Lees knoppen
    goUpButton = buttonPushed(0);
    goDownButton = buttonPushed(2);
}

ISR(TIMER2_COMPA_vect) {
    milliseconden++;
    if (milliseconden >= 500) {
        milliseconden = 0;
        updateFlag = true;
    }
}

void initTimer2() {
    // Set Timer2 in CTC mode (Clear Timer on Compare Match) with prescaler 64
    TCCR2A |= (1 << WGM21);
    TCCR2B |= (1 << CS22) | (1 << CS21);
    // Set compare match value for ~1ms interrupt at 250 (16MHz / 64 / 1000Hz = 250)
    OCR2A = 249;
    // Enable compare match interrupt
    TIMSK2 |= (1 << OCIE2A);
}

char changeBirdPosition(char vogel) {
    if (goUpButton) {
        if (vogel == 'B') {
            vogel = 'A';
        } else if (vogel == 'C') {
            vogel = 'B';
        }
        goUpButton = false; 
    } else if (goDownButton) {
        if (vogel == 'B') {
            vogel = 'C';
        } else if (vogel == 'A') {
            vogel = 'B';
        }
        goDownButton = false;
    } else if(buttonReleased(0)) {
        if (vogel == 'A') {
            vogel = 'B';
        }
    } else if (buttonReleased(3)) {
        if (vogel == 'C') {
            vogel = 'B';
        }
    }
    return vogel;
}

char generateRandomChar(){
  int randomIndex = rand() % 3;
  return "ABC"[randomIndex];
}

void updateMap(){
    displayChars[0] = displayChars[1];
    displayChars[1] = displayChars[2];
    displayChars[2] = generateRandomChar();
}

void displayMap() {
    writeCharsToSegment2(displayChars);
}

bool checkBirdHit(){
    return displayChars[0] == birdPosition;
}

int main(){
    setup(); // Initialize hardware
    while (1)
    {
        if (updateFlag) {
            updateMap();
            updateFlag = false; // Reset update flag
        }
        birdPosition = changeBirdPosition(birdPosition);
        if (checkBirdHit()){
            printf("HIT!");
        }
        
        displayBird(birdPosition);
        displayMap(); // Continuously display the map
    }
}

void setup(){
    initDisplay();
    initADC();
    initUSART();
    initTimer2();
    enableAllButtons();
    enableAllLeds();
    sei(); // Enable global interrupts
}

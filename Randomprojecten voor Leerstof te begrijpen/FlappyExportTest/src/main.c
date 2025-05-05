#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <display.h>
#include <button.h>

#define DELAY_BIRD_MOVE 1000 // Tijd in milliseconden tussen elke beweging van de vogel
#define DELAY_BUTTON_PRESS 100 // Tijd in milliseconden binnen welke de knop moet worden ingedrukt om de vogel te laten stijgen

volatile int goUpButton = false;
volatile int goDownButton = false;
volatile char birdPosition = 'B';
char displayChars[3] = {'A','B','C'};

char generateRandomChar(){
  int randomIndex = rand() % 3;
  return "ABC"[randomIndex];
}

void displayAllChars() {
    writeCharToSegment(1, displayChars[0]);
    writeCharToSegment(2, displayChars[1]);
    writeCharToSegment(3, displayChars[2]);
}

void updateDisplayChars(){
  displayChars[0] = displayChars[1];
  displayChars[1] = displayChars[2];
  displayChars[2] = generateRandomChar();
  _delay_ms(1000);
  displayAllChars();
}



ISR(PCINT1_vect) {
    goUpButton = buttonPushed(0);
    goDownButton = buttonPushed(2);
}

char changeBirdPosition(char vogel) {
    if (goUpButton){
        if (vogel == 'B'){
            vogel = 'A';
            _delay_ms(1000);
        } else if (vogel == 'C'){
            vogel = 'B';
            _delay_ms(1000);
        }
        goUpButton = false; 
    } else if (goDownButton){
      if (vogel == 'B'){
            vogel = 'C';
            _delay_ms(1000);
        } else if (vogel == 'A'){
            vogel = 'B';
            _delay_ms(1000);
      }
      goDownButton = false;
    } else if(buttonReleased(0)){
        if (vogel == 'A'){
          vogel = 'B';
          _delay_ms(1000);
        } 
      else if (buttonReleased(2)){
        if (vogel == 'C'){
          vogel = 'B';
          _delay_ms(1000);
        }
      }
    }
    return vogel;
}

int main() {
    setup();
    while (1) {
        //birdPosition = changeBirdPosition(birdPosition);
        // Update de display met de nieuwe positie van de vogel
        //displayBird(birdPosition);

        updateDisplayChars();
        _delay_ms(1000);
        //displayAllChars();
    }
}

void setup() {
    initDisplay();
    enableAllButtons();
    sei(); // Enable interrupts
}
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "led.h"
#include "button.h"
#include "usart.h"
#include <stdlib.h>
#include "display.h"

// globale variabelen
// plaats hier je globale variabelen
 
volatile int counter;
volatile int minuten;
volatile int sec;
#define VEELVOUD 1000

void initTimer() {
    //vul aan
    TCCR2A |= (1<<WGM21) | (1<<WGM20);
    TIMSK2 |= (1<<OCIE2A);
    sei();
}
 
void startTimer() {
    // vul aan
    TCCR2B |= (1<<CS22); //prescaler is 256 elke 16 microseconden
    OCR2A = 249; //na 250 tellen zitten we op 250 x 16 microseconden
}
 
void stopTimer() {
    // vul aan
    //prescaler op 0 zetten
    TCCR2B = 0;
}
 
void tick() {
    // vul aan
    if (sec >= 60){
      sec = 0;
      minuten++;
    }
    
    printf("tijd %d minuten %d seconden \n", minuten, sec);
}

ISR(PCINT1_vect){
  if (buttonPushed(0)){
    printf("Timer start!");
    startTimer();
  }
  else if (buttonPushed(1)){
    stopTimer();
  }
  else if (buttonPushed(2)){
    printf("TIMER RESET!");
    minuten = 0;
    sec = 0;
  }

}
 
void writeTimeAndWait(uint8_t minuten, uint8_t seconden, int delay) {
    // Vul aan
}
 
 
// Deze ISR loopt elke 4ms
ISR(TIMER2_COMPA_vect) {
  counter++;
  if (counter = VEELVOUD){
    sec++;
    counter = 0;
    printf("Seconden timer %d: ", sec);
    tick();
  }
}

void interruptButton(){
  PCICR |= (1<<PCIE1);
  PCMSK1 |= (1<<PC1) | (1<<PC2) | (1<<PC3);

}
 
int main() {
    initUSART();
    initDisplay();
    initTimer();
    interruptButton();
 
    printf("Start de stopwatch met knop1, stop met knop2, en reset met knop3\n");
    //startTimer();
 
    while (1) {
        // vergeet je timer niet te initialiseren en te starten
        // denk ook aan sei()
    }
    return 0;
}
#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <display.h>

#include <led.h>
#include <usart.h>

int timer1_compare_match;

ISR(TIMER1_COMPA_vect){
  //eens een tick plaats geeft gevonden waarde terug naar 0
  TCNT1 = timer1_compare_match;
  lightUpAllLeds();
}

int main(){
  enableAllLeds();
  
  //timer1 initialiseren
  TCCR1A = 0;
  TCCR1B = 0;

  //timer1_compare_match correcte compare match register toewijzen
  //256 prescaler & 31246 compare match
  timer1_compare_match = 31246;

  //preload timer met compare match val
  TCNT1 = timer1_compare_match;

  //prescaler setten naar 256
  TCCR1B |= (1<<CS12);

  //timer interrupt enable
  TIMSK1 |= (1<<OCIE1A);

  sei();
}
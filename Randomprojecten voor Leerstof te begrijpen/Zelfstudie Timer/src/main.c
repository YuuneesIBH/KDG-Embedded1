#include <Arduino.h>
#include <usart.h>
/*Uitleg
WGM13 WGM12 WGM11 WGM10 bits op 0 voor Normal Mode
Die kan je vinden in de TCCR1A n TCCR1B

CS10 is verantwoordelijk voor de prescaler.
*/

void setup(){
  TCCR1A = 0; //Timer1 resetten naar 0
  TCCR1B = 0; //Timer1 resetten naar 0
  
  TCCR1B |= (1<<CS10); //prescaler 1
  TIMSK1 |= (1<<TOIE1); //enable timer overflow*/
}

ISR(TIMER1_OVF_vect){
  printf(millis());
}

int main(){
  initUSART();
  sei();
  setup();
}
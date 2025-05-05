#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> // voor sei() en cli()
#include <stdbool.h>
#include <button.h>
#include <usart.h>
#include <led.h>

#define BUTTON1 PC1
#define LED1 PB2
#define LED2 PB3
#define DEBOUNCE 50

ISR(PCINT1_vect){
    if (bit_is_clear(PINC, BUTTON1)){ //knop 1 ingedrukt??
      if (bit_is_set(PORTB, LED2)) // als led 2 uit is
      {
        PORTB &= ~_BV(1<<LED2); //zet led  2 aan
      } else {  
        PORTB |= _BV(LED2);
      }
    }
  }

int main(){
  //testCursusInterrupt();

  DDRB |= _BV(LED1) | _BV(LED2);
  PORTB |= _BV(LED1) | _BV(LED2);

  DDRC &= ~_BV(BUTTON1);
  PORTC |= _BV(BUTTON1);
  PCICR |= _BV(PCIE1);

  PCMSK1 |= _BV(BUTTON1);
  
  sei(); //set interrupt bit, bit in een intern register zetten zodat controle op interrupts
  
  while (1)
  {
    PORTB &= ~(_BV(LED1)); //led  1 aanzetten
    _delay_ms(DEBOUNCE);
    PORTB |= _BV(LED1);
    _delay_ms(DEBOUNCE);
  }
  

  initUSART();
  //enableLed(0b00010001);
  //enableButton(0);



  //while (1){
    //if (buttonPushed(2)){
    //_delay_ms(DEBOUNCE);
    //lightDownAllLeds();

    //if (buttonPushed(0)){
      //printf("De eerste knop is ingedrukt!");
      //break;
    //} else if (buttonReleased(0)){
      //printf("De eerste knop is LOSGELATEN!");
    //}
    
  //}
  cli(); //clear interrupt bit
  
}
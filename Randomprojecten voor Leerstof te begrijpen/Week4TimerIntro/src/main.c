#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "led.h"
#include "button.h"
#include "usart.h"
#include <stdlib.h>
#include "display.h"
 
ISR(TIMER0_COMPA_vect){
  lightUpAllLeds();
}
ISR(TIMER0_OVF_vect){
 lightDownAllLeds();
}
 
int main(){
  initDisplay();
  initUSART();
  enableAllLeds();
  
  sei();
  while (1){
      TCCR0A   |= (1<<WGM00)  | (1<< WGM01); 
      TCCR0B |= (1<<CS00) | (1<< CS02); 

      for (size_t i=0; i<30;i++){ 
        printf("TCNT0 %d\n",TCNT0);
      }
      
      //TIMSK -> Timer/COunter interupt Mask Register
      TIMSK0 |= (1 << OCIE0A); //OCRA interupt enable
      TIMSK0 |= (1 << TOIE0); //overflow (op255) enable

      printf("\nGeef de OCROA waarde (tussen 0 en 255)");
      OCR0A= getNumber();
  }
  return 0;
}


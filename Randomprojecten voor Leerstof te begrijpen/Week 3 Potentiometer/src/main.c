#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <display.h>

#include <led.h>
#include <usart.h>

void initADC(){
  ADMUX |= (1<<ADC1D); //tf this?
  ADMUX = (1 << REFS0); //instelle van de voltage
  ADCSRA  |= (1<<ADEN); //enablen van ADC
  ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2); //setten van de clockfrequentie
}

int main() {
  int adcValue;

  initDisplay();
  initUSART();
  initADC();

  while (1){
      //ADCSRA |= (1<<ADSC);
      //loop_until_bit_is_clear(ADCSRA, ADSC);
      //adcValue = ADC;
      //printf("ADCVal is: %d",adcValue);
      //writeNumber(adcValue); //enkel de laatste komt er te staan
      //writeNumberAndWait(adcValue,1000);
      //_delay_ms(1000)

      for (int a = 0; a < 10; a++){
        int b = a + 1;
        int c = a + 2;
        int d = a + 3;
        writeNumberToSegment(a, 1);
        _delay_ms(500);
      }


  }
  return 0;
  
}
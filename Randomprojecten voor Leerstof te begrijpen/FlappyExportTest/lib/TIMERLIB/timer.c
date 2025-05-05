#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "led.h"
#include "button.h"
#include "usart.h"
#include <stdlib.h>
#include "display.h"

void initTimer2() {
  TCCR2A |= (1 << WGM21);  
  OCR2A = 156;              
  TIMSK2 |= (1 << OCIE2A);  
  TCCR2B |= (1 << CS22);
}
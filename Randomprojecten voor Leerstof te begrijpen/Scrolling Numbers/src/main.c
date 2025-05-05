#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <display.h>

int main(){
  initDisplay();

  while (1)
  {
    for (int i = 1; i < 10; i++){
      int d = i;
      int h = (i + 1)%10;
      int t = (i + 2)%10;
      int e = (i + 3)%10;

      int getal = d*1000 + h*100 + t * 10 + e;
      writeNumberAndWait(getal, 1000); // display the number
    }
  }
  return 0;
  
}
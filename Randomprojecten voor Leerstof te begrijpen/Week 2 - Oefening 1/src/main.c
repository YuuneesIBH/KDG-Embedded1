#include <Arduino.h>
#include <avr/io.h>

#include <usart.h>
#include <led.h>

int main (){

  initUSART();
  DDRC &=~(1<<PC1);  // knop 0 Data Direction Register op 0 voor PC1 (knop 0)
  printf("DDRC");

  PORTC |= (1<<PC1); // pull up request 
  printBinaryByte(PORTC);

  while (1){
    printBinaryByte(PINC);
    printf("\n");

    //knop indrukken detecteren
    if (PINC & (1<<PC1)==0)
    {
      printf("Je hebt knop 0 ingedrukt!");
      enableAllLeds();
    } else {
      printf("Knop 0 is NIET ingedrukt!");
    }
    _delay_ms(1000);
  }
  return 0;
}
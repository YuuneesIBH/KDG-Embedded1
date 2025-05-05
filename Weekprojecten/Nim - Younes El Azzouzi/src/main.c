#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <display.h>

#include <led.h>
#include <usart.h>
#include <button.h>
#include <display.h>
#include <adc.h>

#define MAX_AANTAL 21
int lucifers = MAX_AANTAL;
volatile int button_pushed = 0;

typedef struct{
  char speler;
  int lucifersgenomen;
  int lucifersLeft;
} BEURT;

BEURT *beurten;
int aantalBeurten = 0;

void voegBeurtToe(char speler, int genomen, int over) {
    beurten = realloc(beurten, (aantalBeurten + 1) * sizeof(BEURT));
    beurten[aantalBeurten].speler = speler;
    beurten[aantalBeurten].lucifersgenomen = genomen;
    beurten[aantalBeurten].lucifersLeft = over;
    aantalBeurten++;
}

int startUpSpel(){
  printf("Draai nu aan de potentiometer en druk op knop 1 om de waarde vast te zetten!");
  int adcValue;
  while (!button_pushed){
    ADCSRA |= (1<<ADSC);
    loop_until_bit_is_clear(ADCSRA, ADSC);
    adcValue = ADC;
    writeNumberAndWait(adcValue, 1000);
  }
  printf("ADCVal is: %d", adcValue);
  return adcValue;
}

ISR(PCINT1_vect){
  if (!buttonPushed(0)){
    return;
  }
  button_pushed = 1;
}

void startSpel(int startAantal){
  //kiezen wie begint met rand (seed)
  char startPlayer = (rand() % 2 == 0) ? 'P' : 'C';

  //letter tonen en huidige stand lucifers
  while (lucifers > 1) {
    writeNumberAndWait(startAantal, 750);
    writeCharToSegment(0, startPlayer);
    _delay_ms(500);
    writeCharToSegment(2, ' ');
    _delay_ms(500);

    spelLogica(startPlayer);
  }

  ending();
}

void ending(){
  writeNumberAndWait(9999,10000);
    lightUpAllLeds();
    _delay_ms(1500);
    lightDownAllLeds();
    printf("DIT IS HET EINDE VAN HET SPEL ER BLEEF MAAR 1 LUCIFER OVER!");
    printBeurten();
}

void spelLogica(char startPlayer){
  if (startPlayer == 'P'){
      int aantalLucifersNemen = 1;

      while (!buttonPushed(1)){
        writeNumber(aantalLucifersNemen);

        //verhoog en hieronder verlaag het geselecteerde aantal lucifers met knop 3 kan niet hoger dan 3)-
        if (buttonPushed(2)){
          aantalLucifersNemen++;
          if (aantalLucifersNemen > 3)
          {
            aantalLucifersNemen = 3;
          }
        }
        if (buttonPushed(0)){
          aantalLucifersNemen--;
          if (aantalLucifersNemen < 1){
            aantalLucifersNemen = 1;
          }
        }

        //verifieren van afname
        if (buttonPushed(1)){
          voegBeurtToe(startPlayer, aantalLucifersNemen, lucifers);
          lucifers -= aantalLucifersNemen;
          startSpel(lucifers);
        }
        
      }
    }
    else if (startPlayer == 'C'){
      int computerCount = (lucifers - 1) % (MAX_AANTAL + 1);
      if (computerCount < 1 || computerCount > 3){
          computerCount = rand() % 3 + 1;
      }
        
      while (!buttonPushed(1)){
        writeNumber(computerCount);

        //verifieren van afname
        if (buttonPushed(1)){
          voegBeurtToe(startPlayer, computerCount, lucifers);
          lucifers -= computerCount;
          startSpel(lucifers);
        }
        
      }
    }
}

void printBeurten(){
  printf("Aantal beurten %d\n----------------------", aantalBeurten);

  for (int i = 0; i < aantalBeurten; i++){
    printf("\nBeurt %d: Speler %c nam %d lucifers, er zijn er nog %d over\n", i+1, beurten[i].speler, beurten[i].lucifersgenomen, beurten[i].lucifersLeft);
  }
}

int main(){
  initDisplay();
  initUSART();
  initADC();
  enableAllLeds();
  lightDownAllLeds();
  enableAllButtons();
  sei();
  enableAllButtonInterrupts();

  int seed = startUpSpel();
  srand(seed);

  startSpel(MAX_AANTAL);
  free(beurten);
  return 0;
}
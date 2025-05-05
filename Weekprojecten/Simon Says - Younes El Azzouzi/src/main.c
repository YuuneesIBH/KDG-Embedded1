#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include <button.h>
#include <led.h>
#include <usart.h>

volatile int button_pushed = 0;
volatile int teller = 0;

#define DELAY 500

void knipperAnimatie(){
  while (!button_pushed){ //na de interrupt detectie uit de while breken
    lightUpLed(4);
    _delay_ms(1000);
    lightDownLed(4);
    _delay_ms(1000);
    teller++; //waarde ++ naargelang de tijd verstrijkt
  }
}

ISR(PCINT1_vect){
  button_pushed = 1; //interupt detected? waarde op 1 zetten
}

void testRandom(){
  for (int i = 0; i < 10; i++) {
    int random_number = rand() % 10;
    printf("%d\n ", random_number);
  }
}

void generatePuzzle(uint8_t puzzelArr[], uint8_t length){
  for(int i = 0; i < length; i++){
    puzzelArr[i] = rand() % 3;
  }
}

void printPuzzle(uint8_t puzzelArr[], uint8_t length){
  printf("[ ");
  for (int i = 0; i < length; i++) {
    printf("%d ", puzzelArr[i]);
    
  }
  printf(" ]\n");
}

void playPuzzle(uint8_t puzzelArray[], int length){
  for (int i = 0; i < length; i++){
      switch (puzzelArray[i])
      {
        case 0:
          lightUpLed(33);
          _delay_ms(DELAY);
          lightDownLed(33);
          _delay_ms(DELAY);
          break;
        case 1:
          lightUpLed(17);
          _delay_ms(DELAY);
          lightDownLed(17);
          _delay_ms(DELAY);
          break;
        case 2:
          lightUpLed(9);
          _delay_ms(DELAY);
          lightDownLed(9);
          _delay_ms(DELAY);
          break;
        default:
          break;
      }
  }
}
 void readInput(uint8_t puzzel[], int lengte){
  int limietCounter = 0;

  while (limietCounter < lengte){ //voor het weten wanneer hele patroon finished is
    int button;
    for (button = 0; button < 3; button++){ //knoppen preparen
      if (buttonPushed(button)){
          korteLightUp(button); //fancy oplicht methode ;)

          if (button == puzzel[limietCounter]){
            printf("Je hebt op knop %d gedrukt. CORRECT! \n", button+1);
            limietCounter++;
          } else {
            printf("Je drukte op knop %d, FOUTIEF!\n", button + 1);
            return 0;
          }
      }
    }
  }
  printf("Je hebt het patroon juist gespeeld! Dit was het einde van het programma!");
 }

 void korteLightUp(int button){
  if (button = 1){
    lightUpLed(33);
    _delay_ms(DELAY);
    lightDownLed(33);
  } else if (button = 2){
    lightUpLed(17);
    _delay_ms(DELAY);
    lightDownLed(17);
  } else if (button = 3){
    lightUpLed(9);
    _delay_ms(DELAY);
    lightDownLed(9);
  }
 }

int main(){
  //INITIALISATIE van alles
  initUSART();
  enableAllLeds();
  lightDownAllLeds();
  enableAllButtons();

  sei();
  enableAllButtonInterrupts();

  knipperAnimatie(); //blijven knipperen tot indruk
  srand(teller); //random instellen met onze teller van knipperAnimatie
  testRandom();

  uint8_t puzzel[10];

  generatePuzzle(puzzel, 10);
  printPuzzle(puzzel, 10);
  playPuzzle(puzzel, 10);

  printf("Speel nu het patroon na door gebruik te maken van de knoppen.");
  readInput(puzzel, 10);
}
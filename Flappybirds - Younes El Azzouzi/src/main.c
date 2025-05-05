#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>
#include <display.h> //eigen libraries
#include <led.h>
#include <usart.h>
#include <timer.h>
#include <button.h>
#include <buzzer.h>
#include <adc.h>

#define  C5      523.250
#define  D5      587.330
#define  E5      659.250
#define  A5      880.000
#define  B5      987.770
#define  C6      1046.500
#define KORT_DELAY 250

volatile uint16_t milliseconden = 0; //2 bytes only
volatile uint16_t seedTimer = 0;
volatile uint16_t immuneTimer = 0;
volatile bool updateFlag = false;
volatile bool isPaused = false;
volatile bool buttonPressed = false;
volatile bool goUpButton = false;
volatile bool goDownButton = false;
volatile bool startSpel = false;
volatile char birdPosition = 'B'; //default vogel start
char displayChars[3] = {'A', 'B', 'C'}; //map plekken 
int gameSnelheid = 0;
int MAX_LEVENS = 3;

//--------------STRUCT--------------
typedef struct{
  char player;
  int livesCount;
  char birdPlacement;
} GAME;

GAME *gameInfo;
int aantalInfo = 0;

void voegInformatieToe(char speler, int levens, char plaats) {
    GAME *tempGame = (GAME *)malloc(sizeof(GAME));
    if (tempGame == NULL) {
        printf("Geheugen kon niet worden toegewezen.\n");
        return;
    }
    
    tempGame->player = speler;
    tempGame->livesCount = levens;
    tempGame->birdPlacement = plaats;
    printf("\nBeurt: Speler %c had %d leven(s) en de vogel was op positie: %c\n", tempGame->player, tempGame->livesCount, tempGame->birdPlacement); //print info af
    //direct vrijgeven --> gaf issues
    //als u een progamer als mij bent gebruikte je alle storage van de Uno waardoor applicatie vastliep. daarom free
    free(tempGame);
}

void printBeurten(){
  for (int i = 0; i < aantalInfo; i++){
    printf("\nBeurt %d: Speler %c  had %d leven(s), er de vogel was op positie: %c\n", i+1, gameInfo[i].player, gameInfo[i].livesCount, gameInfo[i].birdPlacement);
  }
}
//--------------STARTUP--------------
void fancyLichtShow(){
  int randomLicht = rand();
  lightUpLed(randomLicht);
  _delay_ms(KORT_DELAY);
  lightDownLed(randomLicht);
  enableBuzzer();
  int randomIndex = rand() % 7;
  switch (randomIndex) {
    case 0:
      playTone(E5, KORT_DELAY);
      break;
    case 1:
      playTone(D5, KORT_DELAY);
      break;
    case 2:
      playTone(C5, KORT_DELAY);
      break;
    case 3:
      playTone(D5, KORT_DELAY);
      break;
    case 4:
      playTone(E5, KORT_DELAY);
      break;
    case 5:
      playTone(E5, KORT_DELAY);
      break;
    case 6:
      playTone(D5, KORT_DELAY);
      break;
  }
}

int showUserADCInfo(){
    ADCSRA |= (1<<ADSC);
    loop_until_bit_is_clear(ADCSRA, ADSC);
    int adcValue = ADC; //adc val ophalen
    adcValue %= 10; //eenheden displayen

    writeNumberAndWait(adcValue, KORT_DELAY);

    return gameSnelheid = adcValue;//voor snelheid van de game te bepalen
}

ISR(PCINT1_vect){
  _delay_ms(50);
  buttonPressed = buttonPushed(0) || buttonPushed(1) || buttonPushed(2);
  //gebruiker buttons inlegen wil je omhoog/omlaag?
  goUpButton = buttonPushed(0); 
  goDownButton = buttonPushed(2);
}

ISR(TIMER2_COMPA_vect) {
    milliseconden++; //timer triggert de update
    seedTimer++;
    if (milliseconden >= (1000 - (gameSnelheid * 100))){
        milliseconden = 0;
        updateFlag = true;
    }
}

//--------------VOGELGAMELOGICA--------------
char changeBirdPosition(char *vogel) {
    if (goUpButton) {
        if (*vogel == 'B') {
            *vogel = 'A';
        } else if (*vogel == 'C') {
            *vogel = 'B';
        }
        goUpButton = false; 
    } else if (goDownButton) {
        if (*vogel == 'B') {
            *vogel = 'C';
        } else if (*vogel == 'A') {
            *vogel = 'B';
        }
        goDownButton = false;
    } else if(buttonReleased(0)) {
        if (*vogel == 'A') {
            *vogel = 'B';
        }
    } else if (buttonReleased(3)) {
        if (*vogel == 'C') {
            *vogel = 'B';
        }
    }
    return *vogel;
}
//--------------GAMELOGICA--------------
char generateRandomChar(){
  srand(seedTimer);
  int randomIndex = rand() % 3;
  return "ABC"[randomIndex];
}

void updateMap(){
    displayChars[0] = displayChars[1];
    displayChars[1] = displayChars[2];
    displayChars[2] = generateRandomChar();
}

void displayMap() {
    writeCharsToSegment2(displayChars);
}

//--------------GAMELEVENLOGICA--------------
void speelDeathAf(){
  enableBuzzer();
  playTone(C6, KORT_DELAY);
  _delay_ms(KORT_DELAY);
  playTone(B5, KORT_DELAY);
  _delay_ms(KORT_DELAY);
  playTone(A5, KORT_DELAY);
  _delay_ms(KORT_DELAY);
  disableBuzzer();
}

void printDEAD(){
  writeCharToSegment(0, 'D');
  writeCharToSegment(1, 'I');
  writeCharToSegment(2, 'E');
  writeCharToSegment(3, 'D');
}

void displayLives(int lives){
  lightDownAllLeds();
  if (lives >= 3){
    lightUpLed(17);
    lightUpLed(9);
    lightUpLed(2);
  } else if (lives == 2){
    lightUpLed(9);
    lightUpLed(2);
  } else if (lives == 1){
    lightUpLed(2);
  } else{
    lightDownAllLeds();
    printDEAD();
    speelDeathAf(); //dzd?
    startSpel = false;
    printf("Je hebt geen levens meer je bent dood!");
  }
}
//--------------GAMELOGICA--------------
void updateImmunity() {
    if (immuneTimer > 0) {
        immuneTimer--;
    }
}

bool isPlayerImmune() {
    return immuneTimer > 0;
}

void setPlayerImmune() {
    immuneTimer = 100;
}

bool checkBirdHit(){
    if (!isPlayerImmune() && displayChars[0] == birdPosition) {
        setPlayerImmune(); //speler op imuun zetten na een hit
        MAX_LEVENS--;
        return true;
    }
    return false;
}
//--------------MAIN + SETUP--------------

int main(){
  setup();
  while (!buttonPressed){
    fancyLichtShow();
  }
  printf("Druk op één van de knoppen om de gamesnelheid te bevestigen!");
  buttonPressed = false;
  while (!buttonPressed){
    showUserADCInfo();
  }
  printf("\nGekozen spelsnelheid -> %d. Het spel binnen 5 seconden!\nGebruik knop 1 om omhoog en knop 3 om omlaag te gaan! Pauzeren? knop 2! Have fun!\n", gameSnelheid);
  startSpel = true;
  _delay_ms(5000);
  while (startSpel){
    if (buttonPushed(1)) {
      //toggle waarde van isPaused
      isPaused = !isPaused;
      _delay_ms(100); //debounce delay
    }

    if (isPaused){
      printf("Gepauzeerd! Druk op knop 2 om verder te gaan!");
      continue;
    }
    
    if (updateFlag) {
      //timer triggert het hele update process
        voegInformatieToe("Player 1", MAX_LEVENS, birdPosition);
        updateMap();
        updateImmunity();
        updateFlag = false;//update? op false zetten voor volgende interrupt 
    }
    birdPosition = changeBirdPosition(&birdPosition);//vogelpositie aanpassen elke keer als de map aanpast
    updateImmunity(); //immuniteit
    if (checkBirdHit()) {
        printf("\nJe hebt een buis geraakt! Je bent imuun voor een korte periode!\n");
    }
  
    displayLives(MAX_LEVENS);
    displayBird(birdPosition);
    displayMap(); //blijf constant map showen met vogel
  }
  endGameScreen();
}

void endGameScreen(){
  printf("\nJe hebt het spel verloren!");
  printBeurten();
  free(gameInfo);
}

void setup(){
  printf("\nFlap flap flap dit is Flappybirds! Druk op één van de knoppen om verder te gaan!\n");
  initDisplay();
  initADC();
  initUSART();
  initTimer2();
  enableAllButtons();
  enableAllLeds();
  sei();
}
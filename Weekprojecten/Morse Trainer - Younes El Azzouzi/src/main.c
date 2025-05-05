#include <avr/io.h>
#include <led.h>
#include <usart.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

//array met de waarden van de morse codes
//array onbepaalde lengte zodat er nog cijfers toegevoegd kunnen worden
const char* morseArray[] = {
    //letters
    ".-",    // A
    "-...",  // B
    "-.-.",  // C
    "-..",   // D
    ".",     // E
    "..-.",  // F
    "--.",   // G
    "....",  // H
    "..",    // I
    ".---",  // J
    "-.-",   // K
    ".-..",  // L
    "--",    // M
    "-.",    // N
    "---",   // O
    ".--.",  // P
    "--.-",  // Q
    ".-.",   // R
    "...",   // S
    "-",     // T
    "..-",   // U
    "...-",  // V
    ".--",   // W
    "-..-",  // X
    "-.--",  // Y
    "--..",  // Z
    //cijfers
    "-----",  // 0
    ".----",  // 1
    "..---",  // 2
    "...--",  // 3
    "....-",  // 4
    ".....",  // 5
    "-....",  // 6
    "--...",  // 7
    "---..",  // 8
    "----."   // 9
};

void startEmote(){
    emoteStartUpAnimatie();
}

void toonEenMinteken(){
    lightUpAllLeds();
    _delay_ms(1000);
    lightDownAllLeds();
    _delay_ms(500);
}

void toonEenPunt(){
    lightUpAllLeds();
    _delay_ms(300);
    lightDownAllLeds();
    _delay_ms(300);
}

void loopDoorDeMorse(){
    int index = rand() % 36;
    const char* morse = morseArray[index];
    printf("CHEAT: de random gekozen waarde is: %s\n", morse);

    while (*morse)
    {
        switch (*morse++)
        {
        case '.':
            toonEenPunt();
            break;
        case '-':
            toonEenMinteken();
            break;
        }
    }
    
}

int main(){
    initUSART();
    //huidige tijd als default SEED gebruiken voor altijd random waarden
    //info gevonden StackOverflow
    srand(time(NULL));
    //random generator initialiseren 

    enableAllLeds();
    printf("4.. 3.. 2.. 1.. Laten we beginnen!");
    startEmote();
    _delay_ms(9000);
    printf("De morse trainer begint nu!");

    for (int i = 0; i < 3; i++){
        _delay_ms(5000);
        printf("Dit is morse try %s\n", i);
        loopDoorDeMorse();
    }

    //einde van de applicatie
    printf("Dit was de applicatie!");
    return 0;
}
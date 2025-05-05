#include <util/delay.h>
#include <avr/io.h>
#include <led.h>

int main(){ 
    enableLed(33);
    //_delay_ms(10000);
    //enableLeds(0b00001011); //binair getal doorgeven
    //enableAllLeds();
    _delay_ms(500);
    lightUpLed(33);
    //lightUpLeds(0b00001011);
    //lightUpAllLeds();
    //_delay_ms(5000);
    //lightDownAllLeds();
}
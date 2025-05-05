#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> // voor sei() en cli()
#include <stdbool.h>
#include <button.h>
#include <usart.h>
#include <led.h>

#define LED_PORT PORTB
#define BUTTON_PORT PORTC
#define BUTTON_PIN PINC
#define LED_DDR DDRB
#define BUTTON_DDR DDRC

//knoppen
#define BUTTON1 PC1
#define BUTTON2 PC2
#define BUTTON3 PC3
//LEDS
#define LED1 PB2
#define LED2 PB3
#define LED3 PB4
#define LED4 PB5

int main(){
    sei();
    enableAllLeds();
    lightDownAllLeds();
    enableButton(0);
    enableButton(1);
    enableButton(2);

    enableAllButtonInterrupts();

    while (1)
    {
        if (buttonPushed(BUTTON1))
        {
            lightUpLed(LED1);
        } else if (buttonPushed(BUTTON2))
        {
            lightUpLed(LED2);
        } else if (buttonPushed(BUTTON3))
        {
            lightUpLed(LED3);
        }
        
        
    }
    
    
}

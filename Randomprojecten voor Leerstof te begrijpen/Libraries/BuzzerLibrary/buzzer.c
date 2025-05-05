#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/io.h>
#include <util/delay.h>

void enableBuzzer() {
    DDRD |= _BV(PD3);
}

void playTone(float frequency, uint32_t duration) {
    uint32_t periodInMicro = (uint32_t)(1000000 / frequency);                 // we berekenen de periode in microseconden uit de frequency
    uint32_t durationInMicro = duration * 1000;                               // we willen duration in microseconden
    for (uint32_t time = 0; time < durationInMicro; time += periodInMicro) {  // Zie tutorial over muziek voor meer uitleg!
        PORTD &= ~_BV(PD3);                                                   // aanzetten van de buzzer
        _delay_us(periodInMicro / 2);                                         // helft van de periode wachten
        PORTD |= _BV(PD3);                                                    // afzetten van de buzzer
        _delay_us(periodInMicro / 2);                                         // weer helft van de periode wachten
    }
}

void turnOffBuzzer() {
	PORTD |= _BV(PD3);
}
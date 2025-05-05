#ifndef ledlib_h
#define ledlib_h
#include <stdint.h>

void enableLed(int ledje);
void enableLeds(uint8_t leds);
void enableAllLeds();
void lightUpLed(int ledje);
void lightUpLeds(uint8_t leds);
void lightUpAllLeds();
void lightDownLed(int ledje);
void lightDownLeds(uint8_t leds);
void lightDownAllLeds();

#endif
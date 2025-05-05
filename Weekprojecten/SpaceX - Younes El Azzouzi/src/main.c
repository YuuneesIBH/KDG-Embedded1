#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <display.h>
#include <led.h>
#include <usart.h>
#include <timer.h>
#include <button.h>

int currentSpeed = 100;
float gravity = 1.622;
int distance = 9999;
int fuelReserve = 1500;
int burst = 10;

bool landed = false;
bool crashed = false;

void showParameters(int distance, int fuelReserve) {
    writeNumber(distance);

    for (int i = 0; i < 4; ++i) {
        if (fuelReserve >= (i + 1) * 375) { //1500 / 4 = 375
            enableLed(i);
        } else {
            lightDownLed(i);
        }
    }
}

void calculateNewSituation() {
    currentSpeed += gravity - burst / 5;
    distance -= currentSpeed;
    fuelReserve -= burst;

    if (distance <= 3 && currentSpeed <= 5) {
        landed = true;
    }

    //situatie tonen op display
    showParameters(distance, fuelReserve);

    //checken voor crashed of land
    if (landed || crashed) {
        stopTimer();
    }
}

ISR(PCINT1_vect) {
    if (buttonPushed(0) || buttonPushed(1) || buttonPushed(2)) {
        printf("\nWOOOOOOO BOOOOOOOOOSTTTT\n");
        calculateNewSituation();
    }
}

ISR(TIMER1_COMPA_vect) {
    calculateNewSituation();
}

int main() {
    sei();
    setupAllLibs();

    while (!landed && !crashed) {
        showParameters(distance, fuelReserve);
    }

    if (landed) {
        printf("Geland!");
    } else if (crashed) {
        printf("Gecrasht!");
    }
}

void setupAllLibs() {
    initDisplay();
    initUSART();
    initTimer();
    startTimer();
    enableAllLeds();
    enableAllButtons();
    enableAllButtonInterrupts();
}

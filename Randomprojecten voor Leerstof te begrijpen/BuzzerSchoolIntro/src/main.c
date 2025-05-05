#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

#include <led.h>
#include <usart.h>


typedef struct{
    char straat
    uint8_t nummer;
    int postcode;
} ADRES;

typedef struct {
    //char naam[10];
    char* naam;
    uint8_t punt;
} STUDENT;

void printStudent(STUDENT *s){ //call by reference
    printf("%s: %d/10", (*s).naam, s->punt);

    //(*s).deeltje          s->deeltje --> pointer 
}

STUDENT* newStudent(char *naamN, uint8_t punt){
    STUDENT* this; //ik heb het adres van student
    //HEAP ? malloc of calloc

    this = calloc(1, sizeof(STUDENT));
    this->naam = malloc(strlen(naamN)+1);
    strcpy(this->naam, naamN); //eerst naar adres en dan naar naam
    this->punt = 10;
    return this;
}

ADRES* newAdres(straat, nummer, postcode){
    return this;
}

int main(){

    //DDRD |= (1<<PD3); //enable buzzer
    enableLed(1);
    initUSART();

    STUDENT* st; //ik heb het adres van student
    //HEAP ? malloc of calloc
    st = calloc(1, sizeof(STUDENT)); //zoek voor mij een plek waar een student inpast
    strcpy((*st).naam,"Jef"); //eerst naar adres en dan naar naam
    (*st).punt = 9;
    printStudent(st);
    newStudent("Dounia", 10);
    

    free(st);

    uint32_t periode = (uint32_t)(1000000/880);

    while (1){
        lightUpLed(1);
        PORTD &= ~(1<<PD3);
        _delay_us(periode);
        lightDownLed(1);
        PORTD |= (1<<PD3);
        _delay_us(periode);
    }

    return 0;
    
}
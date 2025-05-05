#include <usart.h>

void increment(int *p){
    printf("Incrementing  %d\n",*p);
    (*p)++;
}

int main(){
    initUSART();

    int a = 10;
    printf(a);
}
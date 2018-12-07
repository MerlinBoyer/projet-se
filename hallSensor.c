#include "hallSensor.h"
#include <avr/io.h>


volatile int HALL_BOOL = 0;

void init_hallsensor(){
    DDRD &= ~(1 << PIN0);
}

void check_hall(){
    if ( PORTD0 ){
        HALL_BOOL = 0;
    } else {
        HALL_BOOL = 1;
    }
}
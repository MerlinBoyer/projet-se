#include "monitor.h"
#include <avr/interrupt.h>
#include "led.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "bluetooth.h"
#include <math.h>
#include "draw.h"

unsigned int last_time = 0;
unsigned char RESET_TCNT = 0; // TCNT0 = 1 register = 8 bits
long long nb_tim_isr = 0; // nb millisecondes depuis dernier passage
int first_int = 1;

ISR (TIMER0_OVF_vect){
  nb_tim_isr++;
  TCNT0 = RESET_TCNT;
}


ISR (INT0_vect){
  if (first_int) {
    first_int = false;
  } else {
    first_int = true;  
    last_time = nb_tim_isr;
    nb_tim_isr = 0;
    TCNT0 = RESET_TCNT;  
  }
}

void init_monitor(){
  nb_tim_isr = 0;
  TCNT0 = RESET_TCNT; 
  TCCR0 = (1 << CS00);
  TIMSK |= (1 << TOIE0) ;   // Enable timer1 overflow interrupt(TOIE0)
  PIND |= (1 << PIND0); // config pin PD0 en entrée
  EICRA |= (1 << ISC00);
  EIMSK |= (1 << INT0); // activate interrupt  
}


int get_current_angle(){
  long one_tour_time = last_time;
  long time_passed = nb_tim_isr;
  return time_passed*360/one_tour_time;
}

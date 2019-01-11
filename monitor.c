#include "monitor.h"
#include <avr/interrupt.h>
#include "led.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "bluetooth.h"
#include <math.h>
#include "draw.h"

unsigned int last_time = 0;       // remember last turn time
unsigned char RESET_TCNT = 0;     // TCNT0 = 1 register = 8 bits
long long nb_tim_isr = 0;         // nb millisecondes since last passage
int first_int = 1;                /* used when Hall sensor trigger interrupt to know if 
                                     it is the first or the second one (arriving or leaving magnet area)   */

/*
  Interrupt to register time elapsed (TIMER0 ticks)
*/
ISR (TIMER0_OVF_vect){
  nb_tim_isr++;
  TCNT0 = RESET_TCNT;
}

/*
  Interrupt when Hall Sensor is triggered (only on escaping magnet)
  reset time elapsed and turn time
*/
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
  PIND |= (1 << PIND0);     // config pin PD0 as entry
  EICRA |= (1 << ISC00);
  EIMSK |= (1 << INT0);     // activate interrupt  
}

/*
  Return the current angle of the board
*/
int get_current_angle(){
  long one_tour_time = last_time;
  long time_passed = nb_tim_isr;
  return time_passed*360/one_tour_time;
}

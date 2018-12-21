#include "monitor.h"
#include <avr/interrupt.h>
#include "led.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "bluetooth.h"
#include "Ffloat.h"
#include <math.h>
#include "draw.h"

#define UPDATE_INTERVAL 1

unsigned int last_time = 0;
unsigned char RESET_TCNT = 0; // TCNT0 = 1 register = 8 bits
const int PERIOD = 1;// = 0.01 * 100
unsigned char NB_INC_INT = 200; // 57 increments for two millisecond
long long nb_tim_isr = 0; // nb millisecondes depuis dernier passage
int updated = 0;
int first_int = 1;

ISR (TIMER0_OVF_vect){
  nb_tim_isr++;
  updated = 1;
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
  //ble_send_str_from_int( (int)last_time );
}

void init_monitor(){
  nb_tim_isr = 0;
  TCNT0 = RESET_TCNT;   // for 1 ms at 13 MHz, to chan
  TCCR0 = (1 << CS00);
  TIMSK |= (1 << TOIE0) ;   // Enable timer1 overflow interrupt(TOIE0)
  PIND |= (1 << PIND0); // config pin PD0 en entrée
  EICRA |= (1 << ISC00);
  EIMSK |= (1 << INT0); // activate interrupt  
}

// radians
int get_current_angle(){
  if (!updated) return -1;
  updated = 0;
  long one_tour_time = last_time;
  long time_passed = nb_tim_isr;
  return ((time_passed*1000)/one_tour_time * 360) / 1000;
}

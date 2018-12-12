#include "monitor.h"
#include <avr/interrupt.h>
#include "led.h"
#include <math.h>
#include <stdbool.h>

unsigned long MAX_SAMPLES = 500000;
unsigned char MAX_TCNT = 255; // TCNT0 = 1 register = 8 bits
double PERIOD = 0.01; // timer déclenché toute les millisecondes
unsigned char NB_INC_INT = 126; // 57 increments for two millisecond

volatile long nb_tim_isr = 0; // nb millisecondes depuis dernier passage
volatile long double avg_speed = 0; // tours / sec
volatile unsigned long nb_samples = 0; // nb samples for average

bool first_int = true;
bool begun = false;
double time_passed_prev;

ISR (TIMER0_OVF_vect){  
  nb_tim_isr++;
  TCNT0 = MAX_TCNT-NB_INC_INT;
}

double compute_time_passed(){
  return (nb_tim_isr + ((TCNT0-MAX_TCNT+NB_INC_INT)/(double)NB_INC_INT)) * PERIOD;
}


ISR (INT0_vect){
  if (first_int) {
    first_int = false;
    return;
  }
  first_int = true;  
  if (begun){
    if (MAX_SAMPLES != nb_samples+1){
    // en secondes
    double time_passed = compute_time_passed();
    // en tours par seconde
    double speed = 1/time_passed;
    // update avg
      avg_speed = (avg_speed * nb_samples + speed) / (nb_samples + 1);
      nb_samples++;
    }
  }
  begun = true;
  nb_tim_isr = 0;
  TCNT0 = MAX_TCNT-NB_INC_INT;  
}

void init_monitor(){
  nb_tim_isr = 0;
  avg_speed = 0;
  nb_samples = 0;
  TCNT0 = MAX_TCNT-NB_INC_INT;   // for 1 ms at 13 MHz, to chan
  TCCR0 |= (1 << CS02) | (1 << CS01) | (1 << CS00);  // Timer mode with 1024 prescler, 12.69 for 1ms
  TIMSK |= (1 << TOIE0) ;   // Enable timer1 overflow interrupt(TOIE0)
  PIND |= (1<<PIND0);
  EICRA |= (1 << ISC00);
  EIMSK |= (1 << INT0);
}

// radians
double get_current_angle(){
  if (nb_samples == 0) return 0;
  double one_tour_time = 1/avg_speed;
  double time_passed = compute_time_passed();
  return (time_passed/one_tour_time * 2*M_PI);
}

//degres
double get_current_angle_degree(){
  if (nb_samples == 0) return 0;
  double one_tour_time = 1/avg_speed;
  double time_passed = compute_time_passed();
  return (time_passed/one_tour_time * 360);
}
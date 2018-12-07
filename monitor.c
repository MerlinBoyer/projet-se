#include "monitor.h"
#include <avr/interrupt.h>
#include "led.h"


#define MAX_NB 4294967295 // max for long
#define MAX_TCNT 255 // TCNT0 = 1 register = 8 bits
#define PERIOD 0.001 // timer déclenché toute les millisecondes
#define PI 3.14
#define NB_INC_INT 13 // 13 increments for one millisecond

volatile long nb_tim_isr; // nb millisecondes depuis dernier passage
volatile double avg_speed = 0; // tours / sec
volatile unsigned long nb_samples = 0; // nb samples for average

ISR (TIMER0_OVF_vect){  
  nb_tim_isr++;
  TCNT0 = MAX_TCNT-NB_INC_INT;
}

double compute_time_passed(){
  return (nb_tim_isr + ((TCNT0-MAX_TCNT+NB_INC_INT)/(double)NB_INC_INT)) * PERIOD;
}

// changer pour la pin du tétecteur à aimant
ISR (INT0_vect){
  return;
  if (MAX_NB == nb_samples) return;
  // en secondes
  double time_passed = compute_time_passed();
  // en tours par seconde
  double speed = 1/time_passed;
  // update avg
  avg_speed = (avg_speed * nb_samples + speed) / (nb_samples + 1);
  nb_samples++;
  nb_tim_isr = 0;
  TCNT0 = MAX_TCNT-NB_INC_INT;
}

void init_monitor(){
  nb_tim_isr = 0;
  avg_speed = 0;
  nb_samples = 0;

  TCNT0 = MAX_TCNT-NB_INC_INT;   // for 1 ms at 13 MHz, to change
    /* to change : bad resolution */
  TCCR0 |= (1 << CS01) | (1 << CS00);  // Timer mode with 1024 prescler, 12.69 for 1ms
  TIMSK |= (1 << TOIE0) ;   // Enable timer1 overflow interrupt(TOIE0)

  DDRD = (1 << PIN0);
  PORTD |= (1 << PIN0);
  EICRB |= (1 << ISC00);
  EIMSK |= (1 << INT0);
}

// radians
double get_current_angle(){
  if (nb_samples == 0) return 0;
  double one_tour_time = 1/avg_speed;
  double time_passed = compute_time_passed();
  return time_passed/one_tour_time * 2 * PI;
}

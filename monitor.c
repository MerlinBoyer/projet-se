#include "monitor.h"
#include <avr/interrupt.h>
#include "led.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "bluetooth.h"

#define NOVAL -1

unsigned char MAX_SAMPLES = 25;
float samples_time[25] = {0};
unsigned char MAX_TCNT = 255; // TCNT0 = 1 register = 8 bits
float PERIOD = 0.01; // timer déclenché toute les millisecondes
unsigned char NB_INC_INT = 126; // 57 increments for two millisecond

volatile int nb_tim_isr = 0; // nb millisecondes depuis dernier passage
volatile float avg_time = 0; // avg time
volatile unsigned char nb_samples = 0; // nb samples for average

bool first_int = true;
bool begun = false;
bool has_noval = true;

ISR (TIMER0_OVF_vect){  
  nb_tim_isr++;
  TCNT0 = MAX_TCNT-NB_INC_INT;
}

float compute_time_passed(){
  return (nb_tim_isr + ((TCNT0-MAX_TCNT+NB_INC_INT)/(float)NB_INC_INT)) * PERIOD;
}


ISR (INT0_vect){
  if (first_int) {
    first_int = false;
    return;
  }
  first_int = true;  
  if (begun){
    if (MAX_SAMPLES <= nb_samples){
      nb_samples = 0;
      has_noval = false;
      char str[50];
      sprintf(str, "%d", (int)(avg_time*1000));
      ble_send_str(str);
    }
    float time_passed = compute_time_passed();
    avg_time += (time_passed - samples_time[nb_samples]) / MAX_SAMPLES;
    samples_time[nb_samples] = time_passed;
    nb_samples++;
  }
  begun = true;
  nb_tim_isr = 0;
  TCNT0 = MAX_TCNT-NB_INC_INT;  
}

void init_monitor(){
  nb_tim_isr = 0;
  avg_time = 0;
  nb_samples = 0;
  TCNT0 = MAX_TCNT-NB_INC_INT;   // for 1 ms at 13 MHz, to chan
  TCCR0 |= (1 << CS02) | (1 << CS01) | (1 << CS00);  // Timer mode with 1024 prescler, 12.69 for 1ms
  TIMSK |= (1 << TOIE0) ;   // Enable timer1 overflow interrupt(TOIE0)
  PIND |= (1<<PIND0); // config pin PD0 en entrée
  EICRA |= (1 << ISC00);
  EIMSK |= (1 << INT0); // activate interrupt
}

// radians
float get_current_angle(){
  if (has_noval) return 0;
  float one_tour_time = avg_time;
  one_tour_time = 0.110;
  float time_passed = compute_time_passed();
  return (time_passed/one_tour_time * 2*M_PI);
}

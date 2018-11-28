#include <avr/io.h>
#include "time.h"
#include <stdio.h>
#include <avr/interrupt.h>

static struct Time m_time;

ISR (TIMER1_OVF_vect){
  m_time.seconds++;
  if (m_time.seconds >= 60){
    m_time.minutes++;
    m_time.seconds = 0;
  }
  if (m_time.minutes >= 60){
    m_time.hours++;
    m_time.minutes = 0;
  }
  if (m_time.hours >= 24){
    m_time.hours = 0;
  }
}

void init_time(struct Time t){
  m_time = t;
  TCCR1A = 0x00;
  TIFR0 |= 0x04; // clr TOV1 with 1
  TCCR1B = (1 << CS10) | (1 << CS12);  // Timer mode with 1024 prescler
  TCCR1B |= (1 << WGM12); //RAZ timer quand comparaison
  OCR1A = 12695;
  TIMSK = (1 << TOIE1) ;   // Enable timer1 overflow interrupt(TOIE1)
}


struct Time get_time(){
  return m_time;
}

void get_time_str(char * out){
  struct Time t = get_time();
  sprintf(out, "%d:%d:%d\r", t.hours, t.minutes, t.seconds);
}

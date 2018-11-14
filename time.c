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
  TCNT1 = 63974;   // for 1 sec at 16 MHz
}

void init_time(struct Time t){
  m_time = t;
  TCNT1 = 63974;   // for 1 sec at 16 MHz	
  TCCR1A = 0x00;
  TCCR1B = (1<<CS10) | (1<<CS12);;  // Timer mode with 1024 prescler
  TIMSK1 = (1 << TOIE1) ;   // Enable timer1 overflow interrupt(TOIE1)
  sei();
}


struct Time get_time(){
  return m_time;
}

void get_time_str(char * out){
  struct Time t = get_time();
  sprintf(out, "%d:%d:%d\r", t.hours, t.minutes, t.seconds);
}
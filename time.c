#include <avr/io.h>
#include "time.h"
#include <stdio.h>
#include <avr/interrupt.h>
#define MAX_CNT 65535

static struct Time m_time;
static int min_changed = 1;

ISR (TIMER1_OVF_vect){
  m_time.seconds++;
  if (m_time.seconds >= 60){
    m_time.minutes++;
    m_time.seconds = 0;
    min_changed = 1;
  }
  if (m_time.minutes >= 60){
    m_time.hours++;
    m_time.minutes = 0;
  }
  if (m_time.hours >= 24){
    m_time.hours = 0;
  }
  TCNT1 = MAX_CNT - 12695;   // for 1 sec at 13 MHz
}

int time_min_has_changed(){
  if (min_changed){
    min_changed = 0;
    return 1;
  }
  return 0;
}

void init_time(struct Time t){
  m_time = t;
  TCCR1A = 0x00;
  TCNT1 = MAX_CNT - 12695;   // for 1 sec at 13 MHz
  TCCR1B = (1 << CS10) | (1 << CS12);  // Timer mode with 1024 prescler
  TIMSK = (1 << TOIE1) ;   // Enable timer1 overflow interrupt(TOIE1)
}


struct Time get_time(){
  return m_time;
}

void get_time_str(char * out){
  struct Time t = get_time();
  if (t.hours < 10)
    sprintf(out,  "0%d:", t.hours);
  else
    sprintf(out,  "%d:", t.hours);
  if (t.minutes < 10)
    sprintf(out,  "%s0%d", out, t.minutes);
  else
    sprintf(out, "%s%d", out, t.minutes);
}

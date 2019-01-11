#include <avr/io.h>
#include "time.h"
#include <stdio.h>
#include <avr/interrupt.h>
#define MAX_CNT 65535

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
  TCNT1 = MAX_CNT - 12695;             // set timer value below MAX_CNT threshold: 1 sec at 13 MHz equal 12695 timer ticks
}


void init_time(struct Time t){
  m_time = t;
  TCCR1A = 0x00;                       // set prescaler part A to 0
  TCNT1 = MAX_CNT - 12695;             // set timer value below MAX_CNT threshold: 1 sec at 13 MHz equal 12695 timer ticks
  TCCR1B = (1 << CS10) | (1 << CS12);  // Timer mode with 1024 prescaler
  TIMSK = (1 << TOIE1) ;               // Enable timer1 overflow interrupt(TOIE1)
}


void set_time(struct Time t){
  m_time = t;
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

#include <stdbool.h>
#include <math.h>
#include "draw.h"
#include "monitor.h"
#include <avr/io.h>
#include "led.h"
#include "draw.h"
#include "time.h"
#include "bluetooth.h"
#include <avr/interrupt.h>
#include <util/delay.h>

#define DIVISOR 4

int first_display = 1;
int last_angle = 0;

static char (*cadrans[4])[2] = {(char (*)[2])un, (char (*)[2])un, (char (*)[2])trois, (char (*)[2])trois};
static char (*figures[10])[2] = {(char (*)[2])zero, (char (*)[2])un, (char (*)[2])deux, (char (*)[2])trois,
(char (*)[2])quatre, (char (*)[2])cinq, (char (*)[2])six, (char (*)[2])sept, (char (*)[2])huit, (char (*)[2])neuf};

void update(char * t){
  cadrans[3] = figures[t[4]-'0'];
  if (t[4] == '0' || first_display){
    cadrans[0] = figures[t[1]-'0'];
    if (t[0] == '0' || first_display){
      cadrans[2] = figures[t[3]-'0'];
      if (t[1] == '0' || first_display)
	cadrans[1] = figures[t[0]-'0'];
    }
  }
}

void set_led_val(){
  int alpha = get_current_angle();

  char leds_seconds = 0x00;
  if ( (alpha + 180)%360 <= 6 * get_time().seconds ){
    leds_seconds |= 0x80;
  }
  alpha = 270-alpha;
  while (alpha < 0)
    alpha = 360 + alpha;
  char v =   cadrans[alpha/90][alpha/DIVISOR][0] & ~(1 << 7);
  set_leds(v | leds_seconds, cadrans[alpha/90][alpha/DIVISOR][1]);
}

void draw(){
  set_led_val();
}

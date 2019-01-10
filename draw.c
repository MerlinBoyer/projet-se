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

#define DIVISOR 2

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
  alpha = 270-alpha;
  while (alpha < 0)
    alpha = 360 + alpha;
  set_leds(cadrans[alpha/90][alpha/DIVISOR][0]
	   , cadrans[alpha/90][alpha/DIVISOR][1]);
}

void draw(){
  set_led_val();
}

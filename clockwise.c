#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "led.h"
#include "time.h"
#include "bluetooth.h"
#include "clockwise.h"
#include "monitor.h"

#define DISPLAY_PRECISION 3  // angle offset when checking angle (ie clockwise width)


/////////////////// convert time into angle //////////////////
int get_angle_from_h( int h ){
  return 30 * (h % 12);
}

int get_angle_from_m( int m){
  return 6 * m;
}

int get_angle_from_s( int s){
  return 6 * s;
}


//////////////////// print clockwise ///////////////
/*
*  Set leds off, get time and corresponding angles
*  if time angles are same as current angle then turn
*  some leds on (hours, minutes or seconds)
*/
void draw_clockwise(){
  int real_angle = (get_current_angle() + 180)%360;
  struct Time t = get_time();
  uint8_t leds_bottom = 0x00;
  uint8_t leds_top = 0x00;

  if ( real_angle <= get_angle_from_h(t.hours) + DISPLAY_PRECISION && real_angle >= get_angle_from_h(t.hours) - DISPLAY_PRECISION){
    leds_bottom = 0x7F;
  }
  if ( real_angle <= get_angle_from_m( t.minutes ) + DISPLAY_PRECISION && real_angle >= get_angle_from_m( t.minutes ) - DISPLAY_PRECISION){
    leds_bottom = 0xFF;
    leds_top = 0x0F;
  }
  if ( real_angle <= get_angle_from_s( t.seconds )){
    leds_top |= 0x80;
  }

  set_leds(leds_top, leds_bottom);
}

#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "led.h"
#include "time.h"
#include "Ffloat.h"
#include "bluetooth.h"
#include "clockwise.h"

#define DISPLAY_PRECISION 3  // angle offset when checking angle


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

  if ( real_angle <= get_angle_from_h(t.hours) + DISPLAY_PRECISION &&  real_angle >= get_angle_from_h(t.hours) - DISPLAY_PRECISION){
    leds_bottom = 0xFF;
  }
  if ( real_angle <= get_angle_from_m( t.minutes ) + DISPLAY_PRECISION && real_angle >= get_angle_from_m( t.minutes ) - DISPLAY_PRECISION){
    leds_bottom = 0xFF;
    leds_top = 0x7F;
  }
  if ( real_angle <= get_angle_from_s( t.seconds )){
    leds_top |= 0x80;
  }

  set_leds(leds_top, leds_bottom);
}


/*
void draw_simple(){
  double angle = get_current_angle();
  struct Time t = get_time();

  //t.hours = 4;
  //t.minutes = 30;
  //t.seconds = 45;

  int a = 22;
  int b = 25;

  uint8_t val1;
  uint8_t val2;

  // hours : from 0 to 23
  for (int i=0; i<12; i++)
  {
      if (a == i || a == 12+i)
      {
        if (i<6)
        {
          if (angle >= 180+30*i && angle <= 180+30*i+10)
          {
            val1= 0x00;
            val2= 0xFF;
          }
          else
          {
            val1 = 0x00;
            val2 = 0x00;
          }
        }
        else    
        {
          if (angle >= 30*(i-6) && angle <= 30*(i-6)+10)
          {
            val1= 0x00;
            val2= 0xFF;
          }
          else
          {
            val1 = 0x00;
            val2 = 0x00;
          }
        }
      }
      set_leds(val1,val2);
  }

  //minutes  
  if (b<30)
  {
    if (angle >= 180+6*b-5 && angle <= 180+6*b+5)
    {
      val1= 0xFF;
      val2= 0xFF;
    }
    else
    {
      val1 = 0x00;
      val2 = 0x00;
    }
  }
  else    
  {
    if (angle >= 10*(b-30)-5 && angle <= 10*(b-30)+5)
    {
      val1= 0xFF;
      val2= 0xFF;
    }
    else
    {
      val1 = 0x00;
      val2 = 0x00;
    }
  }
  set_leds(val1,val2);
  

  // secondes  
  if (t.seconds<30)
  {
    if (angle >= 180 && angle <= 180+6*t.seconds)
    {
      val1= 0x80;
      val2= 0x00;
    }
    else
    {
      val1 = 0x00;
      val2 = 0x00;
    }
  }
  else    
  {
    if ((angle >= 180 && angle <= 359) || (angle <= t.seconds*6))
    {
      val1= 0x80;
      val2= 0x00;
    }
    else
    {
      val1 = 0x00;
      val2 = 0x00;
    }
  }
  set_leds(val1,val2);
  
 
}*/
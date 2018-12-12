#include <stdbool.h>
#include <math.h>
#include "draw.h"
#include "monitor.h"
#include <avr/io.h>
#include "led.h"
#include "draw.h"
#include "figure.h"
#include "time.h"

#define RETARD M_PI/9
// size = 2*radius * 2*radius

Figure m_shape;
float m_radius;

//char tab_sin[360] = {0};
char tab_cos[360/5] = {0};
/*
double compute_sin(int radians){
  int degres = 180 * radians / M_PI;
  degres %= 360;
  if (degres == 0 || degres == 360)
    return 0;
  if (!tab_sin[degres]){
    tab_sin[degres] = sin(radians)*100;
  }
  return tab_sin[degres]*100;
}
*/
float compute_cos(float radians){
  int degres = 180 * radians / M_PI;
  degres %= 360;
  if (degres == 90 || degres == 270)
    return 0;
  if (!tab_cos[degres/5]){
    tab_cos[degres/5] = cos(radians)*100;
  }
  return tab_cos[degres/5]/100.;
}
void init_draw(Figure * shape, int radius){
  int i = 0;
  int j = 0;
  for (i = 0; i < radius*2-1; i++){
    for (j = 0; j < radius*2-1; j++){
      figure_set_pixel(&m_shape, i, j, figure_get_pixel(shape, i, j));
    }
  }  
  m_radius = radius; // 17
}


void get_led_val(bool values[]){
  float alpha = get_current_angle();
  alpha *= -1;
  alpha -= (M_PI/2+RETARD); // led vers le haut par défaut
  int x = m_radius * compute_cos(alpha);// 0
  int y = m_radius * sin(alpha); // 17
  for (int i = 0; i < NB_LED; i++){
    int x_centre = m_radius;
    int y_centre = m_radius;
    values[i] = figure_get_pixel(&m_shape, y_centre-y/(float)NB_LED*i, x_centre+x/(float)NB_LED*i);
  }
}

void draw(){
  bool values[NB_LED] = {0}; 
  get_led_val(values);
  char val1 = 0;
  char val2 = 0;
  for (int i = 0; i < NB_LED; i++){
    if (i == 0) continue;
    if (i <= 8){
      if (values[i])
	val2 |= (1 << i-1);
      else
	val2 &= ~(1 << i-1);
    }
    else {
      if (values[i])
	val1 |= (1 << (i-9));
      else
	val1 &= ~(1 << (i-9));
    }
  }
  set_leds(val1, val2);
}

void draw_simple(){
  double angle = get_current_angle_degree();
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
  
/*
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
  */
 
}
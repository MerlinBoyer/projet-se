#include <stdbool.h>
#include <math.h>
#include "draw.h"
#include "monitor.h"
#include <avr/io.h>
#include "led.h"
#include "draw.h"
#include "figure.h"
#include "time.h"

#define RETARD M_PI/10
// size = 2*radius * 2*radius

const float sinus[] = {0,0.01745,0.03490,0.05234,0.06976,0.08716,0.10453,0.12187,0.13917,0.15643,0.17365,0.19081,0.20791,0.22495,0.24192,0.25882,0.27564,0.29237,0.30902,0.32557,0.34202,0.35837,0.37461,0.39073,0.40674,0.42262,0.43837,0.45399,0.46947,0.48481,0.5,0.51504,0.52992,0.54464,0.55919,0.57358,0.58779,0.60182,0.61566,0.62932,0.64279,0.65606,0.66913,0.68200,0.69466,0.70711,0.71934,0.73135,0.74314,0.75471,0.76604,0.77715,0.78801,0.79864,0.80902,0.81915,0.82904,0.83867,0.84805,0.85717,0.86603,0.87462,0.88295,0.89101,0.89879,0.90631,0.91355,0.92050,0.92718,0.93358,0.93969,0.94552,0.95106,0.95630,0.96126,0.96593,0.97030,0.97437,0.97815,0.98163,0.98481,0.98769,0.99027,0.99255,0.99452,0.99619,0.99756,0.99863,0.99939,0.99985,1};

const float cosinus[] = {1,0.99985,0.99939,0.99863,0.99756,0.99619,0.99452,0.99255,0.99027,0.98769,0.98481,0.98163,0.97815,0.97437,0.97030,0.96593,0.96126,0.95630,0.95106,0.94552,0.93969,0.93358,0.92718,0.92050,0.91355,0.90631,0.89879,0.89101,0.88295,0.87462,0.86603,0.85717,0.84805,0.83867,0.82904,0.81915,0.80902,0.79864,0.78801,0.77715,0.76604,0.75471,0.74314,0.73135,0.71934,0.70711,0.69466,0.68200,0.66913,0.65606,0.64279,0.62932,0.61566,0.60182,0.58779,0.57358,0.55919,0.54464,0.52992,0.51504,0.5,0.48481,0.46947,0.45399,0.43837,0.42262,0.40674,0.39073,0.37461,0.35837,0.34202,0.32557,0.30902,0.29237,0.27564,0.25882,0.24192,0.22495,0.20791,0.19081,0.17365,0.15643,0.13917,0.12187,0.10453,0.08716,0.06976,0.05234,0.03490,0.01745,0};

Figure m_shape;
float m_radius;

float compute_cosinus(float radians){
  int degres = 180. / M_PI * radians;
  if (degres <= 90)
    return cosinus[degres];
  else if (degres <= 180)
    return -1 * cosinus[180-degres];    
  else if (degres <= 270)
    return -1 * cosinus[degres-180];
  else if (degres <= 360)
  return cosinus[360-degres];
}

float compute_sinus(float radians){
  int degres = 180. / M_PI * radians;
  if (degres <= 90)
    return sinus[degres];
  else if (degres <= 180)
    return sinus[180-degres];    
  else if (degres <= 270)
    return -1 * sinus[degres-180];
  else if (degres <= 360)
  return -1 * sinus[360-degres];
}

void init_draw(Figure * shape, int radius){
  int i = 0;
  int j = 0;
  for (i = 0; i < radius*2+1; i++){
    for (j = 0; j < radius*2+1; j++){
      figure_set_pixel(&m_shape, i, j, figure_get_pixel(shape, i, j));
    }
  }  
  m_radius = radius; // 16
}


void get_led_val(bool values[]){
  float alpha = get_current_angle();
  alpha *= -1;
  alpha -= (M_PI/2-RETARD); // led vers le haut par défaut
  while (alpha < 0)
    alpha = 2 * M_PI + alpha;
  float c = compute_cosinus(alpha);// 0
  float s = compute_sinus(alpha); // 16
  float x = m_radius * c;// 0
  float y = m_radius * s; // 16
  for (int i = 0; i < NB_LED; i++){
    int x_centre = m_radius;
    int y_centre = m_radius;
    values[i] = figure_get_pixel(&m_shape, y_centre-roundf(y/(float)(NB_LED-1)*i), x_centre+roundf(x/(float)(NB_LED-1)*i));
  }
}

void draw(){
  bool values[NB_LED] = {0}; 
  get_led_val(values);
  char val1 = 0;
  char val2 = 0;
  for (int i = 0; i < NB_LED; i++){
    if (i == 0)
      continue;
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

#include <stdbool.h>
#include <math.h>
#include "draw.h"
#include "monitor.h"
#include <avr/io.h>
#include "led.h"
#include "draw.h"
#include "figure.h"

#define RETARD M_PI/15
// size = 2*radius * 2*radius

const float sinus[] = {0,0.01745,0.03490,0.05234,0.06976,0.08716,0.10453,0.12187,0.13917,0.15643,0.17365,0.19081,0.20791,0.22495,0.24192,0.25882,0.27564,0.29237,0.30902,0.32557,0.34202,0.35837,0.37461,0.39073,0.40674,0.42262,0.43837,0.45399,0.46947,0.48481,0.5,0.51504,0.52992,0.54464,0.55919,0.57358,0.58779,0.60182,0.61566,0.62932,0.64279,0.65606,0.66913,0.68200,0.69466,0.70711,0.71934,0.73135,0.74314,0.75471,0.76604,0.77715,0.78801,0.79864,0.80902,0.81915,0.82904,0.83867,0.84805,0.85717,0.86603,0.87462,0.88295,0.89101,0.89879,0.90631,0.91355,0.92050,0.92718,0.93358,0.93969,0.94552,0.95106,0.95630,0.96126,0.96593,0.97030,0.97437,0.97815,0.98163,0.98481,0.98769,0.99027,0.99255,0.99452,0.99619,0.99756,0.99863,0.99939,0.99985,1};

const float cosinus[] = {1,0.99985,0.99939,0.99863,0.99756,0.99619,0.99452,0.99255,0.99027,0.98769,0.98481,0.98163,0.97815,0.97437,0.97030,0.96593,0.96126,0.95630,0.95106,0.94552,0.93969,0.93358,0.92718,0.92050,0.91355,0.90631,0.89879,0.89101,0.88295,0.87462,0.86603,0.85717,0.84805,0.83867,0.82904,0.81915,0.80902,0.79864,0.78801,0.77715,0.76604,0.75471,0.74314,0.73135,0.71934,0.70711,0.69466,0.68200,0.66913,0.65606,0.64279,0.62932,0.61566,0.60182,0.58779,0.57358,0.55919,0.54464,0.52992,0.51504,0.5,0.48481,0.46947,0.45399,0.43837,0.42262,0.40674,0.39073,0.37461,0.35837,0.34202,0.32557,0.30902,0.29237,0.27564,0.25882,0.24192,0.22495,0.20791,0.19081,0.17365,0.15643,0.13917,0.12187,0.10453,0.08716,0.06976,0.05234,0.03490,0.01745,0};

Figure m_shape;
double m_radius;

float compute_cosinus(float radians){
  int degres = 180 / M_PI * radians;
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
  int degres = 180 / M_PI * radians;
  if (degres <= 90)
    return sinus[degres];
  else if (degres <= 180)
    return -1 * sinus[180-degres];    
  else if (degres <= 270)
    return -1 * sinus[degres-180];
  else if (degres <= 360)
    return sinus[360-degres];
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
  double alpha = get_current_angle();
  alpha *= -1;
  alpha -= (M_PI/2+RETARD); // led vers le haut par défaut
  int x = m_radius * cos(alpha);// 0
  int y = m_radius * sin(alpha); // 17
  float step = m_radius / (double)NB_LED; // 1
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

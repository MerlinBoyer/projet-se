#include <stdbool.h>
#include <math.h>
#include "draw.h"
#include "monitor.h"
#include <avr/io.h>
#include "led.h"
#include "draw.h"
#include "figure.h"
#include "time.h"
#include "Ffloat.h"
#include "bluetooth.h"
#include <avr/interrupt.h>
#include <util/delay.h>

#define RETARD M_PI/10
#define DISPLAY_PRECISION 5  // angle offset when checking angle

const char truc[34][17] = {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},{0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1},{0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,2},{0,0,0,0,1,1,1,1,1,1,1,1,2,2,2,2,2},{0,0,0,0,1,1,1,1,1,1,2,2,2,2,2,2,3},{0,0,0,1,1,1,1,1,2,2,2,2,2,2,3,3,3},{0,0,0,1,1,1,1,2,2,2,2,2,3,3,3,3,4},{0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4},{0,0,1,1,1,1,2,2,2,3,3,3,3,4,4,4,5},{0,0,1,1,1,2,2,2,3,3,3,3,4,4,4,5,5},{0,0,1,1,1,2,2,2,3,3,3,4,4,4,5,5,6},{0,0,1,1,2,2,2,3,3,3,4,4,5,5,5,6,6},{0,0,1,1,2,2,2,3,3,4,4,4,5,5,6,6,7},{0,0,1,1,2,2,3,3,4,4,4,5,5,6,6,7,7},{0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8},{0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,8},{0,1,1,2,2,3,3,4,4,5,5,6,6,7,7,8,9},{0,1,1,2,2,3,3,4,5,5,6,6,7,7,8,8,9},{0,1,1,2,2,3,4,4,5,5,6,7,7,8,8,9,10},{0,1,1,2,3,3,4,4,5,6,6,7,8,8,9,9,10},{0,1,1,2,3,3,4,5,5,6,7,7,8,9,9,10,11},{0,1,1,2,3,3,4,5,6,6,7,8,8,9,10,10,11},{0,1,1,2,3,4,4,5,6,6,7,8,9,9,10,11,12},{0,1,2,2,3,4,5,5,6,7,8,8,9,10,11,11,12},{0,1,2,2,3,4,5,5,6,7,8,9,9,10,11,12,13},{0,1,2,2,3,4,5,6,7,7,8,9,10,11,11,12,13},{0,1,2,3,3,4,5,6,7,8,8,9,10,11,12,13,14},{0,1,2,3,4,4,5,6,7,8,9,10,11,11,12,13,14},{0,1,2,3,4,5,5,6,7,8,9,10,11,12,13,14,15},{0,1,2,3,4,5,6,7,8,8,9,10,11,12,13,14,15},{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16},{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16},{0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,17}};

const float cosinus[] = {1000,999.85,999.39,998.63,997.56,996.19,994.52,992.55,990.27,987.69,984.81,981.63,978.15,974.37,970.30,965.93,961.26,956.30,951.06,945.52,939.69,933.58,927.18,920.50,913.55,906.31,898.79,891.01,882.95,874.62,866.03,857.17,848.05,838.67,829.04,819.15,809.02,798.64,788.01,777.15,766.04,754.71,743.14,731.35,719.34,707.11,694.66,682.00,669.13,656.06,642.79,629.32,615.66,601.82,587.79,573.58,559.19,544.64,529.92,515.04,500.0,484.81,469.47,453.99,438.37,422.62,406.74,390.73,374.61,358.37,342.02,325.57,309.02,292.37,275.64,258.82,241.92,224.95,207.91,190.81,173.65,156.43,139.17,121.87,104.53,87.16,69.76,52.34,34.90,17.45,0};

Figure m_shape;
float m_radius;

int last_angle = 0;

int abs(int v){
  return (v >= 0)?v:-1*v;
}

float compute_cosinus(int degres){
  if (degres <= 90)
    return cosinus[degres];
  else if (degres <= 180)
    return -1 * cosinus[180-degres];    
  else if (degres <= 270)
    return -1 * cosinus[degres-180];
  else if (degres <= 360)
  return cosinus[360-degres];
}

float compute_sinus(int degres){
  if (degres <= 90)
    return cosinus[abs(degres)];
  else if (degres <= 180)
    return cosinus[abs(90-(180-degres))];    
  else if (degres <= 270)
    return -1 * cosinus[abs(90-(degres-180))];
  else if (degres <= 360)
    return -1 * cosinus[abs(90-(360-degres))];
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

#define NB_CADRANS 1

void get_led_val(bool values[]){
  //_delay_us(50);
  all_leds_OFF();
  int alpha = get_current_angle();  
  if (last_angle != alpha){
    last_angle = alpha;
  }
  else return;
  alpha *= -1;
  alpha -= (90-RETARD); // led vers le haut par défaut
  while (alpha < 0)
    alpha = 360 + alpha;
  int c = compute_cosinus(alpha/NB_CADRANS*NB_CADRANS);// 0
  int s = compute_sinus(alpha/NB_CADRANS*NB_CADRANS); // 16
  int x = m_radius * c;// 0
  int y = m_radius * s; // 16
  char val1 = 0;
  char val2 = 0;
  for (int i = 0; i < NB_LED; i++){
    int x_centre = m_radius;
    int y_centre = m_radius;
    int sign1 = (y >= 0) ? 1: -1;
    int sign2 = (x >= 0) ? 1: -1;
    values[i] = figure_get_pixel(&m_shape, y_centre-sign1*truc[abs(y)/1000][i], x_centre+sign2*truc[abs(x)/1000][i]);
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

void draw(){
  bool values[NB_LED] = {0}; 
  get_led_val(values);
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
int get_angle_from_h( int h ){
  return 30 * (h % 12);
}

int get_angle_from_m( int m){
  return 6 * m;
}

int get_angle_from_s( int s){
  return 6 * s;
}

void draw_simple(){
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


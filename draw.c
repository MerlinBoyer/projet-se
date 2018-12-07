#include <stdbool.h>
#include <math.h>
#include "draw.h"
#include "monitor.h"
#include <avr/io.h>
#include "led.h"
#include "draw.h"
#include "figure.h"

#define PI 3.14

// size = 2*radius * 2*radius

Figure m_shape;
double m_radius;

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
  alpha += PI/2; // led vers le haut par défaut
  int x = m_radius * cos(alpha);
  int y = m_radius * sin(alpha);
  int step = m_radius / NB_LED;
  for (int i = 0; i < NB_LED; i++){
    int x_centre = m_radius;
    int y_centre = m_radius;
    values[NB_LED] = figure_get_pixel(&m_shape, y_centre+i*step*y, x_centre+i*step*x);
  }
}

void draw(){
  bool values[NB_LED] = {0}; 
  get_led_val(values);
  char val1 = 0;
  char val2 = 0;  
  for (int i = 0; i < NB_LED; i++){
    val1 = 0;
    val2 = 0;
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

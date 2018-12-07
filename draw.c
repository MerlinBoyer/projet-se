#include <stdbool.h>
#include <math.h>
#include "draw.h"
#include "monitor.h"

#define PI 3.14
#define NB_LED 10

// size = 2*radius * 2*radius
typedef struct Figure {
  bool ** m_shape;
  double m_radius;
} Figure;

volatile Figure figure;

void init_draw(bool ** shape, int radius){  
  figure.m_shape = shape;
  figure.m_radius = radius;
}


void get_led_val(bool values[]){
  double  alpha = get_current_angle();
  alpha -= PI/2; // led vers le bas par défaut
  int x = figure.m_radius * cos(alpha);
  int y = figure.m_radius * sin(alpha);
  int step = figure.m_radius / NB_LED;
  for (int i = 0; i < NB_LED; i++){
    int x_centre = figure.m_radius;
    int y_centre = figure.m_radius;
    values[NB_LED] = figure.m_shape[x_centre+i*step*x][y_centre+i*step*y];
  }
}

void draw(){
  bool values[NB_LED]; 
  get_led_val(values);
  for (int i = 0; i < NB_LED; i++){
    // set led val
  }
}

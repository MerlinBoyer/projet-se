#include <stdbool.h>
#include <math.h>
#include "draw.h"
#include "monitor.h"
#include <avr/io.h>
#include "led.h"
#include "figure.h"


void init_aiguilles(Figure * aiguille_h, Figure * aiguille_m, Figure * aiguille_s){
  int j = SIZE / 2 + 1;   // 17
  for (int i = SIZE / 2 + 1; i >= 0; i--){
    figure_set_pixel(aiguille_s, i, j, true);
    if (i > 5) figure_set_pixel(aiguille_m, i, j, true);
    if (i > 10) figure_set_pixel(aiguille_h, i, j, true);
  }
}

void rotation(Figure * figure, int size, double angle, Figure * out){
  for (int i = 0; i < size; i++){
    for (int j = 0; j < size; j++){      
      if (figure_get_pixel(figure, i, j)){
	// j <=> x, i <=> -y
	int new_x = cos(angle)*j + sin(angle) * (size-1-i);
	int new_y = -1*sin(angle)*j + cos(angle) * (size-1-i);
	figure_set_pixel(out, new_x, new_y, true);
      }
    }
  }
}

void union_fig(Figure * fig1, Figure * fig2, int size, Figure * out){
  for (int i = 0; i < size; i++){
    for (int j = 0; j < size; j++){
      if (figure_get_pixel(fig1, i, j) || figure_get_pixel(fig2, i, j)){
        figure_set_pixel(out, i, j, true);
      }
    }
  }
}

void init_clock(){
  Figure aiguille_h; figure_init(&aiguille_h);
  Figure aiguille_m; figure_init(&aiguille_m);
  Figure aiguille_s; figure_init(&aiguille_s);
  init_aiguilles(&aiguille_h, &aiguille_m, &aiguille_s);
  Figure clock; figure_init(&clock);
  union_fig(&aiguille_h, &aiguille_m, SIZE, &clock);
  union_fig(&clock, &aiguille_s, SIZE, &clock);
  init_draw(&clock, SIZE/2+1);
  init_monitor();
}


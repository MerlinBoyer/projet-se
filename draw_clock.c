#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "draw.h"
#include "monitor.h"

#define SIZE 33

bool aiguille_h[SIZE][SIZE] = {0};
bool aiguille_m[SIZE][SIZE] = {0};
bool aiguille_s[SIZE][SIZE] = {0};

void init_aiguilles(){
  int j = SIZE / 2 + 1; // 17
  for (int i = SIZE / 2 + 1; i >= 0; i--){
    aiguille_s[i][j] = 1;
    if (i > 5) aiguille_m[i][j] = 1;
    if (i > 10) aiguille_s[i][j] = 1;
  }
}

bool ** rotation(bool ** figure, int size, double angle){
  bool ** out = malloc(sizeof(bool *)*size);
  for (int i = 0; i < size; i++){
    out[i] = malloc(sizeof(bool)*size);
    for (int j = 0; j < size; j++){
      out[i][j] = 0;
    }
  }
  for (int i = 0; i < size; i++){
    for (int j = 0; j < size; j++){      
      if (figure[i][j]){
	// j <=> x, i <=> -y
	int new_x = cos(angle)*j + sin(angle) * (size-1-i);
	int new_y = -1*sin(angle)*j + cos(angle) * (size-1-i);
	out[new_x][new_y] = 1;
      }
    }
  }
  return out;
}

bool ** union_fig(bool ** fig1, bool ** fig2, int size){
  bool ** out = malloc(sizeof(bool *)*size);
  for (int i = 0; i < size; i++){
    out[i] = malloc(sizeof(bool)*size);
    for (int j = 0; j < size; j++){
      out[i][j] = 0;
    }
  }
  for (int i = 0; i < size; i++){
    for (int j = 0; j < size; j++){
      if (fig1[i][j] || fig2[i][j])
	out[i][j] = 1;
    }
  }
  return out;
}

void init_clock(){
  init_aiguilles();
  bool ** temp = union_fig((bool **)aiguille_h, (bool **)aiguille_m, SIZE);
  bool ** clock = union_fig(temp, (bool **)aiguille_s, SIZE);
  for(int i = 0; i < SIZE; i++){
    free(temp[i]);
  }
  free(temp);
  init_draw(clock, SIZE/2+1);
  init_monitor();
}

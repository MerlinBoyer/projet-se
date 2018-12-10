#include "figure.h"

bool figure_get_pixel(Figure *f, int i, int j){
  int j1 = j / 8;
  int j2 = j % 8; // 1
  return f->pixels[i][j1] & (1 << j2);
}

void figure_set_pixel(Figure *f, int i, int j, bool value){
  int j1 = j / 8;
  int j2 = j % 8;
  if (value)
    f->pixels[i][j1] |= (1 << j2);
  else
    f->pixels[i][j1] &= ~(1 << j2);
}

void figure_init(Figure *f){
  for (int i = 0; i < SIZE; i++)
    for (int j = 0; j < SIZE/8+1; j++)
      f->pixels[i][j] = 0;
}

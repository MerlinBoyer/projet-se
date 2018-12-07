#ifndef FIGURE_H
#define FIGURE_H

#include <stdbool.h>

#define SIZE 33

typedef struct Figure {
  unsigned char pixels[SIZE][SIZE/8+1];
} Figure;


bool figure_get_pixel(Figure *f, int i, int j);
void figure_set_pixel(Figure *f, int i, int j, bool value);
void figure_init(Figure *f);

#endif

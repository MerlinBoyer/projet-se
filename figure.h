#ifndef FIGURE_H
#define FIGURE_H

#include <stdbool.h>

#define SIZE 33
#define SIZE_CHIFFRE 16

typedef struct Figure {
  unsigned char pixels[SIZE][SIZE/8+1];
} Figure;

bool figure_get_pixel(Figure *f, int i, int j);
void figure_set_pixel(Figure *f, int i, int j, bool value);
void figure_init(Figure *f);
void figure_set_quadrant(Figure *f, int quadrant, const unsigned char *chiffre);


#endif

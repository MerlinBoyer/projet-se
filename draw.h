#ifndef DRAW_H
#define DRAW_H

#include <stdbool.h>
#include "figure.h"

#define NB_LED 17

void init_draw(Figure * shape, int radius);
void draw();
void draw_simple();

#endif

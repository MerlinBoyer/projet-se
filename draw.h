#ifndef DRAW_H
#define DRAW_H

#include <stdbool.h>
#include "figure.h"

#define NB_LED 17

void init_draw(Figure * shape, int radius);
void draw();
void draw_simple();

int get_angle_from_h( int h );
int get_angle_from_m( int m );
int get_angle_from_s( int s );
#endif

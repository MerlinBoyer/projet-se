#ifndef FFLOAT_H
#define FFLOAT_H

#define DIV_ACC 7

typedef struct Ffloat {
  long value;
  long coma_index;
} Ffloat;

float Ffloat_to_float(Ffloat a);
Ffloat ffloat_new(int value, int coma_index);
Ffloat ffloat_add_ffloat(Ffloat a, Ffloat b);
Ffloat ffloat_sub_ffloat(Ffloat a, Ffloat b);
Ffloat int_add_ffloat(int a, Ffloat b);
Ffloat int_sub_ffloat(int a, Ffloat b);
Ffloat ffloat_add_int(Ffloat a, int b);
Ffloat ffloat_sub_int(Ffloat a, int b);
Ffloat ffloat_mult_ffloat(Ffloat a, Ffloat b);
Ffloat int_mult_ffloat(int a, Ffloat b);
Ffloat ffloat_mult_int(Ffloat a, int b);
Ffloat ffloat_div_ffloat(Ffloat a, Ffloat b);
Ffloat int_div_ffloat(int a, Ffloat b);
Ffloat ffloat_div_int(Ffloat a, int b);
Ffloat int_div_int(int a, int b);

#endif

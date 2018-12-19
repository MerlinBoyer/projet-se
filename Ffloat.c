#include "Ffloat.h"

const long power_val[] = {1,10,100,1000,10000,100000,1000000,10000000,100000000, 1000000000};


typedef struct Ffloat Ffloat;

/*
ex: value * 10 ^ (-1*(coma_index))
 */
Ffloat ffloat_new(int value, int coma_index){
  Ffloat out;
  out.value = value;
  out.coma_index = coma_index;
  return out;
}


Ffloat ffloat_add_ffloat(Ffloat a, Ffloat b){
  Ffloat out;
  if (b.coma_index > a.coma_index){
    int diff = a.coma_index-b.coma_index;
    int multiplier = -1*power_val[diff];
    out.coma_index = b.coma_index;
    out.value = a.value*multiplier+b.value;
  } else {
    int diff = b.coma_index-a.coma_index;
    int multiplier = -1*power_val[diff];
    out.coma_index = a.coma_index;
    out.value = b.value*multiplier+a.value;
  }  
  return out;
}

Ffloat ffloat_sub_ffloat(Ffloat a, Ffloat b){
  Ffloat temp;
  temp.value = b.value * -1;
  temp.coma_index = b.coma_index;
  return ffloat_add_ffloat(a, temp);
}

Ffloat int_add_ffloat(int a, Ffloat b){
  Ffloat temp;
  temp.value = a;
  temp.coma_index = 0;
  return ffloat_add_ffloat(temp, b);
}

Ffloat int_sub_ffloat(int a, Ffloat b){
  Ffloat temp;
  temp.value = a;
  temp.coma_index = 0;
  Ffloat temp2;
  temp2.value = -1 * b.value;
  temp2.coma_index = b.coma_index;
  return ffloat_add_ffloat(temp, temp2);
}

Ffloat ffloat_add_int(Ffloat a, int b){
  return int_add_ffloat(b, a);
}

Ffloat ffloat_sub_int(Ffloat a, int b){
  return int_sub_ffloat(b, a);
}

Ffloat ffloat_mult_ffloat(Ffloat a, Ffloat b){
  Ffloat out;
  out.value = a.value * b.value;
  out.coma_index = a.coma_index + b.coma_index;
  if (out.coma_index > 10){
    int diff = out.coma_index - 10;
    out.value /= power_val[diff];
    out.coma_index -= diff;
  }
  return out;
}

Ffloat int_mult_ffloat(int a, Ffloat b){
  Ffloat out;
  out.value = a * b.value;
  out.coma_index = b.coma_index;
  return out;
}

Ffloat ffloat_mult_int(Ffloat a, int b){
  return int_mult_ffloat(b, a);
}


Ffloat ffloat_div_ffloat(Ffloat a, Ffloat b){
  int multiplier = 1;
  int n = 0;  
  while (a.value*multiplier < b.value && n < 10){
    multiplier *= 10;
    n++;
  }
  multiplier *= power_val[DIV_ACC];
  Ffloat out;
  out.value = a.value * multiplier / b.value;
  out.coma_index = DIV_ACC + n + a.coma_index - b.coma_index;
  if (out.coma_index > 10){
    int diff = out.coma_index - 10;
    out.value /= power_val[diff];
    out.coma_index -= diff;
  }
  return out;
}

Ffloat int_div_int(int a, int b){
  Ffloat temp1 = ffloat_new(a, 0);
  Ffloat temp2 = ffloat_new(b, 0);
  return ffloat_div_ffloat(temp1, temp2);
}

Ffloat int_div_ffloat(int a, Ffloat b){
  Ffloat temp;
  temp.value = a;
  temp.coma_index = 0;
  return ffloat_div_ffloat(temp, b);
}

Ffloat ffloat_div_int(Ffloat a, int b){
  Ffloat temp;
  temp.value = b;
  temp.coma_index = 0;
  return ffloat_div_ffloat(a, temp);
}

float Ffloat_to_float(Ffloat a){
  if (a.coma_index >= 0){
    int diviser = power_val[a.coma_index];
    return (float)(a.value) / ((float) diviser);
  } else {
    int multiplier = power_val[-1*a.coma_index];
    return a.value * multiplier;
  }
}

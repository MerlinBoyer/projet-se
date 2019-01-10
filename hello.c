#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "led.h"
#include "time.h"
#include "hello.h"
#include "monitor.h"
#include "bluetooth.h"


// void init_numbers()
// {
//   const uint8_t (*p0)[2] = ow;
//   const uint8_t (*p1)[2] = one;
//   const uint8_t (*p2)[2] = two;
//   const uint8_t (*p3)[2] = three;
//   const uint8_t (*p4)[2] = four;
//   const uint8_t (*p5)[2] = five;
//   const uint8_t (*p6)[2] = six;
//   const uint8_t (*p7)[2] = seven;
//   const uint8_t (*p8)[2] = eight;
//   const uint8_t (*p9)[2] = nine;
//
//   for (int j=0; j<2; j++)
//     {
//       numbers[0][j] = p0[j];
//       numbers[1][j] = p1[j];
//       numbers[2][j] = p2[j];
//       numbers[3][j] = p3[j];
//       numbers[4][j] = p4[j];
//       numbers[5][j] = p5[j];
//       numbers[6][j] = p6[j];
//       numbers[7][j] = p7[j];
//       numbers[8][j] = p8[j];
//       numbers[9][j] = p9[j];
//     }
// }
// void set_sec( int sec ){
//   sec++;
//   return;
// }

// void set_seca( int sec ){
//   sec++;
//   return;
// }
//
// void set_secb( int sec ){
//   sec++;
//   return;
// }
//
// void set_secc( int sec ){
//   sec++;
//   return;
// }
//
// void set_secd( int sec ){
//   sec++;
//   return;
// }



void set_sec(int sec)
{
  int sec_1 = sec%10;
  int sec_10 = sec/10;
  for (int i =0; i<4; i++)
  {
    ;
    // hour[i][0] = 0x00;
    // hour[i][1] = 0x00;
    // hour[i+4][0] = 0x00;
    // hour[i+4][1] = 0x00;

    hour[i][0] = numbers[sec_1][i][0];
    hour[i][1] = numbers[sec_1][i][1];
    hour[i+4][0] = numbers[sec_10][i][0];
    hour[i+4][1] = numbers[sec_10][i][1];

    // hour[i][0] = *numbers[sec_1][0];
    // hour[i][1] = *numbers[sec_1][1];
    // numbers[sec_1][0] = numbers[sec_1][0] + 2;
    // numbers[sec_1][1] = numbers[sec_1][1] + 2;
    // hour[i+4][0] = *numbers[sec_10][0];
    // hour[i+4][1] = *numbers[sec_10][1];
    // numbers[sec_10][0] = numbers[sec_10][0] + 2;
    // numbers[sec_10][1] = numbers[sec_10][1] + 2;

  }
  secondes_prec = sec;
}


void set_min(int min)
{
  ble_send_str(" m ");
  ble_send_int(min);
  int min_1 = min%10;
  int min_10 = min/10;
  for (int i = 8; i < 12; i++)
  {
    hour[i][0] = numbers[min_1][i][0];
    hour[i][1] = numbers[min_1][i][1];
    hour[i+4][0] = numbers[min_10][i][0];
    hour[i+4][1] = numbers[min_10][i][1];

    // hour[i][0] = *numbers[min_1][0];
    // hour[i][1] = *numbers[min_1][1];
    // numbers[min_1][0]++;
    // numbers[min_1][1]++;
    // hour[i+4][0] = *numbers[min_10][0];
    // hour[i+4][1] = *numbers[min_10][1];
    // numbers[min_10][0]++;
    // numbers[min_10][1]++;

  }
  minutes_prec = min;
}

void set_h(int h)
{
  ble_send_str("h ");
  ble_send_int(h);
  int h_1 = h%10;
  int h_10 = h/10;
  for (int i = 20; i < 24; i++)
  {
    hour[i][0] = numbers[h_1][i][0];
    hour[i][1] = numbers[h_1][i][1];
    hour[i+4][0] = numbers[h_10][i][0];
    hour[i+4][1] = numbers[h_10][i][1];

    // hour[i][0] = *numbers[h_1][0];
    // hour[i][1] = *numbers[h_1][1];
    // numbers[h_1][0]++;
    // numbers[h_1][1]++;
    // hour[i+4][0] = *numbers[h_10][0];
    // hour[i+4][1] = *numbers[h_10][1];
    // numbers[h_10][0]++;
    // numbers[h_10][1]++;

  }
  heures_prec = h;
}

void init_hello(){
  struct Time t = get_time();
  set_sec(t.seconds);
  set_min(t.minutes);
  set_h(t.hours);
  // char str[12] = {0};
  // get_time_str( str );
  // ble_send_str( str );
  // hour[9][0] = space[0][0];
  // hour[9][1] = space[0][1];
  // hour[19][0] = space[0][0];
  // hour[19][1] = space[0][1];
}



void draw_hello(){
  int real_angle = (get_current_angle() + 270)%360;
  struct Time t = get_time();
  uint8_t leds_bottom = 0x00;
  uint8_t leds_top = 0x00;

  int alpha = real_angle/6;
  if(alpha < 30)
  {
      leds_bottom = hello_arr[alpha][0];
      leds_top = hello_arr[alpha][1];
  }


  if(alpha >=30)
  {
    if (t.seconds != secondes_prec)
    {
      set_sec(t.seconds);
      // char str[12] = {0};
      // get_time_str( str );
      // ble_send_str( str );
      if (t.minutes != minutes_prec)
      {
        set_min(t.minutes);
        if(t.hours != heures_prec)
        {
          set_h(t.hours);
        }
      }
    }
    leds_bottom = hour[alpha-30][0];
    leds_top = hour[alpha-30][1];
  }

  set_leds(leds_top, leds_bottom);
}

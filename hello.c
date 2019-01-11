#include <math.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "led.h"
#include "time.h"
#include "hello.h"
#include "monitor.h"
#include "bluetooth.h"


///////////     Set seconds/minutes/hours into "hour" array       //////////
void set_sec(int sec)
{
  int sec_1 = sec%10;
  int sec_10 = sec/10;
  for (int i =0; i<4; i++)
  {
    hour[i][0] = numbers[sec_1][i][0];
    hour[i][1] = numbers[sec_1][i][1];
    hour[i+4][0] = numbers[sec_10][i][0];
    hour[i+4][1] = numbers[sec_10][i][1];
  }

  secondes_prec = sec;
}


void set_min(int min)
{
  ble_send_str(" m ");
  ble_send_int(min);
  int min_1 = min%10;
  int min_10 = min/10;
  ble_send_str(" min 1 ");
  ble_send_int(min_1);
  ble_send_str(" min 10 ");
  ble_send_int(min_10);
  for (int i = 10; i < 14; i++)
  {
    hour[i][0] = numbers[min_1][i-10][0];
    hour[i][1] = numbers[min_1][i-10][1];
    hour[i+4][0] = numbers[min_10][i-10][0];
    hour[i+4][1] = numbers[min_10][i-10][1];
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
    hour[i][0] = numbers[h_1][i-20][0];
    hour[i][1] = numbers[h_1][i-20][1];
    hour[i+4][0] = numbers[h_10][i-20][0];
    hour[i+4][1] = numbers[h_10][i-20][1];
  }

  heures_prec = h;
}



///////////     Update time and display        //////////
void init_hello(){
  struct Time t = get_time();
  set_sec(t.seconds);
  set_min(t.minutes);
  set_h(t.hours);
  hour[9][0] = space[0][0];
  hour[9][1] = space[0][1];
  hour[19][0] = space[0][0];
  hour[19][1] = space[0][1];
}


/*
*   Update time and display "hour" and "hello" arrays
*/
void draw_hello(){
  int real_angle = (get_current_angle() + 270)%360;
  struct Time t = get_time();
  uint8_t leds_bottom = 0x00;
  uint8_t leds_top = 0x00;
  int alpha = real_angle/6;

    // print "hello" if board is at the top
  if(alpha < 30)
  {
      leds_bottom = hello_arr[alpha][0];
      leds_top = hello_arr[alpha][1];
  }

    // print time if board is at the bottom
  if(alpha >=30)
  {
      // update hour array if time have changed
    if (t.seconds != secondes_prec)
    {
      set_sec(t.seconds);
      if (t.minutes != minutes_prec)
      {
        set_min(t.minutes);
        if(t.hours != heures_prec)
        {
          set_h(t.hours);
        }
      }
    }
      // get leds values according to data registered in hour array, depending on current angle
    leds_bottom = hour[alpha-30][0];
    leds_top = hour[alpha-30][1];
  }

    // turn leds on
  set_leds(leds_top, leds_bottom);
}

#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "bluetooth.h"
#include "time.h"
#include "led.h"
#include "draw.h"
#include "clockwise.h"
#include "monitor.h"



//////////////     Initialisation          ////////////

volatile int last_buffer_index;
struct Time t = {05,44,0};
char t_str[999];
char new_time[6];
enum Mode{CLOCKWISE, CHIFFRES} mode;

/*
*  call all initialisation functions required
*/
void global_init()
{
  bluetooth_init();         // init ble
  last_buffer_index = current_index_buff;   // set buffer index for ble data reception
  init_time(t);             // init timers and time counting (time.c)
  SPI_MasterInit();         // init SPI comm (led.c)
  init_monitor();
  sei();                    // Allow interruptions

    // alert user that init is over
  ble_send_str("init ok\n");
  set_leds(0xFF, 0xFF);
  _delay_ms(250);
  set_leds(0x00, 0x00);
}



///// checking functions  ///////
/*
*  Check if data have been saved into ble buffer
*  then update current index and send data to ble
*/
void check_ble()
{
  if( last_buffer_index >= MAXBUFF){
      last_buffer_index = 100;
  }
  while( last_buffer_index < current_index_buff){
      last_buffer_index++;

      char c =  USART_buffer[last_buffer_index ];
      char s[10];
      sprintf(s, "%d", last_buffer_index);
      ble_send_str( s );
      if(c == 'c'){
        mode = CHIFFRES;
      }
      if(c == 'a'){
        mode = CLOCKWISE;
      }
      // if( c == 'h'){
      //   set_heure = 1;
      //   return;
      // }
      // if(c=='1' || c=='2' || c=='3' || c=='4' || c=='5' || c=='6' || c=='7' || c=='8' || c=='9'){
      //   new_time
      // }
  }
}



/////            Main              ////////
void main(){
  mode = CLOCKWISE;   // set mode to print clockwises or numbers
  global_init();      // call all initialisations
  _delay_ms(1);
  char str[99];
  while (1){
    check_ble();
    if (mode == CLOCKWISE){
      draw_clockwise();
    } else if ( mode == CHIFFRES){
      get_time_str(str);
      update(str);
      draw();
    }
    //_delay_ms(250);
  }
}

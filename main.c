#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "bluetooth.h"
#include "time.h"
#include "led.h"
#include "draw_clock.h"
#include "draw.h"
#include "hallSensor.h"

//////       Initialisation          ////////

volatile int last_buffer_index;
struct Time t = {0,0,0};
char t_str[999];



void global_init()
{
  bluetooth_init();         // init ble
  last_buffer_index = current_index_buff;   // init buffer for ble receiving
  init_time(t);         // init timers and time counting (time.c)
  SPI_MasterInit();         // init SPI comm (led.c)
  init_clock();
  init_hallsensor();

  sei();             // Allow interrupt
  ble_send_str("init ok\n");
}



///// checking functions  ///////

/*
*  Check if data have been saved into ble buffer
*  then update current index and send data to ble
*/
void check_ble()
{
//    char nb[60];
//    sprintf( nb, "%d", current_index_buff);
//    ble_send_str( nb );
  while( last_buffer_index != current_index_buff){
      last_buffer_index++;
      if( last_buffer_index >= MAXBUFF){
          last_buffer_index = 0;
      }
      ble_send_char( USART_buffer[last_buffer_index ]);
  }
}

/*
*  Run routines to updates some data
*/
void check(){
  check_ble();
}


/////            Main              ////////


void main(){
  _delay_ms(1);
  global_init();

  while (1){
    check();
    // get_time_str(t_str);
    // ble_send_str( t_str );
    // ble_send_str( "\n" );
    //draw();
    draw_circles();
  }
}

//draw_circles();
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "bluetooth.h"
#include "time.h"
#include "led.h"
#include "draw_clock.h"
#include "draw.h"
#include "clockwise.h"




//////////////     Initialisation          ////////////

volatile int last_buffer_index;
struct Time t = {22,15,0};
char t_str[999];
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
  init_clock();
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
  while( last_buffer_index != current_index_buff){
      last_buffer_index++;
      if( last_buffer_index >= MAXBUFF){
          last_buffer_index = 0;
      }
      ble_send_char( USART_buffer[last_buffer_index ]);
  }
}

/*
*  Run routines to update data
*/
void check(){
  check_ble();
}


/////            Main              ////////


void main(){
  mode = CLOCKWISE;   // set mode to print clockwises or numbers
  global_init();      // call all initialisations
  _delay_ms(1);
  while (1){
    if (mode == CLOCKWISE){
      draw_clockwise();
    }else if ( mode == CHIFFRES){
      draw();
    }
    //_delay_ms(250);
  }
}

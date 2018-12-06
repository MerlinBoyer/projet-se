#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "bluetooth.h"
#include "time.h"
#include "led.h"
#include "draw_clock.h"
#include "draw.h"

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
  //SPI_MasterTransmit(0x01);
  sei();             // Allow interrupt
  ble_send_str("init ok\n");
}

/////

/*
*  Check if data have been saved into ble buffer
*  then update current index and send data to ble
*/
void echo_data_from_ble()
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


/////            Main              ////////


void main(){
  _delay_ms(500);
  global_init();

  //init_clock(); //nothing happens after
  //send_data(0xB3, 0xF0);
  
  while (1){
    
    send_data(0x00, 0x00);
    _delay_ms(200);
    send_data(0x00, 0x01);
    _delay_ms(200);
    send_data(0x00, 0x03);
    _delay_ms(200);
    send_data(0x00, 0x07);
    _delay_ms(200);
    send_data(0x00, 0x0F);
    _delay_ms(200);
    send_data(0x00, 0x1F);
    _delay_ms(200);
    send_data(0x00, 0x3F);
    _delay_ms(200);
    send_data(0x00, 0x7F);
    _delay_ms(200);
    

    send_data(0x00, 0xFF);
    _delay_ms(200);
    send_data(0x01, 0xFE);
    _delay_ms(200);
    send_data(0x03, 0xFC);
    _delay_ms(200);
    send_data(0x07, 0xF8);
    _delay_ms(200);
    send_data(0x0F, 0xF0);
    _delay_ms(200);
    send_data(0x1F, 0xE0);
    _delay_ms(200);
    send_data(0x3F, 0xC0);
    _delay_ms(200);
    send_data(0x7F, 0x80);
    _delay_ms(200);
    send_data(0xFF, 0x00);
    _delay_ms(200);

    send_data(0xFE, 0x00);
    _delay_ms(200);
    send_data(0xFC, 0x00);
    _delay_ms(200);
    send_data(0xF8, 0x00);
    _delay_ms(200);
    send_data(0xF0, 0x00);
    _delay_ms(200);
    send_data(0xE0, 0x00);
    _delay_ms(200);
    send_data(0xC0, 0x00);
    _delay_ms(200);
    send_data(0x80, 0x00);
    _delay_ms(200);
    send_data(0x00, 0x00);
    _delay_ms(200);
    
    // echo_data_from_ble();
    //SPI_MasterTransmit(0x0F);
    //_delay_ms(3000);
    //send_data(0xAA, 0xAA);
    // get_time_str(t_str);
    // ble_send_str( t_str );
    // ble_send_str( "\n" );
    //draw();
  }

}

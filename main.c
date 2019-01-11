#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "time.h"
#include "led.h"
#include "bluetooth.h"
#include "clockwise.h"
#include "monitor.h"
#include "draw.h"
#include "hello.h"


struct Time t = {22,15,0};                            // used to set time during init
static enum Mode{CLOCKWISE, BASIC, IMPROVED} mode;    // display modes


//////////////     Usefull functions          ////////////
/*
*   check if char carac is a number ('0' to '9')
*/
int verif_are_number(unsigned char chiffre1, unsigned char chiffre2 ,unsigned char chiffre3 , unsigned char chiffre4){
  if(chiffre1<48 || chiffre1>57){
    return 0;
  }
  if(chiffre2<48 || chiffre2>57){
    return 0;
  }
  if(chiffre3<48 || chiffre3>57){
    return 0;
  }
  if(chiffre4<48 || chiffre4>57){
    return 0;
  }
  return 1;
}

/*
*   compute integer from 2 char
*/
int digit_to_number(unsigned char chiffre_dizaine,unsigned char chiffre_unite){
    return 10*((int)chiffre_dizaine - 48)+((int)chiffre_unite - 48);
}



//////////////     Initialisation          ////////////
/*
*  call all initialisation functions required
*/
void global_init()
{
  bluetooth_init();         // init ble
  current_index_buff=0;
  char scurrent[10];
  sprintf(scurrent, "%d", current_index_buff);
  ble_send_str( scurrent );
  ble_send_char( ' ' );

  init_time(t);             // init timers and time counting (time.c)
  SPI_MasterInit();         // init SPI comm (led.c)
  init_monitor();
  init_hello();
  sei();                    // Allow interruptions

    // alert user that init is over
  ble_send_str("init ok\n");
  set_leds(0xFF, 0xFF);
  _delay_ms(250);
  set_leds(0x00, 0x00);
}




///// checking function  ///////
/*
*  Check if new data have been saved into ble buffer
*  then process data
*/
void check_ble()
{
  // first check if new data have been received
  if ( USART_buffer[0] == '\0' ){
    return;
  }

  ble_send_str( "new buff:" );
  ble_send_str( USART_buffer );
  ble_send_char( '\n' );

  if ( USART_buffer[0] == 'm'){       // code type "set mode"
    if ( USART_buffer[1] == '1') {
      ble_send_str( "set to Mode 1 \n" );
      mode = CLOCKWISE;
    }
    if ( USART_buffer[1] == '2') {
      ble_send_str( "set to Mode 2 \n" );
      mode = BASIC;
    }
    if ( USART_buffer[1] == '3') {
      ble_send_str( "set to Mode 3 \n" );
      mode = IMPROVED;
    }
  }

  if(USART_buffer[0] == 'h'){        // code type "set time"
    int i = 1;
    for(i=1;i<5;i++){                // check if received code is long enough ('h' + 4digits, ex: h1212)
      if(USART_buffer[i] == '\0'){
        ble_reset_buff();            // delete buff
        return;
      }
    }

    unsigned char chiffre1=USART_buffer[1];
    unsigned char chiffre2=USART_buffer[2];
    unsigned char chiffre3=USART_buffer[3];
    unsigned char chiffre4=USART_buffer[4];
    if ( !verif_are_number( chiffre1, chiffre2 , chiffre3 , chiffre4 ) ){
      ble_reset_buff();     // delete buff
      return;
    }

    int hours=digit_to_number(chiffre1,chiffre2);
    int minutes=digit_to_number(chiffre3,chiffre4);
    struct Time newtime = get_time();       // newtime = current time
    newtime.minutes = minutes > 59 ? newtime.minutes : minutes;     // set new minutes if relevant
    newtime.hours = hours > 23 ? newtime.hours : hours;             // set new hours if relevant
    set_time( newtime );
    ble_send_str( "time set\n" );
  }

    // debug code: send current time to ble
  if ( USART_buffer[0] == 't'){
    char timestr[20] = "";
    get_time_str(timestr);
    ble_send_str( "time : " );
    ble_send_str( timestr );
    ble_send_char( '\n' );
  }

  ble_reset_buff(); // burn after reading

}





/////            Main              ////////
void main(){
  mode = CLOCKWISE;    // set mode to print clockwises or numbers
  global_init();       // call all initialisations
  _delay_ms(1);
  char str[99];        // str store current time, needed for improved mode
  while (1){
    check_ble();       // check if new data have been received by ble
    if (mode == CLOCKWISE){
      draw_clockwise();
    } else if ( mode == BASIC){
      draw_hello();
    } else if ( mode == IMPROVED){
      get_time_str(str);
      update(str);
      draw();
    }
  }
}

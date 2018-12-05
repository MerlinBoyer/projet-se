#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "bluetooth.h"
#include "time.h"
#include "led.h"


//////       Initialisation          ////////

volatile int last_buffer_index;
struct Time t = {0,0,0};
char t_str[999];

void SPI_MasterInit(void)
{
    /* Set MOSI and SCK output, all others input */
    DDRB = (1 << DDB3) | (1 << DDB5);
    /* Enable SPI, Master, set clock rate fck/16 */
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void SPI_MasterTransmit(char cData)
{
    /* Start transmission */
    SPDR = cData;
    /* Wait for transmission complete */
    while (!(SPSR & (1 << SPIF)));
}

void global_init()
{
  bluetooth_init();
  init_time(t);
  last_buffer_index = current_index_buff;
  sei();
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
  global_init();
  while (1){
    echo_data_from_ble();

    // get_time_str(t_str);
    // ble_send_str( t_str );
    // ble_send_str( "\n" );

    _delay_ms(500);
  }

}

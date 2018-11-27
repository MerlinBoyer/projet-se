#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "bluetooth.h"
#include "time.h"

/*ISR(USART_RX_vect)
{
    //PORTB = PORTB | _BV(PB5);
    //int b = RXB80;
    unsigned char carac = UDR0;
    write_char(carac);
}
*/


//////       Initialisation          ////////


void init_interrupt()
{
    //__enable_interrupt();
    sei();
    //SREG |= (1<<_BV(SREG_I)); // Global interrupt enable
    //MCUCR = (1<<_BV(IVCE));
}

// void SPI_MasterInit(void)
// {
//     /* Set MOSI and SCK output, all others input */
//     DDRB = (1 << DDB3) | (1 << DDB5);
//     /* Enable SPI, Master, set clock rate fck/16 */
//     SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
// }

// void SPI_MasterTransmit(char cData)
// {
//     /* Start transmission */
//     SPDR = cData;
//     /* Wait for transmission complete */
//     while (!(SPSR & (1 << SPIF)));
// }



/////            Main              ////////


void main(){
  bluetooth_init();
  struct Time t = {0,0,0};
  init_time(t);
  char t_str[999];

  while (1){
    // get_time_str(t_str);
    
    ble_send_char( 0x40 );
    _delay_ms(1000);
  }

}

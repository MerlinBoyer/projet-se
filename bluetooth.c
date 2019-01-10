#define FOSC 13000000 // clk freq
//#define BAUD 38400
#define BAUD 38400
#include <avr/io.h>
#include <util/delay.h>
#include <util/setbaud.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "bluetooth.h"
#define MYUBRR FOSC/8/BAUD-1


// buffers are used to store data received
//static unsigned char USART_buffer[MAXBUFF];
unsigned char USART_buffer[MAXBUFF]= "UUUUUUUU";
int current_index_buff = 0;

///////////////////////   Setup Usart for Ble device com  /////////////////
void USART_init(unsigned int ubrr)
{
   // init buffer for reception
  current_index_buff = 0;
   /* Set baud rate */
  UBRR0H = (unsigned char)(ubrr >> 8);
  UBRR0L = (unsigned char)ubrr;
  /* Enable receiver and transmitter */
  UCSR0A = (1 << U2X0);
  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
  /* Set frame format: 8bits data */
  //UCSR0C = (3 << UCSZ00);
  UCSR0C = (3<<UCSZ00); //(1<<USBS0)|
}



////////////////    Send    //////////////////
/*
*  write a carac on usart
*/
static void USART_send_char(unsigned char c)
{
  while (!(UCSR0A & (1 << UDRE0)))
    ;
  UDR0 = c;
}


/*
*  write each string carac on USART
*/
static void USART_send_str(const unsigned char *str)
{
  while (*str != '\0')
  {
    USART_send_char(*str);
    str++;
  }
}


///////////////////    Reception    //////////////
/*
*  interrupt when usart reception is trigerred :
*  write carac into buffer and increment buffer index
*/
ISR(USART0_RX_vect)
{

  unsigned char c =  UDR0;

  if( c == '\r' || c=='\n' ){  // fin de la chaine
    USART_buffer[current_index_buff] = current_index_buff > 0 ? '\0' : USART_buffer[current_index_buff];
    current_index_buff = 0;
    return;
  }

  USART_buffer[current_index_buff] = c;

  current_index_buff++;
  if (current_index_buff >= MAXBUFF - 1)
  {
    current_index_buff = 0;
  }
  // ble_send_str( "carac:" );
  // ble_send_char( c );
  // ble_send_str( "chaine:" );
  // ble_send_str( USART_buffer );


  // USART_buffer[current_index_buff] = c;
  // char scurrent[10];
  // sprintf(scurrent, "%d", (char)current_index_buff);
  // ble_send_str( scurrent );
  // ble_send_char( ' ');

  // ble_send_str("Ble interrupt received : ");
}

void ble_reset_buff(){
  USART_buffer[0] = '\0';
}



//////////////    Ble functions      ////////////
void bluetooth_init()
{
  USART_init(MYUBRR);
}

void ble_send_char(unsigned char c)
{
  USART_send_char(c);
}

void ble_send_str(unsigned char *str)
{
  USART_send_str(str);
}

void ble_send_int( int x ){
  char str[15] = {0};
  sprintf( str, "%d", x);
  USART_send_str(str);
  USART_send_char(' ');
}

/////////////////////////////////////////////////

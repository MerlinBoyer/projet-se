#define FOSC 13000000 // clk freq
#define BAUD 38400
#include <avr/io.h>
#include <util/delay.h>
#include <util/setbaud.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "bluetooth.h"
#define MYUBRR FOSC/8/BAUD-1


// buffers are used to store data received
unsigned char USART_buffer[MAXBUFF]= "INITIALIZED";
int current_index_buff = 0;

///////////////////////   Setup Usart for Ble device com  /////////////////
void USART_init(unsigned int ubrr)
{
    
  current_index_buff = 0;   // init buffer for reception
    /* Set baud rate */
  UBRR0H = (unsigned char)(ubrr >> 8);
  UBRR0L = (unsigned char)ubrr;
    /* Enable receiver and transmitter */
  UCSR0A = (1 << U2X0);
  UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
    /* Set frame format: 8bits data */
  UCSR0C = (3<<UCSZ00);
}


////////////////    Send functions    //////////////////
/*
*  write a carac on usart
*/
static void USART_send_char(unsigned char c)
{
  while (!(UCSR0A & (1 << UDRE0)))  // wait for TX not busy before writing carac
    ;
  UDR0 = c;
}


/*
*  write each string character on USART
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
*  write carac into buffer
*/
ISR(USART0_RX_vect)
{

  unsigned char c =  UDR0;       // read char received

  if( c == '\r' || c=='\n' ){    // end of string, reset buffer index and do not store special characters
    USART_buffer[current_index_buff] = current_index_buff > 0 ? '\0' : USART_buffer[current_index_buff];
    current_index_buff = 0;
    return;
  }

  USART_buffer[current_index_buff] = c;    // store char received into buffer

  current_index_buff++;
  if (current_index_buff >= MAXBUFF - 1)
  {
    current_index_buff = 0;
  }
}

/*
*   Empty buffer
*/
void ble_reset_buff()
{
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

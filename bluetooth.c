#define FOSC 13000000 // clk freq
#define BAUD 38400
#include <avr/io.h>
#include <util/delay.h>
#include <util/setbaud.h>
#include <avr/interrupt.h>
#include "bluetooth.h"
#define MYUBRR FOSC / 8 / BAUD - 1

/////   Setup Usart for Ble device com  //////

void USART_init(unsigned int ubrr)
{
  // init buffer for reception
  current_index_buff = 0;
  /* Set baud rate */
  UBRR0H = (unsigned char)(ubrr >> 8);
  UBRR0L = (unsigned char)ubrr;
  /* Enable receiver and transmitter */
  UCSR0A |= (1 << U2X);
  UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | _BV(RXCIE0);
  /* Set frame format: 8bits data */
  UCSR0C = (3 << UCSZ00);
}

/// Send  ///

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
static void USART_send_str(const char *str)
{
  while (*str != 0)
  {
    USART_send_char(*str);
    str++;
  }
}

///  Receive  ///

/*
*  read str on usart (synchronous)
*/
static void USART_get_data(char *out)
{
  int i = 0;
  char c;
  do
  {
    c = USART_get_char();
    out[++i] = c;
  } while (c);
}


/*
*  interrupt on usart receive :
*  write carac into buffer and increment buffer index
*/
ISR(USART0_RX_vect)
{
  //ble_send_str("Ble interrupt received ");
  current_index_buff++;
  if (current_index_buff >= MAXBUFF)
  {
    current_index_buff = 0;
  }
  USART_buffer[current_index_buff] = UDR0;
}

////////    Ble functions      ////////////

// void ble_get_data(char *str)
// {
//   USART_get_data(str);
// }

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

/////////////////////////////////////////////////
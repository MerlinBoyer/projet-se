#define FOSC 13000000 // clk freq
#define BAUD 38400
#include <avr/io.h>
#include <util/delay.h>
#include <util/setbaud.h>
#define MAXBUF 999
#define MYUBRR FOSC / 8 / BAUD - 1

/////   Setup Usart for Ble device com  //////

void USART_init(unsigned int ubrr)
{
  /* Set baud rate */
  UBRR0H = (unsigned char)(ubrr >> 8);
  UBRR0L = (unsigned char)ubrr;
  /* Enable receiver and transmitter */
  UCSR0A = (1 << U2X);
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);
  /* Set frame format: 8data, 2stop bit */
  UCSR0C = (3 << UCSZ00);
}

static void USART_send_char(unsigned char c)
{
  while (!(UCSR0A & (1 << UDRE0)))
    ;
  UDR0 = c;
}

static unsigned char USART_get_char(void)
{
  /* Wait for data to be received */
  while (!(UCSR0A & (1 << RXC0)))
    ;
  /* Get and return received data from buffer */
  return UDR0;
}

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

static void USART_send_str(const char *str)
{
  while (*str != 0)
  {
    USART_send_char(*str);
    str++;
  }
}

////////    Ble functions      ////////////

void ble_get_data(char *str)
{
  USART_get_data(str);
}

void bluetooth_init()
{
  USART_init( MYUBRR );
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
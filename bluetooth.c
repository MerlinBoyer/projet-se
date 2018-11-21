#define BAUD 9600
#include <avr/io.h>
#include <util/delay.h>
#include<util/setbaud.h>
#define MAXBUF 999
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

void USART_Transmit( unsigned char data ){
  while ( !( UCSR0A & (1<<UDRE0)) );
  UDR0 = data;  
}

static unsigned char USART_Receive( void ){
  /* Wait for data to be received */
  while (!(UCSR0A & (1<<RXC0)));
  /* Get and return received data from buffer */
  return UDR0;
}

static void putchar_uart(unsigned char c){
  USART_Transmit(c);
}

static void printf_uart(unsigned char * str){
  while (*str != 0){
    putchar_uart(*str);
    str++;
  }
}

void USART_Init(){
  /*Set baud rate */
  UBRR0H = UBRRH_VALUE; // high value
  UBRR0L = UBRRL_VALUE;  // low value
  UCSR0A = 0;
  UCSR0B = (1<<RXEN0) | (1<<TXEN0); // as writer and reader
  UCSR0C = (3<<UCSZ00); // 8 bit for caracter size
}


static void scanf_uart(char * out){
  int i = 0;
  char c;
  do {
    c = USART_Receive();
    out[++i] = c;
  } while (c);
}

void scanf_bluetooth(char * out){
  scanf_uart(out);
}

void bluetooth_Init(){
  USART_Init();
}

void printf_bluetooth(unsigned char * str){
  printf_uart(str);
}

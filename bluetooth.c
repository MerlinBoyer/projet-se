#define BAUD 9600
#include <avr/io.h>
#include <util/delay.h>
#include<util/setbaud.h>
#define MAXBUF 999
#define FOSC 1843200 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

static void USART_Transmit( unsigned char data ){
  while ( !( UCSRA & (1<<UDRE)) );
  UDR = data;  
}

static unsigned char USART_Receive( void ){
  /* Wait for data to be received */
  while (!(UCSRA & (1<<RXC0)));
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

static void USART_Init(){
  /*Set baud rate */
  UBRRH = (unsigned char)(MYUBRR>>8); // high value
  UBRRL = (unsigned char)MYUBRR; // low value
  UCSRB = (1<<RXEN) | (1<<TXEN); // as writer and reader
  UCSRC = (3<<UCSZ0); // 8 bit for caracter size
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

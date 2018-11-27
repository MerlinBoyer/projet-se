#define BAUD 115200
#include <avr/io.h>
#include <util/delay.h>
#include <util/setbaud.h>
#define MAXBUF 999
#define MYUBRR F_CPU/16/BAUD-1


/////   Setup Usart for Ble device com  //////

void USART_init(){
  /*Set baud rate */
  UBRR0H = UBRRH_VALUE; // high value
  UBRR0L = UBRRL_VALUE;  // low value
  UCSR0A = 0;
  UCSR0B = (1<<RXEN0) | (1<<TXEN0); // as writer and reader
  UCSR0C = (3<<UCSZ00); // 8 bit for caracter size
}

// void USART_init(){
//     UBRR0H = (unsigned char)0x00;
//     UBRR0L = (unsigned char)0x10; //baudrate at 115200

//     UCSR0A = (_BV(U2X0));
//     UCSR0B = (_BV(RXEN0)) | (_BV(TXEN0)) | (_BV(RXCIE0)); //enable RX and Tx
//     UCSR0C = (3 << (UCSZ00));                             //8bit char
// }


static void USART_send_char( unsigned char c ){
  while ( !( UCSR0A & (1<<UDRE0)) );
  UDR0 = 'r';
  while ((UCSR0A & (1 << TXC0)) == 0x00);
}

static unsigned char USART_get_char( void ){
  /* Wait for data to be received */
  while (!(UCSR0A & (1<<RXC0)));
  /* Get and return received data from buffer */
  return UDR0;
}

static void USART_get_data(char * out){
  int i = 0;
  char c;
  do {
    c = USART_get_char();
    out[++i] = c;
  } while (c);
}

static void USART_send_str(const char * str){
  while (*str != 0){
    USART_send_char(*str);
    str++;
  }
}


////////    Ble functions      ////////////


void ble_get_data(char * str){
  USART_get_data(str);
}

void bluetooth_init(){
  USART_init();
}

void ble_send_char(const char c){
  USART_send_char( c );
}

void ble_send_str(const char * str){
  USART_send_str( str );
}

/////////////////////////////////////////////////
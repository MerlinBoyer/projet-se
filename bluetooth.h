#ifndef BLUETOOTH_H
#define BLUETOOTH_H
#define MAXBUFF 99

// buffers are used to store data received
extern unsigned char USART_buffer[MAXBUFF];
extern int current_index_buff;


/*
  Setup Usart for Ble device com
*/
void USART_init(unsigned int ubrr);

/*
  Write string on usart
*/
static void USART_send_str(const unsigned char *str);

/*
  Write character on usart
*/
static void USART_send_char(unsigned char c);


/*
  Init the communication
*/
void bluetooth_init();

/*
  convert int to str and send it with bluetooth
*/
void ble_send_int( int x );

/*
  Send str with bluetooth
*/
void ble_send_str(unsigned char *str);

/*
  Send char with bluetooth
*/
void ble_send_char(unsigned char c);

/*
   free buffers
*/
void ble_reset_buff();

#endif

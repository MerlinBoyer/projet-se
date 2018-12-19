#ifndef BLUETOOTH_H
#define BLUETOOTH_H
#define MAXBUFF 999


 // buffers are used to store data received  
volatile unsigned char USART_buffer[MAXBUFF];
volatile int current_index_buff;


/*
  convert int to str and send it with bluetooth
 */
void ble_send_str_from_int( int x );

/*
  Send str with bluetooth
 */
void ble_send_str(unsigned char *str);

/*
  Send char with bluetooth
 */
void ble_send_char(unsigned char c);

/*
Init the communication
 */
void bluetooth_init();

#endif

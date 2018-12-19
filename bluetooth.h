#ifndef BLUETOOTH_H
#define BLUETOOTH_H
#define MAXBUFF 999

volatile unsigned char USART_buffer[MAXBUFF];
volatile int current_index_buff;

void ble_send_str_from_int( int x );
void ble_send_str(unsigned char *str);
/*
  Send string with bluetooth
 */
void ble_send_char(unsigned char c);
/*
Init the communication
 */
void bluetooth_init();

#endif

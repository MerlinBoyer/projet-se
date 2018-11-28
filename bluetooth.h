#ifndef BLUETOOTH_H
#define BLUETOOTH_H
#define MAXBUFF 999

unsigned char USART_buffer[MAXBUFF];
int current_index_buff;

/*
  Wait for input from bluetooth
 */
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

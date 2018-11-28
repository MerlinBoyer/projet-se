#ifndef BLUETOOTH_H
#define BLUETOOTH_H
#define MAXBUFF 999

unsigned char USART_buffer[MAXBUFF];
int current_index_buff;

/*
  Wait for input from bluetooth
 */
void scanf_bluetooth(char * out);
/*
  Send string with bluetooth
 */
void printf_bluetooth(unsigned char * str);
/*
Init the communication
 */
void bluetooth_Init();

#endif

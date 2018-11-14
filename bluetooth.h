#ifndef BLUETOOTH_H
#define BLUETOOTH_H


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

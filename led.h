#ifndef LED_H
#define LED_H





void SPI_MasterInit(void);

void SPI_MasterTransmit(uint8_t cData);

void send_data(uint8_t octet1, uint8_t octet2);

void set_leds(uint8_t octet1, uint8_t octet2);

#endif

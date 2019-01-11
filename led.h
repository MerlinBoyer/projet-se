#ifndef LED_H
#define LED_H

/*
*   Set OE and LE pin, needed to send data by SPI
*/
void set_OE(int n);
void set_LE(int n);

/*
*   Setup SPI comm
*/
void SPI_MasterInit(void);

/*
*   Send data into SPI
*/
void SPI_MasterTransmit(uint8_t cData);

/*
*   Write leds values : octet1 set top leds, octet2 set bottom leds
*/
void set_leds(uint8_t octet1, uint8_t octet2);

void all_leds_ON(void);

void all_leds_OFF(void);

#endif

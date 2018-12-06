#include <avr/io.h>
#include "led.h"
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "bluetooth.h"

void SPI_MasterInit(void){    
    /* Set /OE and LE as outputs*/
    DDRE |= (1 << DDE4) | (1 << DDE5);
    /* Set /OE to 0 : set outputs activated */
    // PORTE &= ~(1 << DDE4);
    //PORTE |= (1 << DDE4);
   // SPSR = 0x01;
   /* Set MOSI and SCK output, all others input */
    DDRB = (1 << DDB2) | (1 << DDB1) | (1 << DDB0);
    /* Enable SPI, Master, set clock rate fck/4 */
    SPCR = (1 << SPE) | (1 << MSTR);
    // /OE
    PORTE |= (1 << PORTE4);
    // Latch
    PORTE &= ~(1 << PORTE5);
}


void SPI_MasterTransmit(uint8_t cData)
{    
    PORTB &= ~(1 << PORTB0);

    //PORTE &= ~(1 << PORTE4);
    SPDR = cData;
    /* Wait for transmission complete */
    ble_send_str("avant\n");
    while (!(SPSR & (1 << SPIF)))
        ;
    ble_send_str("apres\n");

    PORTB |= (1 << PORTB4);
    //_delay_ms(1);
    
    /* Set output desactivated */
    // PORTE |= (1 << DDE4);
}


void set_OE(int n){
    if (n){
        PORTE |= (1 << PORTE4);
        ble_send_str("OE 1\n");
    }else {
        PORTE &= ~(1 << PORTE4);
        ble_send_str("OE 0\n");
    }
}

void set_LE(int n){
    if (n){
        PORTE |= (1 << PORTE5);
    }else {
        PORTE &= ~(1 << PORTE5);
    }
}

void send_data(uint8_t octet1, uint8_t octet2){
    /* Start transmission */

    // Set /OE to 1
    set_OE(1);

    // transmit data
    SPI_MasterTransmit(octet1);
    _delay_us(1);
    SPI_MasterTransmit(octet2);
    _delay_us(1);

    /* Set LE to 1 then 0 (latch data on driver)*/
    set_LE(1);
    _delay_us(1);
    set_LE(0);
    _delay_us(1);

    /* Set output activated */
    set_OE(0);

}

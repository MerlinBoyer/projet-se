#include <avr/io.h>
#include "led.h"
#include <stdio.h>
#include <avr/interrupt.h>

void SPI_MasterInit(void)
{
    /* Set MOSI and SCK output, all others input */
    DDRB = (1 << DDB3) | (1 << DDB5) | (1 << DDB2);
    /* Enable SPI, Master, set clock rate fck/2 */
    SPCR = (1 << SPE) | (1 << MSTR);
    SPSR = 0x01;
}

void SPI_MasterTransmit(char cData)
{
    /* Start transmission */
    SPDR = cData;
    /* Wait for transmission complete */
    while (!(SPSR & (1 << SPIF)));
}

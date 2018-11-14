#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


void write_char(char c){
    //UCSR0B |= (0<<_BV(RXEN0))|(1<<_BV(TXEN0));
    UCSR0B |= (1<<TXB80);
    UDR0 = c;
    while((UCSR0A & (1<<TXC0)) == 0x00);
    //UCSR0B |= (1<<_BV(RXEN0))|(0<<_BV(TXEN0));
}

ISR(USART_RX_vect)
{ 
    PORTB = PORTB | _BV(PB5);
    int b = RXB80;
    char carac = UDR0;
    write_char(carac);
}

void init_interrupt(){
    //__enable_interrupt();
    SREG |= (1<<_BV(SREG_I)); // Global interrupt enable
    MCUCR = (1<<_BV(IVCE));
}

void UART_init(){
    UBRR0H = 0x00;
    UBRR0L = 0x1F; //baudrate at 115200
    UCSR0A |= (1<<_BV(U2X0));

    UCSR0B = (1<<_BV(RXCIE0))|(1<<_BV(RXEN0)); //interrupt enable
    UCSR0C = (3<<_BV(UCSZ00)); //8bit char
    UCSR0B |= (1<<_BV(RXEN0))|(1<<_BV(TXEN0));
}

void main() {
    // Active et allume la broche PB5 (led)
    init_interrupt();
    UART_init();
    int led = 0;
    DDRB |= (1<<PB5);
    PORTB |= (1<<PB5);
    while(1){
        // _delay_ms(1000);
        // PORTB = PORTB | _BV(PB5);
        //  _delay_ms(1000);
        // PORTB = PORTB & !_BV(PB5);
    }
   
}
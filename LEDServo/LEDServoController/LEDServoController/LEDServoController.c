/*
 * LEDServoController.c
 *
 * Created: 4/9/2014 4:08:45 PM
 *  Author: Nishant Pol
 */ 


#include <avr/io.h>

/* 9600baud at 1MHz clk */
#define BAUD 12
#define MULT 1

/* Pinout */
#define DIAG PIND6

inline static uint8_t receive_data(void);

int main(void)
{
    /* Setup DIAG LED */
    DDRD |= (1<<DIAG);
    PORTD = (0<<DIAG);
    /* Setup UART for 8data, */
    UBRRH = (uint8_t)(BAUD >> 8);
    UBRRL = (uint8_t)BAUD;
    UCSRA = (MULT<<U2X);
    UCSRB = (1<<RXEN)|(1<<TXEN);
    UCSRC = (3<<UCSZ0);
    uint8_t LedR = 0;
    uint8_t LedG = 0;
    uint8_t LedB = 0;
    uint8_t Servo = 127;
    /* Initialize LEDs and Servo */
    
    while(1)
    {
        uint8_t data_in = 0;
        while(!data_in) data_in = receive_data();
        PORTD |= (1<<DIAG);        
        LedR = data_in;
        LedG = receive_data();
        LedB = receive_data();
        Servo = receive_data();
        OCR0A = LedR;
        OCR0B = Servo;
        OCR1A = LedB;
        OCR1B = LedG;
        PORTD &= ~(1<<DIAG);
    }
}

inline static uint8_t receive_data(void){
    while(!(UCSRA & (1<<RXC)));
    return UDR;    
}
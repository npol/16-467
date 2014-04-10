/*
 * LEDServoController.c
 *
 * Created: 4/9/2014 4:08:45 PM
 *  Author: Nishant Pol
 */ 
#define F_CPU 1000000

#include <avr/io.h>
#include <util/delay.h>

/* 9600baud at 1MHz clk */
#define BAUD 12
#define MULT 1

/* Pinout */
#define DIAG PIND6
#define LEDR PINB2
#define LEDG PIND5
#define LEDB PINB4
#define SERVO PINB3

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
    uint8_t LedR = 127;
    uint8_t LedG = 127;
    uint8_t LedB = 127;
    uint8_t Servo = 127;
    /* Initialize LEDs and Servo */
    DDRD |= (1<<LEDG);
    DDRB |= (1<<PINB2)|(1<<PINB3)|(1<<PINB4);
    ICR1 = 2550;
    TCCR1A = (1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);
    TCCR1B = (1<<WGM13)|(1<<WGM12)|(1<<CS11);
    
    TCCR0A = (1<<WGM01)|(1<<WGM00)|(1<<COM0B1)|(1<<COM0B0)|(1<<COM0A0)|(1<<COM0A1);
    TCCR0B = (1<<CS00)|(1<<CS01)|(1<<WGM01);
    
    while(1)
    {
        //uint8_t data_in = 0;
        //while(!data_in) data_in = receive_data();
        PORTD |= (1<<DIAG);        
        //LedR = data_in;
        //LedG = receive_data();
        //LedB = receive_data();
        //Servo = receive_data();
        OCR0A = 0x80;
        OCR0B = 0xc0;//reversed
        OCR1A = 0x950;//0x0 to 0xa00
        OCR1B = 0xc0;//0x100 is 2ms, 0x140 is 2.5ms, 0xc0 is 1.5ms
        PORTD &= ~(1<<DIAG);
        (void)Servo;
        (void)LedB;
        (void)LedR;
        (void)LedG;
        
    }
}

inline static uint8_t receive_data(void){
    while(!(UCSRA & (1<<RXC)));
    return UDR;    
}
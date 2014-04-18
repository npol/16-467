/*
 * SerialServo.c
 *
 * Created: 4/12/2014 4:34:36 PM
 *  Author: Nishant Pol
 */ 
#define F_CPU 1000000

#include <avr/io.h>
#include <util/delay.h>

/* 9600baud at 1MHz clk */
#define BAUD 12
#define MULT 1
#define ADDR 0x20

/* Pinout */
#define DIAG PIND6
#define SERVO2 PINB2  //OC0A
#define SERVO1 PIND5  //OC0B
#define SERVO3 PINB4  //OC1B
#define SERVO4 PINB3  //OC1A

inline static uint8_t receive_data(void);
inline static void transmit_data(uint8_t data);

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
    uint8_t Servo1 = 127;
    uint8_t Servo2 = 127;
    uint8_t Servo3 = 127;
    uint8_t Servo4 = 127;
    /* Initialize LEDs and Servo */
    DDRD |= (1<<SERVO1);
    DDRB |= (1<<SERVO2)|(1<<SERVO3)|(1<<SERVO4);
    ICR1 = 2550;//50Hz PWM
    TCCR1A = (1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);
    TCCR1B = (1<<WGM13)|(1<<WGM12)|(1<<CS11);
    
    TCCR0A = (1<<WGM01)|(1<<WGM00)|(1<<COM0B1)|(1<<COM0B0)|(1<<COM0A0)|(1<<COM0A1);
    TCCR0B = (1<<CS00)|(1<<CS01)|(1<<WGM01);
    OCR0A = 0xff - 0x06;
    OCR0B = 0xff - 0x06;
    OCR1B = (uint16_t)(0x30) << 2;
    OCR1A = (uint16_t)(0x30) << 2;
    
    while(1)
    {
        uint8_t data_in = 0;
        while(data_in != ADDR) data_in = receive_data();
        PORTD |= (1<<DIAG);        
        Servo1 = receive_data();
        Servo2 = receive_data();
        Servo3 = receive_data();
        Servo4 = receive_data();
        
        //Servo 1
        OCR0B = 0xff - Servo1;
        //Servo 2
        OCR0A = 0xff - Servo2;
        //Servo 3
        OCR1B = (uint16_t)(Servo4) << 2;
        //Servo 4
        OCR1A = (uint16_t)(Servo3) << 2;
        
        //OCR0A = 0x80;
        //OCR0B = 0xc0;//reversed
        //OCR1A = 0x950;//0x0 to 0xa00
        //OCR1B = 0xc0;//0x100 is 2ms, 0x140 is 2.5ms, 0xc0 is 1.5ms
        PORTD &= ~(1<<DIAG);
        transmit_data(0x55);
    }
}

inline static uint8_t receive_data(void){
    while(!(UCSRA & (1<<RXC)));
    return UDR;
}

inline static void transmit_data(uint8_t data){
    while (!(UCSRA & (1<<UDRE)));
    UDR = data;
    return;
}

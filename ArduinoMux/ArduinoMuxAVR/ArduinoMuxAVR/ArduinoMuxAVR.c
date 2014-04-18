/*
 * ArduinoMuxAVR.c
 *
 * Created: 4/15/2014 9:39:06 AM
 *  Author: Nishant Pol
 */ 

#include <avr/io.h>
#define DIAG PINB7
//Motor Shield Channel A
#define panC PING5
#define panD PINE3
#define panPWM PINE5//OC3C

#define LED_ADDR 0x10
#define SERVO_ADDR 0x20
#define PAN_ADDR 0x30

#define BAUD 103
#define MULT 0

inline static uint8_t rx_comp(void);
inline static void tx_comp(uint8_t data);
inline static uint8_t rx_LED(void);
inline static void tx_LED(uint8_t data);
inline static uint8_t rx_servo(void);
inline static void tx_servo(uint8_t data);

int main(void)
{
    /* Setup DIAG LED */
    DDRB |= (1<<DIAG);
    PORTB = (0<<DIAG);
    /* Setup Serial Port 0 For Computer 9600baud */
    UBRR0H = (uint8_t)(BAUD >> 8);
    UBRR0L = (uint8_t)(BAUD);
    UCSR0A = (MULT<<U2X0);
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    UCSR0C = (3<<UCSZ00);
    /* Setup Serial Port 1 for LEDController */
    UBRR1H = (uint8_t)(BAUD >> 8);
    UBRR1L = (uint8_t)BAUD;
    UCSR1A = (MULT<<U2X1);
    UCSR1B = (1<<RXEN1)|(1<<TXEN1);
    UCSR1C = (3<<UCSZ10);
    /* Setup Serial Port 2 for Servo Controller */
    UBRR2H = (uint8_t)(BAUD >> 8);
    UBRR2L = (uint8_t)BAUD;
    UCSR2A = (MULT<<U2X2);
    UCSR2B = (1<<RXEN2)|(1<<TXEN2);
    UCSR2C = (3<<UCSZ20);
    /* Setup Motor Shield pins */
    //Phase and Frequency Correct PWM
    //TCCR3A = (1<<COM3C1)|(1<<COM3C0)|(1<<WGM31)|(1<<WGM30);
    //TCCR3B = (0<<CS32)|(1<<CS31)|(0<<CS30)|(1<<WGM33)|(1<<WGM32);
    //ICR3 = 0x100;
    //OCR3C = 0x80;
    /*
    TCCR1A = (1<<COM1A1)|(1<<COM1A0)|(0<<WGM11)|(1<<WGM10);
    TCCR1B = (1<<CS12)|(1<<CS11)|(0<<CS10)|(1<<WGM13)|(0<<WGM12);
    OCR1A = 0x80;
    */
    //Working below
    /*
    DDRB |= (1<<PINB5);
    ICR1 = 2000;
    OCR1A = 1500;
    TCCR1A = (1<<COM1A1);
    TCCR1B = (1<<WGM13)|(1<<CS11);
    */
    //PWM on D3 or PE5 or OC3C
    DDRE |= (1<<panD)|(1<<panPWM);
    DDRG |= (1<<panC);
    ICR3 = 1000;//1ms period
    OCR3C = 0x00;
    TCCR3A = (1<<COM3C1);
    TCCR3B = (1<<WGM33)|(1<<CS31);
    DDRE |= (1<<panD);
    DDRG |= (1<<panC);
    while(1)
    {
        /* Wait for computer data */
        uint8_t data_in = 0;
        while(data_in != 0x80) data_in = rx_comp();
        PORTB |= (1<<DIAG);
        uint8_t addr = rx_comp();
        uint8_t data0 = rx_comp();
        uint8_t data1 = rx_comp();
        uint8_t data2 = rx_comp();
        uint8_t data3 = rx_comp();
        tx_comp(0x33);//ACK byte
        
        /* Send data to device */
        if(addr == LED_ADDR){
            tx_LED(addr);
            tx_LED(data0);
            tx_LED(data1);
            tx_LED(data2);
            tx_LED(data3);
        } else if(addr == SERVO_ADDR){
            tx_servo(addr);
            tx_servo(data0);
            tx_servo(data1);
            tx_servo(data2);
            tx_servo(data3);            
        } else if(addr == PAN_ADDR) {
            if(data0){//Enable byte
                int8_t speed;
                if((int8_t)data1 < 0){
                    PORTG |= (1<<panC);
                    PORTE &= ~(1<<panD);
                    speed = -(int8_t)data1;
                } else {
                    PORTG &= ~(1<<panC);
                    PORTE |= (1<<panD);
                    speed = (int8_t)data1;
                }
                OCR3CL = speed;
            } else {
                PORTG &= ~(1<<panC);
                PORTE &= ~(1<<panD);
            }
        }
        PORTB &= ~(1<<DIAG);
    }
}

inline static uint8_t rx_comp(void){
    while(!(UCSR0A & (1<<RXC0)));
    return UDR0;
}

inline static void tx_comp(uint8_t data){
    while ( !( UCSR0A & (1<<UDRE0)) );
    UDR0 = data;
    return;
}

inline static uint8_t rx_LED(void){
    while(!(UCSR1A & (1<<RXC1)));
    return UDR1;
}

inline static void tx_LED(uint8_t data){
    while ( !( UCSR1A & (1<<UDRE1)) );
    UDR1 = data;
    return;
}

inline static uint8_t rx_servo(void){
    while(!(UCSR2A & (1<<RXC2)));
    return UDR2;
}

inline static void tx_servo(uint8_t data){
    while ( !( UCSR2A & (1<<UDRE2)) );
    UDR2 = data;
    return;
}

/*
 * ArduinoMuxAVR.c
 *
 * Created: 4/15/2014 9:39:06 AM
 *  Author: Nishant Pol
 */ 

#include <avr/io.h>
#define DIAG PINB7
//Motor Shield Channel A
#define panDir PINB6
#define panPWM PINE5//OC3C
#define panBrake PINH6

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
    TCCR3A = (0<<COM3C1)|(1<<COM3C0)|(0<<WGM31)|(1<<WGM30);
    TCCR3B = (0<<CS32)|(1<<CS31)|(0<<CS30)|(0<<WGM33)|(1<<WGM32);
    //TCCR3C = (1<<FOC3C);
    OCR3CL = 0x10;
    DDRB |= (1<<panDir)|(1<<panBrake);
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
                    PORTB |= (1<<panDir);
                    speed = -(int8_t)data1;
                } else {
                    PORTB &= ~(1<<panDir);
                    speed = (int8_t)data1;
                }
                (void)speed;
                OCR3CL = 0x10;
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

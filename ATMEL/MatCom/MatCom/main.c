/*
 * GccApplication2.c
 *
 * Created: 28.10.2015 18:34:49
 * Author : Marco
 */

#define BAUD 9600 
#define F_CPU 14745600

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "../../../lib/bmp085.h"
#include "../../../lib/uart.h"
#include "../../../lib/MPU6050.h"
#include "../../../lib/TC1.h"
//#include "../../../lib/twimaster.h"


int main(void)
{
	// initializing uart data variables for storage of incoming data
// 	signed char uart_data_length = -1;
// 	char char1 = '@';
// 	int innt = 12412;
// 	long long1 = -123456789;
// 	float float1 = 3.1415926;
// 	double double1 = -2.81;
// 	
	
    uart_init( UART_BAUD_SELECT(BAUD,F_CPU) );
 	TC1_Setup();
	sei();
	uart_putc(2);
 	mpu6050_setup();
	uart_putc(3);
		
// 	DDRD  = 0b11111011;   // set PD2 to input
// 	PORTD = 0b00000100;   // set PD2 to high
// 	
// 	EICRA = 0b00000011;
// 	EIMSK = 0b00000001;	
// 	PCICR |= _BV(PCIE0);   //Enable PCINT2
// 	
// 	PCMSK0 |= (1 << PCINT0); /* Enable PCINT0 */

	DDRB |= 1 << PINB0;
    while (1) 
    {
		PORTB ^= 1 << PINB0;
		_delay_ms(500);
    }
}


ISR(INT0_vect)
{
	_delay_ms(40);
uart_putc(1);
}

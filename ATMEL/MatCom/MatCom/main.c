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
//#include "../../../lib/twimaster.h"

int main(void)
{
	// initializing uart data variables for storage of incoming data
	signed char uart_data_length = -1;
	char char1 = '@';
	int innt = 12412;
	long long1 = -123456789;
	float float1 = 3.1415926;
	double double1 = -2.81;
	
    uart_init( UART_BAUD_SELECT(BAUD,F_CPU) );
	sei();
    while (1) 
    {
		uart_putData("cf",&char1,&float1);	
	    _delay_ms(500);
    }
}


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
	char uart_data_array[30] = {0};
	
    uart_init( UART_BAUD_SELECT(BAUD,F_CPU) );
	sei();
    while (1) 
    {
		uart_getData(uart_data_array, &uart_data_length);
		if(uart_data_length != -1 && uart_data_length != 0)
		{
			uart_putData(uart_data_array, &uart_data_length);	
		}
	    //_delay_ms(500);
	   /* uart_putc('b');
	    _delay_ms(500);*/
    }
}


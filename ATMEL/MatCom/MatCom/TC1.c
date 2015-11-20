// #include <avr/io.h>
// #include <stdarg.h>
// #include <stdio.h>
// #include <string.h>
// #include <avr/interrupt.h>
// #include <avr/common.h> // needed for SREG since it is not defined in io.h for some stupid reason...
// #include <avr/pgmspace.h>
// #include "uart.h"
#include "TC1.h"

// 16-bit Timer/Counter unit TC1
#define TIMER_BOTTOM 0x0000
#define TIMER_MAX 0xFFFF

/****************************************************************************
| ISR for incoming MPU data													|
| (at a later stage a more general approach should be considered)			|
|---------------------------------------------------------------------------|
|NOTES:																		|
| PRTIM1 bit in the Power Reduction Register 0 must be written to zero to	|
|  enable Timer/Counter1 module, e.g.: PRR0 |= 0 << PRTIM1;					|									|
| Set Input Capture Interrupt Enable for IC1 e.g.: TIMSK1 |= 1 << ICIE1;	|
****************************************************************************/

/*
The Input Capture Register can capture the Timer/Counter value at a given 
external (edge triggered) event on either the Input Capture pin (ICPn) or on
the Analog Comparator pins (See Section “20.” on page 237.) The Input 
Capture unit includes a digital filtering unit (Noise Canceler) for reducing
the chance of capturing noise spikes.
*/

// Data Storage for Input Capture 1 Timestamps
static volatile unsigned int IC1TS;
static volatile unsigned int IC1TS_OLD;


ISR (TIMER1_CAPT_vect)
{
	int i, xData, yData, zData;
	unsigned char sreg;
	/* Save global interrupt flag */
	sreg = SREG;
	/* Disable interrupts to ensure safe operation*/
	cli();
	// Read IC1 Timestamp (2 cycle-operation)
	IC1TS=ICR1;
	/* Restore global interrupt flag */
	SREG = sreg;
	
	if(IC1TS < IC1TS_OLD) // alternatively check overflow buffer...
	{
		//TODO: Overflow handling Routine
	}
	//TODO: Get MPU data
    mpu6050_readAccelerometerDataRaw(&xData, &yData, &zData);
	i=TC1_Read();
	//TODO: Send MPU data and timestamp via UART
	uart_putData("iiii",&i,&xData,&yData,&zData);
}

void TC1_Setup ( void )
{
// Setting TC1 Control Register A -
// TCCR1A |=
// Setting TC1 Control Register B - Clock Select, prescaler
TCCR1B	|= (1 << ICNC1 | 1 << ICES1 | 1 << CS10 | 0 << CS11 | 1 << CS12 )
}

unsigned int TC1_Read( void )
{
	unsigned char sreg;
	unsigned int i;
	/* Save global interrupt flag */
	sreg = SREG;
	/* Disable interrupts */
	cli();
	/* Read TCNT1 into i (2-cycle operation) */
	i = TCNT1;
	/* Restore global interrupt flag */
	SREG = sreg;
	return i;
}
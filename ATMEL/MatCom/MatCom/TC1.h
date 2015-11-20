#ifndef TC1_H
#define TC1_H

#include <avr/io.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <avr/interrupt.h>
#include <avr/common.h> // needed for SREG since it is not defined in io.h for some stupid reason...
#include <avr/pgmspace.h>
#include "uart.h"

void TC1_Setup (void );

unsigned int TC1_Read( void );

#endif
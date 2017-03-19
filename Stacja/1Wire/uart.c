/*
 * uart.c
 *
 *  Created on: 02-01-2015
 *      Author: Damian Kowalski
 */

#include "avr/io.h"
#include <avr/pgmspace.h>
#include <stdlib.h>

#include "uart.h"



void uart_init( uint16_t _UBRR )
{
	UBRRH = (uint8_t)(_UBRR>>8);
	UBRRL = (uint8_t)_UBRR;

	UCSRB = (1<<TXEN);   //uruchomienie nadawania
}


void uart_putc( char c )     //nadawanie pojedynczego znaku | raczej z tego nie bedziemy korzystac dlatego static
{
	while( !(UCSRA & (1 << UDRE ))); //dopóki rejestr i bit(flaga) nie bêd¹ puste

	UDR = c;  //rejestr gdzie przetrzymywany jest nasz znak
}


void uart_puts( char *s )  //nadawanie ca³ego stringa
{
	while( *s ) uart_putc( *s++ );  //wysy³anie tekstu jako pojedynczych znaków

}

void uart_puts_P(const char *s)   //wysylanie stringa z pamiêci flash
{
	register char c;
	while((c = pgm_read_byte(s++))) uart_putc(c);
}

void uart_putint( uint32_t liczba, uint8_t radix )  //wysy³anie liczb
{
	char buf[17];
	ltoa( liczba, buf, radix);
	uart_puts( buf );

}

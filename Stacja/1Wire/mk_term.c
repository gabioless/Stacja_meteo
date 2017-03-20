
#include <avr/io.h>
#include <avr/pgmspace.h>

#include "uart.h"
#include "mk_term.h"

// aby mieæ polskie ogonki w PUTTY ----> ustawiæ WINDOW / Translation / Win1250

const char UCLS[] PROGMEM = { "\x1b""[2J" };

const char UHOME[] PROGMEM = { "\x1b""[;H" };

const char UCUR_HIDE[] PROGMEM = { "\x1b""[?25l" };
const char UCUR_SHOW[] PROGMEM = { "\x1b""[?25h" };

const char U_ATTR_OFF[] PROGMEM = { "\x1b""[m" };



void tr_cursor_hide( uint8_t hide ) {
	if(hide) uart_puts_P( UCUR_HIDE );
	else uart_puts_P( UCUR_SHOW );
}


void tr_cls(uint8_t cur_onoff) {

	uart_puts_P( U_ATTR_OFF );
	tr_cursor_hide(cur_onoff);
	uart_puts_P( UCLS );

	uart_puts_P( UHOME );

}


void fill_line( char ascii, uint8_t cnt ) {
	for(uint8_t i=0; i<cnt; i++) uart_putc( ascii );
}


void tr_attr( uint8_t atr, uint8_t fg, uint8_t bg ) {
	uart_putc( 0x1b );		// <ESC>[0;32;44m
	uart_putc( '[' );
	uart_putc( atr+'0' );
	uart_putc( ';' );
	uart_putc( '3' );
	uart_putc( fg+'0' );
	uart_putc( ';' );
	uart_putc( '4' );
	uart_putc( bg+'0' );
	uart_putc( 'm' );
}


void tr_pen_color( uint8_t cl ) {
	uart_putc( 0x1b );		// <ESC>[34m
	uart_putc( '[' );
	uart_putc( '3' );
	uart_putc( cl+'0' );
	uart_putc( 'm' );
}

void tr_brush_color( uint8_t cl ) {
	uart_putc( 0x1b );		// <ESC>[44m
	uart_putc( '[' );
	uart_putc( '4' );
	uart_putc( cl+'0' );
	uart_putc( 'm' );
}


void tr_locate( uint8_t y, uint8_t x ) {

	uart_putc( 0x1b );	// <ESC>[y;xH
	uart_putc( '[' );
	uart_putint( y, 10 );
	uart_putc( ';' );
	uart_putint( x, 10 );
	uart_putc( 'H' );
}



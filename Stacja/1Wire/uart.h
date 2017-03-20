/*
 * uart.h
 *
 *  Created on: 02-01-2015
 *      Author: Damian Kowalski
 */

#ifndef UART_H_
#define UART_H_

#define UART_BAUD 19200
#define __UBRR ((F_CPU+UART_BAUD*8UL) / (16UL * UART_BAUD)-1)






void uart_init( uint16_t _UBRR );
void uart_putc( char c );
void uart_puts( char *s );
void uart_puts_P(const char *s);
void uart_putint( uint32_t liczba, uint8_t radix );




#endif /* UART_H_ */

/*
 * main.c
 *
 *  Created on: 23.05.2017
 *      Author: gabog
 */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "MK_USART/mkuart.h"

int main(){
	DDRA |= (1 << PA7);
	PORTA &= ~(1 << PA7);


	sei();
	USART_Init( __UBRR );

	uart_puts("at\r\n");
	uart_putc('a');

	while(1){
		_delay_ms(1000);
		PORTA ^= (1 << PA7);
		uart_putc('a');
	}
	return 0;
}


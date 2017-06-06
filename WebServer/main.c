/*
 * main.c
 *
 *  Created on: 11.04.2017
 *      Author: Damian
 */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>

#include "MK_USART/mkuart.h"

//deklaracje funkcji
void ParseRS232( char* buff );
void SendHTMLCode( void );
void SendHTTP200Ok( void );
void SendPageBody( void );

//deklaracje zmiennych
char buff[512]; //globalny bufor do zdarzenia UART
uint8_t counter = 0;

int main( void )
{
	//konfiguracja modu³u ESP8266
	sei();
	USART_Init( __UBRR );
	//register_uart_str_rx_event_callback( ParseRS232 ); //zarejestrowanie zdarzenia



	DDRA |= (1 << PA7);
	PORTA &= ~(1 << PA7);


	while( 1 )
	{
		//_delay_ms(1000);
		//uart_putc('a');
		UART_RX_STR_EVENT( buff );
		for(int i = 3; i < 512; i++)
			{
				if(buff[i-3] == 'G' && buff[i-2] == 'E' && buff[i-1] == 'T' )
				{
					PORTA ^= (1<<PA7);
					SendHTMLCode();
					break;
				}
			}
	}
	return 0;
}

void ParseRS232( char* buff )
{
	//uart_puts(buff);
	//if( !strncmp("GET", buff, 3) ) // czy w buforze znajduje siê GET /
	//{
		//PORTA &= ~ (1 << PA7);
	//	SendHTMLCode();
	//}
	if( !strncmp("OK", buff, 2) ) // czy w buforze znajduje siê GET /
	{
		PORTA ^= (1<<PA7);
	}

	/*for(int i = 3; i < 512; i++)
		{
			if(buff[i-3] == 'G' && buff[i-2] == 'E' && buff[i-1] == 'T' )
			{
				PORTA ^= (1<<PA7);
				SendHTMLCode();
				break;
			}
		}*/
}

void SendHTMLCode( void )
{
	//czy tu powinna byc komenda AT+CIPSEND=0,<ile bajtow> by zadzia³a³o?
	//uart_puts("AT+CIPSERVER=1,80");
	//_delay_ms(250);
	PORTA ^= (1 << PA7);
	_delay_ms(250);
	uart_puts("AT+CIPSEND=0,40\r\n");
	PORTA ^= (1 << PA7);
	_delay_ms(250);
	//SendHTTP200Ok();
	SendPageBody();
	_delay_ms(750);
	PORTA ^= (1 << PA7);
	uart_puts("AT+CIPCLOSE=0\r\n");
	_delay_ms(250);
	uart_puts("AT+CIPCLOSE=0\r\n");
	PORTA ^= (1 << PA7);
	//zamkniêcie po³¹czenia komend¹ AT+CIPCLOSE ??
}

void SendPageBody( void )
{
	//45znakow
	uart_puts_P( PSTR("<html><body>Hello World ")); //37
	uart_putint(counter++, 10);
	//uart_puts_P( PSTR("<html><body>Hello World\n\r") );
	//uart_putint(counter++, 20);
	uart_puts_P( PSTR("</body></html>\r\n") );
}

void SendHTTP200Ok( void )
{
	uart_puts_P( PSTR("HTTP/1.1 200 OK\r\nContent-Type:text/html\r\nPragma: no-cache\r\n\r\n") );
}

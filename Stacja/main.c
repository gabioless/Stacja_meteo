/*
 * main.c
 *
 *  Created on: 14.03.2017
 *      Author: gabog
 */

//dolaczania naglowek
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

//biblioteka lcd
#include "lcdlibrary/lcd.h"

//zmienne globalne

//funckja glowna
int main(void){

	//inicjalizacja sprzetu
	DDRA |= (1 << PA7);
	PORTA &= ~(1 << PA7);

	//lcd inicjalizacja
	lcd_init(LCD_DISP_ON);

	lcd_clrscr();

	lcd_puts("LCD Test Line 1\n");

	//petla nieskonczona
	while(1){
		PORTA |= (1 << PA7);
		lcd_clrscr();
		lcd_puts("Test 1\n");
		_delay_ms(1000);
		PORTA &= ~(1 << PA7);
		lcd_clrscr();
		lcd_puts("Test 2\n");
		_delay_ms(1000);

	}

	return 0;
}

//funkcje odpowiedzialne za RTC
// testowanie

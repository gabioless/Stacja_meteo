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

//zmienne globalne

//funckja glowna
int main(void){

	//inicjalizacja sprzetu
	//int i = 0;
	DDRA |= (1 << PA0);
	PORTA &= ~(1 << PA0);

	//petla nieskonczona
	while(1){
		PORTA |= (1 << PA0);
		_delay_ms(1000);
		PORTA &= ~(1 << PA0);
		_delay_ms(1000);
	}

	return 0;
}

//funkcje odpowiedzialne za RTC
// testowanie

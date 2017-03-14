/*
 * main.c
 *
 *  Created on: 14.03.2017
 *      Author: gabog
 */

//dolaczania naglowek
#include <avr/io.h>
#include <stdio.h>

//zmienne globalne

//funckja glowna
int main(void){

	//inicjalizacja sprzetu
	int i = 0;


	//petla nieskonczona
	while(1){
		if(i<10){
			printf("siema");
		}
	}

	return 0;
}

//funkcje odpowiedzialne za RTC

/*
 * main.c
 *
 *  Created on: 14.03.2017
 *      Author: gabog
 */

//dolaczania naglowek
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>						//biblioteka opoznien
#include <avr/interrupt.h>					//biblioteka przerwan
#include "lcdlibrary/lcd.h"					//biblioteka lcd
#include "rtc_pcf8563/I2cbase.h" 			//bibliteka i2c
#include "rtc_pcf8563/PCF8563.h"			//biblioteka RTC PCH8563
#include "1Wire/ds18x20.h"
#include "dht/dht.h"
#include "MK_USART/mkuart.h"

#define STALA 3.14 							//obwod wiatromierza

//zmienne globalne
volatile uint8_t licznik = 0;				//timer go uzywwa
volatile uint8_t licznik2 = 0;				//petla while go uzywa
char tekst[3]; 								//do wyswietlania
uint8_t subzero, cel, cel_fract_bits;       //do temp
volatile uint16_t enkoder = 10545;
volatile uint16_t liczba = 0;
volatile uint16_t obroty = 0;
char buff[256];
char buffTMP[26];
int h,m,s;

Date data; //data do zapisania
Time czas; //time do zapisania

//funkcje
void ini_Timer();
void lcd_int(int val);
void DisplayTemp();
void TakeMeasurement();
void ini_enkoder();
void iniTimer16bit();
void Parse(char *buff );
void GetDate();
void SendAt(char* command);
void ini_esp8266();

#if DHT_FLOAT == 0
	int8_t temperature = 0;
	int8_t humidity = 0;
#elif DHT_FLOAT == 1
	float temperature = 0;
	float humidity = 0;
#endif

int main(void){

	//inicjalizacja debugera
	DDRA |= (1 << PA7);
	PORTA &= ~(1 << PA7);

	//inicjalizacja timera
	ini_Timer();
	ini_enkoder();
	iniTimer16bit();
	sei();

	//inicjalizacja RTC
	I2C_Init();
	PCF8563_WriteRegister(CLKUOUTFreqReg, PCF_CLKOUTActivate | PCF_CLKOUT1Hz); 		//init RTC; 1Hz na CKLout

	//lcd inicjalizacja
	lcd_init(LCD_DISP_ON);
	lcd_clrscr();

	data.Year=bin2bcd(17);
	data.Month=bin2bcd(3);
	data.Day=bin2bcd(27);

	czas.Second=bin2bcd(0);
	czas.Minute=bin2bcd(44);
	czas.Hour=bin2bcd(14);

	PCF8563_SetTime(&czas);

	if(!PCF8563_IsDataValid()){
		PCF8563_SetTime(&czas);
		PCF8563_SetDate(&data);
	}

	search_sensors();

	USART_Init( __UBRR );
	register_uart_str_rx_event_callback(Parse);

	//ini_esp8266();

	_delay_ms(2000);




	while(1){

		UART_RX_STR_EVENT( buff );

		if(licznik2 >= 150){
			SendAt("AT+CIPSTART=\"TCP\",\"www.onet.pl\",80");
			PCF8563_GetTime(&czas);
			lcd_gotoxy(0,0);
			bcd2ASCII(czas.Hour, tekst);
			lcd_puts(tekst);
			lcd_puts(":");
			bcd2ASCII(czas.Minute, tekst);
			lcd_puts(tekst);
			lcd_puts(":");
			bcd2ASCII(czas.Second, tekst);
			lcd_puts(tekst);
			TakeMeasurement(); // tu wykona si� pomiar, odczyt i wy�wietlenie na lcd
			licznik2 = 0;
			lcd_gotoxy(10,0);
			lcd_int(enkoder);
			lcd_gotoxy(10,1);
			lcd_int(obroty);
			if(dht_gettemperaturehumidity(&temperature, &humidity) != -1) {
						//#if DHT_FLOAT == 0
						//itoa(temperature, printbuff, 10);
						//#elif DHT_FLOAT == 1
						//dtostrf(temperature, 3, 3, printbuff);
						//#endif
						//uart_puts("temperature: "); uart_puts(printbuff); uart_puts("C");uart_puts("\r\n");
						#if DHT_FLOAT == 0
							lcd_gotoxy(10,0);
							lcd_int(humidity);
							lcd_puts("   ");
						#elif DHT_FLOAT == 1
						dtostrf(humidity, 3, 3, printbuff);
						#endif
						//uart_puts("humidity: "); uart_puts(printbuff); uart_puts("%RH");uart_puts("\r\n");

					}
			/*else {
						lcd_gotoxy(10,0);
						lcd_puts("error");
					}*/
		}
	}
	return 0;
}


void DisplayTemp() {
	lcd_gotoxy(0,1); // druga linia, pierwszy znak
	lcd_puts("T: ");
    if (subzero)
    	lcd_puts("-");
    else
    	lcd_puts(" ");
    lcd_int(cel);
    lcd_puts(".");
    lcd_int(cel_fract_bits);
    lcd_puts("C ");
}


void lcd_int(int val){
	char bufor[17];
	lcd_puts( itoa(val, bufor, 10) );
}


void TakeMeasurement() {
	static uint8_t DS_licznik;
	if(DS_licznik == 0){
		DS18X20_start_meas(DS18X20_POWER_EXTERN, NULL);
	}
	else if(DS_licznik == 1){
		if (DS18X20_OK == DS18X20_read_meas(gSensorIDs[0], &subzero, &cel, &cel_fract_bits))
			DisplayTemp();
		else {
		    lcd_gotoxy(0,1);
		    lcd_puts("error    ");    /* wy�wietlamy informacj� o b��dzie je�li np brak czujnika lub b��d odczytu */
		}
	}
	DS_licznik++;
	if(DS_licznik >= 2){
		DS_licznik = 0;
	}
}

void ini_esp8266(){

	uart_puts("AT+CWJAP=\"p205a\",\"426312638\"\r\n");
	//uart_puts("at\r\n");

}

void Parse(char *buff )
{
	if(!strncmp("CONNECT", buff,7) || !strncmp("ALREADY CONNECTED", buff, 17))
	{
		SendAt("AT+CIPSEND=1");
		_delay_ms(250);
		SendAt("a");
	}

	/*if(!strncmp("OK\n> ", buff, 5))
	{
		SendAt("a");
		//uart_putc('a');
		//uart_putc('\r');
		//uart_putc('\n');
	}*/

	if(!strncmp("Date: ", buff,6))
	{
		PORTA ^= (1 << PA7);
		GetDate();
	}

	/*if(!strncmp("ALREADY CONNECTED", buff, 5))
	{
		SendAt("AT+CIPCLOSE");
		_delay_ms(250);
		SendAt("AT+CIPSTART=\"TCP\",\"www.onet.pl\",80");
	}*/


}

void SendAt(char* command)
{
	uart_puts(command);
	uart_putc('\r');
	uart_putc('\n');
}

void GetDate()
{

	uart_putc('\n');
	for(int i = 5; i < 255; i++)
	{
		if(buff[i-5] == 'D' && buff[i-4] == 'a' && buff[i-3] == 't' && buff[i-2] == 'e' && buff[i-1] == ':' )
		{
			for(int j = 0; j < 26; j++)
			{
				buffTMP[j] = buff[i+j];
			}
			break;
		}
	}

	char tmp[3];
	uart_putc('\n');
	tmp[0] = buffTMP[18];
	tmp[1] = buffTMP[19];
	tmp[2] = '\n';

	h = atoi(tmp);
	h += 2;
	h = h % 24;

	tmp[0] = buffTMP[21];
	tmp[1] = buffTMP[22];
	tmp[2] = '\n';

	m = atoi(tmp);


	tmp[0] = buffTMP[24];
	tmp[1] = buffTMP[25];
	tmp[2] = '\n';

	s = atoi(tmp);



	//uart_putint(h,10);
	//uart_putc(':');
	//uart_putint(m,10);
	//uart_putc(':');
	//uart_putint(s,10);

	czas.Second=bin2bcd(s);
	czas.Minute=bin2bcd(m);
	czas.Hour=bin2bcd(h);

	PCF8563_SetTime(&czas);

}

void ini_enkoder(){
	EICRA |= (1 << ISC01);
	EIMSK |= (1 << INT0);
}

ISR(INT0_vect){
	enkoder = enkoder + 10;
}

void iniTimer16bit(){
	TCCR1B |= (1 << WGM12) || (1 << CS12) || (1 << CS00);		//preskaler 1024, CTC
	TIMSK1 |= (1 << OCIE1A);									//unlock compare channel A
	OCR1A = (int16_t)(F_CPU/1024);								//co 100ms przerwanie
}

ISR(TIMER1_COMPA_vect){
	liczba = enkoder;
	enkoder = 0;
	obroty = liczba / 24;
	obroty *= STALA;
	obroty *= 10;
}

//inicjalizacja timera
void ini_Timer(){
	TCCR0A |= (1 << WGM01); 				//tryb CTC
	TCCR0B |= (1 << CS02) | (1 << CS00); 	//preskaler 1024
	TIMSK0 |= (1 << OCIE0A); 				//odblokowanie przerwania z trybu compare
	OCR0A = 255;								//obliczone 100ms dla 1Mhz
}

//przerwanie dla timera0
ISR(TIMER0_COMPA_vect){
	licznik++;
	licznik2++;
	if(licznik >= 10){
		//PORTA ^= (1 << PA7);
		licznik = 0;
	}
}

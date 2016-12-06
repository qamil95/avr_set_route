/*
 * ProjektSmiw.c
 *
 * Created: 2016-11-23 13:02:00
 *  Author: qamil
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/cpufunc.h> 
#include <util/delay.h>


#define delay_time 2

uint8_t LED_TAB[7];
uint8_t DISPLAY_TAB[5];
const uint8_t DISPLAY[] = {
	0x3F,	// 0
	0x06,	// 1
	0x5B,	// 2
	0x4F,	// 3
	0x66,	// 4
	0x6D,	// 5
	0x7D,	// 6
	0x27,	// 7
	0x7F,	// 8
	0x6F,	// 9
	0x77,	// 10  A
	0x7C,	// 11  b
	0x39,	// 12  C
	0x5E,	// 13  d
	0x79,	// 14  E
	0x71,	// 15  F
	0x00,	// 16  OFF_DISPLAY
};

void set_led_ano(uint8_t i) {
	switch (i)
	{
	case 0 :
		PORTA &= ~(1<<PA7);
		break;
	case 1 :
		PORTB &= ~(1<<PB7);
		break;
	case 2 :
		PORTB &= ~(1<<PB6);
		break;
	case 3 :
		PORTB &= ~(1<<PB5);
		break;
	case 4 :
		PORTC &= ~(1<<PC1);
		break;
	case 5 :
		PORTC &= ~(1<<PC0);
		break;
	case 6 :
		PORTD &= ~(1<<PD7);
		break;
	default:
		break;
	}
}

void clear_led_ano(uint8_t i) {
	switch (i)
	{
	case 0 :
		PORTA |= (1<<PA7);
		break;
	case 1 :
		PORTB |= (1<<PB7);
		break;
	case 2 :
		PORTB |= (1<<PB6);
		break;
	case 3 :
		PORTB |= (1<<PB5);
		break;
	case 4 :
		PORTC |= (1<<PC1);
		break;
	case 5 :
		PORTC |= (1<<PC0);
		break;
	case 6 :
		PORTD |= (1<<PD7);
		break;
	default:
		break;
	}

	
}

void refresh_output() {
	for (uint8_t i = 0; i<7; i++) {
		PORTD &= 0b10000000; //wyzerowanie portu D poza najstarszym bitem - katody LED
		LED_TAB[i] &= 0b011111111; //wyzerowanie najstarszego bitu tablicy LED
		PORTD |= LED_TAB[i]; //suma logiczna (najstarszy bit portu + 7 m³odszych bitów tablicy)
		set_led_ano(i); //w³¹czenie i-tej anody LED
		if (i<5) {
			PORTA &= 0b10000000; //wyzerowanie portu A poza najstarszym bitem - katody wyœwietlaczy
			DISPLAY_TAB[i] &= 0b011111111; //wyzerowanie najstarszego bitu tablicy wyœwietlaczy
			PORTA |= DISPLAY_TAB[i]; //suma logiczna (najstarszy bit portu + 7 m³odszych bitów tablicy)
			PORTB &= (~(1<<i)); //w³¹czenie i-tej anody wyœwietlacza
		}
		_delay_ms(delay_time); //czas œwiecenia
		clear_led_ano(i); //wy³¹czenie i-tej anody LED
		if (i<5) 
			PORTB |= (1<<i); //wy³¹czenie i-tej anody wyœwietlacza	
	}	
}

void scan_keyboard()
{
	PORTC |= (1<<PC2);
	LED_TAB[0] = PINC >> 1;
	PORTC &= ~(1<<PC2);
	_NOP(); //do synchronizacji, zalecany w datasheet a bez niego nie dzia³a
	LED_TAB[1] = PINC >> 1;
	PORTC |= (1<<PC2);
}

int main(void)
{
	/************************************************************************/
	/* INICJALIZACJA                                                        */
	/************************************************************************/
	//podci¹gniêcie wszystkich pinów do VCC
	PORTA = 0xFF;
	PORTB = 0xFF;
	PORTC = 0xFF;
	PORTD = 0xFF;
	
	//ustawienie wejœæ i wyjœæ
	DDRA = 0xFF; // anoda LED na pinie 7, katody wyswietlaczy na pinach 6-0
	DDRB = 0xFF; // anody LED na pinach 7-5, anody wyswietlaczy na pinach 4-0, 
	DDRC = 0x07; // anody LED na pinach 1-0, pin 2 wyjœcie na select multipleksera, piny 7-3 wejœciowe
	DDRD = 0xFF; // anoda LED na pinie 7, katody LED na pinach 6-0
	
	//wyzerowanie wyswietlacza
	for (uint8_t i = 0; i<4; i++)
		DISPLAY_TAB[i] = DISPLAY[16];

	//wyzerowanie ledow
	for (int i = 0; i<7; i++) 
		LED_TAB[i] = 0x00;

	/************************************************************************/
	/* PROGRAM - PSEUDOLICZNIK                                              */
	/************************************************************************/
	uint16_t i = 0;
    while(1)
    {
		refresh_output();
			
		scan_keyboard();
		

		if (i<1000000)
			i++;
		else
			i=0;

		//petla inkrementacji wyswietlacza
		uint16_t numerek = i;
		for (int8_t j = 4; j>=0; j--) {
			uint8_t wart = numerek % 10;
			numerek = numerek / 10;
			DISPLAY_TAB[j] = DISPLAY[wart];	
		}
					
    }
}


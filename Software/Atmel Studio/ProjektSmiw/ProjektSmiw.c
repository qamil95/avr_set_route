/*
 * ProjektSmiw.c
 *
 * Created: 2016-11-23 13:02:00
 *  Author: qamil
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>


// 7SEGMENT WYSWIETLACZ
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


void ustaw_wyswietlacz(uint8_t wartosc) {
	PORTA &= 0b10000000; //zachowanie nieuzywanego pinu
	wartosc &= 0b01111111; //wyzerowanie nieuzywanego pinu
	PORTA |= wartosc;
}


void odswiez_7segment() {	
	uint8_t tmp = PORTA;

	PORTB = 0xFF;
	ustaw_wyswietlacz(DISPLAY_TAB[0]);
	PORTB = 0b11111110;	
	_delay_ms(1);

	PORTB = 0xFF;
	ustaw_wyswietlacz(DISPLAY_TAB[1]);
	PORTB = 0b11111101;
	_delay_ms(1);

	PORTB = 0xFF;
	ustaw_wyswietlacz(DISPLAY_TAB[2]);
	PORTB = 0b11111011;
	_delay_ms(1);

	PORTB = 0xFF;
	ustaw_wyswietlacz(DISPLAY_TAB[3]);
	PORTB = 0b11110111;
	_delay_ms(1);

	PORTB = 0xFF;
	ustaw_wyswietlacz(DISPLAY_TAB[4]);
	PORTB = 0b11101111;
	_delay_ms(1);
	PORTB = 0b11111111;
	PORTA = tmp;
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
	DDRA = 0b00000000; // katody wyswietlaczy na pinach 6-0 jako wejscia
	DDRB = 0b00011111; //anody wyswietlaczy na pinach 4-0
	
	//wyzerowanie wyswietlacza
	DISPLAY_TAB[0] = DISPLAY[16];
	DISPLAY_TAB[1] = DISPLAY[16];
	DISPLAY_TAB[2] = DISPLAY[16];
	DISPLAY_TAB[3] = DISPLAY[16];
	DISPLAY_TAB[4] = DISPLAY[16];

	/************************************************************************/
	/* PROGRAM - PSEUDOLICZNIK                                              */
	/************************************************************************/
	uint16_t i = 0;

    while(1)
    {
		odswiez_7segment();		
		if (!(PINA & (1<<PA7))) {
			if (i<1000000)
				i++;
			else
				i=0;
		}
		uint16_t numerek = i;
		for (int8_t j = 4; j>=0; j--) {
			uint8_t wart = numerek % 10;
			numerek = numerek / 10;
			DISPLAY_TAB[j] = DISPLAY[wart];		
			
		}
    }
}


/*
 * ProjektSmiw.c
 *
 * Created: 2016-11-23 13:02:00
 *  Author: qamil
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

#define czas_delay 3

// 7SEGMENT WYSWIETLACZ
	uint8_t SEG_TABLICA[5];
	const uint8_t SEG_ZGAS =	0xFF;
	const uint8_t SEG_0	=	0x88;
	const uint8_t SEG_1	=	0xBE;
	const uint8_t SEG_2	=	0xC4;
	const uint8_t SEG_3	=	0x94;
	const uint8_t SEG_4	=	0xB2;
	const uint8_t SEG_5	=	0x91;
	const uint8_t SEG_6	=	0x81;
	const uint8_t SEG_7	=	0xB8;
	const uint8_t SEG_8	=	0x80;
	const uint8_t SEG_9	=	0x90;
	const uint8_t SEG_A	=	0xA0;
	const uint8_t SEG_B	=	0x83;
	const uint8_t SEG_C	=	0xC9;
	const uint8_t SEG_D	=	0x86;
	const uint8_t SEG_E	=	0xC1;
	const uint8_t SEG_F	=	0xE1;
	const uint8_t SEG_G	=	0x89;
	const uint8_t SEG_H	=	0xA2;
	const uint8_t SEG_I	=	0xCF;
	const uint8_t SEG_J	=	0x8E;
	const uint8_t SEG_L =	0xCB;
	const uint8_t SEG_N	=	0xA7;
	const uint8_t SEG_O	=	0x87;
	const uint8_t SEG_P	=	0xE0;
	const uint8_t SEG_R	=	0xE7;
	const uint8_t SEG_T	=	0xC3;
	const uint8_t SEG_U	=	0x8A;
	const uint8_t SEG_Y	=	0x92;
	const uint8_t SEG_MINUS	=	0xF7;


void ustaw_wyswietlacz(uint8_t wartosc) {
	wartosc = ~wartosc; //bo zanegowane stale
	PORTA &= 0b10000000; //zachowanie nieuzywanego pinu
	wartosc &= 0b01111111; //wyzerowanie nieuzywanego pinu
	PORTA |= wartosc;
}


void odswiez_7segment() {	
	uint8_t tmp = PORTA;

	PORTB = 0xFF;
	//PORTA = SEG_TABLICA[0];	
	ustaw_wyswietlacz(SEG_TABLICA[0]);
	PORTB = 0b11101111;	
	_delay_ms(1);

	PORTB = 0xFF;
	//PORTA = SEG_TABLICA[1];
	ustaw_wyswietlacz(SEG_TABLICA[1]);
	PORTB = 0b11110111;
	_delay_ms(1);

	PORTB = 0xFF;
	//PORTA = SEG_TABLICA[2];
	ustaw_wyswietlacz(SEG_TABLICA[2]);
	PORTB = 0b11111011;
	_delay_ms(1);

	PORTB = 0xFF;
	//PORTA = SEG_TABLICA[3];
	ustaw_wyswietlacz(SEG_TABLICA[3]);
	PORTB = 0b11111101;
	_delay_ms(1);

	PORTB = 0xFF;
	//PORTA = SEG_TABLICA[4];
	ustaw_wyswietlacz(SEG_TABLICA[4]);
	PORTB = 0b11111110;
	_delay_ms(1);
	PORTB = 0b11111111;
	PORTA = tmp;
}


int main(void)
{
	PORTA = 0xFF;
	PORTB = 0xFF;
	PORTC = 0xFF;
	PORTD = 0xFF;
	
	DDRA = 0x00;
	DDRB = 0b00011111;
	
	SEG_TABLICA[0] = SEG_ZGAS;
	SEG_TABLICA[1] = SEG_ZGAS;
	SEG_TABLICA[2] = SEG_ZGAS;
	SEG_TABLICA[3] = SEG_ZGAS;
	SEG_TABLICA[4] = SEG_ZGAS;

	long int i = 0;

    while(1)
    {
		odswiez_7segment();		
		
		if (!(PINA & (1<<PA7))) {
			if (i<1000000)
				i++;
			else
				i=0;
		}
		long int numerek = i;
		for (int j = 4; j>=0; j--) {
			int wart = numerek % 10;
			numerek = numerek / 10;
			switch (wart) {
				case 0:
					SEG_TABLICA[j] = SEG_0;
					break;
				case 1:
					SEG_TABLICA[j] = SEG_1;
					break;
				case 2:
					SEG_TABLICA[j] = SEG_2;
					break;
				case 3:
					SEG_TABLICA[j] = SEG_3;
					break;
				case 4:
					SEG_TABLICA[j] = SEG_4;
					break;
				case 5:
					SEG_TABLICA[j] = SEG_5;
					break;
				case 6:
					SEG_TABLICA[j] = SEG_6;
					break;
				case 7:
					SEG_TABLICA[j] = SEG_7;
					break;
				case 8:
					SEG_TABLICA[j] = SEG_8;
					break;
				case 9:
					SEG_TABLICA[j] = SEG_9;
					break;
			}
		}



    }
}


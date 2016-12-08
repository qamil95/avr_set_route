/*
 * ProjektSmiw.c
 *
 * Created: 2016-11-23 13:02:00
 *  Author: qamil
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/cpufunc.h> 
#include <avr/interrupt.h>
#include <util/delay.h>


#define delay_time 2

uint16_t licznik = 0;
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
	for (int i = 0; i<7; i++)
		LED_TAB[i] = 0x00;

	PORTC |= (1<<PC2);
	if (!(PINC & (1<<PC3)))
		LED_TAB[2] = PINC >> 1;
	else
		LED_TAB[0] = PINC >> 1;
        
	PORTC &= ~(1<<PC2); //zmiana wejscia multipleksera
	_NOP(); //do synchronizacji, zalecany w datasheet a bez niego nie dzia³a
	if (!(PINC & (1<<PC3)))
		LED_TAB[3] = PINC >> 1;
	else
		LED_TAB[1] = PINC >> 1;
	PORTC |= (1<<PC2);
}

void int_to_display(uint16_t value)
{
    for (int8_t i=4; i>-1; i--)
    {
        DISPLAY_TAB[i] = DISPLAY[value%10];
        value /= 10;
    }
}

/************************************************************************/
/* Co ile czasu ma wywolac sie przerwanie TIMER1_COMPA                  */
/* Wartosc musi byc podzielna przez 4                                   */
/************************************************************************/
void set_interrupt_delay(uint16_t delay)
{
	if (delay %4 == 0) 
		OCR1A = (F_CPU / 256000) * delay;
	else
	{
		DISPLAY_TAB[0]=0x79;
		DISPLAY_TAB[1]=0x50;
		DISPLAY_TAB[2]=0x50;
		DISPLAY_TAB[3]=0x5C;
		DISPLAY_TAB[4]=0x50;
		while (1)
			refresh_output();
	}
}

/************************************************************************/
/* Przerwanie wywolane co czas ustawiony przez set_interrupt_delay      */
/************************************************************************/
ISR(TIMER1_COMPA_vect)
{
	licznik++;
	if (licznik>99999)
		licznik=0;
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
	/* USTAWIENIE ZEGARA                                                    */
	/************************************************************************/
	//tryb CTC
	TCCR1B |= (1 << WGM12);
	
	//ustawienie jak czesto przerwanie
	set_interrupt_delay(1000);
		
	//preskaler zegara T1 na 256, start licznika
	TCCR1B |= (1<<CS12);
	
	//zezwolenie na przerwanie CTC A
	TIMSK |= (1 << OCIE1A);
	
	//zezwolenie na przerwania w ogole
	sei();

	/************************************************************************/
	/* PROGRAM                                                              */
	/************************************************************************/	
    while(1)
    {
		scan_keyboard();
		refresh_output();		
        int_to_display(licznik);
    }
}


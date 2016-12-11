/*
 * ProjektSmiw.c
 *
 * Created: 2016-11-23 13:02:00
 *  Author: qamil
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define delay_time 400

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

/************************************************************************/
/* Ustawienie na diodach i wyœwietlaczu wartoœci z tablic + odœwie¿enie */
/************************************************************************/
void refresh_output() {
	for (uint8_t i = 0; i<7; i++) {
		PORTD &= 0b10000000; //wyzerowanie portu D poza najstarszym bitem - katody LED
		LED_TAB[i] &= 0b011111111; //wyzerowanie najstarszego bitu tablicy LED
		PORTD |= LED_TAB[i]; //suma logiczna (najstarszy bit portu + 7 m³odszych bitów tablicy)
		if (i<5) {
			PORTA &= 0b10000000; //wyzerowanie portu A poza najstarszym bitem - katody wyœwietlaczy
			DISPLAY_TAB[i] &= 0b011111111; //wyzerowanie najstarszego bitu tablicy wyœwietlaczy
			PORTA |= DISPLAY_TAB[i]; //suma logiczna (najstarszy bit portu + 7 m³odszych bitów tablicy)
		}
		
		PORTB &= (~(1<<i)); //w³¹czenie i-tej anody
		_delay_us(delay_time); //czas œwiecenia
		PORTB |= (1<<i); //wy³¹czenie i-tej anody			
	}	
}

/************************************************************************/
/* Funkcja wyœwietlaj¹ca integera na siedmiosegmentowcu                 */
/* value - wartoœæ do wyœwietlenia                                      */
/* start_seg, end_seg - zakres które segmenty maj¹ byæ wyœwietlone      */
/************************************************************************/
void int_to_display(uint16_t val, uint8_t start_seg, uint8_t end_seg)
{
    for (int8_t i=end_seg; i>start_seg-1; i--)
    {
        DISPLAY_TAB[i] = DISPLAY[val%10];
        val /= 10;
    }
}

/************************************************************************/
/* Funkcja wywo³ywana przy b³êdach - wyœwietla ERR + kod b³êdu,         */
/* po czym przerywa dzia³anie programu. Kody b³êdów:					*/
/* 01 - delay przerwania licznika niepodzielny przez 4                  */
/************************************************************************/
void error(uint8_t error_code)
{
	cli();
	DISPLAY_TAB[0]=0x79;
	DISPLAY_TAB[1]=0x50;
	DISPLAY_TAB[2]=0x50;
	int_to_display(error_code, 3,4);	
	while (1)
		refresh_output();
}

/************************************************************************/
/* Funkcja ustawia co ile czasu ma wywolac sie przerwanie TIMER1_COMPA  */
/* Wartosc musi byc podzielna przez 4                                   */
/************************************************************************/
void set_interrupt_delay(uint16_t delay)
{
	if (delay %4 == 0) 
		OCR1A = (F_CPU / 256000) * delay;
	else
		error(01);
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
	DDRA = 0x7F; // katody wyœwietlacza, pin7 jako wejœcie
	DDRB = 0x7F; // wspólne anody, pin7 jako wejœcie
	DDRC = 0x00; // klawiatura - piny wejœciowe
	DDRD = 0x7F; // katody LED, pin7 jako wejœcie
	
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
	set_interrupt_delay(100);
		
	//preskaler zegara T1 na 256, start licznika
	TCCR1B |= (1<<CS12);
	
	//zezwolenie na przerwanie CTC A
	TIMSK |= (1 << OCIE1A);
	
	//zezwolenie na przerwania w ogole
	sei();

	/************************************************************************/
	/* PROGRAM - g³ówna pêtla                                               */
	/************************************************************************/	
    while(1)
    {
		if (!(PIND & (1<<PD7))) //klawiatura
		{
			LED_TAB[0] = PINC >> 1;
			LED_TAB[1] = PINC << 3;
			
		}
		else
		{
			LED_TAB[2] = PINC >> 1;
			LED_TAB[3] = PINC << 3;
		}					
		
		if (!(PINA & (1<<PA7))) //niebieski przycisk do wygaszenia diod
			for (int i = 0; i<7; i++)
				LED_TAB[i] = 0x00;
		
        int_to_display(licznik,0,4);
		refresh_output();	
    }
}


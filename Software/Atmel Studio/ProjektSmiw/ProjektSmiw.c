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
/* Zapalenie LEDa o podanym ID                                          */
/************************************************************************/
void set_led(uint8_t id)
{
	switch (id)
	{
		case 1: LED_TAB[4] |= (1<<6);break;
		case 2: LED_TAB[5] |= (1<<6);break;
		case 3:	LED_TAB[1] |= (1<<6);break;
		case 4:	LED_TAB[0] |= (1<<6);break;
		case 5: LED_TAB[6] |= (1<<6);break;
		case 6: LED_TAB[6] |= (1<<5);break;
		case 7:	LED_TAB[0] |= (1<<5);break;
		case 8:	LED_TAB[1] |= (1<<5);break;
		case 9:	LED_TAB[2] |= (1<<5);break;
		case 10:LED_TAB[4] |= (1<<5);break;
		case 11:LED_TAB[3] |= (1<<5);break;
		case 12:LED_TAB[5] |= (1<<5);break;
		case 13:LED_TAB[3] |= (1<<6);break;
		case 14:LED_TAB[3] |= (1<<2);break;
		case 15:LED_TAB[0] |= (1<<2);break;
		case 16:LED_TAB[6] |= (1<<2);break;
		case 17:LED_TAB[5] |= (1<<2);break;
		case 18:LED_TAB[6] |= (1<<3);break;
		case 19:LED_TAB[0] |= (1<<3);break;
		case 20:LED_TAB[3] |= (1<<3);break;
		case 21:LED_TAB[1] |= (1<<3);break;
		case 22:LED_TAB[1] |= (1<<4);break;
		case 23:LED_TAB[6] |= (1<<4);break;
		case 24:LED_TAB[0] |= (1<<4);break;
		case 25:LED_TAB[1] |= (1<<2);break;
		case 26:LED_TAB[4] |= (1<<2);break;
		case 27:LED_TAB[4] |= (1<<0);break;
		case 28:LED_TAB[1] |= (1<<0);break;
		case 29:LED_TAB[0] |= (1<<0);break;
		case 30:LED_TAB[6] |= (1<<0);break;
		case 31:LED_TAB[5] |= (1<<0);break;
		case 32:LED_TAB[3] |= (1<<0);break;
		case 33:LED_TAB[2] |= (1<<0);break;
		case 34:LED_TAB[3] |= (1<<1);break;
		case 35:LED_TAB[5] |= (1<<1);break;
		case 36:LED_TAB[4] |= (1<<1);break;
		case 37:LED_TAB[2] |= (1<<6);break;
		case 38:LED_TAB[5] |= (1<<3);break;
		case 39:LED_TAB[2] |= (1<<4);break;
		case 40:LED_TAB[4] |= (1<<3);break;
		case 41:LED_TAB[4] |= (1<<4);break;
		case 42:LED_TAB[3] |= (1<<4);break;
		case 43:LED_TAB[5] |= (1<<4);break;
		case 44:LED_TAB[2] |= (1<<2);break;
		case 45:LED_TAB[2] |= (1<<3);break;
		case 46:LED_TAB[1] |= (1<<1);break;
		case 47:LED_TAB[2] |= (1<<1);break;
		case 48:LED_TAB[0] |= (1<<1);break;
		case 49:LED_TAB[6] |= (1<<1);break;
		default:break;
	}	
}

/************************************************************************/
/* Zgaszenie LEDa o podanym ID                                          */
/************************************************************************/
void clear_led(uint8_t id)
{
	switch (id)
	{
		case 1: LED_TAB[4] &= ~(1<<6);break;
		case 2: LED_TAB[5] &= ~(1<<6);break;
		case 3:	LED_TAB[1] &= ~(1<<6);break;
		case 4:	LED_TAB[0] &= ~(1<<6);break;
		case 5: LED_TAB[6] &= ~(1<<6);break;
		case 6: LED_TAB[6] &= ~(1<<5);break;
		case 7:	LED_TAB[0] &= ~(1<<5);break;
		case 8:	LED_TAB[1] &= ~(1<<5);break;
		case 9:	LED_TAB[2] &= ~(1<<5);break;
		case 10:LED_TAB[4] &= ~(1<<5);break;
		case 11:LED_TAB[3] &= ~(1<<5);break;
		case 12:LED_TAB[5] &= ~(1<<5);break;
		case 13:LED_TAB[3] &= ~(1<<6);break;
		case 14:LED_TAB[3] &= ~(1<<2);break;
		case 15:LED_TAB[0] &= ~(1<<2);break;
		case 16:LED_TAB[6] &= ~(1<<2);break;
		case 17:LED_TAB[5] &= ~(1<<2);break;
		case 18:LED_TAB[6] &= ~(1<<3);break;
		case 19:LED_TAB[0] &= ~(1<<3);break;
		case 20:LED_TAB[3] &= ~(1<<3);break;
		case 21:LED_TAB[1] &= ~(1<<3);break;
		case 22:LED_TAB[1] &= ~(1<<4);break;
		case 23:LED_TAB[6] &= ~(1<<4);break;
		case 24:LED_TAB[0] &= ~(1<<4);break;
		case 25:LED_TAB[1] &= ~(1<<2);break;
		case 26:LED_TAB[4] &= ~(1<<2);break;
		case 27:LED_TAB[4] &= ~(1<<0);break;
		case 28:LED_TAB[1] &= ~(1<<0);break;
		case 29:LED_TAB[0] &= ~(1<<0);break;
		case 30:LED_TAB[6] &= ~(1<<0);break;
		case 31:LED_TAB[5] &= ~(1<<0);break;
		case 32:LED_TAB[3] &= ~(1<<0);break;
		case 33:LED_TAB[2] &= ~(1<<0);break;
		case 34:LED_TAB[3] &= ~(1<<1);break;
		case 35:LED_TAB[5] &= ~(1<<1);break;
		case 36:LED_TAB[4] &= ~(1<<1);break;
		case 37:LED_TAB[2] &= ~(1<<6);break;
		case 38:LED_TAB[5] &= ~(1<<3);break;
		case 39:LED_TAB[2] &= ~(1<<4);break;
		case 40:LED_TAB[4] &= ~(1<<3);break;
		case 41:LED_TAB[4] &= ~(1<<4);break;
		case 42:LED_TAB[3] &= ~(1<<4);break;
		case 43:LED_TAB[5] &= ~(1<<4);break;
		case 44:LED_TAB[2] &= ~(1<<2);break;
		case 45:LED_TAB[2] &= ~(1<<3);break;
		case 46:LED_TAB[1] &= ~(1<<1);break;
		case 47:LED_TAB[2] &= ~(1<<1);break;
		case 48:LED_TAB[0] &= ~(1<<1);break;
		case 49:LED_TAB[6] &= ~(1<<1);break;
		default:break;

	}
}

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
	clear_led(licznik %50);
	licznik++;
	if (licznik>99999)
		licznik=0;
	set_led(licznik %50);
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
		set_led(licznik);
		clear_led(licznik-1);
        int_to_display(licznik,0,4);
		refresh_output();	
    }
}


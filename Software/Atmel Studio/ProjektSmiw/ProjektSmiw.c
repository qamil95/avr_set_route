/*
 * ProjektSmiw.c
 *
 * Created: 2016-11-23 13:02:00
 * Author: Kamil Ziêtek
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define delay_time 400 //odœwie¿anie wyœwietlaczy i diod

#define blue_button (!(PINC & (1<<PC5))) //niebieski przycisk na panelu, true jeœli wciœniêty
#define debugmode (!(PINB & (1<<PB7))) //tryb testowy, true jeœli SCK zwarte do masy
#define switch_up (PINA & (1<<PA7)) //prze³¹cznik na panelu, true jeœli w górze

uint16_t counter = 0;
uint8_t switches = 0x00; //stan 8 prze³¹czników przy diodach
uint8_t LED_TAB[7]; //stan ka¿dego LEDa
uint8_t DISPLAY_TAB[5]; //stan ka¿dego wyœwietlacza
const uint8_t DISPLAY_VALUE[] = { //wartoœci do wyœwietlacza siedmiosegmentowego
	0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x27,0x7F,0x6F, // 0 - 9
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
/* Sprawdzenie LEDa o podanym ID (true - wlaczony, false - zgaszony)    */
/************************************************************************/
uint8_t check_led(uint8_t id)
{
	switch (id)
	{
		case 1 :return LED_TAB[4] & (1<<6);
		case 2 :return LED_TAB[5] & (1<<6);
		case 3 :return LED_TAB[1] & (1<<6);
		case 4 :return LED_TAB[0] & (1<<6);
		case 5 :return LED_TAB[6] & (1<<6);
		case 6 :return LED_TAB[6] & (1<<5);
		case 7 :return LED_TAB[0] & (1<<5);
		case 8 :return LED_TAB[1] & (1<<5);
		case 9 :return LED_TAB[2] & (1<<5);
		case 10:return LED_TAB[4] & (1<<5);
		case 11:return LED_TAB[3] & (1<<5);
		case 12:return LED_TAB[5] & (1<<5);
		case 13:return LED_TAB[3] & (1<<6);
		case 14:return LED_TAB[3] & (1<<2);
		case 15:return LED_TAB[0] & (1<<2);
		case 16:return LED_TAB[6] & (1<<2);
		case 17:return LED_TAB[5] & (1<<2);
		case 18:return LED_TAB[6] & (1<<3);
		case 19:return LED_TAB[0] & (1<<3);
		case 20:return LED_TAB[3] & (1<<3);
		case 21:return LED_TAB[1] & (1<<3);
		case 22:return LED_TAB[1] & (1<<4);
		case 23:return LED_TAB[6] & (1<<4);
		case 24:return LED_TAB[0] & (1<<4);
		case 25:return LED_TAB[1] & (1<<2);
		case 26:return LED_TAB[4] & (1<<2);
		case 27:return LED_TAB[4] & (1<<0);
		case 28:return LED_TAB[1] & (1<<0);
		case 29:return LED_TAB[0] & (1<<0);
		case 30:return LED_TAB[6] & (1<<0);
		case 31:return LED_TAB[5] & (1<<0);
		case 32:return LED_TAB[3] & (1<<0);
		case 33:return LED_TAB[2] & (1<<0);
		case 34:return LED_TAB[3] & (1<<1);
		case 35:return LED_TAB[5] & (1<<1);
		case 36:return LED_TAB[4] & (1<<1);
		case 37:return LED_TAB[2] & (1<<6);
		case 38:return LED_TAB[5] & (1<<3);
		case 39:return LED_TAB[2] & (1<<4);
		case 40:return LED_TAB[4] & (1<<3);
		case 41:return LED_TAB[4] & (1<<4);
		case 42:return LED_TAB[3] & (1<<4);
		case 43:return LED_TAB[5] & (1<<4);
		case 44:return LED_TAB[2] & (1<<2);
		case 45:return LED_TAB[2] & (1<<3);
		case 46:return LED_TAB[1] & (1<<1);
		case 47:return LED_TAB[2] & (1<<1);
		case 48:return LED_TAB[0] & (1<<1);
		case 49:return LED_TAB[6] & (1<<1);
		default: return 0;
	}
}

/************************************************************************/
/* Odœwie¿enie prze³¹czników, 0-skrêt, 1-prosto                         */
/************************************************************************/
void refresh_switches()
{
	if (PINC & (1<<PC6))	switches |= (1<<0); else switches &= ~(1<<0);		
	if (!(PINC & (1<<PC7)))	switches |= (1<<1);	else switches &= ~(1<<1);	
	if (PINC & (1<<PC1))	switches |= (1<<2);	else switches &= ~(1<<2);		
	if (!(PINC & (1<<PC3)))	switches |= (1<<3);	else switches &= ~(1<<3);		
	if (!(PINC & (1<<PC2)))	switches |= (1<<4); else switches &= ~(1<<4);	
	if (PINC & (1<<PC4))	switches |= (1<<5); else switches &= ~(1<<5);	
	if (PIND & (1<<PD7))	switches |= (1<<6); else switches &= ~(1<<6);	
	if (!(PINC & (1<<PC0)))	switches |= (1<<7); else switches &= ~(1<<7);
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
        DISPLAY_TAB[i] = DISPLAY_VALUE[val%10];
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
	counter++;
	if (counter>99999)
		counter=0;
}

/************************************************************************/
/* Funkcja testuj¹ca dzia³anie p³ytki:                                  */
/* 1 - zapalenie wszystkich segmentów wyœwietlacza po kolei             */
/* 2 - wyœwietlenie wszystkich liczb na wyœwietlaczu                    */
/* 3 - zapalenie wszystkich diod i ich wygaszenie                       */
/* 4 - test prze³¹czników (wskazanie w któr¹ stronê skierowany switch)  */
/************************************************************************/
void test_board()
{
	set_interrupt_delay(500);
	// test 1
	counter = 0;
	while (counter < 7)
	{
		for (int i=0; i<5; i++)
		DISPLAY_TAB[i] = (1<<counter);
		refresh_output();
	}
	
	//test 2
	counter = 0;
	while (counter < 17)
	{
		for (int i=0; i<5; i++)
		DISPLAY_TAB[i] = DISPLAY_VALUE[counter];
		refresh_output();
	}
	
	//test 3
	counter = 0;
	while (counter < 50)
	{
		set_led(counter);
		int_to_display(counter, 3, 4);
		refresh_output();
	}
	
	//przejœcie dalej po wciœniêciu przycisku
	while (!blue_button)
		refresh_output();
			
	counter = 0;
	while (counter < 50)
	{
		clear_led(counter);
		int_to_display(counter, 3, 4);
		refresh_output();
	}
	DISPLAY_TAB[3] = DISPLAY_VALUE[16];
	DISPLAY_TAB[4] = DISPLAY_VALUE[16];
	
	//test 4
	cli();
	while (1)
	{
		refresh_switches();
		if (switches & (1<<0))
		{
			set_led(3);
			clear_led(37);
		} else
		{
			set_led(37);
			clear_led(3);
		}
		
		if (switches & (1<<1))
		{
			set_led(18);
			clear_led(38);
		} else
		{
			set_led(38);
			clear_led(18);
		}
			
		if (switches & (1<<2))
		{
			set_led(9);
			clear_led(39);
		} else
		{
			set_led(39);
			clear_led(9);
		}
			
		if (switches & (1<<3))
		{
			set_led(21);
			clear_led(40);
		} else
		{
			set_led(40);
			clear_led(21);
		}
		
		if (switches & (1<<4))
		{
			set_led(28);
			clear_led(44);
		} else
		{
			set_led(44);
			clear_led(28);
		}
		
		if (switches & (1<<5))
		{
			set_led(19);
			clear_led(45);
		} else
		{
			set_led(45);
			clear_led(19);
		}
		
		if (switches & (1<<6))
		{
			set_led(22);
			clear_led(46);
		} else
		{
			set_led(46);
			clear_led(22);
		}
		
		if (switches & (1<<7))
		{
			set_led(34);
			clear_led(47);
		} else
		{
			set_led(47);
			clear_led(34);
		}			
			
		if switch_up
		{
			DISPLAY_TAB[0] = DISPLAY_VALUE[16];
			DISPLAY_TAB[1] = DISPLAY_VALUE[16];
			DISPLAY_TAB[2] = DISPLAY_VALUE[16];
			DISPLAY_TAB[3] = DISPLAY_VALUE[8];
			DISPLAY_TAB[4] = DISPLAY_VALUE[8];
		} else 
		{
			DISPLAY_TAB[0] = DISPLAY_VALUE[8];
			DISPLAY_TAB[1] = DISPLAY_VALUE[8];
			DISPLAY_TAB[2] = DISPLAY_VALUE[8];
			DISPLAY_TAB[3] = DISPLAY_VALUE[16];
			DISPLAY_TAB[4] = DISPLAY_VALUE[16];
		}
		refresh_output();
	}
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
		DISPLAY_TAB[i] = DISPLAY_VALUE[16];

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
	/* Aktywacja trybu testowego jeœli SCK zwarte do masy                   */
	/************************************************************************/
	if debugmode
		test_board();
	else
	/************************************************************************/
	/* PROGRAM - g³ówna pêtla                                               */
	/************************************************************************/	
    while(1)
    {
		int_to_display(counter,0,4);
		refresh_output();		
    }
}


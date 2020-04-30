/********************************************
 *
 *  Name: Carol Liang
 *  Email: carollia@usc.edu
 *  Section: W5
 *  Assignment: Project - Music Box
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <stdio.h>

#include "lcd.h"
#include "adc.h"

#define NUM_NOTES 21
#define NUM_TONES 26

/*
   The note_freq array contains the frequencies of the notes from C3 to C5 in
   array locations 1 to 25.  Location 0 is a zero for rest(?) note.  Used
   to calculate the timer delays.
   */
unsigned int note_freq[NUM_TONES] =
{ 0,   131, 139, 147, 156, 165, 176, 185, 196, 208, 220, 233, 247,
	262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494, 523};

/* Some sample tunes for testing */
/*
   The "notes" array is used to store the music that will be played.  Each value
   is an index into the tone value in the note_freq array.
   */

// G, A, F, F(octive lower), C     Close Encounters
//unsigned char notes[NUM_NOTES] = {20, 22, 18, 6, 13};

// D D B C A B G F# E D A C A B    Fight On
//unsigned char notes[NUM_NOTES] = {15, 15, 12, 13, 10, 12, 8, 7, 5, 3, 10, 13, 10, 12};

// E E F G G F E D C C D E E D D   Ode to Joy
unsigned char notes[NUM_NOTES] = {17, 17, 18, 20, 20, 18, 17, 15, 13, 13, 15, 17, 17, 15, 15 };

void play_note(unsigned short);
void variable_delay_us(int);

int main(void) {
	// Initialize various modules and functions and variables
	lcd_init();
	adc_init();
	lcd_writecommand(1);
	DDRB |= (1 << PB4);
	PORTC |= ((1 << 1) | (1 << 5));
	int lcd_col = 1;
	int page_num = 1;
	char initial_page[16] = " E E F G G F E >";
	char second_page[16] = "<D C C D E E D >";
	char last_page[16] = "<D D D D D D ";

	// Show splash screen
	lcd_stringout("Carol Liang");
	_delay_ms(1000);
	lcd_writecommand(1);
	lcd_moveto(0,0);
	lcd_stringout("should move");
	_delay_ms(500);
	lcd_moveto(0,0);
	lcd_stringout(initial_page);


	/*int i;
	for (i = 0; i < NUM_TONES; i++) {
		play_note(note_freq[i]);
	}*/

	while (1) {
		unsigned char curadc = adc_sample(0);
		/* Check if a button on the LCD was pressed */
		if (curadc > 0 && curadc < 5) {
			_delay_ms(200);
			lcd_col += 2;
			if (lcd_col > 15) {
				lcd_col = 1;
				lcd_moveto(0,0);
				if (page_num == 1) {
					lcd_stringout(second_page);
					page_num += 1;
				} else if (page_num == 2) {
					lcd_stringout(last_page);
					page_num += 1;
				}
			}
			lcd_moveto(0, lcd_col);
		} else if (curadc > 154 && curadc < 160) {
			_delay_ms(200);
			lcd_col -= 2;
			if (lcd_col < 0) {
				lcd_col = 15;
				lcd_moveto(0,0);
				if (page_num == 2) {
					lcd_stringout(initial_page);
					page_num -= 1;
				} else if (page_num == 3) {
					lcd_stringout(second_page);
					page_num -= 1;
				}
			}
			lcd_moveto(0, lcd_col);
		}

		/* If rotary encoder was rotated, change note tone */

	}

	while (1) {                 // Loop forever


	}
}


/* ------------------------------------------------------------------ */

/*
   Code for showing notes on the screen and playing the notes.
   */





/* ------------------------------------------------------------------ */

/*
   Code for initializing TIMER1 and its ISR
   */




ISR(TIMER1_COMPA_vect)
{



}

/* ------------------------------------------------------------------ */

/*
   Code for initializing TIMER2
   */

void play_note(unsigned short freq)
{
	unsigned long period;

	period = 1000000 / freq;      // Period of note in microseconds

	while (freq--) {
		// Make PB4 high
		PORTB |= (1 << PB4);

		// Use variable_delay_us to delay for half the period
		variable_delay_us(period/2);

		// Make PB4 low
		PORTB &= ~(1 << PB4);

		// Delay for half the period again
		variable_delay_us(period/2);
	}
}

/*
   variable_delay_us - Delay a variable number of microseconds
   */
void variable_delay_us(int delay)
{
	int i = (delay + 5) / 10;

	while (i--)
		_delay_us(10);
}



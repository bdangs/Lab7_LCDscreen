/*	Author: Bryan Dang
 *  Partner(s) Name: 
 *	Lab Section: 021
 *	Assignment: Lab #7  Exercise #0
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

// could not get LCD screen to work, but can see LCD screen blinking to indicate change
// https://drive.google.com/file/d/1HB0DzIlQlXlhroGo9F9HHTH_HT-LDqAn/view?usp=sharing
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif
#include "io.h"

enum state{start, increment, decrement, reset, wait} state;
unsigned char count = 0;

void Tick(){
	unsigned char A0 = ~PINA & 0x01;
	unsigned char A1 = ~PINA & 0x02;
	
	switch(state){
		case start:
			state = wait;
			break;

		case wait:
			if (A0 && A1){
				state = reset;
			} else if (A0) {
				state = increment;
			} else if (A1) {
				state = decrement;
			}
			break;

		case reset:
			state = wait;
			break;
		
		case increment: 
			state = wait;
			break;

		case decrement:
			state = wait;
		
		default:
			break;

	}

	switch (state){
		case start:
			break;
		case wait:
			break;
		case increment:
			if (count < 9){
				count++;
			}
			break;
		case decrement:
			if (count > 0){
				count--;
			}
			break;
		case reset:
			count = 0x00;
			break;
		default:
			break;
	}
}

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cnt = 0;

void TimerOn(){
	_avr_timer_cnt = _avr_timer_M;
}

void TimerOff(){
	TCCR1B = 0x00;
}

void TimerISR(){
	TimerFlag = 1;
}

void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cnt = _avr_timer_M;
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
    /* Insert your solution below */
	TimerSet(1000);
	LCD_init();
	TimerOn();

    while (1) {
	Tick();
	LCD_ClearScreen();
	LCD_Cursor(2);
	LCD_WriteData(count + '0');
	while(!TimerFlag);
	TimerFlag = 0;
	continue;
    }
    return 1;
}

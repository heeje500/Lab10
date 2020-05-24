/*	Author: Heeje Lee
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #10  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include <avr/interrupt.h>
#endif


volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {
	TCCR1B = 0x0B;
	OCR1A = 125; 
	TIMSK1 = 0x02;
	TCNT1 = 0;
	_avr_timer_cntcurr = _avr_timer_M;
	SREG |= 0x80;

}

void TimerOff() {
	TCCR1B = 0x00;
}

void TimerISR() {
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect) {
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0) {
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

void tickT();
void tickB();
void tickC();

enum Tstates{Tstart, led0, led1, led2}Tstate;
enum Bstates{Bstart, on, off}Bstate;
enum Cstates{Cstart, out}Cstate;

unsigned char temp = 0x00;
unsigned char blink = 0x00;
unsigned char threeLEDs = 0x00;

unsigned short count1 = 0x00;
unsigned short count2 = 0x00;

void tickT() {
	switch(Tstate) { //transitions
		case Tstart:
			Tstate = led0;
			break;

		case led0:
			if(count1 < 0x01) {
				Tstate = led0;
				count1++;
			}

			else {
				Tstate = led1;
				count1 = 0;
			}
			break;

		case led1: 
			if(count1 < 0x01) {
				Tstate = led1;
				count1++;
			}

			else {
				Tstate = led2;
				count1 = 0;
			}
			break;

		case led2:
			if (count1 < 0x01) {
				Tstate = led2;
				count1++;
			}

			else {
				Tstate = led0;
				count1 = 0;
			}
			break;

		default:
			break;
}

	switch(Tstate) { //actions
		case Tstart:
			break;

		case led0:
			threeLEDs = 0x01;
			break;

		case led1: 
			threeLEDs = 0x02;
			break;

		case led2:
			threeLEDs = 0x04;
			break;

		default:
			break;
		}
}

void tickB() {
	switch(Bstate) { //transitions
		case Bstart:
			Bstate = on;
			break;

		case on:
			if(count2 < 0x01) {
				Bstate = on;
				count2++;
			}

			else {
				Bstate = off;
				count2 = 0;
			}
			break;

		case off:
			if(count2 < 0x01) {
				Bstate = off;
				count2++;
			}

			else {
				Bstate = on;
				count2 = 0;
			}
			break;

		default:
			break;
	}

	switch(Bstate) {
		case Bstart:
			break;

		case on:
			blink = 0x08;
			break;

		case off:
			blink = 0x00;
			break;

		default:
			break;
	}
}

void tickC() {
	switch(Cstate) {//transistions
		case Cstart:
			Cstate = out;
			break;

		case out:
			break;

		default:
			break;
	}

	switch(Cstate) {
		case Cstart:
			break;

		case out:
			PORTB = threeLEDs | blink;
			break;
	}

}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRB = 0xFF; PORTB = 0x00;

	Tstate = Tstart;
	Bstate = Bstart;
	Cstate = Cstart;

	TimerSet(1000);
	TimerOn();

    /* Insert your solution below */
    while (1) {
	tickT();
	tickB();
	tickC();

	while(!TimerFlag);
	TimerFlag = 0;
    }
    return 1;
}

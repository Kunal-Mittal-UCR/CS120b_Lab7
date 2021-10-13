/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include "timer.h"
#ifdef _SIMULATE_
#include "io.h"
#include "simAVRHeader.h"
#endif


enum States{init, increment, decrement, reset, wait} counterState;
unsigned int i = 0;
unsigned char tmpA = 0x00;
unsigned char tmpB = 0x00;
unsigned char counter = 0;
unsigned char holdPeriod = 0;



void tick(){
	switch(counterState){
		case init:
			counterState = wait;
			counter = 0;
			holdPeriod = 0;
			break;
			
		case wait:
			if(tmpA & 0x01 && tmpA & 0x02){
				counterState = reset;
				counter = 0;
			}
			else if(tmpA & 0x01){
				counterState = increment;
				counter++;
			}
			else if(tmpA & 0x02){
				counterState = decrement;
				counter--;
			}
			break;
			
		case increment:
			if(tmpA & 0x01 && tmpA & 0x02){
				counterState = reset;
				counter = 0;
			}
			else if(tmpA & 0x01){
				counterState = increment;
				holdPeriod++;
			}
			else
				counterState = wait;
			if(holdPeriod > 5){
				counter++;
				holdPeriod = 0;
			}
			break;
			
		case decrement:
			if(tmpA & 0x01 && tmpA & 0x02){
				counterState = reset;
				counter = 0;
			}
			else if(tmpA & 0x02){
				counterState = decrement;
				holdPeriod++;
			}
			else
				counterState = wait;
			if(holdPeriod > 5){
				counter--;
				holdPeriod = 0;
			}
			break;
		case reset:
			if(tmpA & 0x01 && tmpA & 0x02){
				counterState = reset;
				counter = 0;
			}
			else
				counterState = wait;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRC  = 0xFF;
	PORTC = 0x00;
	DDRB  = 0xFF;
	PORTB = 0x00;
	TimerSet(200);
	TimerOn();
	LCD_init();
    /* Insert your solution below */
    while (1) {
		PORTA = PINA;
		tmpA = PORTA;
		tick();
		tmpB = counter;
		PORTB = tmpB;
		while(!TimerFlag);
		TimerFlag = 0;
    }
    return 1;
}

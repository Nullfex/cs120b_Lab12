/*	Author: Daniel Cook - Dcook020@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section:021
 *	Assignment: Lab #12  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *	
 *	Demo Link: https://www.youtube.com/watch?v=ZGhhAs5d39Q
 *
 */
#include "timer.h"
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "scheduler.h"
#endif

unsigned char pattern = 0xFF;
unsigned char row = 0x1E;

enum UpDownStates{start,waitpress,waitrelease}updownstate;

void UpDownSM(){
unsigned char aval = ~PINA;

switch(updownstate){
	case start:
	updownstate = waitpress;
	break;

	case waitpress:
	if(aval == 0x01){
	if((row&0x1F)!=0x1E){	
	row = (row>>1)|0x10;
	}	
	updownstate = waitrelease;	
	}else if(aval == 0x02){
	if((row&0x1F)!=0x0F){	
	row = (row<<1)|0x01;
	}	
	updownstate = waitrelease;	
	}else{
	updownstate = waitpress;
	}
	break;	
	
	case waitrelease:
	if(aval == 0x00){
	updownstate = waitpress;
	}else{
	updownstate = waitrelease;
	}
}
PORTD = row;
}


int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
    /* Insert your solution below */
	PORTC = pattern;
	PORTD = row;
	updownstate = start;
    while (1){
	UpDownSM();

	//PORTD = 0x0F;
    }    


    return 1;
}

/*	Author: Daniel Cook - Dcook020@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section:021
 *	Assignment: Lab #12  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Demo Link: https://www.youtube.com/watch?v=CCh2o6aR3DQ
 */
#include "timer.h"
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "scheduler.h"
#endif

unsigned char pattern = 0x01;
unsigned char row = 0x00;

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
	}else if(aval == 0x04){
	if(pattern!=0x01){	
	pattern = (pattern >> 1);
	}
	updownstate = waitrelease;
	}else if(aval == 0x08){
	if(pattern!=0x80){	
	pattern = (pattern << 1);
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
PORTC = pattern;
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
	TimerSet(50);
	TimerOn();
    while (1){
	UpDownSM();
	while(!TimerFlag);
	TimerFlag = 0;

    }    


    return 1;
}

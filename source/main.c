/*	Author: Daniel Cook - Dcook020@ucr.edu
 *  Partner(s) Name: 
 *	Lab Section:021
 *	Assignment: Lab #12  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include "timer.h"
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "scheduler.h"
#endif

unsigned char pattern[] = {0x3C,0x24};
unsigned char row[] = {0x15,0x1B};
unsigned char whichregister = 0x00;
enum UpDownStates{start,waitpress,waitrelease}updownstate;

void UpDownSM(){
unsigned char aval = ~PINA;

switch(updownstate){
	case start:
	updownstate = waitpress;
	break;

	case waitpress:
	if(aval == 0x01){
	if((row[0]&0x1F)!=0x1A){	
	row[0] = (row[0]>>1)|0x10;
	row[1] = (row[1]>>1)|0x10;
	}	
	updownstate = waitrelease;//up	
	}else if(aval == 0x02){
	if((row[0]&0x1F)!=0x0B){	
	row[0] = (row[0]<<1)|0x01;
	row[1] = (row[1]<<1)|0x01;
	}
	updownstate = waitrelease;//down	
	}else if(aval == 0x04){
	if(pattern[0]!=0x0F){	
	pattern[0] = (pattern[0] >> 1);
	pattern[1] = (pattern[1] >> 1);
	}
	updownstate = waitrelease;
	}else if(aval == 0x08){
	if(pattern[0]!=0xF0){
	pattern[0] = (pattern[0] << 1);	
	pattern[1] = (pattern[1] << 1);
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
}

void transmit_data(unsigned char data) {
    int i;
    for (i = 0; i < 8 ; ++i) {
   	 // Sets SRCLR to 1 allowing data to be set
   	 // Also clears SRCLK in preparation of sending data
	if(whichregister == 0x00){
   	 PORTC = 0x08;
	}else{
	PORTC = 0x20;
	 }
   	 // set SER = next bit of data to be sent.
   	 PORTC |= ((data >> i) & 0x01);
   	 // set SRCLK = 1. Rising edge shifts next bit of data into the shift register
   	 PORTC |= 0x02;
    }
    // set RCLK = 1. Rising edge copies data from “Shift” register to “Storage” register
    if(whichregister == 0x00){
    PORTC |= 0x04;
    }else{
    PORTC |= 0x10;
    }
    // clears all lines in preparation of a new transmission
    PORTC = 0x00;
    if(whichregister == 0x00){
    whichregister = 0x01;
    }else{
    whichregister = 0x00;
    }
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
    /* Insert your solution below */
	PORTC = pattern[0];
	PORTD = row[0];
	//updownstate = start;
	TimerSet(2);
	TimerOn();
    while (1){
	transmit_data(pattern[0]);
    	transmit_data(row[0]);
	while(!TimerFlag);
	TimerFlag = 0;
	transmit_data(pattern[1]);
        transmit_data(row[1]);
	//PORTC = pattern[0];
	//PORTD = row[0];	
	UpDownSM();
	while(!TimerFlag);
	TimerFlag = 0;

    }    


    return 1;
}

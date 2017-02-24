#include <stdint.h>
#include "msp432p401r.h"
#include "ClockSystem.h"
#include <stdio.h>
//#include "SysTick.h"


void SysTick_Init( )
{
	SYSTICK_STCSR = 0; // 1) disable SysTick during setup
	SYSTICK_STRVR = 0x00FFFFFF; // 2) maximum reload value
	SYSTICK_STCVR = 0; // 3) any write to current clears it
	SYSTICK_STCSR = 0x00000005; // 4) enable SysTick with core clock
}

void Init_GPIO(){
	flush_Ports();
	Button_Setup();
	LED_Setup();
	Sensor_Setup();
}

void Set_FSM_Output(uint8_t Out){

	// setup Nred -> 3.3 N Yellow -> 4.3 N Green 1.5
	//Ered -> 4.6 Eyellow -> 6.5 Egreen -> 6.4
	//port 2-> walk sign
	if(Out == 0x00){	//goEast
		P1OUT &= 0x00;
		P2OUT &= 0x00;
		P3OUT &= 0x00;
		P4OUT &= 0x00;
		P6OUT &= 0x00;

		P6OUT |= 0x10;  // East Light : State is Green
		P2OUT |= 0x01;  // Walk Light : State is Red
		P3OUT |= 0x08; // North Light : State is Red
	}
	else if(Out == 0x01){//slowEast
		P1OUT &= 0x00;
		P2OUT &= 0x00;
		P3OUT &= 0x00;
		P4OUT &= 0x00;
		P6OUT &= 0x00;

		P6OUT |= 0x20;  //East Light : State is Yellow
		P2OUT |= 0x01;  //Walk Light : State is Red
		P3OUT |= 0x08; //North Light : State is Red
	}
	else if(Out == 0x02){//waitNorth
		P1OUT &= 0x00;
		P2OUT &= 0x00;
		P3OUT &= 0x00;
		P4OUT &= 0x00;
		P6OUT &= 0x00;

		P4OUT |= 0x40;  //East Light : State is Red
		P2OUT |= 0x01;  //Walk Light : State is Red
		P3OUT |= 0x08; //North Light : State is Red
	}
	else if(Out == 0x03){ // goNorthWalkOn_28s
		P1OUT &= 0x00;
		P2OUT &= 0x00;
		P3OUT &= 0x00;
		P4OUT &= 0x00;
		P6OUT &= 0x00;

		P4OUT |= 0x40;  //East Light : State is Red
		P2OUT |= 0x02;  //Walk Light : State is Green
		P1OUT |= 0x20; //North Light : State is Green
	}
	else if(Out ==0x04 ){ // goNorthWalkOff
		P1OUT &= 0x00;
		P2OUT &= 0x00;
		P3OUT &= 0x00;
		P4OUT &= 0x00;
		P6OUT &= 0x00;

		P4OUT |= 0x40;  //East Light : State is Red
		P2OUT |= 0x01;  //Walk Light : State is Red
		P1OUT |= 0x20; //North Light : State is Green
	}
	else if(Out == 0x05){ // goNorthWalkOn
		P1OUT &= 0x00;
		P2OUT &= 0x00;
		P3OUT &= 0x00;
		P4OUT &= 0x00;
		P6OUT &= 0x00;

		P4OUT |= 0x40;  //East Light : State is Red
		P2OUT |= 0x02;  //Walk Light : State is Green
		P1OUT |= 0x20; //North Light : State is Green
	}
	else if(Out == 0x06){ // slowNorthWalkOff
		P1OUT &= 0x00;
		P2OUT &= 0x00;
		P3OUT &= 0x00;
		P4OUT &= 0x00;
		P6OUT &= 0x00;

		P4OUT |= 0x40;  //East Light : State is Red
		P2OUT |= 0x01;  //Walk Light : State is Red
		P4OUT |= 0x08; //North Light : State is Yellow
	}
	else if(Out == 0x07){ // waitEast
		P1OUT &= 0x00;
		P2OUT &= 0x00;
		P3OUT &= 0x00;
		P4OUT &= 0x00;
		P6OUT &= 0x00;

		P4OUT |= 0x40;  //East Light : State is Red
		P2OUT |= 0x01;  //Walk Light : State is Red
		P3OUT |= 0x08; //North Light : State is Red
	}
	else{//if(Out == 0x08)	// goEast_28s
		P1OUT &= 0x00;
		P2OUT &= 0x00;
		P3OUT &= 0x00;
		P4OUT &= 0x00;
		P6OUT &= 0x00;

		P6OUT |= 0x10; //East Light : State is Green
		P2OUT |= 0x01; //Walk Light : State is Red
		P3OUT |= 0x08;//North Light : State is Red
	}
}

void SysTick_Wait10ms(uint32_t Time)
{
	SYSTICK_STRVR = Time-1; // number of clock cycles to wait
	SYSTICK_STCVR = 0; // clear the current count

	while( (SYSTICK_STCSR&0x00010000) == 0 )
	{
	// wait for count flag to be asserted

	}
}

uint8_t read_sensors()
{
	P5OUT |= 0x30;

	P5OUT &= 0xCF;


	//P5OUT &= 0xCF;
	P5OUT |= 0x30;
	uint8_t total = 0x00;
	//read in sensors
	uint8_t bool = 0x01;
	uint8_t sensorValue;
	int faraway = 0;
	while(bool == 0x01){
		SysTick_Wait10ms(18231);
		sensorValue = P4IN & 0x30;
		//if there is a long delay until a reciever, ignore
		if(sensorValue == 0x00){
			faraway++;
			if(faraway == 10){
				bool = 0x00;
			}
		}
		else{
			//
			printf("found something");
			if(sensorValue && 0x30){
				total = 0x06;
				bool = 0x00;
			}
			else if(sensorValue && 0x20){
				total = 0x04;
				bool = 0x00;
			}
			else{
				total = 0x02;
				bool = 0x00;
			}
		}
	}
	printf("done");
	uint8_t W = P4IN && 0x01;

	total += W;
	return total;
}

void Button_Setup()
{ // setup
	P4DIR &= 0x00;
	P4SEL0 &= 0x00;
	P4SEL1 &= 0x00;
	P4REN &= 0x00; //2
	P4OUT &= 0x01; //2
}

void LED_Setup()//6.5, 6.4, 4.6,
{// setup Nred -> 3.3 N Yellow -> 4.3 N Green 1.5 Ered -> 4.6 Eyellow -> 6.5 Egreen -> 6.4 port 2-> walk sign
	P6DIR |= 0x30;
	P6SEL0 |= 0x00;
	P6SEL1 |= 0x00;
	P6REN |= 0x00;

	P4DIR |= 0x48;
	P4SEL0 |= 0x00;
	P4SEL1 |= 0x00;
	P4REN |= 0x00;

	P1DIR |= 0x20;
	P1SEL0 |= 0x00;
	P1SEL1 |= 0x00;
	P1REN |= 0x00;

	P3DIR |= 0x08;
	P3SEL0 |= 0x00;
	P3SEL1 |= 0x00;
	P3REN |= 0x00;

	P2DIR |= 0x07;
	P2SEL0 |= 0x00;
	P2SEL1 |= 0x00;
	P2REN |= 0x00;
}

void Sensor_Setup()//east sensor 5.5 north sensor 5.4
{// setup
	P5DIR |= 0x30;
	P5SEL0 |= 0x00;
	P5SEL1 |= 0x00;
	P5REN |= 0x00;
	P5OUT |= 0x00;

	//east input 4.5 north input 4.4

	//P5OUT &= CF;
}

void flush_Ports()
{
	P1DIR &= 0x00;
	P1SEL0 &= 0x00;
	P1SEL1 &= 0x00;
	P1REN &= 0x00;
	P1OUT &= 0x00;

	P2DIR &= 0x00;
	P2SEL0 &= 0x00;
	P2SEL1 &= 0x00;
	P2REN &= 0x00;
	P2OUT &= 0x00;

	P3DIR &= 0x00;
	P3SEL0 &= 0x00;
	P3SEL1 &= 0x00;
	P3REN &= 0x00;
	P3OUT &= 0x00;

	P4DIR &= 0x00;
	P4SEL0 &= 0x00;
	P4SEL1 &= 0x00;
	P4REN &= 0x00;
	P4OUT &= 0x00;
}

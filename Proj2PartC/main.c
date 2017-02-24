//*****************************************************************************
//
// MSP432 main.c template - Empty main
//
//****************************************************************************

#include <stdint.h>
#include "msp432p401r.h"
#include "ClockSystem.h"
#include <stdio.h>
#define buttonInput (HWREG8(0x40004C00))

void Initialize_Port1(void)
{ // setup
	P1DIR |= 0x00;
	P1SEL0 |= 0x00;
	P1SEL1 |= 0x00;
	P1REN |= 0x12;
	P1OUT |= 0x12;
}

void Initialize_Port2(void)
{// setup
	P2DIR |= 0x07;
	P2SEL0 |= 0x00;
	P2SEL1 |= 0x00;
	P2REN |= 0x00;
	//P2OUT |= 0x07;
	P2OUT &= 0x01;
}

void Initialize_Port3(void)
{ // setup
	P3DIR |= 0x40;
	P3SEL0 |= 0x00;
	P3SEL1 |= 0x00;
	P3REN |= 0x00;
	P3OUT &= 0x40;
}

void Initialize_Port1_output(void)
{ // setup
	P1DIR |= 0x01;
	P1SEL0 |= 0x00;
	P1SEL1 |= 0x00;
	P1REN |= 0x00;
	P1OUT |= 0x01;
}

uint8_t Read_Port1(void)
{

}



void Write_Port2(uint8_t value)
{

}

void SysTick_Init( )
{
	SYSTICK_STCSR = 0; // 1) disable SysTick during setup
	SYSTICK_STRVR = 0x00FFFFFF; // 2) maximum reload value
	SYSTICK_STCVR = 0; // 3) any write to current clears it
	SYSTICK_STCSR = 0x00000005; // 4) enable SysTick with core clock
}

//Delay is in units of clock cycles
void SysTick_Wait(uint32_t cycles)
{
	SYSTICK_STRVR = cycles-1; // number of clock cycles to wait
	SYSTICK_STCVR = 0; // clear the current count

	while( (SYSTICK_STCSR&0x00010000) == 0 )
	{
	// wait for count flag to be asserted

	}
}

int main(void)
{
	Initialize_Port1();
	Initialize_Port2();
	Initialize_Port3();
	//Initialize_Port1_output();

	char pressedButtons;
	char heartbeat;
	char redLED;
	int i;
	int startTime;
	int endTime;
	int totalTime;
	int time = 0;

	uint32_t DataBuffer[20];
	//allocate space for debug array 3 seconds worth of input/output data
	int32_t TimeBuffer[20];
	//allocate 3 seconds of timer data

	//Clock_Init48MHz();
	Clock_Init32kHz();
	//legend has it 48mhz does not work

	SysTick_Init();

	//P2OUT |= 0x04;
	i = 0;
	while(1){// This is oscillating part 2hj
			//initializes the heartbeat  change to port 2
			//start time
			SysTick_Init();
			heartbeat = P2OUT & 0x02;
		/*

			if(heartbeat == 0x01){
				P1OUT &= 0x02;
			}
			else{
				P1OUT |= 0x01;
			}

		*/
			pressedButtons = buttonInput & 0x02;
			redLED = P2OUT &= 0x01;
			if(pressedButtons != 0x02){// active low button pressed
				//2ms = 6 cycles
				//port 1 pin 5 on similarly as the button
				P3OUT &= 0x00;
				if(redLED != 0x01 && heartbeat == 0x02){ //if the led is not on and the heartbeat is on just turn the led on
					P2OUT &= 0x00; //flush p2 out
					P2OUT |= 0x01;
					DataBuffer[i] = 0x11;
				}
				else if(redLED != 0x01 && heartbeat != 0x02){ //if the led is not on and the heartbeat is off turn the led and the heartbeat on
					P2OUT |= 0x03;
					DataBuffer[i] = 0x11;
				}
				else{
					if(heartbeat == 0x02){
						P2OUT &= 0x00; //if the led is on and the heartbeat is on turn heartbeat off and led off
					}
					else{
						P2OUT |= 0x02; //if the led is on and the heartbeat is off turn heartbeat on and led off
					}
					DataBuffer[i] = 0x10;
				}
			}
			else{
				P3OUT |= 0x40;
				if(heartbeat == 0x02){
					P2OUT &= 0x00;
					P2OUT |= 0x01;
					DataBuffer[i] = 0x01;
				}
				else{
					P2OUT |= 0x03;
					DataBuffer[i] = 0x01;
				}
			}
			endTime = 0x00FFFFFF - SYSTICK_STRVR;
			time += endTime;
			SysTick_Init();
			SysTick_Wait(187050);
			endTime = 0x00FFFFFF - SYSTICK_STRVR;
			time += endTime;
			TimeBuffer[i] = time;

			if(i == 20){
				i = 0;
			}
			else{
				i++;
			}
		}
	

	
	return 0;
}


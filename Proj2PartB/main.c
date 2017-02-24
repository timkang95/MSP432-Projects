//*****************************************************************************
//
// MSP432 main.c template - Empty main
//
//****************************************************************************

#include <stdint.h>
#include "msp432p401r.h"
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


uint8_t Read_Port1(void)
{

}



void Write_Port2(uint8_t value)
{

}


int main(void)
{
	Initialize_Port1();
	Initialize_Port2();
	Initialize_Port3();

	char pressedButtons;
	int i;


	while(1){// This is oscillating part 2

			pressedButtons = buttonInput & 0x02;
			if(pressedButtons != 0x02){// active low
				//2ms = 6 cycles
				//port 1 pin 5 on similarly as the button
				P3OUT &= 0x00;
				if(P2OUT != 0x01){
					//for(i = 0; i < 125; i++){
					for(i = 0; i < 8063; i++){ //for 8htz
						P2OUT |= 0x01;
					}


				}
				else{
					//for(i = 0; i < 125; i++){
					for(i = 0; i < 8063; i++){ //for 8 htz
						P2OUT &= 0x00;
					}


				}
			}
			else{
				P3OUT |= 0x40;
				P2OUT |= 0x01;
			}

		}
	

	
	return 0;
}


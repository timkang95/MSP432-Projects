//*****************************************************************************
//
// MSP432 main.c template - Empty main

//PROJECT 3
//
// LED~ 6.5, 6.4, 4.6,1.5, 4.3, 3.3

//Goals For tomorrow: #SquadGoals
// Setup board for leds, make it look pretty
// bring a wire stripper
// get ultrasound working
// thats it

//****************************************************************************

#include <stdint.h>
#include "msp432p401r.h"
#include "ClockSystem.h"
#include <stdio.h>
//#include "SysTick.h"
#include "Port_Functions.h"

struct State
{
	uint8_t Out; // 8-bit output
	uint32_t Time; //Units of 10ms
	const struct State *Next_State_ptr[8]; // depends on 2-bit input from sensor
};

typedef const struct State StateType;

#define goEast &FSM[0] 			// green East Light
#define slowEast &FSM[1]		// yellow East Light
#define waitNorth &FSM[2]		// All Red Lights
#define goNorthWalkOn_28s &FSM[3]			// Green North Light, green Walk Light
#define goNorthWalkOff &FSM[4]		// Yellow North Light, red Walk Light
#define goNorthWalkOn &FSM[5]		// All Red Lights
#define slowNorthWalkOff &FSM[6]	// Green North Light, walk off, 2sec
#define WaitEast &FSM[7]	// Green North Light, walk on, 2sec
#define goEast_28s &FSM[8]		// Green East Light, walk off, 2sec

StateType FSM[9] = { //all in milliseconds below
		{0x00, 2000,{goEast, slowEast, goEast, slowEast, slowEast, slowEast, slowEast, slowEast }}, 	//goEast
		{0x01, 2000,{waitNorth, waitNorth, waitNorth,waitNorth, waitNorth,waitNorth,waitNorth,waitNorth}}, 	//slowEast
		{0x02, 2000,{goNorthWalkOn_28s, goNorthWalkOn_28s, goNorthWalkOn_28s,
				     goNorthWalkOn_28s,goNorthWalkOn_28s, goNorthWalkOn_28s, goNorthWalkOn_28s,goNorthWalkOn_28s}}, //waitNorth
		{0x03, 28000,{goNorthWalkOff, goNorthWalkOff, goNorthWalkOff, goNorthWalkOff,
				goNorthWalkOff, goNorthWalkOff, goNorthWalkOff, goNorthWalkOff}},//goNorthWalkOn_28s
		{0x04, 2000,{goNorthWalkOn, goNorthWalkOn, goNorthWalkOn, goNorthWalkOn,
				goNorthWalkOn, goNorthWalkOn, goNorthWalkOn, goNorthWalkOn}},	//goNorthWalkOff
		{0x05, 2000,{goNorthWalkOff, goNorthWalkOff, slowNorthWalkOff, slowNorthWalkOff,
			  goNorthWalkOff, goNorthWalkOff, slowNorthWalkOff, slowNorthWalkOff}},	//goNorthWalkOn
		{0x06, 2000,{WaitEast, WaitEast, WaitEast, WaitEast, WaitEast, WaitEast, WaitEast, WaitEast}},	//slowNorthWalkOff
		{0x07, 2000,{goEast_28s, goEast_28s, goEast_28s, goEast_28s, goEast_28s, goEast_28s, goEast_28s, goEast_28s}},	//WaitEast
		{0x08, 28000,{goEast, goEast, goEast, goEast, goEast, goEast, goEast, goEast}}//goEast_28s
};

/*
 * data structure
 * Struct {
 *
 * }
 * typedef struct student student_type;
 *
 *
 * states
 *
 *  GO  NORTH N green E red 30 seconds  |   no change if no cars are detected then repeat
 * WAIT NORTH N yellow E red 5 seconds  V
 *  GO   EAST N red E green
 * WAIT  EAST N red E yellow
 */



void main(void)
{
	/*
	Init_GPIO();
	//Clock_Init32kHz();

	while(1){
		if(P4IN &= 0x01){
			P2OUT |= 0x01;
			P6OUT |= 0x30;
			P4OUT |= 0x48;
			P1OUT |= 0x20;
			P3OUT |= 0x08;
			P5OUT &= 0x00;
		}
		else{
			P2OUT &= 0x00;
			P6OUT &= 0x00;
			P4OUT &= 0x00;
			P1OUT &= 0x00;
			P3OUT &= 0x00;
			P5OUT |= 0x30;
		}
	}

	P4IN = 0x00;
	
	*/
	StateType *Current_State_Ptr; //state pointer
	uint8_t FSM_Input;

	// initialize SysTick timer
	SysTick_Init();

	//Initialize GPIO inputs and outputs
	Init_GPIO();

	// initial state: Green north; Red east
	Current_State_Ptr = goEast;
	int testState[9] = {4, 0, 0, 0, 0, 2, 0, 0, 0};
	int i = 0;

	while(1)
	{
		FSM_Input = read_sensors();
		if(FSM_Input && 0x02){
			printf("\neast detected");
		}
		else
			printf("\nnope");
		//printf("%d", Current_State_Ptr->Out);
		// set lights to current state's Out value
		/*Set_FSM_Output(Current_State_Ptr->Out);

		// wait 10 ms * current state's Time value
		SysTick_Wait10ms(Current_State_Ptr->Time);

		printf("\n%d, %d", Current_State_Ptr->Out, testState[i]);
		Current_State_Ptr = Current_State_Ptr->Next_State_ptr[ testState[i] ];

		i++;
		if(i>= 9){
			i = 0;
		}

		// get new input from car detectors
		/*FSM_Input = read_sensors();

		// transition to next state
		Current_State_Ptr = Current_State_Ptr->Next_State_ptr[FSM_Input];
		*/

	}

}


















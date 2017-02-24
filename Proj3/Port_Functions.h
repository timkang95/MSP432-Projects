/*
 * Port_Functions.h
 *
 *  Created on: Feb 19, 2017
 *      Author: tik12
 */

#ifndef PORT_FUNCTIONS_H_
#define PORT_FUNCTIONS_H_
void SysTick_Init();
void Set_FSM_Output(uint8_t Out);
void SysTick_Wait10ms(uint32_t Time);
uint8_t read_sensors();
void Button_Setup();
void LED_Setup();
void Sensor_Setup();
void flush_Ports();



#endif /* PORT_FUNCTIONS_H_ */

/*
 * fan.c
 *
 * Created: 18.10.2018 12:42:24
 *  Author: fly8r
 */
#include "include/fan.h"

/************************************************************************/
/* Local VARS                                                           */
/************************************************************************/
volatile	static		uint8_t		FSM_state;

/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/
void FSM_FAN_RPM2String(void);

void FSM_FAN_Init(void)
{
	// Initialize counter
	fan.counter=0;
	// Flush _active flag
	fan._active=0;
	// Flush _failed flag
	fan._failed=0;
	// Set next FSM state
	FSM_state = FSM_FAN_STATE_IDLE;
	// Flush FSM timer
	ResetTimer(TIMER_FAN);
}

void FSM_FAN_Process(void)
{
	switch(FSM_state) {

		case FSM_FAN_STATE_IDLE: {
			// Turn ON fan if received message
			if(GetMessage(MSG_FAN_TURN_ON) || GetBCMessage(MSG_BC_SYSTEM_STARTUP_RUN)) {
				// Set next FSM state
				FSM_state = FSM_FAN_STATE_TURN_ON;
				return;
			}
			// Turn OFF fan if received message
			if(GetMessage(MSG_FAN_TURN_OFF) || GetBCMessage(MSG_BC_SYSTEM_STARTUP_COMPLETE)) {
				// Set next FSM state
				FSM_state = FSM_FAN_STATE_TURN_OFF;
				return;
			}
			// No other works if fan is inactive state
			if(fan._active == 0) {
				// Flush fan counter
				fan.counter=0;
				// Flush FSM timer
				ResetTimer(TIMER_FAN);
				return;
			}
			// Repeat calculating proc every 500ms
			if(GetTimer(TIMER_FAN) >= 500) {
				// Flush FSM timer
				ResetTimer(TIMER_FAN);
				// Set next FSM state
				FSM_state = FSM_FAN_STATE_CALCULATE_RPM;
				return;
			}
			
			return;
		}

		case FSM_FAN_STATE_CALCULATE_RPM: {
			// Disable INT interrupt for processing
			FAN_COUNTER_INTERRUPT_DISABLE();
			// Calculate fan RPM (2 pulse per route, RPM = COUNTER * 60 / 2)
			fan.rpm = fan.counter*60;
			// Flush fan counter
			fan.counter=0;
			// Prepare BCD RPM string
			FSM_FAN_RPM2String();
			// Set _failed flag if we have no RPM value
			if(!fan.rpm) { 
				fan._failed=1; 
			} else {
				fan._failed=0;
			}
			// Enable INT interrupt
			FAN_COUNTER_INTERRUPT_ENABLE();
			
			// Set next FSM state
			if(GetMessage(MSG_FAN_TURN_OFF) || GetBCMessage(MSG_BC_SYSTEM_STARTUP_COMPLETE)) {
				FSM_state = FSM_FAN_STATE_TURN_OFF;
			} else {
				FSM_state = FSM_FAN_STATE_IDLE;
			}
			return;
		}

		case FSM_FAN_STATE_TURN_ON: {
			// Turn ON FAN
			FAN_ON();
			// Set _active flag
			fan._active=1;
			fan._failed=1;
			// Set next FSM state
			FSM_state = FSM_FAN_STATE_IDLE;
			return;
		}

		case FSM_FAN_STATE_TURN_OFF: {
			// Turn OFF FAN
			FAN_OFF();
			// Flush _active flag
			fan._active=0;
			// Flush RPM value when fan is stopped
			fan.rpm=0;
			// Prepare BCD RPM string
			FSM_FAN_RPM2String();
			// Set next FSM state
			FSM_state = FSM_FAN_STATE_IDLE;
			return;
		}

		default: break;
	}
}

void FSM_FAN_RPM2String(void)
{	
	char buffer[6];
	char * p_bcd = fan.bcd_rpm;
	char * p_buff = utoa_cycle_sub16(fan.rpm, buffer);
	for(uint8_t i=6; i>0; i--) {
		*(p_bcd++) = *(p_buff++);
	}
}
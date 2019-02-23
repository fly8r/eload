/*
 * button.h
 *
 * Created: 20.10.2018 13:21:24
 *  Author: fly8r
 */ 
#ifndef BUTTON_H_
#define BUTTON_H_

#include "../../config.h"
#include "../../HAL.h"
#include "../../libs/include/messages.h"
#include "../../libs/include/swtimers.h"

/************************************************************************/
/* FSM states                                                           */
/************************************************************************/
enum BUTTON_STATES_ENUM
{
	BUTTON_STATE_UP,
	BUTTON_STATE_DN,
	BUTTON_STATE_AL,
};

/************************************************************************/
/* MACROS                                                               */
/************************************************************************/
#define			BUTTON_DEBOUNCE_DELAY		10
#define			BUTTON_LONG_PRESS_COUNT		60

/************************************************************************/
/* External VARS                                                        */
/************************************************************************/
enum BUTTON_EVENTS_ENUM
{
	BUTTON_EVENT_NOT_PRESSED,   // No events
	BUTTON_EVENT_SHORT_PRESS,   // Short press event
	BUTTON_EVENT_LONG_PRESS     // Long press event
};


/************************************************************************/
/* External Functions                                                   */
/************************************************************************/
// Main FSM initialize function
void FSM_BUTTON_Init(void);
// Processing FSM function
void FSM_BUTTON_Process(void);
// Return last button event
enum BUTTON_EVENTS_ENUM	FSM_BUTTON_getLastEvent(void);

#endif
/*
 * fan.h
 *
 * Created: 18.10.2018 12:42:33
 *  Author: fly8r
 */
#ifndef FAN_H_
#define FAN_H_

#include "../../global_vars.h"
#include "../../config.h"
#include "../../HAL.h"
#include "../../libs/include/messages.h"
#include "../../libs/include/swtimers.h"
#include "../../libs/include/conv.h"

/************************************************************************/
/* FSM states                                                           */
/************************************************************************/
enum FSM_FAN_STATES_ENUM
{
	FSM_FAN_STATE_IDLE=0,
	FSM_FAN_STATE_CALCULATE_RPM,
	FSM_FAN_STATE_TURN_ON,
	FSM_FAN_STATE_TURN_OFF,
};

/************************************************************************/
/* MACROS                                                               */
/************************************************************************/

/************************************************************************/
/* External VARS                                                        */
/************************************************************************/



/************************************************************************/
/* External Functions                                                   */
/************************************************************************/
// Main FSM initialize function
void FSM_FAN_Init(void);
// Processing FSM function
void FSM_FAN_Process(void);

#endif

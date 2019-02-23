/*
 * system.h
 *
 * Created: 02.10.2018 12:18:08
 *  Author: fly8r
 */
#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "../../global_vars.h"
#include "../../config.h"
#include "../../HAL.h"
#include "../../libs/include/messages.h"
#include "../../libs/include/swtimers.h"
#include "ows.h"
#include "ds18x20.h"
#include "../../strings.h"
#include "../../libs/include/conv.h"
#include "../../drivers/include/drv_UART.h"

#include <avr/eeprom.h>

/************************************************************************/
/* FSM states                                                           */
/************************************************************************/
enum FSM_SYSTEM_STATES_ENUM
{
	FSM_SYSTEM_STATE_STARTUP=0,
	FSM_SYSTEM_STATE_STARTUP_CHK,
	FSM_SYSTEM_STATE_CHECK,
	FSM_SYSTEM_STATE_IDLE,
	FSM_SYSTEM_STATE_LOAD_TURN_ON,
	FSM_SYSTEM_STATE_LOAD_TURN_OFF,
	FSM_SYSTEM_STATE_SAVE_EEPROM,
	FSM_SYSTEM_STATE_LOAD_EEPROM,
	FSM_SYSTEM_STATE_RESET
};

/************************************************************************/
/* MACROS                                                               */
/************************************************************************/
#define		SYSTEM_STARTUP_TIMEOUT				5000

#define		SYSTEM_STATE_BITMASK_EMERGENCY		(1<<7)
#define		SYSTEM_STATE_BITMASK_TSENS			(1<<6)
#define		SYSTEM_STATE_BITMASK_FAN			(1<<5)
#define		SYSTEM_STATE_BITMASK_TLOWER			(1<<4)
#define		SYSTEM_STATE_BITMASK_TBETWEEN		(1<<3)
#define		SYSTEM_STATE_BITMASK_THIGHER		(1<<2)
#define		SYSTEM_STATE_BITMASK_STATE			(1<<1)
#define		SYSTEM_STATE_BITMASK_STARTUP		(1<<0)

/************************************************************************/
/* External VARS                                                        */
/************************************************************************/



/************************************************************************/
/* External Functions                                                   */
/************************************************************************/
// Main FSM initialize function
void FSM_SYSTEM_Init(void);
// Processing FSM function
void FSM_SYSTEM_Process(void);


#endif
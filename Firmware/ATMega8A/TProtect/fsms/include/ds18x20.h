/*
 * temperature.h
 *
 * Created: 26.09.2018 14:54:10
 *  Author: fly8r
 */
#ifndef DS18X20_H_
#define DS18X20_H_

#include "../../global_vars.h"
#include "../../config.h"
#include "../../HAL.h"
#include "../../libs/include/messages.h"
#include "../../libs/include/swtimers.h"
#include "ows.h"

/************************************************************************/
/* FSM states                                                           */
/************************************************************************/
enum FSM_OWS_STATES_ENUM
{
	FSM_DS18X20_STATE_IDLE=0,
	FSM_DS18X20_STATE_GET_SERIAL,
	FSM_DS18X20_STATE_SERIAL_PROCESSING,
	FSM_DS18X20_STATE_SEND_CONVERT_T,
	FSM_DS18X20_STATE_READ_SCRATCHPAD,
	FSM_DS18X20_STATE_T_PROCESSING
};

/************************************************************************/
/* MACROS                                                               */
/************************************************************************/
/* Temperature data refresh period */
#define			DS18X20_TEMP_REFRESH_PERIOD				3000

/* 1-wire specify command list */
#define			DS18X20_FUNC_CMD_CONVERT_T				0x44
#define			DS18X20_FUNC_CMD_READ_SCRATCHPAD		0xBE
#define			DS18X20_FUNC_CMD_WRITE_SCRATCHPAD		0x4E

/************************************************************************/
/* External VARS                                                        */
/************************************************************************/

/************************************************************************/
/* External Functions                                                   */
/************************************************************************/
// Main FSM initialize function
void FSM_DS18X20_Init(void);
// Processing FSM function
void FSM_DS18X20_Process(void);


#endif
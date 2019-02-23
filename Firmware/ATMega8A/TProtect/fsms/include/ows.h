/*
 * ows.h
 *
 * Created: 15.10.2018 10:39:43
 *  Author: fly8r
 */
#ifndef FSM_OWS_H_
#define FSM_OWS_H_

#include "../../global_vars.h"
#include "../../config.h"
#include "../../HAL.h"
#include "../../libs/include/messages.h"
#include "../../libs/include/swtimers.h"
#include "../../libs/include/conv.h"
#include <util/delay.h>

/************************************************************************/
/* FSM states                                                           */
/************************************************************************/
enum FSM_OWS_STATE_ENUM
{
	FSM_OWS_STATE_IDLE=0,
	FSM_OWS_STATE_RESET_START,
	FSM_OWS_STATE_WAIT_PRESENCE,
	FSM_OWS_STATE_SEND_BYTES,
	FSM_OWS_STATE_SEND_BYTE,
	FSM_OWS_STATE_RESEIVE_BYTES,
	FSM_OWS_STATE_RESEIVE_BYTE,
	FSM_OWS_STATE_COMPLETE
};

/************************************************************************/
/* MACROS                                                               */
/************************************************************************/
/* 1-wire command list */
#define			OWS_ROM_CMD_READ_ROM			0x33
#define			OWS_ROM_CMD_MATCH_ROM			0x55
#define			OWS_ROM_CMD_SKIP_ROM			0xCC

/************************************************************************/
/* External VARS                                                        */
/************************************************************************/


/************************************************************************/
/* External Functions                                                   */
/************************************************************************/
// Main FSM initialize function
void FSM_OWS_Init(void);
// Processing FSM function
void FSM_OWS_Process(void);


#endif
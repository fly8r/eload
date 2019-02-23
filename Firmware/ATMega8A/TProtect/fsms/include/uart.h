/*
 * uart.h
 *
 * Created: 05.10.2018 12:58:29
 *  Author: fly8r
 */
#ifndef FSMUART_H_
#define FSMUART_H_

#include "../../global_vars.h"
#include "../../HAL.h"
#include "../../libs/include/messages.h"
#include "../../libs/include/swtimers.h"
#include "../../drivers/include/drv_UART.h"
#include "../../libs/include/conv.h"
#include "../../libs/include/parser.h"
#include "../../strings.h"


/************************************************************************/
/* FSM states                                                           */
/************************************************************************/
enum FSM_UART_STATES_ENUM
{
	FSM_UART_STATE_IDLE=0,
	FSM_UART_STATE_TX_MSG_INFO,
	FSM_UART_STATE_TX_MSG_HELP,
	FSM_UART_STATE_TX_MSG_OK,
	FSM_UART_STATE_TX_MSG_TOFF_VALUE_CHANGED,
	FSM_UART_STATE_TX_MSG_TON_VALUE_CHANGED,
	FSM_UART_STATE_TX_MSG_ERR_CMD_UNCOMPLETE,
	FSM_UART_STATE_TX_MSG_ERR_BUFF_OVERFLOW,
	FSM_UART_STATE_TX_MSG_ERR_VALUE_RANGE,
	FSM_UART_STATE_TX_MSG_ERR_ACTIVE_STATE,
	FSM_UART_STATE_CMD_PROCESSING
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
void FSM_UART_Init(void);
// Processing FSM function
void FSM_UART_Process(void);


#endif

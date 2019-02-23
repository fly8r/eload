/*
 * drv_UART.h
 *
 * Created: 04.10.2018 12:39:16
 *  Author: fly8r
 */
#ifndef DRVUART_H_
#define DRVUART_H_

#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/atomic.h>
#include "../../config.h"
#include <util/delay.h>
#include "../../HAL.h"

/************************************************************************/
/* MACROS                                                               */
/************************************************************************/
enum UART_RX_STATES_ENUM
{
	UART_RX_STATE_IDLE,
	UART_RX_STATE_BUFF_OVERFLOW,
	UART_RX_STATE_PROCESSING,
};

// RX buffer size
#define UART_RX_BUFSIZE			20
// TX buffer size
#define UART_TX_BUFSIZE			500
// Max word in command
#define UART_CMD_WORD_COUNT		3

/************************************************************************/
/* VARS                                                                 */
/************************************************************************/
extern	volatile	char			uart_rx_buffer[UART_RX_BUFSIZE];    //	UART input buffer
extern	volatile	uint8_t			uart_rx_queue_length;				//	UART RX buffer pointer
extern	volatile	enum			UART_RX_STATES_ENUM		UART_RX_STATE;

/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/
void UART_AddString(const char * s);
void UART_AddStringFromFlash(const char * s);
void UART_StartTX(void);
void UART_TX_Service(void);
void UART_RX_Service(void);
void UART_RX_FlushBuffer(void);


#endif
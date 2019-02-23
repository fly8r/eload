/*
 * parser.h
 *
 * Created: 05.10.2018 14:02:15
 *  Author: fly8r
 */
#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>
#include <avr/pgmspace.h>
#include "../../drivers/include/drv_UART.h"
#include "../../libs/include/conv.h"

/************************************************************************/
/* VARS                                                                 */
/************************************************************************/
extern				char				pars_buffer[UART_RX_BUFSIZE];
extern				char				* words[UART_CMD_WORD_COUNT];

/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/
uint8_t		PARS_ParsStringFromBuffer(volatile char * buffer, uint8_t length);
uint8_t		PARS_StrCmp(char * str1, char * str2);
uint8_t		PARS_StrCmpF(char * str1, char const * str2);
uint8_t		PARS_StrToUint8t(char * str);
uint16_t	PARS_StrToUint16t(char * str);

#endif
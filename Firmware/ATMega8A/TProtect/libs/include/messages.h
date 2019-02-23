/*
 * messages.h
 *
 * Created: 08.09.2018 12:37:06
 *  Author: fly8r
 */
#ifndef MESSAGES_H_
#define MESSAGES_H_

#include <stdio.h>
#include <util/atomic.h>

#define USE_BC_MESSAGES

// Messages queue deep
#define	MAX_MESSAGES				16
// Message definitions
enum MSG_ENUM
{
	MSG_SYSTEM_STARTUP_COMPLETE = 0,
	MSG_SYSTEM_LOAD_ON,
	MSG_SYSTEM_LOAD_OFF,
	MSG_UART_SEND_MSG_INFO,
	MSG_UART_SEND_ERR_BUFF_OVERFLOW,
	MSG_UART_SEND_MSG_OK,
	MSG_DS18X20_GET_SERIAL,
	MSG_OWS_RUN,
	MSG_FAN_TURN_ON,
	MSG_FAN_TURN_OFF,
	MSG_LEDS_STARTUP,
	MSG_LEDS_PROCESSING,
	MSG_LEDS_TURN_OFF,
	MSG_EEPROM_SAVE,
	MSG_EEPROM_LOAD,
	MSG_SYSTEM_RESET
};

#ifdef USE_BC_MESSAGES
	#define MAX_BC_MESSAGES 		4

enum MSG_BC_ENUM
{
	MSG_BC_SYSTEM_STARTUP_RUN=0,
	MSG_BC_SYSTEM_STARTUP_COMPLETE,
	MSG_BC_OWS_COMPLETE,
	MSG_BC_KEY_PRESSED,
};

#endif

#define MSG_STATE_INACTIVE			0
#define MSG_STATE_SET				1
#define MSG_STATE_ACTIVE			2


// Messages process function
void		InitMessages(void);
void		ProcessMessages(void);
void		SendMessageWParam(uint8_t Msg, void * pParams);
void		SendMessageWOParam(uint8_t Msg);
uint8_t		GetMessage(uint8_t Msg);
void		*GetMessageParam(uint8_t Msg);
#ifdef USE_BC_MESSAGES
	void	SendBCMessage(uint8_t Msg);
	uint8_t GetBCMessage(uint8_t Msg);
#endif

#endif /* MESSAGES_H_ */
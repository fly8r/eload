/*
 * drv_OWSimple.h
 *
 * Created: 26.09.2018 13:35:05
 *  Author: fly8r
 */
#ifndef DRVOWSMPL_H_
#define DRVOWSMPL_H_

#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/atomic.h>
#include "../config.h"
#include <util/delay.h>
#include "../HAL.h"


/* 1-wire command list */
#define			OWS_DRV_ROM_CMD_READ_ROM		0x33
#define			OWS_DRV_ROM_CMD_MATCH_ROM		0x55
#define			OWS_DRV_ROM_CMD_SKIP_ROM		0xCC

/************************************************************************/
/* VARS                                                                 */
/************************************************************************/
typedef struct
{
	uint8_t	family;
	uint8_t serial[6];
	uint8_t crc;
} ow_addr_t;

enum OWS_DRV_STATES_ENUM
{
	OWS_DRV_STATE_IDLE=0,
	OWS_DRV_STATE_PRESENCE,
	OWS_DRV_STATE_NO_DEVICE,
	OWS_DRV_STATE_OK,
	OWS_DRV_STATE_ERROR
};

enum OWS_DRV_ERRORS_ENUM
{
	OWS_DRV_ERROR_NO_ERROR=0,
	OWS_DRV_ERROR_LINE,
	OWS_DRV_ERROR_PRESENCE_TIMEOUT,
	OWs_DRV_ERROR_CRC
};

extern	enum	OWS_DRV_STATES_ENUM		OWS_DRV_STATE;
extern	enum	OWS_DRV_ERRORS_ENUM		OW_ERROR;

/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/
void	OWS_StartResetProc(void);
uint8_t	OWS_CompleteResetProc(void);


#endif




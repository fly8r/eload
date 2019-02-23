/*
 * main.h
 *
 * Created: 09.02.2019 22:46:14
 *  Author: fly8r
 */ 
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "global_vars.h"
#include "config.h"
#include "HAL.h"
#include "libs/include/messages.h"
#include "libs/include/swtimers.h"
#include "drivers/include/drv_UART.h"
#include "fsms/include/system.h"
#include "fsms/include/uart.h"
#include "fsms/include/ows.h"
#include "fsms/include/ds18x20.h"
#include "fsms/include/fan.h"
#include "fsms/include/button.h"
#include "fsms/include/leds.h"


/************************************************************************/
/* VARS                                                                 */
/************************************************************************/
static	uint8_t		systick;

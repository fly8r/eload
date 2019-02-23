/*
 * globals.h
 *
 * Created: 01.11.2018 14:09:11
 *  Author: fly8r
 */
#ifndef GLOBALS_H_
#define GLOBALS_H_

#include <stdio.h>
#include <avr/wdt.h>

/************************************************************************/
/* main.c                                                               */
/************************************************************************/
typedef struct
{
	uint8_t		_active,
				_emergency,
				current_state_mask,
				last_state_mask;
	struct
	{
		uint8_t		tfon,
					tfoff,
					toff;
	} settings;
} device_data_t;
extern		device_data_t	device;

/************************************************************************/
/* ows.h                                                                */
/************************************************************************/
enum FSM_OWS_ERRORS_ENUM
{
	FSM_OWS_ERROR_NO_ERROR,
	FSM_OWS_ERROR_BAD_LINE_OR_DEVICE,
	FSM_OWS_ERROR_NO_DEVICE,
	FSM_OWS_ERROR_CRC
};

typedef struct
{
	uint8_t		family,							// Family ROM part
				serial[6],						// Serial ROM part
				crc;							// CRC ROM part
} ow_rom_t;

typedef struct
{
	uint8_t		tx_buff[10],					// TX buffer
				rx_buff[10],					// RX buffer
				tx_length,						// Length the data for transmitting
				rx_length;						// Length the data for receiving
	uint8_t		f_chk_crc,						// Check CRC flag
	f_busy;							// BUSY flag
	enum		FSM_OWS_ERRORS_ENUM		error;	// Error handler
} ows_data_t;
extern	ows_data_t		ows;


/************************************************************************/
/* ds18x20.h                                                            */
/************************************************************************/
/* Sensor types */
#define			T_SENS_UNKNOWN							0
#define			T_SENS_DS18S20							1
#define			T_SENS_DS18B20							2

typedef struct
{
	uint8_t		_failed;
	ow_rom_t	rom;
	struct
	{
		int8_t		raw;
		uint8_t		value,
					sign;
		char		bcd_value[4];
	} data;
} tsens_data_t;
extern		tsens_data_t	tsens;


/************************************************************************/
/* fan.h                                                                */
/************************************************************************/
typedef struct
{
	uint8_t		_active, _failed;
	uint16_t	counter,
				rpm;
	char		bcd_rpm[6];
} fan_data_t;
extern		fan_data_t		fan;


/************************************************************************/
/* leds.h                                                               */
/************************************************************************/
enum LED_COLORS_ENUM
{
	RED=0,
	YELLOW,
	GREEN
};

typedef struct
{
	uint8_t		_active, _blinking;
	enum		LED_COLORS_ENUM		color;
	uint16_t	glow_time_ms;
} leds_params_t;




#endif
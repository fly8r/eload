/*
 * config.h
 *
 * Created: 08.09.2018 11:08:33
 *  Author: fly8r
 */
#define F_CPU							8000000UL	// Core CPU Frequency

/* Count for FAN FAIL action */
#define FAN_COUNTER_FAILS				3

/* UART configuration */
#define BAUDRATE						9600L

/* Default values */
#define DEFAULT_TFON					40			// *C Fan ON temperature
#define DEFAULT_TFOFF					35			// *C Fan OFF temperature
#define DEFAULT_TOFF					70			// *C System OFF temperature
#define THYSTERESIS						5			// *C Temperature hysteresis between TFON and TFOFF
#define MAX_TFON						75			// *C -
#define MAX_TFOFF						70			// *C - | Max temperature for user settings
#define MAX_TOFF						80			// *C -
#define MIN_TFON						30			// *C - 
#define MIN_TFOFF						25			// *C - | Min temperature for user settings
#define MIN_TOFF						30			// *C -

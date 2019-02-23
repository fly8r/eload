/*
 * timers.h
 *
 * Created: 08.09.2018 11:17:10
 *  Author: fly8r
 */
#ifndef SWTIMERS_H
#define SWTIMERS_H

#include <stdio.h>
#include <util/atomic.h>

//#define USE_GLOBAL_TIMERS

#define		SEC 				1000
#define		MIN 				60 * SEC
#define		HOUR 				60 * MIN

// Mas timers counter
#define		MAX_TIMERS			7
// Timers enum
enum TIMERS_ENUM
{
	TIMER_SYSTEM=0,
	TIMER_OWS,
	TIMER_DS18X20,
	TIMER_BUTTON,
	TIMER_FAN,
	TIMER_LEDS,
	TIMER_LEDS_BLINK
};

#ifdef USE_GLOBAL_TIMERS
	#define MAX_GTIMERS 	4

	#define GTIMER_1		0
	#define GTIMER_2		1
	#define GTIMER_3		2
	#define GTIMER_4		3
#endif

void		InitSystemTimer(void);
void		InitTimers(void);
void		ProcessTimers(uint8_t * tick);
uint16_t	GetTimer(uint8_t Timer);
void		ResetTimer(uint8_t Timer);

#ifdef USE_GLOBAL_TIMERS
	uint16_t	GetGTimer(uint8_t Timer);
	void		StopGTimer(uint8_t Timer);
	void		StartGTimer(uint8_t Timer);
	void		PauseGTimer(uint8_t Timer);
	void		ResumeGTimer(uint8_t Timer);
#endif


#endif /* SWTIMERS_H_ */
/*
 * system.c
 *
 * Created: 02.10.2018 12:18:00
 *  Author: fly8r
 */
#include	"include/system.h"

/************************************************************************/
/* VARS                                                                 */
/************************************************************************/
volatile	static		uint8_t			FSM_state;

static					leds_params_t	led_params;
static					uint8_t			fan_retry_count;

uint8_t		EEMEM		EE_values[3] = { DEFAULT_TFON, DEFAULT_TFOFF, DEFAULT_TOFF };

/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/
void FSM_SYSTEM_LoadEEPROM(void);

void FSM_SYSTEM_Init(void)
{
	// Set default FSM state
	FSM_state = FSM_SYSTEM_STATE_STARTUP;
	// Flush FSM timer
	ResetTimer(TIMER_SYSTEM);
	// Set default state mask
	device.current_state_mask = device.last_state_mask = 0;
	// Set default fan fail counter
	fan_retry_count = 0;
	// Set startup state bitmask
	device.current_state_mask |= SYSTEM_STATE_BITMASK_STARTUP;
}

void FSM_SYSTEM_Process(void)
{
	switch(FSM_state) {
		// Startup FSM state
		case FSM_SYSTEM_STATE_STARTUP: {
			// Send BC message that system in startup mode
			SendBCMessage(MSG_BC_SYSTEM_STARTUP_RUN);
			// Set next FSM state
			FSM_state = FSM_SYSTEM_STATE_STARTUP_CHK;
			// Flush FSM timer
			ResetTimer(TIMER_SYSTEM);
			return;
		}
		// Startup check FSM state
		case FSM_SYSTEM_STATE_STARTUP_CHK: {
			// Waiting for state measurement complete timeout
			if(GetTimer(TIMER_SYSTEM) >= SYSTEM_STARTUP_TIMEOUT) {
				// Send BC message that system startup complete
				SendBCMessage(MSG_BC_SYSTEM_STARTUP_COMPLETE);
				// Send HELLO message into UART
				SendMessageWOParam(MSG_UART_SEND_MSG_INFO);
				// Set next FSM state
				FSM_state = FSM_SYSTEM_STATE_CHECK;
			}
			return;
		}
		// System check FSM state
		case FSM_SYSTEM_STATE_CHECK: {

			/************************************************************************/
			/* PROTECTION CONTROL                                                   */
			/************************************************************************/
			// Control startup bitmap
			if(device._active) {
				device.current_state_mask |= SYSTEM_STATE_BITMASK_STATE;
			} else {
				device.current_state_mask &= ~SYSTEM_STATE_BITMASK_STATE;
			}

			// CRIT: If temperature sensor is failed - switch OFF load
			//   OR  If current temperature more than user settings - switch OFF load
			if(tsens._failed || tsens.data.value >= device.settings.toff) {
				// Set EMERGENCY flag for blocking start or turn off load
				device._emergency = 1;
				// Set state mask
				device.current_state_mask |= SYSTEM_STATE_BITMASK_EMERGENCY;
			} else {
				// Flush emergency flag
				device._emergency = 0;
				// Flush state mask
				device.current_state_mask &= ~SYSTEM_STATE_BITMASK_EMERGENCY;
			}

			// If is not critical situation
			if(!device._emergency) { // <- Processing other bit in bitmaps
				// Action by device state
				if(device.current_state_mask & SYSTEM_STATE_BITMASK_STATE) { // <- Device in active state
					// Action by fan state
					if(fan._failed) {
						// Check delay cycle
						if(fan_retry_count < 3) {
							// Try to fan turn ON
							SendMessageWOParam(MSG_FAN_TURN_ON);
							// Increment retry count
							fan_retry_count++;
						}
						if(fan_retry_count == 3) { // <- Delay cycle are expired
							// Set state mask
							device.current_state_mask |= SYSTEM_STATE_BITMASK_FAN;
						}
					} else {
						// Flush state mask
						device.current_state_mask &= ~SYSTEM_STATE_BITMASK_FAN;
						// Flush retry count
						fan_retry_count=0;
					}

					// Action by temperature value
					if(tsens.data.value <= device.settings.tfoff) {
						if(!(device.current_state_mask & SYSTEM_STATE_BITMASK_FAN) && fan._active) {
							SendMessageWOParam(MSG_FAN_TURN_OFF);
						}
						device.current_state_mask |= SYSTEM_STATE_BITMASK_TLOWER;
					} else {
						device.current_state_mask &= ~SYSTEM_STATE_BITMASK_TLOWER;
					}

					if(tsens.data.value > device.settings.tfoff && tsens.data.value < device.settings.tfon) {
						device.current_state_mask |= SYSTEM_STATE_BITMASK_TBETWEEN;
					} else {
						device.current_state_mask &= ~SYSTEM_STATE_BITMASK_TBETWEEN;
					}

					if(tsens.data.value >= device.settings.tfon) {
						if(!(device.current_state_mask & SYSTEM_STATE_BITMASK_FAN) && !fan._active) {
							SendMessageWOParam(MSG_FAN_TURN_ON);
						}
						device.current_state_mask |= SYSTEM_STATE_BITMASK_THIGHER;
					} else {
						device.current_state_mask &= ~SYSTEM_STATE_BITMASK_THIGHER;
					}
				} else {
					if(fan._active && tsens.data.value <= device.settings.tfoff) {
						SendMessageWOParam(MSG_FAN_TURN_OFF);
					}
				}
			}

			/************************************************************************/
			/* INDICATION CONTROL                                                   */
			/************************************************************************/
			// Led processing if state was changed
			if(device.current_state_mask != device.last_state_mask) {
				// Store last state mask of bits
				device.last_state_mask = device.current_state_mask;
				// Operation by activity flag
				if(device.current_state_mask & SYSTEM_STATE_BITMASK_STATE) { // <- Device in active state
					// CRIT: If current temperature more than user settings - switch OFF load
					if(tsens.data.value >= device.settings.toff) {
						// Set LED params
						led_params._active = 1;
						led_params._blinking = 0;
						led_params.color = RED;
						led_params.glow_time_ms = 10000;
						// Send message with params
						SendMessageWParam(MSG_LEDS_PROCESSING, (void *)&led_params);
						goto fast_complete;
					}
					// CRIT: If temperature sensor is failed - switch OFF load
					if(tsens._failed) {
						// Set LED params
						led_params._active = 1;
						led_params._blinking = 1;
						led_params.color = RED;
						led_params.glow_time_ms = 10000;
						// Send message with params
						SendMessageWParam(MSG_LEDS_PROCESSING, (void *)&led_params);
						goto fast_complete;
					}

					// INFO: Set led color with temperature value and fan state
					if(device.current_state_mask & SYSTEM_STATE_BITMASK_TLOWER || ((device.current_state_mask & SYSTEM_STATE_BITMASK_TBETWEEN) && !fan._active)) {
						if(device.current_state_mask & SYSTEM_STATE_BITMASK_FAN) {
							led_params._blinking = 1;
						} else {
							led_params._blinking = 0;
						}
						// Set LED params
						led_params._active = 1;
						led_params.color = GREEN;
						led_params.glow_time_ms = 0xFFFF;
						// Send message with params
						SendMessageWParam(MSG_LEDS_PROCESSING, (void *)&led_params);
						goto fast_complete;
					}
					// INFO: Set led color with temperature value and fan state
					if(device.current_state_mask & SYSTEM_STATE_BITMASK_THIGHER || ((device.current_state_mask & SYSTEM_STATE_BITMASK_TBETWEEN) && fan._active)) {
						if(device.current_state_mask & SYSTEM_STATE_BITMASK_FAN) {
							led_params._blinking = 1;
						} else {
							led_params._blinking = 0;
						}
						// Set LED params
						led_params._active = 1;
						led_params.color = YELLOW;
						led_params.glow_time_ms = 0xFFFF;
						// Send message with params
						SendMessageWParam(MSG_LEDS_PROCESSING, (void *)&led_params);
					}
				} else {
					// Processing led only in startup mode
					if(device.current_state_mask & SYSTEM_STATE_BITMASK_STARTUP) {
						if(tsens._failed) { // <- Indicate TSens error
							// Set LED params
							led_params._blinking = 1;
							led_params.color = RED;
							led_params.glow_time_ms = 10000;
							SendMessageWParam(MSG_LEDS_PROCESSING, (void *)&led_params);
						} else if (fan._failed) { // <- Indicate FAN error
							// Set LED params
							led_params._blinking = 1;
							led_params.color = YELLOW;
							led_params.glow_time_ms = 10000;
							SendMessageWParam(MSG_LEDS_PROCESSING, (void *)&led_params);
						}
						// Flush startup bitmask
						device.current_state_mask &= ~SYSTEM_STATE_BITMASK_STARTUP;
						// Startup load params from EEPROM
						FSM_SYSTEM_LoadEEPROM();
					}
				}
			}

		fast_complete:
			// Turn OFF load if emergency flag was setup
			if(device._emergency && device._active) {
				FSM_state = FSM_SYSTEM_STATE_LOAD_TURN_OFF;
			} else {
				FSM_state = FSM_SYSTEM_STATE_IDLE;
			}
			return;
		}

		// Default FSM state
		case FSM_SYSTEM_STATE_IDLE: {
			// Run system check every 1s
			if(GetTimer(TIMER_SYSTEM) >= 1000) {
				// Flush FSM timer
				ResetTimer(TIMER_SYSTEM);
				// Set next FSM state
				FSM_state = FSM_SYSTEM_STATE_CHECK;
				return;
			}

			// Key pressed procedure
			if(GetBCMessage(MSG_BC_KEY_PRESSED)) {
				// Select next FSM state by device state
				if(device._active) { // <- Device have active state
					FSM_state = FSM_SYSTEM_STATE_LOAD_TURN_OFF;
				} else { // <- Device have inactive state
					FSM_state = FSM_SYSTEM_STATE_LOAD_TURN_ON;
				}
				return;
			}

			// Processing for message LOAD_ON
			if(GetMessage(MSG_SYSTEM_LOAD_ON) && device._active == 0) {
				// Set next FSM state
				FSM_state = FSM_SYSTEM_STATE_LOAD_TURN_ON;
				return;
			}

			// Processing for message LOAD_OFF
			if(GetMessage(MSG_SYSTEM_LOAD_OFF) && device._active == 1) {
				// Set next FSM state
				FSM_state = FSM_SYSTEM_STATE_LOAD_TURN_OFF;
				return;
			}

			// Processing message LOAD EEPROM
			if(GetMessage(MSG_EEPROM_LOAD)) {
				// Set next FSM state
				FSM_state = FSM_SYSTEM_STATE_LOAD_EEPROM;
				return;
			}

			// Processing message SAVE EEPROM
			if(GetMessage(MSG_EEPROM_SAVE)) {
				// Set next FSM state
				FSM_state = FSM_SYSTEM_STATE_SAVE_EEPROM;
				return;
			}

			// Processing message RESET
			if(GetMessage(MSG_SYSTEM_RESET)) {
				// Set next FSM state
				FSM_state = FSM_SYSTEM_STATE_RESET;
				return;
			}

			return;
		}
		// Load TURN ON FSM state
		case FSM_SYSTEM_STATE_LOAD_TURN_ON: {
			// If state check successful and run is available
			if(!device._emergency) {
				// Set active flag
				device._active = 1;
				// Send message for TURN ON fan
				SendMessageWOParam(MSG_FAN_TURN_ON);
				// Flush fan retry count
				fan_retry_count = 0;
				// Turn ON relay
				RELAY_ON();
			}
			// Set next FSM state
			FSM_state = FSM_SYSTEM_STATE_IDLE;
			return;
		}
		// Load TURN OFF FSM state
		case FSM_SYSTEM_STATE_LOAD_TURN_OFF: {
			// Flush active flag
			device._active = 0;
			
			// Do not turn off fan while T will not be lower then Tfoff
			if(fan._failed || tsens.data.value <= device.settings.tfoff) {
				// Send message turn OFF FAN
				SendMessageWOParam(MSG_FAN_TURN_OFF);	
			}			
			
			// Led indication time with emergency state flag
			if(!device._emergency) {
				led_params.glow_time_ms = 0;
			} else {
				led_params.glow_time_ms = 10000;
			}
			// Turn OFF relay
			RELAY_OFF();
			// Set next FSM state
			FSM_state = FSM_SYSTEM_STATE_IDLE;
			return;
		}
		// Load EEPROM data FSM state
		case FSM_SYSTEM_STATE_LOAD_EEPROM: {
			// Load data from EEPROM
			FSM_SYSTEM_LoadEEPROM();
			// Send message to UART
			SendMessageWOParam(MSG_UART_SEND_MSG_OK);
 			// Set next FSM state
			FSM_state = FSM_SYSTEM_STATE_IDLE;
			return;
		}
		// Save EEPROM data FSM state
		case FSM_SYSTEM_STATE_SAVE_EEPROM: {
			// Waiting for EEPROM ready
			while(!eeprom_is_ready());
			// Write data block
			eeprom_write_block(&device.settings, &EE_values, sizeof(device.settings));
			// Send message to UART
			SendMessageWOParam(MSG_UART_SEND_MSG_OK);
			// Set next FSM state
			FSM_state = FSM_SYSTEM_STATE_IDLE;
			return;
		}
		// Reset device FSM state
		case FSM_SYSTEM_STATE_RESET: {
			wdt_disable();
			wdt_enable(WDTO_15MS);
			while (1) {}
			return;
		}

		default: break;
	}
}

void FSM_SYSTEM_LoadEEPROM(void)
{
	// Waiting for EEPROM ready
	while(!eeprom_is_ready());
	// Read data block
	eeprom_read_block(&device.settings, &EE_values, sizeof(device.settings));

	//@TODO: Think about check by data damage
	// Load failsafe values
	if(device.settings.tfon == 0xFF || device.settings.tfon == 0) { device.settings.tfon = DEFAULT_TFON; }
	if(device.settings.tfoff == 0xFF || device.settings.tfoff == 0) { device.settings.tfoff = DEFAULT_TFOFF; }
	if(device.settings.toff == 0xFF || device.settings.toff == 0) { device.settings.toff = DEFAULT_TOFF; }
}

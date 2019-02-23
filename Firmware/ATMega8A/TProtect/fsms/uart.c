/*
 * uart.c
 *
 * Created: 05.10.2018 12:58:21
 *  Author: fly8r
 */
#include "include/uart.h"


/************************************************************************/
/* VARS                                                                 */
/************************************************************************/
volatile	static		uint8_t		FSM_state;
static		uint8_t		v_min, v_max;


/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/
void FSM_UART_Init(void)
{
	// Set default FSM state
	FSM_state = FSM_UART_STATE_IDLE;
}

void FSM_UART_Process(void)
{
	switch(FSM_state) {

		case FSM_UART_STATE_IDLE: {

			if(GetMessage(MSG_UART_SEND_MSG_INFO)) {
				// Set next FSM state
				FSM_state = FSM_UART_STATE_TX_MSG_INFO;
				return;
			}

			if(GetMessage(MSG_UART_SEND_MSG_OK)) {
				// Set next FSM state
				FSM_state = FSM_UART_STATE_TX_MSG_OK;
				return;
			}



			switch(UART_RX_STATE) {
				// Buffer state string overflow
				case UART_RX_STATE_BUFF_OVERFLOW: {
					// Set next FSM state
					FSM_state = FSM_UART_STATE_TX_MSG_ERR_BUFF_OVERFLOW;
					break;
				}

				case UART_RX_STATE_PROCESSING: {
					// Set next FSM state
					FSM_state = FSM_UART_STATE_CMD_PROCESSING;
					break;
				}

				default: return;
			}
			UART_RX_STATE = UART_RX_STATE_IDLE;

			break;
		}

		case FSM_UART_STATE_TX_MSG_INFO: {
			char buff[4];
			// Disabling interrupt before send data
			UART_RX_INTERRUPT_DISABLE()

			UART_AddStringFromFlash(CR);
			UART_AddStringFromFlash(FW_STR1);
			UART_AddStringFromFlash(FW_STR2);
			UART_AddStringFromFlash(FW_STR3);
			UART_AddStringFromFlash(FW_STR4);
			UART_AddStringFromFlash(FW_STR5);

			if(tsens.rom.family != T_SENS_UNKNOWN) {
				if(tsens.rom.family == T_SENS_DS18B20) {
					UART_AddStringFromFlash(DS18B);
				} else {
					UART_AddStringFromFlash(DS18S);
				}
				UART_AddStringFromFlash(CR);
				UART_AddStringFromFlash(FW_STR6);
				UART_AddString(hex_to_ascii(tsens.rom.family, buff));
				UART_AddStringFromFlash(VS);
				for(int8_t i=5; i>=0; i--) {
					UART_AddString(hex_to_ascii(tsens.rom.serial[i], buff));
				}
				UART_AddStringFromFlash(VS);
				UART_AddString(hex_to_ascii(tsens.rom.crc, buff));
				UART_AddStringFromFlash(CR);

				UART_AddStringFromFlash(FW_STR7);
				if(tsens.data.sign) {
					UART_AddStringFromFlash(MINUS);
				} else {
					UART_AddStringFromFlash(PLUS);
				}
				UART_AddString(tsens.data.bcd_value);
				UART_AddStringFromFlash(CELS);
			} else {
				UART_AddStringFromFlash(UNKNOWN);
			}
			UART_AddStringFromFlash(CR);
			UART_AddStringFromFlash(FW_STR8);
			if(fan._failed) {
				UART_AddStringFromFlash(FAILED);
			} else {
				UART_AddString(fan.bcd_rpm);
				UART_AddStringFromFlash(RPM);
			}
			UART_AddStringFromFlash(CR);
			// Load state: [on/off]
			UART_AddStringFromFlash(FW_STR9);
			if(device._active) {
				UART_AddStringFromFlash(ON);
			} else {
				UART_AddStringFromFlash(OFF);
			}
			// Params section
			UART_AddStringFromFlash(CR);
			UART_AddStringFromFlash(CR);
			UART_AddStringFromFlash(FW_STR10);
			UART_AddStringFromFlash(FW_STR11);
			UART_AddString(utoa_cycle_sub8(device.settings.tfon, buff));
			UART_AddStringFromFlash(CELS);
			UART_AddStringFromFlash(CR);
			UART_AddStringFromFlash(FW_STR12);
			UART_AddString(utoa_cycle_sub8(device.settings.tfoff, buff));
			UART_AddStringFromFlash(CELS);
			UART_AddStringFromFlash(CR);
			UART_AddStringFromFlash(FW_STR13);
			UART_AddString(utoa_cycle_sub8(device.settings.toff, buff));
			UART_AddStringFromFlash(CELS);
			UART_AddStringFromFlash(CR);
			UART_AddStringFromFlash(CR);
			UART_AddStringFromFlash(CONSOLE);

			// Enable interrupt before send data
			UART_RX_INTERRUPT_ENABLE()

			// Set next FSM state
			FSM_state = FSM_UART_STATE_IDLE;
			return;
		}

		case FSM_UART_STATE_TX_MSG_HELP: {
			// Send error string to UART
			UART_AddStringFromFlash(CR);
			UART_AddStringFromFlash(HLP_STR2);
			UART_AddStringFromFlash(HLP_STR1);
			UART_AddStringFromFlash(HLP_STR2);
			UART_AddStringFromFlash(HLP_STR3);
			UART_AddStringFromFlash(HLP_STR4);
			UART_AddStringFromFlash(HLP_STR5);
			UART_AddStringFromFlash(HLP_STR6);
			UART_AddStringFromFlash(HLP_STR7);
			UART_AddStringFromFlash(HLP_STR8);
			UART_AddStringFromFlash(HLP_STR9);
			UART_AddStringFromFlash(HLP_STR10);
			UART_AddStringFromFlash(HLP_STR11);
			UART_AddStringFromFlash(CR);
			UART_AddStringFromFlash(CONSOLE);

			FSM_state = FSM_UART_STATE_IDLE;

			return;
		}

		case FSM_UART_STATE_TX_MSG_OK: {
			// Send info message OK
			UART_AddStringFromFlash(CR);
			UART_AddStringFromFlash(MSG_OK);
			UART_AddStringFromFlash(CONSOLE);
			// Flush RX buffer
			UART_RX_FlushBuffer();

			FSM_state = FSM_UART_STATE_IDLE;
			return;
		}

		case FSM_UART_STATE_TX_MSG_ERR_CMD_UNCOMPLETE: {
			// Send error string to UART
			UART_AddStringFromFlash(CR);
			UART_AddStringFromFlash(ERROR_UNCOMPLETE_CMD);
			UART_AddStringFromFlash(CONSOLE);

			FSM_state = FSM_UART_STATE_IDLE;
			return;
		}

		case FSM_UART_STATE_TX_MSG_ERR_BUFF_OVERFLOW: {

			// Send error string to UART
			UART_AddStringFromFlash(CR);
			UART_AddStringFromFlash(ERROR_STR_TOO_LARGE);
			UART_AddStringFromFlash(CONSOLE);
			// Flush RX buffer
			UART_RX_FlushBuffer();

			FSM_state = FSM_UART_STATE_IDLE;

			return;
		}

		case FSM_UART_STATE_TX_MSG_ERR_VALUE_RANGE: {

			// Send error string to UART
			UART_AddStringFromFlash(CR);
			UART_AddStringFromFlash(ERROR_INVALID_VALUE_CMD);

			char buff[4];
			UART_AddString(utoa_cycle_sub8(v_min, buff));
			UART_AddStringFromFlash(SLASH);
			UART_AddString(utoa_cycle_sub8(v_max, buff));

			UART_AddStringFromFlash(CR);
			UART_AddStringFromFlash(CONSOLE);
			// Flush RX buffer
			UART_RX_FlushBuffer();

			// Set next FSM state
			FSM_state = FSM_UART_STATE_IDLE;
			return;
		}

		case FSM_UART_STATE_TX_MSG_TOFF_VALUE_CHANGED: {
			UART_AddStringFromFlash(CR);
			UART_AddStringFromFlash(MSG_T_HYSTERESIS);

			char buff[4];
			UART_AddString(utoa_cycle_sub8(THYSTERESIS, buff));
			UART_AddStringFromFlash(CELS);
			UART_AddStringFromFlash(CR);
			UART_AddStringFromFlash(MSG_TFOFF_VALUE_CHANGED);
			UART_AddString(utoa_cycle_sub8(device.settings.tfoff, buff));
			UART_AddStringFromFlash(CELS);
			UART_AddStringFromFlash(CR);
			UART_AddStringFromFlash(CONSOLE);
			// Flush RX buffer
			UART_RX_FlushBuffer();

			// Set next FSM state
			FSM_state = FSM_UART_STATE_IDLE;
			return;
		}

		case FSM_UART_STATE_TX_MSG_TON_VALUE_CHANGED: {
			UART_AddStringFromFlash(CR);
			UART_AddStringFromFlash(MSG_T_HYSTERESIS);

			char buff[4];
			UART_AddString(utoa_cycle_sub8(THYSTERESIS, buff));
			UART_AddStringFromFlash(CELS);
			UART_AddStringFromFlash(CR);
			UART_AddStringFromFlash(MSG_TFON_VALUE_CHANGED);
			UART_AddString(utoa_cycle_sub8(device.settings.tfon, buff));
			UART_AddStringFromFlash(CELS);
			UART_AddStringFromFlash(CR);
			UART_AddStringFromFlash(CONSOLE);
			// Flush RX buffer
			UART_RX_FlushBuffer();

			// Set next FSM state
			FSM_state = FSM_UART_STATE_IDLE;
			return;
		}

		case FSM_UART_STATE_TX_MSG_ERR_ACTIVE_STATE: {
			// Send error string to UART
			UART_AddStringFromFlash(CR);
			UART_AddStringFromFlash(ERROR_CHANGE_VALUE_ACTIVE_CMD);
			UART_AddStringFromFlash(CR);
			UART_AddStringFromFlash(CONSOLE);
			// Flush RX buffer
			UART_RX_FlushBuffer();

			// Set next FSM state
			FSM_state = FSM_UART_STATE_IDLE;
			return;
		}

		case FSM_UART_STATE_CMD_PROCESSING: {

			// Parse string and get words count
			uint8_t argc = PARS_ParsStringFromBuffer(uart_rx_buffer, uart_rx_queue_length);

			if(argc) {
				// Received command - INFO
				if(PARS_StrCmpF(words[0], CMD_INFO)) {
					// Add task to run function
					FSM_state = FSM_UART_STATE_TX_MSG_INFO;
					// Clean UART buffer
					UART_RX_FlushBuffer();
					return;
				}

				// Received command - HELP
				if(PARS_StrCmpF(words[0], CMD_HELP)) {
					// Add task to run function
					FSM_state = FSM_UART_STATE_TX_MSG_HELP;
					// Clean UART buffer
					UART_RX_FlushBuffer();
					return;
				}

				// Received command - RESET
				if(PARS_StrCmpF(words[0], CMD_RESET)) {
					// Send message for reset system
					SendMessageWOParam(MSG_SYSTEM_RESET);
					return;
				}

				// Received command - LOAD
				if(PARS_StrCmpF(words[0], CMD_LOAD)) {
					// Send message for load data from EEPROM
					SendMessageWOParam(MSG_EEPROM_LOAD);
					// Set next FSM state
					FSM_state = FSM_UART_STATE_IDLE;
					return;
				}

				// Received command - SAVE
				if(PARS_StrCmpF(words[0], CMD_SAVE)) {
					// Send message for save data into EEPROM
					SendMessageWOParam(MSG_EEPROM_SAVE);
					// Set next FSM state
					FSM_state = FSM_UART_STATE_IDLE;
					return;
				}

				// Received command - SET
				if(PARS_StrCmpF(words[0], CMD_SET)) {
					// If not enough params in string
					if(argc < 3) {
						// Set next FSM state
						FSM_state = FSM_UART_STATE_TX_MSG_ERR_CMD_UNCOMPLETE;
						return;
					}

					// Received subcommand - LOAD
					if(PARS_StrCmpF(words[1], CMD_SET_LOAD)) {
						// Received subcommand params - ON
						if(PARS_StrCmpF(words[2], CMD_ON)) {
							// Send message for turn ON load
							SendMessageWOParam(MSG_SYSTEM_LOAD_ON);
							// Set next FSM state
							FSM_state = FSM_UART_STATE_TX_MSG_OK;
							return;
						}
						// Received subcommand params - OFF
						if(PARS_StrCmpF(words[2], CMD_OFF)) {
							// Send message for turn OFF load
							SendMessageWOParam(MSG_SYSTEM_LOAD_OFF);
							// Set next FSM state
							FSM_state = FSM_UART_STATE_TX_MSG_OK;
							return;
						}
						// Set next FSM state
						FSM_state = FSM_UART_STATE_TX_MSG_ERR_CMD_UNCOMPLETE;
						return;
					}

					// Received subcommand - TFON
					if(PARS_StrCmpF(words[1], CMD_SET_TFON)) {
						// Check device state
						if(device._active) {
							// Set next FSM state
							FSM_state = FSM_UART_STATE_TX_MSG_ERR_ACTIVE_STATE;
							return;
						}

						if(words[2]) {
							// Parsing unsigned data
							uint8_t data = PARS_StrToUint8t(words[2]);
							// Check value range
							if(data < MIN_TFON || data > MAX_TFON) {
								v_min = MIN_TFON;
								v_max = MAX_TFON;
								FSM_state = FSM_UART_STATE_TX_MSG_ERR_VALUE_RANGE;
								return;
							}
							// Set value if all right
							device.settings.tfon = data;
							// Check value between TFON and TFOFF
							if(data <= (device.settings.tfoff + THYSTERESIS)) {
								device.settings.tfoff = data - THYSTERESIS;
								// Set next FSM state
								FSM_state = FSM_UART_STATE_TX_MSG_TOFF_VALUE_CHANGED;
							} else {
								// Set next FSM state
								FSM_state = FSM_UART_STATE_TX_MSG_OK;
							}
							return;
						}
						// Set next FSM state
						FSM_state = FSM_UART_STATE_TX_MSG_ERR_CMD_UNCOMPLETE;
						return;
					}

					// Received subcommand - TFOFF
					if(PARS_StrCmpF(words[1], CMD_SET_TFOFF)) {
						// Check device state
						if(device._active) {
							// Set next FSM state
							FSM_state = FSM_UART_STATE_TX_MSG_ERR_ACTIVE_STATE;
							return;
						}

						if(words[2]) {
							// Parsing unsigned data
							uint8_t data = PARS_StrToUint8t(words[2]);
							// Check value range
							if(data < MIN_TFOFF || data > MAX_TFOFF) {
								v_min = MIN_TFOFF;
								v_max = MAX_TFOFF;
								FSM_state = FSM_UART_STATE_TX_MSG_ERR_VALUE_RANGE;
								return;
							}
							// Set value if all right
							device.settings.tfoff = data;
							// Check value between TFON and TFOFF
							if(data >= (device.settings.tfon - THYSTERESIS)) {
								device.settings.tfon = data + THYSTERESIS;
								// Set next FSM state
								FSM_state = FSM_UART_STATE_TX_MSG_TON_VALUE_CHANGED;
							} else {
								// Set next FSM state
								FSM_state = FSM_UART_STATE_TX_MSG_OK;
							}
							return;
						}
						// Set next FSM state
						FSM_state = FSM_UART_STATE_TX_MSG_ERR_CMD_UNCOMPLETE;
						return;
					}

					// Received subcommand - TOFF
					if(PARS_StrCmpF(words[1], CMD_SET_TOFF)) {
						// Check device state
						if(device._active) {
							// Set next FSM state
							FSM_state = FSM_UART_STATE_TX_MSG_ERR_ACTIVE_STATE;
							return;
						}

						if(words[2]) {
							// Parsing unsigned data
							uint8_t data = PARS_StrToUint8t(words[2]);
							// Check value range
							if(data < MIN_TOFF || data > MAX_TOFF) {
								v_min = MIN_TOFF;
								v_max = MAX_TOFF;
								FSM_state = FSM_UART_STATE_TX_MSG_ERR_VALUE_RANGE;
								return;
							}
							// Set value if all right
							device.settings.toff = data;
							// Set next FSM state
							FSM_state = FSM_UART_STATE_TX_MSG_OK;
							return;
						}
						// Set next FSM state
						FSM_state = FSM_UART_STATE_TX_MSG_ERR_CMD_UNCOMPLETE;
						return;
					}


					// Set next FSM state
					FSM_state = FSM_UART_STATE_IDLE;
					return;
				}

			}


			UART_AddStringFromFlash(CR);
			UART_AddStringFromFlash(ERROR_UNKNOWN_CMD);
			UART_AddStringFromFlash(CONSOLE);

			UART_RX_FlushBuffer();





			//
			FSM_state = FSM_UART_STATE_IDLE;


			return;
		}


		default: break;
	}
}
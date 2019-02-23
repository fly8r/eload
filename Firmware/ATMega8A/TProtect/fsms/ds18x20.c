/*
 * temperature.c
 *
 * Created: 26.09.2018 14:53:54
 *  Author: fly8r
 */
#include "include/ds18x20.h"

/************************************************************************/
/* VARS                                                                 */
/************************************************************************/
volatile	static		uint8_t		FSM_state;

static		uint8_t		sensor_type;

/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/
void FSM_DS18X20_Init(void)
{
	// Set default FSM state
	FSM_state = FSM_DS18X20_STATE_IDLE;
	// Default sensor type
	sensor_type = T_SENS_DS18S20;
}

void FSM_DS18X20_Process(void)
{
	switch(FSM_state) {

		case FSM_DS18X20_STATE_IDLE: {
			// Message for getting ROM data processing
			if(GetMessage(MSG_DS18X20_GET_SERIAL) || GetBCMessage(MSG_BC_SYSTEM_STARTUP_RUN)) {
				// Set next FSM state
				FSM_state = FSM_DS18X20_STATE_GET_SERIAL;
				return;
			}
			// If timer period exceed - prepare to convert T and read scratch pad
			if(sensor_type != T_SENS_UNKNOWN && GetTimer(TIMER_DS18X20) > DS18X20_TEMP_REFRESH_PERIOD) {
				// Set next FSM state
				FSM_state = FSM_DS18X20_STATE_SEND_CONVERT_T;
				return;
			}

			return;
		}

		case FSM_DS18X20_STATE_GET_SERIAL: {
			// If OWS transmitter is busy - wait
			if(ows.f_busy) { return; }
			// Set CMD in TX OWS buffer
			ows.tx_buff[0] = OWS_ROM_CMD_READ_ROM;
			// Set CMD send buffer length
			ows.tx_length = 1;
			// Set DATA receive buffer length
			ows.rx_length = 8;
			// Set flag for check CRC sum
			ows.f_chk_crc = 1;
			// Flush FAILED flag
			tsens._failed = 0;
			// Send message for run OWS transmitter
			SendMessageWOParam(MSG_OWS_RUN);
			// Set next FSM state
			FSM_state = FSM_DS18X20_STATE_SERIAL_PROCESSING;
			return;
		}

		case FSM_DS18X20_STATE_SERIAL_PROCESSING: {
			// Wait message about OWS complete process
			if(GetBCMessage(MSG_BC_OWS_COMPLETE)) {
				// If OWS transmit without errors - processing with data
				if(ows.error == FSM_OWS_ERROR_NO_ERROR) {
					// Copy ROM data from buffer to ow_rom variable
					uint8_t	*p_buff = ows.rx_buff;
					uint8_t *p_rom = (void *)&tsens.rom;
					for(uint8_t i=8;i;i--) {
						*(p_rom++) = *(p_buff++);
					}
					// If family code 0x28
					if(tsens.rom.family == 0x28) {
						// Sensor is DS18B20 type
						sensor_type = T_SENS_DS18B20;
					} else if(tsens.rom.family != 0x10) {
						// Else if family code have another family code
						// unknown type
						sensor_type = T_SENS_UNKNOWN;
					}
				} else {
					// Set FAILED flag
					tsens._failed=1;
				}
				// Set next FSM state
				FSM_state = FSM_DS18X20_STATE_IDLE;
				// Flush FSM timer
				ResetTimer(TIMER_DS18X20);
			}
			return;
		}

		case FSM_DS18X20_STATE_SEND_CONVERT_T: {
			// If OWS transmitter is busy - wait
			if(ows.f_busy) { return; }
			// Set CMD in TX OWS buffer
			ows.tx_buff[0] = OWS_ROM_CMD_SKIP_ROM;
			// Set FUNC in TX OWS buffer
			ows.tx_buff[1] = DS18X20_FUNC_CMD_CONVERT_T;
			// Set CMD send buffer length
			ows.tx_length = 2;
			// Set DATA receive buffer length
			ows.rx_length = 0;
			// Flush FAILED flag
			tsens._failed = 0;
			// Send message for run OWS transmitter
			SendMessageWOParam(MSG_OWS_RUN);
			// Set next FSM state
			FSM_state = FSM_DS18X20_STATE_READ_SCRATCHPAD;
			// Flush FSM timer
			ResetTimer(TIMER_DS18X20);
			return;
		}

		case FSM_DS18X20_STATE_READ_SCRATCHPAD: {

			if(GetTimer(TIMER_DS18X20) > 800) {
				// If OWS transmit without errors - prepare to read scratch pad
				if(ows.error == FSM_OWS_ERROR_NO_ERROR) {
					// If OWS transmitter is busy - wait
					if(ows.f_busy) { return; }
					// Set CMD in TX OWS buffer
					ows.tx_buff[0] = OWS_ROM_CMD_SKIP_ROM;
					// Set FUNC in TX OWS buffer
					ows.tx_buff[1] = DS18X20_FUNC_CMD_READ_SCRATCHPAD;
					// Set CMD send buffer length
					ows.tx_length = 2;
					// Set DATA receive buffer length
					ows.rx_length = 9;
					// Set flag for check CRC sum
					ows.f_chk_crc = 1;
					// Send message for run OWS transmitter
					SendMessageWOParam(MSG_OWS_RUN);
					// Set next FSM state
					FSM_state = FSM_DS18X20_STATE_T_PROCESSING;
					return;
				} else {
					// Set FAILED flag
					tsens._failed = 1;
				}
				// Set default FSM state
				FSM_state = FSM_DS18X20_STATE_IDLE;
			}
			return;
		}

		case FSM_DS18X20_STATE_T_PROCESSING: {
			// Wait message about OWS complete process
			if(GetBCMessage(MSG_BC_OWS_COMPLETE)) {
				// If transmitted without errors
				if(ows.error == FSM_OWS_ERROR_NO_ERROR) {
					// Processing temperature data with sensor type
					int16_t	*p_temp = (void *)&ows.rx_buff;
					// For DS18B20 sensor type
					if(sensor_type == T_SENS_DS18B20) {
						// Additionally shift value to the right
						*p_temp >>=3;
					}
					// For DS18S20 sensor type shift value to the right
					*p_temp >>=1;
					// Store temperature value
					tsens.data.raw = (int8_t)*p_temp;
					// If temperature are lower zero
					if(tsens.data.raw < 0) {
						// Set sign flag
						tsens.data.sign = 1;
						// Prepare unsigned temperature value
						tsens.data.value = -tsens.data.raw;
					} else {
						// Flush sign flag
						tsens.data.sign = 0;
						// Store unsigned temperature value
						tsens.data.value = tsens.data.raw;
					}

					// Prepare BCD temperature value
					char buffer[4];
					char * p_bcd = tsens.data.bcd_value;
					char * p_buff = utoa_cycle_sub8(tsens.data.value, buffer);
					for(uint8_t i=4; i>0; i--) {
						*(p_bcd++) = *(p_buff++);
					}
				} else {
					// Set FAILED flag
					tsens._failed = 1;
				}
				// Set default FSM state
				FSM_state = FSM_DS18X20_STATE_IDLE;
			}
			return;
		}


		default: break;
	}

}
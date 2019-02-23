/*
 * ows.c
 *
 * Created: 15.10.2018 10:39:29
 *  Author: fly8r
 */
#include "include/ows.h"

/************************************************************************/
/* VARS                                                                 */
/************************************************************************/
volatile	static		uint8_t		FSM_state;

static		uint8_t					curr_byte,
									curr_crc8,
									i,
									*p_buff;


/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/
void FSM_OWS_Init(void)
{
	// Set default FSM state
	FSM_state = FSM_OWS_STATE_IDLE;

}

void FSM_OWS_Process(void)
{
	switch(FSM_state) {

		case FSM_OWS_STATE_IDLE: {

			// Received RUN command
			if(GetMessage(MSG_OWS_RUN)) {
				// Flush error
				ows.error = FSM_OWS_ERROR_NO_ERROR;
				// Set BUSY flag
				ows.f_busy = 1;
				// Set next FSM state
				FSM_state = FSM_OWS_STATE_RESET_START;
			}

			return;
		}

		case FSM_OWS_STATE_RESET_START: {
			// Check line state before RESET state(must be HIGH level)
			if(!OW_DQ_READ_BIT) {
				// Set ERROR - LINE NOT READY
				ows.error = FSM_OWS_ERROR_BAD_LINE_OR_DEVICE;
				// Set default FSM state
				FSM_state = FSM_OWS_STATE_COMPLETE;
				return;
			}

			// Start RESET procedure(set line to LOW level)
			OW_DQ_LOW();
			// Set next FSM state
			FSM_state = FSM_OWS_STATE_WAIT_PRESENCE;
			// Reset FSM timer
			ResetTimer(TIMER_OWS);
			return;
		}

		case FSM_OWS_STATE_WAIT_PRESENCE: {
			// Check timer for 1mS timeout
			if(GetTimer(TIMER_OWS) > 0) {
				// Restore line to high level
				OW_DQ_HIGH();
				// Waiting PRESENCE pulse
				for(uint8_t t=200; t; t--) {
					// If presence pulse found
					if(!OW_DQ_READ_BIT) {
						// Reset FSM timer
						ResetTimer(TIMER_OWS);
						// Get TX buff pointer
						p_buff = ows.tx_buff;
						// Set next FSM state
						FSM_state = FSM_OWS_STATE_SEND_BYTES;
						return;
					}
				}
				// Set ERROR - NO DEVICE
				ows.error = FSM_OWS_ERROR_NO_DEVICE;
				// Set default FSM state
				FSM_state = FSM_OWS_STATE_COMPLETE;
			}
			return;
		}

		case FSM_OWS_STATE_SEND_BYTES: {

			if(GetTimer(TIMER_OWS) == 0) {
				return;
			}

			// If we have data for transmit
			if(ows.tx_length-- > 0) {
				// Load current tx_buff data byte for transmit
				curr_byte = *(p_buff++);
				// Set bit counter
				i = 8;
				// Set next FSM state
				FSM_state = FSM_OWS_STATE_SEND_BYTE;
				return;
			} else {
				//
				if(ows.rx_length) {
					// Get RX buff pointer
					p_buff = ows.rx_buff;
					// Flush current CRC sum
					curr_crc8 = 0;
					// Set next FSM state
					FSM_state = FSM_OWS_STATE_RESEIVE_BYTES;
					return;
				}
			}
			// Set default FSM state
			FSM_state = FSM_OWS_STATE_COMPLETE;

			return;
		}

		case FSM_OWS_STATE_SEND_BYTE: {
			// If we have bit for transmit
			if(i-- > 0) {
				ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
					// Prepare current but of byte
					uint8_t bit = (curr_byte & 0x1);
					// OW to LOW level
					OW_DQ_LOW();
					// If bit is 1 - set high level after ~5uS, else do nothing
					if(bit) {
						// Timeout for logical 1
						_delay_us(5);
						// OW to HIGH level
						OW_DQ_HIGH();
					}
					// Time slot delay ~80uS
					_delay_us(80);
					// Then OW to high LEVEL
					OW_DQ_HIGH();
				}
				// Prepare next bit
				curr_byte >>=1;
				return;
			}
			// Set next FSM state
			FSM_state = FSM_OWS_STATE_SEND_BYTES;
			return;
		}

		case FSM_OWS_STATE_RESEIVE_BYTES: {
			// If we will need to receive data
			if(ows.rx_length-- > 0) {
				// Set bit counter
				i = 8;
				// Set next FSM state
				FSM_state = FSM_OWS_STATE_RESEIVE_BYTE;
				return;
			} else {
				// Check calculated CRC with last byte
				if(ows.f_chk_crc && curr_crc8 != *(p_buff)) {
					// Set error if not match
					ows.error = FSM_OWS_ERROR_CRC;
				}
			}
			// Set default FSM state
			FSM_state = FSM_OWS_STATE_COMPLETE;
			return;
		}

		case FSM_OWS_STATE_RESEIVE_BYTE: {
			// If we have bit for receive
			if(i-- > 0) {
				ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
					// Right shift bit in receiver buffer
					*(p_buff) >>= 1;
					// Set line to low level - start time slot
					OW_DQ_LOW();
					// Delay ~6-9uS
					_delay_us(2);
					// Set line to high level - beginning read line data from slave
					OW_DQ_HIGH();
					// Timeout before read line data
					_delay_us(8);
					// If data bit is 1 - set 1 in MSB data byte
					if(OW_DQ_READ_BIT) {
						*(p_buff) |= 0x80;
					}
					_delay_us(80);
				}
				return;
			} else {
				// If current received byte is not last
				if(ows.rx_length > 0) {
					if(ows.f_chk_crc) {
						// Calculate CRC
						curr_crc8 = CRC8(curr_crc8, *(p_buff));
					}
					// Increment buff pointer
					p_buff++;
				}
			}
			// Set next FSM state
			FSM_state = FSM_OWS_STATE_RESEIVE_BYTES;
			return;
		}

		case FSM_OWS_STATE_COMPLETE: {
			// FLush TX buffer length
			ows.tx_length = 0;
			// Flush RX buffer length
			ows.rx_length = 0;
			// Flush BUSY flag
			ows.f_busy = 0;
			// Send COMPLETE message
			SendBCMessage(MSG_BC_OWS_COMPLETE);
			// Set default FSM state
			FSM_state = FSM_OWS_STATE_IDLE;
			return;
		}

		default: break;
	}
}
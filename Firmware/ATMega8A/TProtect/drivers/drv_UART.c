/*
 * drv_UART.c
 *
 * Created: 04.10.2018 12:39:04
 *  Author: fly8r
 */
#include "include/drv_UART.h"

/************************************************************************/
/* VARS                                                                 */
/************************************************************************/
volatile	enum		UART_RX_STATES_ENUM		UART_RX_STATE=UART_RX_STATE_IDLE;
volatile				char					uart_tx_buffer[UART_TX_BUFSIZE],    //	UART output buffer
												uart_rx_buffer[UART_RX_BUFSIZE];    //	UART input buffer
volatile				uint8_t					uart_rx_queue_length=0,				//	UART RX queue length
												uart_rx_buff_tail=0;				//	UART RX buffer pointer

#if (UART_TX_BUFSIZE > 255)
volatile				uint16_t				uart_tx_queue_length=0,             //	UART TX queue length
												uart_p_write=0,                     //	UART write buffer pointer
												uart_p_send=0;                      //	UART send buffer pointer
#else
volatile				uint8_t					uart_tx_queue_length=0,             //	UART TX queue length
												uart_p_write=0,                     //	UART write buffer pointer
												uart_p_send=0;                      //	UART send buffer pointer
#endif

/************************************************************************/
/* FUNCTIONS                                                            */
/************************************************************************/
/* Add string to UART TX cycle buffer */
void UART_AddString(const char *s)
{
#if (UART_TX_BUFSIZE > 255)
	uint16_t	len=0, i=0, qlength=0;
#else
	uint8_t		len=0, i=0, qlength=0;
#endif

	// Calculating string length with buffer overload check
	while(*(s + len) != '\0')
	{
		len++;
		if(len==UART_TX_BUFSIZE) break;
	}

	// Disable UART UDRE Interrupt
	UART_TX_INTERRUPT_DISABLE();
	qlength = uart_tx_queue_length;
	// Enable UART_UDRE Interrupt
	UART_TX_INTERRUPT_ENABLE();

	// If buffer have enough free space for sending string - load string into TX buffer
	if(len < (UART_TX_BUFSIZE - qlength))
	{
		do
		{
			// Load char from string into TX buffer with offset
			uart_tx_buffer[uart_p_write] = *(s++);
			// Increment TX buffer offset
			uart_p_write++;
			// Detect loop back in TX buffer offset
			if(uart_p_write >= UART_TX_BUFSIZE) uart_p_write=0;
			UART_TX_INTERRUPT_DISABLE();
			// Increment TX queue counter length
			uart_tx_queue_length++;
			UART_TX_INTERRUPT_ENABLE();
			//
			i++;
		}
		while(i < len);
	}
}

/* Add to UART TX buffer string from FLASH memory */
void UART_AddStringFromFlash(const char *s)
{
#if (UART_TX_BUFSIZE > 255)
	uint16_t	len=0, i=0, qlength=0;
#else
	uint8_t		len=0, i=0, qlength=0;
#endif

	// Calculating string length with buffer overload check
	while(pgm_read_byte(s + len) != '\0')
	{
		len++;
		if(len == UART_TX_BUFSIZE) break;
	}

	// Disable UART UDRE Interrupt
	UART_TX_INTERRUPT_DISABLE();
	qlength = uart_tx_queue_length;
	// Enable UART_UDRE Interrupt
	UART_TX_INTERRUPT_ENABLE();

	// If buffer have enough free space for sending string - load string into TX buffer
	if(len < (UART_TX_BUFSIZE - qlength))
	{
		do
		{
			// Load char from FLASH string into TX buffer with offset
			uart_tx_buffer[uart_p_write] = pgm_read_byte(s++);
			// Increment TX buffer offset
			uart_p_write++;
			// Detect loop back in TX buffer offset
			if(uart_p_write >= UART_TX_BUFSIZE) uart_p_write=0;
			// Disable UART UDRE Interrupt
			UART_TX_INTERRUPT_DISABLE();
			// Increment TX queue counter length
			uart_tx_queue_length++;
			// Enable UART_UDRE Interrupt
			UART_TX_INTERRUPT_ENABLE();
			// Index increment
			i++;
		}
		while(i < len);
	}
}

/* ISR Service for UDRE interrupt */
void UART_TX_Service(void)
{
	if(uart_tx_queue_length != 0)
	{
		// Copy char from TX buffer to UDR register
		UDR = uart_tx_buffer[uart_p_send];
		// Increment TX sent offset
		uart_p_send++;
		// Detect loop back in TX sent offset
		if(uart_p_send >= UART_TX_BUFSIZE) uart_p_send = 0;
		// Decrement TX queue
		uart_tx_queue_length--;
	} else {
		UART_TX_INTERRUPT_DISABLE();
	}
}

/* ISR Service for RX interrupt */
void UART_RX_Service(void)
{
	// Read UART data
	int8_t	data = UDR;

	switch(data) {
		// Send ENTER
		case '\r':
			// Set UART RX state to start processing
			UART_RX_STATE = UART_RX_STATE_PROCESSING;
			break;

		// Send BACKSPACE
		case 127:
		case 0x08:
			// If not first char in string
			if(uart_rx_queue_length)
			{
				// Echo data into UART
				UDR = data;
				// Decrement buffer pointer
				uart_rx_queue_length--;
				// Decrement buffer pointer
				if(!uart_rx_buff_tail)
				{
					uart_rx_buff_tail = UART_RX_BUFSIZE;
				}
				uart_rx_buff_tail--;
			}
			break;

		// Other chars
		default:
			// If buffer not full - processing
			if(uart_rx_queue_length < UART_RX_BUFSIZE) {
				// Echo data into UART
				UDR = data;
				// Add data to RX buffer
				uart_rx_buffer[uart_rx_buff_tail] = data;
				// Increment buffer tail pointer
				uart_rx_buff_tail++;
				// If getting tail buffer, then point to start buffer
				if(uart_rx_buff_tail == UART_RX_BUFSIZE) uart_rx_buff_tail = 0;
				// Increment data counter
				uart_rx_queue_length++;
				} else {
				// Set UART RX state to buffer overflow
				UART_RX_STATE = UART_RX_STATE_BUFF_OVERFLOW;
			}
			break;
	}
}

/* Flush UART buffer */
void UART_RX_FlushBuffer(void)
{
	uart_rx_buff_tail=0;
	uart_rx_queue_length=0;
}

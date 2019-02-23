/*
 * parser.c
 *
 * Created: 05.10.2018 14:02:25
 *  Author: fly8r
 */
#include "include/parser.h"


/************************************************************************/
/* VARS                                                                 */
/************************************************************************/
char	pars_buffer[UART_RX_BUFSIZE];
char	* words[UART_CMD_WORD_COUNT];


/************************************************************************/
/* FUNCTION                                                             */
/************************************************************************/

/* Arguments parser from string */
uint8_t PARS_ParsStringFromBuffer(volatile char * buffer, uint8_t length)
{
	// If buffer have zero length - skip parsing
	if(!length) return 0;

	// Clean parser buffer
	for(uint8_t i=0; i < UART_RX_BUFSIZE; i++)
	{
		pars_buffer[i] = '\0';
	}

	// Buffer processing
	uint8_t f_arg=0, f_prev_space=0, argc=0;
	for(uint8_t i=0; i < length; i++)
	{
		// Prepare to processing char
		char chr = buffer[i];

		// If char is space or symbol "="
		if(chr == ' ' || chr == '=')
		{
			// If second space received - skip
			if(f_prev_space) continue;
			// If argc overflow word buffer - break FOR
			if(argc == UART_CMD_WORD_COUNT) break;
			// Replace space for 0 char to buffer in current index
			pars_buffer[i] = '\0';
			// Set flag space char received
			f_prev_space = 1;
			// Clear flag new word(arg)
			f_arg = 0;
			// Skip to next char in buffer
			continue;
		}

		// Add current char to pars buffer
		pars_buffer[i] = chr;
		// Clear flag previous space char
		f_prev_space = 0;

		// If new word started
		if(!f_arg)
		{
			// Set flag new word(arg)
			f_arg = 1;
			// Save address point to the first char of word
			words[argc] = &pars_buffer[i];
			// Increment word(arg) counter
			argc++;
		}
	}
	// Return word(arg) counts
	return argc;
}

/* Compare string */
uint8_t PARS_StrCmp(char * str1, char * str2)
{
	// Compare char by char between strings
	uint8_t i=0;
	while(str1[i] == str2[i] && str1[i] != '\0' && str2[i] != '\0')
	{
		i++;
	}
	// Return result compare
	return (str1[i] == '\0' && str2[i] == '\0');
}

/* Compare string with string from flash */
uint8_t PARS_StrCmpF(char * str1, char const * str2)
{
	// Compare char by char between strings
	uint8_t i=0;
	while(str1[i] == pgm_read_byte(&str2[i]) && str1[i] != '\0' && pgm_read_byte(&str2[i]) != '\0')
	{
		i++;
	}
	// Return result compare
	return (str1[i] == '\0' && pgm_read_byte(&str2[i]) == '\0');
}

/* Convert string digits to uint8 value */
uint8_t PARS_StrToUint8t(char * str)
{
	uint8_t val=0;
	char c;

	// Scan string for digits
	while((c=*str++)) {
		// Use only for numeric
		if(c > 0x29 && c < 0x40) {
			// Multiple for 10 before value
			val *= 10;
			// Additional with current digit
			val += (uint8_t)(c - '0');
		}
	}
	// Return value
	return val;
}

/* Convert string digits to uint16 value */
uint16_t PARS_StrToUint16t(char * str)
{
	uint16_t val=0;
	char c;
	
	// Scan string for digits
	while((c=*str++)) {
		// Use only for numeric
		if(c > 0x29 && c < 0x40) {
			// Multiple for 10 before value
			val *= 10;
			// Additional with current digit
			val += (uint16_t)(c - '0');
		}
	}
	// Return value
	return val;
}

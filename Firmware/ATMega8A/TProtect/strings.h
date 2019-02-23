/*
 * strings.h
 *
 * Created: 04.10.2018 13:06:22
 *  Author: fly8r
 */
#ifndef STRINGS_H_
#define STRINGS_H_

#include <stdio.h>
#include <avr/io.h>
#include <avr/pgmspace.h>


extern	const	char
			CELS[],
			CONSOLE[],
			COLON[],
			CR[],
			FAILED[],
			OFF[],
			ON[],
			OK[],
			RPM[],
			VS[],
			DS18B[],
			DS18S[],
			UNKNOWN[],
			MINUS[],
			PLUS[],
			SPACE[],
			SLASH[];


extern	const	char
			FW_STR1[],
			FW_STR2[],
			FW_STR3[],
			FW_STR4[],
			FW_STR5[],
			FW_STR6[],
			FW_STR7[],
			FW_STR8[],
			FW_STR9[],
			FW_STR10[],
			FW_STR11[],
			FW_STR12[],
			FW_STR13[];

extern	const	char
			HLP_STR1[],
			HLP_STR2[],
			HLP_STR3[],
			HLP_STR4[],
			HLP_STR5[],
			HLP_STR6[],
			HLP_STR7[],
			HLP_STR8[],
			HLP_STR9[],
			HLP_STR10[],
			HLP_STR11[];

extern	const	char
			CMD_HELP[],
			CMD_INFO[],
			CMD_RESET[],
			CMD_LOAD[],
			CMD_SAVE[],
			CMD_SET[],
			CMD_SET_TFON[],
			CMD_SET_TFOFF[],
			CMD_SET_TOFF[],
			CMD_SET_LOAD[],
			CMD_OFF[],
			CMD_ON[];

extern	const	char
			MSG_OK[],
			MSG_T_HYSTERESIS[],
			MSG_TFOFF_VALUE_CHANGED[],
			MSG_TFON_VALUE_CHANGED[];

extern	const	char
			ERROR_STR_TOO_LARGE[],
			ERROR_UNCOMPLETE_CMD[],
			ERROR_INVALID_VALUE_CMD[],
			ERROR_CHANGE_VALUE_ACTIVE_CMD[],
			ERROR_UNKNOWN_CMD[];


#endif
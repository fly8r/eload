/*
 * strings.c
 *
 * Created: 05.10.2018 13:47:36
 *  Author: fly8r
 */
#include "strings.h"

const	char
			CELS[]								PROGMEM = "*C",
			COLON[]								PROGMEM = ":",
			CONSOLE[]							PROGMEM = "> ",
			CR[]								PROGMEM = "\r\n",
			FAILED[]							PROGMEM = "failed",
			OFF[]								PROGMEM = "OFF",
			ON[]								PROGMEM = "ON",
			OK[]								PROGMEM = "OK",
			RPM[]								PROGMEM = "RPM",
			VS[]								PROGMEM = "|",
			DS18B[]								PROGMEM = "DS18B20",
			DS18S[]								PROGMEM = "DS18S20",
			UNKNOWN[]							PROGMEM = "unknown",
			MINUS[]								PROGMEM = "-",
			PLUS[]								PROGMEM = "+",
			SPACE[]								PROGMEM = " ",
			SLASH[]								PROGMEM = "/";

const	char
			FW_STR1[]							PROGMEM = ">>> TERMAL PROTECTION <<<\r\n",
			FW_STR2[]							PROGMEM = "(c)2018 fly8r <fly8r@fly8r.ru>\r\n",
			FW_STR3[]							PROGMEM = "FW: 1.0a("__DATE__", "__TIME__"), HW: 1.0\r\n\r\n",
			FW_STR4[]							PROGMEM = "System:\r\n-------\r\n",
			FW_STR5[]							PROGMEM = "TS type:\t",
			FW_STR6[]							PROGMEM = "TS serail:\t",
			FW_STR7[]							PROGMEM = "TS temp:\t",
			FW_STR8[]							PROGMEM = "Fan speed:\t",
			FW_STR9[]							PROGMEM = "Load state:\t",
			FW_STR10[]							PROGMEM = "Params:\r\n-------\r\n",
			FW_STR11[]							PROGMEM = "Tfon:\t",
			FW_STR12[]							PROGMEM = "Tfoff:\t",
			FW_STR13[]							PROGMEM = "Toff:\t";

const	char
			HLP_STR1[]							PROGMEM = "CMD\t| PARAMS\t| Description\r\n",
			HLP_STR2[]							PROGMEM = "---------------------------------------------\r\n",
			HLP_STR3[]							PROGMEM = "info\t|\t\t| Display system info\r\n",
			HLP_STR4[]							PROGMEM = "help\t|\t\t| Return this help desk\r\n",
			HLP_STR5[]							PROGMEM = "set\t| tfon [val]\t| Fan ON temperature\r\n",
			HLP_STR6[]							PROGMEM = "\t| tfoff [val]\t| Fan OFF temperature\r\n",
			HLP_STR7[]							PROGMEM = "\t| toff [val]\t| Emergency SWITCH OFF temperature\r\n",
			HLP_STR8[]							PROGMEM = "\t| load [on|off]\t| On/off load\r\n",
			HLP_STR9[]							PROGMEM = "reset\t|\t\t| Reset TERMAL PROTECTION module\r\n",
			HLP_STR10[]							PROGMEM = "load\t|\t\t| Load params from EEPROM\r\n",
			HLP_STR11[]							PROGMEM = "save\t|\t\t| Save params to EEPROM\r\n";



const	char
			CMD_HELP[]							PROGMEM = "help",
			CMD_INFO[]							PROGMEM = "info",
			CMD_RESET[]							PROGMEM = "reset",
			CMD_LOAD[]							PROGMEM = "load",
			CMD_SAVE[]							PROGMEM = "save",
			CMD_SET[]							PROGMEM = "set",
			CMD_SET_TFON[]						PROGMEM = "tfon",
			CMD_SET_TFOFF[]						PROGMEM = "tfoff",
			CMD_SET_TOFF[]						PROGMEM = "toff",
			CMD_SET_LOAD[]						PROGMEM = "load",
			CMD_OFF[]							PROGMEM = "off",
			CMD_ON[]							PROGMEM = "on";

const	char
			MSG_OK[]							PROGMEM = "++OK++\r\n\r\n",
			MSG_T_HYSTERESIS[]					PROGMEM = "Temperature hysteresis is ",
			MSG_TFOFF_VALUE_CHANGED[]			PROGMEM = "[Tfoff] value was changed to ",
			MSG_TFON_VALUE_CHANGED[]			PROGMEM = "[Tfon] value was changed to ";

const	char
			ERROR_STR_TOO_LARGE[]				PROGMEM = "String too large\r\n",
			ERROR_UNCOMPLETE_CMD[]				PROGMEM = "Command incomplete. For help - print \"help\"\r\n",
			ERROR_INVALID_VALUE_CMD[]			PROGMEM = "Invalid value range. Min/max: ",
			ERROR_CHANGE_VALUE_ACTIVE_CMD[]		PROGMEM = "Could not change value while device turned ON",
			ERROR_UNKNOWN_CMD[]					PROGMEM = "Unknown command. For help - print \"help\"\r\n";
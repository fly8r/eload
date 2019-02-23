/*
 * tools.h
 *
 * Created: 15.10.2018 10:42:35
 *  Author: fly8r
 */
#include <stdio.h>
#include <avr/pgmspace.h>




/************************************************************************/
/* External Functions                                                   */
/************************************************************************/
uint8_t CRC8(uint8_t crc, uint8_t data);
char * hex_to_ascii(uint8_t number, char * buffer);
char * utoa_cycle_sub8(uint8_t value, char * buffer);
char * utoa_cycle_sub16(uint16_t value, char * buffer);

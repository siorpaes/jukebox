/** \file vlccommander.h
 * \author david.siorpaes@gmail.com
 */

#include <stdint.h>

/* Hidraw codes */
#define ONE    0x59
#define TWO    0x5a
#define THREE  0x5b
#define FOUR   0x5c
#define FIVE   0x5d
#define SIX    0x5e
#define SEVEN  0x5f
#define EIGHT  0x60
#define NINE   0x61
#define ZERO   0x62

#define ENTER  0x58
#define BSPACE 0x2a
#define PLUS   0x57
#define MINUS  0x56
#define STAR   0x55
#define SLASH  0x54
#define NLOCK  0x53
#define DOT    0x63

/* State machine */
#define IDLE      0
#define ENTERING  1

int processEntry(uint8_t entry);
int isNumber(int hrcode, int* number);

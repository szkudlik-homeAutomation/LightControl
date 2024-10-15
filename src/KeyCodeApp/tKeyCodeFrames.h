#pragma once

#include "../global.h"

#if CONFIG_KEY_CODE_APP

/**
 * add a code to eeprom
 */
typedef struct
{
	uint8_t size;		// code size, 0 - a dongle, otherwise - number of digits
	uint32_t code;		// a code in binary format

	uint8_t ValidStart;	    // valid only between start-end. If 0 - valid always.
	uint8_t ValidEnd;		// start-end is a time code from the central node. Only older byte from 16-bit timestamp
    uint8_t ButtonBitmap;   // bitmap of buttons shortClick in MESSAGE_BUTTON_PRESS triggered by this key
    						// incorrect code triggers all buttons long pressed
                            // NOTE - for backward compatibility with LightControl, where messages are 8 bytes long, bitmap must be shortened to 8 bits
} tMessageTypeAddCode;
C_ASSERT(sizeof(tMessageTypeAddCode) <= COMMUNICATION_PAYLOAD_DATA_SIZE);
#define MESSAGE_TYPE_ADD_CODE 0x81

/**
 * trigger a code - for debug, inject code as if entered on keypad
 */
typedef struct
{
	uint8_t size;		// code size, 0 - a dongle, otherwise - number of digits
	uint32_t code;		// a code in binary format
} tMessageTypeTriggerCode;
C_ASSERT(sizeof(tMessageTypeTriggerCode) <= COMMUNICATION_PAYLOAD_DATA_SIZE);
#define MESSAGE_TYPE_TRIGGER_CODE 0x82

/**
 * code recieved - an event, to retrieve all codes. Sent as a broadcast
 */
typedef struct
{
	uint8_t size;		// code size, 0 - a dongle, otherwise - number of digits
	uint32_t code;		// a code in binary format
} tMessageTypeCodeRecieved;
C_ASSERT(sizeof(tMessageTypeCodeRecieved) <= COMMUNICATION_PAYLOAD_DATA_SIZE);
#define MESSAGE_TYPE_CODE_RECIEVED 0x83

#endif // CONFIG_KEY_CODE_APP




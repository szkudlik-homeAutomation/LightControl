#pragma once


#include "../../global.h"

/**
 * Sent by the central node - arbitrary set state of a single output
 * this is a broadcast frame
 */
#define MESSAGE_BUTTON_PRESS 0x06
typedef struct
{
  uint16_t ShortClick;   // bitmap of new buttons clicked
  uint16_t LongClick;    // bitmap of new long clicks (note! it will always be preceded by "short click"
  uint16_t DoubleClick;  // bitmap of new double clicks
  uint8_t  ForceSrcId;   // if != 0, the reciever will take this as a sender ID. If == 0, the real sender will be taken (for compatibility)
} tMessageTypeButtonPress;
C_ASSERT(sizeof(tMessageTypeButtonPress) <= CONFIG_COMMUNICATION_PAYLOAD_DATA_SIZE);

/**
 * Set a reaction for a button pressed
 * send by the central node to setup
 */
#define MESSAGE_TYPE_SET_ACTION 0x07

#define BUTTON_TRIGGER_TYPE_CLICK            0
#define BUTTON_TRIGGER_TYPE_LONG_CLICK       1
#define BUTTON_TRIGGER_TYPE_DOUBLE_CLICK     2
#define BUTTON_TRIGGER_TYPE_ANY_CLICK        3

#define BUTTON_ACTION_TYPE_TOGGLE     0
#define BUTTON_ACTION_TYPE_ON         1
#define BUTTON_ACTION_TYPE_OFF        2

typedef struct
{
  uint8_t  OutId;         // the output id to be set
  uint8_t  SenderDevID;   // the ID of the sender of a MESSAGE_BUTTON_PRESS message
  uint8_t  ButtonId    : 4, // the ID of the button triggering the action
           TriggerType : 2, // type of button action -  BUTTON_TRIGGER_TYPE*
           ActionType  : 2; // type of action - BUTTON_ACTION_TYPE*
  uint16_t Timer;         // in case of button is set to "1", by on or toggle, number of seconds after it should be turned off
  uint8_t  OutputsMask;   // bitmap of other outputs to be checked before action
  uint8_t  OutputsStates; // required states of outputs. All masked bits MUST be zero
} tMessageTypeSetAction;
C_ASSERT(sizeof(tMessageTypeSetAction) <= CONFIG_COMMUNICATION_PAYLOAD_DATA_SIZE);


/**
 * Erase all actions - clear Eeprom actions map, all actions need to be re-programmed
 */
#define MESSAGE_TYPE_CLEAR_ACTIONS 0x08

/**
 * get a CRC of all eeprom
 * sent by the central node, the node must respond with MESSAGE_TYPE_EEPROM_CRC_RESPONSE
 */
#define MESSAGE_TYPE_EEPROM_CRC_REQUEST 0x0B
#define MESSAGE_TYPE_EEPROM_CRC_RESPONSE 0x0C
typedef struct
{
  uint8_t  NumOfActions;
  uint16_t EepromCRC;
} tMessageTypeEepromCRCResponse;
C_ASSERT(sizeof(tMessageTypeEepromCRCResponse) <= CONFIG_COMMUNICATION_PAYLOAD_DATA_SIZE);

#define MESSAGE_TYPE_SET_DEFAULT_TIMER 0x0F
typedef struct
{
  uint8_t  OutputID;              // output ID
  uint16_t DefaultTimerValue;     // timer value
} tMessageTypeSetDefaultTimer;
C_ASSERT(sizeof(tMessageTypeSetDefaultTimer) <= CONFIG_COMMUNICATION_PAYLOAD_DATA_SIZE);

#define MESSAGE_TYPE_DEFAULT_TIMER_REQUEST 0x10
typedef struct
{
  uint8_t  OutputID;      // id of an output
} tMessageTypeDefaultTimerRequest;
C_ASSERT(sizeof(tMessageTypeDefaultTimerRequest) <= CONFIG_COMMUNICATION_PAYLOAD_DATA_SIZE);


#define MESSAGE_TYPE_DEFAULT_TIMER_RESPONSE 0x11
typedef struct
{
  uint8_t  OutputID;      // id of an output
  uint16_t DefTimerValue; // default timer. 0 means there's no timer (set forever)
} tMessageTypeDefaultTimerResponse;
C_ASSERT(sizeof(tMessageTypeDefaultTimerResponse) <= CONFIG_COMMUNICATION_PAYLOAD_DATA_SIZE);

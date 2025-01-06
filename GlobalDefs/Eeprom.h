#pragma once
#include "../global.h"
#include <EEPROM.h>


#define EEPROM_CANNARY 0x90     // change if update changes EEPROM structure

#include "../src/tSerialFrames.h"

typedef struct __tEeprom
{
  uint8_t DeviceID;  // MUST be at the 1st position
#if CONFIG_LIGHT_CONTROL_EEPROM_INPUTS_POLARITY
  uint16_t InputPolarity;      // 0 means negative trigger, 1 positive trigger
#endif // CONFIG_LIGHT_CONTROL_EEPROM_INPUTS_POLARITY
#if CONFIG_LIGHT_CONTROL_EEPROM_OUTPUTS_POLARITY
  uint8_t  OutputPolarity;     // 0 means active low, 1 active high
#endif // CONFIG_LIGHT_CONTROL_EEPROM_OUTPUTS_POLARITY

  uint8_t CANNARY;   // a cannary, if is not set to EEPROM_CANNARY the eeprom should be set to default

#if CONFIG_NETWORK
  uint8_t IP[4];
  uint8_t IPMask[4];
  uint8_t Gateway[4];
  uint8_t DNS[4];
  uint8_t MAC[6];
#endif CONFIG_NETWORK
#if CONFIG_LIGHT_CONTROL_APP
  uint16_t DefaultTimerValue[CONFIG_OUTPUT_PROCESS_NUM_OF_PINS];
  uint8_t ActionTableUsage;
  tMessageTypeSetAction ActionTableEntry[CONFIG_LIGHT_CONTROL_ACTION_TABLE_SIZE];
#endif // CONFIG_LIGHT_CONTROL_APP
};

#define EEPROM_CANNARY_OFFSET offsetof(__tEeprom,CANNARY)
#define EEPROM_DEVICE_ID_OFFSET offsetof(__tEeprom,DeviceID)
#define EEPROM_INPUT_POLARITY_OFFSET offsetof(__tEeprom,InputPolarity)
#define EEPROM_OUTPUT_POLARITY_OFFSET offsetof(__tEeprom,OutputPolarity)

#define EEPROM_IP offsetof(__tEeprom,IP[0])
#define EEPROM_IPMASK offsetof(__tEeprom,IPMask[0])
#define EEPROM_GATEWAY offsetof(__tEeprom,Gateway[0])
#define EEPROM_DNS offsetof(__tEeprom,DNS[0])
#define EEPROM_MAC offsetof(__tEeprom,MAC[0])

#define EEPROM_DEFAULT_TIMER_VALUE_OFFSET offsetof(__tEeprom,DefaultTimerValue[0])
#define EEPROM_ACTION_TABLE_USAGE_OFFSET offsetof(__tEeprom,ActionTableUsage)
#define EEPROM_ACTION_TABLE_OFFSET offsetof(__tEeprom,ActionTableEntry[0])
#define EEPROM_CONFIG_ACTION_TABLE_SIZE sizeof(tMessageTypeSetAction)

void SetDefaultEEPromValues(bool force = false);

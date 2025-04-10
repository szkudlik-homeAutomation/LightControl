#include "../global.h"

void SetDefaultEEPromValues(bool force)
{

  if (EEPROM.read(EEPROM_CANNARY_OFFSET) == EEPROM_CANNARY) {
	  DEBUG_PRINTLN_3("Cannary is OK - EEPROM is already initialized");
	  return;
  }

  DEBUG_PRINTLN_3("!!!!!!!!!!!!!!!!!!!!!!!!! CLEAN EEPROM MEMORY - SETTING DEFAULTS !!!!!!!!!!!!!!!!!");
  EEPROM.write(EEPROM_CANNARY_OFFSET,EEPROM_CANNARY);

  // device ID is supposed to be pre-set for a node

#if CONFIG_LIGHT_CONTROL_APP
  EEPROM.write(EEPROM_ACTION_TABLE_USAGE_OFFSET,0);

  for (uint8_t i=0; i<CONFIG_OUTPUT_PROCESS_NUM_OF_PINS; i++)
  {
    EEPROM.write(EEPROM_DEFAULT_TIMER_VALUE_OFFSET+i*(sizeof(uint16_t)),0);
    EEPROM.write(EEPROM_DEFAULT_TIMER_VALUE_OFFSET+i*(sizeof(uint16_t))+1,0); //16bit
  }
#endif //CONFIG_LIGHT_CONTROL_APP

#if CONFIG_NETWORK
  EEPROM.write(EEPROM_IP+0,192);
  EEPROM.write(EEPROM_IP+1,168);
  EEPROM.write(EEPROM_IP+2,101);
  EEPROM.write(EEPROM_IP+3,24);
  EEPROM.write(EEPROM_IPMASK+0,255);
  EEPROM.write(EEPROM_IPMASK+1,255);
  EEPROM.write(EEPROM_IPMASK+2,255);
  EEPROM.write(EEPROM_IPMASK+3,0);
  EEPROM.write(EEPROM_GATEWAY+0,192);
  EEPROM.write(EEPROM_GATEWAY+1,168);
  EEPROM.write(EEPROM_GATEWAY+2,101);
  EEPROM.write(EEPROM_GATEWAY+3,1);
  EEPROM.write(EEPROM_DNS+0,192);
  EEPROM.write(EEPROM_DNS+1,168);
  EEPROM.write(EEPROM_DNS+2,101);
  EEPROM.write(EEPROM_DNS+3,1);
  EEPROM.write(EEPROM_MAC+0,0x18);
  EEPROM.write(EEPROM_MAC+1,0xC0);
  EEPROM.write(EEPROM_MAC+2,0x4D);
  EEPROM.write(EEPROM_MAC+3,0x5A);
  EEPROM.write(EEPROM_MAC+4,0x5B);
  EEPROM.write(EEPROM_MAC+5,0x24);
#endif //CONFIG_NETWORK
}

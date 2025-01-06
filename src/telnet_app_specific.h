#pragma once

#include "../global.h"

#include "lightControl/lightControl_telnet.h"
#include "keyCodeApp/keyCode_telnet.h"

#if CONFIG_NODE_SCAN_EEPROM_STATUS_TASK
bool send_ScanEeprom(Commander &Cmdr);
#define TELNET_APP_SPECIFIC_COMMANDS_CODE	\
  {"ScanEeprom",    send_ScanEeprom,       "Scan all Eeproms and get the status of each node"},
#endif // CONFIG_NODE_SCAN_EEPROM_STATUS_TASK

#if CONFIG_LIGHT_CONTROL_APP_TELNET_COMMANDS
#define TELNET_APP_SPECIFIC_COMMANDS_CODE1 LIGHT_CONTROL_SERVER_COMMANDS
#endif


#if CONFIG_KEY_CODE_APP_TELNET_COMMANDS
#define TELNET_APP_SPECIFIC_COMMANDS_CODE2 KEY_CONFIG_SERVER_COMMANDS
#endif


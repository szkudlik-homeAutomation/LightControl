#pragma once

#include "../../global.h"

#if CONFIG_LIGHT_CONTROL_APP_TELNET_COMMANDS

bool send_ButtonPress(Commander &Cmdr);
bool send_ClearActions(Commander &Cmdr);
bool send_GetEepromCrc(Commander &Cmdr);
bool send_SetDefaultTimer(Commander &Cmdr);
bool send_GetDefaultTimer(Commander &Cmdr);
bool send_addAction(Commander &Cmdr);

#define LIGHT_CONTROL_SERVER_COMMANDS 									\
  {"ButtonPress",     send_ButtonPress,             "MESSAGE_BUTTON_PRESS with a forced src id"},	\
  {"ClearActions",    send_ClearActions,            "MESSAGE_TYPE_CLEAR_ACTIONS dev_id"},			\
  {"GetEepromCrc",    send_GetEepromCrc,            "MESSAGE_TYPE_EEPROM_CRC_REQUEST dev_id"},		\
  {"SetDefTimer",     send_SetDefaultTimer,         "MESSAGE_TYPE_SET_DEFAULT_TIMER dev_id out_id defTimerValue (0=no timer)"},	\
  {"GetDefTimer",     send_GetDefaultTimer,         "MESSAGE_TYPE_DEFAULT_TIMER_REQUEST dev_id out_id"},						\
  {"addAction",       send_addAction,               "MESSAGE_TYPE_SET_ACTION dev_id OutId SenderID ButtonId [ Timer TriggerType ActionType OutputsMask OutputsStates ]"}


#else	// CONFIG_LIGHT_CONTROL_APP_TELNET_COMMANDS

#define LIGHT_CONTROL_SERVER_COMMANDS

#endif	// CONFIG_LIGHT_CONTROL_APP_TELNET_COMMANDS


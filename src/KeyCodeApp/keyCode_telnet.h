#pragma once

#include "../../global.h"

#if CONFIG_KEY_CODE_APP_TELNET_COMMANDS

bool send_ClearCodes(Commander &Cmdr);
bool send_addCode(Commander &Cmdr);
bool send_triggerCode(Commander &Cmdr);

#define KEY_CONFIG_SERVER_COMMANDS 														\
  {"ClearCodes",    send_ClearCodes,       "KEY CODE: ClearCodes dev_id"},		\
  {"AddCode",       send_addCode,		   "KEY CODE: AddCode dev_id type code ButtonBitmap"},	\
  {"TriggerCode",   send_triggerCode,	   "KEY CODE: TriggerCode dev_id type code"},

#endif	// CONFIG_KEY_CODE_APP_TELNET_COMMANDS


#pragma once

#include "../global.h"

#include "lightControl/lightControl_telnet.h"
#include "keyCodeApp/keyCode_telnet.h"

#if CONFIG_LIGHT_CONTROL_APP_TELNET_COMMANDS
#define TELNET_APP_SPECIFIC_COMMANDS_CODE LIGHT_CONTROL_SERVER_COMMANDS
#endif


#if CONFIG_KEY_CODE_APP_TELNET_COMMANDS
#define TELNET_APP_SPECIFIC_COMMANDS_CODE1 KEY_CONFIG_SERVER_COMMANDS
#endif


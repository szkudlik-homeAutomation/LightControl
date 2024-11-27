#pragma once

#include "../global.h"

#if CONFIG_TELNET_SERVER

#include "lightControl/lightControl_telnet.h"

#define TELNET_APP_SPECIFIC_COMMANDS_CODE LIGHT_CONTROL_SERVER_COMMANDS

#endif // CONFIG_TELNET_SERVER


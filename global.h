#pragma once

#if APP_central
#include "generated/central.config.h"
#include "appDefs/central.config.h"
#elif APP_node
#include "generated/node.config.h"
#include "appDefs/node.config.h"
#elif APP_keyCode
#include "generated/keyCode.config.h"
#include "appDefs/keyCode.config.h"
#else
#error "Unknown app."
#endif

#include "src/Common_code/commonGlobals.h"

#include "GlobalDefs/version.h"
#include "GlobalDefs/Eeprom.h"
#include "GlobalDefs/common.h"

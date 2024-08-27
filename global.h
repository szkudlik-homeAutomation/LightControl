#pragma once

#if APP_central
#include "generated/central.config.h"
#include "appDefs/central.config.h"
#else
#error "Unknown app."
#endif

#include "src/Common_code/commonGlobals.h"

#include "GlobalDefs/version.h"
#include "GlobalDefs/Eeprom.h"


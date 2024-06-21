/*
 * global.h
 *
 * a common file that MUST be included in every .h and .c**
 *
 *  Created on: 6 pa� 2022
 *      Author: szkud
 */
#pragma once

#include <Arduino.h>

#define CENTRAL_NODE_DEVICE_ID 1
#define DEVICE_ID_BROADCAST 0xFF

#define CONFIG_TLE8457_COMM_LIB 1
#define CONFIG_OUTPUT_PROCESS 1

#define CONFIG_WATCHDOG 1

// maximum number of nodes, should not be mode than 32 because of bitmaps
#define MAX_NUM_OF_NODES 32

#define NUM_OF_OUTPUTS 8


#if APP_Central
#include "appDefs\central.h"
#elif APP_generalTest
#include "appDefs\generalTest.h"
#elif APP_node
#include "appDefs\node.h"
#else
#error no app defined
#endif

#include "GlobalDefs/CommDefs.h"
#include "GlobalDefs/version.h"
#include "GlobalDefs/Eeprom.h"
//#include "GlobalDefs/outputPins.h"
#include "src/Common_code/debug.h"
#include "src/Common_code/helpers.h"
#include "src/Common_code/status.h"


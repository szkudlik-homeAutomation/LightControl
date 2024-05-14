/*
 * global.h
 *
 * a common file that MUST be included in every .h and .c**
 *
 *  Created on: 6 pa� 2022
 *      Author: szkud
 */
#pragma once


// #define CONFIG_TLE8457_serial_lib

#ifdef __AVR_ATmega2560__
#define CONFIG_CENTRAL_NODE 1
#elif __AVR_ATmega328P__
#else
#error unknown board
#endif

#include <Arduino.h>

#if CONFIG_CENTRAL_NODE

//#define DEBUG_1_ENABLE
//#define DEBUG_2_ENABLE
#define DEBUG_3_ENABLE

#define DEBUG_SERIAL Serial
#define DEBUG_SERIAL_EVENT serialEvent

#endif //CONFIG_CENTRAL_NODE

//todo: move
#define CENTRAL_NODE_DEVICE_ID 1
#define DEVICE_ID_BROADCAST 0xFF

#define CONFIG_TLE8457_COMM_LIB 1
#define CONFIG_OUTPUT_PROCESS 1

#include "GlobalDefs/CommDefs.h"

#if CONFIG_CENTRAL_NODE
#define CONFIG_NETWORK 1
#define CONFIG_HTTP_SERVER 1
#define CONFIG_LOGGER 1
#define CONFIG_TELNET_SERVER 1
#define CONFIG_WORKER_PROCESS 1
#define CONFIG_NODE_SCAN_TASK 1

#define CONFIG_SENSORS 0
#define CONFIG_SENSOR_HUB 0
#define CONFIG_SENSORS_JSON_OUTPUT 0
#define CONFIG_SYSTEM_STATUS_SENSOR 0
#define CONFIG_SENSOR_STATE_SERVLET 0

#endif

#define CONFIG_WATCHDOG 1


#if APP_Central
#include "appDefs\central.h"
#elif APP_generalTest
#include "appDefs\generalTest.h"
#elif APP_node
#include "appDefs\node.h"
#else
#error no app defined
#endif

#include "GlobalDefs/TLE8457_serial_lib_defs.h"
#include "GlobalDefs/version.h"
#include "GlobalDefs/Eeprom.h"
//#include "GlobalDefs/outputPins.h"
#include "src/Common_code/debug.h"
#include "src/Common_code/helpers.h"
#include "src/Common_code/status.h"


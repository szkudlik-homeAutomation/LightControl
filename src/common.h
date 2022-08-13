#ifndef COMMON_H
#define COMMON_H

#include <Arduino.h>

#ifdef __AVR_ATmega2560__
#define CONTROLLER
#elif __AVR_ATmega328P__
#else
#error unknown board
#endif

#define CONTROLLER_DEVICE_ID 1
#define DEVICE_ID_BROADCAST 0xFF



#ifdef CONTROLLER

//#define DEBUG_1
//#define DEBUG_2
#define DEBUG_3

#define DEBUG_SERIAL Serial
#define DEBUG_SERIAL_EVENT serialEvent

#include "ResponseHandler.h"

#endif //ifdef CONTROLLER




#ifdef DEBUG_1
#define DEBUG_PRINTLN_1(x) RespHandler.println(F(x));
#else
#define DEBUG_PRINTLN_1(x)
#endif

#ifdef DEBUG_2
#define DEBUG_PRINTLN_2(x) RespHandler.println(F(x));
#else
#define DEBUG_PRINTLN_2(x)
#endif

#ifdef DEBUG_3
#define DEBUG_PRINTLN_3(x) RespHandler.println(F(x));
#else
#define DEBUG_PRINTLN_3(x)
#endif









#endif

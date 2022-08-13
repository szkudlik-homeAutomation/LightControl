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

#include "debug.h"

#endif

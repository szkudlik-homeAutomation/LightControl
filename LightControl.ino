#include "global.h"


#include <Arduino.h>
#include "src/lib/ArduinoProcessScheduler/src/ProcessScheduler.h"

#include "src/Common_code/TLE8457_serial/TLE8457_serial_lib.h"
#include "src/Common_code/WatchdogProcess.h"
#include "src/DigitalInputProcess.h"
#include "src/tOutputProcess_lightControl.h"
#include "src/IncomingFrameHandler.h"

#ifdef CONTROLLER
#include "src/Common_code/WorkerProcess.h"
#include "src/servlets.h"
#include "src/Common_code/Network/network.h"
#include "src/Common_code/Network/httpServer.h"
#include "src/LightWebControl.h"
#endif

// restart if no connection for 5 minutes
#define TCP_WATCHDOG_TIMEOUT 300 

Scheduler sched;
IncomingFrameHandler IncomingFrameHandlerCallback;
CommRecieverProcess CommReciever(sched,EEPROM.read(EEPROM_DEVICE_ID_OFFSET),&IncomingFrameHandlerCallback);
CommSenderProcess CommSender(sched,EEPROM.read(EEPROM_DEVICE_ID_OFFSET),EEPROM.read(EEPROM_DEVICE_ID_OFFSET));

#ifdef CONTROLLER
WorkerProcess Worker(sched);
tNetwork Network;
tTcpServerProcess TcpServerProcess(sched,TCP_WATCHDOG_TIMEOUT);
tHttpServer HttpServer;

tHttpServlet * ServletFactory(String *pRequestBuffer)
{
   if (pRequestBuffer->startsWith("/outputState")) return new tOutputStateServlet();
   if (pRequestBuffer->startsWith("/outputSet")) return new tOutputSetServlet();
   if (pRequestBuffer->startsWith("/timerset")) return new tSetTimerServlet();
   if (pRequestBuffer->startsWith("/button")) return new tForceButtonPressServlet();

   if (pRequestBuffer->startsWith("/1.js")) return new tjavaScriptServlet();
   if (pRequestBuffer->startsWith("/garden")) return new tGardenLightsServlet();
   if (pRequestBuffer->startsWith("/indoorLights")) return new tIndoorLightsServlet();

   return new tDefaultPageServlet();
}

#else
DigitalInputProcess DigitalInput(sched);
#endif

tOutputProcess_lightControl OutputProcess(sched);
tWatchdogProcess  WatchdogProcess(sched);

void COMM_SERIAL_EVENT() {
  CommReciever.serialEvent();
}





void setup() {
  if (EEPROM.read(EEPROM_CANNARY_OFFSET) != EEPROM_CANNARY)
    SetDefaultEEPromValues();

  COMM_SERIAL.begin(9600);
  while (!COMM_SERIAL);
#ifdef DEBUG_SERIAL
  DEBUG_SERIAL.begin(115200);
  while (!DEBUG_SERIAL);
  DEBUG_SERIAL.print("START, v");
  DEBUG_SERIAL.println(FW_VERSION);

#endif
  CommSender.add();
  CommReciever.add();
  WatchdogProcess.add(true);
#ifdef DEBUG_SERIAL
  DEBUG_SERIAL.println("START internals");
#endif

#ifdef CONTROLLER
  Network.init();
  TcpServerProcess.add(true);
  Worker.add();
#ifdef DEBUG_SERIAL
  DEBUG_SERIAL.println("START Tcp ");
#endif
#else   
  DigitalInput.add(true);
#ifdef DEBUG_SERIAL
  DEBUG_SERIAL.println("START digital input");
#endif
#endif

  OutputProcess.add(true);
#ifdef DEBUG_SERIAL
  DEBUG_SERIAL.println("SYSTEM INITIALIZED");
#endif
}

void loop() {
  sched.run();
}

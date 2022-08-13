#include <ProcessScheduler.h>
#include <Arduino.h>

#include "src/common.h"
#include "src/TLE8457_serial_lib_defs.h"
#include "src/Eeprom.h"
#include "src/WorkerProcess.h"
#include "src/DigitalInputProcess.h"
#include "src/OutputProcess.h"
#include "src/WatchdogProcess.h"
#include "src/IncomingFrameHandler.h"
#include "src/version.h"

#ifdef CONTROLLER
#include "src/servlets.h"
#include "src/network.h"
#include "src/TelnetServer.h"
#include "src/TCP_Communication_lib/HttpServer.h"
#include "src/LightWebControl.h"
#endif


 
Scheduler sched;
IncomingFrameHandler IncomingFrameHandlerCallback;
CommRecieverProcess CommReciever(sched,EEPROM.read(EEPROM_DEVICE_ID_OFFSET),&IncomingFrameHandlerCallback);
CommSenderProcess CommSender(sched,EEPROM.read(EEPROM_DEVICE_ID_OFFSET),EEPROM.read(EEPROM_DEVICE_ID_OFFSET));
WorkerProcess Worker(sched);

#ifdef CONTROLLER
tNetwork Network;
tTcpServerProcess TcpServerProcess(sched);
tTelnetServer TelnetServer;
tHttpServer HttpServer;

tHttpServlet * ServletFactory(String *pRequestBuffer)
{
   if (pRequestBuffer->startsWith("/outputState")) return new tOutputStateServlet();
   if (pRequestBuffer->startsWith("/outputSet")) return new tOutputSetServlet();
   if (pRequestBuffer->startsWith("/timerset")) return new tSetTimerServlet();

   if (pRequestBuffer->startsWith("/1.js")) return new tjavaScriptServlet();
   if (pRequestBuffer->startsWith("/garden")) return new tGardenLightsServlet();
   if (pRequestBuffer->startsWith("/indoorLights")) return new tIndoorLightsServlet();
   if (pRequestBuffer->startsWith("/button")) return new tForceButtonPressServlet();


   return new tDefaultPageServlet();
}

#else
DigitalInputProcess DigitalInput(sched);
#endif

tOutputProcess OutputProcess(sched);
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
  Worker.add();
  WatchdogProcess.add(true);
#ifdef DEBUG_SERIAL
  DEBUG_SERIAL.println("START internals");
#endif

#ifdef CONTROLLER
  Network.init();
  TcpServerProcess.add(true);
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

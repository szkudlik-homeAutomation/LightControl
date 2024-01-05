#include "global.h"


#include <Arduino.h>
#include "src/lib/ArduinoProcessScheduler/src/ProcessScheduler.h"

#include "src/Common_code/TLE8457_serial/TLE8457_serial_lib.h"
#include "src/Common_code/TLE8457_serial/tIncomingFrameHanlder.h"
#include "src/Common_code/WatchdogProcess.h"
#include "src/DigitalInputProcess.h"
#include "src/tLightControlOutputProcess.h"
#include "src/tLightControl.h"

#if CONFIG_CENTRAL_NODE
#include "src/Common_code/WorkerProcess.h"
#include "src/servlets.h"
#include "src/Common_code/sensors/tSensor.h"
#include "src/Common_code/sensors/tSensorHub.h"
#include "src/Common_code/sensors/tSystemStatusSensor.h"
#include "src/Common_code/Network/servlets/servlets.h"
#include "src/Common_code/Network/servlets/tSensorStateServlet.h"
#include "src/Common_code/Network/network.h"
#include "src/Common_code/Network/httpServer.h"
#include "src/LightWebControl.h"
#endif

// restart if no connection for 10 minutes
#define TCP_WATCHDOG_TIMEOUT 600 

Scheduler sched;
tIncomingFrameHanlder CommonIncomingFrameHandler;
tLightControl LightControlIncomingFrameHandler;
CommRecieverProcess CommReciever(sched,EEPROM.read(EEPROM_DEVICE_ID_OFFSET));
CommSenderProcess CommSender(sched,EEPROM.read(EEPROM_DEVICE_ID_OFFSET),EEPROM.read(EEPROM_DEVICE_ID_OFFSET));

#if CONFIG_CENTRAL_NODE
WorkerProcess Worker(sched);
tNetwork Network;
tTcpServerProcess TcpServerProcess(sched,TCP_WATCHDOG_TIMEOUT);
tHttpServer HttpServer;
#if CONFIG_SENSOR_HUB
tSensorHub SensorHub;
#endif
#if CONFIG_SENSORS
tSensorProcess SensorProcess(sched);
#endif

tHttpServlet * ServletFactory(String *pRequestBuffer)
{
   if (pRequestBuffer->startsWith("/outputState")) return new tOutputStateServlet();
   if (pRequestBuffer->startsWith("/outputSet")) return new tOutputSetServlet();
   if (pRequestBuffer->startsWith("/timerset")) return new tSetTimerServlet();
   if (pRequestBuffer->startsWith("/button")) return new tForceButtonPressServlet();
#if CONFIG_SENSOR_STATE_SERVLET
   if (pRequestBuffer->startsWith("/sensorState")) return new tSensorStateServlet();
#endif

   if (pRequestBuffer->startsWith("/1.js")) return new tjavaScriptServlet();
   if (pRequestBuffer->startsWith("/garden")) return new tGardenLightsServlet();
   if (pRequestBuffer->startsWith("/indoorLights")) return new tIndoorLightsServlet();
   return new tDefaultPageServlet();
}

#else
DigitalInputProcess DigitalInput(sched);
#endif

tLightControlOutputProcess OutputProcess(sched);
tWatchdogProcess  WatchdogProcess(sched);

void COMM_SERIAL_EVENT() {
  CommReciever.serialEvent();
}




#define SENSOR_ID_SYSTEM_STATUS 1

void setup() {
#ifdef DEBUG_SERIAL
  DEBUG_SERIAL.begin(115200);
  while (!DEBUG_SERIAL);
  DEBUG_SERIAL.print("START, v");
  DEBUG_SERIAL.println(FW_VERSION);
#endif

  if (EEPROM.read(EEPROM_CANNARY_OFFSET) != EEPROM_CANNARY)
    SetDefaultEEPromValues();

  COMM_SERIAL.begin(9600);
  while (!COMM_SERIAL);

  CommSender.add();
  CommReciever.add();
  WatchdogProcess.add(true);
#ifdef DEBUG_SERIAL
  DEBUG_SERIAL.println("START internals");
#endif

#if CONFIG_CENTRAL_NODE
  Network.init();
  TcpServerProcess.add(true);
  Worker.add();
  
#if CONFIG_SENSORS
  SensorProcess.add(true);
  tSystemStatusSensor *pSystemStatusSensor = new tSystemStatusSensor;

  pSystemStatusSensor->setConfig(10); // 1 sec
  pSystemStatusSensor->Register(SENSOR_ID_SYSTEM_STATUS,"SystemStatus");
  pSystemStatusSensor->Start();
#endif CONFIG_SENSORS
  
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

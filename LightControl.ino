

#include "global.h"
#include "src/Common_code/tApplication.h"
#include "src/Common_code/Network/httpServer.h"

#include "src/Common_code/sensors/tSensorFactory.h"
#include "src/Common_code/sensors/tSensorCache.h"
#include "src/Common_code/sensors/tSensor.h"
#include "src/Common_code/sensors/tSystemStatusSensor.h"
#include "src/Common_code/sensors/tOutputStateSensor.h"

#if CONFIG_LIGHT_CONTROL_APP
#include "src/LightControl/tLightControlServlets.h"
#include "src/LightControl/nodesEepromScanServlet.h"
#include "src/LightControl/tLightControlgetActionsJSONservlet.h"
#include "src/LightControl/tLightControlOutputProcess.h"
#include "src/LightControl/tLightControlInputProcess.h"

#if CONFIG_OUTPUT_PROCESS & !CONFIG_OUTPUT_PROCESS_INSTANCE
tLightControlOutputProcess LightControlOutputProcess;
#endif

#if CONFIG_INPUT_PROCESS & !CONFIG_INPUT_PROCESS_INSTANCE
tLightControlInputProcess LightControlInputProcess;
#endif

#endif CONFIG_LIGHT_CONTROL_APP

#if CONFIG_HTTP_SERVER

// specific http server

class tAppHttpServer : public tHttpServer {
public:
	tAppHttpServer() : 	tHttpServer() {}

	virtual tHttpServlet *ServletFactory(String *pRequestBuffer)
	{
#if CONFIG_LIGHT_CONTROL_APP_HTTP_SERVLETS
		if (pRequestBuffer->startsWith("/timerset")) return new tLightControl_SetTimerServlet();
		if (pRequestBuffer->startsWith("/button")) return new tLightControl_ForceButtonPressServlet();
		if (pRequestBuffer->startsWith("/1.js")) return new tLightControl_javaScriptServlet();
		if (pRequestBuffer->startsWith("/getactions")) return new tLightControl_getActionsJSON_servlet();
#if CONFIG_NODE_SCAN_EEPROM_STATUS_TASK
		if (pRequestBuffer->startsWith("/nodeseepromscan")) return new tLightControl_nodesEepromScanServlet();
#endif CONFIG_NODE_SCAN_EEPROM_STATUS_TASK
		if (pRequestBuffer->startsWith("/garden")) return new tLightControl_GardenLightsServlet();
		if (pRequestBuffer->startsWith("/indoorlights")) return new tLightControl_IndoorLightsServlet();
#endif CONFIG_LIGHT_CONTROL_APP_HTTP_SERVLETS
		return NULL;
	}

	 virtual tHttpServlet *getDefaultServlet(String *pRequestBuffer)
	 {
		 return new tLightControl_DefaultPageServlet();
	 }
};

// instantiate the process
tAppHttpServer AppHttpServer;

#endif CONFIG_HTTP_SERVER

#define SYSTEM_STATUS_SENSOR_ID 1
#define OUTPUT_STATE_SENSOR_ID 2

class tHomeAutomation: public tApplication {
public:
	tHomeAutomation() : tApplication() {}

protected:

	virtual void AppSetupBefore() {
		// set default values for eeprom
		SetDefaultEEPromValues();
	}
	
	virtual void AppSetupAfter() {

		#if CONFIG_SYSTEM_STATUS_SENSOR 
		// create the system status sensor, 
		tSensorFactory::Instance->CreateSensor(SENSOR_TYPE_SYSTEM_STATUS, SYSTEM_STATUS_SENSOR_ID, F("SystemStatus01"),
			1,NULL,0,CONFIG_SYSTEM_STATUS_SENSOR_PERIOD,true, 1 << EV_TYPE_MEASUREMENT_COMPLETED);
		#endif
		#if CONFIG_OUTPUT_STATE_SENSOR
		// create the output state sensor, 1 minute cycle. Set Node ID as a sensor ID
			tSensorFactory::Instance->CreateSensor(
					SENSOR_TYPE_OUTPUT_STATES,
					OUTPUT_STATE_SENSOR_ID,F("GPIOout"),1,
					NULL,0,640,true, 1 << EV_TYPE_MEASUREMENT_COMPLETED | 1 << EV_TYPE_MEASUREMENT_CHANGE);
		#endif
	
    }
};

tHomeAutomation HomeAutomation;
	
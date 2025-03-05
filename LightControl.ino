

#include "global.h"
#include "src/Common_code/tApplication.h"
#include "src/LightControl/tLightControlOutputProcess.h"
#include "src/LightControl/tLightControlInputProcess.h"
#include "src/Common_code/Network/httpServer.h"
#include "src/Common_code/sensors/tSensorFactory.h"
#include "src/Common_code/sensors/tSensor.h"

#if CONFIG_LIGHT_CONTROL_APP
#include "src/LightControl/tLightControlServlets.h"
#include "src/LightControl/nodesEepromScanServlet.h"
#include "src/LightControl/tLightControlgetActionsJSONservlet.h"

#if CONFIG_INPUT_PROCESS
tLightControlInputProcess LightControlInputProcess;
#endif CONFIG_INPUT_PROCESS

#if CONFIG_OUTPUT_PROCESS
tLightControlOutputProcess LightControlOutputProcess;
#endif CONFIG_OUTPUT_PROCESS

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
		if (pRequestBuffer->startsWith("/indoorLights")) return new tLightControl_IndoorLightsServlet();
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

char sensorName[] = "System Status";
class tHomeAutomation: public tApplication {
public:
	tHomeAutomation() : tApplication() {}

protected:

	virtual void AppSetupBefore() {
		// set default values for eeprom
		SetDefaultEEPromValues();
	}
	
	virtual void AppSetupAfter() {
        // create sensor
        #if CONFIG_SYSTEM_STATUS_SENSOR        
        tSensorFactory::Instance->CreateSensor(SENSOR_TYPE_SYSTEM_STATUS, 1, sensorName,1 , NULL, 0, 10, true);
        #endif
    }
};

tHomeAutomation HomeAutomation;

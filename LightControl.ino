

#include "global.h"
#include "src/Common_code/tApplication.h"
#include "src/LightControl/tLightControlOutputProcess.h"
#include "src/Common_code/Network/httpServer.h"

#if CONFIG_LIGHT_CONTROL_APP
#include "src/LightControl/tLightControlServlets.h"


tLightControlOutputProcess LightControlOutputProcess;
#endif CONFIG_LIGHT_CONTROL_APP

#if CONFIG_HTTP_SERVER

// specific http server

class tAppHttpServer : public tHttpServer {
public:
	tAppHttpServer() : 	tHttpServer() {}

	virtual tHttpServlet *ServletFactory(String *pRequestBuffer)
	{
#if CONFIG_LIGHT_CONTROL_APP_TELNET_COMMANDS
		if (pRequestBuffer->startsWith("/timerset")) return new tLightControl_SetTimerServlet();
		if (pRequestBuffer->startsWith("/button")) return new tLightControl_ForceButtonPressServlet();
		if (pRequestBuffer->startsWith("/1.js")) return new tLightControl_javaScriptServlet();
		if (pRequestBuffer->startsWith("/garden")) return new tLightControl_GardenLightsServlet();
		if (pRequestBuffer->startsWith("/indoorLights")) return new tLightControl_IndoorLightsServlet();
#endif CONFIG_LIGHT_CONTROL_APP_TELNET_COMMANDS
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


class tHomeAutomation: public tApplication {
public:
	tHomeAutomation() : tApplication() {}

protected:

	virtual void AppSetupBefore() {
		// set default values for eeprom
		SetDefaultEEPromValues();
	}
};

tHomeAutomation HomeAutomation;



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
#if CONFIG_LIGHT_CONTROL_APP
		if (pRequestBuffer->startsWith("/timerset")) return new tLightControl_SetTimerServlet();
#endif CONFIG_LIGHT_CONTROL_APP
		return NULL;
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

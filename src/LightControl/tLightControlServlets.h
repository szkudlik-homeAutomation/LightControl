#pragma once

#include "../../global.h"

#if CONFIG_LIGHT_CONTROL_APP_HTTP_SERVLETS

#include "../Common_code/Network/httpServer.h"
#include "../tSerialFrames.h"

class tLightControl_getActions_XML : public tHttpServlet
{
public:
	tLightControl_getActions_XML() : tHttpServlet() {}
	virtual ~tLightControl_getActions_XML() {}

	virtual bool ProcessAndResponse();

private:
	void tMessageTypeSetActionToXML(tMessageTypeSetAction *pAction);
};

class tLightControl_servlets : public tHttpServlet
{
public:
	tLightControl_servlets() : tHttpServlet() {}

protected:
	void SendSetupFooter();
};


/**
 * output servlet - controlling or checking state of an output
 */
class tLightControl_SetTimerServlet : public tLightControl_servlets
{
public:
  tLightControl_SetTimerServlet() : tLightControl_servlets() {}
  virtual ~tLightControl_SetTimerServlet() {}

  virtual bool ProcessAndResponse();
};

/**
 * force button servlet - send a button press
 */
class tLightControl_ForceButtonPressServlet :  public tLightControl_servlets
{
public:
  tLightControl_ForceButtonPressServlet() : tLightControl_servlets() {}
  virtual ~tLightControl_ForceButtonPressServlet() {}

  virtual bool ProcessAndResponse();
};


class tLightControl_javaScriptServlet :  public tLightControl_servlets
{
public:
  tLightControl_javaScriptServlet() : tLightControl_servlets() {}
  virtual ~tLightControl_javaScriptServlet() {}

  virtual bool ProcessAndResponse();
};

class tLightControl_GardenLightsServlet :  public tLightControl_servlets
{
public:
  tLightControl_GardenLightsServlet() : tLightControl_servlets() {}
  virtual ~tLightControl_GardenLightsServlet() {}

  virtual bool ProcessAndResponse();
};

class tLightControl_IndoorLightsServlet :  public tLightControl_servlets
{
public:
  tLightControl_IndoorLightsServlet() : tLightControl_servlets() {}
  virtual ~tLightControl_IndoorLightsServlet() {}

  virtual bool ProcessAndResponse();
};

class tLightControl_DefaultPageServlet :  public tLightControl_servlets
{
public:
  tLightControl_DefaultPageServlet() : tLightControl_servlets() {}
  virtual ~tLightControl_DefaultPageServlet() {}

  virtual bool ProcessAndResponse();
};



#endif // CONFIG_LIGHT_CONTROL_APP_HTTP_SERVLETS


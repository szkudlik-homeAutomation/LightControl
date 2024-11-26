#pragma once

#include "../../global.h"

#if CONFIG_LIGHT_CONTROL_APP

#include "../Common_code/Network/httpServer.h"

/**
 * output servlet - controlling or checking state of an output
 */
class tLightControl_SetTimerServlet : public tHttpServlet
{
public:
  tLightControl_SetTimerServlet() : tHttpServlet() {}
  virtual ~tLightControl_SetTimerServlet() {}

  virtual bool ProcessAndResponse();
};


#endif // CONFIG_LIGHT_CONTROL_APP


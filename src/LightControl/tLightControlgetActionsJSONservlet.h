/*
 * tLightControlgetActionsJSONservlet.h
 *
 *  Created on: 11 sty 2025
 *      Author: szkud
 */

#ifndef SRC_LIGHTCONTROL_TLIGHTCONTROLGETACTIONSJSONSERVLET_H_
#define SRC_LIGHTCONTROL_TLIGHTCONTROLGETACTIONSJSONSERVLET_H_

#include "../../global.h"

#if CONFIG_LIGHT_CONTROL_APP_HTTP_SERVLETS

#include "../Common_code/Network/httpServer.h"
#include "../tSerialFrames.h"

class tLightControl_getActionsJSON_servlet final : public tHttpServlet {
public:
	tLightControl_getActionsJSON_servlet() {}
	virtual ~tLightControl_getActionsJSON_servlet() {}

	virtual bool ProcessAndResponse();

private:
	void tMessageTypeSetActionToJSON(tMessageTypeSetAction *pAction);
};

#endif /* SRC_LIGHTCONTROL_TLIGHTCONTROLGETACTIONSJSONSERVLET_H_ */

#endif /* CONFIG_LIGHT_CONTROL_APP_HTTP_SERVLETS */

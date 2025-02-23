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
#include "../Common_code/tMessageReciever.h"
#include "../tSerialFrames.h"

class tLightControl_getActionsJSON_servlet final : public tHttpServlet, public tMessageReciever {
public:
	tLightControl_getActionsJSON_servlet() : mState(STATE_INIT), mNumOfActons(0), tHttpServlet() {
		RegisterMessageType(MessageType_SerialFrameRecieved);
	}
	virtual ~tLightControl_getActionsJSON_servlet() { isRunning = false; }

	virtual bool ProcessAndResponse();

protected:
	virtual void onMessage(uint8_t type, uint16_t data, void *pData);

private:
	void tMessageTypeSetActionToJSON(tMessageTypeSetAction *pAction);

	static const uint8_t STATE_INIT = 0;
	static const uint8_t STATE_NO_DATA_RECIEVED = 1;
	static const uint8_t STATE_RUNNING = 2;
	static const uint8_t STATE_FINISHED = 3;
	uint8_t mState;
	uint8_t mNumOfActons;
	uint8_t mDeviceId;
	unsigned long mStartTimestamp;
	static bool isRunning;

	static const unsigned long SERVLET_TIMEOUT = 5000;
};

#endif /* SRC_LIGHTCONTROL_TLIGHTCONTROLGETACTIONSJSONSERVLET_H_ */

#endif /* CONFIG_LIGHT_CONTROL_APP_HTTP_SERVLETS */

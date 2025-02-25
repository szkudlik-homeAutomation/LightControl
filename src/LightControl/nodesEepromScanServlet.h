#pragma once

#include "../../global.h"

#if CONFIG_NODE_SCAN_EEPROM_STATUS_TASK
#if CONFIG_LIGHT_CONTROL_APP_HTTP_SERVLETS

#include "../Common_code/Network/httpServer.h"
#include "../NodeScanEepromStatusTask.h"
#include "../Common_code/tMessageReciever.h"

class tLightControl_nodesEepromScanServlet : public tHttpServlet, public tMessageReciever

{
public:
	tLightControl_nodesEepromScanServlet() : mState(STATE_INIT), tHttpServlet()
	{
		RegisterMessageType(MessageType_WorkerTaskFinishedEvent);
	}
	virtual ~tLightControl_nodesEepromScanServlet() {}

	virtual bool ProcessAndResponse();
	void onFinished(uint8_t *mNumberOfActions);
protected:
	virtual void onMessage(uint8_t type, uint16_t data, void *pData);

private:
	static const uint8_t STATE_INIT = 0;
	static const uint8_t STATE_RUNNING = 1;
	static const uint8_t STATE_FINISHED = 2;
	uint8_t mState;
};

#endif CONFIG_NODE_SCAN_EEPROM_STATUS_TASK
#endif CONFIG_LIGHT_CONTROL_APP_HTTP_SERVLETS

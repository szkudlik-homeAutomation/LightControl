#include "../../global.h"

#if CONFIG_NODE_SCAN_EEPROM_STATUS_TASK
#if CONFIG_LIGHT_CONTROL_APP_HTTP_SERVLETS

#include "nodesEepromScanServlet.h"

bool tLightControl_nodesEepromScanServlet::ProcessAndResponse()
{
	bool result;

	switch (mState)
	{
	case STATE_INIT:
		mState = STATE_RUNNING;
		result = NodeScanWithEEpromStatusTask::trigger((uint32_t)this);
		if (result)
			{
			pOwner->SendFlashString(PSTR("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\nAccess-Control-Allow-Headers: Content-Type, Authorization\r\n\r\n{\"Devices\": {"));



			return true;
			}
		else
		{
			SendResponse503();
			return false;
		}
	case STATE_RUNNING:
		return true;
	case STATE_FINISHED:
		return false;
	}

	return false;
}

void tLightControl_nodesEepromScanServlet::onMessage(uint8_t type, uint16_t data, void *pData)
{
	if (type != MessageType_WorkerTaskFinishedEvent)
		return;
	if (data != (uint16_t *)this)
		return;

	tWorkerTaskFinishedEvent *pEvent = (tWorkerTaskFinishedEvent*) pData;
	NodeScanWithEEpromStatusTask *pTask = (NodeScanWithEEpromStatusTask *)pEvent->pTask;

	for (int i = 0; i < CONFIG_TLE8457_MAX_NUM_OF_NODES; i++) {
		if (pTask->mNumberOfActions[i] == 255)
			// node is not available
			continue;
		if (i > 0)
			pOwner->mEthernetClient.print(",");
		pOwner->mEthernetClient.print("\"");
		pOwner->mEthernetClient.print(i+1, HEX);
		pOwner->mEthernetClient.print("\":{");

	    pOwner->SendFlashString(PSTR("\"Node\":"));
	    pOwner->mEthernetClient.print(i + 1,DEC);
	    pOwner->SendFlashString(PSTR(", \"Actions\": "));
	    pOwner->mEthernetClient.print(pTask->mNumberOfActions[i]);
	    pOwner->SendFlashString(PSTR("}"));
   }
	pOwner->SendFlashString(PSTR("}}"));
	mState = STATE_FINISHED;
}


#endif CONFIG_NODE_SCAN_EEPROM_STATUS_TASK
#endif CONFIG_LIGHT_CONTROL_APP_HTTP_SERVLETS

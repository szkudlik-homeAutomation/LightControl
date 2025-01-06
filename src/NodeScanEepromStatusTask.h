#pragma once

#include "../global.h"
#if CONFIG_NODE_SCAN_EEPROM_STATUS_TASK

#include "lib/ArduinoProcessScheduler/src/ProcessScheduler.h"
#include "lib/ArduinoQueue/ArduinoQueue.h"
#include "Common_code/tWorkerProcess.h"
#include "Common_code/tMessageReciever.h"

class NodeScanWithEEpromStatusTask : public WorkerTask, public tMessageReciever
{
public:
	NodeScanWithEEpromStatusTask() : mCurrentNodeID(0)
   {
	   RegisterMessageType(MessageType_SerialFrameRecieved);
   }
   virtual ~NodeScanWithEEpromStatusTask() {}

   virtual bool Process(uint32_t * pNextServiceDelay);
   static void trigger() { tWorkerProcess::Instance->Enqueue(new NodeScanWithEEpromStatusTask()); }

protected:
   virtual void onMessage(uint8_t type, uint16_t data, void *pData);
   virtual void onFinished() {}

private:
   static const uint16_t REQUEST_SENDING_PERIOD = 600;  // 600ms
   static const uint16_t RESPONSE_WAIT_PERIOD   = 2000;  // 2s

   uint8_t mCurrentNodeID;  // if == CONFIG_TLE8457_MAX_NUM_OF_NODES - waiting for responses
   uint8_t mNumberOfActions[CONFIG_TLE8457_MAX_NUM_OF_NODES];
};

#endif // CONFIG_NODE_SCAN_EEPROM_STATUS_TASK

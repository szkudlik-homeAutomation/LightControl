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
	   mRunning = this;
    }
   virtual ~NodeScanWithEEpromStatusTask() {mRunning = NULL;}

   virtual bool Process(uint32_t * pNextServiceDelay);
   static bool trigger(uint16_t cookie) {
	   if (mRunning == NULL)
	   {
		   tWorkerProcess::Instance->Enqueue(new NodeScanWithEEpromStatusTask(), cookie);
	   } else {
		   return false;
	   }
	   return true;
   }
   uint8_t mNumberOfActions[CONFIG_TLE8457_MAX_NUM_OF_NODES];
protected:
   virtual void onMessage(uint8_t type, uint16_t data, void *pData);
   static NodeScanWithEEpromStatusTask *mRunning;


private:
   static const uint16_t REQUEST_SENDING_PERIOD = 200;  // 600ms
   static const uint16_t RESPONSE_WAIT_PERIOD   = 500;  // 2s

   uint8_t mCurrentNodeID;  // if == CONFIG_TLE8457_MAX_NUM_OF_NODES - waiting for responses
};

#endif // CONFIG_NODE_SCAN_EEPROM_STATUS_TASK

#pragma once

#include "../global.h"
#if CONFIG_CENTRAL_NODE

#include "lib/ArduinoProcessScheduler/src/ProcessScheduler.h"
#include "lib/ArduinoQueue/ArduinoQueue.h"
#include "Common_code/WorkerProcess.h"
#include "Common_code/tMessageReciever.h"
#include "Common_code/tMessages.h"

class NodeScanTask : public WorkerTask, public tMessageReciever
{
public:
   NodeScanTask() : mCurrentNodeID(0), mActiveNodesMap(0)
   {
	   RegisterMessageType(tMessageReciever::MessageType_frameRecieved);
   }
   virtual ~NodeScanTask() {}

   virtual bool Process(uint32_t * pPeriod);
   static void trigger() { Worker.Enqueue(new NodeScanTask()); }

protected:
   virtual void onMessage(uint8_t type, uint16_t data, void *pData);

private:
   static const uint16_t REQUEST_SENDING_PERIOD = 600;  // 600ms
   static const uint16_t RESPONSE_WAIT_PERIOD   = 2000;  // 2s

   uint8_t mCurrentNodeID;  // if == MAX_NUM_OF_NODES - waiting for responses
   uint32_t mActiveNodesMap;
};
#endif

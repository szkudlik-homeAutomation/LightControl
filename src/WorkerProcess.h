#pragma once

#include "../global.h"

#include "lib/ArduinoProcessScheduler/src/ProcessScheduler.h"
#include "ResponseHandler.h"
#include <ArduinoQueue.h>

class WorkerTask
{
public:
   WorkerTask() {}
   virtual ~WorkerTask() {}

   virtual bool Process(uint32_t * pPeriod) = 0;
};


class WorkerProcess: public Process, public ResponseHandler
{
  public:
  WorkerProcess(Scheduler &manager) : Process(manager,LOW_PRIORITY,SERVICE_SECONDLY,RUNTIME_FOREVER), pCurrentWorkerTask(NULL) {}

  void Enqueue(WorkerTask *pWorkerTask);

protected:
  virtual void service();
private:

  ArduinoQueue<WorkerTask *> mQueue;
  WorkerTask *pCurrentWorkerTask;

};

extern WorkerProcess Worker;

#ifdef CONTROLLER
class NodeScanTask : public WorkerTask, public ResponseHandler
{
public:
   NodeScanTask() : mCurrentNodeID(0), mActiveNodesMap(0) {}
   virtual ~NodeScanTask() {}

   virtual bool Process(uint32_t * pPeriod);
   static void trigger() { Worker.Enqueue(new NodeScanTask()); }

protected:
   virtual void vVersionResponseHandler(uint8_t DevID, uint8_t Major, uint8_t Minor, uint8_t Patch);

private:
   static const uint16_t REQUEST_SENDING_PERIOD = 600;  // 600ms
   static const uint16_t RESPONSE_WAIT_PERIOD   = 2000;  // 2s

   uint8_t mCurrentNodeID;  // if == MAX_NUM_OF_NODES - waiting for responses
   uint32_t mActiveNodesMap;
};
#endif

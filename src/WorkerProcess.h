#ifndef WORKER_PROCESS
#define WORKER_PROCESS
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

class NodeScanTask : public WorkerTask, public ResponseHandler
{
public:
   NodeScanTask() : mCurrentNodeID(0), mActiveNodesMap(0) {}
   virtual ~NodeScanTask() {}

   virtual bool Process(uint32_t * pPeriod);

protected:
   virtual void vVersionResponseHandler(uint8_t DevID, uint8_t Major, uint8_t Minor, uint8_t Patch);

private:
   static const uint16_t REQUEST_SENDING_PERIOD = 600;  // 600ms
   static const uint16_t RESPONSE_WAIT_PERIOD   = 2000;  // 2s

   uint8_t mCurrentNodeID;  // if == MAX_NUM_OF_NODES - waiting for responses
   uint32_t mActiveNodesMap;
};


class WorkerProcess: public Process, public ResponseHandler
{
  public:
  WorkerProcess(Scheduler &manager) : Process(manager,LOW_PRIORITY,SERVICE_SECONDLY,RUNTIME_FOREVER), pCurrentWorkerTask(NULL) {}

// trigger of all outgoing frames 

  bool SendMsgOverviewStateRequest(uint8_t RecieverID);
  bool SendMsgOverviewStateResponse(uint8_t RecieverID, uint8_t  PowerState, uint8_t  TimerState);
  bool SendMsgOutputStateRequest(uint8_t RecieverID, uint8_t  OutputID);
  bool SendMsgOutputStateResponse(uint8_t RecieverID, uint8_t  OutputID, uint8_t  PowerState, uint16_t TimerValue, uint16_t DefaultTimer);
  bool SendMsgSetOutput(uint8_t RecieverID, uint8_t  OutId, uint8_t  State, uint16_t Timer);
  bool SendMsgButtonPress(uint8_t RecieverID, uint8_t ForceSrcId, uint16_t ShortClick, uint16_t LongClick, uint16_t DoubleClick);
  bool SendMsgAddAction(uint8_t RecieverID, uint8_t OutId, uint8_t SenderDevID, uint8_t ButtonId, uint8_t TriggerType, uint8_t ActionType, uint16_t Timer, uint8_t  OutputsMask, uint8_t  OutputsStates);
  bool SendMsgClearAllActions(uint8_t RecieverID);
  bool SendMsgVersionRequest(uint8_t RecieverID);
  bool SendMsgVersionResponse(uint8_t RecieverID, uint8_t Major, uint8_t Minor, uint8_t Patch);
  bool SendMsgEepromCrcRequest(uint8_t RecieverID);
  bool SendMsgEepromCrcResponse(uint8_t RecieverID,  uint8_t NumOfActions, uint16_t EepromCRC);
  bool SendMsgReset(uint8_t RecieverID);
  bool SendMsgSetDefaultTimer(uint8_t RecieverID, uint8_t OutputID, uint16_t DefTimerValue);
  bool SendMsgDefaultTimerRequest(uint8_t RecieverID,uint8_t OutputID);
  bool SendMsgDefaultTimerResponse(uint8_t RecieverID, uint8_t OutputID, uint16_t DefTimerValue);

//triggers of complex actions
  bool triggerNodesScan();
  
protected:
  virtual void service();
private:

  ArduinoQueue<WorkerTask *> mQueue;
  WorkerTask *pCurrentWorkerTask;

  void Enqueue(WorkerTask *pWorkerTask);
};

extern WorkerProcess Worker;

#endif  // WORKER_PROCESS

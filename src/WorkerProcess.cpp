#include "../global.h"

#include "WorkerProcess.h"
#include "OutgoingMessage.h"
#include "tOutputProcess_lightControl.h"
#include "ResponseHandler.h"
#include "Common_code/TLE8457_serial/TLE8457_serial_lib.h"

bool NodeScanTask::Process(uint32_t * pPeriod)
{
   *pPeriod = REQUEST_SENDING_PERIOD;

   if (mCurrentNodeID < MAX_NUM_OF_NODES)
   {
       // send a frame
       mCurrentNodeID++;
       OutgoingMessage::SendMsgVersionRequest(mCurrentNodeID);  // staring from 1
   }
   else if (mCurrentNodeID == MAX_NUM_OF_NODES)
   {
      *pPeriod = RESPONSE_WAIT_PERIOD;
       mCurrentNodeID++;
   }
   else
   {
       // send result
       ResponseHandler::NodeScanResponse(mActiveNodesMap);
       return false;
   }

   return true;
}

void NodeScanTask::vVersionResponseHandler(uint8_t DevID, uint8_t Major, uint8_t Minor, uint8_t Patch)
{
   mActiveNodesMap |= 1 << (DevID-1);
}

bool WorkerProcess::triggerNodesScan()
{
#ifdef CONTROLLER
   Enqueue(new NodeScanTask());
#endif
  return true;
}

void WorkerProcess::service()
{
  #ifdef CONTROLLER
   if (NULL == pCurrentWorkerTask)
   {
      if (mQueue.isEmpty())
      {
         // go to sleep
         disable();
         return;
      }

      pCurrentWorkerTask = mQueue.dequeue();
   }

   uint32_t nextPeriod;
   bool continueTask;
   continueTask = pCurrentWorkerTask->Process(&nextPeriod);
   if (continueTask)
   {
      // continue after requested time
      setPeriod(nextPeriod);
      return;
   }

   // task finished
   delete (pCurrentWorkerTask);
   pCurrentWorkerTask = NULL;
   setPeriod(SERVICE_CONSTANTLY);   // next iteration will go for next queue item or disable the task if the queue is empty
#else
   disable();
#endif
}

void WorkerProcess::Enqueue(WorkerTask *pWorkerTask)
{
   mQueue.enqueue(pWorkerTask);

   if (! isEnabled())
   {
     setPeriod(SERVICE_CONSTANTLY);
     setIterations(RUNTIME_FOREVER);
     enable();
   }
}

#include "../global.h"

#include "OutgoingMessage.h"
#include "tOutputProcess_lightControl.h"
#include "ResponseHandler.h"
#include "Common_code/TLE8457_serial/TLE8457_serial_lib.h"
#include "NodeScanTask.h"

#ifdef CONTROLLER
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

#endif //CONTROLLER


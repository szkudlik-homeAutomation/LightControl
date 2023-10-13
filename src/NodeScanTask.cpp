#include "../global.h"

#include "tLightControlOutgoingFrames.h"
#include "tOutputProcess_lightControl.h"
#include "Common_code/TLE8457_serial/TLE8457_serial_lib.h"
#include "NodeScanTask.h"
#include "tLightControlMessages.h"

#if CONFIG_CENTRAL_NODE
bool NodeScanTask::Process(uint32_t * pPeriod)
{
   *pPeriod = REQUEST_SENDING_PERIOD;

   if (mCurrentNodeID < MAX_NUM_OF_NODES)
   {
       // send a frame
       mCurrentNodeID++;
       tLightControlOutgoingFrames::SendMsgVersionRequest(mCurrentNodeID);  // staring from 1
   }
   else if (mCurrentNodeID == MAX_NUM_OF_NODES)
   {
      *pPeriod = RESPONSE_WAIT_PERIOD;
       mCurrentNodeID++;
   }
   else
   {
       // send result
       tLightControlMessages::NodeScanResponse(mActiveNodesMap);
       return false;
   }

   return true;
}

void NodeScanTask::onMessage(uint8_t type, uint16_t data, void *pData)
{
	if (data != tLightControlMessages::frameRecieved_VersionResponse)
		return;

	struct tLightControlMessages::tVersionResponse *pVersionResponse = (struct tLightControlMessages::tVersionResponse *)pData;
	mActiveNodesMap |= 1 << (pVersionResponse->SenderID - 1);
}

#endif //CENTRAL_NODE


#include "../global.h"

#include "tLightControlOutgoingMessages.h"
#include "tOutputProcess_lightControl.h"
#include "Common_code/TLE8457_serial/TLE8457_serial_lib.h"
#include "NodeScanTask.h"
#include "LightControlMessages.h"

#if CONFIG_CENTRAL_NODE
bool NodeScanTask::Process(uint32_t * pPeriod)
{
   *pPeriod = REQUEST_SENDING_PERIOD;

   if (mCurrentNodeID < MAX_NUM_OF_NODES)
   {
       // send a frame
       mCurrentNodeID++;
       tLightControlOutgoingMessages::SendMsgVersionRequest(mCurrentNodeID);  // staring from 1
   }
   else if (mCurrentNodeID == MAX_NUM_OF_NODES)
   {
      *pPeriod = RESPONSE_WAIT_PERIOD;
       mCurrentNodeID++;
   }
   else
   {
       // send result
       LightControlMessages::NodeScanResponse(mActiveNodesMap);
       return false;
   }

   return true;
}

void NodeScanTask::onMessage(uint8_t type, uint16_t data, void *pData)
{
	if (data != LightControlMessages::frameRecieved_VersionResponse)
		return;

	struct LightControlMessages::tVersionResponse *pVersionResponse = (struct LightControlMessages::tVersionResponse *)pData;
	mActiveNodesMap |= 1 << (pVersionResponse->SenderID - 1);
}

#endif //CENTRAL_NODE


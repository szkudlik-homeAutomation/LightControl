#include "../global.h"

#include "OutgoingMessage.h"
#include "tOutputProcess_lightControl.h"
#include "Common_code/TLE8457_serial/TLE8457_serial_lib.h"
#include "NodeScanTask.h"

#if CONFIG_CENTRAL_NODE
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
       Message::NodeScanResponse(mActiveNodesMap);
       return false;
   }

   return true;
}

void NodeScanTask::onMessage(uint8_t type, void *data)
{
	if (type != Message::VersionResponseType)
		return;

	struct Message::tVersionResponse *pVersionResponse = (struct Message::tVersionResponse *)data;
	mActiveNodesMap |= 1 << (pVersionResponse->SenderID - 1);
}

#endif //CENTRAL_NODE


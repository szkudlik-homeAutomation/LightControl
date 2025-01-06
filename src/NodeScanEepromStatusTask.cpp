#include "../global.h"

#if CONFIG_NODE_SCAN_EEPROM_STATUS_TASK

#include "NodeScanEepromStatusTask.h"

#include "Common_code/TLE8457_Serial/TLE8457_serial_lib.h"
#include "LightControl/tLightControlOutgoingFrames.h"

#if CONFIG_TELNET_SERVER
#include "lib/Commander/src/Commander.h"
bool send_ScanEeprom(Commander &Cmdr)
{
	NodeScanWithEEpromStatusTask::trigger();
}
#endif // CONFIG_TELNET_SERVER

bool NodeScanWithEEpromStatusTask::Process(uint32_t *pNextServiceDelay)
{
   *pNextServiceDelay = REQUEST_SENDING_PERIOD;

   if (mCurrentNodeID < CONFIG_TLE8457_MAX_NUM_OF_NODES)
   {
       // send a frame, staring from 1
       mCurrentNodeID++;
       mNumberOfActions[mCurrentNodeID-1] = 255;	// set as invalid
       tLightControlOutgoingFrames::SendMsgEepromCrcRequest(mCurrentNodeID);
   }
   else if (mCurrentNodeID == CONFIG_TLE8457_MAX_NUM_OF_NODES)
   {
      *pNextServiceDelay = RESPONSE_WAIT_PERIOD;
       mCurrentNodeID++;
   }
   else
   {
       // send result
		for (int i = 0; i < CONFIG_TLE8457_MAX_NUM_OF_NODES; i++) {
			if (mNumberOfActions[i] == 255)
				continue;

			LOG_PRINT("Node ");
			LOG(print(i + 1,HEX))
			LOG_PRINT(" has ");
			LOG(print(mNumberOfActions[i]));
			LOG_PRINTLN(" actions");
	   }
	   onFinished();
       return false;
   }

   return true;
}

void NodeScanWithEEpromStatusTask::onMessage(uint8_t type, uint16_t data, void *pData)
{
	if (type != MessageType_SerialFrameRecieved || data != MESSAGE_TYPE_EEPROM_CRC_RESPONSE)
		return;

    tCommunicationFrame *pFrame = (tCommunicationFrame *)pData;
	if (pFrame->SenderDevId > CONFIG_TLE8457_MAX_NUM_OF_NODES)	// count from 1
		return;

    tMessageTypeEepromCRCResponse *pMsg = (tMessageTypeEepromCRCResponse *)pFrame->Data;
    mNumberOfActions[pFrame->SenderDevId-1] = pMsg->NumOfActions;
}

#endif //CONFIG_NODE_SCAN_EEPROM_STATUS_TASK

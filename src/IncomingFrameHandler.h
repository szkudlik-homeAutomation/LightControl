/*
 * IncomingFrameHandler.h
 *
 *  Created on: 10 sie 2022
 *      Author: szkud
 */

#ifndef SRC_INCOMINGFRAMEHANDLER_H_
#define SRC_INCOMINGFRAMEHANDLER_H_

#include "../global.h"
#include "Common_code/TLE8457_serial/TLE8457_serial_lib.h"

class IncomingFrameHandler: public CommRecieverProcessCallback {
public:
   IncomingFrameHandler() {};

   virtual void onFrame(void *pData, uint8_t MessageType, uint8_t SenderDevId);

private:
   // handlers of all incoming frames

   void HandleMsgOverviewStateRequest(uint8_t SenderID);
   void HandleMsgOverviewStateResponse(uint8_t SenderID, tMessageTypeOverviewStateResponse* Message);
   void HandleMsgOutputStateRequest(uint8_t SenderID, tMessageTypeOutputStateRequest* Message);
   void HandleMsgOutputStateResponse(uint8_t SenderID, tMessageTypeOutputStateResponse* Message);
   void HandleMsgSetOutput(uint8_t SenderID, tMessageTypeSetOutput* Message);
   void HandleMsgButtonPress(uint8_t SenderID, tMessageTypeButtonPress *Message);
   void HandleMsgSetAction(uint8_t SenderID, tMessageTypeSetAction* Message);
   void HandleMsgClearAllActions(uint8_t SenderID);
   void HandleMsgVersionRequest(uint8_t SenderID);
   void HandleMsgVersionResponse(uint8_t SenderID, tMessageTypeFwVesionResponse *Message);
   void HandleMsgEepromCrcRequest(uint8_t SenderID);
   void HandleMsgEepromCrcResponse(uint8_t SenderID, tMessageTypeEepromCRCResponse* Message);
   void HandleMsgSetDefaultTimer(uint8_t SenderID, tMessageTypeSetDefaultTimer *Message);
   void HandleMsgDefaultTimerRequest(uint8_t SenderID, tMessageTypeDefaultTimerRequest *Message);
   void HandleMsgDefaultTimerResponse(uint8_t SenderID, tMessageTypeDefaultTimerResponse *Message);

};


#endif /* SRC_INCOMINGFRAMEHANDLER_H_ */

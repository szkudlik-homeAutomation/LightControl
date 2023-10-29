/*
 * IncomingFrameHandler.h
 *
 *  Created on: 10 sie 2022
 *      Author: szkud
 */

#pragma once

#include "../global.h"
#include "Common_code/TLE8457_serial/tIncomingFrameHanlder.h"

class tLightControlIncomingFrameHandler: public tIncomingFrameHanlder {
public:
   tLightControlIncomingFrameHandler() { RegisterMessageType(tMessageReciever::MessageType_frameRecieved); }

   virtual void onMessage(uint8_t type, uint16_t data, void *pData);

private:
   // handlers of all incoming frames

   void HandleMsgButtonPress(uint8_t SenderID, tMessageTypeButtonPress *Message);
   void HandleMsgSetAction(uint8_t SenderID, tMessageTypeSetAction* Message);
   void HandleMsgClearAllActions(uint8_t SenderID);
   void HandleMsgEepromCrcRequest(uint8_t SenderID);
   void HandleMsgEepromCrcResponse(uint8_t SenderID, tMessageTypeEepromCRCResponse* Message);
   void HandleMsgSetDefaultTimer(uint8_t SenderID, tMessageTypeSetDefaultTimer *Message);
   void HandleMsgDefaultTimerRequest(uint8_t SenderID, tMessageTypeDefaultTimerRequest *Message);
   void HandleMsgDefaultTimerResponse(uint8_t SenderID, tMessageTypeDefaultTimerResponse *Message);

};

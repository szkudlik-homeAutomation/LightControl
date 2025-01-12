/*
 * IncomingFrameHandler.h
 *
 *  Created on: 10 sie 2022
 *      Author: szkud
 */

#pragma once

#include "../../global.h"

#if CONFIG_LIGHT_CONTROL_APP
#include "../Common_code/tMessageReciever.h"

class tLightControl : public tMessageReciever {
public:
   tLightControl() : tMessageReciever() {
	   RegisterMessageType(MessageType_SerialFrameRecieved);
	   RegisterMessageType(MessageType_DigialInputEvent);
   }

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
   void HandleMsgGetActionsRequest(uint8_t SenderID);
   void LogMsgGetActionsResponse(uint8_t SenderID, tMessageTypeGetActionsResponse *Message);
};

#endif CONFIG_LIGHT_CONTROL_APP

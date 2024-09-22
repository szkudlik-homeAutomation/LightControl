/*
 * tLightControlOutgoingFrames.h
 *
 *  Created on: Oct 10, 2023
 *      Author: mszkudli
 */

#pragma once

#include "../../global.h"
#if CONFIG_LIGHT_CONTROL_APP

#include "../Common_code/TLE8457_serial/tOutgoingFrames.h"

class tLightControlOutgoingFrames : public tOutgoingFrames {
public:
      static bool SendMsgButtonPress(uint8_t RecieverID, uint8_t ForceSrcId, uint16_t ShortClick, uint16_t LongClick, uint16_t DoubleClick);
      static bool SendMsgAddAction(uint8_t RecieverID, uint8_t OutId, uint8_t SenderDevID, uint8_t ButtonId, uint8_t TriggerType, uint8_t ActionType, uint16_t Timer, uint8_t  OutputsMask, uint8_t  OutputsStates);
      static bool SendMsgClearAllActions(uint8_t RecieverID);
      static bool SendMsgEepromCrcRequest(uint8_t RecieverID);
      static bool SendMsgEepromCrcResponse(uint8_t RecieverID,  uint8_t NumOfActions, uint16_t EepromCRC);
      static bool SendMsgSetDefaultTimer(uint8_t RecieverID, uint8_t OutputID, uint16_t DefTimerValue);
      static bool SendMsgDefaultTimerRequest(uint8_t RecieverID,uint8_t OutputID);
      static bool SendMsgDefaultTimerResponse(uint8_t RecieverID, uint8_t OutputID, uint16_t DefTimerValue);
};
#endif // CONFIG_LIGHT_CONTROL_APP


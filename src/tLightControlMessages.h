/*
 * Message.h
 *
 *  Created on: Feb 28, 2023
 *      Author: mszkudli
 */

#pragma once

/*
 * message handler specific to the application
 * uses tMessageReciever to dispatch messages
 */
#include "../global.h"
#include "Common_code/tMessages.h"

class tLightControlMessages : public tMessages {
public:
	  /* other incoming frames - currently log response only, no messages dispatch */
	  static void EepromCRCResponseHandler(uint8_t SenderID, uint8_t NumOfActions, uint16_t EepromCRC);
	  static void NodeScanResponse(uint32_t ActiveNodesMap);
	  static void DefaultTimerResponseHandler(uint8_t SenderID,uint8_t OutputID,uint16_t DefTimerValue);

};

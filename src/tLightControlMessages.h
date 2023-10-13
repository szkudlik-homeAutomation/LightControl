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
	  /*
	   * log and dispatch MessageType_frameRecieved::frameRecieved_OutputStateResponseouput state response through tMessageReciever
	   */
	  static const uint8_t frameRecieved_OutputStateResponse = 128;
	  typedef struct tOutputStateResponse
	  {
		  uint8_t SenderID;
		  uint8_t OutputID;
		  uint8_t PowerState;
		  uint16_t TimerValue;
		  uint16_t DefaultTimer;
	  };
	  static void OutputStateResponseHandler(uint8_t SenderID, uint8_t OutputID, uint8_t PowerState, uint16_t  TimerValue, uint16_t DefaultTimer);
	  static void OutputStateResponseHandler(struct tOutputStateResponse* pOutputStateResponse);

	  /* other incoming frames - currently log response only, no messages dispatch */
	  static void OverviewStateResponseHandler(uint8_t SenderID, uint8_t PowerState, uint8_t  TimerState);
	  static void EepromCRCResponseHandler(uint8_t SenderID, uint8_t NumOfActions, uint16_t EepromCRC);
	  static void NodeScanResponse(uint32_t ActiveNodesMap);
	  static void DefaultTimerResponseHandler(uint8_t SenderID,uint8_t OutputID,uint16_t DefTimerValue);

};

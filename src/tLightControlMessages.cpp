/*
 * Message.cpp
 *
 *  Created on: Feb 28, 2023
 *      Author: mszkudli
 */
#include "../global.h"
#include "Common_code/tMessageReciever.h"
#include "tLightControlMessages.h"

static void tLightControlMessages::OutputStateResponseHandler(uint8_t SenderID, uint8_t OutputID, uint8_t PowerState, uint16_t  TimerValue, uint16_t DefaultTimer)
{
	tOutputStateResponse OutputStateResponse;
	OutputStateResponse.SenderID = SenderID;
	OutputStateResponse.OutputID = OutputID;
	OutputStateResponse.PowerState = PowerState;
	OutputStateResponse.TimerValue = TimerValue;
	OutputStateResponse.DefaultTimer = DefaultTimer;
	OutputStateResponseHandler(&OutputStateResponse);
}

static void tLightControlMessages::OutputStateResponseHandler(struct tOutputStateResponse* pOutputStateResponse)
{
	LOG_PRINT("PowerState for device ");
	LOG(print(pOutputStateResponse->SenderID,HEX));
	LOG_PRINT(" output ID ");
	LOG(print(pOutputStateResponse->OutputID,DEC));
	LOG_PRINT("=");
	LOG(print(pOutputStateResponse->PowerState,DEC));
	LOG_PRINT(" with timers = ");
	LOG(print(pOutputStateResponse->TimerValue,DEC));
    LOG_PRINT(" default timer = ");
    LOG(println(pOutputStateResponse->DefaultTimer,DEC));

    tMessageReciever::Dispatch(tMessageReciever::MessageType_frameRecieved,frameRecieved_OutputStateResponse,pOutputStateResponse);
}


void tLightControlMessages::OverviewStateResponseHandler(uint8_t SenderID, uint8_t PowerState, uint8_t  TimerState)
{
	LOG_PRINT("PowerStateBitmap for device ");
	LOG(print(SenderID,HEX));
	LOG_PRINT("=");
	LOG(print(PowerState,BIN));
	LOG_PRINT(" with timers map=");
	LOG(println(TimerState,BIN));
}

void tLightControlMessages::EepromCRCResponseHandler(uint8_t SenderID, uint8_t NumOfActions, uint16_t EepromCRC)
{
	LOG_PRINT("Eeprom CRC for device ");
	LOG(print(SenderID,HEX));
	LOG_PRINT(" num of actions=");
	LOG(print(NumOfActions,DEC));
	LOG_PRINT(" CRC=");
	LOG(println(EepromCRC,DEC));
}

void tLightControlMessages::NodeScanResponse(uint32_t ActiveNodesMap)
{
	LOG_PRINT("Active node map: ");
	LOG(println(ActiveNodesMap,BIN));
}

void tLightControlMessages::DefaultTimerResponseHandler(uint8_t SenderID,uint8_t OutputID,uint16_t DefTimerValue)
{
	LOG_PRINT("Default timer for device ");
	LOG(print(SenderID,HEX));
	LOG_PRINT(" outId ");
	LOG(print(OutputID,DEC));
	LOG_PRINT("=");
	LOG(println(DefTimerValue,DEC));
}

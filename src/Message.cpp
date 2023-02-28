/*
 * Message.cpp
 *
 *  Created on: Feb 28, 2023
 *      Author: mszkudli
 */


#include "../global.h"


#include "Message.h"
#include "Common_code/tMessageReciever.h"

void Message::VersionResponseHandler(uint8_t SenderID, uint8_t Major, uint8_t Minor, uint8_t Patch)
{
	tVersionResponse  VersionResponse;

	VersionResponse.SenderID = SenderID;
	VersionResponse.Major = Major;
	VersionResponse.Minor = Minor;
	VersionResponse.Patch = Patch;
	VersionResponseHandler(&VersionResponse);
}

void Message::VersionResponseHandler(struct tVersionResponse *pVersionResponse)
{
	LOG_PRINT("FW Version for device ");
	LOG(print(pVersionResponse->SenderID,HEX));
	LOG_PRINT("=");
	LOG(print(pVersionResponse->Major,DEC));
	LOG_PRINT(".");
	LOG(print(pVersionResponse->Minor,DEC));
	LOG_PRINT(".");
	LOG(println(pVersionResponse->Patch,DEC));

	tMessageReciever::Dispatch(VersionResponseType,pVersionResponse);
}

static void Message::OutputStateResponseHandler(uint8_t SenderID, uint8_t OutputID, uint8_t PowerState, uint16_t  TimerValue, uint16_t DefaultTimer)
{
	tOutputStateResponse OutputStateResponse;
	OutputStateResponse.SenderID = SenderID;
	OutputStateResponse.OutputID = OutputID;
	OutputStateResponse.PowerState = PowerState;
	OutputStateResponse.TimerValue = TimerValue;
	OutputStateResponse.DefaultTimer = DefaultTimer;
	OutputStateResponseHandler(&OutputStateResponse);
}

static void Message::OutputStateResponseHandler(struct tOutputStateResponse* pOutputStateResponse)
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

    tMessageReciever::Dispatch(OutputStateResponseType,pOutputStateResponse);
}


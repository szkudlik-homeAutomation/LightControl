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


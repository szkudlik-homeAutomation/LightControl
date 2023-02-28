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

class Message {
public:
	  static const uint8_t VersionResponseType = 0;
	  typedef struct tVersionResponse
	  {
		  uint8_t SenderID;
		  uint8_t Major;
		  uint8_t Minor;
		  uint8_t Patch;
	  };
	  static void VersionResponseHandler(uint8_t SenderID, uint8_t Major, uint8_t Minor, uint8_t Patch);
	  static void VersionResponseHandler(struct tVersionResponse *pVersionResponse);

};

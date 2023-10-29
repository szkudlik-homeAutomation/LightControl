/*
 * Message.cpp
 *
 *  Created on: Feb 28, 2023
 *      Author: mszkudli
 */
#include "../global.h"
#include "Common_code/tMessageReciever.h"
#include "tLightControlMessages.h"

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

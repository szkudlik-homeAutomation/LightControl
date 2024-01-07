/*
 * tKeyRecieverMessageLog.cpp
 *
 *  Created on: Jan 5, 2024
 *      Author: mszkudli
 */

#include "../global.h"
#include "tKeyRecieverMessageLog.h"
#include "Common_code/TLE8457_serial/TLE8457_serial_lib.h"

void tKeyRecieverMessageLog::onMessage(uint8_t type, uint16_t data, void *pData)
{
    if (type != MessageType_SerialFrameRecieved)
        return;

    tCommunicationFrame *pFrame = (tCommunicationFrame *)pData;
    uint8_t SenderDevId = pFrame->SenderDevId;

    switch (data)   // messageType
    {
    case MESSAGE_TYPE_CODE_RECIEVED:
        HandleMsgCodeRecieved((tMessageTypeCodeRecieved*)(pFrame->Data));
        break;
    }
}

void tKeyRecieverMessageLog::HandleMsgCodeRecieved(tMessageTypeCodeRecieved *Msg)
{
    LOG_PRINT(" Code received, size: ");
    LOG(print(Msg->size, DEC));
    LOG_PRINT(" value: ");
    LOG(print(Msg->code, DEC));
    LOG_PRINT(" 0x");
    LOG(println(Msg->code, HEX));
}

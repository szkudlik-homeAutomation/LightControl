/*
 * tKeyRecieverMessageLog.h
 *
 *  Created on: Jan 5, 2024
 *      Author: mszkudli
 */

#pragma once

#include "../global.h"
#include "Common_code/tMessageReciever.h"

class tKeyRecieverMessageLog : public tMessageReciever
{
public:
    tKeyRecieverMessageLog() : tMessageReciever()
    {
        RegisterMessageType(MessageType_SerialFrameRecieved);
    }
protected:
    virtual void onMessage(uint8_t type, uint16_t data, void *pData);

private:
    void HandleMsgCodeRecieved(tMessageTypeCodeRecieved *Msg);
};

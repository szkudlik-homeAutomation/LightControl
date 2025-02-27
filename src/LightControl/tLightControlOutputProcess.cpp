/*
 * tLightControlOutputProcess.cpp
 *
 *  Created on: 27 lis 2023
 *      Author: szkud
 */

#include "../../global.h"

#if CONFIG_LIGHT_CONTROL_APP
#if CONFIG_OUTPUT_PROCESS

#include "tLightControlOutputProcess.h"
#include "../Common_code/TLE8457_serial/tOutgoingFrames.h"
#include "../Common_code/TLE8457_serial/TLE8457_serial_lib.h"

void tLightControlOutputProcess::onMessage(uint8_t type, uint16_t data, void *pData)
{
    if (type != MessageType_SerialFrameRecieved)
    {
    	tOutputProcess::onMessage(type, data, pData);
        return;
    }

    tCommunicationFrame *pFrame = (tCommunicationFrame *)pData;

    /* override default handling of messages - handler default timer */
    switch (data)   // messageType
    {
    case MESSAGE_TYPE_OUTPUT_STATE_REQUEST:
        {
            uint16_t DefTimer;
            tMessageTypeOutputStateRequest* Message = (tMessageTypeOutputStateRequest*)(pFrame->Data);
            if (Message->OutputID < CONFIG_OUTPUT_PROCESS_NUM_OF_PINS)
            {
                EEPROM.get(EEPROM_DEFAULT_TIMER_VALUE_OFFSET+Message->OutputID*(sizeof(uint16_t)),DefTimer);
                tOutgoingFrames::SendMsgOutputStateResponse(pFrame->SenderDevId,Message->OutputID,GetOutputState(Message->OutputID), GetOutputTimer(Message->OutputID),DefTimer);
            }
        }
        break;

    case MESSAGE_TYPE_SET_OUTPUT:
        {
            tMessageTypeSetOutput* Message = (tMessageTypeSetOutput*)(pFrame->Data);
            if (Message->OutId < CONFIG_OUTPUT_PROCESS_NUM_OF_PINS)
            {
               uint16_t Timer = Message->Timer;

               if (DEFAULT_TIMER == Timer)
               {
                 EEPROM.get(EEPROM_DEFAULT_TIMER_VALUE_OFFSET+Message->OutId*(sizeof(uint16_t)),Timer);
               }

               SetOutput(Message->OutId,Message->State,Timer,ForceTimer);
            }
        }
        break;

    default:
    	tOutputProcess::onMessage(type, data, pData);
    }
}

#endif CONFIG_OUTPUT_PROCESS
#endif CONFIG_LIGHT_CONTROL_APP

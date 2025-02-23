/*
 * tLightControlgetActionsJSONservlet.cpp
 *
 *  Created on: 11 sty 2025
 *      Author: szkud
 */
#include "../../global.h"


#if CONFIG_LIGHT_CONTROL_APP_HTTP_SERVLETS

#include "tLightControlgetActionsJSONservlet.h"
#include "../Common_code/TLE8457_serial/TLE8457_serial_lib.h"
#include "tLightControlOutgoingFrames.h"

bool tLightControl_getActionsJSON_servlet::isRunning = false;

void tLightControl_getActionsJSON_servlet::onMessage(uint8_t type, uint16_t data, void *pData)
{
	if (type != MessageType_SerialFrameRecieved || data != MESSAGE_TYPE_GET_ACTIONS_RESPONSE)
		return;

	tCommunicationFrame *pFrame = (tCommunicationFrame *)pData;
	tMessageTypeGetActionsResponse *pAction = (tMessageTypeGetActionsResponse *)pFrame->Data;

	if (mState == STATE_NO_DATA_RECIEVED)
	{
		pOwner->SendFlashString(PSTR("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nAccess-Control-Allow-Origin: *\r\nAccess-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\nAccess-Control-Allow-Headers: Content-Type, Authorization\r\n\r\n{\"DevID\": "));
		pOwner->mEthernetClient.print(mDeviceId, DEC);
		pOwner->SendFlashString(PSTR(", \"Actions\":{"));
		mState = STATE_RUNNING;
	}

	if (pAction->OutId == 255)
	{
		mState = STATE_FINISHED;
		return;
	}

	pOwner->SendFlashString(PSTR("\""));
	pOwner->mEthernetClient.print(mNumOfActons, DEC);
	pOwner->SendFlashString(PSTR("\":"));
	tMessageTypeSetActionToJSON(pAction);
	pOwner->SendFlashString(PSTR(","));
	mNumOfActons++;
}


bool tLightControl_getActionsJSON_servlet::ProcessAndResponse()
 {
	bool result;


	switch (mState)
	{
	case STATE_INIT:
		mState = STATE_NO_DATA_RECIEVED;
		if (!isRunning)
			{
			bool ParametersOK = true;
			uint16_t Device;  // device iD

			ParametersOK &= GetParameter("Dev",&Device);
			if (! ParametersOK)
			{
			  SendResponse400();
			  return false;
			}
			mDeviceId = Device;

			isRunning = true;
			mStartTimestamp = millis();
			tLightControlOutgoingFrames::SendMsgGetActionsRequest(mDeviceId);
			return true;
			}
		else
		{
			SendResponse503();
			return false;
		}
	case STATE_NO_DATA_RECIEVED:
		if ((millis() - mStartTimestamp) > SERVLET_TIMEOUT) {
			SendResponse424();
    		return false;
		}
		return true;

	case STATE_RUNNING:
		if ((millis() - mStartTimestamp) > SERVLET_TIMEOUT) {
			mState = STATE_FINISHED;
		}
		return true;

	case STATE_FINISHED:
	    pOwner->SendFlashString(PSTR("\"NumOfActions\": "));
	    pOwner->mEthernetClient.print(mNumOfActons, DEC);
	    pOwner->SendFlashString(PSTR("}}"));
		return false;
	}

	return false;
}

void tLightControl_getActionsJSON_servlet::tMessageTypeSetActionToJSON(tMessageTypeSetAction *pAction)
{
    pOwner->SendFlashString(PSTR("{"));
    pOwner->SendFlashString(PSTR("\"OutId\": "));
    pOwner->mEthernetClient.print(pAction->OutId, DEC);
    pOwner->SendFlashString(PSTR(", \"SenderDevID\": "));
    pOwner->mEthernetClient.print(pAction->SenderDevID, DEC);
    pOwner->SendFlashString(PSTR(", \"ButtonId\": "));
    pOwner->mEthernetClient.print(pAction->ButtonId, DEC);
    pOwner->SendFlashString(PSTR(", \"TriggerType\": "));
    pOwner->mEthernetClient.print(pAction->TriggerType, DEC);
    pOwner->SendFlashString(PSTR(", \"ActionType\": "));
    pOwner->mEthernetClient.print(pAction->ActionType, DEC);
    pOwner->SendFlashString(PSTR(", \"Timer\": "));
    pOwner->mEthernetClient.print(pAction->Timer, DEC);
    pOwner->SendFlashString(PSTR(", \"OutputsMask\": "));
    pOwner->mEthernetClient.print(pAction->OutputsMask, DEC);
    pOwner->SendFlashString(PSTR(", \"OutputsStates\": "));
    pOwner->mEthernetClient.print(pAction->OutputsStates, DEC);
    pOwner->SendFlashString(PSTR("}"));
}

#endif /* CONFIG_LIGHT_CONTROL_APP_HTTP_SERVLETS */

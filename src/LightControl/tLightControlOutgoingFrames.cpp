/*
 * tLightControltLightControlOutgoingFramess.cpp
 *
 *  Created on: Oct 10, 2023
 *      Author: mszkudli
 */

#include "../../global.h"
#if CONFIG_LIGHT_CONTROL_APP

#include "tLightControlOutgoingFrames.h"
#include "../Common_code/TLE8457_serial/TLE8457_serial_lib.h"

// BUTTON  PRESS
bool tLightControlOutgoingFrames::SendMsgButtonPress(uint8_t RecieverID, uint8_t ForceSrcId, uint16_t ShortClick, uint16_t LongClick, uint16_t DoubleClick)
{
  tMessageTypeButtonPress Msg;
  Msg.ShortClick = ShortClick;
  Msg.LongClick  = LongClick;
  Msg.DoubleClick = DoubleClick;
  Msg.ForceSrcId = ForceSrcId;

  DEBUG_PRINTLN_3("===================>sending MESSAGE_BUTTON_PRESS");
  CommSenderProcess::Instance->Enqueue(RecieverID,MESSAGE_BUTTON_PRESS,sizeof(Msg),&Msg);
  return true;
};

bool tLightControlOutgoingFrames::SendMsgClearAllActions(uint8_t RecieverID)
{
#if CONFIG_LIGHT_CONTROL_CENTRAL_NODE
#if CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS
  DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_CLEAR_ACTIONS");
#endif CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS
  CommSenderProcess::Instance->Enqueue(RecieverID,MESSAGE_TYPE_CLEAR_ACTIONS,0,NULL);
#endif LIGHT_CONTROL_CENTRAL_NODE
  return true;
};

bool tLightControlOutgoingFrames::SendMsgAddAction(uint8_t RecieverID, uint8_t OutId, uint8_t SenderDevID, uint8_t ButtonId, uint8_t TriggerType, uint8_t ActionType, uint16_t Timer, uint8_t  OutputsMask, uint8_t  OutputsStates)
{
#if CONFIG_LIGHT_CONTROL_CENTRAL_NODE
  tMessageTypeSetAction Message;
  Message.OutId = OutId;
  Message.SenderDevID = SenderDevID;
  Message.ButtonId = ButtonId;
  Message.TriggerType = TriggerType;
  Message.ActionType = ActionType;
  Message.Timer = Timer;
  Message.OutputsMask = OutputsMask;
  Message.OutputsStates = OutputsStates;

#if CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS
  DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_SET_ACTION");
#endif CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS
  CommSenderProcess::Instance->Enqueue(RecieverID,MESSAGE_TYPE_SET_ACTION,sizeof(Message),&Message);
#endif
  return true;
};

// EEPROM CRC Handshake
bool tLightControlOutgoingFrames::SendMsgEepromCrcRequest(uint8_t RecieverID)
{
#if CONFIG_LIGHT_CONTROL_CENTRAL_NODE
#if CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS
  DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_EEPROM_CRC_REQUEST");
#endif CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS
  CommSenderProcess::Instance->Enqueue(RecieverID,MESSAGE_TYPE_EEPROM_CRC_REQUEST,0,NULL);
#endif
  return true;
}

bool tLightControlOutgoingFrames::SendMsgEepromCrcResponse(uint8_t RecieverID,  uint8_t NumOfActions, uint16_t EepromCRC)
{
  tMessageTypeEepromCRCResponse Msg;
  Msg.NumOfActions = NumOfActions;
  Msg.EepromCRC = EepromCRC;
#if CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS
  DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_EEPROM_CRC_RESPONSE");
#endif CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS
  CommSenderProcess::Instance->Enqueue(RecieverID,MESSAGE_TYPE_EEPROM_CRC_RESPONSE,sizeof(Msg),&Msg);
  return true;
};


bool tLightControlOutgoingFrames::SendMsgSetDefaultTimer(uint8_t RecieverID, uint8_t OutputID, uint16_t DefTimerValue)
{
  tMessageTypeSetDefaultTimer Msg;
#if CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS
  DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_SET_DEFAULT_TIMER");
#endif CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS
  Msg.OutputID = OutputID;
  Msg.DefaultTimerValue = DefTimerValue;
  CommSenderProcess::Instance->Enqueue(RecieverID,MESSAGE_TYPE_SET_DEFAULT_TIMER,sizeof(Msg),&Msg);
}

bool tLightControlOutgoingFrames::SendMsgDefaultTimerRequest(uint8_t RecieverID, uint8_t OutputID)
{
  tMessageTypeDefaultTimerRequest Msg;
#if CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS
  DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_DEFAULT_TIMER_REQUEST");
#endif CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS
  Msg.OutputID = OutputID;
  CommSenderProcess::Instance->Enqueue(RecieverID,MESSAGE_TYPE_DEFAULT_TIMER_REQUEST,sizeof(Msg),&Msg);
}

bool tLightControlOutgoingFrames::SendMsgDefaultTimerResponse(uint8_t RecieverID, uint8_t OutputID, uint16_t DefTimerValue)
{
  tMessageTypeDefaultTimerResponse Msg;
#if CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS
  DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_DEFAULT_TIMER_RESPONSE");
#endif CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS
  Msg.OutputID = OutputID;
  Msg.DefTimerValue = DefTimerValue;
  CommSenderProcess::Instance->Enqueue(RecieverID,MESSAGE_TYPE_DEFAULT_TIMER_RESPONSE,sizeof(Msg),&Msg);
}

bool tLightControlOutgoingFrames::SendMsgGetActionsRequest(uint8_t RecieverID) {
#if CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS
	DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_GET_ACTIONS_REQUEST");
#endif CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS`
	  CommSenderProcess::Instance->Enqueue(RecieverID,MESSAGE_TYPE_GET_ACTIONS_REQUEST,0,NULL);
}

bool tLightControlOutgoingFrames::SendMsgGetActionsResponse(uint8_t RecieverID, tMessageTypeGetActionsResponse Msg)
{
#if CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS
	DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_GET_ACTIONS_RESPONSE");
#endif CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS`
	  CommSenderProcess::Instance->Enqueue(RecieverID,MESSAGE_TYPE_GET_ACTIONS_RESPONSE,sizeof(Msg),&Msg);
}

#endif // CONFIG_LIGHT_CONTROL_APP

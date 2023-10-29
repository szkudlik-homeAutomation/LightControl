/*
 * tLightControltLightControlOutgoingFramess.cpp
 *
 *  Created on: Oct 10, 2023
 *      Author: mszkudli
 */

#include "tLightControlOutgoingFrames.h"
#include "../global.h"
#include "tLightControlOutgoingFrames.h"
#include "Common_code/TLE8457_serial/TLE8457_serial_lib.h"

// BUTTON  PRESS
bool tLightControlOutgoingFrames::SendMsgButtonPress(uint8_t RecieverID, uint8_t ForceSrcId, uint16_t ShortClick, uint16_t LongClick, uint16_t DoubleClick)
{
  tMessageTypeButtonPress Msg;
  Msg.ShortClick = ShortClick;
  Msg.LongClick  = LongClick;
  Msg.DoubleClick = DoubleClick;
  Msg.ForceSrcId = ForceSrcId;

  DEBUG_PRINTLN_3("===================>sending MESSAGE_BUTTON_PRESS");
  CommSender.Enqueue(RecieverID,MESSAGE_BUTTON_PRESS,sizeof(Msg),&Msg);
  return true;
};

bool tLightControlOutgoingFrames::SendMsgClearAllActions(uint8_t RecieverID)
{
#if CONFIG_CENTRAL_NODE
  DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_CLEAR_ACTIONS");
  CommSender.Enqueue(RecieverID,MESSAGE_TYPE_CLEAR_ACTIONS,0,NULL);
#endif
  return true;
};

bool tLightControlOutgoingFrames::SendMsgAddAction(uint8_t RecieverID, uint8_t OutId, uint8_t SenderDevID, uint8_t ButtonId, uint8_t TriggerType, uint8_t ActionType, uint16_t Timer, uint8_t  OutputsMask, uint8_t  OutputsStates)
{
#if CONFIG_CENTRAL_NODE
  tMessageTypeSetAction Message;
  Message.OutId = OutId;
  Message.SenderDevID = SenderDevID;
  Message.ButtonId = ButtonId;
  Message.TriggerType = TriggerType;
  Message.ActionType = ActionType;
  Message.Timer = Timer;
  Message.OutputsMask = OutputsMask;
  Message.OutputsStates = OutputsStates;

  DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_SET_ACTION");
  CommSender.Enqueue(RecieverID,MESSAGE_TYPE_SET_ACTION,sizeof(Message),&Message);
#endif
  return true;
};

// EEPROM CRC Handshake
bool tLightControlOutgoingFrames::SendMsgEepromCrcRequest(uint8_t RecieverID)
{
#if CONFIG_CENTRAL_NODE
  DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_EEPROM_CRC_REQUEST");
  CommSender.Enqueue(RecieverID,MESSAGE_TYPE_EEPROM_CRC_REQUEST,0,NULL);
#endif
  return true;
}

bool tLightControlOutgoingFrames::SendMsgEepromCrcResponse(uint8_t RecieverID,  uint8_t NumOfActions, uint16_t EepromCRC)
{
  tMessageTypeEepromCRCResponse Msg;
  Msg.NumOfActions = NumOfActions;
  Msg.EepromCRC = EepromCRC;
  DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_EEPROM_CRC_RESPONSE");
  CommSender.Enqueue(RecieverID,MESSAGE_TYPE_EEPROM_CRC_RESPONSE,sizeof(Msg),&Msg);
  return true;
};


bool tLightControlOutgoingFrames::SendMsgSetDefaultTimer(uint8_t RecieverID, uint8_t OutputID, uint16_t DefTimerValue)
{
  tMessageTypeSetDefaultTimer Msg;
  DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_SET_DEFAULT_TIMER");
  Msg.OutputID = OutputID;
  Msg.DefaultTimerValue = DefTimerValue;
  CommSender.Enqueue(RecieverID,MESSAGE_TYPE_SET_DEFAULT_TIMER,sizeof(Msg),&Msg);
}

bool tLightControlOutgoingFrames::SendMsgDefaultTimerRequest(uint8_t RecieverID, uint8_t OutputID)
{
  tMessageTypeDefaultTimerRequest Msg;
  DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_DEFAULT_TIMER_REQUEST");
  Msg.OutputID = OutputID;
  CommSender.Enqueue(RecieverID,MESSAGE_TYPE_DEFAULT_TIMER_REQUEST,sizeof(Msg),&Msg);
}

bool tLightControlOutgoingFrames::SendMsgDefaultTimerResponse(uint8_t RecieverID, uint8_t OutputID, uint16_t DefTimerValue)
{
  tMessageTypeDefaultTimerResponse Msg;
  DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_DEFAULT_TIMER_RESPONSE");
  Msg.OutputID = OutputID;
  Msg.DefTimerValue = DefTimerValue;
  CommSender.Enqueue(RecieverID,MESSAGE_TYPE_DEFAULT_TIMER_RESPONSE,sizeof(Msg),&Msg);
}


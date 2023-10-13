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

// OVERVIEW STATE HANDSHAKE
bool tLightControlOutgoingFrames::SendMsgOverviewStateRequest(uint8_t RecieverID)
{
#if CONFIG_CENTRAL_NODE
  DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_OVERVIEW_STATE_REQUEST");
  CommSender.Enqueue(RecieverID, MESSAGE_TYPE_OVERVIEW_STATE_REQUEST, 0, NULL);
#endif
  return true;
}

bool tLightControlOutgoingFrames::SendMsgOverviewStateResponse(uint8_t RecieverID, uint8_t  PowerState, uint8_t  TimerState)
{
  tMessageTypeOverviewStateResponse Msg;
  Msg.PowerState = PowerState;
  Msg.TimerState = TimerState;
  DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_OVERVIEW_STATE_RESPONSE");
  CommSender.Enqueue(RecieverID, MESSAGE_TYPE_OVERVIEW_STATE_RESPONSE, sizeof(Msg), &Msg);
  return true;
}

// GET OUTPUT STATE HANDSHAKE
bool tLightControlOutgoingFrames::SendMsgOutputStateRequest(uint8_t RecieverID, uint8_t  OutputID)
{
#if CONFIG_CENTRAL_NODE
  tMessageTypeOutputStateRequest Msg;
  Msg.OutputID = OutputID;
  DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_OUTPUT_STATE_REQUEST");
  CommSender.Enqueue(RecieverID, MESSAGE_TYPE_OUTPUT_STATE_REQUEST, sizeof(Msg), &Msg);
#endif
  return true;
};

bool tLightControlOutgoingFrames::SendMsgOutputStateResponse(uint8_t RecieverID, uint8_t  OutputID, uint8_t  PowerState, uint16_t TimerValue, uint16_t DefaultTimer)
{
  tMessageTypeOutputStateResponse Msg;
  Msg.OutputID = OutputID;
  Msg.PowerState = PowerState;
  Msg.TimerValue = TimerValue;
  Msg.DefaultTimer = DefaultTimer;
  DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_OUTPUT_STATE_RESPONSE");
  CommSender.Enqueue(RecieverID, MESSAGE_TYPE_OUTPUT_STATE_RESPONSE, sizeof(Msg), &Msg);

  return true;
};

// SET OUTPUT
bool tLightControlOutgoingFrames::SendMsgSetOutput(uint8_t RecieverID, uint8_t  OutId, uint8_t  State, uint16_t Timer)
{
#if CONFIG_CENTRAL_NODE
  tMessageTypeSetOutput Message;
  Message.OutId = OutId;
  Message.State = State;
  Message.Timer = Timer;
  DEBUG_PRINTLN_3("===================>sending MESSAGE_TYPE_SET_OUTPUT");
  CommSender.Enqueue(RecieverID, MESSAGE_TYPE_SET_OUTPUT, sizeof(Message), &Message);
#endif
  return true;
}

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


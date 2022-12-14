/*
 * IncomingFrameHandler.cpp
 *
 *  Created on: 10 sie 2022
 *      Author: szkud
 */

#include <AceCRC.h>
using namespace ace_crc::crc16ccitt_nibble;


#include "IncomingFrameHandler.h"
#include "common.h"

#include "WorkerProcess.h"
#include "OutputProcess.h"
#include "version.h"
#include "Eeprom.h"


void IncomingFrameHandler::onFrame(void *pData, uint8_t MessageType, uint8_t SenderDevId)
{
      switch (MessageType)
      {
       case MESSAGE_TYPE_OVERVIEW_STATE_REQUEST:
           DEBUG_PRINTLN_3("===================>MESSAGE_TYPE_OVERVIEW_STATE_REQUEST");
           HandleMsgOverviewStateRequest(SenderDevId);
           break;

       case MESSAGE_TYPE_OVERVIEW_STATE_RESPONSE:
           DEBUG_PRINTLN_3("===================>MESSAGE_TYPE_OVERVIEW_STATE_RESPONSE");
           HandleMsgOverviewStateResponse(SenderDevId,(tMessageTypeOverviewStateResponse*) (pData));
           break;

       case MESSAGE_TYPE_OUTPUT_STATE_REQUEST:
           DEBUG_PRINTLN_3("===================>MESSAGE_TYPE_OUTPUT_STATE_REQUEST");
           HandleMsgOutputStateRequest(SenderDevId,(tMessageTypeOutputStateRequest*)(pData));
           break;

       case MESSAGE_TYPE_OUTPUT_STATE_RESPONSE:
           DEBUG_PRINTLN_3("===================>MESSAGE_TYPE_OUTPUT_STATE_RESPONSE");
           HandleMsgOutputStateResponse(SenderDevId,(tMessageTypeOutputStateResponse*) (pData));
           break;

       case MESSAGE_TYPE_SET_OUTPUT:
           DEBUG_PRINTLN_3("===================>MESSAGE_TYPE_SET_OUTPUT");
           HandleMsgSetOutput(SenderDevId,(tMessageTypeSetOutput*)(pData));
           break;

       case MESSAGE_BUTTON_PRESS:
           DEBUG_PRINTLN_3("===================>MESSAGE_BUTTON_PRESS");
           HandleMsgButtonPress(SenderDevId, (tMessageTypeButtonPress*)(pData));
           break;

       case MESSAGE_TYPE_SET_ACTION:
             DEBUG_PRINTLN_3("===================>MESSAGE_TYPE_SET_ACTION");
             HandleMsgSetAction(SenderDevId, (tMessageTypeSetAction*)(pData));
           break;

       case MESSAGE_TYPE_CLEAR_ACTIONS:
             DEBUG_PRINTLN_3("===================>MESSAGE_TYPE_CLEAR_ACTIONS");
             HandleMsgClearAllActions(SenderDevId);
           break;

       case MESSAGE_TYPE_FW_VERSION_REQUEST:
             DEBUG_PRINTLN_3("===================>MESSAGE_TYPE_FW_VERSION_REQUEST");
             HandleMsgVersionRequest(SenderDevId);
           break;

       case MESSAGE_TYPE_FW_VERSION_RESPONSE:
             DEBUG_PRINTLN_3("===================>MESSAGE_TYPE_FW_VERSION_RESPONSE");
             HandleMsgVersionResponse(SenderDevId,(tMessageTypeFwVesionResponse*)(pData));
           break;

       case MESSAGE_TYPE_EEPROM_CRC_REQUEST:
             DEBUG_PRINTLN_3("===================>MESSAGE_TYPE_EEPROM_CRC_REQUEST");
             HandleMsgEepromCrcRequest(SenderDevId);
           break;

       case MESSAGE_TYPE_EEPROM_CRC_RESPONSE:
             DEBUG_PRINTLN_3("===================>MESSAGE_TYPE_EEPROM_CRC_RESPONSE");
             HandleMsgEepromCrcResponse(SenderDevId,(tMessageTypeEepromCRCResponse*)(pData));
           break;

       case MESSAGE_TYPE_FORCE_RESET:
             DEBUG_PRINTLN_3("===================>MESSAGE_TYPE_FORCE_RESET");
             cli();
             while(1); // let watchdog reboot the device
           break;

       case MESSAGE_TYPE_SET_DEFAULT_TIMER:
             DEBUG_PRINTLN_3("===================>MESSAGE_TYPE_SET_DEFAULT_TIMER");
             HandleMsgSetDefaultTimer(SenderDevId,(tMessageTypeSetDefaultTimer*)(pData));
           break;

       case MESSAGE_TYPE_DEFAULT_TIMER_REQUEST:
             DEBUG_PRINTLN_3("===================>MESSAGE_TYPE_DEFAULT_TIMER_REQUEST");
             HandleMsgDefaultTimerRequest(SenderDevId,(tMessageTypeDefaultTimerRequest*)(pData));
           break;

       case MESSAGE_TYPE_DEFAULT_TIMER_RESPONSE:
             DEBUG_PRINTLN_3("===================>MESSAGE_TYPE_DEFAULT_TIMER_RESPONSE");
             HandleMsgDefaultTimerResponse(SenderDevId,(tMessageTypeDefaultTimerResponse*)(pData));
           break;


     default:
         DEBUG_PRINTLN_3("MESSAGE  unknown type, drop");
      }
}


void IncomingFrameHandler::HandleMsgOverviewStateRequest(uint8_t SenderID)
{
   Worker.SendMsgOverviewStateResponse(SenderID,OutputProcess.GetOutputStateMap(),OutputProcess.GetOutputTimersStateMap());
}


void IncomingFrameHandler::HandleMsgOverviewStateResponse(uint8_t SenderID, tMessageTypeOverviewStateResponse* Message)
{
#ifdef CONTROLLER
      RespHandler.OverviewStateResponseHandler(SenderID,Message->PowerState,Message->TimerState);
#endif
}

void IncomingFrameHandler::HandleMsgOutputStateRequest(uint8_t SenderID, tMessageTypeOutputStateRequest* Message)
{
  if (Message->OutputID < NUM_OF_OUTPUTS)
  {
      uint16_t DefTimer;
      EEPROM.get(EEPROM_DEFAULT_TIMER_VALUE_OFFSET+Message->OutputID*(sizeof(uint16_t)),DefTimer);

      Worker.SendMsgOutputStateResponse(SenderID,Message->OutputID, OutputProcess.GetOutputState(Message->OutputID), OutputProcess.GetOutputTimer(Message->OutputID),DefTimer);
  }
}

void IncomingFrameHandler::HandleMsgOutputStateResponse(uint8_t SenderID, tMessageTypeOutputStateResponse* Message)
{
#ifdef CONTROLLER
  RespHandler.OutputStateResponseHandler(SenderID,Message->OutputID,Message->PowerState,Message->TimerValue,Message->DefaultTimer);
#endif
}

void IncomingFrameHandler::HandleMsgSetOutput(uint8_t SenderID, tMessageTypeSetOutput* Message)
{
   if (Message->OutId >= NUM_OF_OUTPUTS)
   {
     // drop it
     return;
   }
   uint16_t Timer = Message->Timer;
   if (DEFAULT_TIMER == Timer)
   {
     EEPROM.get(EEPROM_DEFAULT_TIMER_VALUE_OFFSET+Message->OutId*(sizeof(uint16_t)),Timer);
   }

  OutputProcess.SetOutput(Message->OutId,Message->State,Timer,tOutputProcess::ForceTimer);
}


void IncomingFrameHandler::HandleMsgButtonPress(uint8_t SenderID, tMessageTypeButtonPress *Message)
{
#ifdef CONTROLLER
#ifdef DEBUG_3
    RespHandler.print(F("Dev ID:"));
    RespHandler.print(SenderID,HEX);
    RespHandler.print(F(" ForcedSrc:"));
    RespHandler.print(Message->ForceSrcId,HEX);
    RespHandler.print(F(" short:"));
    RespHandler.print(Message->ShortClick,BIN);
    RespHandler.print(F(" long:"));
    RespHandler.print(Message->LongClick,BIN);
    RespHandler.print(F(" dbl:"));
    RespHandler.println(Message->DoubleClick,BIN);
#endif
#endif

  if (Message->ForceSrcId)
    SenderID = Message->ForceSrcId;

  // iterate through the action table
  tMessageTypeSetAction Action;
  uint8_t i = EEPROM.read(EEPROM_ACTION_TABLE_USAGE_OFFSET);
  // remeber output state BEFORE performing action set
  uint8_t OutputState = OutputProcess.GetOutputStateMap();
  while (i--)
  {
    EEPROM.get(EEPROM_ACTION_TABLE_OFFSET+(EEPROM_ACTION_TABLE_SIZE*i),Action);
    // does the sender ID match?
    if (Action.SenderDevID != SenderID) continue;

    // does the action type and button ID match?
    uint16_t ButtonMask = (1 << Action.ButtonId);
    if ( (BUTTON_TRIGGER_TYPE_ANY_CLICK == Action.TriggerType)    && (0 == (Message->ShortClick & ButtonMask)) && (0 == (Message->DoubleClick & ButtonMask))) continue;

    if ( (BUTTON_TRIGGER_TYPE_CLICK == Action.TriggerType)        && (0 == (Message->ShortClick & ButtonMask))) continue;

    if ( (BUTTON_TRIGGER_TYPE_LONG_CLICK == Action.TriggerType)   && (0 == (Message->LongClick & ButtonMask))) continue;

    if ( (BUTTON_TRIGGER_TYPE_DOUBLE_CLICK == Action.TriggerType) && (0 == (Message->DoubleClick & ButtonMask))) continue;

    // does the current output state match the mask?
    uint8_t OutputStateMasked = OutputState & Action.OutputsMask;
    if (OutputStateMasked != Action.OutputsStates) continue;

    if (Action.OutId  >= NUM_OF_OUTPUTS)
    {
      // drop it
      return;
    }

    uint16_t Timer = Action.Timer;
    if (DEFAULT_TIMER == Timer)
    {
      EEPROM.get(EEPROM_DEFAULT_TIMER_VALUE_OFFSET+Action.OutId*(sizeof(uint16_t)),Timer);
    }

    switch (Action.ActionType)
    {
      case BUTTON_ACTION_TYPE_ON:
         // when "turn on" action is triggered bu a button, don't set a timer if it is shorter than current timer
        OutputProcess.SetOutput(Action.OutId,1,Timer,tOutputProcess::TimerLongerOnly);
        break;

      case BUTTON_ACTION_TYPE_OFF:
        OutputProcess.SetOutput(Action.OutId,0,0,tOutputProcess::ForceTimer);
        break;

      case BUTTON_ACTION_TYPE_TOGGLE:
        OutputProcess.ToggleOutput(Action.OutId,Timer);
        break;
    }
  }
}


void IncomingFrameHandler::HandleMsgClearAllActions(uint8_t SenderID)
{
    EEPROM.write(EEPROM_ACTION_TABLE_USAGE_OFFSET,0);
}


void IncomingFrameHandler::HandleMsgSetAction(uint8_t SenderID, tMessageTypeSetAction* Message)
{
  uint8_t ActionTableUsage = EEPROM.read(EEPROM_ACTION_TABLE_USAGE_OFFSET);
  if (ActionTableUsage < ACTION_TABLE_SIZE)
  {
    EEPROM.put(EEPROM_ACTION_TABLE_OFFSET+(EEPROM_ACTION_TABLE_SIZE*ActionTableUsage),*Message);
    ActionTableUsage++;
    EEPROM.write(EEPROM_ACTION_TABLE_USAGE_OFFSET,ActionTableUsage);
  }
}


void IncomingFrameHandler::HandleMsgEepromCrcRequest(uint8_t SenderID)
{

  int NumOfActions = EEPROM.read(EEPROM_ACTION_TABLE_USAGE_OFFSET);
  tMessageTypeSetAction Action;
  crc_t crc = crc_init();
  uint8_t i = NumOfActions;
  while (i--)
  {
    EEPROM.get(EEPROM_ACTION_TABLE_OFFSET+(EEPROM_ACTION_TABLE_SIZE*i),Action);
    crc = crc_update(crc, &Action, sizeof(Action));
  }
  crc = crc_finalize(crc);
  Worker.SendMsgEepromCrcResponse(SenderID,NumOfActions,crc);
}


void IncomingFrameHandler::HandleMsgEepromCrcResponse(uint8_t SenderID, tMessageTypeEepromCRCResponse* Message)
{
#ifdef CONTROLLER
  RespHandler.EepromCRCResponseHandler(SenderID,Message->NumOfActions,Message->EepromCRC);
#endif
}


void IncomingFrameHandler::HandleMsgVersionRequest(uint8_t SenderID)
{
   Worker.SendMsgVersionResponse(SenderID,FW_VERSION_MAJOR,FW_VERSION_MINOR,FW_VERSION_PATCH);
}


void IncomingFrameHandler::HandleMsgVersionResponse(uint8_t SenderID, tMessageTypeFwVesionResponse *Message)
{
#ifdef CONTROLLER
  RespHandler.VersionResponseHandler(SenderID,Message->Major,Message->Minor,Message->Patch);
#endif
}


void IncomingFrameHandler::HandleMsgSetDefaultTimer(uint8_t SenderID, tMessageTypeSetDefaultTimer *Message)
{
  if (Message->OutputID >= NUM_OF_OUTPUTS) return;
  EEPROM.put(EEPROM_DEFAULT_TIMER_VALUE_OFFSET+Message->OutputID*(sizeof(uint16_t)),Message->DefaultTimerValue);
}

void IncomingFrameHandler::HandleMsgDefaultTimerRequest(uint8_t SenderID, tMessageTypeDefaultTimerRequest *Message)
{
  if (Message->OutputID >= NUM_OF_OUTPUTS) return;
  uint16_t DefTimer;
  EEPROM.get(EEPROM_DEFAULT_TIMER_VALUE_OFFSET+Message->OutputID*(sizeof(uint16_t)),DefTimer);
  Worker.SendMsgDefaultTimerResponse(SenderID,Message->OutputID,DefTimer);
}

void IncomingFrameHandler::HandleMsgDefaultTimerResponse(uint8_t SenderID, tMessageTypeDefaultTimerResponse *Message)
{
#ifdef CONTROLLER
  RespHandler.DefaultTimerResponseHandler(SenderID,Message->OutputID,Message->DefTimerValue);
#endif
}


/*
 * IncomingFrameHandler.cpp
 *
 *  Created on: 10 sie 2022
 *      Author: szkud
 */
//#define LOCAL_LOGLEVEL_1 1
//#define LOCAL_LOGLEVEL_2 1

#include "../../global.h"

#if CONFIG_LIGHT_CONTROL_APP

#include "tLightControl.h"
#include "tLightControlFrames.h"
#include "tLightControlOutputProcess.h"
#include "tLightControlOutgoingFrames.h"

#include "../lib/AceCRC/src/AceCRC.h"
#include "../Common_code/TLE8457_serial/TLE8457_serial_lib.h"
using namespace ace_crc::crc16ccitt_nibble;

// instance of the class
tLightControl LightControl;


void tLightControl::onMessage(uint8_t type, uint16_t data, void *pData)
{
    uint8_t ret;

    if (type == MessageType_SerialFrameRecieved)
    {
        tCommunicationFrame *pFrame = (tCommunicationFrame *)pData;
        uint8_t SenderDevId = pFrame->SenderDevId;

        switch (data)	// messageType
          {
           case MESSAGE_BUTTON_PRESS:
               HandleMsgButtonPress(SenderDevId, (tMessageTypeButtonPress*)(pFrame->Data));
               break;

           case MESSAGE_TYPE_SET_ACTION:
                 HandleMsgSetAction(SenderDevId, (tMessageTypeSetAction*)(pFrame->Data));
               break;

           case MESSAGE_TYPE_CLEAR_ACTIONS:
                 HandleMsgClearAllActions(SenderDevId);
               break;

           case MESSAGE_TYPE_EEPROM_CRC_REQUEST:
                 HandleMsgEepromCrcRequest(SenderDevId);
               break;

           case MESSAGE_TYPE_EEPROM_CRC_RESPONSE:
                 HandleMsgEepromCrcResponse(SenderDevId,(tMessageTypeEepromCRCResponse*)(pFrame->Data));
               break;

           case MESSAGE_TYPE_SET_DEFAULT_TIMER:
                 HandleMsgSetDefaultTimer(SenderDevId,(tMessageTypeSetDefaultTimer*)(pFrame->Data));
               break;

           case MESSAGE_TYPE_DEFAULT_TIMER_REQUEST:
                 HandleMsgDefaultTimerRequest(SenderDevId,(tMessageTypeDefaultTimerRequest*)(pFrame->Data));
               break;

           case MESSAGE_TYPE_DEFAULT_TIMER_RESPONSE:
                 HandleMsgDefaultTimerResponse(SenderDevId,(tMessageTypeDefaultTimerResponse*)(pFrame->Data));
               break;
          }
    }

    if (type == MessageType_DigialInputEvent)
    {
    	tDigialInputEvent *event = (tDigialInputEvent *)pData;

    	// send message to all devices
    	tLightControlOutgoingFrames::SendMsgButtonPress(DEVICE_ID_BROADCAST,0,
    			event->ShortClick, event->LongClick, event->DoubleClick);
    }
}

void tLightControl::HandleMsgButtonPress(uint8_t SenderID, tMessageTypeButtonPress *Message)
{
#if CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS
    DEBUG_PRINTLN_3("===================>MESSAGE_BUTTON_PRESS");

    DEBUG_PRINT_3("Dev ID:");
	DEBUG_3(print(SenderID,HEX));
	DEBUG_PRINT_3(" ForcedSrc:");
	DEBUG_3(print(Message->ForceSrcId,HEX));
    DEBUG_PRINT_3(" short:");
    DEBUG_3(print(Message->ShortClick,BIN));
    DEBUG_PRINT_3(" long:");
    DEBUG_3(print(Message->LongClick,BIN));
    DEBUG_PRINT_3(" dbl:");
    DEBUG_3(println(Message->DoubleClick,BIN));
#endif CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS

  if (Message->ForceSrcId)
    SenderID = Message->ForceSrcId;

  // iterate through the action table
  tMessageTypeSetAction Action;
  uint8_t i = EEPROM.read(EEPROM_ACTION_TABLE_USAGE_OFFSET);
  DEBUG_PRINT_1("num of EEPROM entries:");
  DEBUG_1(println(i,DEC));


  // remeber output state BEFORE performing action set
  uint8_t OutputState = tOutputProcess::Instance->GetOutputStateMap();
  while (i--)
  {
    EEPROM.get(EEPROM_ACTION_TABLE_OFFSET+(EEPROM_CONFIG_ACTION_TABLE_SIZE*i),Action);

    DEBUG_PRINT_1("Eeprom entry #");
	DEBUG_1(print(i,DEC));

    DEBUG_PRINT_1(" OutId:");
	DEBUG_1(print(Action.OutId,DEC));
	DEBUG_PRINT_1(" SenderDevID:");
	DEBUG_1(print(Action.SenderDevID,HEX));
    DEBUG_PRINT_1(" ButtonId:");
	DEBUG_1(print(Action.SenderDevID,HEX));
    DEBUG_PRINT_1(" TriggerType:");
    DEBUG_1(print(Action.TriggerType,DEC));
    DEBUG_PRINT_1(" ActionType:");
    DEBUG_1(print(Action.ActionType,DEC));
    DEBUG_PRINT_1(" Timer:");
    DEBUG_1(print(Action.Timer,DEC));
    DEBUG_PRINT_1(" OutputsMask:");
    DEBUG_1(print(Action.OutputsMask,BIN));
    DEBUG_PRINT_1(" OutputsStates:");
    DEBUG_1(println(Action.OutputsStates,BIN));

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

    if (Action.OutId  >= CONFIG_OUTPUT_PROCESS_NUM_OF_PINS)
    {
      // drop it
      return;
    }

    uint16_t Timer = Action.Timer;
    if (DEFAULT_TIMER == Timer)
    {
      EEPROM.get(EEPROM_DEFAULT_TIMER_VALUE_OFFSET+Action.OutId*(sizeof(uint16_t)),Timer);
    }

    DEBUG_PRINTLN_2("Action HIT!!!");

    switch (Action.ActionType)
    {
      case BUTTON_ACTION_TYPE_ON:
         // when "turn on" action is triggered bu a button, don't set a timer if it is shorter than current timer
    	DEBUG_PRINTLN_2("Setting output ");
        tOutputProcess::Instance->SetOutput(Action.OutId,1,Timer,tOutputProcess::TimerLongerOnly);
        break;

      case BUTTON_ACTION_TYPE_OFF:
        tOutputProcess::Instance->SetOutput(Action.OutId,0,0,tOutputProcess::ForceTimer);
        break;

      case BUTTON_ACTION_TYPE_TOGGLE:
        tOutputProcess::Instance->ToggleOutput(Action.OutId,Timer);
        break;
    }
  }
}

void tLightControl::HandleMsgClearAllActions(uint8_t SenderID)
{
#if CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS
    DEBUG_PRINTLN_3("===================>MESSAGE_TYPE_CLEAR_ACTIONS");
#endif CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS

    EEPROM.write(EEPROM_ACTION_TABLE_USAGE_OFFSET,0);
}


void tLightControl::HandleMsgSetAction(uint8_t SenderID, tMessageTypeSetAction* Message)
{
#if CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS
    DEBUG_PRINTLN_3("===================>MESSAGE_TYPE_SET_ACTION");
#endif CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS

  uint8_t ActionTableUsage = EEPROM.read(EEPROM_ACTION_TABLE_USAGE_OFFSET);
  if (ActionTableUsage < CONFIG_LIGHT_CONTROL_ACTION_TABLE_SIZE)
  {
    EEPROM.put(EEPROM_ACTION_TABLE_OFFSET+(EEPROM_CONFIG_ACTION_TABLE_SIZE*ActionTableUsage),*Message);
    ActionTableUsage++;
    EEPROM.write(EEPROM_ACTION_TABLE_USAGE_OFFSET,ActionTableUsage);
  }
}


void tLightControl::HandleMsgEepromCrcRequest(uint8_t SenderID)
{
#if CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS
    DEBUG_PRINTLN_3("===================>MESSAGE_TYPE_EEPROM_CRC_REQUEST");
#endif CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS

  int NumOfActions = EEPROM.read(EEPROM_ACTION_TABLE_USAGE_OFFSET);
  tMessageTypeSetAction Action;
  crc_t crc = crc_init();
  uint8_t i = NumOfActions;
  while (i--)
  {
    EEPROM.get(EEPROM_ACTION_TABLE_OFFSET+(EEPROM_CONFIG_ACTION_TABLE_SIZE*i),Action);
    crc = crc_update(crc, &Action, sizeof(Action));
  }
  crc = crc_finalize(crc);
  tLightControlOutgoingFrames::SendMsgEepromCrcResponse(SenderID,NumOfActions,crc);
}


void tLightControl::HandleMsgEepromCrcResponse(uint8_t SenderID, tMessageTypeEepromCRCResponse* Message)
{
#if CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS
    DEBUG_PRINTLN_3("===================>MESSAGE_TYPE_EEPROM_CRC_RESPONSE");

    LOG_PRINT("Eeprom CRC for device ");
	LOG(print(SenderID,HEX));
	LOG_PRINT(" num of actions=");
	LOG(print(Message->NumOfActions,DEC));
	LOG_PRINT(" CRC=");
	LOG(println(Message->EepromCRC,DEC));
#endif CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS
}

void tLightControl::HandleMsgSetDefaultTimer(uint8_t SenderID, tMessageTypeSetDefaultTimer *Message)
{
#if CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS
  DEBUG_PRINTLN_3("===================>MESSAGE_TYPE_SET_DEFAULT_TIMER");
#endif CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS

  if (Message->OutputID >= CONFIG_OUTPUT_PROCESS_NUM_OF_PINS) return;
  EEPROM.put(EEPROM_DEFAULT_TIMER_VALUE_OFFSET+Message->OutputID*(sizeof(uint16_t)),Message->DefaultTimerValue);
}

void tLightControl::HandleMsgDefaultTimerRequest(uint8_t SenderID, tMessageTypeDefaultTimerRequest *Message)
{
#if CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS
  DEBUG_PRINTLN_3("===================>MESSAGE_TYPE_DEFAULT_TIMER_REQUEST");
#endif CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS

  if (Message->OutputID >= CONFIG_OUTPUT_PROCESS_NUM_OF_PINS) return;
  uint16_t DefTimer;
  EEPROM.get(EEPROM_DEFAULT_TIMER_VALUE_OFFSET+Message->OutputID*(sizeof(uint16_t)),DefTimer);
  tLightControlOutgoingFrames::SendMsgDefaultTimerResponse(SenderID,Message->OutputID,DefTimer);
}

void tLightControl::HandleMsgDefaultTimerResponse(uint8_t SenderID, tMessageTypeDefaultTimerResponse *Message)
{
#if CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS
    DEBUG_PRINTLN_3("===================>MESSAGE_TYPE_DEFAULT_TIMER_RESPONSE");

    LOG_PRINT("Default timer for device ");
	LOG(print(SenderID,HEX));
	LOG_PRINT(" outId ");
	LOG(print(Message->OutputID,DEC));
	LOG_PRINT("=");
	LOG(println(Message->DefTimerValue,DEC));
#endif CONFIG_LIGHT_CONTROL_LOG_INCOMING_EVENTS
}

#endif CONFIG_LIGHT_CONTROL_APP


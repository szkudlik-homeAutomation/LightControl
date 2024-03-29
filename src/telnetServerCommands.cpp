#include "../global.h"

#include "tLightControlOutgoingFrames.h"
#include "Common_code/Network/telnetServer.h"
#include "lib/Commander/src/Commander.h"

#if CONFIG_CENTRAL_NODE


bool send_ClearActions(Commander &Cmdr);
bool send_addAction(Commander &Cmdr);
bool send_GetEepromCrc(Commander &Cmdr);
bool send_SetDefaultTimer(Commander &Cmdr);
bool send_GetDefaultTimer(Commander &Cmdr);
bool send_ButtonPress(Commander &Cmdr);
bool send_ClearCodes(Commander &Cmdr);
bool send_addCode(Commander &Cmdr);
bool send_triggerCode(Commander &Cmdr);

const commandList_t TelnetCommands[] = {
  {"enableLogs",      TelnetEnableLogs,             "enable logs on telnet console"},
  {"disableLogs",     TelnetDisableLogs,            "enable logs on telnet console"},
  {"StateOverview",   send_stateOverviewHandler,    "MESSAGE_TYPE_REQUEST_OVERVIEW_STATE dev_id"},
  {"OutputState",     send_OutputStateHandler,      "MESSAGE_TYPE_OUTPUT_STATE_REQUEST dev_id out_id"},
  {"SetOutput",       send_SetOutputHandler,        "MESSAGE_TYPE_OUTPUT_STATE_REQUEST dev_id out_id state [timer] (not set=default, 0-forever)"},
  {"ButtonPress",     send_ButtonPress,             "MESSAGE_BUTTON_PRESS with a forced src id"},
  {"ClearActions",    send_ClearActions,            "MESSAGE_TYPE_CLEAR_ACTIONS dev_id"},
  {"GetEepromCrc",    send_GetEepromCrc,            "MESSAGE_TYPE_EEPROM_CRC_REQUEST dev_id"},
  {"GetVersion",      send_GetVersion,              "MESSAGE_TYPE_FW_VERSION_REQUEST dev_id"},
  {"SetDefTimer",     send_SetDefaultTimer,         "MESSAGE_TYPE_SET_DEFAULT_TIMER dev_id out_id defTimerValue (0=no timer)"},
  {"GetDefTimer",     send_GetDefaultTimer,         "MESSAGE_TYPE_DEFAULT_TIMER_REQUEST dev_id out_id"},
  {"Reset",           send_Reset,                   "MESSAGE_TYPE_FORCE_RESET dev_id (may be 255 - broadcast)"},
  {"ScanActiveNodes", trigger_ScanNodes,            "Scan the bus for nodes from 1 to 32"},
  {"addAction",       send_addAction,               "MESSAGE_TYPE_SET_ACTION dev_id OutId SenderID ButtonId [ Timer TriggerType ActionType OutputsMask OutputsStates ]"},
  {"ClearCodes",      send_ClearCodes,              "KEY CODE: ClearCodes dev_id"},
  {"AddCode",       send_addCode, "KEY CODE: AddCode dev_id type code ButtonBitmap"},
  {"TriggerCode",   send_triggerCode, "KEY CODE: TriggerCode dev_id type code"},};

tTelnetServer TelnetServer(TelnetCommands,sizeof(TelnetCommands));

bool send_ClearActions(Commander &Cmdr)
{
  int Dst;
  if(Cmdr.getInt(Dst))
  {
    tLightControlOutgoingFrames::SendMsgClearAllActions(Dst);
  }
  else
  {
    Cmdr.println(F("Usage: ClearActions dst_dev_id"));
    return false;
  }

  return true;
}

bool send_addAction(Commander &Cmdr)
{
  int RecieverID;
  int OutId;
  int SenderDevID;
  int ButtonId;
  int TriggerType = BUTTON_TRIGGER_TYPE_ANY_CLICK;
  int ActionType = BUTTON_ACTION_TYPE_TOGGLE;
  int Timer = 0xFFFF;
  int OutputsMask = 0;
  int OutputsStates = 0;

  if(!Cmdr.getInt(RecieverID))
  {
    goto error;
  }
  if(!Cmdr.getInt(OutId))
  {
    goto error;
  }
  if(!Cmdr.getInt(SenderDevID))
  {
    goto error;
  }
  if(!Cmdr.getInt(ButtonId))
  {
    goto error;
  }
  if(!Cmdr.getInt(Timer))
  {
    goto final;
  }
  if(!Cmdr.getInt(TriggerType))
  {
    goto final;
  }
  if(!Cmdr.getInt(ActionType))
  {
    goto final;
  }
  if(!Cmdr.getInt(OutputsMask))
  {
    goto final;
  }
  if(!Cmdr.getInt(OutputsStates))
  {
    goto final;
  }

final:
  tLightControlOutgoingFrames::SendMsgAddAction(RecieverID, OutId, SenderDevID, ButtonId, TriggerType, ActionType, Timer, OutputsMask, OutputsStates);
  return true;

error:
  Cmdr.println(F("Usage: addAction dev_id OutId SenderID ButtonId [ Timer TriggerType ActionType OutputsMask OutputsStates ] "));
  Cmdr.println(F("   TriggerType -  0=CLICK,   1=LONG_CLICK,  2=DBL_CLICK    3=ANY_CLICK (default)"));
  Cmdr.println(F("   ActionType -   0=TOGGLE (default),  1=ON,          2=OFF"));
  Cmdr.println(F("   Timer = 0 - no timer, 65535(0xFFFF) - default timer (default)"));
  return false;
}

bool send_GetEepromCrc(Commander &Cmdr)
{
  int Dst;
  if(Cmdr.getInt(Dst))
  {
    tLightControlOutgoingFrames::SendMsgEepromCrcRequest(Dst);
  }
  else
  {
    Cmdr.println(F("Usage: StateOverview dst_dev_id"));
    return false;
  }

  return true;
}

bool send_SetDefaultTimer(Commander &Cmdr)
{

  int Dst;
  int OutId;
  int DefTimerValue;

  if(!Cmdr.getInt(Dst))
  {
    goto error;
  }
  if (! Cmdr.getInt(OutId))
  {
    goto error;
  }
  if (! Cmdr.getInt(DefTimerValue))
  {
    goto error;
  }

   tLightControlOutgoingFrames::SendMsgSetDefaultTimer(Dst,OutId,DefTimerValue);

  return true;
error:
  Cmdr.println(F("Usage: SetDefTimer dst_dev_id output_id DefaultTimer (0=no timer)"));
  return false;
}

bool send_GetDefaultTimer(Commander &Cmdr)
{

  int Dst;
  int OutId;
  if(!Cmdr.getInt(Dst))
  {
    goto error;
  }
  if (! Cmdr.getInt(OutId))
  {
    goto error;
  }

   tLightControlOutgoingFrames::SendMsgDefaultTimerRequest(Dst,OutId);

  return true;
error:
  Cmdr.println(F("Usage: GetDefTimer dst_dev_id output_id"));
  return false;
}

bool send_ButtonPress(Commander &Cmdr)
{
  uint8_t ForcedSenderId = 0;
  uint16_t ShortClick = 0;
  uint16_t LongClick = 0;
  uint16_t DoubleClick = 0;

  if(!Cmdr.getInt(ForcedSenderId))
  {
    goto error;
  }
  if(!Cmdr.getInt(ShortClick))
  {
    goto error;
  }

  if(!Cmdr.getInt(LongClick))
  {
    goto final;
  }
  if(!Cmdr.getInt(DoubleClick))
  {
    goto final;
  }

final:
  tLightControlOutgoingFrames::SendMsgButtonPress(DEVICE_ID_BROADCAST,ForcedSenderId, ShortClick, LongClick, DoubleClick);
  return true;

error:
  Cmdr.println(F("Usage: ButtonPress forcedSrcID ShortClick [LongClick DoubleClick] (bitmaps)"));
  return false;
}
#endif //CENTRAL_NODE

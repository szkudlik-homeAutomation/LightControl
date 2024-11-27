/*
 * lightControl_telnet.cpp
 *
 *  Created on: 25 lis 2024
 *      Author: szkud
 */




#include "../../global.h"

#if CONFIG_LIGHT_CONTROL_APP_TELNET_COMMANDS

#include "../lib/Commander/src/Commander.h"
#include "tLightControlOutgoingFrames.h"
#include "../Common_code/TLE8457_serial/TLE8457_serial_lib.h"


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

#endif	// CONFIG_LIGHT_CONTROL_APP_TELNET_COMMANDS


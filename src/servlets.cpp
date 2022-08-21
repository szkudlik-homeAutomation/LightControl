#include "servlets.h"

bool tjavaScriptServlet::ProcessAndResponse()
{
	pOwner->SendFlashString(javascript_js_raw,javascript_js_raw_len);
	return false;
}

bool tOutputSetServlet::ProcessAndResponse()
{
	uint16_t Device;  // device iD
	uint16_t Output;  // output iD
	uint16_t State;  // 1 - on, 0 - off
	uint16_t Timer = DEFAULT_TIMER;
	bool ParametersOK = true;
	ParametersOK &= GetParameter("Dev",&Device);
	ParametersOK &= GetParameter("Out",&Output);
	ParametersOK &= GetParameter("State",&State);
	GetParameter("Timer",&Timer);    // optional

	if (! ParametersOK)
	{
	  SendResponse400();
	  return false;
	}

	// execute
	#ifdef DEBUG_3
		RespHandler.print(F("==>HTTP set output, dev="));
		RespHandler.print(Device,DEC);
		RespHandler.print(F(" Out="));
		RespHandler.print(Output,DEC);
		RespHandler.print(F(" State="));
		RespHandler.print(State,DEC);
		RespHandler.print(F(" Timer="));
		RespHandler.println(Timer,DEC);
	#endif
	Worker.SendMsgSetOutput(Device, Output, State, Timer);
	SendResponse200();

return false;
}

bool tSetTimerServlet::ProcessAndResponse()
{
	uint16_t Device;  // device iD
	uint16_t Output;  // output iD
	uint16_t Timer = 0;  // new timer
	bool ParametersOK = true;
	ParametersOK &= GetParameter("Dev",&Device);
	ParametersOK &= GetParameter("Out",&Output);
	GetParameter("Timer",&Timer);    // optional

	if (! ParametersOK)
	{
	  SendResponse400();
	  return false;
	}

	// execute

	#ifdef DEBUG_3
		RespHandler.print(F("==>HTTP set timer, dev="));
		RespHandler.print(Device,DEC);
		RespHandler.print(F(" Out="));
		RespHandler.print(Output,DEC);
		RespHandler.print(F(" Timer="));
		RespHandler.println(Timer,DEC);
	#endif
	Worker.SendMsgSetDefaultTimer(Device, Output, Timer);
	SendResponse200();

	return false;
}

bool tForceButtonPressServlet::ProcessAndResponse()
{
   uint16_t Device;  // forced device iD sender
   uint16_t ShortClickBitmap = 0;
   uint16_t LongClickBitmap = 0;
   uint16_t DoubleClickBitmap = 0;
   bool ParametersOK = true;

   ParametersOK = GetParameter("Dev",&Device);
   if (! ParametersOK)
   {
     SendResponse400();
     return false;
   }

   // other params optional
   GetParameter("Short",&ShortClickBitmap);
   GetParameter("Long",&LongClickBitmap);
   GetParameter("Double",&DoubleClickBitmap);


   // execute

   #ifdef DEBUG_3
   RespHandler.print(F("HTTP forced buttom press - forced SRC Dev ID:"));
   RespHandler.print(Device,HEX);
   RespHandler.print(F(" short:"));
   RespHandler.print(ShortClickBitmap,BIN);
   RespHandler.print(F(" long:"));
   RespHandler.print(LongClickBitmap,BIN);
   RespHandler.print(F(" dbl:"));
   RespHandler.println(DoubleClickBitmap,BIN);
   #endif

   Worker.SendMsgButtonPress(DEVICE_ID_BROADCAST, Device, ShortClickBitmap, LongClickBitmap, DoubleClickBitmap);

   SendResponse200();

   return false;
}




void tOutputStateServlet::vOutputStateResponseHandler(uint8_t DevID, uint8_t OutputID, uint8_t PowerState, uint16_t  TimerValue, uint16_t DefaultTimer)
  {
    if (DevID != mExpectedDevID) return;
    if (OutputID != mExpectedOutputID) return;
    mPowerState = PowerState;
    mTimerValue = TimerValue;
    mDefaultTimer = DefaultTimer;
  }

void tOutputStateServlet::SendOutputStateRequest(uint8_t DevID, uint8_t OutputID)
{
  mPowerState = 255;
  mStartTimestamp = millis();
  mExpectedDevID = DevID;
  mExpectedOutputID = OutputID;

  Worker.SendMsgOutputStateRequest(DevID,OutputID);
}


bool tOutputStateServlet::ProcessOutputState(const char * caption)
{
  uint8_t Status = CheckStateRequest();
  if (STATUS_WAIT == Status)
     return false; // keep waiting

  pOwner->SendFlashString(PSTR("<r>"));
  pOwner->SendFlashString(caption);
  pOwner->SendFlashString(PSTR(" </r>"));

  if (STATUS_TIMEOUT == Status)
  {
    pOwner->SendFlashString(PSTR("???<br>"));
    return true; // give up
  }

  pOwner->SendFlashString(PSTR("<script>OutputControl(\""));
  pOwner->mEthernetClient.print(mExpectedDevID,DEC);
  pOwner->SendFlashString(PSTR("\",\""));
  pOwner->mEthernetClient.print(mExpectedOutputID,DEC);
  pOwner->SendFlashString(PSTR("\",\""));
  pOwner->mEthernetClient.print(mPowerState,DEC);
  pOwner->SendFlashString(PSTR("\",\""));
  pOwner->mEthernetClient.print(mTimerValue,DEC);
  pOwner->SendFlashString(PSTR("\",\""));
  pOwner->mEthernetClient.print(mDefaultTimer,DEC);
  pOwner->SendFlashString(PSTR("\");</script>"));

  return true; // done
}

uint8_t tOutputStateServlet::CheckStateRequest()
{
  if ((millis() - mStartTimestamp) > REQUEST_TIMEOUT) return STATUS_TIMEOUT;
  if (mPowerState == 255) return STATUS_WAIT;
  return STATUS_COMPLETE;
}

bool tOutputStateServlet::ProcessAndResponse()
{
   uint16_t Device;  // device iD
   uint16_t Output;  // output iD
   if (false == mRequestSent)
   {
      bool ParametersOK = true;
      ParametersOK &= GetParameter("Dev",&Device);
      ParametersOK &= GetParameter("Out",&Output);

      if (! ParametersOK)
      {
        SendResponse400();
        return false;
      }

      #ifdef DEBUG_3
         RespHandler.print(F("==>HTTP output state get, Dev="));
         RespHandler.print(Device,DEC);
         RespHandler.print(F(" Out="));
         RespHandler.println(Output,DEC);
      #endif
      SendOutputStateRequest(Device,Output);
      mRequestSent = true;
      return true;   // wait for result
   }

   uint8_t Status = CheckStateRequest();
   switch (Status)
   {
      case STATUS_WAIT:
         return true; // keep waiting

      case STATUS_COMPLETE:
         break;

      case STATUS_TIMEOUT:
      default:
         SendResponse400();
         return false;
   }

   // format JSON response
   pOwner->SendFlashString(PSTR("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n{\"Device\": "));
   pOwner->mEthernetClient.print(mExpectedDevID,DEC);
   pOwner->SendFlashString(PSTR(", \"Output\": "));
   pOwner->mEthernetClient.print(mExpectedOutputID,DEC);
   pOwner->SendFlashString(PSTR(", \"State\": "));
   pOwner->mEthernetClient.print(mPowerState,DEC);
   pOwner->SendFlashString(PSTR(", \"Timer\": "));
   pOwner->mEthernetClient.print(mTimerValue,DEC);
   pOwner->SendFlashString(PSTR(", \"DefaultTimer\": "));
   pOwner->mEthernetClient.print(mDefaultTimer,DEC);
   pOwner->SendFlashString(PSTR("}\r\n"));

   return false;
}

#include "../global.h"

#include "servlets.h"
#if CONFIG_CENTRAL_NODE

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
	DEBUG_PRINT_3("==>HTTP set output, dev=");
	DEBUG_3(print(Device,DEC));
	DEBUG_PRINT_3(" Out=");
	DEBUG_3(print(Output,DEC));
	DEBUG_PRINT_3(" State=");
	DEBUG_3(print(State,DEC));
	DEBUG_PRINT_3(" Timer=");
	DEBUG_3(println(Timer,DEC));
	tLightControlOutgoingMessages::SendMsgSetOutput(Device, Output, State, Timer);
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

	DEBUG_PRINT_3("==>HTTP set timer, dev=");
	DEBUG_3(print(Device,DEC));
	DEBUG_PRINT_3(" Out=");
	DEBUG_3(print(Output,DEC));
	DEBUG_PRINT_3(" Timer=");
	DEBUG_3(println(Timer,DEC));
	tLightControlOutgoingMessages::SendMsgSetDefaultTimer(Device, Output, Timer);
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

   DEBUG_PRINT_3("HTTP forced buttom press - forced SRC Dev ID:");
   DEBUG_3(print(Device,HEX));
   DEBUG_PRINT_3(" short:");
   DEBUG_3(print(ShortClickBitmap,BIN));
   DEBUG_PRINT_3(" long:");
   DEBUG_3(print(LongClickBitmap,BIN));
   DEBUG_PRINT_3(" dbl:");
   DEBUG_3(println(DoubleClickBitmap,BIN));

   tLightControlOutgoingMessages::SendMsgButtonPress(DEVICE_ID_BROADCAST, Device, ShortClickBitmap, LongClickBitmap, DoubleClickBitmap);

   SendResponse200();

   return false;
}

void tOutputStateServlet::onMessage(uint8_t type, uint16_t data, void *pData)
{
	if (data != LightControlMessages::frameRecieved_OutputStateResponse)
		return;

	struct LightControlMessages::tOutputStateResponse *OutputStateResponse = (struct LightControlMessages::tOutputStateResponse *)pData;

    if (OutputStateResponse->SenderID != mExpectedDevID) return;
    if (OutputStateResponse->OutputID != mExpectedOutputID) return;
    mPowerState = OutputStateResponse->PowerState;
    mTimerValue = OutputStateResponse->TimerValue;
    mDefaultTimer = OutputStateResponse->DefaultTimer;
}

void tOutputStateServlet::SendOutputStateRequest(uint8_t DevID, uint8_t OutputID)
{
  mPowerState = 255;
  mStartTimestamp = millis();
  mExpectedDevID = DevID;
  mExpectedOutputID = OutputID;

  tLightControlOutgoingMessages::SendMsgOutputStateRequest(DevID,OutputID);
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

      DEBUG_PRINT_3("==>HTTP output state get, Dev=");
      DEBUG_3(print(Device,DEC));
      DEBUG_PRINT_3(" Out=");
      DEBUG_3(println(Output,DEC));
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
#endif

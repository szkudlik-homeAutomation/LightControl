#include "../../global.h"


#if CONFIG_LIGHT_CONTROL_APP

#include "tLightControlServlets.h"
#include "tLightControlOutgoingFrames.h"
#include "../Common_code/TLE8457_serial/TLE8457_serial_lib.h"

bool tLightControl_SetTimerServlet::ProcessAndResponse()
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

	DEBUG_PRINT_2("==>HTTP set timer, dev=");
	DEBUG_2(print(Device,DEC));
	DEBUG_PRINT_2(" Out=");
	DEBUG_2(print(Output,DEC));
	DEBUG_PRINT_2(" Timer=");
	DEBUG_2(println(Timer,DEC));


	tLightControlOutgoingFrames::SendMsgSetDefaultTimer(Device, Output, Timer);

	SendResponse200();
	return false;
}

bool tLightControl_ForceButtonPressServlet::ProcessAndResponse()
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

   DEBUG_PRINT_2("HTTP forced buttom press - forced SRC Dev ID:");
   DEBUG_2(print(Device,HEX));
   DEBUG_PRINT_2(" short:");
   DEBUG_2(print(ShortClickBitmap,BIN));
   DEBUG_PRINT_2(" long:");
   DEBUG_2(print(LongClickBitmap,BIN));
   DEBUG_PRINT_2(" dbl:");
   DEBUG_3(println(DoubleClickBitmap,BIN));

   tLightControlOutgoingFrames::SendMsgButtonPress(DEVICE_ID_BROADCAST, Device, ShortClickBitmap, LongClickBitmap, DoubleClickBitmap);

   SendResponse200();

   return false;
}

#endif // CONFIG_LIGHT_CONTROL_APP


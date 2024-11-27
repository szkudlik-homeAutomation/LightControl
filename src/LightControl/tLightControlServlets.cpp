#include "../../global.h"


#if CONFIG_LIGHT_CONTROL_APP_HTTP_SERVLETS

#include "tLightControlServlets.h"
#include "tLightControlOutgoingFrames.h"
#include "lightControl_http_binaries.h"
#include "../Common_code/TLE8457_serial/TLE8457_serial_lib.h"

void tLightControl_servlets::SendSetupFooter()
{
	uint16_t SetupParam=0;
	GetParameter("setup",&SetupParam);
	if (! SetupParam)
	{
		pOwner->SendFlashString(PSTR("<br><small><a href=\"?setup=1\"><i>setup</i></a></small><br>"));
	}
	else
	{
		pOwner->SendFlashString(PSTR("<small><br><a href=\"?setup=0\"><i>hide setup</i></a><br>"));
	}
		pOwner->SendFlashString(PSTR("<br><a href=\"index\">strona główna</a><br>"));
}

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

bool tLightControl_javaScriptServlet::ProcessAndResponse()
{
	pOwner->SendFlashString(OutputControl_js_raw,OutputControl_js_raw_len);
	return false;
}

bool tLightControl_GardenLightsServlet::ProcessAndResponse()
{
	pOwner->SendFlashString(gardenLightsPageHeader_http_raw,gardenLightsPageHeader_http_raw_len);
	SendSetupFooter();
	SendVersionAndPageClose();
	return false;
}

bool tLightControl_IndoorLightsServlet::ProcessAndResponse()
{
	pOwner->SendFlashString(indoorLightsPageHeader_http_raw,indoorLightsPageHeader_http_raw_len);
	SendSetupFooter();
	SendVersionAndPageClose();
	return false;
}

bool tLightControl_DefaultPageServlet::ProcessAndResponse()
{
	pOwner->SendFlashString(defaultPage_http_raw,defaultPage_http_raw_len);
	SendVersionAndPageClose();
	return false;
}


#endif // CONFIG_LIGHT_CONTROL_APP_HTTP_SERVLETS


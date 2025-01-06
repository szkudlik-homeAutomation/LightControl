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

bool tLightControl_getActions_XML::ProcessAndResponse()
 {
	tMessageTypeSetAction action1 = {1,2,3,4,5,6,7,8};
	tMessageTypeSetAction action2 = {10,20,30,40,50,60,70,80};
	uint8_t num_of_devices = 2;
	uint8_t num_of_actions = 2;

	pOwner->SendFlashString(PSTR("HTTP/1.1 200 OK\r\nContent-Type: application/json\r\n\r\n{"));

	pOwner->SendFlashString(PSTR("\"Devices\":{"));
	for (uint8_t j = 0; j < num_of_devices; j++) {
		if (j > 0) {
			pOwner->mEthernetClient.print(",");
		}

		pOwner->mEthernetClient.print("\"");
		pOwner->mEthernetClient.print(j, DEC);
		pOwner->mEthernetClient.print("\":");

		pOwner->SendFlashString(PSTR("{\"DevID\":"));
		pOwner->mEthernetClient.print(j, DEC);
		pOwner->SendFlashString(PSTR(", \"Actions\":{"));
		for (uint8_t i = 0; i < num_of_actions; i++) {
			if (i > 0) {
				pOwner->SendFlashString(PSTR(","));
			}
			pOwner->SendFlashString(PSTR("\""));
			pOwner->mEthernetClient.print(i, DEC);
			pOwner->SendFlashString(PSTR("\":"));
			if (i == 0) tMessageTypeSetActionToXML(&action1);
			if (i > 0) tMessageTypeSetActionToXML(&action2);
		}

		pOwner->SendFlashString(PSTR("}}"));
	}
	pOwner->SendFlashString(PSTR("}}"));

return false;
}

void tLightControl_getActions_XML::tMessageTypeSetActionToXML(tMessageTypeSetAction *pAction)
{
    pOwner->SendFlashString(PSTR("{"));
    pOwner->SendFlashString(PSTR("\"OutId\": "));
    pOwner->mEthernetClient.print(pAction->OutId, DEC);
    pOwner->SendFlashString(PSTR(", \"SenderDevID\": "));
    pOwner->mEthernetClient.print(pAction->SenderDevID, DEC);
    pOwner->SendFlashString(PSTR(", \"ButtonId\": "));
    pOwner->mEthernetClient.print(pAction->ButtonId, DEC);
    pOwner->SendFlashString(PSTR(", \"TriggerType\": "));
    pOwner->mEthernetClient.print(pAction->TriggerType, DEC);
    pOwner->SendFlashString(PSTR(", \"ActionType\": "));
    pOwner->mEthernetClient.print(pAction->ActionType, DEC);
    pOwner->SendFlashString(PSTR(", \"Timer\": "));
    pOwner->mEthernetClient.print(pAction->Timer, DEC);
    pOwner->SendFlashString(PSTR(", \"OutputsMask\": "));
    pOwner->mEthernetClient.print(pAction->OutputsMask, DEC);
    pOwner->SendFlashString(PSTR(", \"OutputsStates\": "));
    pOwner->mEthernetClient.print(pAction->OutputsStates, DEC);
    pOwner->SendFlashString(PSTR("}"));
}

#endif // CONFIG_LIGHT_CONTROL_APP_HTTP_SERVLETS


#include "../../global.h"


#if CONFIG_LIGHT_CONTROL_APP

#include "tLightControlServlets.h"
#include "tLightControlOutgoingFrames.h"

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


#endif // CONFIG_LIGHT_CONTROL_APP


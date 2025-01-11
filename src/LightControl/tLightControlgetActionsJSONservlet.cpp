/*
 * tLightControlgetActionsJSONservlet.cpp
 *
 *  Created on: 11 sty 2025
 *      Author: szkud
 */
#include "../../global.h"


#if CONFIG_LIGHT_CONTROL_APP_HTTP_SERVLETS

#include "tLightControlgetActionsJSONservlet.h"

bool tLightControl_getActionsJSON_servlet::ProcessAndResponse()
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
			if (i == 0) tMessageTypeSetActionToJSON(&action1);
			if (i > 0) tMessageTypeSetActionToJSON(&action2);
		}

		pOwner->SendFlashString(PSTR("}}"));
	}
	pOwner->SendFlashString(PSTR("}}"));

return false;
}

void tLightControl_getActionsJSON_servlet::tMessageTypeSetActionToJSON(tMessageTypeSetAction *pAction)
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

#endif /* CONFIG_LIGHT_CONTROL_APP_HTTP_SERVLETS */

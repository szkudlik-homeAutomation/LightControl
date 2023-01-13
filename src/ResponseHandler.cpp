#include "../global.h"

#include "ResponseHandler.h"
#include "Common_code/Logger.h"

ResponseHandler* ResponseHandler::pFirst = NULL;

ResponseHandler::~ResponseHandler()
{
  ResponseHandler *i = pFirst;
  ResponseHandler *prev = NULL;
  while(i != NULL)
  {
    if (i == this)
    {
      if (NULL == prev)        // first item?
      {
        pFirst = this->pNext; // next to this becomes first
      }
      else if (NULL == this->pNext) // last item?
      {
        prev->pNext = NULL;   // prev to this becomes last
      }
      else  // middle item
      {
        prev->pNext = this->pNext;
      }
      return; // done
    }
    prev = i;
    i = i->pNext;
  }
}

void ResponseHandler::OverviewStateResponseHandler(uint8_t SenderID, uint8_t PowerState, uint8_t  TimerState)
{
	LOG_PRINT("PowerStateBitmap for device ");
	LOG(print(SenderID,HEX));
	LOG_PRINT("=");
	LOG(print(PowerState,BIN));
	LOG_PRINT(" with timers map=");
	LOG(println(TimerState,BIN));
	for (ResponseHandler * i = pFirst; i != NULL ; i = i->pNext)  i->vOverviewStateResponseHandler(SenderID,PowerState,TimerState);
}

void ResponseHandler::OutputStateResponseHandler(uint8_t SenderID, uint8_t OutputID, uint8_t PowerState, uint16_t  TimerValue, uint16_t DefaultTimer)
{
	LOG_PRINT("PowerState for device ");
	LOG(print(SenderID,HEX));
	LOG_PRINT(" output ID ");
	LOG(print(OutputID,DEC));
	LOG_PRINT("=");
	LOG(print(PowerState,DEC));
	LOG_PRINT(" with timers = ");
	LOG(print(TimerValue,DEC));
    LOG_PRINT(" default timer = ");
    LOG(println(DefaultTimer,DEC));

	for (ResponseHandler * i = pFirst; i != NULL ; i = i->pNext)  i->vOutputStateResponseHandler(SenderID,OutputID,PowerState,TimerValue,DefaultTimer);
}

void ResponseHandler::EepromCRCResponseHandler(uint8_t SenderID, uint8_t NumOfActions, uint16_t EepromCRC)
{
	LOG_PRINT("Eeprom CRC for device ");
	LOG(print(SenderID,HEX));
	LOG_PRINT(" num of actions=");
	LOG(print(NumOfActions,DEC));
	LOG_PRINT(" CRC=");
	LOG(println(EepromCRC,DEC));
	for (ResponseHandler * i = pFirst; i != NULL ; i = i->pNext)  i->vEepromCRCResponseHandler(SenderID,NumOfActions,EepromCRC);
}

void ResponseHandler::VersionResponseHandler(uint8_t SenderID, uint8_t Major, uint8_t Minor, uint8_t Patch)
{
	LOG_PRINT("FW Version for device ");
	LOG(print(SenderID,HEX));
	LOG_PRINT("=");
	LOG(print(Major,DEC));
	LOG_PRINT(".");
	LOG(print(Minor,DEC));
	LOG_PRINT(".");
	LOG(println(Patch,DEC));

	for (ResponseHandler * i = pFirst; i != NULL ; i = i->pNext) i->vVersionResponseHandler(SenderID,Major,Minor,Patch);
}

void ResponseHandler::NodeScanResponse(uint32_t ActiveNodesMap)
{
	LOG_PRINT("Active node map: ");
	LOG(println(ActiveNodesMap,BIN));

	for (ResponseHandler * i = pFirst; i != NULL ; i = i->pNext) i->vNodeScanResponse(ActiveNodesMap);
}

void ResponseHandler::DefaultTimerResponseHandler(uint8_t SenderID,uint8_t OutputID,uint16_t DefTimerValue)
{
	LOG_PRINT("Default timer for device ");
	LOG(print(SenderID,HEX));
	LOG_PRINT(" outId ");
	LOG(print(OutputID,DEC));
	LOG_PRINT("=");
	LOG(println(DefTimerValue,DEC));

	for (ResponseHandler * i = pFirst; i != NULL ; i = i->pNext) i->vDefaultTimerResponseHandler(SenderID,OutputID,DefTimerValue);
}

ResponseHandler RespHandler;
